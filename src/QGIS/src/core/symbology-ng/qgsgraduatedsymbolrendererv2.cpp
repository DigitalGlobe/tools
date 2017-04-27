
/***************************************************************************
    qgsgraduatedsymbolrendererv2.cpp
    ---------------------
    begin                : November 2009
    copyright            : (C) 2009 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "qgsgraduatedsymbolrendererv2.h"

#include "qgssymbolv2.h"
#include "qgssymbollayerv2utils.h"
#include "qgsvectorcolorrampv2.h"
#include "qgspointdisplacementrenderer.h"
#include "qgsinvertedpolygonrenderer.h"
#include "qgspainteffect.h"
#include "qgspainteffectregistry.h"
#include "qgsscaleexpression.h"
#include "qgsdatadefined.h"

#include "qgsfeature.h"
#include "qgsvectorlayer.h"
#include "qgslogger.h"
#include "qgsvectordataprovider.h"
#include "qgsexpression.h"
#include <QDomDocument>
#include <QDomElement>
#include <QSettings> // for legend
#include <limits> // for jenks classification
#include <ctime>

QgsRendererRangeV2::QgsRendererRangeV2()
    : mLowerValue( 0 )
    , mUpperValue( 0 )
    , mSymbol( nullptr )
    , mLabel()
    , mRender( true )
{
}

QgsRendererRangeV2::QgsRendererRangeV2( double lowerValue, double upperValue, QgsSymbolV2* symbol, const QString& label, bool render )
    : mLowerValue( lowerValue )
    , mUpperValue( upperValue )
    , mSymbol( symbol )
    , mLabel( label )
    , mRender( render )
{
}

QgsRendererRangeV2::QgsRendererRangeV2( const QgsRendererRangeV2& range )
    : mLowerValue( range.mLowerValue )
    , mUpperValue( range.mUpperValue )
    , mSymbol( range.mSymbol.data() ? range.mSymbol->clone() : nullptr )
    , mLabel( range.mLabel )
    , mRender( range.mRender )
{
}

// cpy and swap idiom, note that the cpy is done with 'pass by value'
QgsRendererRangeV2& QgsRendererRangeV2::operator=( QgsRendererRangeV2 range )
{
  swap( range );
  return *this;
}

bool QgsRendererRangeV2::operator<( const QgsRendererRangeV2 &other ) const
{
  return
    lowerValue() < other.lowerValue() ||
    ( qgsDoubleNear( lowerValue(), other.lowerValue() ) && upperValue() < other.upperValue() );
}


void QgsRendererRangeV2::swap( QgsRendererRangeV2 & other )
{
  qSwap( mLowerValue, other.mLowerValue );
  qSwap( mUpperValue, other.mUpperValue );
  qSwap( mSymbol, other.mSymbol );
  std::swap( mLabel, other.mLabel );
}

double QgsRendererRangeV2::lowerValue() const
{
  return mLowerValue;
}

double QgsRendererRangeV2::upperValue() const
{
  return mUpperValue;
}

QgsSymbolV2* QgsRendererRangeV2::symbol() const
{
  return mSymbol.data();
}

QString QgsRendererRangeV2::label() const
{
  return mLabel;
}

void QgsRendererRangeV2::setSymbol( QgsSymbolV2* s )
{
  if ( mSymbol.data() != s ) mSymbol.reset( s );
}

void QgsRendererRangeV2::setLabel( const QString& label )
{
  mLabel = label;
}

void QgsRendererRangeV2::setUpperValue( double upperValue )
{
  mUpperValue = upperValue;
}

void QgsRendererRangeV2::setLowerValue( double lowerValue )
{
  mLowerValue = lowerValue;
}

bool QgsRendererRangeV2::renderState() const
{
  return mRender;
}

void QgsRendererRangeV2::setRenderState( bool render )
{
  mRender = render;
}

QString QgsRendererRangeV2::dump() const
{
  return QString( "%1 - %2::%3::%4\n" ).arg( mLowerValue ).arg( mUpperValue ).arg( mLabel, mSymbol.data() ? mSymbol->dump() : "(no symbol)" );
}

void QgsRendererRangeV2::toSld( QDomDocument &doc, QDomElement &element, QgsStringMap props, bool firstRange ) const
{
  if ( !mSymbol.data() || props.value( "attribute", "" ).isEmpty() )
    return;

  QString attrName = props[ "attribute" ];

  QDomElement ruleElem = doc.createElement( "se:Rule" );
  element.appendChild( ruleElem );

  QDomElement nameElem = doc.createElement( "se:Name" );
  nameElem.appendChild( doc.createTextNode( mLabel ) );
  ruleElem.appendChild( nameElem );

  QDomElement descrElem = doc.createElement( "se:Description" );
  QDomElement titleElem = doc.createElement( "se:Title" );
  QString descrStr = QString( "range: %1 - %2" ).arg( qgsDoubleToString( mLowerValue ), qgsDoubleToString( mUpperValue ) );
  titleElem.appendChild( doc.createTextNode( !mLabel.isEmpty() ? mLabel : descrStr ) );
  descrElem.appendChild( titleElem );
  ruleElem.appendChild( descrElem );

  // create the ogc:Filter for the range
  QString filterFunc = QString( "%1 %2 %3 AND %1 <= %4" )
                       .arg( attrName.replace( '\"', "\"\"" ),
                             firstRange ? ">=" : ">",
                             qgsDoubleToString( mLowerValue ),
                             qgsDoubleToString( mUpperValue ) );
  QgsSymbolLayerV2Utils::createFunctionElement( doc, ruleElem, filterFunc );

  mSymbol->toSld( doc, ruleElem, props );
}

///////////

int QgsRendererRangeV2LabelFormat::MaxPrecision = 15;
int QgsRendererRangeV2LabelFormat::MinPrecision = -6;

QgsRendererRangeV2LabelFormat::QgsRendererRangeV2LabelFormat()
    : mFormat( " %1 - %2 " )
    , mPrecision( 4 )
    , mTrimTrailingZeroes( false )
    , mNumberScale( 1.0 )
    , mNumberSuffix( "" )
    , mReTrailingZeroes( "[.,]?0*$" )
    , mReNegativeZero( "^\\-0(?:[.,]0*)?$" )
{
}

QgsRendererRangeV2LabelFormat::QgsRendererRangeV2LabelFormat( const QString& format, int precision, bool trimTrailingZeroes )
    : mReTrailingZeroes( "[.,]?0*$" )
    , mReNegativeZero( "^\\-0(?:[.,]0*)?$" )
{
  setFormat( format );
  setPrecision( precision );
  setTrimTrailingZeroes( trimTrailingZeroes );
}


bool QgsRendererRangeV2LabelFormat::operator==( const QgsRendererRangeV2LabelFormat &other ) const
{
  return
    format() == other.format() &&
    precision() == other.precision() &&
    trimTrailingZeroes() == other.trimTrailingZeroes();
}

bool QgsRendererRangeV2LabelFormat::operator!=( const QgsRendererRangeV2LabelFormat &other ) const
{
  return !( *this == other );
}

void QgsRendererRangeV2LabelFormat::setPrecision( int precision )
{
  // Limit the range of decimal places to a reasonable range
  precision = qBound( MinPrecision, precision, MaxPrecision );
  mPrecision = precision;
  mNumberScale = 1.0;
  mNumberSuffix = "";
  while ( precision < 0 )
  {
    precision++;
    mNumberScale /= 10.0;
    mNumberSuffix.append( '0' );
  }
}

QString QgsRendererRangeV2LabelFormat::labelForRange( const QgsRendererRangeV2 &range ) const
{
  return labelForRange( range.lowerValue(), range.upperValue() );
}

QString QgsRendererRangeV2LabelFormat::formatNumber( double value ) const
{
  if ( mPrecision > 0 )
  {
    QString valueStr = QString::number( value, 'f', mPrecision );
    if ( mTrimTrailingZeroes )
      valueStr = valueStr.remove( mReTrailingZeroes );
    if ( mReNegativeZero.exactMatch( valueStr ) )
      valueStr = valueStr.mid( 1 );
    return valueStr;
  }
  else
  {
    QString valueStr = QString::number( value * mNumberScale, 'f', 0 );
    if ( valueStr == "-0" )
      valueStr = '0';
    if ( valueStr != "0" )
      valueStr = valueStr + mNumberSuffix;
    return valueStr;
  }
}

QString QgsRendererRangeV2LabelFormat::labelForRange( double lower, double upper ) const
{
  QString lowerStr = formatNumber( lower );
  QString upperStr = formatNumber( upper );

  QString legend( mFormat );
  return legend.replace( "%1", lowerStr ).replace( "%2", upperStr );
}

void QgsRendererRangeV2LabelFormat::setFromDomElement( QDomElement &element )
{
  mFormat = element.attribute( "format",
                               element.attribute( "prefix", " " ) + "%1" +
                               element.attribute( "separator", " - " ) + "%2" +
                               element.attribute( "suffix", " " )
                             );
  setPrecision( element.attribute( "decimalplaces", "4" ).toInt() );
  mTrimTrailingZeroes = element.attribute( "trimtrailingzeroes", "false" ) == "true";
}

void QgsRendererRangeV2LabelFormat::saveToDomElement( QDomElement &element )
{
  element.setAttribute( "format", mFormat );
  element.setAttribute( "decimalplaces", mPrecision );
  element.setAttribute( "trimtrailingzeroes", mTrimTrailingZeroes ? "true" : "false" );
}

///////////

QgsGraduatedSymbolRendererV2::QgsGraduatedSymbolRendererV2( const QString& attrName, const QgsRangeList& ranges )
    : QgsFeatureRendererV2( "graduatedSymbol" )
    , mAttrName( attrName )
    , mMode( Custom )
    , mInvertedColorRamp( false )
    , mScaleMethod( DEFAULT_SCALE_METHOD )
    , mGraduatedMethod( GraduatedColor )
    , mAttrNum( -1 )
    , mCounting( false )

{
  // TODO: check ranges for sanity (NULL symbols, invalid ranges)

  //important - we need a deep copy of the ranges list, not a shared copy. This is required because
  //QgsRendererRangeV2::symbol() is marked const, and so retrieving the symbol via this method does not
  //trigger a detachment and copy of mRanges BUT that same method CAN be used to modify a symbol in place
  Q_FOREACH ( const QgsRendererRangeV2& range, ranges )
  {
    mRanges << range;
  }

}

QgsGraduatedSymbolRendererV2::~QgsGraduatedSymbolRendererV2()
{
  mRanges.clear(); // should delete all the symbols
}

QgsSymbolV2* QgsGraduatedSymbolRendererV2::symbolForValue( double value )
{
  Q_FOREACH ( const QgsRendererRangeV2& range, mRanges )
  {
    if ( range.lowerValue() <= value && range.upperValue() >= value )
    {
      if ( range.renderState() || mCounting )
        return range.symbol();
      else
        return nullptr;
    }
  }
  // the value is out of the range: return NULL instead of symbol
  return nullptr;
}

QString QgsGraduatedSymbolRendererV2::legendKeyForValue( double value ) const
{
  int i = 0;
  Q_FOREACH ( const QgsRendererRangeV2& range, mRanges )
  {
    if ( range.lowerValue() <= value && range.upperValue() >= value )
    {
      if ( range.renderState() || mCounting )
        return QString::number( i );
      else
        return QString::null;
    }
    i++;
  }
  // the value is out of the range: return NULL
  return QString::null;
}

QgsSymbolV2* QgsGraduatedSymbolRendererV2::symbolForFeature( QgsFeature& feature, QgsRenderContext &context )
{
  QgsSymbolV2* symbol = originalSymbolForFeature( feature, context );
  if ( !symbol )
    return nullptr;

  if ( !mRotation.data() && !mSizeScale.data() )
    return symbol; // no data-defined rotation/scaling - just return the symbol

  // find out rotation, size scale
  const double rotation = mRotation.data() ? mRotation->evaluate( &context.expressionContext() ).toDouble() : 0;
  const double sizeScale = mSizeScale.data() ? mSizeScale->evaluate( &context.expressionContext() ).toDouble() : 1.;

  // take a temporary symbol (or create it if doesn't exist)
  QgsSymbolV2* tempSymbol = mTempSymbols[symbol];

  // modify the temporary symbol and return it
  if ( tempSymbol->type() == QgsSymbolV2::Marker )
  {
    QgsMarkerSymbolV2* markerSymbol = static_cast<QgsMarkerSymbolV2*>( tempSymbol );
    if ( mRotation.data() ) markerSymbol->setAngle( rotation );
    markerSymbol->setSize( sizeScale * static_cast<QgsMarkerSymbolV2*>( symbol )->size() );
    markerSymbol->setScaleMethod( mScaleMethod );
  }
  else if ( tempSymbol->type() == QgsSymbolV2::Line )
  {
    QgsLineSymbolV2* lineSymbol = static_cast<QgsLineSymbolV2*>( tempSymbol );
    lineSymbol->setWidth( sizeScale * static_cast<QgsLineSymbolV2*>( symbol )->width() );
  }
  return tempSymbol;
}

QVariant QgsGraduatedSymbolRendererV2::valueForFeature( QgsFeature& feature, QgsRenderContext &context ) const
{
  QgsAttributes attrs = feature.attributes();
  QVariant value;
  if ( mAttrNum < 0 || mAttrNum >= attrs.count() )
  {
    value = mExpression->evaluate( &context.expressionContext() );
  }
  else
  {
    value = attrs.at( mAttrNum );
  }

  return value;
}

QgsSymbolV2* QgsGraduatedSymbolRendererV2::originalSymbolForFeature( QgsFeature& feature, QgsRenderContext &context )
{
  QVariant value = valueForFeature( feature, context );

  // Null values should not be categorized
  if ( value.isNull() )
    return nullptr;

  // find the right category
  return symbolForValue( value.toDouble() );
}

void QgsGraduatedSymbolRendererV2::startRender( QgsRenderContext& context, const QgsFields& fields )
{
  mCounting = context.rendererScale() == 0.0;

  // find out classification attribute index from name
  mAttrNum = fields.fieldNameIndex( mAttrName );

  if ( mAttrNum == -1 )
  {
    mExpression.reset( new QgsExpression( mAttrName ) );
    mExpression->prepare( &context.expressionContext() );
  }

  Q_FOREACH ( const QgsRendererRangeV2& range, mRanges )
  {
    if ( !range.symbol() )
      continue;

    range.symbol()->startRender( context, &fields );

    if ( mRotation.data() || mSizeScale.data() )
    {
      QgsSymbolV2* tempSymbol = range.symbol()->clone();
      tempSymbol->setRenderHints(( mRotation.data() ? QgsSymbolV2::DataDefinedRotation : 0 ) |
                                 ( mSizeScale.data() ? QgsSymbolV2::DataDefinedSizeScale : 0 ) );
      tempSymbol->startRender( context, &fields );
      mTempSymbols[ range.symbol()] = tempSymbol;
    }
  }
  return;
}

void QgsGraduatedSymbolRendererV2::stopRender( QgsRenderContext& context )
{
  Q_FOREACH ( const QgsRendererRangeV2& range, mRanges )
  {
    if ( !range.symbol() )
      continue;

    range.symbol()->stopRender( context );
  }

  // cleanup mTempSymbols
  QHash<QgsSymbolV2*, QgsSymbolV2*>::const_iterator it2 = mTempSymbols.constBegin();
  for ( ; it2 != mTempSymbols.constEnd(); ++it2 )
  {
    it2.value()->stopRender( context );
    delete it2.value();
  }
  mTempSymbols.clear();
}

QList<QString> QgsGraduatedSymbolRendererV2::usedAttributes()
{
  QSet<QString> attributes;

  // mAttrName can contain either attribute name or an expression.
  // Sometimes it is not possible to distinguish between those two,
  // e.g. "a - b" can be both a valid attribute name or expression.
  // Since we do not have access to fields here, try both options.
  attributes << mAttrName;

  QgsExpression testExpr( mAttrName );
  if ( !testExpr.hasParserError() )
    attributes.unite( testExpr.referencedColumns().toSet() );

  if ( mRotation.data() ) attributes.unite( mRotation->referencedColumns().toSet() );
  if ( mSizeScale.data() ) attributes.unite( mSizeScale->referencedColumns().toSet() );

  QgsRangeList::const_iterator range_it = mRanges.constBegin();
  for ( ; range_it != mRanges.constEnd(); ++range_it )
  {
    QgsSymbolV2* symbol = range_it->symbol();
    if ( symbol )
    {
      attributes.unite( symbol->usedAttributes() );
    }
  }
  return attributes.toList();
}

bool QgsGraduatedSymbolRendererV2::updateRangeSymbol( int rangeIndex, QgsSymbolV2* symbol )
{
  if ( rangeIndex < 0 || rangeIndex >= mRanges.size() )
    return false;
  mRanges[rangeIndex].setSymbol( symbol );
  return true;
}

bool QgsGraduatedSymbolRendererV2::updateRangeLabel( int rangeIndex, const QString& label )
{
  if ( rangeIndex < 0 || rangeIndex >= mRanges.size() )
    return false;
  mRanges[rangeIndex].setLabel( label );
  return true;
}

bool QgsGraduatedSymbolRendererV2::updateRangeUpperValue( int rangeIndex, double value )
{
  if ( rangeIndex < 0 || rangeIndex >= mRanges.size() )
    return false;
  QgsRendererRangeV2 &range = mRanges[rangeIndex];
  bool isDefaultLabel = range.label() == mLabelFormat.labelForRange( range );
  range.setUpperValue( value );
  if ( isDefaultLabel ) range.setLabel( mLabelFormat.labelForRange( range ) );
  return true;
}

bool QgsGraduatedSymbolRendererV2::updateRangeLowerValue( int rangeIndex, double value )
{
  if ( rangeIndex < 0 || rangeIndex >= mRanges.size() )
    return false;
  QgsRendererRangeV2 &range = mRanges[rangeIndex];
  bool isDefaultLabel = range.label() == mLabelFormat.labelForRange( range );
  range.setLowerValue( value );
  if ( isDefaultLabel ) range.setLabel( mLabelFormat.labelForRange( range ) );
  return true;
}

bool QgsGraduatedSymbolRendererV2::updateRangeRenderState( int rangeIndex, bool value )
{
  if ( rangeIndex < 0 || rangeIndex >= mRanges.size() )
    return false;
  mRanges[rangeIndex].setRenderState( value );
  return true;
}

QString QgsGraduatedSymbolRendererV2::dump() const
{
  QString s = QString( "GRADUATED: attr %1\n" ).arg( mAttrName );
  for ( int i = 0; i < mRanges.count(); i++ )
    s += mRanges[i].dump();
  return s;
}

QgsGraduatedSymbolRendererV2* QgsGraduatedSymbolRendererV2::clone() const
{
  QgsGraduatedSymbolRendererV2* r = new QgsGraduatedSymbolRendererV2( mAttrName, mRanges );
  r->setMode( mMode );
  if ( mSourceSymbol.data() )
    r->setSourceSymbol( mSourceSymbol->clone() );
  if ( mSourceColorRamp.data() )
  {
    r->setSourceColorRamp( mSourceColorRamp->clone() );
    r->setInvertedColorRamp( mInvertedColorRamp );
  }
  r->setUsingSymbolLevels( usingSymbolLevels() );
  r->setSizeScaleField( sizeScaleField() );
  r->setLabelFormat( labelFormat() );
  r->setGraduatedMethod( graduatedMethod() );
  copyRendererData( r );
  return r;
}

void QgsGraduatedSymbolRendererV2::toSld( QDomDocument& doc, QDomElement &element ) const
{
  QgsStringMap props;
  props[ "attribute" ] = mAttrName;
  props[ "method" ] = graduatedMethodStr( mGraduatedMethod );
  if ( mRotation.data() )
    props[ "angle" ] = mRotation->expression();
  if ( mSizeScale.data() )
    props[ "scale" ] = mSizeScale->expression();

  // create a Rule for each range
  bool first = true;
  for ( QgsRangeList::const_iterator it = mRanges.constBegin(); it != mRanges.constEnd(); ++it )
  {
    QgsStringMap catProps( props );
    it->toSld( doc, element, catProps, first );
    first = false;
  }
}

QgsSymbolV2List QgsGraduatedSymbolRendererV2::symbols( QgsRenderContext &context )
{
  Q_UNUSED( context );
  QgsSymbolV2List lst;
  lst.reserve( mRanges.count() );
  Q_FOREACH ( const QgsRendererRangeV2& range, mRanges )
  {
    lst.append( range.symbol() );
  }
  return lst;
}

static QList<double> _calcEqualIntervalBreaks( double minimum, double maximum, int classes )
{

  // Equal interval algorithm
  //
  // Returns breaks based on dividing the range ('minimum' to 'maximum')
  // into 'classes' parts.

  double step = ( maximum - minimum ) / classes;

  QList<double> breaks;
  double value = minimum;
  breaks.reserve( classes );
  for ( int i = 0; i < classes; i++ )
  {
    value += step;
    breaks.append( value );
  }

  // floating point arithmetics is not precise:
  // set the last break to be exactly maximum so we do not miss it
  breaks[classes-1] = maximum;

  return breaks;
}

static QList<double> _calcQuantileBreaks( QList<double> values, int classes )
{
  // q-th quantile of a data set:
  // value where q fraction of data is below and (1-q) fraction is above this value
  // Xq = (1 - r) * X_NI1 + r * X_NI2
  //   NI1 = (int) (q * (n+1))
  //   NI2 = NI1 + 1
  //   r = q * (n+1) - (int) (q * (n+1))
  // (indices of X: 1...n)

  // sort the values first
  qSort( values );

  QList<double> breaks;

  // If there are no values to process: bail out
  if ( values.isEmpty() )
    return breaks;

  int n = values.count();
  double Xq = n > 0 ? values[0] : 0.0;

  breaks.reserve( classes );
  for ( int i = 1; i < classes; i++ )
  {
    if ( n > 1 )
    {
      double q = i  / static_cast< double >( classes );
      double a = q * ( n - 1 );
      int aa = static_cast<  int >( a );

      double r = a - aa;
      Xq = ( 1 - r ) * values[aa] + r * values[aa+1];
    }
    breaks.append( Xq );
  }

  breaks.append( values[ n-1 ] );

  return breaks;
}

static QList<double> _calcStdDevBreaks( QList<double> values, int classes, QList<double> &labels )
{

  // C++ implementation of the standard deviation class interval algorithm
  // as implemented in the 'classInt' package available for the R statistical
  // prgramming language.

  // Returns breaks based on 'prettyBreaks' of the centred and scaled
  // values of 'values', and may have a number of classes different from 'classes'.

  // If there are no values to process: bail out
  if ( values.isEmpty() )
    return QList<double>();

  double mean = 0.0;
  double stdDev = 0.0;
  int n = values.count();
  double minimum = values[0];
  double maximum = values[0];

  for ( int i = 0; i < n; i++ )
  {
    mean += values[i];
    minimum = qMin( values[i], minimum ); // could use precomputed max and min
    maximum = qMax( values[i], maximum ); // but have to go through entire list anyway
  }
  mean = mean / static_cast< double >( n );

  double sd = 0.0;
  for ( int i = 0; i < n; i++ )
  {
    sd = values[i] - mean;
    stdDev += sd * sd;
  }
  stdDev = sqrt( stdDev / n );

  QList<double> breaks = QgsSymbolLayerV2Utils::prettyBreaks(( minimum - mean ) / stdDev, ( maximum - mean ) / stdDev, classes );
  for ( int i = 0; i < breaks.count(); i++ )
  {
    labels.append( breaks[i] );
    breaks[i] = ( breaks[i] * stdDev ) + mean;
  }

  return breaks;
} // _calcStdDevBreaks

static QList<double> _calcJenksBreaks( QList<double> values, int classes,
                                       double minimum, double maximum,
                                       int maximumSize = 3000 )
{
  // Jenks Optimal (Natural Breaks) algorithm
  // Based on the Jenks algorithm from the 'classInt' package available for
  // the R statistical prgramming language, and from Python code from here:
  // http://danieljlewis.org/2010/06/07/jenks-natural-breaks-algorithm-in-python/
  // and is based on a JAVA and Fortran code available here:
  // https://stat.ethz.ch/pipermail/r-sig-geo/2006-March/000811.html

  // Returns class breaks such that classes are internally homogeneous while
  // assuring heterogeneity among classes.

  if ( values.isEmpty() )
    return QList<double>();

  if ( classes <= 1 )
  {
    return QList<double>() << maximum;
  }

  if ( classes >= values.size() )
  {
    return values;
  }

  QVector<double> sample;

  // if we have lots of values, we need to take a random sample
  if ( values.size() > maximumSize )
  {
    // for now, sample at least maximumSize values or a 10% sample, whichever
    // is larger. This will produce a more representative sample for very large
    // layers, but could end up being computationally intensive...

    sample.resize( qMax( maximumSize, values.size() / 10 ) );

    QgsDebugMsg( QString( "natural breaks (jenks) sample size: %1" ).arg( sample.size() ) );
    QgsDebugMsg( QString( "values:%1" ).arg( values.size() ) );

    sample[ 0 ] = minimum;
    sample[ 1 ] = maximum;
    for ( int i = 2; i < sample.size(); i++ )
    {
      // pick a random integer from 0 to n
      double r = qrand();
      int j = floor( r / RAND_MAX * ( values.size() - 1 ) );
      sample[ i ] = values[ j ];
    }
  }
  else
  {
    sample = values.toVector();
  }

  int n = sample.size();

  // sort the sample values
  qSort( sample );

  QVector< QVector<int> > matrixOne( n + 1 );
  QVector< QVector<double> > matrixTwo( n + 1 );

  for ( int i = 0; i <= n; i++ )
  {
    matrixOne[i].resize( classes + 1 );
    matrixTwo[i].resize( classes + 1 );
  }

  for ( int i = 1; i <= classes; i++ )
  {
    matrixOne[0][i] = 1;
    matrixOne[1][i] = 1;
    matrixTwo[0][i] = 0.0;
    for ( int j = 2; j <= n; j++ )
    {
      matrixTwo[j][i] = std::numeric_limits<double>::max();
    }
  }

  for ( int l = 2; l <= n; l++ )
  {
    double s1 = 0.0;
    double s2 = 0.0;
    int w = 0;

    double v = 0.0;

    for ( int m = 1; m <= l; m++ )
    {
      int i3 = l - m + 1;

      double val = sample[ i3 - 1 ];

      s2 += val * val;
      s1 += val;
      w++;

      v = s2 - ( s1 * s1 ) / static_cast< double >( w );
      int i4 = i3 - 1;
      if ( i4 != 0 )
      {
        for ( int j = 2; j <= classes; j++ )
        {
          if ( matrixTwo[l][j] >= v + matrixTwo[i4][j - 1] )
          {
            matrixOne[l][j] = i4;
            matrixTwo[l][j] = v + matrixTwo[i4][j - 1];
          }
        }
      }
    }
    matrixOne[l][1] = 1;
    matrixTwo[l][1] = v;
  }

  QVector<double> breaks( classes );
  breaks[classes-1] = sample[n-1];

  for ( int j = classes, k = n; j >= 2; j-- )
  {
    int id = matrixOne[k][j] - 1;
    breaks[j - 2] = sample[id];
    k = matrixOne[k][j] - 1;
  }

  return breaks.toList();
} //_calcJenksBreaks


QgsGraduatedSymbolRendererV2* QgsGraduatedSymbolRendererV2::createRenderer(
  QgsVectorLayer* vlayer,
  const QString& attrName,
  int classes,
  Mode mode,
  QgsSymbolV2* symbol,
  QgsVectorColorRampV2* ramp,
  bool inverted,
  const QgsRendererRangeV2LabelFormat& labelFormat
)
{
  QgsRangeList ranges;
  QgsGraduatedSymbolRendererV2* r = new QgsGraduatedSymbolRendererV2( attrName, ranges );
  r->setSourceSymbol( symbol->clone() );
  r->setSourceColorRamp( ramp->clone() );
  r->setInvertedColorRamp( inverted );
  r->setMode( mode );
  r->setLabelFormat( labelFormat );
  r->updateClasses( vlayer, mode, classes );
  return r;
}

QList<double> QgsGraduatedSymbolRendererV2::getDataValues( QgsVectorLayer *vlayer )
{
  bool ok;
  return vlayer->getDoubleValues( mAttrName, ok );
}

void QgsGraduatedSymbolRendererV2::updateClasses( QgsVectorLayer *vlayer, Mode mode, int nclasses )
{
  if ( mAttrName.isEmpty() )
    return;

  setMode( mode );
  // Custom classes are not recalculated
  if ( mode == Custom )
    return;

  if ( nclasses < 1 )
    nclasses = 1;

  QList<double> values;
  bool valuesLoaded = false;
  double minimum;
  double maximum;

  int attrNum = vlayer->fieldNameIndex( mAttrName );

  bool ok;
  if ( attrNum == -1 )
  {
    values = vlayer->getDoubleValues( mAttrName, ok );
    if ( !ok || values.isEmpty() )
      return;

    qSort( values ); // vmora: is wondering if O( n log(n) ) is really necessary here, min and max are O( n )
    minimum = values.first();
    maximum = values.last();
    valuesLoaded = true;
  }
  else
  {
    minimum = vlayer->minimumValue( attrNum ).toDouble();
    maximum = vlayer->maximumValue( attrNum ).toDouble();
  }

  QgsDebugMsg( QString( "min %1 // max %2" ).arg( minimum ).arg( maximum ) );
  QList<double> breaks;
  QList<double> labels;
  if ( mode == EqualInterval )
  {
    breaks = _calcEqualIntervalBreaks( minimum, maximum, nclasses );
  }
  else if ( mode == Pretty )
  {
    breaks = QgsSymbolLayerV2Utils::prettyBreaks( minimum, maximum, nclasses );
  }
  else if ( mode == Quantile || mode == Jenks || mode == StdDev )
  {
    // get values from layer
    if ( !valuesLoaded )
    {
      values = vlayer->getDoubleValues( mAttrName, ok );
    }

    // calculate the breaks
    if ( mode == Quantile )
    {
      breaks = _calcQuantileBreaks( values, nclasses );
    }
    else if ( mode == Jenks )
    {
      breaks = _calcJenksBreaks( values, nclasses, minimum, maximum );
    }
    else if ( mode == StdDev )
    {
      breaks = _calcStdDevBreaks( values, nclasses, labels );
    }
  }
  else
  {
    Q_ASSERT( false );
  }

  double lower, upper = minimum;
  QString label;
  deleteAllClasses();

  // "breaks" list contains all values at class breaks plus maximum as last break

  int i = 0;
  for ( QList<double>::iterator it = breaks.begin(); it != breaks.end(); ++it, ++i )
  {
    lower = upper; // upper border from last interval
    upper = *it;

    // Label - either StdDev label or default label for a range
    if ( mode == StdDev )
    {
      if ( i == 0 )
      {
        label = "< " + QString::number( labels[i], 'f', 2 ) + " Std Dev";
      }
      else if ( i == labels.count() - 1 )
      {
        label = ">= " + QString::number( labels[i-1], 'f', 2 ) + " Std Dev";
      }
      else
      {
        label = QString::number( labels[i-1], 'f', 2 ) + " Std Dev" + " - " + QString::number( labels[i], 'f', 2 ) + " Std Dev";
      }
    }
    else
    {
      label = mLabelFormat.labelForRange( lower, upper );
    }
    QgsSymbolV2* newSymbol = mSourceSymbol ? mSourceSymbol->clone() : QgsSymbolV2::defaultSymbol( vlayer->geometryType() );
    addClass( QgsRendererRangeV2( lower, upper, newSymbol, label ) );
  }
  updateColorRamp( nullptr, mInvertedColorRamp );
}

QgsFeatureRendererV2* QgsGraduatedSymbolRendererV2::create( QDomElement& element )
{
  QDomElement symbolsElem = element.firstChildElement( "symbols" );
  if ( symbolsElem.isNull() )
    return nullptr;

  QDomElement rangesElem = element.firstChildElement( "ranges" );
  if ( rangesElem.isNull() )
    return nullptr;

  QgsSymbolV2Map symbolMap = QgsSymbolLayerV2Utils::loadSymbols( symbolsElem );
  QgsRangeList ranges;

  QDomElement rangeElem = rangesElem.firstChildElement();
  while ( !rangeElem.isNull() )
  {
    if ( rangeElem.tagName() == "range" )
    {
      double lowerValue = rangeElem.attribute( "lower" ).toDouble();
      double upperValue = rangeElem.attribute( "upper" ).toDouble();
      QString symbolName = rangeElem.attribute( "symbol" );
      QString label = rangeElem.attribute( "label" );
      bool render = rangeElem.attribute( "render", "true" ) != "false";
      if ( symbolMap.contains( symbolName ) )
      {
        QgsSymbolV2* symbol = symbolMap.take( symbolName );
        ranges.append( QgsRendererRangeV2( lowerValue, upperValue, symbol, label, render ) );
      }
    }
    rangeElem = rangeElem.nextSiblingElement();
  }

  QString attrName = element.attribute( "attr" );

  QgsGraduatedSymbolRendererV2* r = new QgsGraduatedSymbolRendererV2( attrName, ranges );

  QString attrMethod = element.attribute( "graduatedMethod" );
  if ( !attrMethod.isEmpty() )
  {
    if ( attrMethod == graduatedMethodStr( GraduatedColor ) )
      r->setGraduatedMethod( GraduatedColor );
    else if ( attrMethod == graduatedMethodStr( GraduatedSize ) )
      r->setGraduatedMethod( GraduatedSize );
  }


  // delete symbols if there are any more
  QgsSymbolLayerV2Utils::clearSymbolMap( symbolMap );

  // try to load source symbol (optional)
  QDomElement sourceSymbolElem = element.firstChildElement( "source-symbol" );
  if ( !sourceSymbolElem.isNull() )
  {
    QgsSymbolV2Map sourceSymbolMap = QgsSymbolLayerV2Utils::loadSymbols( sourceSymbolElem );
    if ( sourceSymbolMap.contains( "0" ) )
    {
      r->setSourceSymbol( sourceSymbolMap.take( "0" ) );
    }
    QgsSymbolLayerV2Utils::clearSymbolMap( sourceSymbolMap );
  }

  // try to load color ramp (optional)
  QDomElement sourceColorRampElem = element.firstChildElement( "colorramp" );
  if ( !sourceColorRampElem.isNull() && sourceColorRampElem.attribute( "name" ) == "[source]" )
  {
    r->setSourceColorRamp( QgsSymbolLayerV2Utils::loadColorRamp( sourceColorRampElem ) );
    QDomElement invertedColorRampElem = element.firstChildElement( "invertedcolorramp" );
    if ( !invertedColorRampElem.isNull() )
      r->setInvertedColorRamp( invertedColorRampElem.attribute( "value" ) == "1" );
  }

  // try to load mode
  QDomElement modeElem = element.firstChildElement( "mode" );
  if ( !modeElem.isNull() )
  {
    QString modeString = modeElem.attribute( "name" );
    if ( modeString == "equal" )
      r->setMode( EqualInterval );
    else if ( modeString == "quantile" )
      r->setMode( Quantile );
    else if ( modeString == "jenks" )
      r->setMode( Jenks );
    else if ( modeString == "stddev" )
      r->setMode( StdDev );
    else if ( modeString == "pretty" )
      r->setMode( Pretty );
  }

  QDomElement rotationElem = element.firstChildElement( "rotation" );
  if ( !rotationElem.isNull() && !rotationElem.attribute( "field" ).isEmpty() )
  {
    Q_FOREACH ( const QgsRendererRangeV2& range, r->mRanges )
    {
      convertSymbolRotation( range.symbol(), rotationElem.attribute( "field" ) );
    }
    if ( r->mSourceSymbol.data() )
    {
      convertSymbolRotation( r->mSourceSymbol.data(), rotationElem.attribute( "field" ) );
    }
  }

  QDomElement sizeScaleElem = element.firstChildElement( "sizescale" );
  if ( !sizeScaleElem.isNull() && !sizeScaleElem.attribute( "field" ).isEmpty() )
  {
    Q_FOREACH ( const QgsRendererRangeV2& range, r->mRanges )
    {
      convertSymbolSizeScale( range.symbol(),
                              QgsSymbolLayerV2Utils::decodeScaleMethod( sizeScaleElem.attribute( "scalemethod" ) ),
                              sizeScaleElem.attribute( "field" ) );
    }
    if ( r->mSourceSymbol.data() && r->mSourceSymbol->type() == QgsSymbolV2::Marker )
    {
      convertSymbolSizeScale( r->mSourceSymbol.data(),
                              QgsSymbolLayerV2Utils::decodeScaleMethod( sizeScaleElem.attribute( "scalemethod" ) ),
                              sizeScaleElem.attribute( "field" ) );
    }
  }

  QDomElement labelFormatElem = element.firstChildElement( "labelformat" );
  if ( ! labelFormatElem.isNull() )
  {
    QgsRendererRangeV2LabelFormat labelFormat;
    labelFormat.setFromDomElement( labelFormatElem );
    r->setLabelFormat( labelFormat );
  }
  // TODO: symbol levels
  return r;
}

QDomElement QgsGraduatedSymbolRendererV2::save( QDomDocument& doc )
{
  QDomElement rendererElem = doc.createElement( RENDERER_TAG_NAME );
  rendererElem.setAttribute( "type", "graduatedSymbol" );
  rendererElem.setAttribute( "symbollevels", ( mUsingSymbolLevels ? "1" : "0" ) );
  rendererElem.setAttribute( "forceraster", ( mForceRaster ? "1" : "0" ) );
  rendererElem.setAttribute( "attr", mAttrName );
  rendererElem.setAttribute( "graduatedMethod", graduatedMethodStr( mGraduatedMethod ) );

  // ranges
  int i = 0;
  QgsSymbolV2Map symbols;
  QDomElement rangesElem = doc.createElement( "ranges" );
  QgsRangeList::const_iterator it = mRanges.constBegin();
  for ( ; it != mRanges.constEnd(); ++it )
  {
    const QgsRendererRangeV2& range = *it;
    QString symbolName = QString::number( i );
    symbols.insert( symbolName, range.symbol() );

    QDomElement rangeElem = doc.createElement( "range" );
    rangeElem.setAttribute( "lower", QString::number( range.lowerValue(), 'f', 15 ) );
    rangeElem.setAttribute( "upper", QString::number( range.upperValue(), 'f', 15 ) );
    rangeElem.setAttribute( "symbol", symbolName );
    rangeElem.setAttribute( "label", range.label() );
    rangeElem.setAttribute( "render", range.renderState() ? "true" : "false" );
    rangesElem.appendChild( rangeElem );
    i++;
  }

  rendererElem.appendChild( rangesElem );

  // save symbols
  QDomElement symbolsElem = QgsSymbolLayerV2Utils::saveSymbols( symbols, "symbols", doc );
  rendererElem.appendChild( symbolsElem );

  // save source symbol
  if ( mSourceSymbol.data() )
  {
    QgsSymbolV2Map sourceSymbols;
    sourceSymbols.insert( "0", mSourceSymbol.data() );
    QDomElement sourceSymbolElem = QgsSymbolLayerV2Utils::saveSymbols( sourceSymbols, "source-symbol", doc );
    rendererElem.appendChild( sourceSymbolElem );
  }

  // save source color ramp
  if ( mSourceColorRamp.data() )
  {
    QDomElement colorRampElem = QgsSymbolLayerV2Utils::saveColorRamp( "[source]", mSourceColorRamp.data(), doc );
    rendererElem.appendChild( colorRampElem );
    QDomElement invertedElem = doc.createElement( "invertedcolorramp" );
    invertedElem.setAttribute( "value", mInvertedColorRamp );
    rendererElem.appendChild( invertedElem );
  }

  // save mode
  QString modeString;
  if ( mMode == EqualInterval )
    modeString = "equal";
  else if ( mMode == Quantile )
    modeString = "quantile";
  else if ( mMode == Jenks )
    modeString = "jenks";
  else if ( mMode == StdDev )
    modeString = "stddev";
  else if ( mMode == Pretty )
    modeString = "pretty";
  if ( !modeString.isEmpty() )
  {
    QDomElement modeElem = doc.createElement( "mode" );
    modeElem.setAttribute( "name", modeString );
    rendererElem.appendChild( modeElem );
  }

  QDomElement rotationElem = doc.createElement( "rotation" );
  if ( mRotation.data() )
    rotationElem.setAttribute( "field", QgsSymbolLayerV2Utils::fieldOrExpressionFromExpression( mRotation.data() ) );
  rendererElem.appendChild( rotationElem );

  QDomElement sizeScaleElem = doc.createElement( "sizescale" );
  if ( mSizeScale.data() )
    sizeScaleElem.setAttribute( "field", QgsSymbolLayerV2Utils::fieldOrExpressionFromExpression( mSizeScale.data() ) );
  sizeScaleElem.setAttribute( "scalemethod", QgsSymbolLayerV2Utils::encodeScaleMethod( mScaleMethod ) );
  rendererElem.appendChild( sizeScaleElem );

  QDomElement labelFormatElem = doc.createElement( "labelformat" );
  mLabelFormat.saveToDomElement( labelFormatElem );
  rendererElem.appendChild( labelFormatElem );

  if ( mPaintEffect && !QgsPaintEffectRegistry::isDefaultStack( mPaintEffect ) )
    mPaintEffect->saveProperties( doc, rendererElem );

  if ( !mOrderBy.isEmpty() )
  {
    QDomElement orderBy = doc.createElement( "orderby" );
    mOrderBy.save( orderBy );
    rendererElem.appendChild( orderBy );
  }
  rendererElem.setAttribute( "enableorderby", ( mOrderByEnabled ? "1" : "0" ) );

  return rendererElem;
}

QgsLegendSymbologyList QgsGraduatedSymbolRendererV2::legendSymbologyItems( QSize iconSize )
{
  QgsLegendSymbologyList lst;
  int count = ranges().count();
  lst.reserve( count );
  for ( int i = 0; i < count; i++ )
  {
    const QgsRendererRangeV2& range = ranges()[i];
    QPixmap pix = QgsSymbolLayerV2Utils::symbolPreviewPixmap( range.symbol(), iconSize );
    lst << qMakePair( range.label(), pix );
  }
  return lst;
}

QgsLegendSymbolListV2 QgsGraduatedSymbolRendererV2::legendSymbolItemsV2() const
{
  QgsLegendSymbolListV2 list;
  if ( mSourceSymbol.data() && mSourceSymbol->type() == QgsSymbolV2::Marker )
  {
    // check that all symbols that have the same size expression
    QgsDataDefined ddSize;
    Q_FOREACH ( const QgsRendererRangeV2& range, mRanges )
    {
      const QgsMarkerSymbolV2 * symbol = static_cast<const QgsMarkerSymbolV2 *>( range.symbol() );
      if ( !ddSize.hasDefaultValues() && symbol->dataDefinedSize() != ddSize )
      {
        // no common size expression
        return QgsFeatureRendererV2::legendSymbolItemsV2();
      }
      else
      {
        ddSize = symbol->dataDefinedSize();
      }
    }

    if ( !ddSize.isActive() || !ddSize.useExpression() )
    {
      return QgsFeatureRendererV2::legendSymbolItemsV2();
    }

    QgsScaleExpression exp( ddSize.expressionString() );
    if ( exp.type() != QgsScaleExpression::Unknown )
    {
      QgsLegendSymbolItemV2 title( nullptr, exp.baseExpression(), "" );
      list << title;
      Q_FOREACH ( double v, QgsSymbolLayerV2Utils::prettyBreaks( exp.minValue(), exp.maxValue(), 4 ) )
      {
        QgsLegendSymbolItemV2 si( mSourceSymbol.data(), QString::number( v ), "" );
        QgsMarkerSymbolV2 * s = static_cast<QgsMarkerSymbolV2 *>( si.symbol() );
        s->setDataDefinedSize( QgsDataDefined() );
        s->setSize( exp.size( v ) );
        list << si;
      }
      // now list the graduated symbols
      const QgsLegendSymbolListV2 list2 = QgsFeatureRendererV2::legendSymbolItemsV2() ;
      Q_FOREACH ( const QgsLegendSymbolItemV2& item, list2 )
        list << item;
      return list;
    }
  }

  return QgsFeatureRendererV2::legendSymbolItemsV2();
}

QSet< QString > QgsGraduatedSymbolRendererV2::legendKeysForFeature( QgsFeature& feature, QgsRenderContext& context )
{
  QVariant value = valueForFeature( feature, context );

  // Null values should not be categorized
  if ( value.isNull() )
    return QSet< QString >();

  // find the right category
  QString key = legendKeyForValue( value.toDouble() );
  if ( !key.isNull() )
    return QSet< QString >() << key;
  else
    return QSet< QString >();
}

QgsLegendSymbolList QgsGraduatedSymbolRendererV2::legendSymbolItems( double scaleDenominator, const QString& rule )
{
  Q_UNUSED( scaleDenominator );
  QgsLegendSymbolList lst;

  Q_FOREACH ( const QgsRendererRangeV2& range, mRanges )
  {
    if ( rule.isEmpty() || range.label() == rule )
    {
      lst << qMakePair( range.label(), range.symbol() );
    }
  }
  return lst;
}

QgsSymbolV2* QgsGraduatedSymbolRendererV2::sourceSymbol()
{
  return mSourceSymbol.data();
}
void QgsGraduatedSymbolRendererV2::setSourceSymbol( QgsSymbolV2* sym )
{
  mSourceSymbol.reset( sym );
}

QgsVectorColorRampV2* QgsGraduatedSymbolRendererV2::sourceColorRamp()
{
  return mSourceColorRamp.data();
}

void QgsGraduatedSymbolRendererV2::setSourceColorRamp( QgsVectorColorRampV2* ramp )
{
  mSourceColorRamp.reset( ramp );
}

double QgsGraduatedSymbolRendererV2::minSymbolSize() const
{
  double min = DBL_MAX;
  for ( int i = 0; i < mRanges.count(); i++ )
  {
    double sz = 0;
    if ( mRanges[i].symbol()->type() == QgsSymbolV2::Marker )
      sz = static_cast< QgsMarkerSymbolV2 * >( mRanges[i].symbol() )->size();
    else if ( mRanges[i].symbol()->type() == QgsSymbolV2::Line )
      sz = static_cast< QgsLineSymbolV2 * >( mRanges[i].symbol() )->width();
    min = qMin( sz, min );
  }
  return min;
}

double QgsGraduatedSymbolRendererV2::maxSymbolSize() const
{
  double max = DBL_MIN;
  for ( int i = 0; i < mRanges.count(); i++ )
  {
    double sz = 0;
    if ( mRanges[i].symbol()->type() == QgsSymbolV2::Marker )
      sz = static_cast< QgsMarkerSymbolV2 * >( mRanges[i].symbol() )->size();
    else if ( mRanges[i].symbol()->type() == QgsSymbolV2::Line )
      sz = static_cast< QgsLineSymbolV2 * >( mRanges[i].symbol() )->width();
    max = qMax( sz, max );
  }
  return max;
}

void QgsGraduatedSymbolRendererV2::setSymbolSizes( double minSize, double maxSize )
{
  for ( int i = 0; i < mRanges.count(); i++ )
  {
    QScopedPointer<QgsSymbolV2> symbol( mRanges.at( i ).symbol() ? mRanges.at( i ).symbol()->clone() : nullptr );
    const double size =  mRanges.count() > 1
                         ? minSize + i * ( maxSize - minSize ) / ( mRanges.count() - 1 )
                         : .5 * ( maxSize + minSize );
    if ( symbol->type() == QgsSymbolV2::Marker )
      static_cast< QgsMarkerSymbolV2 * >( symbol.data() )->setSize( size );
    if ( symbol->type() == QgsSymbolV2::Line )
      static_cast< QgsLineSymbolV2 * >( symbol.data() )->setWidth( size );
    updateRangeSymbol( i, symbol.take() );
  }
}

void QgsGraduatedSymbolRendererV2::updateColorRamp( QgsVectorColorRampV2 *ramp, bool inverted )
{
  int i = 0;
  if ( ramp )
  {
    setSourceColorRamp( ramp );
    setInvertedColorRamp( inverted );
  }

  if ( mSourceColorRamp )
  {
    Q_FOREACH ( const QgsRendererRangeV2& range, mRanges )
    {
      QgsSymbolV2 *symbol = range.symbol() ? range.symbol()->clone() : nullptr;
      if ( symbol )
      {
        double colorValue;
        if ( inverted )
          colorValue = ( mRanges.count() > 1 ? static_cast< double >( mRanges.count() - i - 1 ) / ( mRanges.count() - 1 ) : 0 );
        else
          colorValue = ( mRanges.count() > 1 ? static_cast< double >( i ) / ( mRanges.count() - 1 ) : 0 );
        symbol->setColor( mSourceColorRamp->color( colorValue ) );
      }
      updateRangeSymbol( i, symbol );
      ++i;
    }
  }

}

void QgsGraduatedSymbolRendererV2::updateSymbols( QgsSymbolV2 *sym )
{
  if ( !sym )
    return;

  int i = 0;
  Q_FOREACH ( const QgsRendererRangeV2& range, mRanges )
  {
    QScopedPointer<QgsSymbolV2> symbol( sym->clone() );
    if ( mGraduatedMethod == GraduatedColor )
    {
      symbol->setColor( range.symbol()->color() );
    }
    else if ( mGraduatedMethod == GraduatedSize )
    {
      if ( symbol->type() == QgsSymbolV2::Marker )
        static_cast<QgsMarkerSymbolV2 *>( symbol.data() )->setSize(
          static_cast<QgsMarkerSymbolV2 *>( range.symbol() )->size() );
      else if ( symbol->type() == QgsSymbolV2::Line )
        static_cast<QgsLineSymbolV2 *>( symbol.data() )->setWidth(
          static_cast<QgsLineSymbolV2 *>( range.symbol() )->width() );
    }
    updateRangeSymbol( i, symbol.take() );
    ++i;
  }
  setSourceSymbol( sym->clone() );
}

void QgsGraduatedSymbolRendererV2::setRotationField( const QString& fieldOrExpression )
{
  if ( mSourceSymbol->type() == QgsSymbolV2::Marker )
  {
    QgsMarkerSymbolV2 * s = static_cast<QgsMarkerSymbolV2 *>( mSourceSymbol.data() );
    s->setDataDefinedAngle( QgsDataDefined( fieldOrExpression ) );
  }

}

QString QgsGraduatedSymbolRendererV2::rotationField() const
{
  if ( mSourceSymbol->type() == QgsSymbolV2::Marker )
  {
    QgsMarkerSymbolV2 * s = static_cast<QgsMarkerSymbolV2 *>( mSourceSymbol.data() );
    QgsDataDefined ddAngle = s->dataDefinedAngle();
    return ddAngle.useExpression() ? ddAngle.expressionString() : ddAngle.field();
  }

  return QString();
}

void QgsGraduatedSymbolRendererV2::setSizeScaleField( const QString& fieldOrExpression )
{
  mSizeScale.reset( QgsSymbolLayerV2Utils::fieldOrExpressionToExpression( fieldOrExpression ) );
}

QString QgsGraduatedSymbolRendererV2::sizeScaleField() const
{
  return mSizeScale.data() ? QgsSymbolLayerV2Utils::fieldOrExpressionFromExpression( mSizeScale.data() ) : QString();
}

void QgsGraduatedSymbolRendererV2::setScaleMethod( QgsSymbolV2::ScaleMethod scaleMethod )
{
  mScaleMethod = scaleMethod;
  Q_FOREACH ( const QgsRendererRangeV2& range, mRanges )
  {
    if ( range.symbol() )
      setScaleMethodToSymbol( range.symbol(), scaleMethod );
  }
}

bool QgsGraduatedSymbolRendererV2::legendSymbolItemsCheckable() const
{
  return true;
}

bool QgsGraduatedSymbolRendererV2::legendSymbolItemChecked( const QString& key )
{
  bool ok;
  int index = key.toInt( &ok );
  if ( ok && index >= 0 && index < mRanges.size() )
    return mRanges.at( index ).renderState();
  else
    return true;
}

void QgsGraduatedSymbolRendererV2::checkLegendSymbolItem( const QString& key, bool state )
{
  bool ok;
  int index = key.toInt( &ok );
  if ( ok )
    updateRangeRenderState( index, state );
}

void QgsGraduatedSymbolRendererV2::setLegendSymbolItem( const QString& key, QgsSymbolV2* symbol )
{
  bool ok;
  int index = key.toInt( &ok );
  if ( ok )
    updateRangeSymbol( index, symbol );
  else
    delete symbol;
}

void QgsGraduatedSymbolRendererV2::addClass( QgsSymbolV2* symbol )
{
  QgsSymbolV2* newSymbol = symbol->clone();
  QString label = "0.0 - 0.0";
  mRanges.insert( 0, QgsRendererRangeV2( 0.0, 0.0, newSymbol, label ) );
}

void QgsGraduatedSymbolRendererV2::addClass( double lower, double upper )
{
  QgsSymbolV2* newSymbol = mSourceSymbol->clone();
  QString label = mLabelFormat.labelForRange( lower, upper );
  mRanges.append( QgsRendererRangeV2( lower, upper, newSymbol, label ) );
}

void QgsGraduatedSymbolRendererV2::addBreak( double breakValue, bool updateSymbols )
{
  QMutableListIterator< QgsRendererRangeV2 > it( mRanges );
  while ( it.hasNext() )
  {
    QgsRendererRangeV2 range = it.next();
    if ( range.lowerValue() < breakValue && range.upperValue() > breakValue )
    {
      QgsRendererRangeV2 newRange = QgsRendererRangeV2();
      newRange.setLowerValue( breakValue );
      newRange.setUpperValue( range.upperValue() );
      newRange.setLabel( mLabelFormat.labelForRange( newRange ) );
      newRange.setSymbol( mSourceSymbol->clone() );

      //update old range
      bool isDefaultLabel = range.label() == mLabelFormat.labelForRange( range );
      range.setUpperValue( breakValue );
      if ( isDefaultLabel ) range.setLabel( mLabelFormat.labelForRange( range.lowerValue(), breakValue ) );
      it.setValue( range );

      it.insert( newRange );
      break;
    }
  }

  if ( updateSymbols )
  {
    switch ( mGraduatedMethod )
    {
      case GraduatedColor:
        updateColorRamp( mSourceColorRamp.data(), mInvertedColorRamp );
        break;
      case GraduatedSize:
        setSymbolSizes( minSymbolSize(), maxSymbolSize() );
        break;
    }
  }
}

void QgsGraduatedSymbolRendererV2::addClass( const QgsRendererRangeV2& range )
{
  mRanges.append( range );
}

void QgsGraduatedSymbolRendererV2::deleteClass( int idx )
{
  mRanges.removeAt( idx );
}

void QgsGraduatedSymbolRendererV2::deleteAllClasses()
{
  mRanges.clear();
}

void QgsGraduatedSymbolRendererV2::setLabelFormat( const QgsRendererRangeV2LabelFormat &labelFormat, bool updateRanges )
{
  if ( updateRanges && labelFormat != mLabelFormat )
  {
    for ( QgsRangeList::iterator it = mRanges.begin(); it != mRanges.end(); ++it )
    {
      it->setLabel( labelFormat.labelForRange( *it ) );
    }
  }
  mLabelFormat = labelFormat;
}


void QgsGraduatedSymbolRendererV2::calculateLabelPrecision( bool updateRanges )
{
  // Find the minimum size of a class
  double minClassRange = 0.0;
  Q_FOREACH ( const QgsRendererRangeV2& rendererRange, mRanges )
  {
    double range = rendererRange.upperValue() - rendererRange.lowerValue();
    if ( range <= 0.0 )
      continue;
    if ( minClassRange == 0.0 || range < minClassRange )
      minClassRange = range;
  }
  if ( minClassRange <= 0.0 )
    return;

  // Now set the number of decimal places to ensure no more than 20% error in
  // representing this range (up to 10% at upper and lower end)

  int ndp = 10;
  double nextDpMinRange = 0.0000000099;
  while ( ndp > 0 && nextDpMinRange < minClassRange )
  {
    ndp--;
    nextDpMinRange *= 10.0;
  }
  mLabelFormat.setPrecision( ndp );
  if ( updateRanges ) setLabelFormat( mLabelFormat, true );
}

void QgsGraduatedSymbolRendererV2::moveClass( int from, int to )
{
  if ( from < 0 || from >= mRanges.size() || to < 0 || to >= mRanges.size() )
    return;
  mRanges.move( from, to );
}

bool valueLessThan( const QgsRendererRangeV2 &r1, const QgsRendererRangeV2 &r2 )
{
  return r1 < r2;
}

bool valueGreaterThan( const QgsRendererRangeV2 &r1, const QgsRendererRangeV2 &r2 )
{
  return !valueLessThan( r1, r2 );
}

void QgsGraduatedSymbolRendererV2::sortByValue( Qt::SortOrder order )
{
  if ( order == Qt::AscendingOrder )
  {
    qSort( mRanges.begin(), mRanges.end(), valueLessThan );
  }
  else
  {
    qSort( mRanges.begin(), mRanges.end(), valueGreaterThan );
  }
}

bool QgsGraduatedSymbolRendererV2::rangesOverlap() const
{
  QgsRangeList sortedRanges = mRanges;
  qSort( sortedRanges.begin(), sortedRanges.end(), valueLessThan );

  QgsRangeList::const_iterator it = sortedRanges.constBegin();
  if ( it == sortedRanges.constEnd() )
    return false;

  if (( *it ).upperValue() < ( *it ).lowerValue() )
    return true;

  double prevMax = ( *it ).upperValue();
  ++it;

  for ( ; it != sortedRanges.constEnd(); ++it )
  {
    if (( *it ).upperValue() < ( *it ).lowerValue() )
      return true;

    if (( *it ).lowerValue() < prevMax )
      return true;

    prevMax = ( *it ).upperValue();
  }
  return false;
}

bool QgsGraduatedSymbolRendererV2::rangesHaveGaps() const
{
  QgsRangeList sortedRanges = mRanges;
  qSort( sortedRanges.begin(), sortedRanges.end(), valueLessThan );

  QgsRangeList::const_iterator it = sortedRanges.constBegin();
  if ( it == sortedRanges.constEnd() )
    return false;

  double prevMax = ( *it ).upperValue();
  ++it;

  for ( ; it != sortedRanges.constEnd(); ++it )
  {
    if ( !qgsDoubleNear(( *it ).lowerValue(), prevMax ) )
      return true;

    prevMax = ( *it ).upperValue();
  }
  return false;
}

bool labelLessThan( const QgsRendererRangeV2 &r1, const QgsRendererRangeV2 &r2 )
{
  return QString::localeAwareCompare( r1.label(), r2.label() ) < 0;
}

bool labelGreaterThan( const QgsRendererRangeV2 &r1, const QgsRendererRangeV2 &r2 )
{
  return !labelLessThan( r1, r2 );
}

void QgsGraduatedSymbolRendererV2::sortByLabel( Qt::SortOrder order )
{
  if ( order == Qt::AscendingOrder )
  {
    qSort( mRanges.begin(), mRanges.end(), labelLessThan );
  }
  else
  {
    qSort( mRanges.begin(), mRanges.end(), labelGreaterThan );
  }
}

QgsGraduatedSymbolRendererV2* QgsGraduatedSymbolRendererV2::convertFromRenderer( const QgsFeatureRendererV2 *renderer )
{
  QgsGraduatedSymbolRendererV2* r = nullptr;
  if ( renderer->type() == "graduatedSymbol" )
  {
    r = dynamic_cast<QgsGraduatedSymbolRendererV2*>( renderer->clone() );
  }
  else if ( renderer->type() == "pointDisplacement" )
  {
    const QgsPointDisplacementRenderer* pointDisplacementRenderer = dynamic_cast<const QgsPointDisplacementRenderer*>( renderer );
    if ( pointDisplacementRenderer )
      r = convertFromRenderer( pointDisplacementRenderer->embeddedRenderer() );
  }
  else if ( renderer->type() == "invertedPolygonRenderer" )
  {
    const QgsInvertedPolygonRenderer* invertedPolygonRenderer = dynamic_cast<const QgsInvertedPolygonRenderer*>( renderer );
    if ( invertedPolygonRenderer )
      r = convertFromRenderer( invertedPolygonRenderer->embeddedRenderer() );
  }

  // If not one of the specifically handled renderers, then just grab the symbol from the renderer
  // Could have applied this to specific renderer types (singleSymbol, graduatedSymbo)

  if ( !r )
  {
    r = new QgsGraduatedSymbolRendererV2( "", QgsRangeList() );
    QgsRenderContext context;
    QgsSymbolV2List symbols = const_cast<QgsFeatureRendererV2 *>( renderer )->symbols( context );
    if ( !symbols.isEmpty() )
    {
      r->setSourceSymbol( symbols.at( 0 )->clone() );
    }
  }

  r->setOrderBy( renderer->orderBy() );
  r->setOrderByEnabled( renderer->orderByEnabled() );

  return r;
}

const char * QgsGraduatedSymbolRendererV2::graduatedMethodStr( GraduatedMethod method )
{
  switch ( method )
  {
    case GraduatedColor:
      return "GraduatedColor";
    case GraduatedSize:
      return "GraduatedSize";
  }
  return "";
}


