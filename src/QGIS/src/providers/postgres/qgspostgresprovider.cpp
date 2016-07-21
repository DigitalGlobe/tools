/***************************************************************************
  qgspostgresprovider.cpp  -  QGIS data provider for PostgreSQL/PostGIS layers
                             -------------------
    begin                : 2004/01/07
    copyright            : (C) 2004 by Gary E.Sherman
    email                : sherman at mrcc.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qgsapplication.h>
#include <qgsfeature.h>
#include <qgsfield.h>
#include <qgsgeometry.h>
#include <qgsmessageoutput.h>
#include <qgsmessagelog.h>
#include <qgsrectangle.h>
#include <qgscoordinatereferencesystem.h>

#include <QMessageBox>

#include "qgsvectorlayerimport.h"
#include "qgsprovidercountcalcevent.h"
#include "qgsproviderextentcalcevent.h"
#include "qgspostgresprovider.h"
#include "qgspostgresconn.h"
#include "qgspostgresconnpool.h"
#include "qgspgsourceselect.h"
#include "qgspostgresdataitems.h"
#include "qgspostgresfeatureiterator.h"
#include "qgspostgrestransaction.h"
#include "qgslogger.h"
#include "qgscrscache.h"

const QString POSTGRES_KEY = "postgres";
const QString POSTGRES_DESCRIPTION = "PostgreSQL/PostGIS data provider";

inline qint64 PKINT2FID( qint32 x )
{
  return QgsPostgresUtils::int32pk_to_fid( x );
}

inline qint32 FID2PKINT( qint64 x )
{
  return QgsPostgresUtils::fid_to_int32pk( x );
}

QgsPostgresPrimaryKeyType
QgsPostgresProvider::pkType( const QgsField& f ) const
{
  switch ( f.type() )
  {
    case QVariant::LongLong:
      // unless we can guarantee all values are unsigned
      // (in which case we could use pktUint64)
      // we'll have to use a Map type.
      // See http://hub.qgis.org/issues/14262
      return pktFidMap; // pktUint64

    case QVariant::Int:
      return pktInt;

    default:
      return pktFidMap;
  }
}



QgsPostgresProvider::QgsPostgresProvider( QString const & uri )
    : QgsVectorDataProvider( uri )
    , mValid( false )
    , mPrimaryKeyType( pktUnknown )
    , mSpatialColType( sctNone )
    , mDetectedGeomType( QGis::WKBUnknown )
    , mForce2d( false )
    , mRequestedGeomType( QGis::WKBUnknown )
    , mShared( new QgsPostgresSharedData )
    , mUseEstimatedMetadata( false )
    , mSelectAtIdDisabled( false )
    , mEnabledCapabilities( 0 )
    , mConnectionRO( nullptr )
    , mConnectionRW( nullptr )
    , mTransaction( nullptr )
{

  QgsDebugMsg( QString( "URI: %1 " ).arg( uri ) );

  mUri = QgsDataSourceURI( uri );

  // populate members from the uri structure
  mSchemaName = mUri.schema();
  mTableName = mUri.table();
  mGeometryColumn = mUri.geometryColumn();
  mSqlWhereClause = mUri.sql();
  mRequestedSrid = mUri.srid();
  mRequestedGeomType = QGis::fromNewWkbType( mUri.newWkbType() );

  if ( mSchemaName.isEmpty() && mTableName.startsWith( '(' ) && mTableName.endsWith( ')' ) )
  {
    mIsQuery = true;
    mQuery = mTableName;
    mTableName = "";
  }
  else
  {
    mIsQuery = false;

    if ( !mSchemaName.isEmpty() )
    {
      mQuery += quotedIdentifier( mSchemaName ) + '.';
    }

    if ( !mTableName.isEmpty() )
    {
      mQuery += quotedIdentifier( mTableName );
    }
  }

  mUseEstimatedMetadata = mUri.useEstimatedMetadata();
  mSelectAtIdDisabled = mUri.selectAtIdDisabled();

  QgsDebugMsg( QString( "Connection info is %1" ).arg( mUri.connectionInfo( false ) ) );
  QgsDebugMsg( QString( "Geometry column is: %1" ).arg( mGeometryColumn ) );
  QgsDebugMsg( QString( "Schema is: %1" ).arg( mSchemaName ) );
  QgsDebugMsg( QString( "Table name is: %1" ).arg( mTableName ) );
  QgsDebugMsg( QString( "Query is: %1" ).arg( mQuery ) );
  QgsDebugMsg( QString( "Where clause is: %1" ).arg( mSqlWhereClause ) );

  // no table/query passed, the provider could be used to get tables
  if ( mQuery.isEmpty() )
  {
    return;
  }

  mConnectionRO = QgsPostgresConn::connectDb( mUri.connectionInfo( false ), true );
  if ( !mConnectionRO )
  {
    return;
  }

  if ( !hasSufficientPermsAndCapabilities() ) // check permissions and set capabilities
  {
    disconnectDb();
    return;
  }

  if ( !getGeometryDetails() ) // gets srid, geometry and data type
  {
    // the table is not a geometry table
    QgsMessageLog::logMessage( tr( "invalid PostgreSQL layer" ), tr( "PostGIS" ) );
    disconnectDb();
    return;
  }

  // NOTE: mValid would be true after true return from
  // getGeometryDetails, see http://hub.qgis.org/issues/13781

  if ( mSpatialColType == sctTopoGeometry )
  {
    if ( !getTopoLayerInfo() ) // gets topology name and layer id
    {
      QgsMessageLog::logMessage( tr( "invalid PostgreSQL topology layer" ), tr( "PostGIS" ) );
      mValid = false;
      disconnectDb();
      return;
    }
  }

  mLayerExtent.setMinimal();

  // set the primary key
  if ( !determinePrimaryKey() )
  {
    QgsMessageLog::logMessage( tr( "PostgreSQL layer has no primary key." ), tr( "PostGIS" ) );
    mValid = false;
    disconnectDb();
    return;
  }

  // Set the postgresql message level so that we don't get the
  // 'there is no transaction in progress' warning.
#ifndef QGISDEBUG
  mConnectionRO->PQexecNR( "set client_min_messages to error" );
#endif

  //fill type names into sets
  mNativeTypes
  // integer types
  << QgsVectorDataProvider::NativeType( tr( "Whole number (smallint - 16bit)" ), "int2", QVariant::Int, -1, -1, 0, 0 )
  << QgsVectorDataProvider::NativeType( tr( "Whole number (integer - 32bit)" ), "int4", QVariant::Int, -1, -1, 0, 0 )
  << QgsVectorDataProvider::NativeType( tr( "Whole number (integer - 64bit)" ), "int8", QVariant::LongLong, -1, -1, 0, 0 )
  << QgsVectorDataProvider::NativeType( tr( "Decimal number (numeric)" ), "numeric", QVariant::Double, 1, 20, 0, 20 )
  << QgsVectorDataProvider::NativeType( tr( "Decimal number (decimal)" ), "decimal", QVariant::Double, 1, 20, 0, 20 )

  // floating point
  << QgsVectorDataProvider::NativeType( tr( "Decimal number (real)" ), "real", QVariant::Double, -1, -1, -1, -1 )
  << QgsVectorDataProvider::NativeType( tr( "Decimal number (double)" ), "double precision", QVariant::Double, -1, -1, -1, -1 )

  // string types
  << QgsVectorDataProvider::NativeType( tr( "Text, fixed length (char)" ), "char", QVariant::String, 1, 255, -1, -1 )
  << QgsVectorDataProvider::NativeType( tr( "Text, limited variable length (varchar)" ), "varchar", QVariant::String, 1, 255, -1, -1 )
  << QgsVectorDataProvider::NativeType( tr( "Text, unlimited length (text)" ), "text", QVariant::String, -1, -1, -1, -1 )

  // date type
  << QgsVectorDataProvider::NativeType( tr( "Date" ), "date", QVariant::Date, -1, -1, -1, -1 )
  << QgsVectorDataProvider::NativeType( tr( "Time" ), "time", QVariant::Time, -1, -1, -1, -1 )
  << QgsVectorDataProvider::NativeType( tr( "Date & Time" ), "timestamp without time zone", QVariant::DateTime, -1, -1, -1, -1 )
  ;

  QString key;
  switch ( mPrimaryKeyType )
  {
    case pktOid:
      key = "oid";
      break;
    case pktTid:
      key = "tid";
      break;
    case pktInt:
    case pktUint64:
      Q_ASSERT( mPrimaryKeyAttrs.size() == 1 );
      Q_ASSERT( mPrimaryKeyAttrs[0] >= 0 && mPrimaryKeyAttrs[0] < mAttributeFields.count() );
      key = mAttributeFields.at( mPrimaryKeyAttrs.at( 0 ) ).name();
      break;
    case pktFidMap:
    {
      QString delim;
      Q_FOREACH ( int idx, mPrimaryKeyAttrs )
      {
        key += delim + quotedIdentifier( mAttributeFields.at( idx ).name() );
        delim = ',';
      }
    }
    break;
    case pktUnknown:
      QgsMessageLog::logMessage( tr( "PostgreSQL layer has unknown primary key type." ), tr( "PostGIS" ) );
      mValid = false;
      break;
  }

  if ( mValid )
  {
    mUri.setKeyColumn( key );
    setDataSourceUri( mUri.uri( false ) );
  }
  else
  {
    disconnectDb();
  }
}

QgsPostgresProvider::~QgsPostgresProvider()
{
  disconnectDb();

  QgsDebugMsg( "deconstructing." );
}


QgsAbstractFeatureSource* QgsPostgresProvider::featureSource() const
{
  return new QgsPostgresFeatureSource( this );
}

QgsPostgresConn* QgsPostgresProvider::connectionRO() const
{
  return mTransaction ? mTransaction->connection() : mConnectionRO;
}

QgsPostgresConn* QgsPostgresProvider::connectionRW()
{
  if ( mTransaction )
  {
    return mTransaction->connection();
  }
  else if ( !mConnectionRW )
  {
    mConnectionRW = QgsPostgresConn::connectDb( mUri.connectionInfo( false ), false );
  }
  return mConnectionRW;
}

QgsTransaction* QgsPostgresProvider::transaction() const
{
  return static_cast<QgsTransaction*>( mTransaction );
}

void QgsPostgresProvider::setTransaction( QgsTransaction* transaction )
{
  // static_cast since layers cannot be added to a transaction of a non-matching provider
  mTransaction = static_cast<QgsPostgresTransaction*>( transaction );
}

void QgsPostgresProvider::disconnectDb()
{
  if ( mConnectionRO )
  {
    mConnectionRO->unref();
    mConnectionRO = nullptr;
  }

  if ( mConnectionRW )
  {
    mConnectionRW->unref();
    mConnectionRW = nullptr;
  }
}

QString QgsPostgresProvider::storageType() const
{
  return "PostgreSQL database with PostGIS extension";
}

// Qt5 has that built in
#if QT_VERSION < 0x050000
static bool operator<( const QVariant &a, const QVariant &b )
{
  // invalid < NULL < any value
  if ( !a.isValid() )
    return b.isValid();
  else if ( a.isNull() )
    return b.isValid() && !b.isNull();
  else if ( !b.isValid() || b.isNull() )
    return false;

  if ( a.type() == b.type() )
  {
    switch ( a.type() )
    {
      case QVariant::Int:
      case QVariant::Char:
        return a.toInt() < b.toInt();

      case QVariant::Double:
        return a.toDouble() < b.toDouble();

      case QVariant::LongLong:
        return a.toLongLong() < b.toLongLong();

      case QVariant::List:
      {
        const QList<QVariant> &al = a.toList();
        const QList<QVariant> &bl = b.toList();

        int i, n = qMin( al.size(), bl.size() );
        for ( i = 0; i < n && al[i].type() == bl[i].type() && al[i].isNull() == bl[i].isNull() && al[i] == bl[i]; i++ )
          ;

        if ( i == n )
          return al.size() < bl.size();
        else
          return al[i] < bl[i];
      }

      case QVariant::StringList:
      {
        const QStringList &al = a.toStringList();
        const QStringList &bl = b.toStringList();

        int i, n = qMin( al.size(), bl.size() );
        for ( i = 0; i < n && al[i] == bl[i]; i++ )
          ;

        if ( i == n )
          return al.size() < bl.size();
        else
          return al[i] < bl[i];
      }

      case QVariant::Date:
        return a.toDate() < b.toDate();

      case QVariant::Time:
        return a.toTime() < b.toTime();

      case QVariant::DateTime:
        return a.toDateTime() < b.toDateTime();

      case QVariant::Bool:
        return a.toBool() < b.toBool();

      case QVariant::UInt:
        return a.toUInt() < b.toUInt();

      case QVariant::ULongLong:
        return a.toULongLong() < b.toULongLong();

      default:
        break;
    }
  }

  return a.canConvert( QVariant::String ) && b.canConvert( QVariant::String ) && a.toString() < b.toString();
}
#endif

QgsFeatureIterator QgsPostgresProvider::getFeatures( const QgsFeatureRequest& request )
{
  if ( !mValid )
  {
    QgsMessageLog::logMessage( tr( "Read attempt on an invalid postgresql data source" ), tr( "PostGIS" ) );
    return QgsFeatureIterator();
  }

  QgsPostgresFeatureSource* featureSrc = static_cast<QgsPostgresFeatureSource*>( featureSource() );
  return QgsFeatureIterator( new QgsPostgresFeatureIterator( featureSrc, true, request ) );
}



QString QgsPostgresProvider::pkParamWhereClause( int offset, const char *alias ) const
{
  QString whereClause;

  QString aliased;
  if ( alias ) aliased = QString( "%1." ).arg( alias );

  switch ( mPrimaryKeyType )
  {
    case pktTid:
      whereClause = QString( "%2ctid=$%1" ).arg( offset ).arg( aliased );
      break;

    case pktOid:
      whereClause = QString( "%2oid=$%1" ).arg( offset ).arg( aliased );
      break;

    case pktInt:
    case pktUint64:
      Q_ASSERT( mPrimaryKeyAttrs.size() == 1 );
      whereClause = QString( "%3%1=$%2" ).arg( quotedIdentifier( field( mPrimaryKeyAttrs[0] ).name() ) ).arg( offset ).arg( aliased );
      break;

    case pktFidMap:
    {
      QString delim = "";
      for ( int i = 0; i < mPrimaryKeyAttrs.size(); i++ )
      {
        int idx = mPrimaryKeyAttrs[i];
        const QgsField &fld = field( idx );

        whereClause += delim + QString( "%3%1=$%2" ).arg( connectionRO()->fieldExpression( fld ) ).arg( offset++ ).arg( aliased );
        delim = " AND ";
      }
    }
    break;

    case pktUnknown:
      Q_ASSERT( !"FAILURE: Primary key unknown" );
      whereClause = "NULL";
      break;
  }

  if ( !mSqlWhereClause.isEmpty() )
  {
    if ( !whereClause.isEmpty() )
      whereClause += " AND ";

    whereClause += '(' + mSqlWhereClause + ')';
  }

  return whereClause;
}

void QgsPostgresProvider::appendPkParams( QgsFeatureId featureId, QStringList &params ) const
{
  switch ( mPrimaryKeyType )
  {
    case pktOid:
    case pktUint64:
      params << QString::number( featureId );
      break;

    case pktInt:
      params << QString::number( FID2PKINT( featureId ) );
      break;

    case pktTid:
      params << QString( "'(%1,%2)'" ).arg( FID_TO_NUMBER( featureId ) >> 16 ).arg( FID_TO_NUMBER( featureId ) & 0xffff );
      break;

    case pktFidMap:
    {
      QVariant pkValsVariant = mShared->lookupKey( featureId );
      QList<QVariant> pkVals;
      if ( !pkValsVariant.isNull() )
      {
        pkVals = pkValsVariant.toList();
        Q_ASSERT( pkVals.size() == mPrimaryKeyAttrs.size() );
      }

      for ( int i = 0; i < mPrimaryKeyAttrs.size(); i++ )
      {
        if ( i < pkVals.size() )
        {
          params << pkVals[i].toString();
        }
        else
        {
          QgsDebugMsg( QString( "FAILURE: Key value %1 for feature %2 not found." ).arg( mPrimaryKeyAttrs[i] ).arg( featureId ) );
          params << "NULL";
        }
      }

      QgsDebugMsg( QString( "keys params: %1" ).arg( params.join( "; " ) ) );
    }
    break;

    case pktUnknown:
      Q_ASSERT( !"FAILURE: Primary key unknown" );
      break;
  }
}


QString QgsPostgresProvider::whereClause( QgsFeatureId featureId ) const
{
  return QgsPostgresUtils::whereClause( featureId, mAttributeFields, connectionRO(), mPrimaryKeyType, mPrimaryKeyAttrs, mShared );
}


QString QgsPostgresUtils::whereClause( QgsFeatureId featureId, const QgsFields& fields, QgsPostgresConn* conn, QgsPostgresPrimaryKeyType pkType, const QList<int>& pkAttrs, QSharedPointer<QgsPostgresSharedData> sharedData )
{
  QString whereClause;

  switch ( pkType )
  {
    case pktTid:
      whereClause = QString( "ctid='(%1,%2)'" )
                    .arg( FID_TO_NUMBER( featureId ) >> 16 )
                    .arg( FID_TO_NUMBER( featureId ) & 0xffff );
      break;

    case pktOid:
      whereClause = QString( "oid=%1" ).arg( featureId );
      break;

    case pktInt:
      Q_ASSERT( pkAttrs.size() == 1 );
      whereClause = QString( "%1=%2" ).arg( QgsPostgresConn::quotedIdentifier( fields[ pkAttrs[0] ].name() ) ).arg( FID2PKINT( featureId ) );
      break;

    case pktUint64:
      Q_ASSERT( pkAttrs.size() == 1 );
      whereClause = QString( "%1=%2" ).arg( QgsPostgresConn::quotedIdentifier( fields[ pkAttrs[0] ].name() ) ).arg( featureId );
      break;

    case pktFidMap:
    {
      QVariant pkValsVariant = sharedData->lookupKey( featureId );
      if ( !pkValsVariant.isNull() )
      {
        QList<QVariant> pkVals = pkValsVariant.toList();

        Q_ASSERT( pkVals.size() == pkAttrs.size() );

        QString delim = "";
        for ( int i = 0; i < pkAttrs.size(); i++ )
        {
          int idx = pkAttrs[i];
          const QgsField &fld = fields[ idx ];

          whereClause += delim + conn->fieldExpression( fld );
          if ( pkVals[i].isNull() )
            whereClause += " IS NULL";
          else
            whereClause += '=' + QgsPostgresConn::quotedValue( pkVals[i].toString() );

          delim = " AND ";
        }
      }
      else
      {
        QgsDebugMsg( QString( "FAILURE: Key values for feature %1 not found." ).arg( featureId ) );
        whereClause = "NULL";
      }
    }
    break;

    case pktUnknown:
      Q_ASSERT( !"FAILURE: Primary key unknown" );
      whereClause = "NULL";
      break;
  }

  return whereClause;
}

QString QgsPostgresUtils::whereClause( const QgsFeatureIds& featureIds, const QgsFields& fields, QgsPostgresConn* conn, QgsPostgresPrimaryKeyType pkType, const QList<int>& pkAttrs, QSharedPointer<QgsPostgresSharedData> sharedData )
{
  switch ( pkType )
  {
    case pktOid:
    case pktInt:
    case pktUint64:
    {
      QString expr;

      //simple primary key, so prefer to use an "IN (...)" query. These are much faster then multiple chained ...OR... clauses
      if ( !featureIds.isEmpty() )
      {
        QString delim;
        expr = QString( "%1 IN (" ).arg(( pkType == pktOid ? "oid" : QgsPostgresConn::quotedIdentifier( fields[ pkAttrs[0] ].name() ) ) );

        Q_FOREACH ( const QgsFeatureId featureId, featureIds )
        {
          expr += delim + FID_TO_STRING( pkType == pktOid ? featureId : pkType == pktUint64 ? featureId : FID2PKINT( featureId ) );
          delim = ',';
        }
        expr += ')';
      }

      return expr;
    }
    case pktFidMap:
    case pktTid:
    case pktUnknown:
    {
      //complex primary key, need to build up where string
      QStringList whereClauses;
      Q_FOREACH ( const QgsFeatureId featureId, featureIds )
      {
        whereClauses << whereClause( featureId, fields, conn, pkType, pkAttrs, sharedData );
      }
      return whereClauses.isEmpty() ? "" : whereClauses.join( " OR " ).prepend( '(' ).append( ')' );
    }
  }
  return QString(); //avoid warning
}

QString QgsPostgresUtils::andWhereClauses( const QString& c1, const QString& c2 )
{
  if ( c1.isEmpty() )
    return c2;
  if ( c2.isEmpty() )
    return c1;

  return QString( "(%1) AND (%2)" ).arg( c1, c2 );
}

QString QgsPostgresProvider::filterWhereClause() const
{
  QString where;
  QString delim = " WHERE ";

  if ( !mSqlWhereClause.isEmpty() )
  {
    where += delim + '(' + mSqlWhereClause + ')';
    delim = " AND ";
  }

  if ( !mRequestedSrid.isEmpty() && ( mRequestedSrid != mDetectedSrid || mRequestedSrid.toInt() == 0 ) )
  {
    where += delim + QString( "%1(%2%3)=%4" )
             .arg( connectionRO()->majorVersion() < 2 ? "srid" : "st_srid",
                   quotedIdentifier( mGeometryColumn ),
                   mSpatialColType == sctGeography ? "::geography" : "",
                   mRequestedSrid );
    delim = " AND ";
  }

  if ( mRequestedGeomType != QGis::WKBUnknown && mRequestedGeomType != mDetectedGeomType )
  {
    where += delim + QgsPostgresConn::postgisTypeFilter( mGeometryColumn, ( QgsWKBTypes::Type )mRequestedGeomType, mSpatialColType == sctGeography );
    delim = " AND ";
  }

  return where;
}

void QgsPostgresProvider::setExtent( QgsRectangle& newExtent )
{
  mLayerExtent.setXMaximum( newExtent.xMaximum() );
  mLayerExtent.setXMinimum( newExtent.xMinimum() );
  mLayerExtent.setYMaximum( newExtent.yMaximum() );
  mLayerExtent.setYMinimum( newExtent.yMinimum() );
}

/**
 * Return the feature type
 */
QGis::WkbType QgsPostgresProvider::geometryType() const
{
  return mRequestedGeomType != QGis::WKBUnknown ? mRequestedGeomType : mDetectedGeomType;
}

const QgsField &QgsPostgresProvider::field( int index ) const
{
  if ( index < 0 || index >= mAttributeFields.count() )
  {
    QgsMessageLog::logMessage( tr( "FAILURE: Field %1 not found." ).arg( index ), tr( "PostGIS" ) );
    throw PGFieldNotFound();
  }

  return mAttributeFields[index];
}

const QgsFields & QgsPostgresProvider::fields() const
{
  return mAttributeFields;
}

QString QgsPostgresProvider::dataComment() const
{
  return mDataComment;
}


/** @todo XXX Perhaps this should be promoted to QgsDataProvider? */
QString QgsPostgresProvider::endianString()
{
  switch ( QgsApplication::endian() )
  {
    case QgsApplication::NDR:
      return QString( "NDR" );
    case QgsApplication::XDR:
      return QString( "XDR" );
    default :
      return QString( "Unknown" );
  }
}


struct PGTypeInfo
{
  QString typeName;
  QString typeType;
  QString typeElem;
  int typeLen;
};

bool QgsPostgresProvider::loadFields()
{
  if ( !mIsQuery )
  {
    QgsDebugMsg( QString( "Loading fields for table %1" ).arg( mTableName ) );

    // Get the relation oid for use in later queries
    QString sql = QString( "SELECT regclass(%1)::oid" ).arg( quotedValue( mQuery ) );
    QgsPostgresResult tresult( connectionRO()->PQexec( sql ) );
    QString tableoid = tresult.PQgetvalue( 0, 0 );

    // Get the table description
    sql = QString( "SELECT description FROM pg_description WHERE objoid=%1 AND objsubid=0" ).arg( tableoid );
    tresult = connectionRO()->PQexec( sql );
    if ( tresult.PQntuples() > 0 )
      mDataComment = tresult.PQgetvalue( 0, 0 );
  }

  // Populate the field vector for this layer. The field vector contains
  // field name, type, length, and precision (if numeric)
  QString sql = QString( "SELECT * FROM %1 LIMIT 0" ).arg( mQuery );

  QgsPostgresResult result( connectionRO()->PQexec( sql ) );

  // Collect type info
  sql = "SELECT oid,typname,typtype,typelem,typlen FROM pg_type";
  QgsPostgresResult typeResult( connectionRO()->PQexec( sql ) );

  QMap<int, PGTypeInfo> typeMap;
  for ( int i = 0; i < typeResult.PQntuples(); ++i )
  {
    PGTypeInfo typeInfo =
    {
      /* typeName = */ typeResult.PQgetvalue( i, 1 ),
      /* typeType = */ typeResult.PQgetvalue( i, 2 ),
      /* typeElem = */ typeResult.PQgetvalue( i, 3 ),
      /* typeLen = */ typeResult.PQgetvalue( i, 4 ).toInt()
    };
    typeMap.insert( typeResult.PQgetvalue( i, 0 ).toInt(), typeInfo );
  }


  QMap<int, QMap<int, QString> > fmtFieldTypeMap, descrMap, defValMap;
  QMap<int, QMap<int, int> > attTypeIdMap;
  if ( result.PQnfields() > 0 )
  {
    // Collect table oids
    QSet<int> tableoids;
    for ( int i = 0; i < result.PQnfields(); i++ )
    {
      int tableoid = result.PQftable( i );
      if ( tableoid > 0 )
      {
        tableoids.insert( tableoid );
      }
    }

    if ( !tableoids.isEmpty() )
    {
      QStringList tableoidsList;
      Q_FOREACH ( int tableoid, tableoids )
      {
        tableoidsList.append( QString::number( tableoid ) );
      }

      QString tableoidsFilter = '(' + tableoidsList.join( "," ) + ')';

      // Collect formatted field types
      sql = "SELECT attrelid, attnum, pg_catalog.format_type(atttypid,atttypmod), pg_catalog.col_description(attrelid,attnum), pg_catalog.pg_get_expr(adbin,adrelid), atttypid"
            " FROM pg_attribute"
            " LEFT OUTER JOIN pg_attrdef ON attrelid=adrelid AND attnum=adnum"
            " WHERE attrelid IN " + tableoidsFilter;
      QgsPostgresResult fmtFieldTypeResult( connectionRO()->PQexec( sql ) );
      for ( int i = 0; i < fmtFieldTypeResult.PQntuples(); ++i )
      {
        int attrelid = fmtFieldTypeResult.PQgetvalue( i, 0 ).toInt();
        int attnum = fmtFieldTypeResult.PQgetvalue( i, 1 ).toInt();
        QString formatType = fmtFieldTypeResult.PQgetvalue( i, 2 );
        QString descr = fmtFieldTypeResult.PQgetvalue( i, 3 );
        QString defVal = fmtFieldTypeResult.PQgetvalue( i, 4 );
        int attType = fmtFieldTypeResult.PQgetvalue( i, 5 ).toInt();
        fmtFieldTypeMap[attrelid][attnum] = formatType;
        descrMap[attrelid][attnum] = descr;
        defValMap[attrelid][attnum] = defVal;
        attTypeIdMap[attrelid][attnum] = attType;
      }
    }
  }

  QSet<QString> fields;
  mAttributeFields.clear();
  for ( int i = 0; i < result.PQnfields(); i++ )
  {
    QString fieldName = result.PQfname( i );
    if ( fieldName == mGeometryColumn )
      continue;

    int fldtyp = result.PQftype( i );
    int fldMod = result.PQfmod( i );
    int fieldPrec = -1;
    int tableoid = result.PQftable( i );
    int attnum = result.PQftablecol( i );
    int atttypid = attTypeIdMap[tableoid][attnum];

    const PGTypeInfo& typeInfo = typeMap.value( fldtyp );
    QString fieldTypeName = typeInfo.typeName;
    QString fieldTType = typeInfo.typeType;
    int fieldSize = typeInfo.typeLen;

    bool isDomain = ( typeMap.value( atttypid ).typeType == "d" );

    QString formattedFieldType = fmtFieldTypeMap[tableoid][attnum];
    QString originalFormattedFieldType = formattedFieldType;
    if ( isDomain )
    {
      // get correct formatted field type for domain
      sql = QString( "SELECT format_type(%1, %2)" ).arg( fldtyp ).arg( fldMod );
      QgsPostgresResult fmtFieldModResult( connectionRO()->PQexec( sql ) );
      if ( fmtFieldModResult.PQntuples() > 0 )
      {
        formattedFieldType = fmtFieldModResult.PQgetvalue( 0, 0 );
      }
    }

    QString fieldComment = descrMap[tableoid][attnum];

    QVariant::Type fieldType;

    if ( fieldTType == "b" )
    {
      bool isArray = fieldTypeName.startsWith( '_' );

      if ( isArray )
        fieldTypeName = fieldTypeName.mid( 1 );

      if ( fieldTypeName == "int8" || fieldTypeName == "serial8" )
      {
        fieldType = QVariant::LongLong;
        fieldSize = -1;
        fieldPrec = 0;
      }
      else if ( fieldTypeName == "int2" || fieldTypeName == "int4" ||
                fieldTypeName == "oid" || fieldTypeName == "serial" )
      {
        fieldType = QVariant::Int;
        fieldSize = -1;
        fieldPrec = 0;
      }
      else if ( fieldTypeName == "real" || fieldTypeName == "double precision" ||
                fieldTypeName == "float4" || fieldTypeName == "float8" )
      {
        fieldType = QVariant::Double;
        fieldSize = -1;
        fieldPrec = -1;
      }
      else if ( fieldTypeName == "numeric" )
      {
        fieldType = QVariant::Double;

        if ( formattedFieldType == "numeric" || formattedFieldType == "" )
        {
          fieldSize = -1;
          fieldPrec = -1;
        }
        else
        {
          QRegExp re( "numeric\\((\\d+),(\\d+)\\)" );
          if ( re.exactMatch( formattedFieldType ) )
          {
            fieldSize = re.cap( 1 ).toInt();
            fieldPrec = re.cap( 2 ).toInt();
          }
          else if ( formattedFieldType != "numeric" )
          {
            QgsMessageLog::logMessage( tr( "unexpected formatted field type '%1' for field %2" )
                                       .arg( formattedFieldType,
                                             fieldName ),
                                       tr( "PostGIS" ) );
            fieldSize = -1;
            fieldPrec = -1;
          }
        }
      }
      else if ( fieldTypeName == "varchar" )
      {
        fieldType = QVariant::String;

        QRegExp re( "character varying\\((\\d+)\\)" );
        if ( re.exactMatch( formattedFieldType ) )
        {
          fieldSize = re.cap( 1 ).toInt();
        }
        else
        {
          fieldSize = -1;
        }
      }
      else if ( fieldTypeName == "date" )
      {
        fieldType = QVariant::Date;
        fieldSize = -1;
      }
      else if ( fieldTypeName == "time" )
      {
        fieldType = QVariant::Time;
        fieldSize = -1;
      }
      else if ( fieldTypeName == "timestamp" )
      {
        fieldType = QVariant::DateTime;
        fieldSize = -1;
      }
      else if ( fieldTypeName == "text" ||
                fieldTypeName == "bool" ||
                fieldTypeName == "geometry" ||
                fieldTypeName == "hstore" ||
                fieldTypeName == "inet" ||
                fieldTypeName == "money" ||
                fieldTypeName == "ltree" ||
                fieldTypeName == "uuid" ||
                fieldTypeName == "xml" ||
                fieldTypeName.startsWith( "time" ) ||
                fieldTypeName.startsWith( "date" ) )
      {
        fieldType = QVariant::String;
        fieldSize = -1;
      }
      else if ( fieldTypeName == "bpchar" )
      {
        // although postgres internally uses "bpchar", this is exposed to users as character in postgres
        fieldTypeName = "character";

        fieldType = QVariant::String;

        QRegExp re( "character\\((\\d+)\\)" );
        if ( re.exactMatch( formattedFieldType ) )
        {
          fieldSize = re.cap( 1 ).toInt();
        }
        else
        {
          QgsDebugMsg( QString( "unexpected formatted field type '%1' for field %2" )
                       .arg( formattedFieldType,
                             fieldName ) );
          fieldSize = -1;
          fieldPrec = -1;
        }
      }
      else if ( fieldTypeName == "char" )
      {
        fieldType = QVariant::String;

        QRegExp re( "char\\((\\d+)\\)" );
        if ( re.exactMatch( formattedFieldType ) )
        {
          fieldSize = re.cap( 1 ).toInt();
        }
        else
        {
          QgsMessageLog::logMessage( tr( "unexpected formatted field type '%1' for field %2" )
                                     .arg( formattedFieldType,
                                           fieldName ) );
          fieldSize = -1;
          fieldPrec = -1;
        }
      }
      else
      {
        QgsMessageLog::logMessage( tr( "Field %1 ignored, because of unsupported type %2" ).arg( fieldName, fieldTypeName ), tr( "PostGIS" ) );
        continue;
      }

      if ( isArray )
      {
        fieldTypeName = '_' + fieldTypeName;
        fieldType = QVariant::String;
        fieldSize = -1;
      }
    }
    else if ( fieldTType == "e" )
    {
      // enum
      fieldType = QVariant::String;
      fieldSize = -1;
    }
    else
    {
      QgsMessageLog::logMessage( tr( "Field %1 ignored, because of unsupported type %2" ).arg( fieldName, fieldTType ), tr( "PostGIS" ) );
      continue;
    }

    if ( fields.contains( fieldName ) )
    {
      QgsMessageLog::logMessage( tr( "Duplicate field %1 found\n" ).arg( fieldName ), tr( "PostGIS" ) );
      return false;
    }

    fields << fieldName;

    if ( isDomain )
    {
      //field was defined using domain, so use domain type name for fieldTypeName
      fieldTypeName = originalFormattedFieldType;
    }

    mAttrPalIndexName.insert( i, fieldName );
    mDefaultValues.insert( mAttributeFields.size(), defValMap[tableoid][attnum] );
    mAttributeFields.append( QgsField( fieldName, fieldType, fieldTypeName, fieldSize, fieldPrec, fieldComment ) );
  }

  return true;
}

bool QgsPostgresProvider::hasSufficientPermsAndCapabilities()
{
  QgsDebugMsg( "Checking for permissions on the relation" );

  QgsPostgresResult testAccess;
  if ( !mIsQuery )
  {
    // Check that we can read from the table (i.e., we have select permission).
    QString sql = QString( "SELECT * FROM %1 LIMIT 1" ).arg( mQuery );
    QgsPostgresResult testAccess( connectionRO()->PQexec( sql ) );
    if ( testAccess.PQresultStatus() != PGRES_TUPLES_OK )
    {
      QgsMessageLog::logMessage( tr( "Unable to access the %1 relation.\nThe error message from the database was:\n%2.\nSQL: %3" )
                                 .arg( mQuery,
                                       testAccess.PQresultErrorMessage(),
                                       sql ), tr( "PostGIS" ) );
      return false;
    }

    bool inRecovery = false;

    if ( connectionRO()->pgVersion() >= 90000 )
    {
      testAccess = connectionRO()->PQexec( "SELECT pg_is_in_recovery()" );
      if ( testAccess.PQresultStatus() != PGRES_TUPLES_OK || testAccess.PQgetvalue( 0, 0 ) == "t" )
      {
        QgsMessageLog::logMessage( tr( "PostgreSQL is still in recovery after a database crash\n(or you are connected to a (read-only) slave).\nWrite accesses will be denied." ), tr( "PostGIS" ) );
        inRecovery = true;
      }
    }

    // postgres has fast access to features at id (thanks to primary key / unique index)
    // the latter flag is here just for compatibility
    if ( !mSelectAtIdDisabled )
    {
      mEnabledCapabilities = QgsVectorDataProvider::SelectAtId | QgsVectorDataProvider::SelectGeometryAtId;
    }

    if ( !inRecovery )
    {
      if ( connectionRO()->pgVersion() >= 80400 )
      {
        sql = QString( "SELECT "
                       "has_table_privilege(%1,'DELETE'),"
                       "has_any_column_privilege(%1,'UPDATE'),"
                       "%2"
                       "has_table_privilege(%1,'INSERT'),"
                       "current_schema()" )
              .arg( quotedValue( mQuery ),
                    mGeometryColumn.isNull()
                    ? QString( "'f'," )
                    : QString( "has_column_privilege(%1,%2,'UPDATE')," )
                    .arg( quotedValue( mQuery ),
                          quotedValue( mGeometryColumn ) )
                  );
      }
      else
      {
        sql = QString( "SELECT "
                       "has_table_privilege(%1,'DELETE'),"
                       "has_table_privilege(%1,'UPDATE'),"
                       "has_table_privilege(%1,'UPDATE'),"
                       "has_table_privilege(%1,'INSERT'),"
                       "current_schema()" )
              .arg( quotedValue( mQuery ) );
      }

      testAccess = connectionRO()->PQexec( sql );
      if ( testAccess.PQresultStatus() != PGRES_TUPLES_OK )
      {
        QgsMessageLog::logMessage( tr( "Unable to determine table access privileges for the %1 relation.\nThe error message from the database was:\n%2.\nSQL: %3" )
                                   .arg( mQuery,
                                         testAccess.PQresultErrorMessage(),
                                         sql ),
                                   tr( "PostGIS" ) );
        return false;
      }


      if ( testAccess.PQgetvalue( 0, 0 ) == "t" )
      {
        // DELETE
        mEnabledCapabilities |= QgsVectorDataProvider::DeleteFeatures;
      }

      if ( testAccess.PQgetvalue( 0, 1 ) == "t" )
      {
        // UPDATE
        mEnabledCapabilities |= QgsVectorDataProvider::ChangeAttributeValues;
      }

      if ( testAccess.PQgetvalue( 0, 2 ) == "t" )
      {
        // UPDATE
        mEnabledCapabilities |= QgsVectorDataProvider::ChangeGeometries;
      }

      if ( testAccess.PQgetvalue( 0, 3 ) == "t" )
      {
        // INSERT
        mEnabledCapabilities |= QgsVectorDataProvider::AddFeatures;
      }

      if ( mSchemaName.isEmpty() )
        mSchemaName = testAccess.PQgetvalue( 0, 4 );

      sql = QString( "SELECT 1 FROM pg_class,pg_namespace WHERE "
                     "pg_class.relnamespace=pg_namespace.oid AND "
                     "%3 AND "
                     "relname=%1 AND nspname=%2" )
            .arg( quotedValue( mTableName ),
                  quotedValue( mSchemaName ),
                  connectionRO()->pgVersion() < 80100 ? "pg_get_userbyid(relowner)=current_user" : "pg_has_role(relowner,'MEMBER')" );
      testAccess = connectionRO()->PQexec( sql );
      if ( testAccess.PQresultStatus() == PGRES_TUPLES_OK && testAccess.PQntuples() == 1 )
      {
        mEnabledCapabilities |= QgsVectorDataProvider::AddAttributes | QgsVectorDataProvider::DeleteAttributes | QgsVectorDataProvider::RenameAttributes;
      }
    }
  }
  else
  {
    // Check if the sql is a select query
    if ( !mQuery.startsWith( '(' ) && !mQuery.endsWith( ')' ) )
    {
      QgsMessageLog::logMessage( tr( "The custom query is not a select query." ), tr( "PostGIS" ) );
      return false;
    }

    // get a new alias for the subquery
    int index = 0;
    QString alias;
    QRegExp regex;
    do
    {
      alias = QString( "subQuery_%1" ).arg( QString::number( index++ ) );
      QString pattern = QString( "(\\\"?)%1\\1" ).arg( QRegExp::escape( alias ) );
      regex.setPattern( pattern );
      regex.setCaseSensitivity( Qt::CaseInsensitive );
    }
    while ( mQuery.contains( regex ) );

    // convert the custom query into a subquery
    mQuery = QString( "%1 AS %2" )
             .arg( mQuery,
                   quotedIdentifier( alias ) );

    QString sql = QString( "SELECT * FROM %1 LIMIT 1" ).arg( mQuery );

    testAccess = connectionRO()->PQexec( sql );
    if ( testAccess.PQresultStatus() != PGRES_TUPLES_OK )
    {
      QgsMessageLog::logMessage( tr( "Unable to execute the query.\nThe error message from the database was:\n%1.\nSQL: %2" )
                                 .arg( testAccess.PQresultErrorMessage(),
                                       sql ), tr( "PostGIS" ) );
      return false;
    }

    if ( !mSelectAtIdDisabled )
    {
      mEnabledCapabilities = QgsVectorDataProvider::SelectAtId | QgsVectorDataProvider::SelectGeometryAtId;
    }
  }

  // supports geometry simplification on provider side
  mEnabledCapabilities |= ( QgsVectorDataProvider::SimplifyGeometries | QgsVectorDataProvider::SimplifyGeometriesWithTopologicalValidation );

  //supports transactions
  mEnabledCapabilities |= QgsVectorDataProvider::TransactionSupport;

  // supports circular geometries
  mEnabledCapabilities |= QgsVectorDataProvider::CircularGeometries;

  if (( mEnabledCapabilities & QgsVectorDataProvider::ChangeGeometries ) &&
      ( mEnabledCapabilities & QgsVectorDataProvider::ChangeAttributeValues ) &&
      mSpatialColType != sctTopoGeometry )
  {
    mEnabledCapabilities |= QgsVectorDataProvider::ChangeFeatures;
  }

  return true;
}

bool QgsPostgresProvider::determinePrimaryKey()
{
  if ( !loadFields() )
  {
    return false;
  }

  // check to see if there is an unique index on the relation, which
  // can be used as a key into the table. Primary keys are always
  // unique indices, so we catch them as well.

  QString sql;
  if ( !mIsQuery )
  {
    sql = QString( "SELECT count(*) FROM pg_inherits WHERE inhparent=%1::regclass" ).arg( quotedValue( mQuery ) );
    QgsDebugMsg( QString( "Checking whether %1 is a parent table" ).arg( sql ) );
    QgsPostgresResult res( connectionRO()->PQexec( sql ) );
    bool isParentTable( res.PQntuples() == 0 || res.PQgetvalue( 0, 0 ).toInt() > 0 );

    sql = QString( "SELECT indexrelid FROM pg_index WHERE indrelid=%1::regclass AND (indisprimary OR indisunique) ORDER BY CASE WHEN indisprimary THEN 1 ELSE 2 END LIMIT 1" ).arg( quotedValue( mQuery ) );
    QgsDebugMsg( QString( "Retrieving first primary or unique index: %1" ).arg( sql ) );

    res = connectionRO()->PQexec( sql );
    QgsDebugMsg( QString( "Got %1 rows." ).arg( res.PQntuples() ) );

    QStringList log;

    // no primary or unique indizes found
    if ( res.PQntuples() == 0 )
    {
      QgsDebugMsg( "Relation has no primary key -- investigating alternatives" );

      // Two options here. If the relation is a table, see if there is
      // an oid column that can be used instead.
      // If the relation is a view try to find a suitable column to use as
      // the primary key.

      sql = QString( "SELECT relkind FROM pg_class WHERE oid=regclass(%1)::oid" ).arg( quotedValue( mQuery ) );
      res = connectionRO()->PQexec( sql );
      QString type = res.PQgetvalue( 0, 0 );

      if ( type == "r" ) // the relation is a table
      {
        QgsDebugMsg( "Relation is a table. Checking to see if it has an oid column." );

        mPrimaryKeyAttrs.clear();

        // If there is an oid on the table, use that instead,
        sql = QString( "SELECT attname FROM pg_attribute WHERE attname='oid' AND attrelid=regclass(%1)" ).arg( quotedValue( mQuery ) );

        res = connectionRO()->PQexec( sql );
        if ( res.PQntuples() == 1 )
        {
          // Could warn the user here that performance will suffer if
          // oid isn't indexed (and that they may want to add a
          // primary key to the table)
          mPrimaryKeyType = pktOid;
        }
        else
        {
          sql = QString( "SELECT attname FROM pg_attribute WHERE attname='ctid' AND attrelid=regclass(%1)" ).arg( quotedValue( mQuery ) );

          res = connectionRO()->PQexec( sql );
          if ( res.PQntuples() == 1 )
          {
            mPrimaryKeyType = pktTid;

            QgsMessageLog::logMessage( tr( "Primary key is ctid - changing of existing features disabled (%1; %2)" ).arg( mGeometryColumn, mQuery ) );
            mEnabledCapabilities &= ~( QgsVectorDataProvider::DeleteFeatures | QgsVectorDataProvider::ChangeAttributeValues | QgsVectorDataProvider::ChangeGeometries | QgsVectorDataProvider::ChangeFeatures );
          }
          else
          {
            QgsMessageLog::logMessage( tr( "The table has no column suitable for use as a key. QGIS requires a primary key, a PostgreSQL oid column or a ctid for tables." ), tr( "PostGIS" ) );
          }
        }
      }
      else if ( type == "v" || type == "m" ) // the relation is a view
      {
        determinePrimaryKeyFromUriKeyColumn();
      }
      else
      {
        QgsMessageLog::logMessage( tr( "Unexpected relation type '%1'." ).arg( type ), tr( "PostGIS" ) );
      }
    }
    else
    {
      // have a primary key or unique index
      QString indrelid = res.PQgetvalue( 0, 0 );
      sql = QString( "SELECT attname,attnotnull FROM pg_index,pg_attribute WHERE indexrelid=%1 AND indrelid=attrelid AND pg_attribute.attnum=any(pg_index.indkey)" ).arg( indrelid );

      QgsDebugMsg( "Retrieving key columns: " + sql );
      res = connectionRO()->PQexec( sql );
      QgsDebugMsg( QString( "Got %1 rows." ).arg( res.PQntuples() ) );

      bool mightBeNull = false;
      QString primaryKey;
      QString delim = "";

      mPrimaryKeyType = pktFidMap; // map by default, will downgrade if needed
      for ( int i = 0; i < res.PQntuples(); i++ )
      {
        QString name = res.PQgetvalue( i, 0 );
        if ( res.PQgetvalue( i, 1 ).startsWith( 'f' ) )
        {
          QgsMessageLog::logMessage( tr( "Unique column '%1' doesn't have a NOT NULL constraint." ).arg( name ), tr( "PostGIS" ) );
          mightBeNull = true;
        }

        primaryKey += delim + quotedIdentifier( name );
        delim = ',';

        int idx = fieldNameIndex( name );
        if ( idx == -1 )
        {
          QgsDebugMsg( "Skipping " + name );
          continue;
        }
        const QgsField& fld = mAttributeFields.at( idx );

        // Always use pktFidMap for multi-field keys
        mPrimaryKeyType = i ? pktFidMap : pkType( fld );

        mPrimaryKeyAttrs << idx;
      }

      if (( mightBeNull || isParentTable ) && !mUseEstimatedMetadata && !uniqueData( primaryKey ) )
      {
        QgsMessageLog::logMessage( tr( "Ignoring key candidate because of NULL values or inheritance" ), tr( "PostGIS" ) );
        mPrimaryKeyType = pktUnknown;
        mPrimaryKeyAttrs.clear();
      }
    }
  }
  else
  {
    determinePrimaryKeyFromUriKeyColumn();
  }

  mValid = mPrimaryKeyType != pktUnknown;

  return mValid;
}

void QgsPostgresProvider::determinePrimaryKeyFromUriKeyColumn()
{
  QString primaryKey = mUri.keyColumn();
  mPrimaryKeyType = pktUnknown;

  if ( !primaryKey.isEmpty() )
  {
    QStringList cols;

    // remove quotes from key list
    if ( primaryKey.startsWith( '"' ) && primaryKey.endsWith( '"' ) )
    {
      int i = 1;
      QString col;
      while ( i < primaryKey.size() )
      {
        if ( primaryKey[i] == '"' )
        {
          if ( i + 1 < primaryKey.size() && primaryKey[i+1] == '"' )
          {
            i++;
          }
          else
          {
            cols << col;
            col = "";

            if ( ++i == primaryKey.size() )
              break;

            Q_ASSERT( primaryKey[i] == ',' );
            i++;
            Q_ASSERT( primaryKey[i] == '"' );
            i++;
            col = "";
            continue;
          }
        }

        col += primaryKey[i++];
      }
    }
    else if ( primaryKey.contains( ',' ) )
    {
      cols = primaryKey.split( ',' );
    }
    else
    {
      cols << primaryKey;
      primaryKey = quotedIdentifier( primaryKey );
    }

    Q_FOREACH ( const QString& col, cols )
    {
      int idx = fieldNameIndex( col );
      if ( idx < 0 )
      {
        QgsMessageLog::logMessage( tr( "Key field '%1' for view/query not found." ).arg( col ), tr( "PostGIS" ) );
        mPrimaryKeyAttrs.clear();
        break;
      }

      mPrimaryKeyAttrs << idx;
    }

    if ( !mPrimaryKeyAttrs.isEmpty() )
    {
      if ( mUseEstimatedMetadata || uniqueData( primaryKey ) )
      {
        mPrimaryKeyType = pktFidMap; // Map by default
        if ( mPrimaryKeyAttrs.size() == 1 )
        {
          const QgsField& fld = mAttributeFields.at( 0 );
          mPrimaryKeyType = pkType( fld );
        }
      }
      else
      {
        QgsMessageLog::logMessage( tr( "Primary key field '%1' for view/query not unique." ).arg( primaryKey ), tr( "PostGIS" ) );
      }
    }
    else
    {
      QgsMessageLog::logMessage( tr( "Keys for view/query undefined." ), tr( "PostGIS" ) );
    }
  }
  else
  {
    QgsMessageLog::logMessage( tr( "No key field for view/query given." ), tr( "PostGIS" ) );
  }
}

bool QgsPostgresProvider::uniqueData( const QString& quotedColNames )
{
  // Check to see if the given columns contain unique data
  QString sql = QString( "SELECT count(distinct (%1))=count((%1)) FROM %2%3" )
                .arg( quotedColNames,
                      mQuery,
                      filterWhereClause() );

  QgsPostgresResult unique( connectionRO()->PQexec( sql ) );

  if ( unique.PQresultStatus() != PGRES_TUPLES_OK )
  {
    pushError( unique.PQresultErrorMessage() );
    return false;
  }

  return unique.PQntuples() == 1 && unique.PQgetvalue( 0, 0 ).startsWith( 't' );
}

// Returns the minimum value of an attribute
QVariant QgsPostgresProvider::minimumValue( int index )
{
  try
  {
    // get the field name
    const QgsField &fld = field( index );
    QString sql = QString( "SELECT min(%1) AS %1 FROM %2" )
                  .arg( quotedIdentifier( fld.name() ),
                        mQuery );

    if ( !mSqlWhereClause.isEmpty() )
    {
      sql += QString( " WHERE %1" ).arg( mSqlWhereClause );
    }

    sql = QString( "SELECT %1 FROM (%2) foo" ).arg( connectionRO()->fieldExpression( fld ), sql );

    QgsPostgresResult rmin( connectionRO()->PQexec( sql ) );
    return convertValue( fld.type(), rmin.PQgetvalue( 0, 0 ) );
  }
  catch ( PGFieldNotFound )
  {
    return QVariant( QString::null );
  }
}

// Returns the list of unique values of an attribute
void QgsPostgresProvider::uniqueValues( int index, QList<QVariant> &uniqueValues, int limit )
{
  uniqueValues.clear();

  try
  {
    // get the field name
    const QgsField &fld = field( index );
    QString sql = QString( "SELECT DISTINCT %1 FROM %2" )
                  .arg( quotedIdentifier( fld.name() ),
                        mQuery );

    if ( !mSqlWhereClause.isEmpty() )
    {
      sql += QString( " WHERE %1" ).arg( mSqlWhereClause );
    }

    sql +=  QString( " ORDER BY %1" ).arg( quotedIdentifier( fld.name() ) );

    if ( limit >= 0 )
    {
      sql += QString( " LIMIT %1" ).arg( limit );
    }

    sql = QString( "SELECT %1 FROM (%2) foo" ).arg( connectionRO()->fieldExpression( fld ), sql );

    QgsPostgresResult res( connectionRO()->PQexec( sql ) );
    if ( res.PQresultStatus() == PGRES_TUPLES_OK )
    {
      for ( int i = 0; i < res.PQntuples(); i++ )
        uniqueValues.append( convertValue( fld.type(), res.PQgetvalue( i, 0 ) ) );
    }
  }
  catch ( PGFieldNotFound )
  {
  }
}

void QgsPostgresProvider::enumValues( int index, QStringList& enumList )
{
  enumList.clear();

  if ( index < 0 || index >= mAttributeFields.count() )
    return;

  //find out type of index
  QString fieldName = mAttributeFields.at( index ).name();
  QString typeName = mAttributeFields.at( index ).typeName();

  //is type an enum?
  QString typeSql = QString( "SELECT typtype FROM pg_type WHERE typname=%1" ).arg( quotedValue( typeName ) );
  QgsPostgresResult typeRes( connectionRO()->PQexec( typeSql ) );
  if ( typeRes.PQresultStatus() != PGRES_TUPLES_OK || typeRes.PQntuples() < 1 )
  {
    return;
  }


  QString typtype = typeRes.PQgetvalue( 0, 0 );
  if ( typtype.compare( "e", Qt::CaseInsensitive ) == 0 )
  {
    //try to read enum_range of attribute
    if ( !parseEnumRange( enumList, fieldName ) )
    {
      enumList.clear();
    }
  }
  else
  {
    //is there a domain check constraint for the attribute?
    if ( !parseDomainCheckConstraint( enumList, fieldName ) )
    {
      enumList.clear();
    }
  }
}

bool QgsPostgresProvider::parseEnumRange( QStringList& enumValues, const QString& attributeName ) const
{
  enumValues.clear();

  QString enumRangeSql = QString( "SELECT enumlabel FROM pg_catalog.pg_enum WHERE enumtypid=(SELECT atttypid::regclass FROM pg_attribute WHERE attrelid=%1::regclass AND attname=%2)" )
                         .arg( quotedValue( mQuery ),
                               quotedValue( attributeName ) );
  QgsPostgresResult enumRangeRes( connectionRO()->PQexec( enumRangeSql ) );
  if ( enumRangeRes.PQresultStatus() != PGRES_TUPLES_OK )
    return false;

  for ( int i = 0; i < enumRangeRes.PQntuples(); i++ )
  {
    enumValues << enumRangeRes.PQgetvalue( i, 0 );
  }

  return true;
}

bool QgsPostgresProvider::parseDomainCheckConstraint( QStringList& enumValues, const QString& attributeName ) const
{
  enumValues.clear();

  //is it a domain type with a check constraint?
  QString domainSql = QString( "SELECT domain_name FROM information_schema.columns WHERE table_name=%1 AND column_name=%2" ).arg( quotedValue( mTableName ), quotedValue( attributeName ) );
  QgsPostgresResult domainResult( connectionRO()->PQexec( domainSql ) );
  if ( domainResult.PQresultStatus() == PGRES_TUPLES_OK && domainResult.PQntuples() > 0 )
  {
    //a domain type
    QString domainCheckDefinitionSql = QString( "SELECT consrc FROM pg_constraint WHERE conname=(SELECT constraint_name FROM information_schema.domain_constraints WHERE domain_name=%1)" ).arg( quotedValue( domainResult.PQgetvalue( 0, 0 ) ) );
    QgsPostgresResult domainCheckRes( connectionRO()->PQexec( domainCheckDefinitionSql ) );
    if ( domainCheckRes.PQresultStatus() == PGRES_TUPLES_OK && domainCheckRes.PQntuples() > 0 )
    {
      QString checkDefinition = domainCheckRes.PQgetvalue( 0, 0 );

      //we assume that the constraint is of the following form:
      //(VALUE = ANY (ARRAY['a'::text, 'b'::text, 'c'::text, 'd'::text]))
      //normally, postgresql creates that if the contstraint has been specified as 'VALUE in ('a', 'b', 'c', 'd')

      int anyPos = checkDefinition.indexOf( QRegExp( "VALUE\\s*=\\s*ANY\\s*\\(\\s*ARRAY\\s*\\[" ) );
      int arrayPosition = checkDefinition.lastIndexOf( "ARRAY[" );
      int closingBracketPos = checkDefinition.indexOf( ']', arrayPosition + 6 );

      if ( anyPos == -1 || anyPos >= arrayPosition )
      {
        return false; //constraint has not the required format
      }

      if ( arrayPosition != -1 )
      {
        QString valueList = checkDefinition.mid( arrayPosition + 6, closingBracketPos );
        QStringList commaSeparation = valueList.split( ',', QString::SkipEmptyParts );
        QStringList::const_iterator cIt = commaSeparation.constBegin();
        for ( ; cIt != commaSeparation.constEnd(); ++cIt )
        {
          //get string between ''
          int beginQuotePos = cIt->indexOf( '\'' );
          int endQuotePos = cIt->lastIndexOf( '\'' );
          if ( beginQuotePos != -1 && ( endQuotePos - beginQuotePos ) > 1 )
          {
            enumValues << cIt->mid( beginQuotePos + 1, endQuotePos - beginQuotePos - 1 );
          }
        }
      }
      return true;
    }
  }
  return false;
}

// Returns the maximum value of an attribute
QVariant QgsPostgresProvider::maximumValue( int index )
{
  try
  {
    // get the field name
    const QgsField &fld = field( index );
    QString sql = QString( "SELECT max(%1) AS %1 FROM %2" )
                  .arg( quotedIdentifier( fld.name() ),
                        mQuery );

    if ( !mSqlWhereClause.isEmpty() )
    {
      sql += QString( " WHERE %1" ).arg( mSqlWhereClause );
    }

    sql = QString( "SELECT %1 FROM (%2) foo" ).arg( connectionRO()->fieldExpression( fld ), sql );

    QgsPostgresResult rmax( connectionRO()->PQexec( sql ) );

    return convertValue( fld.type(), rmax.PQgetvalue( 0, 0 ) );
  }
  catch ( PGFieldNotFound )
  {
    return QVariant( QString::null );
  }
}


bool QgsPostgresProvider::isValid()
{
  return mValid;
}

QVariant QgsPostgresProvider::defaultValue( int fieldId )
{
  QVariant defVal = mDefaultValues.value( fieldId, QString::null );

  if ( providerProperty( EvaluateDefaultValues, false ).toBool() && !defVal.isNull() )
  {
    const QgsField& fld = field( fieldId );

    QgsPostgresResult res( connectionRO()->PQexec( QString( "SELECT %1" ).arg( defVal.toString() ) ) );

    return convertValue( fld.type(), res.PQgetvalue( 0, 0 ) );
  }

  return defVal;
}

QString QgsPostgresProvider::paramValue( const QString& fieldValue, const QString &defaultValue ) const
{
  if ( fieldValue.isNull() )
    return QString::null;

  if ( fieldValue == defaultValue && !defaultValue.isNull() )
  {
    QgsPostgresResult result( connectionRO()->PQexec( QString( "SELECT %1" ).arg( defaultValue ) ) );
    if ( result.PQresultStatus() != PGRES_TUPLES_OK )
      throw PGException( result );

    return result.PQgetvalue( 0, 0 );
  }

  return fieldValue;
}


/* private */
bool QgsPostgresProvider::getTopoLayerInfo()
{
  QString sql = QString( "SELECT t.name, l.layer_id "
                         "FROM topology.layer l, topology.topology t "
                         "WHERE l.topology_id = t.id AND l.schema_name=%1 "
                         "AND l.table_name=%2 AND l.feature_column=%3" )
                .arg( quotedValue( mSchemaName ),
                      quotedValue( mTableName ),
                      quotedValue( mGeometryColumn ) );
  QgsPostgresResult result( connectionRO()->PQexec( sql ) );
  if ( result.PQresultStatus() != PGRES_TUPLES_OK )
  {
    throw PGException( result ); // we should probably not do this
  }
  if ( result.PQntuples() < 1 )
  {
    QgsMessageLog::logMessage( tr( "Could not find topology of layer %1.%2.%3" )
                               .arg( quotedValue( mSchemaName ),
                                     quotedValue( mTableName ),
                                     quotedValue( mGeometryColumn ) ),
                               tr( "PostGIS" ) );
    return false;
  }
  mTopoLayerInfo.topologyName = result.PQgetvalue( 0, 0 );
  mTopoLayerInfo.layerId = result.PQgetvalue( 0, 1 ).toLong();
  return true;
}

/* private */
void QgsPostgresProvider::dropOrphanedTopoGeoms()
{
  QString sql = QString( "DELETE FROM %1.relation WHERE layer_id = %2 AND "
                         "topogeo_id NOT IN ( SELECT id(%3) FROM %4.%5 )" )
                .arg( quotedIdentifier( mTopoLayerInfo.topologyName ) )
                .arg( mTopoLayerInfo.layerId )
                .arg( quotedIdentifier( mGeometryColumn ),
                      quotedIdentifier( mSchemaName ),
                      quotedIdentifier( mTableName ) )
                ;

  QgsDebugMsg( "TopoGeom orphans cleanup query: " + sql );

  connectionRW()->PQexecNR( sql );
}

QString QgsPostgresProvider::geomParam( int offset ) const
{
  QString geometry;

  bool forceMulti = false;

  if ( mSpatialColType != sctTopoGeometry )
  {
    switch ( geometryType() )
    {
      case QGis::WKBPoint:
      case QGis::WKBLineString:
      case QGis::WKBPolygon:
      case QGis::WKBPoint25D:
      case QGis::WKBLineString25D:
      case QGis::WKBPolygon25D:
      case QGis::WKBUnknown:
      case QGis::WKBNoGeometry:
        forceMulti = false;
        break;

      case QGis::WKBMultiPoint:
      case QGis::WKBMultiLineString:
      case QGis::WKBMultiPolygon:
      case QGis::WKBMultiPoint25D:
      case QGis::WKBMultiLineString25D:
      case QGis::WKBMultiPolygon25D:
        forceMulti = true;
        break;
    }
  }

  if ( mSpatialColType == sctTopoGeometry )
  {
    geometry += QString( "toTopoGeom(" );
  }

  if ( forceMulti )
  {
    geometry += connectionRO()->majorVersion() < 2 ? "multi(" : "st_multi(";
  }

  geometry += QString( "%1($%2%3,%4)" )
              .arg( connectionRO()->majorVersion() < 2 ? "geomfromwkb" : "st_geomfromwkb" )
              .arg( offset )
              .arg( connectionRO()->useWkbHex() ? "" : "::bytea",
                    mRequestedSrid.isEmpty() ? mDetectedSrid : mRequestedSrid );

  if ( forceMulti )
  {
    geometry += ')';
  }

  if ( mSpatialColType == sctTopoGeometry )
  {
    geometry += QString( ",%1,%2)" )
                .arg( quotedValue( mTopoLayerInfo.topologyName ) )
                .arg( mTopoLayerInfo.layerId );
  }

  return geometry;
}

bool QgsPostgresProvider::addFeatures( QgsFeatureList &flist )
{
  if ( flist.isEmpty() )
    return true;

  if ( mIsQuery )
    return false;

  QgsPostgresConn* conn = connectionRW();
  if ( !conn )
  {
    return false;
  }
  conn->lock();

  bool returnvalue = true;

  try
  {
    conn->begin();

    // Prepare the INSERT statement
    QString insert = QString( "INSERT INTO %1(" ).arg( mQuery );
    QString values = ") VALUES (";
    QString delim = "";
    int offset = 1;

    QStringList defaultValues;
    QList<int> fieldId;

    if ( !mGeometryColumn.isNull() )
    {
      insert += quotedIdentifier( mGeometryColumn );

      values += geomParam( offset++ );

      delim = ',';
    }

    if ( mPrimaryKeyType == pktInt || mPrimaryKeyType == pktFidMap || mPrimaryKeyType == pktUint64 )
    {
      Q_FOREACH ( int idx, mPrimaryKeyAttrs )
      {
        insert += delim + quotedIdentifier( field( idx ).name() );
        values += delim + QString( "$%1" ).arg( defaultValues.size() + offset );
        delim = ',';
        fieldId << idx;
        defaultValues << defaultValue( idx ).toString();
      }
    }

    QgsAttributes attributevec = flist[0].attributes();

    // look for unique attribute values to place in statement instead of passing as parameter
    // e.g. for defaults
    for ( int idx = 0; idx < attributevec.count(); ++idx )
    {
      QVariant v = attributevec.at( idx );
      if ( fieldId.contains( idx ) )
        continue;

      if ( idx >= mAttributeFields.count() )
        continue;

      QString fieldname = mAttributeFields.at( idx ).name();
      QString fieldTypeName = mAttributeFields.at( idx ).typeName();

      QgsDebugMsg( "Checking field against: " + fieldname );

      if ( fieldname.isEmpty() || fieldname == mGeometryColumn )
        continue;

      int i;
      for ( i = 1; i < flist.size(); i++ )
      {
        QgsAttributes attrs2 = flist[i].attributes();
        QVariant v2 = attrs2.at( idx );

        if ( v2 != v )
          break;
      }

      insert += delim + quotedIdentifier( fieldname );

      QString defVal = defaultValue( idx ).toString();

      if ( i == flist.size() )
      {
        if ( v == defVal )
        {
          if ( defVal.isNull() )
          {
            values += delim + "NULL";
          }
          else
          {
            values += delim + defVal;
          }
        }
        else if ( fieldTypeName == "geometry" )
        {
          values += QString( "%1%2(%3)" )
                    .arg( delim,
                          connectionRO()->majorVersion() < 2 ? "geomfromewkt" : "st_geomfromewkt",
                          quotedValue( v.toString() ) );
        }
        else if ( fieldTypeName == "geography" )
        {
          values += QString( "%1st_geographyfromewkt(%2)" )
                    .arg( delim,
                          quotedValue( v.toString() ) );
        }
        else
        {
          values += delim + quotedValue( v );
        }
      }
      else
      {
        // value is not unique => add parameter
        if ( fieldTypeName == "geometry" )
        {
          values += QString( "%1%2($%3)" )
                    .arg( delim,
                          connectionRO()->majorVersion() < 2 ? "geomfromewkt" : "st_geomfromewkt" )
                    .arg( defaultValues.size() + offset );
        }
        else if ( fieldTypeName == "geography" )
        {
          values += QString( "%1st_geographyfromewkt($%2)" )
                    .arg( delim )
                    .arg( defaultValues.size() + offset );
        }
        else
        {
          values += QString( "%1$%2" )
                    .arg( delim )
                    .arg( defaultValues.size() + offset );
        }
        defaultValues.append( defVal );
        fieldId.append( idx );
      }

      delim = ',';
    }

    insert += values + ')';

    if ( mPrimaryKeyType == pktFidMap || mPrimaryKeyType == pktInt || mPrimaryKeyType == pktUint64 )
    {
      insert += " RETURNING ";

      QString delim;
      Q_FOREACH ( int idx, mPrimaryKeyAttrs )
      {
        insert += delim + quotedIdentifier( mAttributeFields.at( idx ).name() );
        delim = ',';
      }
    }

    QgsDebugMsg( QString( "prepare addfeatures: %1" ).arg( insert ) );
    QgsPostgresResult stmt( conn->PQprepare( "addfeatures", insert, fieldId.size() + offset - 1, nullptr ) );

    if ( stmt.PQresultStatus() != PGRES_COMMAND_OK )
      throw PGException( stmt );

    for ( QgsFeatureList::iterator features = flist.begin(); features != flist.end(); ++features )
    {
      QgsAttributes attrs = features->attributes();

      QStringList params;
      if ( !mGeometryColumn.isNull() )
      {
        appendGeomParam( features->constGeometry(), params );
      }

      params.reserve( fieldId.size() );
      for ( int i = 0; i < fieldId.size(); i++ )
      {
        int attrIdx = fieldId[i];
        QVariant value = attrs.at( attrIdx );

        QString v;
        if ( value.isNull() )
        {
          const QgsField &fld = field( attrIdx );
          v = paramValue( defaultValues[ i ], defaultValues[ i ] );
          features->setAttribute( attrIdx, convertValue( fld.type(), v ) );
        }
        else
        {
          v = paramValue( value.toString(), defaultValues[ i ] );

          if ( v != value.toString() )
          {
            const QgsField &fld = field( attrIdx );
            features->setAttribute( attrIdx, convertValue( fld.type(), v ) );
          }
        }

        params << v;
      }

      QgsPostgresResult result( conn->PQexecPrepared( "addfeatures", params ) );

      if ( result.PQresultStatus() == PGRES_TUPLES_OK )
      {
        for ( int i = 0; i < mPrimaryKeyAttrs.size(); ++i )
        {
          int idx = mPrimaryKeyAttrs.at( i );
          features->setAttribute( idx, convertValue( mAttributeFields.at( idx ).type(), result.PQgetvalue( 0, i ) ) );
        }
      }
      else if ( result.PQresultStatus() != PGRES_COMMAND_OK )
        throw PGException( result );

      if ( mPrimaryKeyType == pktOid )
      {
        features->setFeatureId( result.PQoidValue() );
        QgsDebugMsgLevel( QString( "new fid=%1" ).arg( features->id() ), 4 );
      }
    }

    // update feature ids
    if ( mPrimaryKeyType == pktInt || mPrimaryKeyType == pktFidMap || mPrimaryKeyType == pktUint64 )
    {
      for ( QgsFeatureList::iterator features = flist.begin(); features != flist.end(); ++features )
      {
        QgsAttributes attrs = features->attributes();

        if ( mPrimaryKeyType == pktUint64 )
        {
          features->setFeatureId( STRING_TO_FID( attrs.at( mPrimaryKeyAttrs.at( 0 ) ) ) );
        }
        else if ( mPrimaryKeyType == pktInt )
        {
          features->setFeatureId( PKINT2FID( STRING_TO_FID( attrs.at( mPrimaryKeyAttrs.at( 0 ) ) ) ) );
        }
        else
        {
          QList<QVariant> primaryKeyVals;

          Q_FOREACH ( int idx, mPrimaryKeyAttrs )
          {
            primaryKeyVals << attrs.at( idx );
          }

          features->setFeatureId( mShared->lookupFid( QVariant( primaryKeyVals ) ) );
        }
        QgsDebugMsgLevel( QString( "new fid=%1" ).arg( features->id() ), 4 );
      }
    }

    conn->PQexecNR( "DEALLOCATE addfeatures" );

    returnvalue &= conn->commit();

    mShared->addFeaturesCounted( flist.size() );
  }
  catch ( PGException &e )
  {
    pushError( tr( "PostGIS error while adding features: %1" ).arg( e.errorMessage() ) );
    conn->rollback();
    conn->PQexecNR( "DEALLOCATE addfeatures" );
    returnvalue = false;
  }

  conn->unlock();
  return returnvalue;
}

bool QgsPostgresProvider::deleteFeatures( const QgsFeatureIds & id )
{
  bool returnvalue = true;

  if ( mIsQuery )
    return false;

  QgsPostgresConn* conn = connectionRW();
  if ( !conn )
  {
    return false;
  }
  conn->lock();

  try
  {
    conn->begin();

    for ( QgsFeatureIds::const_iterator it = id.begin(); it != id.end(); ++it )
    {
      QString sql = QString( "DELETE FROM %1 WHERE %2" )
                    .arg( mQuery, whereClause( *it ) );
      QgsDebugMsg( "delete sql: " + sql );

      //send DELETE statement and do error handling
      QgsPostgresResult result( conn->PQexec( sql ) );
      if ( result.PQresultStatus() != PGRES_COMMAND_OK && result.PQresultStatus() != PGRES_TUPLES_OK )
        throw PGException( result );

      mShared->removeFid( *it );
    }

    returnvalue &= conn->commit();

    if ( mSpatialColType == sctTopoGeometry )
    {
      // NOTE: in presence of multiple TopoGeometry objects
      //       for the same table or when deleting a Geometry
      //       layer _also_ having a TopoGeometry component,
      //       orphans would still be left.
      // TODO: decouple layer from table and signal table when
      //       records are added or removed
      dropOrphanedTopoGeoms();
    }

    mShared->addFeaturesCounted( -id.size() );
  }
  catch ( PGException &e )
  {
    pushError( tr( "PostGIS error while deleting features: %1" ).arg( e.errorMessage() ) );
    conn->rollback();
    returnvalue = false;
  }

  conn->unlock();
  return returnvalue;
}

bool QgsPostgresProvider::addAttributes( const QList<QgsField> &attributes )
{
  bool returnvalue = true;

  if ( mIsQuery )
    return false;

  if ( attributes.isEmpty() )
    return true;

  QgsPostgresConn* conn = connectionRW();
  if ( !conn )
  {
    return false;
  }
  conn->lock();

  try
  {
    conn->begin();

    QString delim = "";
    QString sql = QString( "ALTER TABLE %1 " ).arg( mQuery );
    for ( QList<QgsField>::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter )
    {
      QString type = iter->typeName();
      if ( type == "char" || type == "varchar" )
      {
        if ( iter->length() > 0 )
          type = QString( "%1(%2)" ).arg( type ).arg( iter->length() );
      }
      else if ( type == "numeric" || type == "decimal" )
      {
        if ( iter->length() > 0 && iter->precision() >= 0 )
          type = QString( "%1(%2,%3)" ).arg( type ).arg( iter->length() ).arg( iter->precision() );
      }
      sql.append( QString( "%1ADD COLUMN %2 %3" ).arg( delim, quotedIdentifier( iter->name() ), type ) );
      delim = ',';
    }

    //send sql statement and do error handling
    QgsPostgresResult result( conn->PQexec( sql ) );
    if ( result.PQresultStatus() != PGRES_COMMAND_OK )
      throw PGException( result );

    for ( QList<QgsField>::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter )
    {
      if ( !iter->comment().isEmpty() )
      {
        sql = QString( "COMMENT ON COLUMN %1.%2 IS %3" )
              .arg( mQuery,
                    quotedIdentifier( iter->name() ),
                    quotedValue( iter->comment() ) );
        result = conn->PQexec( sql );
        if ( result.PQresultStatus() != PGRES_COMMAND_OK )
          throw PGException( result );
      }
    }

    returnvalue &= conn->commit();
  }
  catch ( PGException &e )
  {
    pushError( tr( "PostGIS error while adding attributes: %1" ).arg( e.errorMessage() ) );
    conn->rollback();
    returnvalue = false;
  }

  loadFields();
  conn->unlock();
  return returnvalue;
}

bool QgsPostgresProvider::deleteAttributes( const QgsAttributeIds& ids )
{
  bool returnvalue = true;

  if ( mIsQuery )
    return false;

  QgsPostgresConn* conn = connectionRW();
  if ( !conn )
  {
    return false;
  }
  conn->lock();

  try
  {
    conn->begin();

    QList<int> idsList = ids.values();
    qSort( idsList.begin(), idsList.end(), qGreater<int>() );

    for ( QList<int>::const_iterator iter = idsList.begin(); iter != idsList.end(); ++iter )
    {
      int index = *iter;
      if ( index < 0 || index >= mAttributeFields.count() )
        continue;

      QString column = mAttributeFields.at( index ).name();
      QString sql = QString( "ALTER TABLE %1 DROP COLUMN %2" )
                    .arg( mQuery,
                          quotedIdentifier( column ) );

      //send sql statement and do error handling
      QgsPostgresResult result( conn->PQexec( sql ) );
      if ( result.PQresultStatus() != PGRES_COMMAND_OK )
        throw PGException( result );

      //delete the attribute from mAttributeFields
      mAttributeFields.remove( index );
    }

    returnvalue &= conn->commit();
  }
  catch ( PGException &e )
  {
    pushError( tr( "PostGIS error while deleting attributes: %1" ).arg( e.errorMessage() ) );
    conn->rollback();
    returnvalue = false;
  }

  loadFields();
  conn->unlock();
  return returnvalue;
}

bool QgsPostgresProvider::renameAttributes( const QgsFieldNameMap& renamedAttributes )
{
  if ( mIsQuery )
    return false;


  QString sql = "BEGIN;";

  QgsFieldNameMap::const_iterator renameIt = renamedAttributes.constBegin();
  bool returnvalue = true;
  for ( ; renameIt != renamedAttributes.constEnd(); ++renameIt )
  {
    int fieldIndex = renameIt.key();
    if ( fieldIndex < 0 || fieldIndex >= mAttributeFields.count() )
    {
      pushError( tr( "Invalid attribute index: %1" ).arg( fieldIndex ) );
      return false;
    }
    if ( mAttributeFields.indexFromName( renameIt.value() ) >= 0 )
    {
      //field name already in use
      pushError( tr( "Error renaming field %1: name '%2' already exists" ).arg( fieldIndex ).arg( renameIt.value() ) );
      return false;
    }

    sql += QString( "ALTER TABLE %1 RENAME COLUMN %2 TO %3;" )
           .arg( mQuery,
                 quotedIdentifier( mAttributeFields.at( fieldIndex ).name() ),
                 quotedIdentifier( renameIt.value() ) );
  }
  sql += "COMMIT;";

  QgsPostgresConn* conn = connectionRW();
  if ( !conn )
  {
    return false;
  }
  conn->lock();

  try
  {
    conn->begin();
    //send sql statement and do error handling
    QgsPostgresResult result( conn->PQexec( sql ) );
    if ( result.PQresultStatus() != PGRES_COMMAND_OK )
      throw PGException( result );
    returnvalue = conn->commit();
  }
  catch ( PGException &e )
  {
    pushError( tr( "PostGIS error while renaming attributes: %1" ).arg( e.errorMessage() ) );
    conn->rollback();
    returnvalue = false;
  }

  loadFields();
  conn->unlock();
  return returnvalue;
}

bool QgsPostgresProvider::changeAttributeValues( const QgsChangedAttributesMap &attr_map )
{
  bool returnvalue = true;

  if ( mIsQuery )
    return false;

  if ( attr_map.isEmpty() )
    return true;

  QgsPostgresConn *conn = connectionRW();
  if ( !conn )
    return false;

  conn->lock();

  try
  {
    conn->begin();

    // cycle through the features
    for ( QgsChangedAttributesMap::const_iterator iter = attr_map.constBegin(); iter != attr_map.constEnd(); ++iter )
    {
      QgsFeatureId fid = iter.key();

      // skip added features
      if ( FID_IS_NEW( fid ) )
        continue;

      const QgsAttributeMap &attrs = iter.value();
      if ( attrs.isEmpty() )
        continue;

      QString sql = QString( "UPDATE %1 SET " ).arg( mQuery );

      bool pkChanged = false;

      // cycle through the changed attributes of the feature
      QString delim;
      for ( QgsAttributeMap::const_iterator siter = attrs.constBegin(); siter != attrs.constEnd(); ++siter )
      {
        try
        {
          QgsField fld = field( siter.key() );

          pkChanged = pkChanged || mPrimaryKeyAttrs.contains( siter.key() );

          sql += delim + QString( "%1=" ).arg( quotedIdentifier( fld.name() ) );
          delim = ',';

          if ( fld.typeName() == "geometry" )
          {
            sql += QString( "%1(%2)" )
                   .arg( connectionRO()->majorVersion() < 2 ? "geomfromewkt" : "st_geomfromewkt",
                         quotedValue( siter->toString() ) );
          }
          else if ( fld.typeName() == "geography" )
          {
            sql += QString( "st_geographyfromewkt(%1)" )
                   .arg( quotedValue( siter->toString() ) );
          }
          else
          {
            sql += quotedValue( *siter );
          }
        }
        catch ( PGFieldNotFound )
        {
          // Field was missing - shouldn't happen
        }
      }

      sql += QString( " WHERE %1" ).arg( whereClause( fid ) );

      QgsPostgresResult result( conn->PQexec( sql ) );
      if ( result.PQresultStatus() != PGRES_COMMAND_OK && result.PQresultStatus() != PGRES_TUPLES_OK )
        throw PGException( result );

      // update feature id map if key was changed
      if ( pkChanged && mPrimaryKeyType == pktFidMap )
      {
        QVariant v = mShared->removeFid( fid );

        QList<QVariant> k = v.toList();

        for ( int i = 0; i < mPrimaryKeyAttrs.size(); i++ )
        {
          int idx = mPrimaryKeyAttrs.at( i );
          if ( !attrs.contains( idx ) )
            continue;

          k[i] = attrs[ idx ];
        }

        mShared->insertFid( fid, k );
      }
    }

    returnvalue &= conn->commit();
  }
  catch ( PGException &e )
  {
    pushError( tr( "PostGIS error while changing attributes: %1" ).arg( e.errorMessage() ) );
    conn->rollback();
    returnvalue = false;
  }

  conn->unlock();
  return returnvalue;
}

void QgsPostgresProvider::appendGeomParam( const QgsGeometry *geom, QStringList &params ) const
{
  if ( !geom )
  {
    params << QString::null;
    return;
  }

  QString param;

  QScopedPointer<QgsGeometry> convertedGeom( convertToProviderType( geom ) );
  const unsigned char *buf = convertedGeom ? convertedGeom->asWkb() : geom->asWkb();
  size_t wkbSize = convertedGeom ? convertedGeom->wkbSize() : geom->wkbSize();

  for ( size_t i = 0; i < wkbSize; ++i )
  {
    if ( connectionRO()->useWkbHex() )
      param += QString( "%1" ).arg(( int ) buf[i], 2, 16, QChar( '0' ) );
    else
      param += QString( "\\%1" ).arg(( int ) buf[i], 3, 8, QChar( '0' ) );
  }
  params << param;
}

bool QgsPostgresProvider::changeGeometryValues( const QgsGeometryMap &geometry_map )
{

  if ( mIsQuery || mGeometryColumn.isNull() )
    return false;

  QgsPostgresConn* conn = connectionRW();
  if ( !conn )
  {
    return false;
  }
  conn->lock();

  bool returnvalue = true;

  try
  {
    // Start the PostGIS transaction
    conn->begin();

    QString update;
    QgsPostgresResult result;

    if ( mSpatialColType == sctTopoGeometry )
    {
      // We will create a new TopoGeometry object with the new shape.
      // Later, we'll replace the old TopoGeometry with the new one,
      // to avoid orphans and retain higher level in an eventual
      // hierarchical definition
      update = QString( "SELECT id(%1) FROM %2 o WHERE %3" )
               .arg( geomParam( 1 ),
                     mQuery,
                     pkParamWhereClause( 2 ) );

      QString getid = QString( "SELECT id(%1) FROM %2 WHERE %3" )
                      .arg( quotedIdentifier( mGeometryColumn ),
                            mQuery,
                            pkParamWhereClause( 1 ) );

      QgsDebugMsg( "getting old topogeometry id: " + getid );

      result = connectionRO()->PQprepare( "getid", getid, 1, nullptr );
      if ( result.PQresultStatus() != PGRES_COMMAND_OK )
      {
        QgsDebugMsg( QString( "Exception thrown due to PQprepare of this query returning != PGRES_COMMAND_OK (%1 != expected %2): %3" )
                     .arg( result.PQresultStatus() ).arg( PGRES_COMMAND_OK ).arg( getid ) );
        throw PGException( result );
      }

      QString replace = QString( "UPDATE %1 SET %2="
                                 "( topology_id(%2),layer_id(%2),$1,type(%2) )"
                                 "WHERE %3" )
                        .arg( mQuery,
                              quotedIdentifier( mGeometryColumn ),
                              pkParamWhereClause( 2 ) );
      QgsDebugMsg( "TopoGeom swap: " + replace );
      result = conn->PQprepare( "replacetopogeom", replace, 2, nullptr );
      if ( result.PQresultStatus() != PGRES_COMMAND_OK )
      {
        QgsDebugMsg( QString( "Exception thrown due to PQprepare of this query returning != PGRES_COMMAND_OK (%1 != expected %2): %3" )
                     .arg( result.PQresultStatus() ).arg( PGRES_COMMAND_OK ).arg( replace ) );
        throw PGException( result );
      }

    }
    else
    {
      update = QString( "UPDATE %1 SET %2=%3 WHERE %4" )
               .arg( mQuery,
                     quotedIdentifier( mGeometryColumn ),
                     geomParam( 1 ),
                     pkParamWhereClause( 2 ) );
    }

    QgsDebugMsg( "updating: " + update );

    result = conn->PQprepare( "updatefeatures", update, 2, nullptr );
    if ( result.PQresultStatus() != PGRES_COMMAND_OK && result.PQresultStatus() != PGRES_TUPLES_OK )
    {
      QgsDebugMsg( QString( "Exception thrown due to PQprepare of this query returning != PGRES_COMMAND_OK (%1 != expected %2): %3" )
                   .arg( result.PQresultStatus() ).arg( PGRES_COMMAND_OK ).arg( update ) );
      throw PGException( result );
    }

    QgsDebugMsg( "iterating over the map of changed geometries..." );

    for ( QgsGeometryMap::const_iterator iter = geometry_map.constBegin();
          iter != geometry_map.constEnd();
          ++iter )
    {
      QgsDebugMsg( "iterating over feature id " + FID_TO_STRING( iter.key() ) );

      // Save the id of the current topogeometry
      long old_tg_id = -1;
      if ( mSpatialColType == sctTopoGeometry )
      {
        QStringList params;
        appendPkParams( iter.key(), params );
        result = connectionRO()->PQexecPrepared( "getid", params );
        if ( result.PQresultStatus() != PGRES_TUPLES_OK )
        {
          QgsDebugMsg( QString( "Exception thrown due to PQexecPrepared of 'getid' returning != PGRES_TUPLES_OK (%1 != expected %2)" )
                       .arg( result.PQresultStatus() ).arg( PGRES_TUPLES_OK ) );
          throw PGException( result );
        }
        // TODO: watch out for NULL, handle somehow
        old_tg_id = result.PQgetvalue( 0, 0 ).toLong();
        QgsDebugMsg( QString( "Old TG id is %1" ).arg( old_tg_id ) );
      }

      QStringList params;
      appendGeomParam( &*iter, params );
      appendPkParams( iter.key(), params );

      result = conn->PQexecPrepared( "updatefeatures", params );
      if ( result.PQresultStatus() != PGRES_COMMAND_OK && result.PQresultStatus() != PGRES_TUPLES_OK )
        throw PGException( result );

      if ( mSpatialColType == sctTopoGeometry )
      {
        long new_tg_id = result.PQgetvalue( 0, 0 ).toLong(); // new topogeo_id

        // Replace old TopoGeom with new TopoGeom, so that
        // any hierarchically defined TopoGeom will still have its
        // definition and we'll leave no orphans
        QString replace = QString( "DELETE FROM %1.relation WHERE "
                                   "layer_id = %2 AND topogeo_id = %3" )
                          .arg( quotedIdentifier( mTopoLayerInfo.topologyName ) )
                          .arg( mTopoLayerInfo.layerId )
                          .arg( old_tg_id );
        result = conn->PQexec( replace );
        if ( result.PQresultStatus() != PGRES_COMMAND_OK )
        {
          QgsDebugMsg( QString( "Exception thrown due to PQexec of this query returning != PGRES_COMMAND_OK (%1 != expected %2): %3" )
                       .arg( result.PQresultStatus() ).arg( PGRES_COMMAND_OK ).arg( replace ) );
          throw PGException( result );
        }
        // TODO: use prepared query here
        replace = QString( "UPDATE %1.relation SET topogeo_id = %2 "
                           "WHERE layer_id = %3 AND topogeo_id = %4" )
                  .arg( quotedIdentifier( mTopoLayerInfo.topologyName ) )
                  .arg( old_tg_id )
                  .arg( mTopoLayerInfo.layerId )
                  .arg( new_tg_id );
        QgsDebugMsg( "relation swap: " + replace );
        result = conn->PQexec( replace );
        if ( result.PQresultStatus() != PGRES_COMMAND_OK )
        {
          QgsDebugMsg( QString( "Exception thrown due to PQexec of this query returning != PGRES_COMMAND_OK (%1 != expected %2): %3" )
                       .arg( result.PQresultStatus() ).arg( PGRES_COMMAND_OK ).arg( replace ) );
          throw PGException( result );
        }
      } // if TopoGeometry

    } // for each feature

    conn->PQexecNR( "DEALLOCATE updatefeatures" );
    if ( mSpatialColType == sctTopoGeometry )
    {
      connectionRO()->PQexecNR( "DEALLOCATE getid" );
      conn->PQexecNR( "DEALLOCATE replacetopogeom" );
    }

    returnvalue &= conn->commit();
  }
  catch ( PGException &e )
  {
    pushError( tr( "PostGIS error while changing geometry values: %1" ).arg( e.errorMessage() ) );
    conn->rollback();
    conn->PQexecNR( "DEALLOCATE updatefeatures" );
    if ( mSpatialColType == sctTopoGeometry )
    {
      connectionRO()->PQexecNR( "DEALLOCATE getid" );
      conn->PQexecNR( "DEALLOCATE replacetopogeom" );
    }
    returnvalue = false;
  }

  conn->unlock();

  QgsDebugMsg( "leaving." );

  return returnvalue;
}

bool QgsPostgresProvider::changeFeatures( const QgsChangedAttributesMap &attr_map,
    const QgsGeometryMap &geometry_map )
{
  Q_ASSERT( mSpatialColType != sctTopoGeometry );

  bool returnvalue = true;

  if ( mIsQuery )
    return false;

  if ( attr_map.isEmpty() )
    return true;

  QgsPostgresConn *conn = connectionRW();
  if ( !conn )
    return false;

  conn->lock();

  try
  {
    conn->begin();

    QgsFeatureIds ids( attr_map.keys().toSet() );
    ids |= geometry_map.keys().toSet();

    // cycle through the features
    Q_FOREACH ( QgsFeatureId fid, ids )
    {
      // skip added features
      if ( FID_IS_NEW( fid ) )
        continue;

      const QgsAttributeMap &attrs = attr_map.value( fid );
      if ( attrs.isEmpty() && !geometry_map.contains( fid ) )
        continue;

      QString sql = QString( "UPDATE %1 SET " ).arg( mQuery );

      bool pkChanged = false;

      // cycle through the changed attributes of the feature
      QString delim;
      for ( QgsAttributeMap::const_iterator siter = attrs.constBegin(); siter != attrs.constEnd(); ++siter )
      {
        try
        {
          QgsField fld = field( siter.key() );

          pkChanged = pkChanged || mPrimaryKeyAttrs.contains( siter.key() );

          sql += delim + QString( "%1=" ).arg( quotedIdentifier( fld.name() ) );
          delim = ',';

          if ( fld.typeName() == "geometry" )
          {
            sql += QString( "%1(%2)" )
                   .arg( connectionRO()->majorVersion() < 2 ? "geomfromewkt" : "st_geomfromewkt",
                         quotedValue( siter->toString() ) );
          }
          else if ( fld.typeName() == "geography" )
          {
            sql += QString( "st_geographyfromewkt(%1)" )
                   .arg( quotedValue( siter->toString() ) );
          }
          else
          {
            sql += quotedValue( *siter );
          }
        }
        catch ( PGFieldNotFound )
        {
          // Field was missing - shouldn't happen
        }
      }

      if ( !geometry_map.contains( fid ) )
      {
        sql += QString( " WHERE %1" ).arg( whereClause( fid ) );

        QgsPostgresResult result( conn->PQexec( sql ) );
        if ( result.PQresultStatus() != PGRES_COMMAND_OK && result.PQresultStatus() != PGRES_TUPLES_OK )
          throw PGException( result );
      }
      else
      {
        sql += QString( "%1%2=%3" ).arg( delim, quotedIdentifier( mGeometryColumn ), geomParam( 1 ) );
        sql += QString( " WHERE %1" ).arg( whereClause( fid ) );

        QgsPostgresResult result( conn->PQprepare( "updatefeature", sql, 1, nullptr ) );
        if ( result.PQresultStatus() != PGRES_COMMAND_OK && result.PQresultStatus() != PGRES_TUPLES_OK )
        {
          QgsDebugMsg( QString( "Exception thrown due to PQprepare of this query returning != PGRES_COMMAND_OK (%1 != expected %2): %3" )
                       .arg( result.PQresultStatus() ).arg( PGRES_COMMAND_OK ).arg( sql ) );
          throw PGException( result );
        }

        QStringList params;
        const QgsGeometry &geom = geometry_map[ fid ];
        appendGeomParam( &geom, params );

        result = conn->PQexecPrepared( "updatefeature", params );
        if ( result.PQresultStatus() != PGRES_COMMAND_OK && result.PQresultStatus() != PGRES_TUPLES_OK )
          throw PGException( result );

        conn->PQexecNR( "DEALLOCATE updatefeature" );
      }

      // update feature id map if key was changed
      if ( pkChanged && mPrimaryKeyType == pktFidMap )
      {
        QVariant v = mShared->removeFid( fid );

        QList<QVariant> k = v.toList();

        for ( int i = 0; i < mPrimaryKeyAttrs.size(); i++ )
        {
          int idx = mPrimaryKeyAttrs.at( i );
          if ( !attrs.contains( idx ) )
            continue;

          k[i] = attrs[ idx ];
        }

        mShared->insertFid( fid, k );
      }
    }

    returnvalue &= conn->commit();
  }
  catch ( PGException &e )
  {
    pushError( tr( "PostGIS error while changing attributes: %1" ).arg( e.errorMessage() ) );
    conn->rollback();
    returnvalue = false;
  }

  conn->unlock();

  QgsDebugMsg( "leaving." );

  return returnvalue;
}

QgsAttributeList QgsPostgresProvider::attributeIndexes()
{
  QgsAttributeList lst;
  lst.reserve( mAttributeFields.count() );
  for ( int i = 0; i < mAttributeFields.count(); ++i )
    lst.append( i );
  return lst;
}

int QgsPostgresProvider::capabilities() const
{
  return mEnabledCapabilities;
}

bool QgsPostgresProvider::setSubsetString( const QString& theSQL, bool updateFeatureCount )
{
  QString prevWhere = mSqlWhereClause;

  mSqlWhereClause = theSQL.trimmed();

  QString sql = QString( "SELECT * FROM %1" ).arg( mQuery );

  if ( !mSqlWhereClause.isEmpty() )
  {
    sql += QString( " WHERE %1" ).arg( mSqlWhereClause );
  }

  sql += " LIMIT 0";

  QgsPostgresResult res( connectionRO()->PQexec( sql ) );
  if ( res.PQresultStatus() != PGRES_TUPLES_OK )
  {
    pushError( res.PQresultErrorMessage() );
    mSqlWhereClause = prevWhere;
    return false;
  }

#if 0
  // FIXME
  if ( mPrimaryKeyType == pktInt && !uniqueData( primaryKeyAttr ) )
  {
    sqlWhereClause = prevWhere;
    return false;
  }
#endif

  // Update datasource uri too
  mUri.setSql( theSQL );
  // Update yet another copy of the uri. Why are there 3 copies of the
  // uri? Perhaps this needs some rationalisation.....
  setDataSourceUri( mUri.uri( false ) );

  if ( updateFeatureCount )
  {
    mShared->setFeaturesCounted( -1 );
  }
  mLayerExtent.setMinimal();

  emit dataChanged();

  return true;
}

/**
 * Return the feature count
 */
long QgsPostgresProvider::featureCount() const
{
  int featuresCounted = mShared->featuresCounted();
  if ( featuresCounted >= 0 )
    return featuresCounted;

  // get total number of features
  QString sql;

  // use estimated metadata even when there is a where clause,
  // although we get an incorrect feature count for the subset
  // - but make huge dataset usable.
  if ( !mIsQuery && mUseEstimatedMetadata )
  {
    sql = QString( "SELECT reltuples::int FROM pg_catalog.pg_class WHERE oid=regclass(%1)::oid" ).arg( quotedValue( mQuery ) );
  }
  else
  {
    sql = QString( "SELECT count(*) FROM %1%2" ).arg( mQuery, filterWhereClause() );
  }

  QgsPostgresResult result( connectionRO()->PQexec( sql ) );

  QgsDebugMsg( "number of features as text: " + result.PQgetvalue( 0, 0 ) );

  long num = result.PQgetvalue( 0, 0 ).toLong();
  mShared->setFeaturesCounted( num );

  QgsDebugMsg( "number of features: " + QString::number( num ) );

  return num;
}

QgsRectangle QgsPostgresProvider::extent()
{
  if ( mGeometryColumn.isNull() )
    return QgsRectangle();

  if ( mSpatialColType == sctGeography )
    return QgsRectangle( -180.0, -90.0, 180.0, 90.0 );

  if ( mLayerExtent.isEmpty() )
  {
    QString sql;
    QgsPostgresResult result;
    QString ext;

    // get the extents
    if ( !mIsQuery && ( mUseEstimatedMetadata || mSqlWhereClause.isEmpty() ) )
    {
      // do stats exists?
      sql = QString( "SELECT count(*) FROM pg_stats WHERE schemaname=%1 AND tablename=%2 AND attname=%3" )
            .arg( quotedValue( mSchemaName ),
                  quotedValue( mTableName ),
                  quotedValue( mGeometryColumn ) );
      result = connectionRO()->PQexec( sql );
      if ( result.PQresultStatus() == PGRES_TUPLES_OK && result.PQntuples() == 1 )
      {
        if ( result.PQgetvalue( 0, 0 ).toInt() > 0 )
        {
          sql = QString( "SELECT reltuples::int FROM pg_catalog.pg_class WHERE oid=regclass(%1)::oid" ).arg( quotedValue( mQuery ) );
          result = connectionRO()->PQexec( sql );
          if ( result.PQresultStatus() == PGRES_TUPLES_OK
               && result.PQntuples() == 1
               && result.PQgetvalue( 0, 0 ).toLong() > 0 )
          {
            sql = QString( "SELECT %1(%2,%3,%4)" )
                  .arg( connectionRO()->majorVersion() < 2 ? "estimated_extent" :
                        ( connectionRO()->majorVersion() == 2 && connectionRO()->minorVersion() < 1 ? "st_estimated_extent" : "st_estimatedextent" ),
                        quotedValue( mSchemaName ),
                        quotedValue( mTableName ),
                        quotedValue( mGeometryColumn ) );
            result = mConnectionRO->PQexec( sql );
            if ( result.PQresultStatus() == PGRES_TUPLES_OK && result.PQntuples() == 1 && !result.PQgetisnull( 0, 0 ) )
            {
              ext = result.PQgetvalue( 0, 0 );

              // fix for what might be a postgis bug: when the extent crosses the
              // dateline extent() returns -180 to 180 (which appears right), but
              // estimated_extent() returns eastern bound of data (>-180) and
              // 180 degrees.
              if ( !ext.startsWith( "-180 " ) && ext.contains( ",180 " ) )
              {
                ext.clear();
              }
            }
          }
          else
          {
            // no features => ignore estimated extent
            ext.clear();
          }
        }
      }
      else
      {
        QgsDebugMsg( QString( "no column statistics for %1.%2.%3" ).arg( mSchemaName, mTableName, mGeometryColumn ) );
      }
    }

    if ( ext.isEmpty() )
    {
      sql = QString( "SELECT %1(%2%3) FROM %4%5" )
            .arg( connectionRO()->majorVersion() < 2 ? "extent" : "st_extent",
                  quotedIdentifier( mGeometryColumn ),
                  mSpatialColType == sctPcPatch ? "::geometry" : "",
                  mQuery,
                  filterWhereClause() );

      result = connectionRO()->PQexec( sql );
      if ( result.PQresultStatus() != PGRES_TUPLES_OK )
        connectionRO()->PQexecNR( "ROLLBACK" );
      else if ( result.PQntuples() == 1 && !result.PQgetisnull( 0, 0 ) )
        ext = result.PQgetvalue( 0, 0 );
    }

    if ( !ext.isEmpty() )
    {
      QgsDebugMsg( "Got extents using: " + sql );

      QRegExp rx( "\\((.+) (.+),(.+) (.+)\\)" );
      if ( ext.contains( rx ) )
      {
        QStringList ex = rx.capturedTexts();

        mLayerExtent.setXMinimum( ex[1].toDouble() );
        mLayerExtent.setYMinimum( ex[2].toDouble() );
        mLayerExtent.setXMaximum( ex[3].toDouble() );
        mLayerExtent.setYMaximum( ex[4].toDouble() );
      }
      else
      {
        QgsMessageLog::logMessage( tr( "result of extents query invalid: %1" ).arg( ext ), tr( "PostGIS" ) );
      }
    }

    QgsDebugMsg( "Set extents to: " + mLayerExtent.toString() );
  }

  return mLayerExtent;
}

void QgsPostgresProvider::updateExtents()
{
  mLayerExtent.setMinimal();
}

bool QgsPostgresProvider::getGeometryDetails()
{
  if ( mGeometryColumn.isNull() )
  {
    mDetectedGeomType = QGis::WKBNoGeometry;
    mValid = true;
    return true;
  }

  QgsPostgresResult result;
  QString sql;

  QString schemaName = mSchemaName;
  QString tableName = mTableName;
  QString geomCol = mGeometryColumn;
  QString geomColType;

  if ( mIsQuery )
  {
    sql = QString( "SELECT %1 FROM %2 LIMIT 0" ).arg( quotedIdentifier( mGeometryColumn ), mQuery );

    QgsDebugMsg( QString( "Getting geometry column: %1" ).arg( sql ) );

    QgsPostgresResult result( connectionRO()->PQexec( sql ) );
    if ( PGRES_TUPLES_OK == result.PQresultStatus() )
    {
      Oid tableoid = result.PQftable( 0 );
      int column = result.PQftablecol( 0 );

      result = connectionRO()->PQexec( sql );
      if ( tableoid > 0 && PGRES_TUPLES_OK == result.PQresultStatus() )
      {
        sql = QString( "SELECT pg_namespace.nspname,pg_class.relname FROM pg_class,pg_namespace WHERE pg_class.relnamespace=pg_namespace.oid AND pg_class.oid=%1" ).arg( tableoid );
        result = connectionRO()->PQexec( sql );

        if ( PGRES_TUPLES_OK == result.PQresultStatus() && 1 == result.PQntuples() )
        {
          schemaName = result.PQgetvalue( 0, 0 );
          tableName = result.PQgetvalue( 0, 1 );

          sql = QString( "SELECT a.attname, t.typname FROM pg_attribute a, pg_type t WHERE a.attrelid=%1 AND a.attnum=%2 AND a.atttypid = t.oid" ).arg( tableoid ).arg( column );
          result = connectionRO()->PQexec( sql );
          if ( PGRES_TUPLES_OK == result.PQresultStatus() && 1 == result.PQntuples() )
          {
            geomCol = result.PQgetvalue( 0, 0 );
            geomColType = result.PQgetvalue( 0, 1 );
            if ( geomColType == "geometry" )
              mSpatialColType = sctGeometry;
            else if ( geomColType == "geography" )
              mSpatialColType = sctGeography;
            else if ( geomColType == "topogeometry" )
              mSpatialColType = sctTopoGeometry;
            else if ( geomColType == "pcpatch" )
              mSpatialColType = sctPcPatch;
            else
              mSpatialColType = sctNone;
          }
          else
          {
            schemaName = mSchemaName;
            tableName = mTableName;
          }
        }
      }
      else
      {
        schemaName = "";
        tableName = mQuery;
      }
    }
    else
    {
      mValid = false;
      return false;
    }
  }

  QString detectedType;
  QString detectedSrid = mRequestedSrid;
  if ( !schemaName.isEmpty() )
  {
    // check geometry columns
    sql = QString( "SELECT upper(type),srid,coord_dimension FROM geometry_columns WHERE f_table_name=%1 AND f_geometry_column=%2 AND f_table_schema=%3" )
          .arg( quotedValue( tableName ),
                quotedValue( geomCol ),
                quotedValue( schemaName ) );

    QgsDebugMsg( QString( "Getting geometry column: %1" ).arg( sql ) );
    result = connectionRO()->PQexec( sql );
    QgsDebugMsg( QString( "Geometry column query returned %1 rows" ).arg( result.PQntuples() ) );

    if ( result.PQntuples() == 1 )
    {
      detectedType = result.PQgetvalue( 0, 0 );
      QString dim = result.PQgetvalue( 0, 2 );
      if ( dim == "3" && !detectedType.endsWith( 'M' ) )
        detectedType += "Z";
      else if ( dim == "4" )
        detectedType += "ZM";

      detectedSrid = result.PQgetvalue( 0, 1 );
      mSpatialColType = sctGeometry;
    }
    else
    {
      connectionRO()->PQexecNR( "COMMIT" );
    }

    if ( detectedType.isEmpty() )
    {
      // check geography columns
      sql = QString( "SELECT upper(type),srid FROM geography_columns WHERE f_table_name=%1 AND f_geography_column=%2 AND f_table_schema=%3" )
            .arg( quotedValue( tableName ),
                  quotedValue( geomCol ),
                  quotedValue( schemaName ) );

      QgsDebugMsg( QString( "Getting geography column: %1" ).arg( sql ) );
      result = connectionRO()->PQexec( sql, false );
      QgsDebugMsg( QString( "Geography column query returned %1" ).arg( result.PQntuples() ) );

      if ( result.PQntuples() == 1 )
      {
        detectedType = result.PQgetvalue( 0, 0 );
        detectedSrid = result.PQgetvalue( 0, 1 );
        mSpatialColType = sctGeography;
      }
      else
      {
        connectionRO()->PQexecNR( "COMMIT" );
      }
    }

    if ( detectedType.isEmpty() && connectionRO()->hasTopology() )
    {
      // check topology.layer
      sql = QString( "SELECT CASE "
                     "WHEN l.feature_type = 1 THEN 'MULTIPOINT' "
                     "WHEN l.feature_type = 2 THEN 'MULTILINESTRING' "
                     "WHEN l.feature_type = 3 THEN 'MULTIPOLYGON' "
                     "WHEN l.feature_type = 4 THEN 'GEOMETRYCOLLECTION' "
                     "END AS type, t.srid FROM topology.layer l, topology.topology t "
                     "WHERE l.topology_id = t.id AND l.schema_name=%3 "
                     "AND l.table_name=%1 AND l.feature_column=%2" )
            .arg( quotedValue( tableName ),
                  quotedValue( geomCol ),
                  quotedValue( schemaName ) );

      QgsDebugMsg( QString( "Getting TopoGeometry column: %1" ).arg( sql ) );
      result = connectionRO()->PQexec( sql, false );
      QgsDebugMsg( QString( "TopoGeometry column query returned %1" ).arg( result.PQntuples() ) );

      if ( result.PQntuples() == 1 )
      {
        detectedType = result.PQgetvalue( 0, 0 );
        detectedSrid = result.PQgetvalue( 0, 1 );
        mSpatialColType = sctTopoGeometry;
      }
      else
      {
        connectionRO()->PQexecNR( "COMMIT" );
      }
    }

    if ( detectedType.isEmpty() && connectionRO()->hasPointcloud() )
    {
      // check pointcloud columns
      sql = QString( "SELECT 'POLYGON',srid FROM pointcloud_columns WHERE \"table\"=%1 AND \"column\"=%2 AND \"schema\"=%3" )
            .arg( quotedValue( tableName ),
                  quotedValue( geomCol ),
                  quotedValue( schemaName ) );

      QgsDebugMsg( QString( "Getting pointcloud column: %1" ).arg( sql ) );
      result = connectionRO()->PQexec( sql, false );
      QgsDebugMsg( QString( "Pointcloud column query returned %1" ).arg( result.PQntuples() ) );

      if ( result.PQntuples() == 1 )
      {
        detectedType = result.PQgetvalue( 0, 0 );
        detectedSrid = result.PQgetvalue( 0, 1 );
        mSpatialColType = sctPcPatch;
      }
      else
      {
        connectionRO()->PQexecNR( "COMMIT" );
      }
    }

    if ( mSpatialColType == sctNone )
    {
      sql = QString( "SELECT t.typname FROM "
                     "pg_attribute a, pg_class c, pg_namespace n, pg_type t "
                     "WHERE a.attrelid=c.oid AND c.relnamespace=n.oid "
                     "AND a.atttypid=t.oid "
                     "AND n.nspname=%3 AND c.relname=%1 AND a.attname=%2" )
            .arg( quotedValue( tableName ),
                  quotedValue( geomCol ),
                  quotedValue( schemaName ) );
      QgsDebugMsg( QString( "Getting column datatype: %1" ).arg( sql ) );
      result = connectionRO()->PQexec( sql, false );
      QgsDebugMsg( QString( "Column datatype query returned %1" ).arg( result.PQntuples() ) );
      if ( result.PQntuples() == 1 )
      {
        geomColType = result.PQgetvalue( 0, 0 );
        if ( geomColType == "geometry" )
          mSpatialColType = sctGeometry;
        else if ( geomColType == "geography" )
          mSpatialColType = sctGeography;
        else if ( geomColType == "topogeometry" )
          mSpatialColType = sctTopoGeometry;
        else if ( geomColType == "pcpatch" )
          mSpatialColType = sctPcPatch;
      }
      else
      {
        connectionRO()->PQexecNR( "COMMIT" );
      }
    }
  }
  else
  {
    sql = QString( "SELECT %1 FROM %2 LIMIT 0" ).arg( quotedIdentifier( mGeometryColumn ), mQuery );
    result = connectionRO()->PQexec( sql );
    if ( PGRES_TUPLES_OK == result.PQresultStatus() )
    {
      sql = QString( "SELECT (SELECT t.typname FROM pg_type t WHERE oid = %1), upper(postgis_typmod_type(%2)), postgis_typmod_srid(%2)" )
            .arg( QString::number( result.PQftype( 0 ) ), QString::number( result.PQfmod( 0 ) ) );
      result = connectionRO()->PQexec( sql, false );
      if ( result.PQntuples() == 1 )
      {
        geomColType  = result.PQgetvalue( 0, 0 );
        detectedType = result.PQgetvalue( 0, 1 );
        detectedSrid = result.PQgetvalue( 0, 2 );
        if ( geomColType == "geometry" )
          mSpatialColType = sctGeometry;
        else if ( geomColType == "geography" )
          mSpatialColType = sctGeography;
        else if ( geomColType == "topogeometry" )
          mSpatialColType = sctTopoGeometry;
        else if ( geomColType == "pcpatch" )
          mSpatialColType = sctPcPatch;
        else
        {
          detectedType = mRequestedGeomType == QGis::WKBUnknown ? "" : QgsPostgresConn::postgisWkbTypeName( mRequestedGeomType );
          detectedSrid = mRequestedSrid;
        }
      }
      else
      {
        connectionRO()->PQexecNR( "COMMIT" );
        detectedType = mRequestedGeomType == QGis::WKBUnknown ? "" : QgsPostgresConn::postgisWkbTypeName( mRequestedGeomType );
      }
    }
    else
    {
      mValid = false;
      return false;
    }
  }

  mDetectedGeomType = QgsPostgresConn::wkbTypeFromPostgis( detectedType );
  mDetectedSrid     = detectedSrid;

  if ( mDetectedGeomType == QGis::WKBUnknown )
  {
    mDetectedSrid = "";

    QgsPostgresLayerProperty layerProperty;
    if ( !mIsQuery )
    {
      layerProperty.schemaName = schemaName;
      layerProperty.tableName  = tableName;
    }
    else
    {
      layerProperty.schemaName = "";
      layerProperty.tableName  = mQuery;
    }
    layerProperty.geometryColName = mGeometryColumn;
    layerProperty.geometryColType = mSpatialColType;
    layerProperty.force2d         = false;

    QString delim = "";

    if ( !mSqlWhereClause.isEmpty() )
    {
      layerProperty.sql += delim + '(' + mSqlWhereClause + ')';
      delim = " AND ";
    }

    connectionRO()->retrieveLayerTypes( layerProperty, mUseEstimatedMetadata );

    mSpatialColType = layerProperty.geometryColType;

    if ( layerProperty.size() == 0 )
    {
      // no data - so take what's requested
      if ( mRequestedGeomType == QGis::WKBUnknown || mRequestedSrid.isEmpty() )
      {
        QgsMessageLog::logMessage( tr( "Geometry type and srid for empty column %1 of %2 undefined." ).arg( mGeometryColumn, mQuery ) );
      }
    }
    else
    {
      int i;
      for ( i = 0; i < layerProperty.size(); i++ )
      {
        QGis::WkbType wkbType = layerProperty.types.at( i );

        if (( wkbType != QGis::WKBUnknown && ( mRequestedGeomType == QGis::WKBUnknown || mRequestedGeomType == wkbType ) ) &&
            ( mRequestedSrid.isEmpty() || layerProperty.srids.at( i ) == mRequestedSrid.toInt() ) )
          break;
      }

      // requested type && srid is available
      if ( i < layerProperty.size() )
      {
        if ( layerProperty.size() == 1 )
        {
          // only what we requested is available
          mDetectedGeomType = layerProperty.types.at( 0 );
          mDetectedSrid     = QString::number( layerProperty.srids.at( 0 ) );
          mForce2d          = layerProperty.force2d;
        }
      }
      else
      {
        // geometry type undetermined or not unrequested
        QgsMessageLog::logMessage( tr( "Feature type or srid for %1 of %2 could not be determined or was not requested." ).arg( mGeometryColumn, mQuery ) );
      }
    }
  }

  QgsDebugMsg( QString( "Detected SRID is %1" ).arg( mDetectedSrid ) );
  QgsDebugMsg( QString( "Requested SRID is %1" ).arg( mRequestedSrid ) );
  QgsDebugMsg( QString( "Detected type is %1" ).arg( mDetectedGeomType ) );
  QgsDebugMsg( QString( "Requested type is %1" ).arg( mRequestedGeomType ) );
  QgsDebugMsg( QString( "Force to 2D %1" ).arg( mForce2d ? "Yes" : "No" ) );

  mValid = ( mDetectedGeomType != QGis::WKBUnknown || mRequestedGeomType != QGis::WKBUnknown )
           && ( !mDetectedSrid.isEmpty() || !mRequestedSrid.isEmpty() );

  if ( !mValid )
    return false;

  // store whether the geometry includes measure value
  if ( detectedType == "POINTM" || detectedType == "MULTIPOINTM" ||
       detectedType == "LINESTRINGM" || detectedType == "MULTILINESTRINGM" ||
       detectedType == "POLYGONM" || detectedType == "MULTIPOLYGONM" ||
       mForce2d )
  {
    // explicitly disable adding new features and editing of geometries
    // as this would lead to corruption of measures
    QgsMessageLog::logMessage( tr( "Editing and adding disabled for 2D+ layer (%1; %2)" ).arg( mGeometryColumn, mQuery ) );
    mEnabledCapabilities &= ~( QgsVectorDataProvider::AddFeatures );
  }

  QgsDebugMsg( QString( "Feature type name is %1" ).arg( QGis::featureType( geometryType() ) ) );
  QgsDebugMsg( QString( "Spatial column type is %1" ).arg( QgsPostgresConn::displayStringForGeomType( mSpatialColType ) ) );

  return mValid;
}

bool QgsPostgresProvider::convertField( QgsField &field, const QMap<QString, QVariant> *options )
{
  //determine field type to use for strings
  QString stringFieldType = "varchar";
  if ( options && options->value( "dropStringConstraints", false ).toBool() )
  {
    //drop string length constraints by using PostgreSQL text type for strings
    stringFieldType = "text";
  }

  QString fieldType = stringFieldType; //default to string
  int fieldSize = field.length();
  int fieldPrec = field.precision();
  switch ( field.type() )
  {
    case QVariant::LongLong:
      fieldType = "int8";
      fieldPrec = 0;
      break;

    case QVariant::DateTime:
      fieldType = "timestamp without time zone";
      break;

    case QVariant::Time:
      fieldType = "time";
      break;

    case QVariant::String:
      fieldType = stringFieldType;
      fieldPrec = -1;
      break;

    case QVariant::Int:
      fieldType = "int4";
      fieldPrec = 0;
      break;

    case QVariant::Date:
      fieldType = "date";
      fieldPrec = 0;
      break;

    case QVariant::Double:
      if ( fieldPrec > 0 )
      {
        fieldType = "numeric";
      }
      else
      {
        fieldType = "float8";
        fieldSize = -1;
        fieldPrec = -1;
      }
      break;

    default:
      return false;
  }

  field.setTypeName( fieldType );
  field.setLength( fieldSize );
  field.setPrecision( fieldPrec );
  return true;
}

QgsVectorLayerImport::ImportError QgsPostgresProvider::createEmptyLayer(
  const QString& uri,
  const QgsFields &fields,
  QGis::WkbType wkbType,
  const QgsCoordinateReferenceSystem *srs,
  bool overwrite,
  QMap<int, int> *oldToNewAttrIdxMap,
  QString *errorMessage,
  const QMap<QString, QVariant> *options )
{
  // populate members from the uri structure
  QgsDataSourceURI dsUri( uri );
  QString schemaName = dsUri.schema();
  QString tableName = dsUri.table();

  QString geometryColumn = dsUri.geometryColumn();
  QString geometryType;

  QString primaryKey = dsUri.keyColumn();
  QString primaryKeyType;

  QString schemaTableName = "";
  if ( !schemaName.isEmpty() )
  {
    schemaTableName += quotedIdentifier( schemaName ) + '.';
  }
  schemaTableName += quotedIdentifier( tableName );

  QgsDebugMsg( QString( "Connection info is: %1" ).arg( dsUri.connectionInfo( false ) ) );
  QgsDebugMsg( QString( "Geometry column is: %1" ).arg( geometryColumn ) );
  QgsDebugMsg( QString( "Schema is: %1" ).arg( schemaName ) );
  QgsDebugMsg( QString( "Table name is: %1" ).arg( tableName ) );

  // create the table
  QgsPostgresConn *conn = QgsPostgresConn::connectDb( dsUri.connectionInfo( false ), false );
  if ( !conn )
  {
    if ( errorMessage )
      *errorMessage = QObject::tr( "Connection to database failed" );
    return QgsVectorLayerImport::ErrConnectionFailed;
  }

  // get the pk's name and type

  // if no pk name was passed, define the new pk field name
  if ( primaryKey.isEmpty() )
  {
    int index = 0;
    QString pk = primaryKey = "id";
    for ( int fldIdx = 0; fldIdx < fields.count(); ++fldIdx )
    {
      if ( fields[fldIdx].name() == primaryKey )
      {
        // it already exists, try again with a new name
        primaryKey = QString( "%1_%2" ).arg( pk ).arg( index++ );
        fldIdx = -1; // it is incremented in the for loop, i.e. restarts at 0
      }
    }
  }
  else
  {
    // search for the passed field
    for ( int fldIdx = 0; fldIdx < fields.count(); ++fldIdx )
    {
      if ( fields[fldIdx].name() == primaryKey )
      {
        // found, get the field type
        QgsField fld = fields[fldIdx];
        if ( convertField( fld, options ) )
        {
          primaryKeyType = fld.typeName();
        }
      }
    }
  }

  // if the pk field doesn't exist yet, create a serial pk field
  // as it's autoincremental
  if ( primaryKeyType.isEmpty() )
  {
    primaryKeyType = "serial";
#if 0
    // TODO: check the feature count to choose if create a serial8 pk field
    if ( layer->featureCount() > 0xffffffff )
    {
      primaryKeyType = "serial8";
    }
#endif
  }
  else
  {
    // if the pk field's type is one of the postgres integer types,
    // use the equivalent autoincremental type (serialN)
    if ( primaryKeyType == "int2" || primaryKeyType == "int4" )
    {
      primaryKeyType = "serial";
    }
    else if ( primaryKeyType == "int8" )
    {
      primaryKeyType = "serial8";
    }
  }

  try
  {
    conn->PQexecNR( "BEGIN" );

    QString sql = QString( "SELECT 1"
                           " FROM pg_class AS cls JOIN pg_namespace AS nsp"
                           " ON nsp.oid=cls.relnamespace "
                           " WHERE cls.relname=%1 AND nsp.nspname=%2" )
                  .arg( quotedValue( tableName ),
                        quotedValue( schemaName ) );

    QgsPostgresResult result( conn->PQexec( sql ) );
    if ( result.PQresultStatus() != PGRES_TUPLES_OK )
      throw PGException( result );

    bool exists = result.PQntuples() > 0;

    if ( exists && overwrite )
    {
      // delete the table if exists, then re-create it
      QString sql = QString( "SELECT DropGeometryTable(%1,%2)"
                             " FROM pg_class AS cls JOIN pg_namespace AS nsp"
                             " ON nsp.oid=cls.relnamespace "
                             " WHERE cls.relname=%2 AND nsp.nspname=%1" )
                    .arg( quotedValue( schemaName ),
                          quotedValue( tableName ) );

      result = conn->PQexec( sql );
      if ( result.PQresultStatus() != PGRES_TUPLES_OK )
        throw PGException( result );
    }

    if ( options && options->value( "lowercaseFieldNames", false ).toBool() )
    {
      //convert primary key name to lowercase
      //this must happen after determining the field type of the primary key
      primaryKey = primaryKey.toLower();
    }

    sql = QString( "CREATE TABLE %1(%2 %3 PRIMARY KEY)" )
          .arg( schemaTableName,
                quotedIdentifier( primaryKey ),
                primaryKeyType );

    result = conn->PQexec( sql );
    if ( result.PQresultStatus() != PGRES_COMMAND_OK )
      throw PGException( result );

    // get geometry type, dim and srid
    int dim = 2;
    long srid = srs->postgisSrid();

    QgsPostgresConn::postgisWkbType( wkbType, geometryType, dim );

    // create geometry column
    if ( !geometryType.isEmpty() )
    {
      sql = QString( "SELECT AddGeometryColumn(%1,%2,%3,%4,%5,%6)" )
            .arg( quotedValue( schemaName ),
                  quotedValue( tableName ),
                  quotedValue( geometryColumn ) )
            .arg( srid )
            .arg( quotedValue( geometryType ) )
            .arg( dim );

      result = conn->PQexec( sql );
      if ( result.PQresultStatus() != PGRES_TUPLES_OK )
        throw PGException( result );
    }
    else
    {
      geometryColumn.clear();
    }

    conn->PQexecNR( "COMMIT" );
  }
  catch ( PGException &e )
  {
    if ( errorMessage )
      *errorMessage = QObject::tr( "Creation of data source %1 failed: \n%2" )
                      .arg( schemaTableName,
                            e.errorMessage() );

    conn->PQexecNR( "ROLLBACK" );
    conn->unref();
    return QgsVectorLayerImport::ErrCreateLayer;
  }
  conn->unref();

  QgsDebugMsg( QString( "layer %1 created" ).arg( schemaTableName ) );

  // use the provider to edit the table
  dsUri.setDataSource( schemaName, tableName, geometryColumn, QString(), primaryKey );
  QgsPostgresProvider *provider = new QgsPostgresProvider( dsUri.uri( false ) );
  if ( !provider->isValid() )
  {
    if ( errorMessage )
      *errorMessage = QObject::tr( "Loading of the layer %1 failed" ).arg( schemaTableName );

    delete provider;
    return QgsVectorLayerImport::ErrInvalidLayer;
  }

  QgsDebugMsg( "layer loaded" );

  // add fields to the layer
  if ( oldToNewAttrIdxMap )
    oldToNewAttrIdxMap->clear();

  if ( fields.size() > 0 )
  {
    int offset = 1;

    // get the list of fields
    QList<QgsField> flist;
    for ( int fldIdx = 0; fldIdx < fields.count(); ++fldIdx )
    {
      QgsField fld = fields[fldIdx];

      if ( fld.name() == geometryColumn )
      {
        //the "lowercaseFieldNames" option does not affect the name of the geometry column, so we perform
        //this test before converting the field name to lowercase
        QgsDebugMsg( "Found a field with the same name of the geometry column. Skip it!" );
        continue;
      }

      if ( options && options->value( "lowercaseFieldNames", false ).toBool() )
      {
        //convert field name to lowercase
        fld.setName( fld.name().toLower() );
      }

      if ( fld.name() == primaryKey )
      {
        oldToNewAttrIdxMap->insert( fldIdx, 0 );
        continue;
      }

      if ( !convertField( fld, options ) )
      {
        if ( errorMessage )
          *errorMessage = QObject::tr( "Unsupported type for field %1" ).arg( fld.name() );

        delete provider;
        return QgsVectorLayerImport::ErrAttributeTypeUnsupported;
      }

      QgsDebugMsg( QString( "creating field #%1 -> #%2 name %3 type %4 typename %5 width %6 precision %7" )
                   .arg( fldIdx ).arg( offset )
                   .arg( fld.name(), QVariant::typeToName( fld.type() ), fld.typeName() )
                   .arg( fld.length() ).arg( fld.precision() )
                 );

      flist.append( fld );
      if ( oldToNewAttrIdxMap )
        oldToNewAttrIdxMap->insert( fldIdx, offset++ );
    }

    if ( !provider->addAttributes( flist ) )
    {
      if ( errorMessage )
        *errorMessage = QObject::tr( "Creation of fields failed" );

      delete provider;
      return QgsVectorLayerImport::ErrAttributeCreationFailed;
    }

    QgsDebugMsg( "Done creating fields" );
  }
  return QgsVectorLayerImport::NoError;
}

QgsCoordinateReferenceSystem QgsPostgresProvider::crs()
{
  QgsCoordinateReferenceSystem srs;
  int srid = mRequestedSrid.isEmpty() ? mDetectedSrid.toInt() : mRequestedSrid.toInt();
  srs.createFromSrid( srid );
  if ( !srs.isValid() )
  {
    QgsPostgresResult result( connectionRO()->PQexec( QString( "SELECT proj4text FROM spatial_ref_sys WHERE srid=%1" ).arg( srid ) ) );
    if ( result.PQresultStatus() == PGRES_TUPLES_OK )
      srs = QgsCRSCache::instance()->crsByProj4( result.PQgetvalue( 0, 0 ) );
  }
  return srs;
}

QString QgsPostgresProvider::subsetString()
{
  return mSqlWhereClause;
}

QString QgsPostgresProvider::getTableName()
{
  return mTableName;
}

size_t QgsPostgresProvider::layerCount() const
{
  return 1;                   // XXX need to return actual number of layers
} // QgsPostgresProvider::layerCount()


QString  QgsPostgresProvider::name() const
{
  return POSTGRES_KEY;
} //  QgsPostgresProvider::name()

QString  QgsPostgresProvider::description() const
{
  QString pgVersion( tr( "PostgreSQL version: unknown" ) );
  QString postgisVersion( tr( "unknown" ) );

  if ( connectionRO() )
  {
    QgsPostgresResult result;

    result = connectionRO()->PQexec( "SELECT version()" );
    if ( result.PQresultStatus() == PGRES_TUPLES_OK )
    {
      pgVersion = result.PQgetvalue( 0, 0 );
    }

    result = connectionRO()->PQexec( "SELECT postgis_version()" );
    if ( result.PQresultStatus() == PGRES_TUPLES_OK )
    {
      postgisVersion = result.PQgetvalue( 0, 0 );
    }
  }
  else
  {
    pgVersion = tr( "PostgreSQL not connected" );
  }

  return tr( "PostgreSQL/PostGIS provider\n%1\nPostGIS %2" ).arg( pgVersion, postgisVersion );
} //  QgsPostgresProvider::description()

/**
 * Class factory to return a pointer to a newly created
 * QgsPostgresProvider object
 */
QGISEXTERN QgsPostgresProvider * classFactory( const QString *uri )
{
  return new QgsPostgresProvider( *uri );
}
/** Required key function (used to map the plugin to a data store type)
*/
QGISEXTERN QString providerKey()
{
  return  POSTGRES_KEY;
}
/**
 * Required description function
 */
QGISEXTERN QString description()
{
  return POSTGRES_DESCRIPTION;
}
/**
 * Required isProvider function. Used to determine if this shared library
 * is a data provider plugin
 */
QGISEXTERN bool isProvider()
{
  return true;
}

QGISEXTERN QgsPgSourceSelect *selectWidget( QWidget *parent, Qt::WindowFlags fl )
{
  return new QgsPgSourceSelect( parent, fl );
}

QGISEXTERN int dataCapabilities()
{
  return QgsDataProvider::Database;
}

QGISEXTERN QgsDataItem *dataItem( QString thePath, QgsDataItem *parentItem )
{
  Q_UNUSED( thePath );
  return new QgsPGRootItem( parentItem, "PostGIS", "pg:" );
}

// ---------------------------------------------------------------------------

QGISEXTERN QgsVectorLayerImport::ImportError createEmptyLayer(
  const QString& uri,
  const QgsFields &fields,
  QGis::WkbType wkbType,
  const QgsCoordinateReferenceSystem *srs,
  bool overwrite,
  QMap<int, int> *oldToNewAttrIdxMap,
  QString *errorMessage,
  const QMap<QString, QVariant> *options )
{
  return QgsPostgresProvider::createEmptyLayer(
           uri, fields, wkbType, srs, overwrite,
           oldToNewAttrIdxMap, errorMessage, options
         );
}

QGISEXTERN bool deleteLayer( const QString& uri, QString& errCause )
{
  QgsDebugMsg( "deleting layer " + uri );

  QgsDataSourceURI dsUri( uri );
  QString schemaName = dsUri.schema();
  QString tableName = dsUri.table();
  QString geometryCol = dsUri.geometryColumn();

  QString schemaTableName;
  if ( !schemaName.isEmpty() )
  {
    schemaTableName = QgsPostgresConn::quotedIdentifier( schemaName ) + '.';
  }
  schemaTableName += QgsPostgresConn::quotedIdentifier( tableName );

  QgsPostgresConn *conn = QgsPostgresConn::connectDb( dsUri.connectionInfo( false ), false );
  if ( !conn )
  {
    errCause = QObject::tr( "Connection to database failed" );
    return false;
  }

  // check the geometry column count
  QString sql = QString( "SELECT count(*) "
                         "FROM geometry_columns, pg_class, pg_namespace "
                         "WHERE f_table_name=relname AND f_table_schema=nspname "
                         "AND pg_class.relnamespace=pg_namespace.oid "
                         "AND f_table_schema=%1 AND f_table_name=%2" )
                .arg( QgsPostgresConn::quotedValue( schemaName ),
                      QgsPostgresConn::quotedValue( tableName ) );
  QgsPostgresResult result( conn->PQexec( sql ) );
  if ( result.PQresultStatus() != PGRES_TUPLES_OK )
  {
    errCause = QObject::tr( "Unable to delete layer %1: \n%2" )
               .arg( schemaTableName,
                     result.PQresultErrorMessage() );
    conn->unref();
    return false;
  }

  int count = result.PQgetvalue( 0, 0 ).toInt();

  if ( !geometryCol.isEmpty() && count > 1 )
  {
    // the table has more geometry columns, drop just the geometry column
    sql = QString( "SELECT DropGeometryColumn(%1,%2,%3)" )
          .arg( QgsPostgresConn::quotedValue( schemaName ),
                QgsPostgresConn::quotedValue( tableName ),
                QgsPostgresConn::quotedValue( geometryCol ) );
  }
  else
  {
    // drop the table
    sql = QString( "SELECT DropGeometryTable(%1,%2)" )
          .arg( QgsPostgresConn::quotedValue( schemaName ),
                QgsPostgresConn::quotedValue( tableName ) );
  }

  result = conn->PQexec( sql );
  if ( result.PQresultStatus() != PGRES_TUPLES_OK )
  {
    errCause = QObject::tr( "Unable to delete layer %1: \n%2" )
               .arg( schemaTableName,
                     result.PQresultErrorMessage() );
    conn->unref();
    return false;
  }

  conn->unref();
  return true;
}

QGISEXTERN bool deleteSchema( const QString& schema, const QgsDataSourceURI& uri, QString& errCause, bool cascade = false )
{
  QgsDebugMsg( "deleting schema " + schema );

  if ( schema.isEmpty() )
    return false;

  QString schemaName = QgsPostgresConn::quotedIdentifier( schema );

  QgsPostgresConn *conn = QgsPostgresConn::connectDb( uri.connectionInfo( false ), false );
  if ( !conn )
  {
    errCause = QObject::tr( "Connection to database failed" );
    return false;
  }

  // drop the schema
  QString sql = QString( "DROP SCHEMA %1 %2" )
                .arg( schemaName, cascade ? QString( "CASCADE" ) : QString() );

  QgsPostgresResult result( conn->PQexec( sql ) );
  if ( result.PQresultStatus() != PGRES_COMMAND_OK )
  {
    errCause = QObject::tr( "Unable to delete schema %1: \n%2" )
               .arg( schemaName,
                     result.PQresultErrorMessage() );
    conn->unref();
    return false;
  }

  conn->unref();
  return true;
}

QGISEXTERN bool saveStyle( const QString& uri, const QString& qmlStyle, const QString& sldStyle,
                           const QString& styleName, const QString& styleDescription,
                           const QString& uiFileContent, bool useAsDefault, QString& errCause )
{
  QgsDataSourceURI dsUri( uri );

  QgsPostgresConn *conn = QgsPostgresConn::connectDb( dsUri.connectionInfo( false ), false );
  if ( !conn )
  {
    errCause = QObject::tr( "Connection to database failed" );
    return false;
  }

  QgsPostgresResult res( conn->PQexec( "SELECT COUNT(*) FROM information_schema.tables WHERE table_name='layer_styles'" ) );
  if ( res.PQgetvalue( 0, 0 ).toInt() == 0 )
  {
    res = conn->PQexec( "CREATE TABLE layer_styles("
                        "id SERIAL PRIMARY KEY"
                        ",f_table_catalog varchar"
                        ",f_table_schema varchar"
                        ",f_table_name varchar"
                        ",f_geometry_column varchar"
                        ",styleName varchar(30)"
                        ",styleQML xml"
                        ",styleSLD xml"
                        ",useAsDefault boolean"
                        ",description text"
                        ",owner varchar(30)"
                        ",ui xml"
                        ",update_time timestamp DEFAULT CURRENT_TIMESTAMP"
                        ")" );
    if ( res.PQresultStatus() != PGRES_COMMAND_OK )
    {
      errCause = QObject::tr( "Unable to save layer style. It's not possible to create the destination table on the database. Maybe this is due to table permissions (user=%1). Please contact your database admin" ).arg( dsUri.username() );
      conn->unref();
      return false;
    }
  }

  QString uiFileColumn;
  QString uiFileValue;
  if ( !uiFileContent.isEmpty() )
  {
    uiFileColumn = ",ui";
    uiFileValue = QString( ",XMLPARSE(DOCUMENT %1)" ).arg( QgsPostgresConn::quotedValue( uiFileContent ) );
  }

  // Note: in the construction of the INSERT and UPDATE strings the qmlStyle and sldStyle values
  // can contain user entered strings, which may themselves include %## values that would be
  // replaced by the QString.arg function.  To ensure that the final SQL string is not corrupt these
  // two values are both replaced in the final .arg call of the string construction.

  QString sql = QString( "INSERT INTO layer_styles("
                         "f_table_catalog,f_table_schema,f_table_name,f_geometry_column,styleName,styleQML,styleSLD,useAsDefault,description,owner%11"
                         ") VALUES ("
                         "%1,%2,%3,%4,%5,XMLPARSE(DOCUMENT %16),XMLPARSE(DOCUMENT %17),%8,%9,%10%12"
                         ")" )
                .arg( QgsPostgresConn::quotedValue( dsUri.database() ) )
                .arg( QgsPostgresConn::quotedValue( dsUri.schema() ) )
                .arg( QgsPostgresConn::quotedValue( dsUri.table() ) )
                .arg( QgsPostgresConn::quotedValue( dsUri.geometryColumn() ) )
                .arg( QgsPostgresConn::quotedValue( styleName.isEmpty() ? dsUri.table() : styleName ) )
                .arg( useAsDefault ? "true" : "false" )
                .arg( QgsPostgresConn::quotedValue( styleDescription.isEmpty() ? QDateTime::currentDateTime().toString() : styleDescription ) )
                .arg( QgsPostgresConn::quotedValue( dsUri.username() ) )
                .arg( uiFileColumn )
                .arg( uiFileValue )
                // Must be the final .arg replacement - see above
                .arg( QgsPostgresConn::quotedValue( qmlStyle ),
                      QgsPostgresConn::quotedValue( sldStyle ) );

  QString checkQuery = QString( "SELECT styleName"
                                " FROM layer_styles"
                                " WHERE f_table_catalog=%1"
                                " AND f_table_schema=%2"
                                " AND f_table_name=%3"
                                " AND f_geometry_column=%4"
                                " AND styleName=%5" )
                       .arg( QgsPostgresConn::quotedValue( dsUri.database() ) )
                       .arg( QgsPostgresConn::quotedValue( dsUri.schema() ) )
                       .arg( QgsPostgresConn::quotedValue( dsUri.table() ) )
                       .arg( QgsPostgresConn::quotedValue( dsUri.geometryColumn() ) )
                       .arg( QgsPostgresConn::quotedValue( styleName.isEmpty() ? dsUri.table() : styleName ) );

  res = conn->PQexec( checkQuery );
  if ( res.PQntuples() > 0 )
  {
    if ( QMessageBox::question( nullptr, QObject::tr( "Save style in database" ),
                                QObject::tr( "A style named \"%1\" already exists in the database for this layer. Do you want to overwrite it?" )
                                .arg( styleName.isEmpty() ? dsUri.table() : styleName ),
                                QMessageBox::Yes | QMessageBox::No ) == QMessageBox::No )
    {
      errCause = QObject::tr( "Operation aborted. No changes were made in the database" );
      conn->unref();
      return false;
    }

    sql = QString( "UPDATE layer_styles"
                   " SET useAsDefault=%1"
                   ",styleQML=XMLPARSE(DOCUMENT %12)"
                   ",styleSLD=XMLPARSE(DOCUMENT %13)"
                   ",description=%4"
                   ",owner=%5"
                   " WHERE f_table_catalog=%6"
                   " AND f_table_schema=%7"
                   " AND f_table_name=%8"
                   " AND f_geometry_column=%9"
                   " AND styleName=%10" )
          .arg( useAsDefault ? "true" : "false" )
          .arg( QgsPostgresConn::quotedValue( styleDescription.isEmpty() ? QDateTime::currentDateTime().toString() : styleDescription ) )
          .arg( QgsPostgresConn::quotedValue( dsUri.username() ) )
          .arg( QgsPostgresConn::quotedValue( dsUri.database() ) )
          .arg( QgsPostgresConn::quotedValue( dsUri.schema() ) )
          .arg( QgsPostgresConn::quotedValue( dsUri.table() ) )
          .arg( QgsPostgresConn::quotedValue( dsUri.geometryColumn() ) )
          .arg( QgsPostgresConn::quotedValue( styleName.isEmpty() ? dsUri.table() : styleName ) )
          // Must be the final .arg replacement - see above
          .arg( QgsPostgresConn::quotedValue( qmlStyle ),
                QgsPostgresConn::quotedValue( sldStyle ) );
  }

  if ( useAsDefault )
  {
    QString removeDefaultSql = QString( "UPDATE layer_styles"
                                        " SET useAsDefault=false"
                                        " WHERE f_table_catalog=%1"
                                        " AND f_table_schema=%2"
                                        " AND f_table_name=%3"
                                        " AND f_geometry_column=%4" )
                               .arg( QgsPostgresConn::quotedValue( dsUri.database() ) )
                               .arg( QgsPostgresConn::quotedValue( dsUri.schema() ) )
                               .arg( QgsPostgresConn::quotedValue( dsUri.table() ) )
                               .arg( QgsPostgresConn::quotedValue( dsUri.geometryColumn() ) );
    sql = QString( "BEGIN; %1; %2; COMMIT;" ).arg( removeDefaultSql, sql );
  }

  res = conn->PQexec( sql );

  bool saved = res.PQresultStatus() == PGRES_COMMAND_OK;
  if ( !saved )
    errCause = QObject::tr( "Unable to save layer style. It's not possible to insert a new record into the style table. Maybe this is due to table permissions (user=%1). Please contact your database administrator." ).arg( dsUri.username() );

  conn->unref();

  return saved;
}


QGISEXTERN QString loadStyle( const QString& uri, QString& errCause )
{
  QgsDataSourceURI dsUri( uri );

  QgsPostgresConn *conn = QgsPostgresConn::connectDb( dsUri.connectionInfo( false ), false );
  if ( !conn )
  {
    errCause = QObject::tr( "Connection to database failed" );
    return "";
  }

  QgsPostgresResult result( conn->PQexec( "SELECT COUNT(*) FROM information_schema.tables WHERE table_name='layer_styles'" ) );
  if ( result.PQgetvalue( 0, 0 ).toInt() == 0 )
  {
    return "";
  }

  QString selectQmlQuery = QString( "SELECT styleQML"
                                    " FROM layer_styles"
                                    " WHERE f_table_catalog=%1"
                                    " AND f_table_schema=%2"
                                    " AND f_table_name=%3"
                                    " AND f_geometry_column=%4"
                                    " ORDER BY CASE WHEN useAsDefault THEN 1 ELSE 2 END"
                                    ",update_time DESC LIMIT 1" )
                           .arg( QgsPostgresConn::quotedValue( dsUri.database() ) )
                           .arg( QgsPostgresConn::quotedValue( dsUri.schema() ) )
                           .arg( QgsPostgresConn::quotedValue( dsUri.table() ) )
                           .arg( QgsPostgresConn::quotedValue( dsUri.geometryColumn() ) );

  result = conn->PQexec( selectQmlQuery );

  QString style = result.PQntuples() == 1 ? result.PQgetvalue( 0, 0 ) : "";
  conn->unref();

  return style;
}

QGISEXTERN int listStyles( const QString &uri, QStringList &ids, QStringList &names,
                           QStringList &descriptions, QString& errCause )
{
  QgsDataSourceURI dsUri( uri );

  QgsPostgresConn *conn = QgsPostgresConn::connectDb( dsUri.connectionInfo( false ), false );
  if ( !conn )
  {
    errCause = QObject::tr( "Connection to database failed using username: %1" ).arg( dsUri.username() );
    return -1;
  }

  QString selectRelatedQuery = QString( "SELECT id,styleName,description"
                                        " FROM layer_styles"
                                        " WHERE f_table_catalog=%1"
                                        " AND f_table_schema=%2"
                                        " AND f_table_name=%3"
                                        " AND f_geometry_column=%4" )
                               .arg( QgsPostgresConn::quotedValue( dsUri.database() ) )
                               .arg( QgsPostgresConn::quotedValue( dsUri.schema() ) )
                               .arg( QgsPostgresConn::quotedValue( dsUri.table() ) )
                               .arg( QgsPostgresConn::quotedValue( dsUri.geometryColumn() ) );

  QgsPostgresResult result( conn->PQexec( selectRelatedQuery ) );
  if ( result.PQresultStatus() != PGRES_TUPLES_OK )
  {
    QgsMessageLog::logMessage( QObject::tr( "Error executing query: %1" ).arg( selectRelatedQuery ) );
    errCause = QObject::tr( "Error executing the select query for related styles. The query was logged" );
    conn->unref();
    return -1;
  }

  int numberOfRelatedStyles = result.PQntuples();
  for ( int i = 0; i < numberOfRelatedStyles; i++ )
  {
    ids.append( result.PQgetvalue( i, 0 ) );
    names.append( result.PQgetvalue( i, 1 ) );
    descriptions.append( result.PQgetvalue( i, 2 ) );
  }

  QString selectOthersQuery = QString( "SELECT id,styleName,description"
                                       " FROM layer_styles"
                                       " WHERE NOT (f_table_catalog=%1 AND f_table_schema=%2 AND f_table_name=%3 AND f_geometry_column=%4)"
                                       " ORDER BY update_time DESC" )
                              .arg( QgsPostgresConn::quotedValue( dsUri.database() ) )
                              .arg( QgsPostgresConn::quotedValue( dsUri.schema() ) )
                              .arg( QgsPostgresConn::quotedValue( dsUri.table() ) )
                              .arg( QgsPostgresConn::quotedValue( dsUri.geometryColumn() ) );

  result = conn->PQexec( selectOthersQuery );
  if ( result.PQresultStatus() != PGRES_TUPLES_OK )
  {
    QgsMessageLog::logMessage( QObject::tr( "Error executing query: %1" ).arg( selectOthersQuery ) );
    errCause = QObject::tr( "Error executing the select query for unrelated styles. The query was logged" );
    conn->unref();
    return -1;
  }

  for ( int i = 0; i < result.PQntuples(); i++ )
  {
    ids.append( result.PQgetvalue( i, 0 ) );
    names.append( result.PQgetvalue( i, 1 ) );
    descriptions.append( result.PQgetvalue( i, 2 ) );
  }

  conn->unref();

  return numberOfRelatedStyles;
}

QGISEXTERN QString getStyleById( const QString& uri, QString styleId, QString& errCause )
{
  QgsDataSourceURI dsUri( uri );

  QgsPostgresConn *conn = QgsPostgresConn::connectDb( dsUri.connectionInfo( false ), false );
  if ( !conn )
  {
    errCause = QObject::tr( "Connection to database failed using username: %1" ).arg( dsUri.username() );
    return "";
  }

  QString style;
  QString selectQmlQuery = QString( "SELECT styleQml FROM layer_styles WHERE id=%1" ).arg( QgsPostgresConn::quotedValue( styleId ) );
  QgsPostgresResult result( conn->PQexec( selectQmlQuery ) );
  if ( result.PQresultStatus() == PGRES_TUPLES_OK )
  {
    if ( result.PQntuples() == 1 )
      style = result.PQgetvalue( 0, 0 );
    else
      errCause = QObject::tr( "Consistency error in table '%1'. Style id should be unique" ).arg( "layer_styles" );
  }
  else
  {
    QgsMessageLog::logMessage( QObject::tr( "Error executing query: %1" ).arg( selectQmlQuery ) );
    errCause = QObject::tr( "Error executing the select query. The query was logged" );
  }

  conn->unref();

  return style;
}

QGISEXTERN QgsTransaction* createTransaction( const QString& connString )
{
  return new QgsPostgresTransaction( connString );
}

QGISEXTERN void cleanupProvider()
{
  QgsPostgresConnPool::cleanupInstance();
}

// ----------

QgsPostgresSharedData::QgsPostgresSharedData()
    : mFeaturesCounted( -1 )
    , mFidCounter( 0 )
{
}

void QgsPostgresSharedData::addFeaturesCounted( long diff )
{
  QMutexLocker locker( &mMutex );

  if ( mFeaturesCounted >= 0 )
    mFeaturesCounted += diff;
}

void QgsPostgresSharedData::ensureFeaturesCountedAtLeast( long fetched )
{
  QMutexLocker locker( &mMutex );

  /* only updates the feature count if it was already once.
   * Otherwise, this would lead to false feature count if
   * an existing project is open at a restrictive extent.
   */
  if ( mFeaturesCounted > 0 && mFeaturesCounted < fetched )
  {
    QgsDebugMsg( QString( "feature count adjusted from %1 to %2" ).arg( mFeaturesCounted ).arg( fetched ) );
    mFeaturesCounted = fetched;
  }
}

long QgsPostgresSharedData::featuresCounted()
{
  QMutexLocker locker( &mMutex );
  return mFeaturesCounted;
}

void QgsPostgresSharedData::setFeaturesCounted( long count )
{
  QMutexLocker locker( &mMutex );
  mFeaturesCounted = count;
}


QgsFeatureId QgsPostgresSharedData::lookupFid( const QVariant &v )
{
  QMutexLocker locker( &mMutex );

  QMap<QVariant, QgsFeatureId>::const_iterator it = mKeyToFid.constFind( v );

  if ( it != mKeyToFid.constEnd() )
  {
    return it.value();
  }

  mFidToKey.insert( ++mFidCounter, v );
  mKeyToFid.insert( v, mFidCounter );

  return mFidCounter;
}


QVariant QgsPostgresSharedData::removeFid( QgsFeatureId fid )
{
  QMutexLocker locker( &mMutex );

  QVariant v = mFidToKey[ fid ];
  mFidToKey.remove( fid );
  mKeyToFid.remove( v );
  return v;
}

void QgsPostgresSharedData::insertFid( QgsFeatureId fid, const QVariant& k )
{
  QMutexLocker locker( &mMutex );

  mFidToKey.insert( fid, k );
  mKeyToFid.insert( k, fid );
}

QVariant QgsPostgresSharedData::lookupKey( QgsFeatureId featureId )
{
  QMutexLocker locker( &mMutex );

  QMap<QgsFeatureId, QVariant>::const_iterator it = mFidToKey.constFind( featureId );
  if ( it != mFidToKey.constEnd() )
    return it.value();
  return QVariant();
}
