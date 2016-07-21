/***************************************************************************
  qgspostgresconn.cpp  -  connection class to PostgreSQL/PostGIS
                             -------------------
    begin                : 2011/01/28
    copyright            : (C) 2011 by Juergen E. Fischer
    email                : jef at norbit dot de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgspostgresconn.h"
#include "qgsauthmanager.h"
#include "qgslogger.h"
#include "qgsdatasourceuri.h"
#include "qgsmessagelog.h"
#include "qgscredentials.h"
#include "qgsfield.h"
#include "qgspgtablemodel.h"
#include "qgsproviderregistry.h"
#include "qgsvectordataprovider.h"
#include "qgswkbtypes.h"

#include <QApplication>
#include <QSettings>
#include <QThread>

#include <climits>

// for htonl
#ifdef Q_OS_WIN
#include <winsock.h>
#else
#include <netinet/in.h>
#endif


QgsPostgresResult::~QgsPostgresResult()
{
  if ( mRes )
    ::PQclear( mRes );
  mRes = nullptr;
}

QgsPostgresResult &QgsPostgresResult::operator=( PGresult * theRes )
{
  if ( mRes )
    ::PQclear( mRes );
  mRes = theRes;
  return *this;
}

QgsPostgresResult &QgsPostgresResult::operator=( const QgsPostgresResult & src )
{
  if ( mRes )
    ::PQclear( mRes );
  mRes = src.result();
  return *this;
}

ExecStatusType QgsPostgresResult::PQresultStatus()
{
  return mRes ? ::PQresultStatus( mRes ) : PGRES_FATAL_ERROR;
}

QString QgsPostgresResult::PQresultErrorMessage()
{
  return mRes ? QString::fromUtf8( ::PQresultErrorMessage( mRes ) ) : QObject::tr( "no result buffer" );
}

int QgsPostgresResult::PQntuples()
{
  Q_ASSERT( mRes );
  return ::PQntuples( mRes );
}

QString QgsPostgresResult::PQgetvalue( int row, int col )
{
  Q_ASSERT( mRes );
  return PQgetisnull( row, col )
         ? QString::null
         : QString::fromUtf8( ::PQgetvalue( mRes, row, col ) );
}

bool QgsPostgresResult::PQgetisnull( int row, int col )
{
  Q_ASSERT( mRes );
  return ::PQgetisnull( mRes, row, col );
}

int QgsPostgresResult::PQnfields()
{
  Q_ASSERT( mRes );
  return ::PQnfields( mRes );
}

QString QgsPostgresResult::PQfname( int col )
{
  Q_ASSERT( mRes );
  return QString::fromUtf8( ::PQfname( mRes, col ) );
}

int QgsPostgresResult::PQftable( int col )
{
  Q_ASSERT( mRes );
  return ::PQftable( mRes, col );
}

int QgsPostgresResult::PQftablecol( int col )
{
  Q_ASSERT( mRes );
  return ::PQftablecol( mRes, col );
}

int QgsPostgresResult::PQftype( int col )
{
  Q_ASSERT( mRes );
  return ::PQftype( mRes, col );
}

int QgsPostgresResult::PQfmod( int col )
{
  Q_ASSERT( mRes );
  return ::PQfmod( mRes, col );
}

Oid QgsPostgresResult::PQoidValue()
{
  Q_ASSERT( mRes );
  return ::PQoidValue( mRes );
}


QMap<QString, QgsPostgresConn *> QgsPostgresConn::sConnectionsRO;
QMap<QString, QgsPostgresConn *> QgsPostgresConn::sConnectionsRW;
const int QgsPostgresConn::sGeomTypeSelectLimit = 100;

QgsPostgresConn *QgsPostgresConn::connectDb( QString conninfo, bool readonly, bool shared, bool transaction )
{
  QMap<QString, QgsPostgresConn *> &connections =
    readonly ? QgsPostgresConn::sConnectionsRO : QgsPostgresConn::sConnectionsRW;

  if ( shared )
  {
    // sharing connection between threads is not safe
    // See http://hub.qgis.org/issues/13141
    Q_ASSERT( QApplication::instance()->thread() == QThread::currentThread() );

    if ( connections.contains( conninfo ) )
    {
      QgsDebugMsg( QString( "Using cached connection for %1" ).arg( conninfo ) );
      connections[conninfo]->mRef++;
      return connections[conninfo];
    }
  }

  QgsPostgresConn *conn = new QgsPostgresConn( conninfo, readonly, shared, transaction );

  if ( conn->mRef == 0 )
  {
    delete conn;
    return nullptr;
  }

  if ( shared )
  {
    connections.insert( conninfo, conn );
  }

  return conn;
}

static void noticeProcessor( void *arg, const char *message )
{
  Q_UNUSED( arg );
  QString msg( QString::fromUtf8( message ) );
  msg.chop( 1 );
  QgsMessageLog::logMessage( QObject::tr( "NOTICE: %1" ).arg( msg ), QObject::tr( "PostGIS" ) );
}

QgsPostgresConn::QgsPostgresConn( const QString& conninfo, bool readOnly, bool shared, bool transaction )
    : mRef( 1 )
    , mOpenCursors( 0 )
    , mConnInfo( conninfo )
    , mGeosAvailable( false )
    , mTopologyAvailable( false )
    , mGotPostgisVersion( false )
    , mPostgresqlVersion( 0 )
    , mPostgisVersionMajor( 0 )
    , mPostgisVersionMinor( 0 )
    , mGistAvailable( false )
    , mProjAvailable( false )
    , mPointcloudAvailable( false )
    , mUseWkbHex( false )
    , mReadOnly( readOnly )
    , mSwapEndian( false )
    , mNextCursorId( 0 )
    , mShared( shared )
    , mTransaction( transaction )
{
  QgsDebugMsg( QString( "New PostgreSQL connection for " ) + conninfo );

  // expand connectionInfo
  QgsDataSourceURI uri( conninfo );
  QString expandedConnectionInfo = uri.connectionInfo( true );

  mConn = PQconnectdb( expandedConnectionInfo.toLocal8Bit() );  // use what is set based on locale; after connecting, use Utf8

  // remove temporary cert/key/CA if they exist
  QgsDataSourceURI expandedUri( expandedConnectionInfo );
  QStringList parameters;
  parameters << "sslcert" << "sslkey" << "sslrootcert";
  Q_FOREACH ( const QString& param, parameters )
  {
    if ( expandedUri.hasParam( param ) )
    {
      QString fileName = expandedUri.param( param );
      fileName.remove( "'" );
      QFile::remove( fileName );
    }
  }

  // check the connection status
  if ( PQstatus() != CONNECTION_OK )
  {
    QString username = uri.username();
    QString password = uri.password();

    QgsCredentials::instance()->lock();

    int i = 0;
    while ( PQstatus() != CONNECTION_OK && i < 5 )
    {
      ++i;
      bool ok = QgsCredentials::instance()->get( conninfo, username, password, PQerrorMessage() );
      if ( !ok )
        break;

      PQfinish();

      if ( !username.isEmpty() )
        uri.setUsername( username );

      if ( !password.isEmpty() )
        uri.setPassword( password );

      QgsDebugMsg( "Connecting to " + uri.connectionInfo( false ) );
      mConn = PQconnectdb( uri.connectionInfo().toLocal8Bit() );
    }

    if ( PQstatus() == CONNECTION_OK )
      QgsCredentials::instance()->put( conninfo, username, password );

    QgsCredentials::instance()->unlock();
  }

  if ( PQstatus() != CONNECTION_OK )
  {
    QString errorMsg = PQerrorMessage();
    PQfinish();
    QgsMessageLog::logMessage( tr( "Connection to database failed" ) + '\n' + errorMsg, tr( "PostGIS" ) );
    mRef = 0;
    return;
  }

  //set client encoding to unicode because QString uses UTF-8 anyway
  QgsDebugMsg( "setting client encoding to UNICODE" );
  int errcode = PQsetClientEncoding( mConn, QString( "UNICODE" ).toLocal8Bit() );
  if ( errcode == 0 )
  {
    QgsDebugMsg( "encoding successfully set" );
  }
  else if ( errcode == -1 )
  {
    QgsMessageLog::logMessage( tr( "error in setting encoding" ), tr( "PostGIS" ) );
  }
  else
  {
    QgsMessageLog::logMessage( tr( "undefined return value from encoding setting" ), tr( "PostGIS" ) );
  }

  QgsDebugMsg( "Connection to the database was successful" );

  deduceEndian();

  /* Check to see if we have working PostGIS support */
  if ( !postgisVersion().isNull() )
  {
    /* Check to see if we have GEOS support and if not, warn the user about
       the problems they will see :) */
    QgsDebugMsg( "Checking for GEOS support" );

    if ( !hasGEOS() )
    {
      QgsMessageLog::logMessage( tr( "Your PostGIS installation has no GEOS support. Feature selection and identification will not work properly. Please install PostGIS with GEOS support (http://geos.refractions.net)" ), tr( "PostGIS" ) );
    }

    if ( hasTopology() )
    {
      QgsDebugMsg( "Topology support available!" );
    }
  }

  if ( mPostgresqlVersion >= 90000 )
  {
    PQexecNR( "SET application_name='QGIS'" );
  }


  PQsetNoticeProcessor( mConn, noticeProcessor, nullptr );
}

QgsPostgresConn::~QgsPostgresConn()
{
  Q_ASSERT( mRef == 0 );
  if ( mConn )
    ::PQfinish( mConn );
  mConn = nullptr;
}

void QgsPostgresConn::unref()
{
  if ( --mRef > 0 )
    return;

  if ( mShared )
  {
    QMap<QString, QgsPostgresConn *>& connections = mReadOnly ? sConnectionsRO : sConnectionsRW;

    QString key = connections.key( this, QString::null );

    Q_ASSERT( !key.isNull() );
    connections.remove( key );
  }

  delete this;
}

/* private */
void QgsPostgresConn::addColumnInfo( QgsPostgresLayerProperty& layerProperty, const QString& schemaName, const QString& viewName, bool fetchPkCandidates )
{
  // TODO: optimize this query when pk candidates aren't needed
  //       could use array_agg() and count()
  //       array output would look like this: "{One,tWo}"
  QString sql = QString( "SELECT attname, CASE WHEN typname = ANY(ARRAY['geometry','geography','topogeometry']) THEN 1 ELSE null END AS isSpatial FROM pg_attribute JOIN pg_type ON atttypid=pg_type.oid WHERE attrelid=regclass('%1.%2') AND attnum>0 ORDER BY attnum" )
                .arg( quotedIdentifier( schemaName ),
                      quotedIdentifier( viewName ) );
  //QgsDebugMsg( sql );
  QgsPostgresResult colRes( PQexec( sql ) );

  layerProperty.pkCols.clear();
  layerProperty.nSpCols = 0;

  if ( colRes.PQresultStatus() == PGRES_TUPLES_OK )
  {
    for ( int i = 0; i < colRes.PQntuples(); i++ )
    {
      if ( fetchPkCandidates )
      {
        //QgsDebugMsg( colRes.PQgetvalue( i, 0 ) );
        layerProperty.pkCols << colRes.PQgetvalue( i, 0 );
      }

      if ( colRes.PQgetisnull( i, 1 ) == 0 )
      {
        ++layerProperty.nSpCols;
      }
    }
  }
  else
  {
    QgsMessageLog::logMessage( tr( "SQL:%1\nresult:%2\nerror:%3\n" ).arg( sql ).arg( colRes.PQresultStatus() ).arg( colRes.PQresultErrorMessage() ), tr( "PostGIS" ) );
  }

}

bool QgsPostgresConn::getTableInfo( bool searchGeometryColumnsOnly, bool searchPublicOnly, bool allowGeometrylessTables, const QString& schema )
{
  int nColumns = 0;
  int foundInTables = 0;
  QgsPostgresResult result;
  QgsPostgresLayerProperty layerProperty;

  //QgsDebugMsg( "Entering." );

  mLayersSupported.clear();

  for ( int i = sctGeometry; i <= sctPcPatch; ++i )
  {
    QString sql, tableName, schemaName, columnName, typeName, sridName, gtableName, dimName;
    QgsPostgresGeometryColumnType columnType = sctGeometry;

    if ( i == sctGeometry )
    {
      tableName  = "l.f_table_name";
      schemaName = "l.f_table_schema";
      columnName = "l.f_geometry_column";
      typeName   = "upper(l.type)";
      sridName   = "l.srid";
      dimName    = "l.coord_dimension";
      gtableName = "geometry_columns";
      columnType = sctGeometry;
    }
    else if ( i == sctGeography )
    {
      tableName  = "l.f_table_name";
      schemaName = "l.f_table_schema";
      columnName = "l.f_geography_column";
      typeName   = "upper(l.type)";
      sridName   = "l.srid";
      dimName    = "2";
      gtableName = "geography_columns";
      columnType = sctGeography;
    }
    else if ( i == sctTopoGeometry )
    {
      if ( !hasTopology() )
        continue;

      schemaName = "l.schema_name";
      tableName  = "l.table_name";
      columnName = "l.feature_column";
      typeName   = "CASE "
                   "WHEN l.feature_type = 1 THEN 'MULTIPOINT' "
                   "WHEN l.feature_type = 2 THEN 'MULTILINESTRING' "
                   "WHEN l.feature_type = 3 THEN 'MULTIPOLYGON' "
                   "WHEN l.feature_type = 4 THEN 'GEOMETRYCOLLECTION' "
                   "END AS type";
      sridName   = "(SELECT srid FROM topology.topology t WHERE l.topology_id=t.id)";
      dimName    = "2";
      gtableName = "topology.layer";
      columnType = sctTopoGeometry;
    }
    else if ( i == sctPcPatch )
    {
      if ( !hasPointcloud() )
        continue;

      tableName  = "l.\"table\"";
      schemaName = "l.\"schema\"";
      columnName = "l.\"column\"";
      typeName   = "'POLYGON'";
      sridName   = "l.srid";
      dimName    = "2";
      gtableName = "pointcloud_columns";
      columnType = sctPcPatch;
    }
    else
    {
      QgsMessageLog::logMessage( tr( "Unsupported spatial column type %1" )
                                 .arg( displayStringForGeomType(( QgsPostgresGeometryColumnType )i ) ) );
      continue;
    }

    // The following query returns only tables that exist and the user has SELECT privilege on.
    // Can't use regclass here because table must exist, else error occurs.
    sql = QString( "SELECT %1,%2,%3,%4,%5,%6,c.relkind,obj_description(c.oid)"
                   " FROM %7 l,pg_class c,pg_namespace n"
                   " WHERE c.relname=%1"
                   " AND %2=n.nspname"
                   " AND n.oid=c.relnamespace"
                   " AND has_schema_privilege(n.nspname,'usage')"
                   " AND has_table_privilege('\"'||n.nspname||'\".\"'||c.relname||'\"','select')" // user has select privilege
                 )
          .arg( tableName, schemaName, columnName, typeName, sridName, dimName, gtableName );

    if ( searchPublicOnly )
      sql += " AND n.nspname='public'";

    if ( !schema.isEmpty() )
      sql += QString( " AND %1='%2'" ).arg( schemaName, schema );

    sql += QString( " ORDER BY n.nspname,c.relname,%1" ).arg( columnName );

    //QgsDebugMsg( "getting table info: " + sql );
    result = PQexec( sql, i == 0 );
    if ( result.PQresultStatus() != PGRES_TUPLES_OK )
    {
      PQexecNR( "COMMIT" );
      continue;
    }

    for ( int idx = 0; idx < result.PQntuples(); idx++ )
    {
      QString tableName = result.PQgetvalue( idx, 0 );
      QString schemaName = result.PQgetvalue( idx, 1 );
      QString column = result.PQgetvalue( idx, 2 );
      QString type = result.PQgetvalue( idx, 3 );
      QString ssrid = result.PQgetvalue( idx, 4 );
      int dim = result.PQgetvalue( idx, 5 ).toInt();
      QString relkind = result.PQgetvalue( idx, 6 );
      bool isView = relkind == "v" || relkind == "m";
      QString comment = result.PQgetvalue( idx, 7 );

      int srid = ssrid.isEmpty() ? INT_MIN : ssrid.toInt();
      if ( majorVersion() >= 2 && srid == 0 )
      {
        // 0 doesn't constraint => detect
        srid = INT_MIN;
      }

#if 0
      QgsDebugMsg( QString( "%1 : %2.%3.%4: %5 %6 %7 %8" )
                   .arg( gtableName )
                   .arg( schemaName ).arg( tableName ).arg( column )
                   .arg( type )
                   .arg( srid )
                   .arg( relkind )
                   .arg( dim ) );
#endif

      layerProperty.schemaName = schemaName;
      layerProperty.tableName = tableName;
      layerProperty.geometryColName = column;
      layerProperty.geometryColType = columnType;
      if ( dim == 3 && !type.endsWith( 'M' ) )
        type += "Z";
      else if ( dim == 4 )
        type += "ZM";
      layerProperty.types = QList<QGis::WkbType>() << ( QgsPostgresConn::wkbTypeFromPostgis( type ) );
      layerProperty.srids = QList<int>() << srid;
      layerProperty.sql = "";
      layerProperty.relKind = relkind;
      layerProperty.isView = isView;
      layerProperty.tableComment = comment;
      /*
       * force2d may get a false negative value
       * (dim == 2 but is not really constrained)
       * http://trac.osgeo.org/postgis/ticket/3068
       */
      layerProperty.force2d = dim > 3;
      addColumnInfo( layerProperty, schemaName, tableName, isView );

      if ( isView && layerProperty.pkCols.empty() )
      {
        //QgsDebugMsg( "no key columns found." );
        continue;
      }

      mLayersSupported << layerProperty;
      nColumns++;
    }

    foundInTables |= 1 << i;
  }

  //search for geometry columns in tables that are not in the geometry_columns metatable
  if ( !searchGeometryColumnsOnly )
  {
    // Now have a look for geometry columns that aren't in the geometry_columns table.
    QString sql = "SELECT"
                  " c.relname"
                  ",n.nspname"
                  ",a.attname"
                  ",c.relkind"
                  ",CASE WHEN t.typname IN ('geometry','geography','topogeometry') THEN t.typname ELSE b.typname END AS coltype"
                  ",obj_description(c.oid)"
                  " FROM pg_attribute a"
                  " JOIN pg_class c ON c.oid=a.attrelid"
                  " JOIN pg_namespace n ON n.oid=c.relnamespace"
                  " JOIN pg_type t ON t.oid=a.atttypid"
                  " LEFT JOIN pg_type b ON b.oid=t.typbasetype"
                  " WHERE c.relkind IN ('v','r','m')"
                  " AND has_schema_privilege( n.nspname, 'usage' )"
                  " AND has_table_privilege( '\"' || n.nspname || '\".\"' || c.relname || '\"', 'select' )"
                  " AND (t.typname IN ('geometry','geography','topogeometry') OR b.typname IN ('geometry','geography','topogeometry','pcpatch'))";

    // user has select privilege
    if ( searchPublicOnly )
      sql += " AND n.nspname='public'";

    if ( !schema.isEmpty() )
      sql += QString( " AND n.nspname='%2'" ).arg( schema );

    // skip columns of which we already derived information from the metadata tables
    if ( nColumns > 0 )
    {
      if ( foundInTables & ( 1 << sctGeometry ) )
      {
        sql += " AND (n.nspname,c.relname,a.attname) NOT IN (SELECT f_table_schema,f_table_name,f_geometry_column FROM geometry_columns)";
      }

      if ( foundInTables & ( 1 << sctGeography ) )
      {
        sql += " AND (n.nspname,c.relname,a.attname) NOT IN (SELECT f_table_schema,f_table_name,f_geography_column FROM geography_columns)";
      }

      if ( foundInTables & ( 1 << sctPcPatch ) )
      {
        sql += " AND (n.nspname,c.relname,a.attname) NOT IN (SELECT \"schema\",\"table\",\"column\" FROM pointcloud_columns)";
      }
    }

    //QgsDebugMsg( "sql: " + sql );

    result = PQexec( sql );

    if ( result.PQresultStatus() != PGRES_TUPLES_OK )
    {
      QgsMessageLog::logMessage( tr( "Database connection was successful, but the accessible tables could not be determined. The error message from the database was:\n%1\n" )
                                 .arg( result.PQresultErrorMessage() ),
                                 tr( "PostGIS" ) );
      PQexecNR( "COMMIT" );
      return false;
    }

    for ( int i = 0; i < result.PQntuples(); i++ )
    {
      // Have the column name, schema name and the table name. The concept of a
      // catalog doesn't exist in postgresql so we ignore that, but we
      // do need to get the geometry type.

      QString tableName  = result.PQgetvalue( i, 0 ); // relname
      QString schemaName = result.PQgetvalue( i, 1 ); // nspname
      QString column     = result.PQgetvalue( i, 2 ); // attname
      QString relkind    = result.PQgetvalue( i, 3 ); // relation kind
      QString coltype    = result.PQgetvalue( i, 4 ); // column type
      bool isView = relkind == "v" || relkind == "m";
      QString comment    = result.PQgetvalue( i, 5 ); // table comment

      //QgsDebugMsg( QString( "%1.%2.%3: %4" ).arg( schemaName ).arg( tableName ).arg( column ).arg( relkind ) );

      layerProperty.types = QList<QGis::WkbType>() << QGis::WKBUnknown;
      layerProperty.srids = QList<int>() << INT_MIN;
      layerProperty.schemaName = schemaName;
      layerProperty.tableName = tableName;
      layerProperty.geometryColName = column;
      layerProperty.relKind = relkind;
      layerProperty.isView = isView;
      layerProperty.tableComment = comment;
      if ( coltype == "geometry" )
      {
        layerProperty.geometryColType = sctGeometry;
      }
      else if ( coltype == "geography" )
      {
        layerProperty.geometryColType = sctGeography;
      }
      else if ( coltype == "topogeometry" )
      {
        layerProperty.geometryColType = sctTopoGeometry;
      }
      else if ( coltype == "pcpatch" )
      {
        layerProperty.geometryColType = sctPcPatch;
      }
      else
      {
        Q_ASSERT( !"Unknown geometry type" );
      }

      addColumnInfo( layerProperty, schemaName, tableName, isView );
      if ( isView && layerProperty.pkCols.empty() )
      {
        //QgsDebugMsg( "no key columns found." );
        continue;
      }

      layerProperty.sql = "";

      mLayersSupported << layerProperty;
      nColumns++;
    }
  }

  if ( allowGeometrylessTables )
  {
    QString sql = "SELECT "
                  "pg_class.relname"
                  ",pg_namespace.nspname"
                  ",pg_class.relkind"
                  ",obj_description(pg_class.oid)"
                  " FROM "
                  " pg_class"
                  ",pg_namespace"
                  " WHERE pg_namespace.oid=pg_class.relnamespace"
                  " AND has_schema_privilege(pg_namespace.nspname,'usage')"
                  " AND has_table_privilege('\"' || pg_namespace.nspname || '\".\"' || pg_class.relname || '\"','select')"
                  " AND pg_class.relkind IN ('v','r','m')";

    // user has select privilege
    if ( searchPublicOnly )
      sql += " AND pg_namespace.nspname='public'";

    if ( !schema.isEmpty() )
      sql += QString( " AND pg_namespace.nspname='%2'" ).arg( schema );

    //QgsDebugMsg( "sql: " + sql );

    result = PQexec( sql );

    if ( result.PQresultStatus() != PGRES_TUPLES_OK )
    {
      QgsMessageLog::logMessage( tr( "Database connection was successful, but the accessible tables could not be determined.\nThe error message from the database was:\n%1" )
                                 .arg( result.PQresultErrorMessage() ),
                                 tr( "PostGIS" ) );
      return false;
    }

    for ( int i = 0; i < result.PQntuples(); i++ )
    {
      QString table   = result.PQgetvalue( i, 0 ); // relname
      QString schema  = result.PQgetvalue( i, 1 ); // nspname
      QString relkind = result.PQgetvalue( i, 2 ); // relation kind
      bool isView = relkind == "v" || relkind == "m";
      QString comment = result.PQgetvalue( i, 3 ); // table comment

      //QgsDebugMsg( QString( "%1.%2: %3" ).arg( schema ).arg( table ).arg( relkind ) );

      layerProperty.types = QList<QGis::WkbType>() << QGis::WKBNoGeometry;
      layerProperty.srids = QList<int>() << INT_MIN;
      layerProperty.schemaName = schema;
      layerProperty.tableName = table;
      layerProperty.geometryColName = QString::null;
      layerProperty.geometryColType = sctNone;
      layerProperty.relKind = relkind;
      layerProperty.isView = isView;
      layerProperty.tableComment = comment;

      //check if we've already added this layer in some form
      bool alreadyFound = false;
      Q_FOREACH ( const QgsPostgresLayerProperty& foundLayer, mLayersSupported )
      {
        if ( foundLayer.schemaName == schema && foundLayer.tableName == table )
        {
          //already found this table
          alreadyFound = true;
          break;
        }
      }
      if ( alreadyFound )
        continue;

      addColumnInfo( layerProperty, schema, table, isView );
      layerProperty.sql = "";

      mLayersSupported << layerProperty;
      nColumns++;
    }
  }

  if ( nColumns == 0 && schema.isEmpty() )
  {
    QgsMessageLog::logMessage( tr( "Database connection was successful, but the accessible tables could not be determined." ), tr( "PostGIS" ) );
  }

  return true;
}

bool QgsPostgresConn::supportedLayers( QVector<QgsPostgresLayerProperty> &layers, bool searchGeometryColumnsOnly, bool searchPublicOnly, bool allowGeometrylessTables, QString schema )
{
  // Get the list of supported tables
  if ( !getTableInfo( searchGeometryColumnsOnly, searchPublicOnly, allowGeometrylessTables, schema ) )
  {
    QgsMessageLog::logMessage( tr( "Unable to get list of spatially enabled tables from the database" ), tr( "PostGIS" ) );
    return false;
  }

  layers = mLayersSupported;

  //QgsDebugMsg( "Exiting." );

  return true;
}

bool QgsPostgresConn::getSchemas( QList<QgsPostgresSchemaProperty> &schemas )
{
  schemas.clear();
  QgsPostgresResult result;

  QString sql = QString( "SELECT nspname, pg_get_userbyid(nspowner), pg_catalog.obj_description(oid) FROM pg_namespace WHERE nspname !~ '^pg_' AND nspname != 'information_schema' ORDER BY nspname" );

  result = PQexec( sql, true );
  if ( result.PQresultStatus() != PGRES_TUPLES_OK )
  {
    PQexecNR( "COMMIT" );
    return false;
  }

  for ( int idx = 0; idx < result.PQntuples(); idx++ )
  {
    QgsPostgresSchemaProperty schema;
    schema.name = result.PQgetvalue( idx, 0 );
    schema.owner = result.PQgetvalue( idx, 1 );
    schema.description = result.PQgetvalue( idx, 2 );
    schemas << schema;
  }
  return true;
}

/**
 * Check to see if GEOS is available
 */
bool QgsPostgresConn::hasGEOS()
{
  // make sure info is up to date for the current connection
  postgisVersion();
  return mGeosAvailable;
}

/**
 * Check to see if topology is available
 */
bool QgsPostgresConn::hasTopology()
{
  // make sure info is up to date for the current connection
  postgisVersion();
  return mTopologyAvailable;
}

/**
 * Check to see if pointcloud is available
 */
bool QgsPostgresConn::hasPointcloud()
{
  // make sure info is up to date for the current connection
  postgisVersion();
  return mPointcloudAvailable;
}

/* Functions for determining available features in postGIS */
QString QgsPostgresConn::postgisVersion()
{
  if ( mGotPostgisVersion )
    return mPostgisVersionInfo;

  mPostgresqlVersion = PQserverVersion( mConn );

  QgsPostgresResult result( PQexec( "SELECT postgis_version()", false ) );
  if ( result.PQntuples() != 1 )
  {
    QgsMessageLog::logMessage( tr( "No PostGIS support in the database." ), tr( "PostGIS" ) );
    mGotPostgisVersion = true;
    return QString::null;
  }

  mPostgisVersionInfo = result.PQgetvalue( 0, 0 );

  QgsDebugMsg( "PostGIS version info: " + mPostgisVersionInfo );

  QStringList postgisParts = mPostgisVersionInfo.split( ' ', QString::SkipEmptyParts );

  // Get major and minor version
  QStringList postgisVersionParts = postgisParts[0].split( '.', QString::SkipEmptyParts );
  if ( postgisVersionParts.size() < 2 )
  {
    QgsMessageLog::logMessage( tr( "Could not parse postgis version string '%1'" ).arg( mPostgisVersionInfo ), tr( "PostGIS" ) );
    return QString::null;
  }

  mPostgisVersionMajor = postgisVersionParts[0].toInt();
  mPostgisVersionMinor = postgisVersionParts[1].toInt();

  mUseWkbHex = mPostgisVersionMajor < 1;

  // apparently postgis 1.5.2 doesn't report capabilities in postgis_version() anymore
  if ( mPostgisVersionMajor > 1 || ( mPostgisVersionMajor == 1 && mPostgisVersionMinor >= 5 ) )
  {
    result = PQexec( "SELECT postgis_geos_version(),postgis_proj_version()" );
    mGeosAvailable = result.PQntuples() == 1 && !result.PQgetisnull( 0, 0 );
    mProjAvailable = result.PQntuples() == 1 && !result.PQgetisnull( 0, 1 );
    QgsDebugMsg( QString( "geos:%1 proj:%2" )
                 .arg( mGeosAvailable ? result.PQgetvalue( 0, 0 ) : "none",
                       mProjAvailable ? result.PQgetvalue( 0, 1 ) : "none" ) );
    mGistAvailable = true;
  }
  else
  {
    // assume no capabilities
    mGeosAvailable = false;
    mGistAvailable = false;
    mProjAvailable = false;

    // parse out the capabilities and store them
    QStringList geos = postgisParts.filter( "GEOS" );
    if ( geos.size() == 1 )
    {
      mGeosAvailable = ( geos[0].indexOf( "=1" ) > -1 );
    }
    QStringList gist = postgisParts.filter( "STATS" );
    if ( gist.size() == 1 )
    {
      mGistAvailable = ( gist[0].indexOf( "=1" ) > -1 );
    }
    QStringList proj = postgisParts.filter( "PROJ" );
    if ( proj.size() == 1 )
    {
      mProjAvailable = ( proj[0].indexOf( "=1" ) > -1 );
    }
  }

  // checking for topology support
  QgsDebugMsg( "Checking for topology support" );
  mTopologyAvailable = false;
  if ( mPostgisVersionMajor > 1 )
  {
    QgsPostgresResult result( PQexec( "SELECT EXISTS ( SELECT c.oid FROM pg_class AS c JOIN pg_namespace AS n ON c.relnamespace=n.oid WHERE n.nspname='topology' AND c.relname='topology' )" ) );
    if ( result.PQntuples() >= 1 && result.PQgetvalue( 0, 0 ) == "t" )
    {
      mTopologyAvailable = true;
    }
  }

  mGotPostgisVersion = true;

  if ( mPostgresqlVersion >= 90000 )
  {
    QgsDebugMsg( "Checking for pointcloud support" );
    result = PQexec( "SELECT oid FROM pg_catalog.pg_extension WHERE extname = 'pointcloud_postgis'", false );
    if ( result.PQntuples() == 1 )
    {
      mPointcloudAvailable = true;
      QgsDebugMsg( "Pointcloud support available!" );
    }
  }

  return mPostgisVersionInfo;
}

QString QgsPostgresConn::quotedIdentifier( QString ident )
{
  ident.replace( '"', "\"\"" );
  ident = ident.prepend( '\"' ).append( '\"' );
  return ident;
}

QString QgsPostgresConn::quotedValue( const QVariant& value )
{
  if ( value.isNull() )
    return "NULL";

  switch ( value.type() )
  {
    case QVariant::Int:
    case QVariant::LongLong:
    case QVariant::Double:
      return value.toString();

    case QVariant::Bool:
      return value.toBool() ? "TRUE" : "FALSE";

    default:
    case QVariant::String:
      QString v = value.toString();
      v.replace( '\'', "''" );
      if ( v.contains( '\\' ) )
        return v.replace( '\\', "\\\\" ).prepend( "E'" ).append( '\'' );
      else
        return v.prepend( '\'' ).append( '\'' );
  }
}

PGresult *QgsPostgresConn::PQexec( const QString& query, bool logError )
{
  if ( PQstatus() != CONNECTION_OK )
  {
    if ( logError )
    {
      QgsMessageLog::logMessage( tr( "Connection error: %1 returned %2 [%3]" )
                                 .arg( query ).arg( PQstatus() ).arg( PQerrorMessage() ),
                                 tr( "PostGIS" ) );
    }
    else
    {
      QgsDebugMsg( QString( "Connection error: %1 returned %2 [%3]" )
                   .arg( query ).arg( PQstatus() ).arg( PQerrorMessage() ) );
    }

    return nullptr;
  }

  QgsDebugMsgLevel( QString( "Executing SQL: %1" ).arg( query ), 3 );
  PGresult *res = ::PQexec( mConn, query.toUtf8() );

  if ( res )
  {
    int errorStatus = PQresultStatus( res );
    if ( errorStatus != PGRES_COMMAND_OK && errorStatus != PGRES_TUPLES_OK )
    {
      if ( logError )
      {
        QgsMessageLog::logMessage( tr( "Erroneous query: %1 returned %2 [%3]" )
                                   .arg( query ).arg( errorStatus ).arg( PQresultErrorMessage( res ) ),
                                   tr( "PostGIS" ) );
      }
      else
      {
        QgsDebugMsg( QString( "Not logged erroneous query: %1 returned %2 [%3]" )
                     .arg( query ).arg( errorStatus ).arg( PQresultErrorMessage( res ) ) );
      }
    }
  }
  else if ( logError )
  {
    QgsMessageLog::logMessage( tr( "Query failed: %1\nError: no result buffer" ).arg( query ), tr( "PostGIS" ) );
  }
  else
  {
    QgsDebugMsg( QString( "Not logged query failed: %1\nError: no result buffer" ).arg( query ) );
  }

  return res;
}

bool QgsPostgresConn::openCursor( const QString& cursorName, const QString& sql )
{
  if ( mOpenCursors++ == 0 && !mTransaction )
  {
    QgsDebugMsg( QString( "Starting read-only transaction: %1" ).arg( mPostgresqlVersion ) );
    if ( mPostgresqlVersion >= 80000 )
      PQexecNR( "BEGIN READ ONLY" );
    else
      PQexecNR( "BEGIN" );
  }
  QgsDebugMsgLevel( QString( "Binary cursor %1 for %2" ).arg( cursorName, sql ), 3 );
  return PQexecNR( QString( "DECLARE %1 BINARY CURSOR%2 FOR %3" ).
                   arg( cursorName, !mTransaction ? "" : QString( " WITH HOLD" ), sql ) );
}

bool QgsPostgresConn::closeCursor( const QString& cursorName )
{
  if ( !PQexecNR( QString( "CLOSE %1" ).arg( cursorName ) ) )
    return false;

  if ( --mOpenCursors == 0 && !mTransaction )
  {
    QgsDebugMsg( "Committing read-only transaction" );
    PQexecNR( "COMMIT" );
  }

  return true;
}

QString QgsPostgresConn::uniqueCursorName()
{
  return QString( "qgis_%1" ).arg( ++mNextCursorId );
}

bool QgsPostgresConn::PQexecNR( const QString& query, bool retry )
{
  QgsPostgresResult res( PQexec( query, false ) );

  ExecStatusType errorStatus = res.PQresultStatus();
  if ( errorStatus == PGRES_COMMAND_OK )
    return true;

  QgsMessageLog::logMessage( tr( "Query: %1 returned %2 [%3]" )
                             .arg( query )
                             .arg( errorStatus )
                             .arg( res.PQresultErrorMessage() ),
                             tr( "PostGIS" ) );

  if ( mOpenCursors )
  {
    QgsMessageLog::logMessage( tr( "%1 cursor states lost.\nSQL: %2\nResult: %3 (%4)" )
                               .arg( mOpenCursors ).arg( query ).arg( errorStatus )
                               .arg( res.PQresultErrorMessage() ), tr( "PostGIS" ) );
    mOpenCursors = 0;
  }

  if ( PQstatus() == CONNECTION_OK )
  {
    PQexecNR( "ROLLBACK" );
  }
  else if ( retry )
  {
    QgsMessageLog::logMessage( tr( "resetting bad connection." ), tr( "PostGIS" ) );
    ::PQreset( mConn );
    if ( PQstatus() == CONNECTION_OK )
    {
      if ( PQexecNR( query, false ) )
      {
        QgsMessageLog::logMessage( tr( "retry after reset succeeded." ), tr( "PostGIS" ) );
        return true;
      }
      else
      {
        QgsMessageLog::logMessage( tr( "retry after reset failed again." ), tr( "PostGIS" ) );
        return false;
      }
    }
    else
    {
      QgsMessageLog::logMessage( tr( "connection still bad after reset." ), tr( "PostGIS" ) );
    }
  }
  else
  {
    QgsMessageLog::logMessage( tr( "bad connection, not retrying." ), tr( "PostGIS" ) );
  }

  return false;
}

PGresult *QgsPostgresConn::PQgetResult()
{
  return ::PQgetResult( mConn );
}

PGresult *QgsPostgresConn::PQprepare( const QString& stmtName, const QString& query, int nParams, const Oid *paramTypes )
{
  return ::PQprepare( mConn, stmtName.toUtf8(), query.toUtf8(), nParams, paramTypes );
}

PGresult *QgsPostgresConn::PQexecPrepared( const QString& stmtName, const QStringList &params )
{
  const char **param = new const char *[ params.size()];
  QList<QByteArray> qparam;

  qparam.reserve( params.size() );
  for ( int i = 0; i < params.size(); i++ )
  {
    qparam << params[i].toUtf8();

    if ( params[i].isNull() )
      param[i] = nullptr;
    else
      param[i] = qparam[i];
  }

  PGresult *res = ::PQexecPrepared( mConn, stmtName.toUtf8(), params.size(), param, nullptr, nullptr, 0 );

  delete [] param;

  return res;
}

void QgsPostgresConn::PQfinish()
{
  Q_ASSERT( mConn );
  ::PQfinish( mConn );
  mConn = nullptr;
}

int QgsPostgresConn::PQstatus()
{
  Q_ASSERT( mConn );
  return ::PQstatus( mConn );
}

QString QgsPostgresConn::PQerrorMessage()
{
  Q_ASSERT( mConn );
  return QString::fromUtf8( ::PQerrorMessage( mConn ) );
}

int QgsPostgresConn::PQsendQuery( const QString& query )
{
  Q_ASSERT( mConn );
  return ::PQsendQuery( mConn, query.toUtf8() );
}

bool QgsPostgresConn::begin()
{
  if ( mTransaction )
  {
    return PQexecNR( "SAVEPOINT transaction_savepoint" );
  }
  else
  {
    return PQexecNR( "BEGIN" );
  }
}

bool QgsPostgresConn::commit()
{
  if ( mTransaction )
  {
    return PQexecNR( "RELEASE SAVEPOINT transaction_savepoint" );
  }
  else
  {
    return PQexecNR( "COMMIT" );
  }
}

bool QgsPostgresConn::rollback()
{
  if ( mTransaction )
  {
    return PQexecNR( "ROLLBACK TO SAVEPOINT transaction_savepoint" )
           && PQexecNR( "RELEASE SAVEPOINT transaction_savepoint" );
  }
  else
  {
    return PQexecNR( "ROLLBACK" );
  }
}

qint64 QgsPostgresConn::getBinaryInt( QgsPostgresResult &queryResult, int row, int col )
{
  quint64 oid;
  char *p = PQgetvalue( queryResult.result(), row, col );
  size_t s = PQgetlength( queryResult.result(), row, col );

#ifdef QGISDEBUG
  if ( QgsLogger::debugLevel() >= 4 )
  {
    QString buf;
    for ( size_t i = 0; i < s; i++ )
    {
      buf += QString( "%1 " ).arg( *( unsigned char * )( p + i ), 0, 16, QLatin1Char( ' ' ) );
    }

    QgsDebugMsg( QString( "int in hex:%1" ).arg( buf ) );
  }
#endif

  switch ( s )
  {
    case 2:
      oid = *( quint16 * )p;
      if ( mSwapEndian )
        oid = ntohs( oid );
      /* cast to signed 16bit
       * See http://hub.qgis.org/issues/14262 */
      oid = ( qint16 )oid;
      break;

    case 6:
    {
      quint64 block  = *( quint32 * ) p;
      quint64 offset = *( quint16 * )( p + sizeof( quint32 ) );

      if ( mSwapEndian )
      {
        block = ntohl( block );
        offset = ntohs( offset );
      }

      oid = ( block << 16 ) + offset;
    }
    break;

    case 8:
    {
      quint32 oid0 = *( quint32 * ) p;
      quint32 oid1 = *( quint32 * )( p + sizeof( quint32 ) );

      if ( mSwapEndian )
      {
        QgsDebugMsgLevel( QString( "swap oid0:%1 oid1:%2" ).arg( oid0 ).arg( oid1 ), 4 );
        oid0 = ntohl( oid0 );
        oid1 = ntohl( oid1 );
      }

      QgsDebugMsgLevel( QString( "oid0:%1 oid1:%2" ).arg( oid0 ).arg( oid1 ), 4 );
      oid   = oid0;
      QgsDebugMsgLevel( QString( "oid:%1" ).arg( oid ), 4 );
      oid <<= 32;
      QgsDebugMsgLevel( QString( "oid:%1" ).arg( oid ), 4 );
      oid  |= oid1;
      QgsDebugMsgLevel( QString( "oid:%1" ).arg( oid ), 4 );
    }
    break;

    default:
      QgsDebugMsg( QString( "unexpected size %1" ).arg( s ) );
      //intentional fall-through
      FALLTHROUGH;
    case 4:
      oid = *( quint32 * )p;
      if ( mSwapEndian )
        oid = ntohl( oid );
      /* cast to signed 32bit
       * See http://hub.qgis.org/issues/14262 */
      oid = ( qint32 )oid;
      break;
  }

  return oid;
}

QString QgsPostgresConn::fieldExpression( const QgsField &fld, QString expr )
{
  const QString &type = fld.typeName();
  expr = expr.arg( quotedIdentifier( fld.name() ) );
  if ( type == "money" )
  {
    return QString( "cash_out(%1)::text" ).arg( expr );
  }
  else if ( type.startsWith( '_' ) )
  {
    return QString( "array_out(%1)::text" ).arg( expr );
  }
  else if ( type == "bool" )
  {
    return QString( "boolout(%1)::text" ).arg( expr );
  }
  else if ( type == "geometry" )
  {
    return QString( "%1(%2)" )
           .arg( majorVersion() < 2 ? "asewkt" : "st_asewkt",
                 expr );
  }
  else if ( type == "geography" )
  {
    return QString( "st_astext(%1)" ).arg( expr );
  }
  else
  {
    return expr + "::text";
  }
}

void QgsPostgresConn::deduceEndian()
{
  // need to store the PostgreSQL endian format used in binary cursors
  // since it appears that starting with
  // version 7.4, binary cursors return data in XDR whereas previous versions
  // return data in the endian of the server

  QgsPostgresResult res( PQexec( "select regclass('pg_class')::oid" ) );
  QString oidValue = res.PQgetvalue( 0, 0 );

  QgsDebugMsg( "Creating binary cursor" );

  // get the same value using a binary cursor
  openCursor( "oidcursor", "select regclass('pg_class')::oid" );

  QgsDebugMsg( "Fetching a record and attempting to get check endian-ness" );

  res = PQexec( "fetch forward 1 from oidcursor" );

  mSwapEndian = true;
  if ( res.PQntuples() > 0 )
  {
    // get the oid value from the binary cursor
    qint64 oid = getBinaryInt( res, 0, 0 );

    QgsDebugMsg( QString( "Got oid of %1 from the binary cursor" ).arg( oid ) );
    QgsDebugMsg( QString( "First oid is %1" ).arg( oidValue ) );

    // compare the two oid values to determine if we need to do an endian swap
    if ( oid != oidValue.toLongLong() )
      mSwapEndian = false;
  }

  closeCursor( "oidcursor" );
}

void QgsPostgresConn::retrieveLayerTypes( QgsPostgresLayerProperty &layerProperty, bool useEstimatedMetadata )
{
  QString table;

  if ( !layerProperty.schemaName.isEmpty() )
  {
    table = QString( "%1.%2" )
            .arg( quotedIdentifier( layerProperty.schemaName ),
                  quotedIdentifier( layerProperty.tableName ) );
  }
  else
  {
    // Query
    table = layerProperty.tableName;
  }

  if ( !layerProperty.geometryColName.isEmpty() )
  {
    // our estimatation ignores that a where clause might restrict the feature type or srid
    if ( useEstimatedMetadata )
    {
      table = QString( "(SELECT %1 FROM %2%3 LIMIT %4) AS t" )
              .arg( quotedIdentifier( layerProperty.geometryColName ),
                    table,
                    layerProperty.sql.isEmpty() ? "" : QString( " WHERE %1" ).arg( layerProperty.sql ) )
              .arg( sGeomTypeSelectLimit );
    }
    else if ( !layerProperty.sql.isEmpty() )
    {
      table += QString( " WHERE %1" ).arg( layerProperty.sql );
    }

    QString query = "SELECT DISTINCT ";

    bool castToGeometry = layerProperty.geometryColType == sctGeography ||
                          layerProperty.geometryColType == sctPcPatch;

    QGis::WkbType type = layerProperty.types.value( 0, QGis::WKBUnknown );
    if ( type == QGis::WKBUnknown )
    {
      query += QString( "upper(geometrytype(%1%2))" )
               .arg( quotedIdentifier( layerProperty.geometryColName ),
                     castToGeometry ?  "::geometry" : "" );
    }
    else
    {
      query += quotedValue( QgsPostgresConn::postgisWkbTypeName( type ) );
    }

    query += ',';

    int srid = layerProperty.srids.value( 0, INT_MIN );
    if ( srid  == INT_MIN )
    {
      query += QString( "%1(%2%3)" )
               .arg( majorVersion() < 2 ? "srid" : "st_srid",
                     quotedIdentifier( layerProperty.geometryColName ),
                     castToGeometry ?  "::geometry" : "" );
    }
    else
    {
      query += QString::number( srid );
    }

    if ( !layerProperty.force2d )
    {
      query += QString( ",%1(%2%3)" )
               .arg( majorVersion() < 2 ? "ndims" : "st_ndims",
                     quotedIdentifier( layerProperty.geometryColName ),
                     castToGeometry ?  "::geometry" : "" );
    }

    query += " FROM " + table;

    //QgsDebugMsg( "Retrieving geometry types,srids and dims: " + query );

    QgsPostgresResult gresult( PQexec( query ) );

    if ( gresult.PQresultStatus() == PGRES_TUPLES_OK )
    {
      for ( int i = 0; i < gresult.PQntuples(); i++ )
      {
        QString type = gresult.PQgetvalue( i, 0 );
        QString srid = gresult.PQgetvalue( i, 1 );

        if ( !layerProperty.force2d && gresult.PQgetvalue( i, 2 ).toInt() > 3 )
        {
          layerProperty.force2d = true;
        }

        if ( type.isEmpty() )
          continue;

        // if both multi and single types exists, go for the multi type,
        // so that st_multi can be applied if necessary.
        QGis::WkbType wkbType0 = QGis::flatType( QgsPostgresConn::wkbTypeFromPostgis( type ) );
        QGis::WkbType multiType0 = QGis::multiType( wkbType0 );

        int j;
        for ( j = 0; j < layerProperty.size(); j++ )
        {
          if ( layerProperty.srids.at( j ) != srid.toInt() )
            continue;

          QGis::WkbType wkbType1 = layerProperty.types.at( j );
          QGis::WkbType multiType1 = QGis::multiType( wkbType1 );
          if ( multiType0 == multiType1 && wkbType0 != wkbType1 )
          {
            layerProperty.types[j] = multiType0;
            break;
          }
        }

        if ( j < layerProperty.size() )
          break;

        layerProperty.types << wkbType0;
        layerProperty.srids << srid.toInt();
      }
    }
  }
}

void QgsPostgresConn::postgisWkbType( QGis::WkbType wkbType, QString &geometryType, int &dim )
{
  switch ( wkbType )
  {
    case QGis::WKBPoint25D:
      dim = 3;
      FALLTHROUGH;
    case QGis::WKBPoint:
      geometryType = "POINT";
      break;

    case QGis::WKBLineString25D:
      dim = 3;
      FALLTHROUGH;
    case QGis::WKBLineString:
      geometryType = "LINESTRING";
      break;

    case QGis::WKBPolygon25D:
      dim = 3;
      FALLTHROUGH;
    case QGis::WKBPolygon:
      geometryType = "POLYGON";
      break;

    case QGis::WKBMultiPoint25D:
      dim = 3;
      FALLTHROUGH;
    case QGis::WKBMultiPoint:
      geometryType = "MULTIPOINT";
      break;

    case QGis::WKBMultiLineString25D:
      dim = 3;
      FALLTHROUGH;
    case QGis::WKBMultiLineString:
      geometryType = "MULTILINESTRING";
      break;

    case QGis::WKBMultiPolygon25D:
      dim = 3;
      FALLTHROUGH;
    case QGis::WKBMultiPolygon:
      geometryType = "MULTIPOLYGON";
      break;

    case QGis::WKBUnknown:
      geometryType = "GEOMETRY";
      break;

    case QGis::WKBNoGeometry:
    default:
      dim = 0;
      break;
  }
}

QString QgsPostgresConn::postgisWkbTypeName( QGis::WkbType wkbType )
{
  QString geometryType;
  int dim;

  postgisWkbType( wkbType, geometryType, dim );

  return geometryType;
}

QString QgsPostgresConn::postgisTypeFilter( QString geomCol, QgsWKBTypes::Type wkbType, bool castToGeometry )
{
  geomCol = quotedIdentifier( geomCol );
  if ( castToGeometry )
    geomCol += "::geometry";

  QgsWKBTypes::GeometryType geomType = QgsWKBTypes::geometryType( wkbType );
  switch ( geomType )
  {
    case QgsWKBTypes::PointGeometry:
      return QString( "upper(geometrytype(%1)) IN ('POINT','POINTZ','POINTM','POINTZM','MULTIPOINT','MULTIPOINTZ','MULTIPOINTM','MULTIPOINTZM')" ).arg( geomCol );
    case QgsWKBTypes::LineGeometry:
      return QString( "upper(geometrytype(%1)) IN ('LINESTRING','LINESTRINGZ','LINESTRINGM','LINESTRINGZM','CIRCULARSTRING','CIRCULARSTRINGZ','CIRCULARSTRINGM','CIRCULARSTRINGZM','COMPOUNDCURVE','COMPOUNDCURVEZ','COMPOUNDCURVEM','COMPOUNDCURVEZM','MULTILINESTRING','MULTILINESTRINGZ','MULTILINESTRINGM','MULTILINESTRINGZM','MULTICURVE','MULTICURVEZ','MULTICURVEM','MULTICURVEZM')" ).arg( geomCol );
    case QgsWKBTypes::PolygonGeometry:
      return QString( "upper(geometrytype(%1)) IN ('POLYGON','POLYGONZ','POLYGONM','POLYGONZM','CURVEPOLYGON','CURVEPOLYGONZ','CURVEPOLYGONM','CURVEPOLYGONZM','MULTIPOLYGON','MULTIPOLYGONZ','MULTIPOLYGONM','MULTIPOLYGONZM','MULTIPOLYGONM','MULTISURFACE','MULTISURFACEZ','MULTISURFACEM','MULTISURFACEZM','POLYHEDRALSURFACE','TIN')" ).arg( geomCol );
    case QgsWKBTypes::NullGeometry:
      return QString( "geometrytype(%1) IS NULL" ).arg( geomCol );
    default: //unknown geometry
      return QString::null;
  }
}

int QgsPostgresConn::postgisWkbTypeDim( QGis::WkbType wkbType )
{
  QString geometryType;
  int dim;

  postgisWkbType( wkbType, geometryType, dim );

  return dim;
}

QGis::WkbType QgsPostgresConn::wkbTypeFromPostgis( const QString& type )
{
  // Polyhedral surfaces and TIN are stored in PostGIS as geometry collections
  // of Polygons and Triangles.
  // So, since QGIS does not natively support PS and TIN, but we would like to open them if possible,
  // we consider them as multipolygons. WKB will be converted by the feature iterator
  if (( type == "POLYHEDRALSURFACE" ) || ( type == "TIN" ) )
  {
    return QGis::WKBMultiPolygon;
  }
  else if ( type == "TRIANGLE" )
  {
    return QGis::WKBPolygon;
  }
  return ( QGis::WkbType )QgsWKBTypes::parseType( type );
}

QgsWKBTypes::Type QgsPostgresConn::wkbTypeFromOgcWkbType( unsigned int wkbType )
{
  // PolyhedralSurface => MultiPolygon
  if ( wkbType % 1000 == 15 )
    return ( QgsWKBTypes::Type )( wkbType / 1000 * 1000 + QgsWKBTypes::MultiPolygon );
  // TIN => MultiPolygon
  if ( wkbType % 1000 == 16 )
    return ( QgsWKBTypes::Type )( wkbType / 1000 * 1000 + QgsWKBTypes::MultiPolygon );
  // Triangle => Polygon
  if ( wkbType % 1000 == 17 )
    return ( QgsWKBTypes::Type )( wkbType / 1000 * 1000 + QgsWKBTypes::Polygon );
  return ( QgsWKBTypes::Type ) wkbType;
}

QString QgsPostgresConn::displayStringForWkbType( QGis::WkbType type )
{
  return QgsWKBTypes::displayString( QgsWKBTypes::Type( type ) );
}

QString QgsPostgresConn::displayStringForGeomType( QgsPostgresGeometryColumnType type )
{
  switch ( type )
  {
    case sctNone:
      return tr( "None" );
    case sctGeometry:
      return tr( "Geometry" );
    case sctGeography:
      return tr( "Geography" );
    case sctTopoGeometry:
      return tr( "TopoGeometry" );
    case sctPcPatch:
      return tr( "PcPatch" );
  }

  Q_ASSERT( !"unexpected geometry column type" );
  return QString::null;
}

QGis::WkbType QgsPostgresConn::wkbTypeFromGeomType( QGis::GeometryType geomType )
{
  switch ( geomType )
  {
    case QGis::Point:
      return QGis::WKBPoint;
    case QGis::Line:
      return QGis::WKBLineString;
    case QGis::Polygon:
      return QGis::WKBPolygon;
    case QGis::NoGeometry:
      return QGis::WKBNoGeometry;
    case QGis::UnknownGeometry:
      return QGis::WKBUnknown;
  }

  Q_ASSERT( !"unexpected geomType" );
  return QGis::WKBUnknown;
}

QStringList QgsPostgresConn::connectionList()
{
  QSettings settings;
  settings.beginGroup( "/PostgreSQL/connections" );
  return settings.childGroups();
}

QString QgsPostgresConn::selectedConnection()
{
  QSettings settings;
  return settings.value( "/PostgreSQL/connections/selected" ).toString();
}

void QgsPostgresConn::setSelectedConnection( const QString& name )
{
  QSettings settings;
  return settings.setValue( "/PostgreSQL/connections/selected", name );
}

QgsDataSourceURI QgsPostgresConn::connUri( const QString& theConnName )
{
  QgsDebugMsg( "theConnName = " + theConnName );

  QSettings settings;

  QString key = "/PostgreSQL/connections/" + theConnName;

  QString service = settings.value( key + "/service" ).toString();
  QString host = settings.value( key + "/host" ).toString();
  QString port = settings.value( key + "/port" ).toString();
  if ( port.length() == 0 )
  {
    port = "5432";
  }
  QString database = settings.value( key + "/database" ).toString();

  bool useEstimatedMetadata = settings.value( key + "/estimatedMetadata", false ).toBool();
  int sslmode = settings.value( key + "/sslmode", QgsDataSourceURI::SSLprefer ).toInt();

  QString username;
  QString password;
  if ( settings.value( key + "/saveUsername" ).toString() == "true" )
  {
    username = settings.value( key + "/username" ).toString();
  }

  if ( settings.value( key + "/savePassword" ).toString() == "true" )
  {
    password = settings.value( key + "/password" ).toString();
  }

  // Old save setting
  if ( settings.contains( key + "/save" ) )
  {
    username = settings.value( key + "/username" ).toString();

    if ( settings.value( key + "/save" ).toString() == "true" )
    {
      password = settings.value( key + "/password" ).toString();
    }
  }

  QString authcfg = settings.value( key + "/authcfg" ).toString();

  if ( !authcfg.isEmpty() )
  {
    username.clear();
    password.clear();
  }

  QgsDataSourceURI uri;
  if ( !service.isEmpty() )
  {
    uri.setConnection( service, database, username, password, ( QgsDataSourceURI::SSLmode ) sslmode, authcfg );
  }
  else
  {
    uri.setConnection( host, port, database, username, password, ( QgsDataSourceURI::SSLmode ) sslmode, authcfg );
  }
  uri.setUseEstimatedMetadata( useEstimatedMetadata );

  return uri;
}

bool QgsPostgresConn::publicSchemaOnly( const QString& theConnName )
{
  QSettings settings;
  return settings.value( "/PostgreSQL/connections/" + theConnName + "/publicOnly", false ).toBool();
}

bool QgsPostgresConn::geometryColumnsOnly( const QString& theConnName )
{
  QSettings settings;

  return settings.value( "/PostgreSQL/connections/" + theConnName + "/geometryColumnsOnly", false ).toBool();
}

bool QgsPostgresConn::dontResolveType( const QString& theConnName )
{
  QSettings settings;

  return settings.value( "/PostgreSQL/connections/" + theConnName + "/dontResolveType", false ).toBool();
}

bool QgsPostgresConn::allowGeometrylessTables( const QString& theConnName )
{
  QSettings settings;
  return settings.value( "/PostgreSQL/connections/" + theConnName + "/allowGeometrylessTables", false ).toBool();
}

void QgsPostgresConn::deleteConnection( const QString& theConnName )
{
  QSettings settings;

  QString key = "/PostgreSQL/connections/" + theConnName;
  settings.remove( key + "/service" );
  settings.remove( key + "/host" );
  settings.remove( key + "/port" );
  settings.remove( key + "/database" );
  settings.remove( key + "/username" );
  settings.remove( key + "/password" );
  settings.remove( key + "/sslmode" );
  settings.remove( key + "/publicOnly" );
  settings.remove( key + "/geometryColumnsOnly" );
  settings.remove( key + "/allowGeometrylessTables" );
  settings.remove( key + "/estimatedMetadata" );
  settings.remove( key + "/saveUsername" );
  settings.remove( key + "/savePassword" );
  settings.remove( key + "/save" );
  settings.remove( key + "/authcfg" );
  settings.remove( key );
}

bool QgsPostgresConn::cancel()
{
  PGcancel *c = ::PQgetCancel( mConn );
  if ( !c )
  {
    QgsMessageLog::logMessage( tr( "Query could not be canceled [%1]" ).arg( tr( "PQgetCancel failed" ) ),
                               tr( "PostGIS" ) );
    return false;
  }

  char errbuf[256];
  int res = ::PQcancel( c, errbuf, sizeof errbuf );
  ::PQfreeCancel( c );

  if ( !res )
    QgsMessageLog::logMessage( tr( "Query could not be canceled [%1]" ).arg( errbuf ), tr( "PostGIS" ) );

  return res == 0;
}
