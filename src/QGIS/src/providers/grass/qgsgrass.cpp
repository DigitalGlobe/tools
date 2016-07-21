/***************************************************************************
    qgsgrass.cpp  -  Data provider for GRASS format
                             -------------------
    begin                : March, 2004
    copyright            : (C) 2004 by Radim Blazek
    email                : blazek@itc.it
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef _MSC_VER
// to avoid conflicting SF_UNKNOWN
#define _OLE2_H_
#endif

#include <setjmp.h>

// for Sleep / usleep for debugging
#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <time.h>
#endif

#include "qgsgrass.h"
#include "qgsgrassoptions.h"
#include "qgsgrassvector.h"

#include "qgsapplication.h"
#include "qgscrscache.h"
#include "qgsconfig.h"
#include "qgscoordinatereferencesystem.h"
#include "qgsfield.h"
#include "qgslocalec.h"
#include "qgslogger.h"
#include "qgsproject.h"
#include "qgsrectangle.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QTextStream>
#include <QTemporaryFile>
#include <QHash>

#include <QTextCodec>

extern "C"
{
#ifndef Q_OS_WIN
#include <unistd.h>
#include <sys/types.h>
#endif
#include <grass/version.h>
#if defined(_MSC_VER) && defined(M_PI_4)
#undef M_PI_4 //avoid redefinition warning
#endif
#include <grass/gprojects.h>

#if GRASS_VERSION_MAJOR < 7
#include <grass/Vect.h>
#else
#include <grass/vector.h>
#include <grass/raster.h>
#endif
}

#if GRASS_VERSION_MAJOR >= 7
#define G_get_gdal_link Rast_get_gdal_link
#define G_close_gdal_link Rast_close_gdal_link
#endif

#if !defined(GRASS_VERSION_MAJOR) || \
    !defined(GRASS_VERSION_MINOR) || \
    GRASS_VERSION_MAJOR<6 || \
    (GRASS_VERSION_MAJOR == 6 && GRASS_VERSION_MINOR <= 2)
#define G__setenv(name,value) G__setenv( ( char * ) (name), (char *) (value) )
#endif

#define GRASS_LOCK sMutex.lock();
#define GRASS_UNLOCK sMutex.unlock();

QgsGrassObject::QgsGrassObject( const QString& gisdbase, const QString& location,
                                const QString& mapset, const QString& name, Type type )
    : mGisdbase( gisdbase )
    , mLocation( location )
    , mMapset( mapset )
    , mName( name )
    , mType( type )
{
}

QString QgsGrassObject::fullName() const
{
  if ( mName.isEmpty() )
  {
    return QString();
  }
  if ( mMapset.isEmpty() )
  {
    return mName;
  }
  return mName + "@" + mMapset;
}

void QgsGrassObject::setFullName( const QString& fullName )
{
  QStringList parts = fullName.split( '@' );
  mName = parts.value( 0 );
  mMapset.clear();
  if ( !fullName.isEmpty() )
  {
    mMapset = parts.size() > 1 ? parts.value( 1 ) : QgsGrass::getDefaultMapset();
  }
}

bool QgsGrassObject::setFromUri( const QString& uri )
{
  QgsDebugMsg( "uri = " + uri );
  QFileInfo fi( uri );

  if ( fi.isFile() )
  {
    QString path = fi.canonicalFilePath();
    QgsDebugMsg( "path = " + path );
    // /gisdbase_path/location/mapset/cellhd/raster_map
    QRegExp rx( "(.*)/([^/]*)/([^/]*)/cellhd/([^/]*)", Qt::CaseInsensitive );
    if ( rx.indexIn( path ) > -1 )
    {
      mGisdbase = rx.cap( 1 );
      mLocation = rx.cap( 2 );
      mMapset = rx.cap( 3 );
      mName = rx.cap( 4 );
      mType = Raster;
      return QgsGrass::isLocation( mGisdbase + "/" + mLocation );
    }
  }
  else
  {
    // /gisdbase_path/location/mapset/vector_map/layer
    // QFileInfo.canonicalPath() on non existing file does not work (returns empty string)
    // QFileInfo.absolutePath() does not necessarily remove symbolic links or redundant "." or ".."
    QDir dir = fi.dir(); // .../mapset/vector_map - does not exist
    if ( dir.cdUp() ) // .../mapset/
    {
      QString path = dir.canonicalPath();
      QRegExp rx( "(.*)/([^/]*)/([^/]*)" );
      if ( rx.indexIn( path ) > -1 )
      {
        mGisdbase = rx.cap( 1 );
        mLocation = rx.cap( 2 );
        mMapset = rx.cap( 3 );
        mName = fi.dir().dirName();
        mType = Vector;
        QgsDebugMsg( "parsed : " + toString() );
        return QgsGrass::isLocation( mGisdbase + "/" + mLocation );
      }
    }
  }
  return false;
}

QString QgsGrassObject::elementShort() const
{
  return elementShort( mType );
}

QString QgsGrassObject::elementShort( Type type )
{
  if ( type == Raster )
#if GRASS_VERSION_MAJOR < 7
    return "rast";
#else
    return "raster";
#endif
  else if ( type == Group )
    return "group";
  else if ( type == Vector )
#if GRASS_VERSION_MAJOR < 7
    return "vect";
#else
    return "vector";
#endif
  else if ( type == Region )
    return "region";
  else if ( type == Strds )
    return "strds";
  else if ( type == Stvds )
    return "stvds";
  else if ( type == Str3ds )
    return "str3ds";
  else if ( type == Stds )
    return "stds";
  else
    return "";
}

QString QgsGrassObject::elementName() const
{
  return elementName( mType );
}

QString QgsGrassObject::elementName( Type type )
{
  if ( type == Raster )
    return "raster";
  else if ( type == Group )
    return "group";
  else if ( type == Vector )
    return "vector";
  else if ( type == Region )
    return "region";
  else
    return "";
}

QString QgsGrassObject::dirName() const
{
  return dirName( mType );
}

QString QgsGrassObject::dirName( Type type )
{
  if ( type == Raster )
    return "cellhd";
  else if ( type == Group )
    return "group";
  else if ( type == Vector )
    return "vector";
  else if ( type == Region )
    return "windows";
  else
    return "";
}

QString QgsGrassObject::toString() const
{
  return elementName() + " : " + mapsetPath() + " : " + mName;
}

bool QgsGrassObject::locationIdentical( const QgsGrassObject &other ) const
{
  QFileInfo fi( locationPath() );
  QFileInfo otherFi( other.locationPath() );
  return fi == otherFi;
}

bool QgsGrassObject::mapsetIdentical( const QgsGrassObject &other ) const
{
  QFileInfo fi( mapsetPath() );
  QFileInfo otherFi( other.mapsetPath() );
  return fi == otherFi;
}

QRegExp QgsGrassObject::newNameRegExp( Type type )
{
  QRegExp rx;
  if ( type == QgsGrassObject::Vector )
  {
    rx.setPattern( "[A-Za-z_][A-Za-z0-9_]+" );
  }
  else // location, raster, see G_legal_filename
  {
    rx.setPattern( "[\\w_\\-][\\w_\\-.]+" );
  }
  return rx;
}

bool QgsGrassObject::operator==( const QgsGrassObject& other ) const
{
  return mGisdbase == other.mGisdbase && mLocation == other.mLocation && mMapset == other.mMapset
         && mName == other.mName && mType == other.mType;
}

QgsGrass::QgsGrass()
    : mMapsetSearchPathWatcher( 0 )
{
}

QString QgsGrass::pathSeparator()
{
#ifdef Q_OS_WIN
  return ";";
#else
  return ":";
#endif
}

#ifdef Q_OS_WIN
#include <windows.h>
QString QgsGrass::shortPath( const QString &path )
{
  TCHAR buf[MAX_PATH];
  int len = GetShortPathName( path.toUtf8().constData(), buf, MAX_PATH );

  if ( len == 0 || len > MAX_PATH )
  {
    QgsDebugMsg( QString( "GetShortPathName('%1') failed with %2: %3" )
                 .arg( path ).arg( len ).arg( GetLastError() ) );
    return path;
  }

  QString res = QString::fromUtf8( buf );
  // GRASS wxpyton GUI fails on paths with backslash, Windows do support forward slashesin paths
  res.replace( "\\", "/" );
  return res;
}
#endif

bool QgsGrass::init( void )
{
  // Do not show warning dialog in this function, it may cause problems in non interactive tests etc.

  // Warning!!!
  // G_set_error_routine() once called from plugin
  // is not valid in provider -> call it always

  if ( mNonInitializable )
  {
    return false;
  }

  if ( initialized )
  {
    return true;
  }

  // Set error function
  G_set_error_routine( &error_routine );

  lock();
  QgsDebugMsg( "do init" );

  active = false;
  // Is it active mode ?
  if ( getenv( "GISRC" ) )
  {
    G_TRY
    {
      // Store default values
      defaultGisdbase = G_gisdbase();
      defaultLocation = G_location();
      defaultMapset = G_mapset();
      active = true;
    }
    G_CATCH( QgsGrass::Exception &e )
    {
      QgsDebugMsg( QString( "GISRC set but cannot get gisdbase/location/mapset: %1" ).arg( e.what() ) );
    }
  }

  // Don't use GISRC file and read/write GRASS variables (from location G_VAR_GISRC) to memory only.
  G_set_gisrc_mode( G_GISRC_MODE_MEMORY );

  // Init GRASS libraries (required)
  // G_no_gisinit() may end with fatal error if QGIS is run with a version of GRASS different from that used for compilation
  G_TRY
  {
    G_no_gisinit();  // Doesn't check write permissions for mapset compare to G_gisinit("libgrass++");
  }
  G_CATCH( QgsGrass::Exception &e )
  {
    mInitError = tr( "Problem in GRASS initialization, GRASS provider and plugin will not work : %1" ).arg( e.what() );
    QgsDebugMsg( mInitError );
    mNonInitializable = true;
    unlock();
    return false;
  }

  // I think that mask should not be used in QGIS as it can confuse users,
  // anyway, I don't think anybody is using MASK
  // TODO7: Rast_suppress_masking (see G_suppress_masking() macro above) needs MAPSET
  // (it should not be necessary, because rasters are read by modules qgis.g.info and qgis.d.rast
  //  where masking is suppressed)
#if GRASS_VERSION_MAJOR < 7
  G_suppress_masking();
#endif

  // Set program name
  G_set_program_name( "QGIS" );

  // Require GISBASE to be set. This should point to the location of
  // the GRASS installation. The GRASS libraries use it to know
  // where to look for things.
  if ( !isValidGrassBaseDir( gisbase() ) )
  {
    mNonInitializable = true;
    mInitError = tr( "GRASS was not found in '%1' (GISBASE), provider and plugin will not work." ).arg( gisbase() );
    QgsDebugMsg( mInitError );
#if 0
    // TODO: how to emit message from provider (which does not know about QgisApp)
    QgisApp::instance()->messageBar()->pushMessage( tr( "GRASS error" ),
      error_message, QgsMessageBar: WARNING );
#endif
    unlock();
    return false;
  }
  else
  {
    QgsDebugMsg( "Valid GRASS gisbase is: " + gisbase() );
    // GISBASE environment variable must be set because is required by directly called GRASS functions
    putEnv( "GISBASE", gisbase() );

    // Create list of paths to GRASS modules
    // PATH environment variable is not used to search for modules (since 2.12) because it could
    // create a lot of confusion especially if both GRASS 6 and 7 are installed and path to one version
    // $GISBASE/bin somehow gets to PATH and another version plugin is loaded to QGIS, because if a module
    // is missing in one version, it could be found in another $GISBASE/bin and misleadin error could be reported
    mGrassModulesPaths.clear();
    mGrassModulesPaths << gisbase() + "/bin";
    mGrassModulesPaths << gisbase() + "/scripts";
    mGrassModulesPaths << QgsApplication::pkgDataPath() + "/grass/scripts";
    mGrassModulesPaths << qgisGrassModulePath();

    // On windows the GRASS libraries are in
    // QgsApplication::prefixPath(), we have to add them
    // to PATH to enable running of GRASS modules
    // and database drivers
#ifdef Q_OS_WIN
    // It seems that QgsApplication::prefixPath() is not initialized at this point
    // TODO: verify if this is required and why (PATH to required libs should be set in qgis-grass.bat)
    //mGrassModulesPaths << shortPath( QCoreApplication::applicationDirPath() ) );

    // Add path to MSYS bin
    // Warning: MSYS sh.exe will translate this path to '/bin'
    QString msysBin = QCoreApplication::applicationDirPath() + "/msys/bin/";
    if ( QFileInfo( msysBin ).isDir() )
    {
      mGrassModulesPaths << shortPath( QCoreApplication::applicationDirPath() + "/msys/bin/" );
    }
#endif

    //QString p = getenv( "PATH" );
    //path.append( sep + p );

    QgsDebugMsg( "mGrassModulesPaths = " + mGrassModulesPaths.join( "," ) );
    //putEnv( "PATH", path );

    // TODO: move where it is required for QProcess
    // Set GRASS_PAGER if not set, it is necessary for some
    // modules printing to terminal, e.g. g.list
    // We use 'cat' because 'more' is not present in MSYS (Win)
    // and it doesn't work well in built in shell (Unix/Mac)
    // and 'less' is not user friendly (for example user must press
    // 'q' to quit which is definitely difficult for normal user)
    // Also scroling can be don in scrollable window in both
    // MSYS terminal and built in shell.
    if ( !getenv( "GRASS_PAGER" ) )
    {
      QString pager;
      QStringList pagers;
      //pagers << "more" << "less" << "cat"; // se notes above
      pagers << "cat";

      for ( int i = 0; i < pagers.size(); i++ )
      {
        int state;

        QProcess p;
        p.start( pagers.at( i ) );
        p.waitForStarted();
        state = p.state();
        p.write( "\004" ); // Ctrl-D
        p.closeWriteChannel();
        p.waitForFinished( 1000 );
        p.kill();

        if ( state == QProcess::Running )
        {
          pager = pagers.at( i );
          break;
        }
      }

      if ( pager.length() > 0 )
      {
        putEnv( "GRASS_PAGER", pager );
      }
    }
    initialized = 1;
  }

  unlock();

  // after unlock because it is using setMapset() which calls init()
  if ( active )
  {
    QgsGrass::instance()->loadMapsetSearchPath(); // must be after G_no_gisinit()
    QgsGrass::instance()->setMapsetSearchPathWatcher();
  }

  return true;
}

/*
 * Check if given directory contains a GRASS installation
 */
bool QgsGrass::isValidGrassBaseDir( const QString& gisbase )
{
  QgsDebugMsg( "isValidGrassBaseDir()" );
  // GRASS currently doesn't handle paths with blanks
  if ( gisbase.isEmpty() || gisbase.contains( " " ) )
  {
    return false;
  }

  /* TODO: G_is_gisbase() was added to GRASS 6.1 06-05-24,
           enable its use after some period (others do update) */
#if 0
  if ( QgsGrass::versionMajor() > 6 || QgsGrass::versionMinor() > 0 )
  {
    if ( G_is_gisbase( gisbase.toUtf8().constData() ) )
      return true;
  }
  else
  {
#endif
    QFileInfo gbi( gisbase + "/etc/element_list" );
    if ( gbi.exists() )
      return true;
#if 0
  }
#endif
  return false;
}

QgsGrass *QgsGrass::instance()
{
  static QgsGrass sInstance;
  return &sInstance;
}

void QgsGrass::lock()
{
  QgsDebugMsg( "lock" );
  sMutex.lock();
}

void QgsGrass::unlock()
{
  QgsDebugMsg( "unlock" );
  sMutex.unlock();
}

bool QgsGrass::activeMode()
{
  return active;
}

QString QgsGrass::getDefaultGisdbase()
{
  return defaultGisdbase;
}

QString QgsGrass::getDefaultLocation()
{
  return defaultLocation;
}

QgsGrassObject QgsGrass::getDefaultLocationObject()
{
  return QgsGrassObject( defaultGisdbase, defaultLocation, "", "", QgsGrassObject::Location );
}

QString QgsGrass::getDefaultLocationPath()
{
  if ( !active )
  {
    return QString();
  }
  return defaultGisdbase + "/" + defaultLocation;
}

QString QgsGrass::getDefaultMapset()
{
  return defaultMapset;
}

QgsGrassObject QgsGrass::getDefaultMapsetObject()
{
  return QgsGrassObject( defaultGisdbase, defaultLocation, defaultMapset, "", QgsGrassObject::Mapset );
}

QString QgsGrass::getDefaultMapsetPath()
{
  return getDefaultLocationPath() + "/" + defaultMapset;
}

void QgsGrass::setLocation( QString gisdbase, QString location )
{
  QgsDebugMsg( QString( "gisdbase = %1 location = %2" ).arg( gisdbase, location ) );
  setMapset( gisdbase, location, "PERMANENT" );
}

void QgsGrass::setMapset( QString gisdbase, QString location, QString mapset )
{
  QgsDebugMsg( QString( "gisdbase = %1 location = %2 mapset = %3" ).arg( gisdbase, location, mapset ) );
  if ( !init() )
  {
    QgsDebugMsg( QgsGrass::initError() );
    return;
  }

  // Set principal GRASS variables (in memory)
#ifdef Q_OS_WIN
  G__setenv( "GISDBASE", shortPath( gisdbase ).toUtf8().data() );
#else
  G__setenv( "GISDBASE", gisdbase.toUtf8().data() );
#endif
  G__setenv( "LOCATION_NAME", location.toUtf8().data() );
  G__setenv( "MAPSET", mapset.toUtf8().data() );

  // Add all available mapsets to search path
  // Why? Other mapsets should not be necessary.
#if 0
  // G_get_available_mapsets in GRASS 6 returs pointer to memory managed by GRASS,
  // in GRASS 7 it always allocates new memory and caller must free that
  char **ms = 0;
  G_TRY
  {
    ms = G_available_mapsets();
  }
  G_CATCH( QgsGrass::Exception &e )
  {
    Q_UNUSED( e );
    QgsDebugMsg( QString( "No available mapsets found: %1" ).arg( e.what() ) );
    return;
  }

  // There seems to be no function to clear search path
  for ( int i = 0; ms[i]; i++ )
  {
    G_add_mapset_to_search_path( ms[i] ); // only adds mapset if it is not yet in path
#if GRASS_VERSION_MAJOR >= 7
    free( ms[i] );
#endif
  }
#if GRASS_VERSION_MAJOR >= 7
  free( ms );
#endif
#endif
}

void QgsGrass::setMapset( QgsGrassObject grassObject )
{
  setMapset( grassObject.gisdbase(), grassObject.location(), grassObject.mapset() );
}

bool QgsGrass::isMapsetInSearchPath( QString mapset )
{
  return mMapsetSearchPath.contains( mapset );
}

void QgsGrass::addMapsetToSearchPath( const QString & mapset, QString& error )
{
  QString cmd = gisbase() + "/bin/g.mapsets";
  QStringList arguments;

#if GRASS_VERSION_MAJOR < 7
  arguments << "addmapset=" + mapset;
#else
  arguments << "operation=add" << "mapset=" + mapset;
#endif

  try
  {
    int timeout = -1; // What timeout to use? It can take long time on network or database
    runModule( getDefaultGisdbase(), getDefaultLocation(), getDefaultMapset(), cmd, arguments, timeout, false );
  }
  catch ( QgsGrass::Exception &e )
  {
    error = tr( "Cannot add mapset %1 to search path:" ).arg( mapset ) + " " + e.what();
  }
}

void QgsGrass::removeMapsetFromSearchPath( const QString & mapset, QString& error )
{
  QString cmd = gisbase() + "/bin/g.mapsets";
  QStringList arguments;

#if GRASS_VERSION_MAJOR < 7
  arguments << "removemapset=" + mapset;
#else
  arguments << "operation=remove" << "mapset=" + mapset;
#endif

  try
  {
    int timeout = -1; // What timeout to use? It can take long time on network or database
    runModule( getDefaultGisdbase(), getDefaultLocation(), getDefaultMapset(), cmd, arguments, timeout, false );
  }
  catch ( QgsGrass::Exception &e )
  {
    error = tr( "Cannot remove mapset %1 from search path: %2" ).arg( mapset, e.what() );
  }
}

void QgsGrass::loadMapsetSearchPath()
{
  // do not lock, it is called from locked function
  QStringList oldMapsetSearchPath = mMapsetSearchPath;
  mMapsetSearchPath.clear();
  if ( !activeMode() )
  {
    QgsDebugMsg( "not active" );
    emit mapsetSearchPathChanged();
    return;
  }
  G_TRY
  {
    QgsGrass::setMapset( getDefaultGisdbase(), getDefaultLocation(), getDefaultMapset() );
    const char *mapset = 0;
#if GRASS_VERSION_MAJOR >= 7
    G_reset_mapsets();
    for ( int i = 0; ( mapset = G_get_mapset_name( i ) ); i++ )
#else
    int result = G_reset_mapsets();
    Q_UNUSED( result );
    for ( int i = 0; ( mapset = G__mapset_name( i ) ); i++ )
#endif
    {
      QgsDebugMsg( QString( "mapset = %1" ).arg( mapset ) );
      if ( G_is_mapset_in_search_path( mapset ) )
      {
        mMapsetSearchPath << mapset;
      }
    }
  }
  G_CATCH( QgsGrass::Exception &e )
  {
    QgsDebugMsg( "cannot load mapset search path: " + QString( e.what() ) );
  }
  QgsDebugMsg( "mMapsetSearchPath = " +  mMapsetSearchPath.join( "," ) );
  if ( mMapsetSearchPath != oldMapsetSearchPath )
  {
    emit mapsetSearchPathChanged();
  }
}

void QgsGrass::setMapsetSearchPathWatcher()
{
  QgsDebugMsg( "etered" );
  if ( mMapsetSearchPathWatcher )
  {
    delete mMapsetSearchPathWatcher;
    mMapsetSearchPathWatcher = 0;
  }
  if ( !activeMode() )
  {
    return;
  }
  mMapsetSearchPathWatcher = new QFileSystemWatcher( this );

  QString searchFilePath = getDefaultMapsetPath() + "/SEARCH_PATH";

  if ( QFileInfo( searchFilePath ).exists() )
  {
    QgsDebugMsg( "add watcher on SEARCH_PATH file " + searchFilePath );
    mMapsetSearchPathWatcher->addPath( searchFilePath );
    connect( mMapsetSearchPathWatcher, SIGNAL( fileChanged( const QString & ) ), SLOT( onSearchPathFileChanged( const QString & ) ) );
  }
  else
  {
    QgsDebugMsg( "add watcher on mapset " + getDefaultMapsetPath() );
    mMapsetSearchPathWatcher->addPath( getDefaultMapsetPath() );
    connect( mMapsetSearchPathWatcher, SIGNAL( directoryChanged( const QString & ) ), SLOT( onSearchPathFileChanged( const QString & ) ) );
  }
}

void QgsGrass::onSearchPathFileChanged( const QString & path )
{
  QgsDebugMsg( "path = " + path );
  QString searchFilePath = getDefaultMapsetPath() + "/SEARCH_PATH";
  if ( path == searchFilePath )
  {
    // changed or removed
    loadMapsetSearchPath();
    if ( !QFileInfo( searchFilePath ).exists() ) // removed
    {
      // reset watcher to mapset
      setMapsetSearchPathWatcher();
    }
  }
  else
  {
    // mapset directory changed
    if ( QFileInfo( searchFilePath ).exists() ) // search path file added
    {
      loadMapsetSearchPath();
      setMapsetSearchPathWatcher();
    }
  }
}

jmp_buf QgsGrass::jumper;

bool QgsGrass::mNonInitializable = false;
int QgsGrass::initialized = 0;

bool QgsGrass::active = 0;

QgsGrass::GERROR QgsGrass::lastError = QgsGrass::OK;

QString QgsGrass::error_message;
QString QgsGrass::mInitError;

QStringList QgsGrass::mGrassModulesPaths;
QString QgsGrass::defaultGisdbase;
QString QgsGrass::defaultLocation;
QString QgsGrass::defaultMapset;

QString QgsGrass::mMapsetLock;
QString QgsGrass::mGisrc;
QString QgsGrass::mTmp;

QMutex QgsGrass::sMutex;

bool QgsGrass::mMute = false;

int QgsGrass::error_routine( char *msg, int fatal )
{
  return error_routine(( const char* ) msg, fatal );
}

int QgsGrass::error_routine( const char *msg, int fatal )
{
  // G_fatal_error obviously is not thread safe (everything static in GRASS, especially fatal_jmp_buf)
  // it means that anything which may end up with G_fatal_error must use mutex

  // Unfortunately the exceptions thrown here can only be caught if GRASS libraries are compiled
  // with -fexception option on Linux (works on Windows)
  // GRASS developers are reluctant to add -fexception by default
  // https://trac.osgeo.org/grass/ticket/869
  QgsDebugMsg( QString( "error_routine (fatal = %1): %2" ).arg( fatal ).arg( msg ) );

  error_message = msg;

  if ( fatal )
  {
    QgsDebugMsg( "fatal -> longjmp" );
    // Exceptions cannot be thrown from here if GRASS lib is not compiled with -fexceptions
    //throw QgsGrass::Exception( QString::fromUtf8( msg ) );
    lastError = FATAL;

#if (GRASS_VERSION_MAJOR < 7)
    // longjump() is called by G_fatal_error in GRASS >= 7
    longjmp( QgsGrass::jumper, 1 );
#endif
  }
  else
  {
    lastError = WARNING;
  }

  return 1;
}

void QgsGrass::resetError( void )
{
  lastError = OK;
}

int QgsGrass::error( void )
{
  return lastError;
}

QString QgsGrass::errorMessage( void )
{
  return error_message;
}

bool QgsGrass::isOwner( const QString& gisdbase, const QString& location, const QString& mapset )
{
  QString mapsetPath = gisdbase + "/" + location + "/" + mapset;

  // G_mapset_permissions() (check_owner() in GRASS 7) on Windows consider all mapsets to be owned by user
  // There is more complex G_owner() but that is not used in G_gisinit() (7.1.svn).
  // On Windows and on systems where files do not have owners ownerId() returns ((uint) -2).
#ifndef Q_OS_WIN
  bool owner = QFileInfo( mapsetPath ).ownerId() == getuid();
#else
  bool owner = true;
#endif
  QgsDebugMsg( QString( "%1 : owner = %2" ).arg( mapsetPath ).arg( owner ) );
  return owner;
}

QString QgsGrass::openMapset( const QString& gisdbase,
                              const QString& location, const QString& mapset )
{
  QgsDebugMsg( QString( "gisdbase = %1" ).arg( gisdbase.toUtf8().constData() ) );
  QgsDebugMsg( QString( "location = %1" ).arg( location.toUtf8().constData() ) );
  QgsDebugMsg( QString( "mapset = %1" ).arg( mapset.toUtf8().constData() ) );

  closeMapset(); // close currently opened mapset (if any)

  QString mapsetPath = gisdbase + "/" + location + "/" + mapset;

  // Check if the mapset is in use
  if ( !isValidGrassBaseDir( gisbase() ) )
    return QObject::tr( "GISBASE is not set." );

  QFileInfo fi( mapsetPath + "/WIND" );
  if ( !fi.exists() )
  {
    return QObject::tr( "%1 is not a GRASS mapset." ).arg( mapsetPath );
  }

  QString lock = mapsetPath + "/.gislock";

#ifndef _MSC_VER
  int pid = getpid();
#else
  int pid = GetCurrentProcessId();
#endif

  QgsDebugMsg( QString( "pid = %1" ).arg( pid ) );

#ifndef Q_OS_WIN
  QFile lockFile( lock );
  QProcess process;
  QString lockProgram( gisbase() + "/etc/lock" );
  QStringList lockArguments;
  lockArguments << lock << QString::number( pid );
  QString lockCommand = lockProgram + " " + lockArguments.join( " " ); // for debug
  QgsDebugMsg( "lock command: " + lockCommand );

  process.start( lockProgram, lockArguments );
  if ( !process.waitForStarted( 5000 ) )
  {
    return QObject::tr( "Cannot start %1" ).arg( lockCommand );
  }
  process.waitForFinished( 5000 );

  QString processResult = QString( "exitStatus=%1, exitCode=%2, errorCode=%3, error=%4 stdout=%5, stderr=%6" )
                          .arg( process.exitStatus() ).arg( process.exitCode() )
                          .arg( process.error() ).arg( process.errorString(),
                                                       process.readAllStandardOutput().data(), process.readAllStandardError().data() );
  QgsDebugMsg( "processResult: " + processResult );

  // lock exit code:
  // 0 - ok
  // 1 - error
  // 2 - mapset in use
  if ( process.exitCode() == 2 )
  {
    return QObject::tr( "Mapset is already in use." );
  }

  if ( process.exitStatus() != QProcess::NormalExit || process.exitCode() != 0 )
  {
    QString message = QObject::tr( "Mapset lock failed (%1)" ).arg( processResult );
    return message;
  }

#endif // Q_OS_WIN

  // Create temporary directory
  QFileInfo info( mapsetPath );
  QString user = info.owner();

  mTmp = QDir::tempPath() + "/grass6-" + user + "-" + QString::number( pid );
  QDir dir( mTmp );
  if ( dir.exists() )
  {
    QFileInfo dirInfo( mTmp );
    if ( !dirInfo.isWritable() )
    {
#ifndef Q_OS_WIN
      lockFile.remove();
#endif
      return QObject::tr( "Temporary directory %1 exists but is not writable" ).arg( mTmp );
    }
  }
  else if ( !dir.mkdir( mTmp ) )
  {
#ifndef Q_OS_WIN
    lockFile.remove();
#endif
    return QObject::tr( "Cannot create temporary directory %1" ).arg( mTmp );
  }

  // Create GISRC file
  QString globalGisrc =  QDir::home().path() + "/.grassrc6";
  mGisrc = mTmp + "/gisrc";

  QgsDebugMsg( QString( "globalGisrc = %1" ).arg( globalGisrc ) );
  QgsDebugMsg( QString( "mGisrc = %1" ).arg( mGisrc ) );

  QFile out( mGisrc );
  if ( !out.open( QIODevice::WriteOnly ) )
  {
#ifndef Q_OS_WIN
    lockFile.remove();
#endif
    return QObject::tr( "Cannot create %1" ).arg( mGisrc );
  }
  QTextStream stream( &out );

  QFile in( globalGisrc );
  QString line;
  bool guiSet = false;
  char buf[1000];
  if ( in.open( QIODevice::ReadOnly ) )
  {
    while ( in.readLine( buf, 1000 ) != -1 )
    {
      line = buf;
      if ( line.contains( "GISDBASE:" ) ||
           line.contains( "LOCATION_NAME:" ) ||
           line.contains( "MAPSET:" ) )
      {
        continue;
      }
      if ( line.contains( "GRASS_GUI:" ) )
        guiSet = true;
      stream << line;
    }
    in.close();
  }
  line = "GISDBASE: " + gisdbase + "\n";
  stream << line;
  line = "LOCATION_NAME: " + location + "\n";
  stream << line;
  line = "MAPSET: " + mapset + "\n";
  stream << line;
  if ( !guiSet )
  {
    stream << "GRASS_GUI: wxpython\n";
  }

  out.close();

  // Set GISRC environment variable
  // Mapset must be set before Vect_close()
  /* _Correct_ putenv() implementation is not making copy! */
  putEnv( "GISRC", mGisrc );

  // Reinitialize GRASS
  G__setenv( "GISRC", mGisrc.toUtf8().data() );
#ifdef Q_OS_WIN
  G__setenv( "GISDBASE", shortPath( gisdbase ).toLocal8Bit().data() );
#else
  G__setenv( "GISDBASE", gisdbase.toUtf8().data() );
#endif
  G__setenv( "LOCATION_NAME", location.toLocal8Bit().data() );
  G__setenv( "MAPSET", mapset.toLocal8Bit().data() );
  defaultGisdbase = gisdbase;
  defaultLocation = location;
  defaultMapset = mapset;

  active = true;

  QgsGrass::instance()->loadMapsetSearchPath();
  QgsGrass::instance()->setMapsetSearchPathWatcher();

// closeMapset() added at the beginning
#if 0
#ifndef Q_OS_WIN
  // Close old mapset
  if ( mMapsetLock.length() > 0 )
  {
    QFile file( mMapsetLock );
    file.remove();
  }
#endif
#endif

  mMapsetLock = lock;

  emit QgsGrass::instance()->mapsetChanged();
  return QString::null;
}

QString QgsGrass::closeMapset()
{

  if ( mMapsetLock.length() > 0 )
  {
#ifndef Q_OS_WIN
    QFile file( mMapsetLock );
    if ( !file.remove() )
    {
      return QObject::tr( "Cannot remove mapset lock: %1" ).arg( mMapsetLock );
    }
#endif
    mMapsetLock = "";

    putenv(( char * ) "GISRC" );

    // Reinitialize GRASS
    G__setenv( "GISRC", ( char * ) "" );

    // Temporarily commented because of
    //   http://trac.osgeo.org/qgis/ticket/1900
    //   http://trac.osgeo.org/gdal/ticket/3313
    // it can be uncommented once GDAL with patch gets deployed (probably GDAL 1.8)
    //G__setenv( "GISDBASE", ( char * ) "" );
    //G__setenv( "LOCATION_NAME", ( char * ) "" );
    //G__setenv( "MAPSET", ( char * ) "" );

    defaultGisdbase = "";
    defaultLocation = "";
    defaultMapset = "";
    active = 0;

    // Delete temporary dir

    // To be sure that we don't delete '/' for example
    if ( mTmp.left( 4 ) == "/tmp" )
    {
      QDir dir( mTmp );
      for ( unsigned int i = 0; i < dir.count(); i++ )
      {
        if ( dir[i] == "." || dir[i] == ".." )
          continue;

        dir.remove( dir[i] );
        if ( dir.remove( dir[i] ) )
        {
          QgsDebugMsg( QString( "Cannot remove temporary file %1" ).arg( dir[i] ) );
        }
      }

      if ( !dir.rmdir( mTmp ) )
      {
        QgsDebugMsg( QString( "Cannot remove temporary directory %1" ).arg( mTmp ) );
      }
    }
  }

  QgsGrass::instance()->setMapsetSearchPathWatcher(); // unset watcher
  emit QgsGrass::instance()->mapsetChanged();
  return QString::null;
}

bool QgsGrass::closeMapsetWarn()
{

  QString err = QgsGrass::closeMapset();

  if ( !err.isNull() )
  {
    warning( tr( "Cannot close mapset. %1" ).arg( err ) );
    return false;
  }
  return true;
}

void QgsGrass::saveMapset()
{

  // Save working mapset in project file
  QgsProject::instance()->writeEntry( "GRASS", "/WorkingGisdbase",
                                      QgsProject::instance()->writePath( getDefaultGisdbase() ) );

  QgsProject::instance()->writeEntry( "GRASS", "/WorkingLocation",
                                      getDefaultLocation() );

  QgsProject::instance()->writeEntry( "GRASS", "/WorkingMapset",
                                      getDefaultMapset() );
}

void QgsGrass::createMapset( const QString& gisdbase, const QString& location,
                             const QString& mapset, QString& error )
{
  QString locationPath = gisdbase + "/" + location;
  QDir locationDir( locationPath );

  if ( !locationDir.mkdir( mapset ) )
  {
    error = tr( "Cannot create new mapset directory" );
    return;
  }

  QString src = locationPath + "/PERMANENT/DEFAULT_WIND";
  QString dest = locationPath + "/" + mapset + "/WIND";
  if ( !QFile::copy( src, dest ) )
  {
    error = tr( "Cannot copy %1 to %2" ).arg( src, dest );
  }
}

QStringList QgsGrass::locations( const QString& gisdbase )
{
  QgsDebugMsg( QString( "gisdbase = %1" ).arg( gisdbase ) );

  QStringList list;

  if ( gisdbase.isEmpty() )
    return list;

  QDir d = QDir( gisdbase );
  d.setFilter( QDir::NoDotAndDotDot | QDir::Dirs );

  for ( unsigned int i = 0; i < d.count(); i++ )
  {
    if ( QFile::exists( gisdbase + "/" + d[i]
                        + "/PERMANENT/DEFAULT_WIND" ) )
    {
      list.append( QString( d[i] ) );
    }
  }
  return list;
}

QStringList QgsGrass::mapsets( const QString& gisdbase, const QString& locationName )
{
  QgsDebugMsg( QString( "gisbase = %1 locationName = %2" ).arg( gisdbase, locationName ) );

  if ( gisdbase.isEmpty() || locationName.isEmpty() )
    return QStringList();

  return QgsGrass::mapsets( gisdbase + "/" + locationName );
}

QStringList QgsGrass::mapsets( const QString& locationPath )
{
  QgsDebugMsg( QString( "locationPath = %1" ).arg( locationPath ) );

  QStringList list;

  if ( locationPath.isEmpty() )
    return list;

  QDir d = QDir( locationPath );
  d.setFilter( QDir::NoDotAndDotDot | QDir::Dirs );

  for ( unsigned int i = 0; i < d.count(); i++ )
  {
    if ( QFile::exists( locationPath + "/" + d[i] + "/WIND" ) )
    {
      list.append( d[i] );
    }
  }
  return list;
}

QStringList QgsGrass::vectors( const QString& gisdbase, const QString& locationName,
                               const QString& mapsetName )
{

  if ( gisdbase.isEmpty() || locationName.isEmpty() || mapsetName.isEmpty() )
    return QStringList();

  /* TODO: G_list() was added to GRASS 6.1 06-05-24,
  enable its use after some period (others do update) */
  /*
  if ( QgsGrass::versionMajor() > 6 || QgsGrass::versionMinor() > 0 )
  {
  QStringList list;

  char **glist = G_list( G_ELEMENT_VECTOR,
  gisbase.toUtf8().constData(),
  locationName.toUtf8().constData(),
  mapsetName.toUtf8().constData() );

  int i = 0;

  while ( glist[i] )
  {
  list.append( QString(glist[i]) );
  i++;
  }

  G_free_list ( glist );

  return list;
  }
  */

  return QgsGrass::vectors( gisdbase + "/" + locationName + "/" + mapsetName );
}

QStringList QgsGrass::vectors( const QString& mapsetPath )
{
  QgsDebugMsg( QString( "mapsetPath = %1" ).arg( mapsetPath ) );

  QStringList list;

  if ( mapsetPath.isEmpty() )
    return list;

  QDir d = QDir( mapsetPath + "/vector" );
  d.setFilter( QDir::NoDotAndDotDot | QDir::Dirs );

  list.reserve( d.count() );
  for ( unsigned int i = 0; i < d.count(); ++i )
  {
#if 0
    if ( QFile::exists( mapsetPath + "/vector/" + d[i] + "/head" ) )
    {
      list.append( d[i] );
    }
#endif
    list.append( d[i] );
  }
  return list;
}

bool QgsGrass::topoVersion( const QString& gisdbase, const QString& location,
                            const QString& mapset, const QString& mapName, int &major, int &minor )
{
  QString path = gisdbase + "/" + location + "/" + mapset + "/vector/" + mapName + "/topo";
  QFile file( path );
  if ( !file.exists( path ) || file.size() < 5 )
  {
    return false;
  }
  if ( !file.open( QIODevice::ReadOnly ) )
  {
    return false;
  }
  QDataStream stream( &file );
  quint8 maj, min;
  stream >> maj;
  stream >> min;
  file.close();
  major = maj;
  minor = min;
  return true;
}

QStringList QgsGrass::vectorLayers( const QString& gisdbase, const QString& location,
                                    const QString& mapset, const QString& mapName )
{
  QgsDebugMsg( QString( "gisdbase = %1 location = %2 mapset = %3 mapName = %4" ).arg( gisdbase, location, mapset, mapName ) );

  QStringList list;
  QgsGrassVector vector( gisdbase, location, mapset, mapName );
  if ( !vector.openHead() )
  {
    throw QgsGrass::Exception( vector.error() );
  }

  QgsDebugMsg( "GRASS vector successfully opened" );

  // Get layers
  Q_FOREACH ( QgsGrassVectorLayer * layer, vector.layers() )
  {
    QString fs = QString::number( layer->number() );
    QgsDebugMsg( "layer number = " + fs );

    /* Points */
    int npoints = layer->typeCount( GV_POINT );
    QgsDebugMsg( QString( "npoints = %1" ).arg( npoints ) );
    if ( npoints > 0 )
    {
      QString l = fs + "_point";
      list.append( l );
    }

    /* Lines */
    /* Lines without category appears in layer 0, but not boundaries */
    int nlines = layer->typeCount( GV_LINE );
    if ( layer->number() > 0 )
    {
      nlines += layer->typeCount( GV_BOUNDARY );
    }
    QgsDebugMsg( QString( "nlines = %1" ).arg( nlines ) );
    if ( nlines > 0 )
    {
      QString l = fs + "_line";
      list.append( l );
    }

    /* Faces */
    int nfaces = layer->typeCount( GV_FACE );
    QgsDebugMsg( QString( "nfaces = %1" ).arg( nfaces ) );
    if ( nfaces > 0 )
    {
      QString l = fs + "_face";
      list.append( l );
    }

    /* Polygons */
    int nareas = layer->typeCount( GV_AREA );
    QgsDebugMsg( QString( "nareas = %1" ).arg( nareas ) );
    if ( nareas > 0 )
    {
      QString l = fs + "_polygon";
      list.append( l );
    }
  }
  QgsDebugMsg( "standard layers listed: " + list.join( "," ) );

  // TODO: add option in GUI to set listTopoLayers
  QSettings settings;
  bool listTopoLayers =  settings.value( "/GRASS/showTopoLayers", false ).toBool();
  if ( listTopoLayers )
  {
    // add topology layers
    if ( vector.typeCount( GV_POINTS ) > 0 )
    {
#if GRASS_VERSION_MAJOR < 7 /* no more point in GRASS 7 topo */
      list.append( "topo_point" );
#endif
    }
    if ( vector.typeCount( GV_LINES ) > 0 )
    {
      list.append( "topo_line" );
    }
    if ( vector.nodeCount() > 0 )
    {
      list.append( "topo_node" );
    }
  }

  return list;
}

QStringList QgsGrass::rasters( const QString& gisdbase, const QString& locationName,
                               const QString& mapsetName )
{

  if ( gisdbase.isEmpty() || locationName.isEmpty() || mapsetName.isEmpty() )
    return QStringList();


  /* TODO: G_list() was added to GRASS 6.1 06-05-24,
  enable its use after some period (others do update) */
  /*
  if ( QgsGrass::versionMajor() > 6 || QgsGrass::versionMinor() > 0 )
  {
  QStringList list;

  char **glist = G_list( G_ELEMENT_RASTER,
  gisbase.toUtf8().constData(),
  locationName.toUtf8().constData(),
  mapsetName.toUtf8().constData() );

  int i = 0;

  while ( glist[i] )
  {
  list.append( QString(glist[i]) );
  i++;
  }

  G_free_list ( glist );

  return list;
  }
  */

  return QgsGrass::rasters( gisdbase + "/" + locationName + "/" + mapsetName );
}

QStringList QgsGrass::rasters( const QString& mapsetPath )
{
  QgsDebugMsg( QString( "mapsetPath = %1" ).arg( mapsetPath ) );

  QStringList list;

  if ( mapsetPath.isEmpty() )
    return list;

  QDir d = QDir( mapsetPath + "/cellhd" );
  d.setFilter( QDir::Files );

  list.reserve( d.count() );
  for ( unsigned int i = 0; i < d.count(); ++i )
  {
    list.append( d[i] );
  }
  return list;
}

QStringList QgsGrass::groups( const QString& gisdbase, const QString& locationName,
                              const QString& mapsetName )
{
  return elements( gisdbase, locationName, mapsetName, "group" );
}

QStringList QgsGrass::groups( const QString& mapsetPath )
{
  return elements( mapsetPath, "group" );
}

QStringList QgsGrass::elements( const QString& gisdbase, const QString& locationName,
                                const QString& mapsetName, const QString& element )
{
  if ( gisdbase.isEmpty() || locationName.isEmpty() || mapsetName.isEmpty() )
  {
    return QStringList();
  }

  return QgsGrass::elements( gisdbase + "/" + locationName + "/" + mapsetName, element );
}

QStringList QgsGrass::elements( const QString&  mapsetPath, const QString&  element )
{
  QgsDebugMsg( QString( "mapsetPath = %1 element = %2" ).arg( mapsetPath, element ) );

  QStringList list;

  if ( mapsetPath.isEmpty() )
    return list;

  QDir d = QDir( mapsetPath + "/" + element );
  if ( element == "vector" || element == "group" )
  {
    d.setFilter( QDir::Dirs | QDir::NoDotAndDotDot );
  }
  else
  {
    d.setFilter( QDir::Files );
  }

  list.reserve( d.count() );
  for ( unsigned int i = 0; i < d.count(); ++i )
  {
    list.append( d[i] );
  }
  return list;
}

QStringList QgsGrass::grassObjects( const QgsGrassObject& mapsetObject, QgsGrassObject::Type type )
{
  QgsDebugMsg( "mapsetPath = " + mapsetObject.mapsetPath() + " type = " +  QgsGrassObject::elementShort( type ) );
  QTime time;
  time.start();
  QStringList list;
  if ( !QDir( mapsetObject.mapsetPath() ).isReadable() )
  {
    QgsDebugMsg( "mapset is not readable" );
    return QStringList();
  }
  else if ( type == QgsGrassObject::Strds || type == QgsGrassObject::Stvds
            || type == QgsGrassObject::Str3ds || type == QgsGrassObject::Stds )
  {
#if GRASS_VERSION_MAJOR >= 7
    QString cmd = "t.list";

    QStringList arguments;

    // Running t.list module is quite slow (about 500ms) -> check first if temporal db exists.
    // Also, if tgis/sqlite.db does not exist, it is created by t.list for current mapset!
    // If user is not owner of the mapset (even if has read permission) t.list fails because it checks ownership.
    if ( !QFile( mapsetObject.mapsetPath() + "/tgis/sqlite.db" ).exists() )
    {
      QgsDebugMsg( "tgis/sqlite.db does not exist" );
    }
    else
    {
      if ( type == QgsGrassObject::Stds )
      {
        arguments << "type=strds,stvds,str3ds";
      }
      else
      {
        arguments << "type=" + QgsGrassObject::elementShort( type );
      }

      int timeout = -1; // What timeout to use? It can take long time on network or database
      try
      {
        QByteArray data = runModule( mapsetObject.gisdbase(), mapsetObject.location(), mapsetObject.mapset(), cmd, arguments, timeout, false );
        Q_FOREACH ( QString fullName, QString::fromLocal8Bit( data ).split( '\n' ) )
        {
          fullName = fullName.trimmed();
          if ( !fullName.isEmpty() )
          {
            QStringList nameMapset = fullName.split( "@" );
            if ( nameMapset.value( 1 ) == mapsetObject.mapset() || nameMapset.value( 1 ).isEmpty() )
            {
              list << nameMapset.value( 0 );
            }
          }
        }
      }
      catch ( QgsGrass::Exception &e )
      {
        // TODO: notify somehow user
        QgsDebugMsg( QString( "Cannot run %1: %2" ).arg( cmd ).arg( e.what() ) );
      }
    }
#endif
  }
  else
  {
    list = QgsGrass::elements( mapsetObject.mapsetPath(), QgsGrassObject::dirName( type ) );
  }
  QgsDebugMsg( "list = " + list.join( "," ) );
  QgsDebugMsg( QString( "time (ms) = %1" ).arg( time.elapsed() ) );
  return list;
}

bool QgsGrass::objectExists( const QgsGrassObject& grassObject )
{
  if ( grassObject.name().isEmpty() )
  {
    return false;
  }
  QString path = grassObject.mapsetPath() + "/" + QgsGrassObject::dirName( grassObject.type() )
                 + "/" + grassObject.name();
  QFileInfo fi( path );
  return fi.exists();
}

QString QgsGrass::regionString( const struct Cell_head *window )
{
  QString reg;
  int fmt;
  char buf[1024];

  fmt = window->proj;

  // TODO 3D

  reg = "proj:" + QString::number( window->proj ) + ";";
  reg += "zone:" + QString::number( window->zone ) + ";";

  G_format_northing( window->north, buf, fmt );
  reg += "north:" + QString( buf ) + ";";

  G_format_northing( window->south, buf, fmt );
  reg += "south:" + QString( buf ) + ";";

  G_format_easting( window->east, buf, fmt );
  reg += "east:" + QString( buf ) + ";";

  G_format_easting( window->west, buf, fmt );
  reg += "west:" + QString( buf ) + ";";

  reg += "cols:" + QString::number( window->cols ) + ";";
  reg += "rows:" + QString::number( window->rows ) + ";";

  G_format_resolution( window->ew_res, buf, fmt );
  reg += "e-w resol:" + QString( buf ) + ";";

  G_format_resolution( window->ns_res, buf, fmt );
  reg += "n-s resol:" + QString( buf ) + ";";

  return reg;
}


bool QgsGrass::defaultRegion( const QString& gisdbase, const QString& location,
                              struct Cell_head *window )
{
  initRegion( window );
  QgsGrass::setLocation( gisdbase, location );
  try
  {
    G_get_default_window( window );
    return true;
  }
  catch ( QgsGrass::Exception &e )
  {
    Q_UNUSED( e );
    return false;
  }
}

void QgsGrass::region( const QString& gisdbase,
                       const QString& location, const QString& mapset,
                       struct Cell_head *window )
{
  QgsGrass::setLocation( gisdbase, location );

#if GRASS_VERSION_MAJOR < 7
  if ( G__get_window( window, ( char * ) "", ( char * ) "WIND", mapset.toUtf8().data() ) )
  {
    throw QgsGrass::Exception( QObject::tr( "Cannot get current region" ) );
  }
#else
  // In GRASS 7 G__get_window does not return error code and calls G_fatal_error on error
  G_FATAL_THROW( G__get_window( window, ( char * ) "", ( char * ) "WIND", mapset.toUtf8().data() ) );
#endif
}

void QgsGrass::region( struct Cell_head *window )
{
  region( getDefaultGisdbase(), getDefaultLocation(), getDefaultMapset(), window );
}

bool QgsGrass::writeRegion( const QString& gisbase,
                            const QString& location, const QString& mapset,
                            const struct Cell_head *window )
{
  QgsDebugMsg( QString( "n = %1 s = %2" ).arg( window->north ).arg( window->south ) );
  QgsDebugMsg( QString( "e = %1 w = %2" ).arg( window->east ).arg( window->west ) );

  if ( !window )
  {
    return false;
  }

  QgsGrass::setMapset( gisbase, location, mapset );

  if ( G_put_window( window ) == -1 )
  {
    return false;
  }

  return true;
}

void QgsGrass::writeRegion( const struct Cell_head *window )
{
  QString error = tr( "Cannot write region" );
  if ( !activeMode() )
  {
    throw QgsGrass::Exception( error += ", " + tr( "no mapset open" ) );
  }
  if ( !writeRegion( getDefaultGisdbase(), getDefaultLocation(), getDefaultMapset(), window ) )
  {
    throw QgsGrass::Exception( error );
  }
  emit regionChanged();
}

void QgsGrass::copyRegionExtent( struct Cell_head *source,
                                 struct Cell_head *target )
{
  target->north = source->north;
  target->south = source->south;
  target->east = source->east;
  target->west = source->west;
  target->top = source->top;
  target->bottom = source->bottom;
}

void QgsGrass::copyRegionResolution( struct Cell_head *source,
                                     struct Cell_head *target )
{
  target->ns_res = source->ns_res;
  target->ew_res = source->ew_res;
  target->tb_res = source->tb_res;
  target->ns_res3 = source->ns_res3;
  target->ew_res3 = source->ew_res3;
}

void QgsGrass::extendRegion( struct Cell_head *source,
                             struct Cell_head *target )
{
  if ( source->north > target->north )
    target->north = source->north;

  if ( source->south < target->south )
    target->south = source->south;

  if ( source->east > target->east )
    target->east = source->east;

  if ( source->west < target->west )
    target->west = source->west;

  if ( source->top > target->top )
    target->top = source->top;

  if ( source->bottom < target->bottom )
    target->bottom = source->bottom;
}

void QgsGrass::initRegion( struct Cell_head *window )
{
  window->format = 0;
  window->rows = 0;
  window->rows3 = 0;
  window->cols = 0;
  window->cols3 = 0;
  window->depths = 1;
  window->proj = -1;
  window->zone = -1;
  window->compressed = -1;
  window->ew_res = 0.0;
  window->ew_res3 = 1.0;
  window->ns_res = 0.0;
  window->ns_res3 = 1.0;
  window->tb_res = 1.0;
  window->top = 1.0;
  window->bottom = 0.0;
  window->west = 0;
  window->south = 0;
  window->east = 1;
  window->north = 1;
  window->rows = 1;
  window->cols = 1;
}

void QgsGrass::setRegion( struct Cell_head *window, QgsRectangle rect )
{
  window->west = rect.xMinimum();
  window->south = rect.yMinimum();
  window->east = rect.xMaximum();
  window->north = rect.yMaximum();
}

QString QgsGrass::setRegion( struct Cell_head *window, QgsRectangle rect, int rows, int cols )
{
  initRegion( window );
  window->west = rect.xMinimum();
  window->south = rect.yMinimum();
  window->east = rect.xMaximum();
  window->north = rect.yMaximum();
  window->rows = rows;
  window->cols = cols;

  QString error;
#if GRASS_VERSION_MAJOR < 7
  char* err = G_adjust_Cell_head( window, 1, 1 );
  if ( err )
  {
    error = QString( err );
  }
#else
  try
  {
    G_adjust_Cell_head( window, 1, 1 );
  }
  catch ( QgsGrass::Exception &e )
  {
    error = e.what();
  }
#endif
  return error;
}

QgsRectangle QgsGrass::extent( struct Cell_head *window )
{
  if ( !window )
  {
    return QgsRectangle();
  }
  return QgsRectangle( window->west, window->south, window->east, window->north );
}

bool QgsGrass::mapRegion( QgsGrassObject::Type type, QString gisdbase,
                          QString location, QString mapset, QString map,
                          struct Cell_head *window )
{
  QgsDebugMsg( QString( "map = %1" ).arg( map ) );
  QgsDebugMsg( QString( "mapset = %1" ).arg( mapset ) );

  QgsGrass::setLocation( gisdbase, location );

  if ( type == QgsGrassObject::Raster )
  {

    QString error = tr( "Cannot read raster map region (%1/%2/%3)" ).arg( gisdbase, location, mapset );
#if GRASS_VERSION_MAJOR < 7
    if ( G_get_cellhd( map.toUtf8().data(),
                       mapset.toUtf8().data(), window ) < 0 )
    {
      warning( error );
      return false;
    }
#else
    G_TRY
    {
      Rast_get_cellhd( map.toUtf8().data(), mapset.toUtf8().data(), window );
    }
    G_CATCH( QgsGrass::Exception &e )
    {
      warning( error + " : " + e.what() );
      return false;
    }
#endif
  }
  else if ( type == QgsGrassObject::Vector )
  {
    // Get current projection
    try
    {
      QgsGrass::region( gisdbase, location, mapset, window );
    }
    catch ( QgsGrass::Exception &e )
    {
      warning( e );
      return false;
    }

    struct Map_info *Map = 0;
    int level = -1;
    G_TRY
    {
      Map = vectNewMapStruct();
      Vect_set_open_level( 2 );
      level = Vect_open_old_head( Map, map.toUtf8().data(), mapset.toUtf8().data() );
    }
    G_CATCH( QgsGrass::Exception &e )
    {
      warning( e );
      vectDestroyMapStruct( Map );
      return false;
    }

    if ( level < 2 )
    {
      warning( QObject::tr( "Cannot read vector map region" ) );
      if ( level == 1 )
      {
        G_TRY
        {
          Vect_close( Map );
        }
        G_CATCH( QgsGrass::Exception &e )
        {
          QgsDebugMsg( e.what() );
        }
      }
      vectDestroyMapStruct( Map );
      return false;
    }

    BOUND_BOX box;
    Vect_get_map_box( Map, &box );
    window->north = box.N;
    window->south = box.S;
    window->west  = box.W;
    window->east  = box.E;
    window->top  = box.T;
    window->bottom  = box.B;

    // Is this optimal ?
    window->ns_res = ( window->north - window->south ) / 1000;
    window->ew_res = window->ns_res;
    if ( window->top > window->bottom )
    {
      window->tb_res = ( window->top - window->bottom ) / 10;
    }
    else
    {
      window->top = window->bottom + 1;
      window->tb_res = 1;
    }
    G_adjust_Cell_head3( window, 0, 0, 0 );

    Vect_close( Map );
    vectDestroyMapStruct( Map );
  }
  else if ( type == QgsGrassObject::Region )
  {
#if GRASS_VERSION_MAJOR < 7
    if ( G__get_window( window, ( char * ) "windows",
                        map.toUtf8().data(),
                        mapset.toUtf8().data() ) )
    {
      warning( tr( "Cannot read region" ) );
      return false;
    }
#else
    // G__get_window does not return error code in GRASS 7 and calls G_fatal_error on error
    G_TRY
    {
      G__get_window( window, ( char * ) "windows", map.toUtf8().data(), mapset.toUtf8().data() );
    }
    G_CATCH( QgsGrass::Exception &e )
    {
      warning( e );
      return false;
    }
#endif
  }
  return true;
}

QString QgsGrass::findModule( QString module )
{
  QgsDebugMsg( "called." );
  if ( QFile::exists( module ) )
  {
    return module;  // full path
  }

  QStringList extensions;
#ifdef Q_OS_WIN
  // On windows try .bat first
  extensions << ".bat" << ".py" << ".exe";
#endif
  // and then try if it's a module without extension (standard on UNIX)
  extensions << "";

  QStringList paths;
  // Try first full path
  paths << "";
  paths << QgsGrass::grassModulesPaths();

  // Extensions first to prefer .bat over .exe on Windows
  Q_FOREACH ( const QString& ext, extensions )
  {
    Q_FOREACH ( const QString& path, paths )
    {
      QString full = module + ext;;
      if ( !path.isEmpty() )
      {
        full.prepend( path + "/" );
      }
      if ( QFile::exists( full ) )
      {
        QgsDebugMsg( "found " + full );
        return full;
      }
      else
      {
        QgsDebugMsg( "not found " + full );
      }
    }
  }
  return QString();
}

QProcess *QgsGrass::startModule( const QString& gisdbase, const QString&  location,
                                 const QString&  mapset, const QString& moduleName, const QStringList& arguments,
                                 QTemporaryFile &gisrcFile, bool qgisModule )
{
  QgsDebugMsg( QString( "gisdbase = %1 location = %2" ).arg( gisdbase, location ) );
  QProcess *process = new QProcess();

  QString module = moduleName;
  if ( qgisModule )
  {
    module += QString::number( QgsGrass::versionMajor() );
  }

  QString modulePath = findModule( module );
  if ( modulePath.isEmpty() )
  {
    throw QgsGrass::Exception( QObject::tr( "Cannot find module %1" ).arg( module ) );
  }

  // We have to set GISRC file, uff
  if ( !gisrcFile.open() )
  {
    throw QgsGrass::Exception( QObject::tr( "Cannot open GISRC file" ) );
  }

  QString error = tr( "Cannot start module" ) + "\n" + tr( "command: %1 %2" ).arg( module, arguments.join( " " ) );

  QTextStream out( &gisrcFile );
  out << "GISDBASE: " << gisdbase << "\n";
  out << "LOCATION_NAME: " << location << "\n";
  if ( mapset.isEmpty() )
  {
    out << "MAPSET: PERMANENT\n";
  }
  else
  {
    out << "MAPSET: " << mapset << "\n";
  }
  out.flush();
  QgsDebugMsg( gisrcFile.fileName() );
  gisrcFile.close();
  QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
  QStringList paths = QgsGrass::grassModulesPaths();
  // PYTHONPATH necessary for t.list.py
  // PATH necessary for g.parser called by t.list.py
  paths += environment.value( "PATH" ).split( QgsGrass::pathSeparator() );
  environment.insert( "PATH", paths.join( QgsGrass::pathSeparator() ) );
  environment.insert( "PYTHONPATH", QgsGrass::getPythonPath() );
  environment.insert( "GISRC", gisrcFile.fileName() );
  environment.insert( "GRASS_MESSAGE_FORMAT", "gui" );
  // Normaly modules must be run in a mapset owned by user, because each module calls G_gisinit()
  // which checks if G_mapset() is owned by user. The check is disabled by GRASS_SKIP_MAPSET_OWNER_CHECK.
  environment.insert( "GRASS_SKIP_MAPSET_OWNER_CHECK", "1" );

  process->setProcessEnvironment( environment );

  QgsDebugMsg( modulePath + " " + arguments.join( " " ) );
  process->start( modulePath, arguments );
  if ( !process->waitForStarted() )
  {
    throw QgsGrass::Exception( error );
  }
  return process;
}

QByteArray QgsGrass::runModule( const QString& gisdbase, const QString&  location,
                                const QString& mapset, const QString&  moduleName,
                                const QStringList& arguments, int timeOut, bool qgisModule )
{
  QgsDebugMsg( QString( "gisdbase = %1 location = %2 timeOut = %3" ).arg( gisdbase, location ).arg( timeOut ) );
  QTime time;
  time.start();

  QTemporaryFile gisrcFile;
  QProcess *process = startModule( gisdbase, location, mapset, moduleName, arguments, gisrcFile, qgisModule );

  if ( !process->waitForFinished( timeOut )
       || ( process->exitCode() != 0 && process->exitCode() != 255 ) )
  {
    QgsDebugMsg( "process->exitCode() = " + QString::number( process->exitCode() ) );

    throw QgsGrass::Exception( QObject::tr( "Cannot run module" ) + "\n"
                               + QObject::tr( "command: %1 %2\nstdout: %3\nstderr: %4" )
                               .arg( moduleName, arguments.join( " " ),
                                     process->readAllStandardOutput().constData(),
                                     process->readAllStandardError().constData() ) );
  }
  QByteArray data = process->readAllStandardOutput();
  QgsDebugMsg( QString( "time (ms) = %1" ).arg( time.elapsed() ) );
  delete process;
  return data;
}

QString QgsGrass::getInfo( const QString&  info, const QString&  gisdbase,
                           const QString&  location, const QString&  mapset,
                           const QString&  map, const QgsGrassObject::Type type,
                           double x, double y,
                           const QgsRectangle& extent, int sampleRows,
                           int sampleCols, int timeOut )
{
  QgsDebugMsg( QString( "gisdbase = %1 location = %2" ).arg( gisdbase, location ) );

  QStringList arguments;

  QString cmd = qgisGrassModulePath() + "/qgis.g.info";

  arguments.append( "info=" + info );
  if ( !map.isEmpty() )
  {
    QString opt;
    switch ( type )
    {
      case QgsGrassObject::Raster:
        opt = "rast";
        break;
      case QgsGrassObject::Vector:
        opt = "vect";
        break;
      default:
        QgsDebugMsg( QString( "unexpected type:%1" ).arg( type ) );
        return "";
    }
    arguments.append( opt + "=" +  map + "@" + mapset );
  }
  if ( info == "query" )
  {
    arguments.append( QString( "coor=%1,%2" ).arg( x ).arg( y ) );
  }
  if ( info == "stats" )
  {
    arguments.append( QString( "north=%1" ).arg( extent.yMaximum() ) );
    arguments.append( QString( "south=%1" ).arg( extent.yMinimum() ) );
    arguments.append( QString( "east=%1" ).arg( extent.xMaximum() ) );
    arguments.append( QString( "west=%1" ).arg( extent.xMinimum() ) );
    arguments.append( QString( "rows=%1" ).arg( sampleRows ) );
    arguments.append( QString( "cols=%1" ).arg( sampleCols ) );
  }

  //QByteArray data =  runModule( gisdbase, location, mapset, cmd, arguments, timeOut );
  // Run module with empty mapset so that it tries to find a mapset owned by user
  QByteArray data = runModule( gisdbase, location, "", cmd, arguments, timeOut );
  QgsDebugMsg( data );
  return QString( data );
}

QgsCoordinateReferenceSystem QgsGrass::crs( const QString& gisdbase, const QString& location,
    QString &error )
{
  QgsDebugMsg( QString( "gisdbase = %1 location = %2" ).arg( gisdbase, location ) );
  QgsCoordinateReferenceSystem crs = QgsCoordinateReferenceSystem();
  try
  {
    QString wkt = getInfo( "proj", gisdbase, location );
    QgsDebugMsg( "wkt: " + wkt );
    crs = QgsCRSCache::instance()->crsByWkt( wkt );
    QgsDebugMsg( "crs.toWkt: " + crs.toWkt() );
  }
  catch ( QgsGrass::Exception &e )
  {
    error = tr( "Cannot get projection" ) + "\n" + e.what();
    QgsDebugMsg( error );
  }

  return crs;
}

QgsCoordinateReferenceSystem QgsGrass::crsDirect( const QString& gisdbase, const QString& location )
{
  QString Wkt;

  struct Cell_head cellhd;

  QgsGrass::resetError();
  QgsGrass::setLocation( gisdbase, location );

  {
    QgsLocaleNumC l;

    G_TRY
    {
      G_get_default_window( &cellhd );
    }
    G_CATCH( QgsGrass::Exception &e )
    {
      Q_UNUSED( e );
      QgsDebugMsg( QString( "Cannot get default window: %1" ).arg( e.what() ) );
      return QgsCoordinateReferenceSystem();
    }

    if ( cellhd.proj != PROJECTION_XY )
    {
      struct Key_Value *projinfo = G_get_projinfo();
      struct Key_Value *projunits = G_get_projunits();
      char *wkt = GPJ_grass_to_wkt( projinfo, projunits, 0, 0 );
      Wkt = QString( wkt );
      G_free( wkt );
    }
  }

  QgsCoordinateReferenceSystem srs = QgsCRSCache::instance()->crsByWkt( Wkt );

  return srs;
}

QgsRectangle QgsGrass::extent( const QString& gisdbase, const QString& location,
                               const QString& mapset, const QString& map,
                               QgsGrassObject::Type type, QString &error )
{
  QgsDebugMsg( QString( "gisdbase = %1 location = %2" ).arg( gisdbase, location ) );

  try
  {
    QString str = getInfo( "window", gisdbase, location, mapset, map, type );
    QStringList list = str.split( "," );
    if ( list.size() != 4 )
    {
      throw QgsGrass::Exception( "Cannot parse GRASS map extent: " + str );
    }
    return QgsRectangle( list[0].toDouble(), list[1].toDouble(), list[2].toDouble(), list[3].toDouble() );
  }
  catch ( QgsGrass::Exception &e )
  {
    error = tr( "Cannot get raster extent" ) + " : " + e.what();
  }
  return QgsRectangle( 0, 0, 0, 0 );
}

void QgsGrass::size( const QString& gisdbase, const QString& location, const QString& mapset,
                     const QString& map, int *cols, int *rows, QString &error )
{
  QgsDebugMsg( QString( "gisdbase = %1 location = %2" ).arg( gisdbase, location ) );

  *cols = 0;
  *rows = 0;
  try
  {
    QString str = getInfo( "size", gisdbase, location, mapset, map, QgsGrassObject::Raster );
    QStringList list = str.split( "," );
    if ( list.size() != 2 )
    {
      throw QgsGrass::Exception( "Cannot parse GRASS map size: " + str );
    }
    *cols = list[0].toInt();
    *rows = list[1].toInt();
  }
  catch ( QgsGrass::Exception &e )
  {
    error = tr( "Cannot get raster extent" ) + " : " + e.what();
    QgsDebugMsg( error );
  }

  QgsDebugMsg( QString( "raster size = %1 %2" ).arg( *cols ).arg( *rows ) );
}

QHash<QString, QString> QgsGrass::info( const QString& gisdbase, const QString& location,
                                        const QString& mapset, const QString& map,
                                        QgsGrassObject::Type type,
                                        const QString& info,
                                        const QgsRectangle& extent,
                                        int sampleRows, int sampleCols,
                                        int timeOut, QString &error )
{
  QgsDebugMsg( QString( "gisdbase = %1 location = %2" ).arg( gisdbase, location ) );
  QHash<QString, QString> inf;

  try
  {
    QString str = getInfo( info, gisdbase, location, mapset, map, type, 0, 0, extent, sampleRows, sampleCols, timeOut );
    QgsDebugMsg( str );
    QStringList list = str.split( "\n" );
    for ( int i = 0; i < list.size(); i++ )
    {
      QStringList keyVal = list[i].split( ':' );
      if ( list[i].isEmpty() )
        continue;
      if ( keyVal.size() != 2 )
      {
        throw QgsGrass::Exception( "Cannot parse GRASS map info key value : " + list[i] + " (" + str + " ) " );
      }
      inf[keyVal[0]] = keyVal[1];
    }
  }
  catch ( QgsGrass::Exception &e )
  {
    error = tr( "Cannot get map info" ) + "\n" + e.what();
    QgsDebugMsg( error );
  }
  return inf;
}

QList<QgsGrass::Color> QgsGrass::colors( QString gisdbase, QString location, QString mapset,
    QString map, QString& error )
{
  QgsDebugMsg( QString( "gisdbase = %1 location = %2" ).arg( gisdbase, location ) );
  QList<QgsGrass::Color> ct;

  try
  {
    QString str = getInfo( "colors", gisdbase, location, mapset, map, QgsGrassObject::Raster );
    QgsDebugMsg( str );
    QStringList list = str.split( "\n" );
    for ( int i = 0; i < list.size(); i++ )
    {
      QgsGrass::Color c;
      if ( list[i].isEmpty() )
        continue;
      if ( sscanf( list[i].toUtf8().data(), "%lf %lf %d %d %d %d %d %d", &( c.value1 ), &( c.value2 ), &( c.red1 ), &( c.green1 ), &( c.blue1 ), &( c.red2 ), &( c.green2 ), &( c.blue2 ) ) != 8 )
      {
        throw QgsGrass::Exception( "Cannot parse GRASS colors" + list[i] + " (" + str + " ) " );
      }
      ct.append( c );
    }
  }
  catch ( QgsGrass::Exception &e )
  {
    error = tr( "Cannot get colors" ) + " : " + e.what();
    QgsDebugMsg( error );
  }
  return ct;
}

QMap<QString, QString> QgsGrass::query( QString gisdbase, QString location, QString mapset, QString map, QgsGrassObject::Type type, double x, double y )
{
  QgsDebugMsg( QString( "gisdbase = %1 location = %2" ).arg( gisdbase, location ) );

  QMap<QString, QString> result;
  // TODO: multiple values (more rows)
  try
  {
    QString str = getInfo( "query", gisdbase, location, mapset, map, type, x, y );
    QStringList list = str.trimmed().split( ":" );
    if ( list.size() == 2 )
    {
      result[list[0]] = list[1];
    }
  }
  catch ( QgsGrass::Exception &e )
  {
    warning( tr( "Cannot query raster " ) + "\n" + e.what() );
  }
  return result;
}

void QgsGrass::renameObject( const QgsGrassObject & object, const QString& newName )
{
  QString cmd =  gisbase() + "/bin/g.rename";
  QStringList arguments;

  arguments << object.elementShort() + "=" + object.name() + "," + newName;

  int timeout = -1; // What timeout to use? It can take long time on network or database
  // throws QgsGrass::Exception
  runModule( object.gisdbase(), object.location(), object.mapset(), cmd, arguments, timeout, false );
}

void QgsGrass::copyObject( const QgsGrassObject & srcObject, const QgsGrassObject & destObject )
{
  QgsDebugMsg( "srcObject = " + srcObject.toString() );
  QgsDebugMsg( "destObject = " + destObject.toString() );

  if ( !srcObject.locationIdentical( destObject ) ) // should not happen
  {
    throw QgsGrass::Exception( QObject::tr( "Attempt to copy from different location." ) );
  }

  QString cmd = gisbase() + "/bin/g.copy";
  QStringList arguments;

  arguments << srcObject.elementShort() + "=" + srcObject.name() + "@" + srcObject.mapset() + "," + destObject.name();

  int timeout = -1; // What timeout to use? It can take long time on network or database
  // throws QgsGrass::Exception
  // TODO: g.copy does not seem to return error code if fails (6.4.3RC1)
  runModule( destObject.gisdbase(), destObject.location(), destObject.mapset(), cmd, arguments, timeout, false );
}

bool QgsGrass::deleteObject( const QgsGrassObject & object )
{

  // TODO: check if user has permissions

  /*
  if ( QMessageBox::question( this, tr( "Question" ),
                              tr( "Are you sure you want to delete %n selected layer(s)?", "number of layers to delete", indexes.size() ),
                              QMessageBox::Yes | QMessageBox::No ) == QMessageBox::No )
  {
    return false;
  }
  */

  QString cmd = gisbase() + "/bin/g.remove";
  QStringList arguments;

#if GRASS_VERSION_MAJOR < 7
  arguments << object.elementShort() + "=" + object.name();
#else
  arguments << "-f" << "type=" + object.elementShort() << "name=" + object.name();
#endif

  try
  {
    runModule( object.gisdbase(), object.location(), object.mapset(), cmd, arguments, 5000, false );
  }
  catch ( QgsGrass::Exception &e )
  {
    warning( tr( "Cannot delete" ) + " " + object.elementName() + " " + object.name() + ": " + e.what() );
    return false;
  }
  return true;
}

bool QgsGrass::deleteObjectDialog( const QgsGrassObject & object )
{

  return QMessageBox::question( 0, QObject::tr( "Delete confirmation" ),
                                QObject::tr( "Are you sure you want to delete %1 %2?" ).arg( object.elementName(), object.name() ),
                                QMessageBox::Yes | QMessageBox::No ) == QMessageBox::Yes;
}

void QgsGrass::createVectorMap( const QgsGrassObject & object, QString &error )
{

  QgsGrass::setMapset( object );

  struct Map_info *Map = 0;
  QgsGrass::lock();
  G_TRY
  {
    Map = vectNewMapStruct();
    Vect_open_new( Map, object.name().toUtf8().data(), 0 );

#if ( GRASS_VERSION_MAJOR == 6 && GRASS_VERSION_MINOR >= 4 ) || GRASS_VERSION_MAJOR > 6
    Vect_build( Map );
#else
    Vect_build( Map, stderr );
#endif
    Vect_set_release_support( Map );
    Vect_close( Map );
  }
  G_CATCH( QgsGrass::Exception &e )
  {
    error = tr( "Cannot create new vector: %1" ).arg( e.what() );
  }
  QgsGrass::vectDestroyMapStruct( Map );
  QgsGrass::unlock();
}

void QgsGrass::createTable( dbDriver *driver, const QString tableName, const QgsFields &fields )
{
  if ( !driver ) // should not happen
  {
    throw QgsGrass::Exception( "driver is null" );
  }

  QStringList fieldsStringList;
  Q_FOREACH ( const QgsField& field, fields )
  {
    QString name = field.name().toLower().replace( " ", "_" );
    if ( name.at( 0 ).isDigit() )
    {
      name = "_" + name;
    }
    QString typeName;
    switch ( field.type() )
    {
      case QVariant::Int:
      case QVariant::LongLong:
      case QVariant::Bool:
        typeName = "integer";
        break;
      case QVariant::Double:
        typeName = "double precision";
        break;
        // TODO: verify how is it with spatialite/dbf support for date, time, datetime, v.in.ogr is using all
      case QVariant::Date:
        typeName = "date";
        break;
      case QVariant::Time:
        typeName = "time";
        break;
      case QVariant::DateTime:
        typeName = "datetime";
        break;
      case QVariant::String:
        typeName = QString( "varchar (%1)" ).arg( field.length() );
        break;
      default:
        typeName = QString( "varchar (%1)" ).arg( field.length() > 0 ? field.length() : 255 );
    }
    fieldsStringList <<  name + " " + typeName;
  }
  QString sql = QString( "create table %1 (%2);" ).arg( tableName, fieldsStringList.join( ", " ) );

  dbString dbstr;
  db_init_string( &dbstr );
  db_set_string( &dbstr, sql.toLatin1().data() );

  int result = db_execute_immediate( driver, &dbstr );
  db_free_string( &dbstr );
  if ( result != DB_OK )
  {
    throw QgsGrass::Exception( QObject::tr( "Cannot create table" ) + ": " + QString::fromLatin1( db_get_error_msg() ) );
  }
}

void QgsGrass::insertRow( dbDriver *driver, const QString tableName,
                          const QgsAttributes& attributes )
{
  if ( !driver ) // should not happen
  {
    throw QgsGrass::Exception( "driver is null" );
  }

  QStringList valuesStringList;
  Q_FOREACH ( const QVariant& attribute, attributes )
  {
    QString valueString;

    bool quote = true;
    switch ( attribute.type() )
    {
      case QVariant::Int:
      case QVariant::Double:
      case QVariant::LongLong:
        valueString = attribute.toString();
        quote = false;
        break;
        // TODO: use rbool according to driver
      case QVariant::Bool:
        valueString = attribute.toBool() ? "1" : "0";
        quote = false;
        break;
      case QVariant::Date:
        valueString = attribute.toDate().toString( Qt::ISODate );
        break;
      case QVariant::Time:
        valueString = attribute.toTime().toString( Qt::ISODate );
        break;
      case QVariant::DateTime:
        valueString = attribute.toDateTime().toString( Qt::ISODate );
        break;
      default:
        valueString = attribute.toString();
    }
    valueString.replace( "'", "''" );

    if ( quote )
    {
      valueString = "'" + valueString + "'";
    }

    valuesStringList <<  valueString;
  }
  QString sql = QString( "insert into %1 values (%2);" ).arg( tableName, valuesStringList.join( ", " ) );

  dbString dbstr;
  db_init_string( &dbstr );
  db_set_string( &dbstr, sql.toLatin1().data() );

  int result = db_execute_immediate( driver, &dbstr );
  db_free_string( &dbstr );
  if ( result != DB_OK )
  {
    throw QgsGrass::Exception( QObject::tr( "Cannot insert, statement: '%1' error: '%2'" ).arg( sql, QString::fromLatin1( db_get_error_msg() ) ) );
  }
}

bool QgsGrass::isExternal( const QgsGrassObject & object )
{
  if ( object.type() != QgsGrassObject::Raster )
  {
    return false;
  }
  lock();
  bool isExternal = false;
  G_TRY
  {
    QgsGrass::setLocation( object.gisdbase(), object.location() );
    struct GDAL_link *gdal;
    gdal = G_get_gdal_link( object.name().toUtf8().data(), object.mapset().toUtf8().data() );
    if ( gdal )
    {
      isExternal = true;
      G_close_gdal_link( gdal );
    }
  }
  G_CATCH( QgsGrass::Exception &e )
  {
    QgsDebugMsg( "error getting external link: " + QString( e.what() ) );
  }
  unlock();
  return isExternal;
}

void QgsGrass::adjustCellHead( struct Cell_head *cellhd, int row_flag, int col_flag )
{
#if (GRASS_VERSION_MAJOR < 7)
  char* err = G_adjust_Cell_head( cellhd, row_flag, col_flag );
  if ( err )
  {
    throw QgsGrass::Exception( QObject::tr( "Cannot adjust region, error: '%1'" ).arg( err ) );
  }
#else
  G_FATAL_THROW( G_adjust_Cell_head( cellhd, row_flag, col_flag ) );
#endif
}

// Map of vector types

QMap<int, QString> QgsGrass::vectorTypeMap()
{
  static QMap<int, QString> vectorTypes;
  static QMutex sMutex;
  if ( vectorTypes.isEmpty() )
  {
    sMutex.lock();
    if ( vectorTypes.isEmpty() )
    {
      vectorTypes.insert( GV_POINT, "point" );
      vectorTypes.insert( GV_CENTROID, "centroid" );
      vectorTypes.insert( GV_LINE, "line" );
      vectorTypes.insert( GV_BOUNDARY, "boundary" );
      vectorTypes.insert( GV_AREA, "area" );
      vectorTypes.insert( GV_FACE, "face" );
      vectorTypes.insert( GV_KERNEL, "kernel" );
    }
    sMutex.unlock();
  }
  return vectorTypes;
}

int QgsGrass::vectorType( const QString & typeName )
{
  return QgsGrass::vectorTypeMap().key( typeName );
}

QString QgsGrass::vectorTypeName( int type )
{
  return QgsGrass::vectorTypeMap().value( type );
}

// GRASS version constants have been changed on 26.4.2007
// http://freegis.org/cgi-bin/viewcvs.cgi/grass6/include/version.h.in.diff?r1=1.4&r2=1.5
// The following lines workaround this change

int QgsGrass::versionMajor()
{
#ifdef GRASS_VERSION_MAJOR
  return GRASS_VERSION_MAJOR;
#else
  return QString( GRASS_VERSION_MAJOR ).toInt();
#endif
}

int QgsGrass::versionMinor()
{
#ifdef GRASS_VERSION_MINOR
  return GRASS_VERSION_MINOR;
#else
  return QString( GRASS_VERSION_MINOR ).toInt();
#endif
}

int QgsGrass::versionRelease()
{
#ifdef GRASS_VERSION_RELEASE
#define QUOTE(x)  #x
  return QString( QUOTE( GRASS_VERSION_RELEASE ) ).toInt();
#else
  return QString( GRASS_VERSION_RELEASE ).toInt();
#endif
}
QString QgsGrass::versionString()
{
  return QString( GRASS_VERSION_STRING );
}

Qt::CaseSensitivity QgsGrass::caseSensitivity()
{
#ifdef Q_OS_WIN
  return Qt::CaseInsensitive;
#else
  return Qt::CaseSensitive;
#endif
}

bool QgsGrass::isLocation( const QString& path )
{
  return G_is_location( path.toUtf8().constData() ) == 1;
}

bool QgsGrass::isMapset( const QString& path )
{
  return G_is_mapset( path.toUtf8().constData() ) == 1;
}

QString QgsGrass::lockFilePath()
{
  return mMapsetLock;
}

QString QgsGrass::gisrcFilePath()
{
  if ( mGisrc.isEmpty() )
  {
    // Started from GRASS shell
    if ( getenv( "GISRC" ) )
    {
      return QString( getenv( "GISRC" ) );
    }
  }
  return mGisrc;
}

void QgsGrass::putEnv( QString name, QString value )
{
  QString env = name + "=" + value;
  /* _Correct_ putenv() implementation is not making copy! */
  char *envChar = new char[env.toUtf8().length()+1];
  strcpy( envChar, env.toUtf8().constData() );
  putenv( envChar );
}

QString QgsGrass::getPythonPath()
{
  QString pythonpath = getenv( "PYTHONPATH" );
  pythonpath += pathSeparator() + gisbase() + "/etc/python";
  pythonpath += pathSeparator() + gisbase() + "/gui/wxpython";
  QgsDebugMsg( "pythonpath = " + pythonpath );
  return pythonpath;
}

QString QgsGrass::defaultGisbase()
{
  // Look first to see if GISBASE env var is already set.
  // This is set when QGIS is run from within GRASS
  // or when set explicitly by the user.
  // This value should always take precedence.
  QString gisbase;
#ifdef Q_OS_WIN
  gisbase = getenv( "WINGISBASE" ) ? getenv( "WINGISBASE" ) : getenv( "GISBASE" );
  gisbase = shortPath( gisbase );
#else
  gisbase = getenv( "GISBASE" );
#endif
  QgsDebugMsg( "gisbase from envar = " + gisbase );

  if ( !gisbase.isEmpty() )
  {
    return gisbase;
  }

#ifdef Q_OS_WIN
  // Use the applicationDirPath()/grass
#ifdef _MSC_VER
  gisbase = shortPath( QCoreApplication::applicationDirPath() + ( QgsApplication::isRunningFromBuildDir() ?  + "/../.." : "" ) + "/grass" );
#else
  gisbase = shortPath( QCoreApplication::applicationDirPath() + ( QgsApplication::isRunningFromBuildDir() ?  + "/.." : "" ) + "/grass" );
#endif
  // Use the location specified by WITH_GRASS during configure
#elif defined(Q_OS_MACX)
  // check for bundled GRASS, fall back to configured path
  gisbase = QCoreApplication::applicationDirPath().append( "/grass" );
  if ( !isValidGrassBaseDir( gisbase ) )
  {
    gisbase = GRASS_BASE;
  }
#else
  gisbase = GRASS_BASE;
#endif

  QgsDebugMsg( "gisbase = " + gisbase );
  return gisbase;
}


QString QgsGrass::gisbase()
{
  QSettings settings;
  bool customGisbase = settings.value( "/GRASS/gidbase/custom", false ).toBool();
  QString customGisdbaseDir = settings.value( "/GRASS/gidbase/customDir" ).toString();

  QString gisbase;
  if ( customGisbase && !customGisdbaseDir.isEmpty() )
  {
    gisbase = customGisdbaseDir;
  }
  else
  {
    gisbase = defaultGisbase();
  }
#ifdef Q_OS_WIN
  gisbase = shortPath( gisbase );
#endif
  return gisbase;
}

void QgsGrass::setGisbase( bool custom, const QString &customDir )
{
  QgsDebugMsg( QString( "custom = %1 customDir = %2" ).arg( custom ).arg( customDir ) );
  QSettings settings;

  bool previousCustom = settings.value( "/GRASS/gidbase/custom", false ).toBool();
  QString previousCustomDir = settings.value( "/GRASS/gidbase/customDir" ).toString();
  settings.setValue( "/GRASS/gidbase/custom", custom );
  settings.setValue( "/GRASS/gidbase/customDir", customDir );

  if ( custom != previousCustom || ( custom && customDir != previousCustomDir ) )
  {
    mNonInitializable = false;
    initialized = false;
    mInitError.clear();
    if ( !QgsGrass::init() )
    {
      QgsDebugMsg( "cannot init : " + QgsGrass::initError() );
    }
    emit gisbaseChanged();
  }
}


QString QgsGrass::modulesConfigDefaultDirPath()
{
  if ( QgsApplication::isRunningFromBuildDir() )
  {
    return QgsApplication::buildSourcePath() + "/src/plugins/grass/modules";
  }

  return QgsApplication::pkgDataPath() + "/grass/modules";
}

QString QgsGrass::modulesConfigDirPath()
{
  QSettings settings;
  bool customModules = settings.value( "/GRASS/modules/config/custom", false ).toBool();
  QString customModulesDir = settings.value( "/GRASS/modules/config/customDir" ).toString();

  if ( customModules && !customModulesDir.isEmpty() )
  {
    return customModulesDir;
  }
  else
  {
    return modulesConfigDefaultDirPath();
  }
}

void QgsGrass::setModulesConfig( bool custom, const QString &customDir )
{
  QSettings settings;

  bool previousCustom = settings.value( "/GRASS/modules/config/custom", false ).toBool();
  QString previousCustomDir = settings.value( "/GRASS/modules/config/customDir" ).toString();
  settings.setValue( "/GRASS/modules/config/custom", custom );
  settings.setValue( "/GRASS/modules/config/customDir", customDir );

  if ( custom != previousCustom || ( custom && customDir != previousCustomDir ) )
  {
    emit modulesConfigChanged();
  }
}

QPen QgsGrass::regionPen()
{
  QSettings settings;
  QPen pen;
  pen.setColor( QColor( settings.value( "/GRASS/region/color", "#ff0000" ).toString() ) );
  pen.setWidthF( settings.value( "/GRASS/region/width", 0 ).toFloat() );
  return pen;
}

void QgsGrass::setRegionPen( const QPen & pen )
{
  QSettings settings;
  settings.setValue( "/GRASS/region/color", pen.color().name() );
  settings.setValue( "/GRASS/region/width", pen.widthF() );
  emit regionPenChanged();
}

bool QgsGrass::modulesDebug()
{
  QSettings settings;
  return settings.value( "/GRASS/modules/debug", false ).toBool();
}

void QgsGrass::setModulesDebug( bool debug )
{
  QSettings settings;
  bool previous = modulesDebug();
  settings.setValue( "/GRASS/modules/debug", debug );
  if ( previous != debug )
  {
    emit modulesDebugChanged();
  }
}

void QgsGrass::openOptions()
{
  QgsGrassOptions dialog;
  dialog.exec();
}

void QgsGrass::warning( const QString &message )
{
  if ( !mMute )
  {
    QMessageBox::warning( 0, QObject::tr( "Warning" ), message );
  }
  else
  {
    error_message = message;
    QgsDebugMsg( message );
  }
}

void QgsGrass::warning( QgsGrass::Exception &e )
{
  warning( e.what() );
}

struct Map_info *QgsGrass::vectNewMapStruct()
{
  // In OSGeo4W there is GRASS compiled by MinGW while QGIS compiled by MSVC, the compilers
  // may have different sizes of types, see issue #13002. Because there is no Vect_new_map_struct (GRASS 7.0.0, July 2015)
  // the structure is allocated here using doubled (should be enough) space.

  // The same problem was also reported for QGIS 2.11-master compiled on Xubuntu 14.04 LTS
  // using GRASS 7.0.1-2~ubuntu14.04.1 from https://launchpad.net/~grass/+archive/ubuntu/grass-stable
  // -> allocate more space on all systems

  // TODO: replace by Vect_new_map_struct once it appears in GRASS
  // Patch supplied: https://trac.osgeo.org/grass/ticket/2729
#if GRASS_VERSION_MAJOR > 999
  G_TRY
  {
    return Vect_new_map_struct();
  }
  G_CATCH( QgsGrass::Exception &e ) // out of memory
  {
    warning( e );
    return 0;
  }
#else
  return ( struct Map_info* ) qgsMalloc( 2*sizeof( struct Map_info ) );
#endif
}

void QgsGrass::vectDestroyMapStruct( struct Map_info *map )
{
  // TODO: replace by Vect_destroy_map_struct once it appears in GRASS
  // TODO: until switch to hypothetical Vect_destroy_map_struct verify that Vect_destroy_map_struct cannot
  // call G_fatal_error, otherwise check and remove use of vectDestroyMapStruct from G_CATCH blocks
  QgsDebugMsg( QString( "free map = %1" ).arg(( long )map ) );
  qgsFree( map );
  map = 0;
}

void QgsGrass::sleep( int ms )
{
// Stolen from QTest::qSleep
#ifdef Q_OS_WIN
  Sleep( uint( ms ) );
#else
  struct timespec ts = { ms / 1000, ( ms % 1000 ) * 1000 * 1000 };
  nanosleep( &ts, nullptr );
#endif
}

QgsGrass::ModuleOutput QgsGrass::parseModuleOutput( const QString & input, QString &text, QString &html, int &value )
{
  QgsDebugMsg( "input = " + input );
#ifdef QGISDEBUG
  QString ascii;
  for ( int i = 0; i < input.size(); i++ )
  {
    int c = input.at( i ).toAscii();
    ascii += QString().sprintf( "%2x ", c );
  }
  QgsDebugMsg( "ascii = " + ascii );
#endif

  QRegExp rxpercent( "GRASS_INFO_PERCENT: (\\d+)" );
  QRegExp rxmessage( "GRASS_INFO_MESSAGE\\(\\d+,\\d+\\): (.*)" );
  QRegExp rxwarning( "GRASS_INFO_WARNING\\(\\d+,\\d+\\): (.*)" );
  QRegExp rxerror( "GRASS_INFO_ERROR\\(\\d+,\\d+\\): (.*)" );
  QRegExp rxend( "GRASS_INFO_END\\(\\d+,\\d+\\)" );
  // GRASS added G_progress() which does not support GRASS_MESSAGE_FORMAT=gui
  // and it is printing fprintf(stderr, "%10ld\b\b\b\b\b\b\b\b\b\b", n);
  // Ticket created https://trac.osgeo.org/grass/ticket/2751
  QRegExp rxprogress( " +(\\d+)\\b\\b\\b\\b\\b\\b\\b\\b\\b\\b" );

  // We return simple messages in html non formated, monospace text should be set on widget
  // where it is used because output may be formated assuming fixed width font
  if ( input.trimmed().isEmpty() )
  {
    return OutputNone;
  }
  else if ( rxpercent.indexIn( input ) != -1 )
  {
    value = rxpercent.cap( 1 ).toInt();
    return OutputPercent;
  }
  else if ( rxmessage.indexIn( input ) != -1 )
  {
    text = rxmessage.cap( 1 );
    html = text;
    return OutputMessage;
  }
  else if ( rxwarning.indexIn( input ) != -1 )
  {
    text = rxwarning.cap( 1 );
    QString img = QgsApplication::pkgDataPath() + "/themes/default/grass/grass_module_warning.png";
    html = "<img src=\"" + img + "\">" + text;
    return OutputWarning;
  }
  else if ( rxerror.indexIn( input ) != -1 )
  {
    text = rxerror.cap( 1 );
    QString img = QgsApplication::pkgDataPath() + "/themes/default/grass/grass_module_error.png";
    html =  "<img src=\"" + img + "\">" + text;
    return OutputError;
  }
  else if ( rxend.indexIn( input ) != -1 )
  {
    return OutputNone;
  }
  else if ( rxprogress.indexIn( input ) != -1 )
  {
    value = rxprogress.cap( 1 ).toInt();
    return OutputProgress;
  }
  else // some plain text which cannot be parsed
  {
    text = input;
    html = text;
    return OutputMessage;
  }
}
