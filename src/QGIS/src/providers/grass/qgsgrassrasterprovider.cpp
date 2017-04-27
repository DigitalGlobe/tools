/***************************************************************************
  qgsgrassrasterprovider.cpp  -  QGIS Data provider for
                           GRASS rasters
                             -------------------
    begin                : 16 Jan, 2010
    copyright            : (C) 2010 by Radim Blazek
    email                : radim dot blazek at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <limits>

#include "qgslogger.h"
#include "qgsgrass.h"
#include "qgsrasteridentifyresult.h"
#include "qgsgrassrasterprovider.h"
#include "qgsconfig.h"

#include "qgsapplication.h"
#include "qgscoordinatetransform.h"
#include "qgsrectangle.h"
#include "qgscoordinatereferencesystem.h"
#include "qgsrasterbandstats.h"

#include <QImage>
#include <QSettings>
#include <QColor>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QHash>

#define ERR(message) QGS_ERROR_MESSAGE(message,"GRASS provider")
#define QGS_ERROR(message) QgsError(message,"GRASS provider")

// Do not use warning dialogs, providers are also created on threads (rendering) where dialogs connot be used (constructing QPixmap icon)

QgsGrassRasterProvider::QgsGrassRasterProvider( QString const & uri )
    : QgsRasterDataProvider( uri )
    , mValid( false )
    , mGrassDataType( 0 )
    , mCols( 0 )
    , mRows( 0 )
    , mYBlockSize( 0 )
    , mNoDataValue( std::numeric_limits<double>::quiet_NaN() )
{
  QgsDebugMsg( "QgsGrassRasterProvider: constructing with uri '" + uri + "'." );

  if ( !QgsGrass::init() )
  {
    return;
  }

  // Parse URI, it is the same like using GDAL, i.e. path to raster cellhd, i.e.
  // /path/to/gisdbase/location/mapset/cellhd/map
  QFileInfo fileInfo( uri );
  if ( !fileInfo.exists() ) // then we keep it valid forever
  {
    appendError( ERR( tr( "cellhd file %1 does not exist" ).arg( uri ) ) );
    return;
  }

  mMapName = fileInfo.fileName();
  QDir dir = fileInfo.dir();
  QString element = dir.dirName();
  if ( element != "cellhd" )
  {
    appendError( ERR( tr( "Groups not yet supported" ) ) );
    return;
  }
  dir.cdUp(); // skip cellhd
  mMapset = dir.dirName();
  dir.cdUp();
  mLocation = dir.dirName();
  dir.cdUp();
  mGisdbase = dir.path();

  QgsDebugMsg( QString( "gisdbase: %1" ).arg( mGisdbase ) );
  QgsDebugMsg( QString( "location: %1" ).arg( mLocation ) );
  QgsDebugMsg( QString( "mapset: %1" ).arg( mMapset ) );
  QgsDebugMsg( QString( "mapName: %1" ).arg( mMapName ) );

  mTimestamp = dataTimestamp();

  mRasterValue.set( mGisdbase, mLocation, mMapset, mMapName );
  //mValidNoDataValue = true;

  QString error;
  mCrs = QgsGrass::crs( mGisdbase, mLocation, error );
  appendIfError( error );
  QgsDebugMsg( "mCrs: " + mCrs.toWkt() );

  // the block size can change of course when the raster is overridden
  // ibut it is only called once when statistics are calculated
  error.clear();
  QgsGrass::size( mGisdbase, mLocation, mMapset, mMapName, &mCols, &mRows, error );
  appendIfError( error );

  error.clear();
  mInfo = QgsGrass::info( mGisdbase, mLocation, mMapset, mMapName, QgsGrassObject::Raster,
                          "info", QgsRectangle(), 0, 0, 3000, error );
  appendIfError( error );

  mGrassDataType = mInfo["TYPE"].toInt();
  QgsDebugMsg( "mGrassDataType = " + QString::number( mGrassDataType ) );

  // TODO: avoid showing these strange numbers in GUI
  // TODO: don't save no data values in project file, add a flag if value was defined by user

  double myInternalNoDataValue;
  if ( mGrassDataType == CELL_TYPE )
  {
    myInternalNoDataValue = INT_MIN;
  }
  else if ( mGrassDataType == DCELL_TYPE )
  {
    // Don't use numeric limits, raster layer is using
    //    qAbs( myValue - mNoDataValue ) <= TINY_VALUE
    // if the mNoDataValue would be a limit, the subtraction could overflow.
    // No data value is shown in GUI, use some nice number.
    // Choose values with small representation error.
    // limit: 1.7976931348623157e+308
    //myInternalNoDataValue = -1e+300;
    myInternalNoDataValue = std::numeric_limits<double>::quiet_NaN();
  }
  else
  {
    if ( mGrassDataType != FCELL_TYPE )
    {
      QgsDebugMsg( "unexpected data type" );
    }

    // limit: 3.40282347e+38
    //myInternalNoDataValue = -1e+30;
    myInternalNoDataValue = std::numeric_limits<float>::quiet_NaN();
  }
  mNoDataValue = myInternalNoDataValue;
  mSrcHasNoDataValue.append( true );
  mSrcNoDataValue.append( mNoDataValue );
  mUseSrcNoDataValue.append( true );
  QgsDebugMsg( QString( "myInternalNoDataValue = %1" ).arg( myInternalNoDataValue ) );

  // TODO: refresh mRows and mCols if raster was rewritten
  // We have to decide some reasonable block size, not to big to occupate too much
  // memory, not too small to result in too many calls to readBlock -> qgis.d.rast
  // for statistics
  int typeSize = dataTypeSize( dataType( 1 ) );
  if ( mCols > 0 && typeSize > 0 )
  {
    const int cache_size = 10000000; // ~ 10 MB
    mYBlockSize = cache_size / typeSize / mCols;
    if ( mYBlockSize > mRows )
    {
      mYBlockSize = mRows;
    }
    QgsDebugMsg( "mYBlockSize = " + QString::number( mYBlockSize ) );
    mValid = true;
  }
}

QgsGrassRasterProvider::~QgsGrassRasterProvider()
{
  QgsDebugMsg( "QgsGrassRasterProvider: deconstructing." );
}

QgsRasterInterface * QgsGrassRasterProvider::clone() const
{
  QgsGrassRasterProvider * provider = new QgsGrassRasterProvider( dataSourceUri() );
  provider->copyBaseSettings( *this );
  return provider;
}

QImage* QgsGrassRasterProvider::draw( QgsRectangle  const & viewExtent, int pixelWidth, int pixelHeight )
{
  QgsDebugMsg( "pixelWidth = "  + QString::number( pixelWidth ) );
  QgsDebugMsg( "pixelHeight = "  + QString::number( pixelHeight ) );
  QgsDebugMsg( "viewExtent: " + viewExtent.toString() );
  clearLastError();

  QImage *image = new QImage( pixelWidth, pixelHeight, QImage::Format_ARGB32 );
  image->fill( QColor( Qt::gray ).rgb() );

  QStringList arguments;
  arguments.append( "map=" +  mMapName + "@" + mMapset );

  arguments.append(( QString( "window=%1,%2,%3,%4,%5,%6" )
                     .arg( QgsRasterBlock::printValue( viewExtent.xMinimum() ),
                           QgsRasterBlock::printValue( viewExtent.yMinimum() ),
                           QgsRasterBlock::printValue( viewExtent.xMaximum() ),
                           QgsRasterBlock::printValue( viewExtent.yMaximum() ) )
                     .arg( pixelWidth ).arg( pixelHeight ) ) );
  QString cmd = QgsApplication::libexecPath() + "grass/modules/qgis.d.rast";
  QByteArray data;
  try
  {
    data = QgsGrass::runModule( mGisdbase, mLocation, mMapset, cmd, arguments );
  }
  catch ( QgsGrass::Exception &e )
  {
    QString error = tr( "Cannot draw raster" ) + " : " + e.what();
    QgsDebugMsg( error );
    appendError( error );
    // We don't set mValid to false, because the raster can be recreated and work next time
    return image;
  }
  QgsDebugMsg( QString( "%1 bytes read from modules stdout" ).arg( data.size() ) );
  uchar * ptr = image->bits();
  // byteCount() in Qt >= 4.6
  //int size = image->byteCount() < data.size() ? image->byteCount() : data.size();
  int size = pixelWidth * pixelHeight * 4 < data.size() ? pixelWidth * pixelHeight * 4 : data.size();
  memcpy( ptr, data.data(), size );

  return image;
}


void QgsGrassRasterProvider::readBlock( int bandNo, int xBlock, int yBlock, void *block )
{
  Q_UNUSED( xBlock );
  clearLastError();
  // TODO: optimize, see extent()

  QgsDebugMsg( "yBlock = "  + QString::number( yBlock ) );

  QStringList arguments;
  arguments.append( "map=" +  mMapName + "@" + mMapset );

  QgsRectangle ext = extent();


  // TODO: cut the last block
  double cellHeight = ext.height() / mRows;
  double yMaximum = ext.yMaximum() - cellHeight * yBlock * mYBlockSize;
  double yMinimum = yMaximum - cellHeight * mYBlockSize;

  QgsDebugMsg( "mYBlockSize = " + QString::number( mYBlockSize ) );
  arguments.append(( QString( "window=%1,%2,%3,%4,%5,%6" )
                     .arg( QgsRasterBlock::printValue( ext.xMinimum() ),
                           QgsRasterBlock::printValue( yMinimum ),
                           QgsRasterBlock::printValue( ext.xMaximum() ),
                           QgsRasterBlock::printValue( yMaximum ) )
                     .arg( mCols ).arg( mYBlockSize ) ) );

  arguments.append( "format=value" );
  QString cmd = QgsApplication::libexecPath() + "grass/modules/qgis.d.rast";
  QByteArray data;
  try
  {
    data = QgsGrass::runModule( mGisdbase, mLocation, mMapset, cmd, arguments );
  }
  catch ( QgsGrass::Exception &e )
  {
    QString error = tr( "Cannot read raster" ) + " : " + e.what();
    QgsDebugMsg( error );
    appendError( error );
    // We don't set mValid to false, because the raster can be recreated and work next time
  }
  QgsDebugMsg( QString( "%1 bytes read from modules stdout" ).arg( data.size() ) );
  // byteCount() in Qt >= 4.6
  //int size = image->byteCount() < data.size() ? image->byteCount() : data.size();
  int size = mCols * mYBlockSize * dataTypeSize( bandNo );
  QgsDebugMsg( QString( "mCols = %1 mYBlockSize = %2 dataTypeSize = %3" ).arg( mCols ).arg( mYBlockSize ).arg( dataTypeSize( bandNo ) ) );
  if ( size != data.size() )
  {
    QString error = tr( "%1 bytes expected but %2 byte were read from qgis.d.rast" ).arg( size ).arg( data.size() );
    QgsDebugMsg( error );
    appendError( error );
    size = size < data.size() ? size : data.size();
  }
  memcpy( block, data.data(), size );
}

void QgsGrassRasterProvider::readBlock( int bandNo, QgsRectangle  const & viewExtent, int pixelWidth, int pixelHeight, void *block )
{
  QgsDebugMsg( "pixelWidth = "  + QString::number( pixelWidth ) );
  QgsDebugMsg( "pixelHeight = "  + QString::number( pixelHeight ) );
  QgsDebugMsg( "viewExtent: " + viewExtent.toString() );
  clearLastError();

  if ( pixelWidth <= 0 || pixelHeight <= 0 )
    return;

  QStringList arguments;
  arguments.append( "map=" +  mMapName + "@" + mMapset );

  arguments.append(( QString( "window=%1,%2,%3,%4,%5,%6" )
                     .arg( QgsRasterBlock::printValue( viewExtent.xMinimum() ),
                           QgsRasterBlock::printValue( viewExtent.yMinimum() ),
                           QgsRasterBlock::printValue( viewExtent.xMaximum() ),
                           QgsRasterBlock::printValue( viewExtent.yMaximum() ) )
                     .arg( pixelWidth ).arg( pixelHeight ) ) );
  arguments.append( "format=value" );
  QString cmd = QgsApplication::libexecPath() + "grass/modules/qgis.d.rast";
  QByteArray data;
  try
  {
    data = QgsGrass::runModule( mGisdbase, mLocation, mMapset, cmd, arguments );
  }
  catch ( QgsGrass::Exception &e )
  {
    QString error = tr( "Cannot read raster" ) + " : " + e.what();
    QgsDebugMsg( error );
    appendError( error );

    // We don't set mValid to false, because the raster can be recreated and work next time
    return;
  }
  QgsDebugMsg( QString( "%1 bytes read from modules stdout" ).arg( data.size() ) );
  // byteCount() in Qt >= 4.6
  //int size = image->byteCount() < data.size() ? image->byteCount() : data.size();
  int size = pixelWidth * pixelHeight * dataTypeSize( bandNo );
  if ( size != data.size() )
  {
    QString error = tr( "%1 bytes expected but %2 byte were read from qgis.d.rast" ).arg( size ).arg( data.size() );
    QgsDebugMsg( error );
    appendError( error );
    size = size < data.size() ? size : data.size();
  }
  memcpy( block, data.data(), size );
}

QgsRasterBandStats QgsGrassRasterProvider::bandStatistics( int theBandNo, int theStats, const QgsRectangle & theExtent, int theSampleSize )
{
  QgsDebugMsg( QString( "theBandNo = %1 theSampleSize = %2" ).arg( theBandNo ).arg( theSampleSize ) );
  QgsRasterBandStats myRasterBandStats;
  initStatistics( myRasterBandStats, theBandNo, theStats, theExtent, theSampleSize );

  Q_FOREACH ( const QgsRasterBandStats& stats, mStatistics )
  {
    if ( stats.contains( myRasterBandStats ) )
    {
      QgsDebugMsg( "Using cached statistics." );
      return stats;
    }
  }

  QgsRectangle extent = myRasterBandStats.extent;

  int sampleRows = myRasterBandStats.height;
  int sampleCols = myRasterBandStats.width;

  // With stats we have to be careful about timeout, empirical value,
  // 0.001 / cell should be sufficient using 0.005 to be sure + constant (ms)
  int timeout = 30000 + 0.005 * xSize() * ySize();

  QString error;
  QHash<QString, QString> info = QgsGrass::info( mGisdbase, mLocation, mMapset, mMapName, QgsGrassObject::Raster,
                                 "stats", extent, sampleRows, sampleCols, timeout, error );

  if ( info.isEmpty() || !error.isEmpty() )
  {
    return myRasterBandStats;
  }

  myRasterBandStats.sum = info["SUM"].toDouble();
  myRasterBandStats.elementCount = info["COUNT"].toInt();
  myRasterBandStats.minimumValue = info["MIN"].toDouble();
  myRasterBandStats.maximumValue = info["MAX"].toDouble();
  myRasterBandStats.range = myRasterBandStats.maximumValue - myRasterBandStats.minimumValue;
  myRasterBandStats.sumOfSquares = info["SQSUM"].toDouble();
  myRasterBandStats.mean = info["MEAN"].toDouble();
  myRasterBandStats.stdDev = info["STDEV"].toDouble();

  QgsDebugMsg( QString( "min = %1" ).arg( myRasterBandStats.minimumValue ) );
  QgsDebugMsg( QString( "max = %1" ).arg( myRasterBandStats.maximumValue ) );
  QgsDebugMsg( QString( "count = %1" ).arg( myRasterBandStats.elementCount ) );
  QgsDebugMsg( QString( "stdev = %1" ).arg( myRasterBandStats.stdDev ) );

  myRasterBandStats.statsGathered = QgsRasterBandStats::Min | QgsRasterBandStats::Max |
                                    QgsRasterBandStats::Range | QgsRasterBandStats::Mean |
                                    QgsRasterBandStats::Sum | QgsRasterBandStats::SumOfSquares |
                                    QgsRasterBandStats::StdDev;

  mStatistics.append( myRasterBandStats );
  return myRasterBandStats;
}

QList<QgsColorRampShader::ColorRampItem> QgsGrassRasterProvider::colorTable( int bandNo )const
{
  Q_UNUSED( bandNo );
  QList<QgsColorRampShader::ColorRampItem> ct;

  // TODO: check if color can be realy discontinuous in GRASS,
  // for now we just believe that they are continuous, i.e. end and beginning
  // of the ramp with the same value has the same color
  // we are also expecting ordered CT records in the list
  QString error;
  QList<QgsGrass::Color> colors = QgsGrass::colors( mGisdbase, mLocation, mMapset, mMapName, error );
  if ( !error.isEmpty() )
  {
    return ct;
  }
  QList<QgsGrass::Color>::iterator i;

  double v = 0.0, r = 0.0, g = 0.0, b = 0.0;
  for ( i = colors.begin(); i != colors.end(); ++i )
  {
    if ( ct.count() == 0 || i->value1 != v || i->red1 != r || i->green1 != g || i->blue1 != b )
    {
      // not added in previous rule
      QgsColorRampShader::ColorRampItem ctItem1;
      ctItem1.value = i->value1;
      ctItem1.color = QColor::fromRgb( i->red1, i->green1, i->blue1 );
      ct.append( ctItem1 );
      QgsDebugMsg( QString( "color %1 %2 %3 %4" ).arg( i->value1 ).arg( i->red1 ).arg( i->green1 ).arg( i->blue1 ) );
    }
    QgsColorRampShader::ColorRampItem ctItem2;
    ctItem2.value = i->value2;
    ctItem2.color = QColor::fromRgb( i->red2, i->green2, i->blue2 );
    ct.append( ctItem2 );
    QgsDebugMsg( QString( "color %1 %2 %3 %4" ).arg( i->value2 ).arg( i->red2 ).arg( i->green2 ).arg( i->blue2 ) );

    v = i->value2;
    r = i->red2;
    g = i->green2;
    b = i->blue2;
  }
  return ct;
}

QgsCoordinateReferenceSystem QgsGrassRasterProvider::crs()
{
  return mCrs;
}

QgsRectangle QgsGrassRasterProvider::extent()
{
  // The extend can change of course so we get always fresh, to avoid running always the module
  // we should save mExtent and mLastModified and check if the map was modified

  QString error;
  mExtent = QgsGrass::extent( mGisdbase, mLocation, mMapset, mMapName, QgsGrassObject::Raster, error );

  QgsDebugMsg( "Extent got" );
  return mExtent;
}

// this is only called once when statistics are calculated
int QgsGrassRasterProvider::xBlockSize() const { return mCols; }
int QgsGrassRasterProvider::yBlockSize() const
{
  return mYBlockSize;
}

// TODO this should be always refreshed if raster has changed ?
// maybe also only for stats
int QgsGrassRasterProvider::xSize() const { return mCols; }
int QgsGrassRasterProvider::ySize() const { return mRows; }

QgsRasterIdentifyResult QgsGrassRasterProvider::identify( const QgsPoint & thePoint, QgsRaster::IdentifyFormat theFormat, const QgsRectangle &theExtent, int theWidth, int theHeight, int /*theDpi*/ )
{
  Q_UNUSED( theExtent );
  Q_UNUSED( theWidth );
  Q_UNUSED( theHeight );
  QMap<int, QVariant> results;
  QMap<int, QVariant> noDataResults;
  noDataResults.insert( 1, QVariant() );
  QgsRasterIdentifyResult noDataResult( QgsRaster::IdentifyFormatValue, results );

  if ( theFormat != QgsRaster::IdentifyFormatValue )
  {
    return QgsRasterIdentifyResult( QGS_ERROR( tr( "Format not supported" ) ) );
  }

  if ( !extent().contains( thePoint ) )
  {
    return noDataResult;
  }

  // TODO: use doubles instead of strings

  // attention, value tool does his own tricks with grass identify() so it stops to refresh values outside extent or null values e.g.

  bool ok;
  double value = mRasterValue.value( thePoint.x(), thePoint.y(), &ok );

  if ( !ok )
  {
    return QgsRasterIdentifyResult( QGS_ERROR( tr( "Cannot read data" ) ) );
  }

  // no data?
  if ( qIsNaN( value ) || qgsDoubleNear( value, mNoDataValue ) )
  {
    return noDataResult;
  }

  // Apply user no data
  QgsRasterRangeList myNoDataRangeList = userNoDataValues( 1 );
  if ( QgsRasterRange::contains( value, myNoDataRangeList ) )
  {
    return noDataResult;
  }

  results.insert( 1, value );

  return QgsRasterIdentifyResult( QgsRaster::IdentifyFormatValue, results );
}

int QgsGrassRasterProvider::capabilities() const
{
  int capability = QgsRasterDataProvider::Identify
                   | QgsRasterDataProvider::IdentifyValue
                   | QgsRasterDataProvider::Size;
  return capability;
}

QGis::DataType QgsGrassRasterProvider::dataType( int bandNo ) const
{
  return srcDataType( bandNo );
}

QGis::DataType QgsGrassRasterProvider::srcDataType( int bandNo ) const
{
  Q_UNUSED( bandNo );
  switch ( mGrassDataType )
  {
    case CELL_TYPE:
      return QGis::Int32;
    case FCELL_TYPE:
      return QGis::Float32;
    case DCELL_TYPE:
      return QGis::Float64;
  }
  return QGis::UnknownDataType;
}

int QgsGrassRasterProvider::bandCount() const
{
  // TODO
  return 1;
}

int QgsGrassRasterProvider::colorInterpretation( int bandNo ) const
{
  // TODO: avoid loading color table here or cache it
  QList<QgsColorRampShader::ColorRampItem> ct = colorTable( bandNo );
  if ( ct.size() > 0 )
  {
    return QgsRaster::ContinuousPalette;
  }
  return QgsRaster::GrayIndex;
}

QString QgsGrassRasterProvider::metadata()
{
  QString myMetadata;
  QStringList myList;
  myList.append( "GISDBASE: " + mGisdbase );
  myList.append( "LOCATION: " + mLocation );
  myList.append( "MAPSET: " + mMapset );
  myList.append( "MAP: " + mMapName );

  QHash<QString, QString>::iterator i;
  for ( i = mInfo.begin(); i != mInfo.end(); ++i )
  {
    myList.append( i.key() + " : " + i.value() );
  }
  myMetadata += QgsRasterDataProvider::makeTableCells( myList );


  return myMetadata;
}

bool QgsGrassRasterProvider::isValid()
{
  return mValid;
}

void QgsGrassRasterProvider::setLastError( QString error )
{
  mLastErrorTitle = tr( "GRASS raster provider" );
  mLastError = error;
}

void QgsGrassRasterProvider::clearLastError()
{
  mLastErrorTitle.clear();
  mLastError.clear();
}

void QgsGrassRasterProvider::appendIfError( QString error )
{
  if ( !error.isEmpty() )
  {
    appendError( ERR( error ) );
  }
}

QString QgsGrassRasterProvider::lastErrorTitle()
{
  return mLastErrorTitle;
}

QString QgsGrassRasterProvider::lastError()
{
  return mLastError;
}

QString  QgsGrassRasterProvider::name() const
{
  return QString( "grassraster" );
}

QString  QgsGrassRasterProvider::description() const
{
  return QString( "GRASS %1 raster provider" ).arg( GRASS_VERSION_MAJOR );
}

QDateTime QgsGrassRasterProvider::dataTimestamp() const
{
  QDateTime time;
  QString mapset = mGisdbase + "/" + mLocation + "/" + mMapset;
  QStringList dirs;
  dirs << "cell" << "colr";
  Q_FOREACH ( const QString& dir, dirs )
  {
    QString path = mapset + "/" + dir + "/" + mMapName;
    QFileInfo fi( path );
    if ( fi.exists() && fi.lastModified() > time )
    {
      time = fi.lastModified();
    }
  }
  QgsDebugMsg( "timestamp = " + time.toString() );

  return time;
}

void QgsGrassRasterProvider::freeze()
{
  mRasterValue.stop();
  mValid = false;
}

void QgsGrassRasterProvider::thaw()
{
  mValid = true;
}

//-------------------------------- QgsGrassRasterValue ----------------------------------------

QgsGrassRasterValue::QgsGrassRasterValue()
    : mProcess( 0 )
{
}

QgsGrassRasterValue::~QgsGrassRasterValue()
{
  stop();
}

void QgsGrassRasterValue::set( const QString & gisdbase, const QString & location, const QString & mapset, const QString & map )
{
  mGisdbase = gisdbase;
  mLocation = location;
  mMapset = mapset;
  mMapName = map;
}

void QgsGrassRasterValue::start()
{
  if ( mProcess )
  {
    QgsDebugMsg( "already running" );
  }
  // TODO: catch exceptions
  QString module = QgsGrass::qgisGrassModulePath() + "/qgis.g.info";
  QStringList arguments;

  arguments.append( "info=query" );
  arguments.append( "rast=" +  mMapName + "@" + mMapset );
  try
  {
    mProcess = QgsGrass::startModule( mGisdbase, mLocation, mMapset, module, arguments, mGisrcFile );
  }
  catch ( QgsGrass::Exception &e )
  {
    QString error = e.what();
    Q_UNUSED( error )
    QgsDebugMsg( error );
  }
}

void QgsGrassRasterValue::stop()
{
  if ( mProcess )
  {
    QgsDebugMsg( "closing process" );
    mProcess->closeWriteChannel();
    mProcess->waitForFinished();
    QgsDebugMsg( "process finished" );
    delete mProcess;
    mProcess = 0;
  }
}

double QgsGrassRasterValue::value( double x, double y, bool *ok )
{
  *ok = false;
  double value = std::numeric_limits<double>::quiet_NaN();

  if ( !mProcess )
  {
    start();
  }

  if ( !mProcess )
  {
    return value;
  }

  QString coor = QString( "%1 %2\n" ).arg( QgsRasterBlock::printValue( x ),
                 QgsRasterBlock::printValue( y ) );
  QgsDebugMsg( "coor : " + coor );
  mProcess->write( coor.toAscii() ); // how to flush, necessary?
  mProcess->waitForReadyRead();
  QString str = mProcess->readLine().trimmed();
  QgsDebugMsg( "read from stdout : " + str );

  // TODO: use doubles instead of strings

  QStringList list = str.trimmed().split( ":" );
  if ( list.size() == 2 )
  {
    if ( list[1] == "error" ) return value;
    value = list[1].toDouble( ok );
  }
  return value;
}


