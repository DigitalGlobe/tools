/***************************************************************************
                         testqgscomposertablev2.cpp
                         ----------------------
    begin                : September 2014
    copyright            : (C) 2014 by Nyall Dawson
    email                : nyall dot dawson at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsapplication.h"
#include "qgscomposition.h"
#include "qgscomposermap.h"
#include "qgscomposerattributetablev2.h"
#include "qgscomposertablecolumn.h"
#include "qgscomposerframe.h"
#include "qgsmapsettings.h"
#include "qgsvectorlayer.h"
#include "qgsvectordataprovider.h"
#include "qgsfeature.h"
#include "qgsmultirenderchecker.h"
#include "qgsfontutils.h"
#include "qgsmaplayerregistry.h"
#include "qgsproject.h"
#include "qgsrelationmanager.h"

#include <QObject>
#include <QtTest/QtTest>

class TestQgsComposerTableV2 : public QObject
{
    Q_OBJECT

  public:
    TestQgsComposerTableV2()
        : mComposition( 0 )
        , mMapSettings( 0 )
        , mVectorLayer( 0 )
        , mComposerAttributeTable( 0 )
        , mFrame1( 0 )
        , mFrame2( 0 )
    {}

  private slots:
    void initTestCase();// will be called before the first testfunction is executed.
    void cleanupTestCase();// will be called after the last testfunction was executed.
    void init();// will be called before each testfunction is executed.
    void cleanup();// will be called after every testfunction.

    void attributeTableHeadings(); //test retrieving attribute table headers
    void attributeTableRows(); //test retrieving attribute table rows
    void attributeTableFilterFeatures(); //test filtering attribute table rows
    void attributeTableSetAttributes(); //test subset of attributes in table
    void attributeTableVisibleOnly(); //test displaying only visible attributes
    void attributeTableRender(); //test rendering attribute table
    void manualColumnWidth(); //test setting manual column widths
    void attributeTableEmpty(); //test empty modes for attribute table
    void showEmptyRows(); //test showing empty rows
    void attributeTableExtend();
    void attributeTableRepeat();
    void attributeTableAtlasSource(); //test attribute table in atlas feature mode
    void attributeTableRelationSource(); //test attribute table in relation mode
    void contentsContainsRow(); //test the contentsContainsRow function
    void removeDuplicates(); //test removing duplicate rows
    void multiLineText(); //test rendering a table with multiline text
    void align(); //test alignment of table cells
    void wrapChar(); //test setting wrap character
    void autoWrap(); //test auto word wrap
    void cellStyles(); //test cell styles
    void cellStylesRender(); //test rendering cell styles

  private:
    QgsComposition* mComposition;
    QgsMapSettings *mMapSettings;
    QgsVectorLayer* mVectorLayer;
    QgsComposerAttributeTableV2* mComposerAttributeTable;
    QgsComposerFrame* mFrame1;
    QgsComposerFrame* mFrame2;
    QString mReport;

    //compares rows in mComposerAttributeTable to expected rows
    void compareTable( QList<QStringList> &expectedRows );
};

void TestQgsComposerTableV2::initTestCase()
{
  QgsApplication::init();
  QgsApplication::initQgis();

  mMapSettings = new QgsMapSettings();

  //create maplayers from testdata and add to layer registry
  QFileInfo vectorFileInfo( QString( TEST_DATA_DIR ) + "/points.shp" );
  mVectorLayer = new QgsVectorLayer( vectorFileInfo.filePath(),
                                     vectorFileInfo.completeBaseName(),
                                     "ogr" );
  QgsMapLayerRegistry::instance()->addMapLayer( mVectorLayer );

  mMapSettings->setLayers( QStringList() << mVectorLayer->id() );
  mMapSettings->setCrsTransformEnabled( false );

  mReport = "<h1>Composer TableV2 Tests</h1>\n";
}

void TestQgsComposerTableV2::cleanupTestCase()
{
  delete mMapSettings;

  QString myReportFile = QDir::tempPath() + "/qgistest.html";
  QFile myFile( myReportFile );
  if ( myFile.open( QIODevice::WriteOnly | QIODevice::Append ) )
  {
    QTextStream myQTextStream( &myFile );
    myQTextStream << mReport;
    myFile.close();
  }
  QgsApplication::exitQgis();
}

void TestQgsComposerTableV2::init()
{
  //create composition with composer map
  mComposition = new QgsComposition( *mMapSettings );
  mComposition->setPaperSize( 297, 210 ); //A4 portrait

  mComposerAttributeTable = new QgsComposerAttributeTableV2( mComposition, false );
  mComposition->addMultiFrame( mComposerAttributeTable );

  mFrame1 = new QgsComposerFrame( mComposition, mComposerAttributeTable, 5, 5, 100, 30 );
  mFrame2 = new QgsComposerFrame( mComposition, mComposerAttributeTable, 5, 40, 100, 30 );

  mFrame1->setFrameEnabled( true );
  mFrame2->setFrameEnabled( true );
  mComposerAttributeTable->addFrame( mFrame1 );
  mComposerAttributeTable->addFrame( mFrame2 );

  mComposition->addComposerTableFrame( mComposerAttributeTable, mFrame1 );
  mComposition->addComposerTableFrame( mComposerAttributeTable, mFrame2 );
  mComposerAttributeTable->setVectorLayer( mVectorLayer );
  mComposerAttributeTable->setDisplayOnlyVisibleFeatures( false );
  mComposerAttributeTable->setMaximumNumberOfFeatures( 10 );
  mComposerAttributeTable->setContentFont( QgsFontUtils::getStandardTestFont() );
  mComposerAttributeTable->setHeaderFont( QgsFontUtils::getStandardTestFont() );
  mComposerAttributeTable->setBackgroundColor( Qt::yellow );
}

void TestQgsComposerTableV2::cleanup()
{
  delete mComposition;
}

void TestQgsComposerTableV2::attributeTableHeadings()
{
  //test retrieving attribute table headers
  QStringList expectedHeaders;
  expectedHeaders << "Class" << "Heading" << "Importance" << "Pilots" << "Cabin Crew" << "Staff";

  //get header labels and compare
  QMap<int, QString> headerMap = mComposerAttributeTable->headerLabels();
  QMap<int, QString>::const_iterator headerIt = headerMap.constBegin();
  QString expected;
  QString evaluated;
  for ( ; headerIt != headerMap.constEnd(); ++headerIt )
  {
    evaluated = headerIt.value();
    expected = expectedHeaders.at( headerIt.key() );
    QCOMPARE( evaluated, expected );
  }
}

void TestQgsComposerTableV2::compareTable( QList<QStringList> &expectedRows )
{
  //retrieve rows and check
  QgsComposerTableContents tableContents;
  bool result = mComposerAttributeTable->getTableContents( tableContents );
  QCOMPARE( result, true );

  QgsComposerTableContents::const_iterator resultIt = tableContents.constBegin();
  int rowNumber = 0;
  int colNumber = 0;

  //check that number of rows matches expected
  QCOMPARE( tableContents.count(), expectedRows.count() );

  for ( ; resultIt != tableContents.constEnd(); ++resultIt )
  {
    colNumber = 0;
    QgsComposerTableRow::const_iterator cellIt = ( *resultIt ).constBegin();
    for ( ; cellIt != ( *resultIt ).constEnd(); ++cellIt )
    {
      QCOMPARE(( *cellIt ).toString(), expectedRows.at( rowNumber ).at( colNumber ) );
      colNumber++;
    }
    //also check that number of columns matches expected
    QCOMPARE(( *resultIt ).count(), expectedRows.at( rowNumber ).count() );

    rowNumber++;
  }
}

void TestQgsComposerTableV2::attributeTableRows()
{
  //test retrieving attribute table rows

  QList<QStringList> expectedRows;
  QStringList row;
  row << "Jet" << "90" << "3" << "2" << "0" << "2";
  expectedRows.append( row );
  row.clear();
  row << "Biplane" << "0" << "1" << "3" << "3" << "6";
  expectedRows.append( row );
  row.clear();
  row << "Jet" << "85" << "3" << "1" << "1" << "2";
  expectedRows.append( row );

  //retrieve rows and check
  mComposerAttributeTable->setMaximumNumberOfFeatures( 3 );
  compareTable( expectedRows );
}

void TestQgsComposerTableV2::attributeTableFilterFeatures()
{
  //test filtering attribute table rows
  mComposerAttributeTable->setMaximumNumberOfFeatures( 10 );
  mComposerAttributeTable->setFeatureFilter( QString( "\"Class\"='B52'" ) );
  mComposerAttributeTable->setFilterFeatures( true );

  QList<QStringList> expectedRows;
  QStringList row;
  row << "B52" << "0" << "10" << "2" << "1" << "3";
  expectedRows.append( row );
  row.clear();
  row << "B52" << "12" << "10" << "1" << "1" << "2";
  expectedRows.append( row );
  row.clear();
  row << "B52" << "34" << "10" << "2" << "1" << "3";
  expectedRows.append( row );
  row.clear();
  row << "B52" << "80" << "10" << "2" << "1" << "3";
  expectedRows.append( row );

  //retrieve rows and check
  compareTable( expectedRows );

  mComposerAttributeTable->setFilterFeatures( false );
}

void TestQgsComposerTableV2::attributeTableSetAttributes()
{
  //test subset of attributes in table
  QStringList attributes;
  attributes << "Class" << "Pilots" << "Cabin Crew";
  mComposerAttributeTable->setDisplayedFields( attributes );
  mComposerAttributeTable->setMaximumNumberOfFeatures( 3 );

  //check headers
  QStringList expectedHeaders;
  expectedHeaders << "Class" << "Pilots" << "Cabin Crew";

  //get header labels and compare
  QMap<int, QString> headerMap = mComposerAttributeTable->headerLabels();
  QMap<int, QString>::const_iterator headerIt = headerMap.constBegin();
  QString expected;
  QString evaluated;
  for ( ; headerIt != headerMap.constEnd(); ++headerIt )
  {
    evaluated = headerIt.value();
    expected = expectedHeaders.at( headerIt.key() );
    QCOMPARE( evaluated, expected );
  }

  QList<QStringList> expectedRows;
  QStringList row;
  row << "Jet" << "2" << "0";
  expectedRows.append( row );
  row.clear();
  row << "Biplane" << "3" << "3";
  expectedRows.append( row );
  row.clear();
  row << "Jet" << "1" << "1";
  expectedRows.append( row );

  //retrieve rows and check
  compareTable( expectedRows );

  attributes.clear();
  mComposerAttributeTable->setDisplayedFields( attributes );
}

void TestQgsComposerTableV2::attributeTableVisibleOnly()
{
  //test displaying only visible attributes

  QgsComposerMap* composerMap = new QgsComposerMap( mComposition, 20, 20, 200, 100 );
  composerMap->setFrameEnabled( true );
  composerMap->setNewExtent( QgsRectangle( -131.767, 30.558, -110.743, 41.070 ) );

  mComposerAttributeTable->setComposerMap( composerMap );
  mComposerAttributeTable->setDisplayOnlyVisibleFeatures( true );

  QList<QStringList> expectedRows;
  QStringList row;
  row << "Jet" << "90" << "3" << "2" << "0" << "2";
  expectedRows.append( row );
  row.clear();
  row << "Biplane" << "240" << "1" << "3" << "2" << "5";
  expectedRows.append( row );
  row.clear();
  row << "Jet" << "180" << "3" << "1" << "0" << "1";
  expectedRows.append( row );

  //retrieve rows and check
  compareTable( expectedRows );

  mComposerAttributeTable->setDisplayOnlyVisibleFeatures( false );
  mComposerAttributeTable->setComposerMap( 0 );
  delete composerMap;
}

void TestQgsComposerTableV2::attributeTableRender()
{
  mComposerAttributeTable->setMaximumNumberOfFeatures( 20 );
  QgsCompositionChecker checker( "composerattributetable_render", mComposition );
  checker.setControlPathPrefix( "composer_table" );
  bool result = checker.testComposition( mReport );
  QVERIFY( result );
}

void TestQgsComposerTableV2::manualColumnWidth()
{
  mComposerAttributeTable->setMaximumNumberOfFeatures( 20 );
  mComposerAttributeTable->columns()->at( 0 )->setWidth( 5 );
  QgsCompositionChecker checker( "composerattributetable_columnwidth", mComposition );
  checker.setControlPathPrefix( "composer_table" );
  bool result = checker.testComposition( mReport, 0 );
  mComposerAttributeTable->columns()->at( 0 )->setWidth( 0 );
  QVERIFY( result );
}

void TestQgsComposerTableV2::attributeTableEmpty()
{
  mComposerAttributeTable->setMaximumNumberOfFeatures( 20 );
  //hide all features from table
  mComposerAttributeTable->setFeatureFilter( QString( "1=2" ) );
  mComposerAttributeTable->setFilterFeatures( true );

  mComposerAttributeTable->setEmptyTableBehaviour( QgsComposerTableV2::HeadersOnly );
  QgsCompositionChecker checker( "composerattributetable_headersonly", mComposition );
  checker.setControlPathPrefix( "composer_table" );
  QVERIFY( checker.testComposition( mReport, 0 ) );

  mComposerAttributeTable->setEmptyTableBehaviour( QgsComposerTableV2::HideTable );
  QgsCompositionChecker checker2( "composerattributetable_hidetable", mComposition );
  checker2.setControlPathPrefix( "composer_table" );
  QVERIFY( checker2.testComposition( mReport, 0 ) );

  mComposerAttributeTable->setEmptyTableBehaviour( QgsComposerTableV2::ShowMessage );
  mComposerAttributeTable->setEmptyTableMessage( "no rows" );
  QgsCompositionChecker checker3( "composerattributetable_showmessage", mComposition );
  checker3.setControlPathPrefix( "composer_table" );
  QVERIFY( checker3.testComposition( mReport, 0 ) );

  mComposerAttributeTable->setFilterFeatures( false );
}

void TestQgsComposerTableV2::showEmptyRows()
{
  mComposerAttributeTable->setMaximumNumberOfFeatures( 3 );
  mComposerAttributeTable->setShowEmptyRows( true );
  QgsCompositionChecker checker( "composerattributetable_drawempty", mComposition );
  checker.setControlPathPrefix( "composer_table" );
  QVERIFY( checker.testComposition( mReport, 0 ) );
  mComposerAttributeTable->setMaximumNumberOfFeatures( 20 );
  mComposerAttributeTable->setShowEmptyRows( false );
}

void TestQgsComposerTableV2::attributeTableExtend()
{
  //test that adding and removing frames automatically does not result in a crash
  mComposerAttributeTable->removeFrame( 1 );

  //force auto creation of some new frames
  mComposerAttributeTable->setResizeMode( QgsComposerMultiFrame::ExtendToNextPage );

  mComposition->setSelectedItem( mComposerAttributeTable->frame( 1 ) );

  //now auto remove extra created frames
  mComposerAttributeTable->setMaximumNumberOfFeatures( 1 );
}

void TestQgsComposerTableV2::attributeTableRepeat()
{
  //test that creating and removing new frames in repeat mode does not crash

  mComposerAttributeTable->setResizeMode( QgsComposerMultiFrame::UseExistingFrames );
  //remove extra frames
  for ( int idx = mComposerAttributeTable->frameCount(); idx > 0; --idx )
  {
    mComposerAttributeTable->removeFrame( idx - 1 );
  }

  mComposerAttributeTable->setMaximumNumberOfFeatures( 1 );

  //force auto creation of some new frames
  mComposerAttributeTable->setResizeMode( QgsComposerMultiFrame::RepeatUntilFinished );

  for ( int features = 0; features < 50; ++features )
  {
    mComposerAttributeTable->setMaximumNumberOfFeatures( features );
  }

  //and then the reverse....
  for ( int features = 50; features > 1; --features )
  {
    mComposerAttributeTable->setMaximumNumberOfFeatures( features );
  }
}

void TestQgsComposerTableV2::attributeTableAtlasSource()
{
  QgsComposerAttributeTableV2* table = new QgsComposerAttributeTableV2( mComposition, false );


  table->setSource( QgsComposerAttributeTableV2::AtlasFeature );

  //setup atlas
  QgsVectorLayer* vectorLayer;
  QFileInfo vectorFileInfo( QString( TEST_DATA_DIR ) + "/points.shp" );
  vectorLayer = new QgsVectorLayer( vectorFileInfo.filePath(),
                                    vectorFileInfo.completeBaseName(),
                                    "ogr" );
  QgsMapLayerRegistry::instance()->addMapLayer( vectorLayer );
  mComposition->atlasComposition().setCoverageLayer( vectorLayer );
  mComposition->atlasComposition().setEnabled( true );
  QVERIFY( mComposition->atlasComposition().beginRender() );

  QVERIFY( mComposition->atlasComposition().prepareForFeature( 0 ) );
  QCOMPARE( table->contents()->length(), 1 );
  QgsComposerTableRow row = table->contents()->at( 0 );

  //check a couple of results
  QCOMPARE( row.at( 0 ), QVariant( "Jet" ) );
  QCOMPARE( row.at( 1 ), QVariant( 90 ) );
  QCOMPARE( row.at( 2 ), QVariant( 3 ) );
  QCOMPARE( row.at( 3 ), QVariant( 2 ) );
  QCOMPARE( row.at( 4 ), QVariant( 0 ) );
  QCOMPARE( row.at( 5 ), QVariant( 2 ) );

  //next atlas feature
  QVERIFY( mComposition->atlasComposition().prepareForFeature( 1 ) );
  QCOMPARE( table->contents()->length(), 1 );
  row = table->contents()->at( 0 );
  QCOMPARE( row.at( 0 ), QVariant( "Biplane" ) );
  QCOMPARE( row.at( 1 ), QVariant( 0 ) );
  QCOMPARE( row.at( 2 ), QVariant( 1 ) );
  QCOMPARE( row.at( 3 ), QVariant( 3 ) );
  QCOMPARE( row.at( 4 ), QVariant( 3 ) );
  QCOMPARE( row.at( 5 ), QVariant( 6 ) );

  //next atlas feature
  QVERIFY( mComposition->atlasComposition().prepareForFeature( 2 ) );
  QCOMPARE( table->contents()->length(), 1 );
  row = table->contents()->at( 0 );
  QCOMPARE( row.at( 0 ), QVariant( "Jet" ) );
  QCOMPARE( row.at( 1 ), QVariant( 85 ) );
  QCOMPARE( row.at( 2 ), QVariant( 3 ) );
  QCOMPARE( row.at( 3 ), QVariant( 1 ) );
  QCOMPARE( row.at( 4 ), QVariant( 1 ) );
  QCOMPARE( row.at( 5 ), QVariant( 2 ) );

  mComposition->atlasComposition().endRender();

  //try for a crash when removing current atlas layer
  QgsMapLayerRegistry::instance()->removeMapLayer( vectorLayer->id() );
  table->refreshAttributes();

  mComposition->removeMultiFrame( table );
  delete table;
}


void TestQgsComposerTableV2::attributeTableRelationSource()
{
  QFileInfo vectorFileInfo( QString( TEST_DATA_DIR ) + "/points_relations.shp" );
  QgsVectorLayer* atlasLayer = new QgsVectorLayer( vectorFileInfo.filePath(),
      vectorFileInfo.completeBaseName(),
      "ogr" );

  QgsMapLayerRegistry::instance()->addMapLayer( atlasLayer );

  //setup atlas
  mComposition->atlasComposition().setCoverageLayer( atlasLayer );
  mComposition->atlasComposition().setEnabled( true );

  //create a relation
  QgsRelation relation;
  relation.setRelationId( "testrelation" );
  relation.setReferencedLayer( atlasLayer->id() );
  relation.setReferencingLayer( mVectorLayer->id() );
  relation.addFieldPair( "Class", "Class" );
  QgsProject::instance()->relationManager()->addRelation( relation );

  QgsComposerAttributeTableV2* table = new QgsComposerAttributeTableV2( mComposition, false );
  table->setMaximumNumberOfFeatures( 50 );
  table->setSource( QgsComposerAttributeTableV2::RelationChildren );
  table->setRelationId( relation.id() );

  QVERIFY( mComposition->atlasComposition().beginRender() );
  QVERIFY( mComposition->atlasComposition().prepareForFeature( 0 ) );

  QCOMPARE( mComposition->atlasComposition().feature().attribute( "Class" ).toString(), QString( "Jet" ) );
  QCOMPARE( table->contents()->length(), 8 );

  QgsComposerTableRow row = table->contents()->at( 0 );

  //check a couple of results
  QCOMPARE( row.at( 0 ), QVariant( "Jet" ) );
  QCOMPARE( row.at( 1 ), QVariant( 90 ) );
  QCOMPARE( row.at( 2 ), QVariant( 3 ) );
  QCOMPARE( row.at( 3 ), QVariant( 2 ) );
  QCOMPARE( row.at( 4 ), QVariant( 0 ) );
  QCOMPARE( row.at( 5 ), QVariant( 2 ) );
  row = table->contents()->at( 1 );
  QCOMPARE( row.at( 0 ), QVariant( "Jet" ) );
  QCOMPARE( row.at( 1 ), QVariant( 85 ) );
  QCOMPARE( row.at( 2 ), QVariant( 3 ) );
  QCOMPARE( row.at( 3 ), QVariant( 1 ) );
  QCOMPARE( row.at( 4 ), QVariant( 1 ) );
  QCOMPARE( row.at( 5 ), QVariant( 2 ) );
  row = table->contents()->at( 2 );
  QCOMPARE( row.at( 0 ), QVariant( "Jet" ) );
  QCOMPARE( row.at( 1 ), QVariant( 95 ) );
  QCOMPARE( row.at( 2 ), QVariant( 3 ) );
  QCOMPARE( row.at( 3 ), QVariant( 1 ) );
  QCOMPARE( row.at( 4 ), QVariant( 1 ) );
  QCOMPARE( row.at( 5 ), QVariant( 2 ) );

  //next atlas feature
  QVERIFY( mComposition->atlasComposition().prepareForFeature( 1 ) );
  QCOMPARE( mComposition->atlasComposition().feature().attribute( "Class" ).toString(), QString( "Biplane" ) );
  QCOMPARE( table->contents()->length(), 5 );
  row = table->contents()->at( 0 );
  QCOMPARE( row.at( 0 ), QVariant( "Biplane" ) );
  QCOMPARE( row.at( 1 ), QVariant( 0 ) );
  QCOMPARE( row.at( 2 ), QVariant( 1 ) );
  QCOMPARE( row.at( 3 ), QVariant( 3 ) );
  QCOMPARE( row.at( 4 ), QVariant( 3 ) );
  QCOMPARE( row.at( 5 ), QVariant( 6 ) );
  row = table->contents()->at( 1 );
  QCOMPARE( row.at( 0 ), QVariant( "Biplane" ) );
  QCOMPARE( row.at( 1 ), QVariant( 340 ) );
  QCOMPARE( row.at( 2 ), QVariant( 1 ) );
  QCOMPARE( row.at( 3 ), QVariant( 3 ) );
  QCOMPARE( row.at( 4 ), QVariant( 3 ) );
  QCOMPARE( row.at( 5 ), QVariant( 6 ) );

  mComposition->atlasComposition().endRender();

  //try for a crash when removing current atlas layer
  QgsMapLayerRegistry::instance()->removeMapLayer( atlasLayer->id() );

  table->refreshAttributes();

  mComposition->removeMultiFrame( table );
  delete table;
}

void TestQgsComposerTableV2::contentsContainsRow()
{
  QgsComposerTableContents testContents;
  QgsComposerTableRow row1;
  row1 << QVariant( QString( "string 1" ) ) << QVariant( 2 ) << QVariant( 1.5 ) << QVariant( QString( "string 2" ) );
  QgsComposerTableRow row2;
  row2 << QVariant( QString( "string 2" ) ) << QVariant( 2 ) << QVariant( 1.5 ) << QVariant( QString( "string 2" ) );
  //same as row1
  QgsComposerTableRow row3;
  row3 << QVariant( QString( "string 1" ) ) << QVariant( 2 ) << QVariant( 1.5 ) << QVariant( QString( "string 2" ) );
  QgsComposerTableRow row4;
  row4 << QVariant( QString( "string 1" ) ) << QVariant( 2 ) << QVariant( 1.7 ) << QVariant( QString( "string 2" ) );

  testContents << row1;
  testContents << row2;

  QVERIFY( mComposerAttributeTable->contentsContainsRow( testContents, row1 ) );
  QVERIFY( mComposerAttributeTable->contentsContainsRow( testContents, row2 ) );
  QVERIFY( mComposerAttributeTable->contentsContainsRow( testContents, row3 ) );
  QVERIFY( !mComposerAttributeTable->contentsContainsRow( testContents, row4 ) );
}

void TestQgsComposerTableV2::removeDuplicates()
{
  QgsVectorLayer* dupesLayer = new QgsVectorLayer( "Point?field=col1:integer&field=col2:integer&field=col3:integer", "dupes", "memory" );
  QVERIFY( dupesLayer->isValid() );
  QgsFeature f1( dupesLayer->dataProvider()->fields(), 1 );
  f1.setAttribute( "col1", 1 );
  f1.setAttribute( "col2", 1 );
  f1.setAttribute( "col3", 1 );
  QgsFeature f2( dupesLayer->dataProvider()->fields(), 2 );
  f2.setAttribute( "col1", 1 );
  f2.setAttribute( "col2", 2 );
  f2.setAttribute( "col3", 2 );
  QgsFeature f3( dupesLayer->dataProvider()->fields(), 3 );
  f3.setAttribute( "col1", 1 );
  f3.setAttribute( "col2", 2 );
  f3.setAttribute( "col3", 3 );
  QgsFeature f4( dupesLayer->dataProvider()->fields(), 4 );
  f4.setAttribute( "col1", 1 );
  f4.setAttribute( "col2", 1 );
  f4.setAttribute( "col3", 1 );
  dupesLayer->dataProvider()->addFeatures( QgsFeatureList() << f1 << f2 << f3 << f4 );

  QgsComposerAttributeTableV2* table = new QgsComposerAttributeTableV2( mComposition, false );
  table->setSource( QgsComposerAttributeTableV2::LayerAttributes );
  table->setVectorLayer( dupesLayer );
  table->setMaximumNumberOfFeatures( 50 );
  QCOMPARE( table->contents()->length(), 4 );

  table->setUniqueRowsOnly( true );
  QCOMPARE( table->contents()->length(), 3 );

  //check if removing attributes in unique mode works correctly (should result in duplicate rows,
  //which will be stripped out)
  delete table->columns()->takeLast();
  table->refreshAttributes();
  QCOMPARE( table->contents()->length(), 2 );
  delete table->columns()->takeLast();
  table->refreshAttributes();
  QCOMPARE( table->contents()->length(), 1 );
  table->setUniqueRowsOnly( false );
  QCOMPARE( table->contents()->length(), 4 );

  mComposition->removeMultiFrame( table );
  delete table;
  delete dupesLayer;
}

void TestQgsComposerTableV2::multiLineText()
{
  QgsVectorLayer* multiLineLayer = new QgsVectorLayer( "Point?field=col1:string&field=col2:string&field=col3:string", "multiline", "memory" );
  QVERIFY( multiLineLayer->isValid() );
  QgsFeature f1( multiLineLayer->dataProvider()->fields(), 1 );
  f1.setAttribute( "col1", "multiline\nstring" );
  f1.setAttribute( "col2", "singleline string" );
  f1.setAttribute( "col3", "singleline" );
  QgsFeature f2( multiLineLayer->dataProvider()->fields(), 2 );
  f2.setAttribute( "col1", "singleline string" );
  f2.setAttribute( "col2", "multiline\nstring" );
  f2.setAttribute( "col3", "singleline" );
  QgsFeature f3( multiLineLayer->dataProvider()->fields(), 3 );
  f3.setAttribute( "col1", "singleline" );
  f3.setAttribute( "col2", "singleline" );
  f3.setAttribute( "col3", "multiline\nstring" );
  QgsFeature f4( multiLineLayer->dataProvider()->fields(), 4 );
  f4.setAttribute( "col1", "long triple\nline\nstring" );
  f4.setAttribute( "col2", "double\nlinestring" );
  f4.setAttribute( "col3", "singleline" );
  multiLineLayer->dataProvider()->addFeatures( QgsFeatureList() << f1 << f2 << f3 << f4 );

  mFrame2->setSceneRect( QRectF( 5, 40, 100, 90 ) );

  mComposerAttributeTable->setMaximumNumberOfFeatures( 20 );
  mComposerAttributeTable->setVectorLayer( multiLineLayer );
  QgsCompositionChecker checker( "composerattributetable_multiline", mComposition );
  checker.setControlPathPrefix( "composer_table" );
  bool result = checker.testComposition( mReport );
  QVERIFY( result );

  delete multiLineLayer;
}

void TestQgsComposerTableV2::align()
{
  QgsVectorLayer* multiLineLayer = new QgsVectorLayer( "Point?field=col1:string&field=col2:string&field=col3:string", "multiline", "memory" );
  QVERIFY( multiLineLayer->isValid() );
  QgsFeature f1( multiLineLayer->dataProvider()->fields(), 1 );
  f1.setAttribute( "col1", "multiline\nstring" );
  f1.setAttribute( "col2", "singleline string" );
  f1.setAttribute( "col3", "singleline" );
  QgsFeature f2( multiLineLayer->dataProvider()->fields(), 2 );
  f2.setAttribute( "col1", "singleline string" );
  f2.setAttribute( "col2", "multiline\nstring" );
  f2.setAttribute( "col3", "singleline" );
  QgsFeature f3( multiLineLayer->dataProvider()->fields(), 3 );
  f3.setAttribute( "col1", "singleline" );
  f3.setAttribute( "col2", "singleline" );
  f3.setAttribute( "col3", "multiline\nstring" );
  QgsFeature f4( multiLineLayer->dataProvider()->fields(), 4 );
  f4.setAttribute( "col1", "long triple\nline\nstring" );
  f4.setAttribute( "col2", "double\nlinestring" );
  f4.setAttribute( "col3", "singleline" );
  multiLineLayer->dataProvider()->addFeatures( QgsFeatureList() << f1 << f2 << f3 << f4 );

  mFrame2->setSceneRect( QRectF( 5, 40, 100, 90 ) );

  mComposerAttributeTable->setMaximumNumberOfFeatures( 20 );
  mComposerAttributeTable->setVectorLayer( multiLineLayer );

  mComposerAttributeTable->columns()->at( 0 )->setHAlignment( Qt::AlignLeft );
  mComposerAttributeTable->columns()->at( 0 )->setVAlignment( Qt::AlignTop );
  mComposerAttributeTable->columns()->at( 1 )->setHAlignment( Qt::AlignHCenter );
  mComposerAttributeTable->columns()->at( 1 )->setVAlignment( Qt::AlignVCenter );
  mComposerAttributeTable->columns()->at( 2 )->setHAlignment( Qt::AlignRight );
  mComposerAttributeTable->columns()->at( 2 )->setVAlignment( Qt::AlignBottom );
  QgsCompositionChecker checker( "composerattributetable_align", mComposition );
  checker.setControlPathPrefix( "composer_table" );
  bool result = checker.testComposition( mReport );
  QVERIFY( result );

  delete multiLineLayer;
}

void TestQgsComposerTableV2::wrapChar()
{
  QgsVectorLayer* multiLineLayer = new QgsVectorLayer( "Point?field=col1:string&field=col2:string&field=col3:string", "multiline", "memory" );
  QVERIFY( multiLineLayer->isValid() );
  QgsFeature f1( multiLineLayer->dataProvider()->fields(), 1 );
  f1.setAttribute( "col1", "multiline\nstring" );
  f1.setAttribute( "col2", "singleline string" );
  f1.setAttribute( "col3", "singleline" );
  multiLineLayer->dataProvider()->addFeatures( QgsFeatureList() << f1 );

  mComposerAttributeTable->setMaximumNumberOfFeatures( 1 );
  mComposerAttributeTable->setVectorLayer( multiLineLayer );
  mComposerAttributeTable->setWrapString( "in" );

  QList<QStringList> expectedRows;
  QStringList row;
  row << "multil\ne\nstr\ng" << "s\nglel\ne str\ng" << "s\nglel\ne";
  expectedRows.append( row );

  //retrieve rows and check
  compareTable( expectedRows );
}

void TestQgsComposerTableV2::autoWrap()
{
  QgsVectorLayer* multiLineLayer = new QgsVectorLayer( "Point?field=col1:string&field=col2:string&field=col3:string", "multiline", "memory" );
  QVERIFY( multiLineLayer->isValid() );
  QgsFeature f1( multiLineLayer->dataProvider()->fields(), 1 );
  f1.setAttribute( "col1", "long multiline\nstring" );
  f1.setAttribute( "col2", "singleline string" );
  f1.setAttribute( "col3", "singleline" );
  QgsFeature f2( multiLineLayer->dataProvider()->fields(), 2 );
  f2.setAttribute( "col1", "singleline string" );
  f2.setAttribute( "col2", "multiline\nstring" );
  f2.setAttribute( "col3", "singleline" );
  QgsFeature f3( multiLineLayer->dataProvider()->fields(), 3 );
  f3.setAttribute( "col1", "singleline" );
  f3.setAttribute( "col2", "singleline" );
  f3.setAttribute( "col3", "multiline\nstring" );
  QgsFeature f4( multiLineLayer->dataProvider()->fields(), 4 );
  f4.setAttribute( "col1", "a bit long triple line string" );
  f4.setAttribute( "col2", "double toolongtofitononeline string with some more lines on the end andanotherreallylongline" );
  f4.setAttribute( "col3", "singleline" );
  multiLineLayer->dataProvider()->addFeatures( QgsFeatureList() << f1 << f2 << f3 << f4 );

  mFrame2->setSceneRect( QRectF( 5, 40, 100, 90 ) );

  mComposerAttributeTable->setMaximumNumberOfFeatures( 20 );
  mComposerAttributeTable->setVectorLayer( multiLineLayer );
  mComposerAttributeTable->setWrapBehaviour( QgsComposerTableV2::WrapText );

  mComposerAttributeTable->columns()->at( 0 )->setWidth( 25 );
  mComposerAttributeTable->columns()->at( 1 )->setWidth( 25 );
  QgsCompositionChecker checker( "composerattributetable_autowrap", mComposition );
  checker.setControlPathPrefix( "composer_table" );
  bool result = checker.testComposition( mReport, 0 );
  mComposerAttributeTable->columns()->at( 0 )->setWidth( 0 );
  QVERIFY( result );
}

void TestQgsComposerTableV2::cellStyles()
{
  QgsComposerTableStyle original;
  original.enabled = true;
  original.cellBackgroundColor = QColor( 200, 100, 150, 90 );

  //write to xml
  QDomImplementation DomImplementation;
  QDomDocumentType documentType =
    DomImplementation.createDocumentType(
      "qgis", "http://mrcc.com/qgis.dtd", "SYSTEM" );
  QDomDocument doc( documentType );

  //test writing with no node
  QDomElement node = doc.createElement( "style" );
  QVERIFY( original.writeXML( node, doc ) );

  //read from xml
  QgsComposerTableStyle styleFromXml;
  styleFromXml.readXML( node );

  //check
  QCOMPARE( original.enabled, styleFromXml.enabled );
  QCOMPARE( original.cellBackgroundColor, styleFromXml.cellBackgroundColor );


  // check writing/reading whole set of styles
  QgsComposerAttributeTableV2 originalTable( mComposition, false );

  QgsComposerTableStyle style1;
  style1.enabled = true;
  style1.cellBackgroundColor = QColor( 25, 50, 75, 100 );
  originalTable.setCellStyle( QgsComposerTableV2::FirstRow, style1 );
  QgsComposerTableStyle style2;
  style1.enabled = false;
  style1.cellBackgroundColor = QColor( 60, 62, 64, 68 );
  originalTable.setCellStyle( QgsComposerTableV2::LastColumn, style2 );

  //write to XML
  QDomElement tableElement = doc.createElement( "table" );
  QVERIFY( originalTable.writeXML( tableElement, doc, true ) );

  //read from XML
  QgsComposerAttributeTableV2 tableFromXml( mComposition, false );
  tableFromXml.readXML( tableElement, doc, true );

  //check that styles were correctly read
  QCOMPARE( tableFromXml.cellStyle( QgsComposerTableV2::FirstRow )->enabled, originalTable.cellStyle( QgsComposerTableV2::FirstRow )->enabled );
  QCOMPARE( tableFromXml.cellStyle( QgsComposerTableV2::FirstRow )->cellBackgroundColor, originalTable.cellStyle( QgsComposerTableV2::FirstRow )->cellBackgroundColor );
  QCOMPARE( tableFromXml.cellStyle( QgsComposerTableV2::LastColumn )->enabled, originalTable.cellStyle( QgsComposerTableV2::LastColumn )->enabled );
  QCOMPARE( tableFromXml.cellStyle( QgsComposerTableV2::LastColumn )->cellBackgroundColor, originalTable.cellStyle( QgsComposerTableV2::LastColumn )->cellBackgroundColor );

  //check backgroundColor method
  //build up rules in descending order of precedence
  mComposerAttributeTable->setBackgroundColor( QColor( 50, 50, 50, 50 ) );
  QgsComposerTableStyle style;
  style.enabled = true;
  style.cellBackgroundColor = QColor( 25, 50, 75, 100 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::OddColumns, style );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 0 ), QColor( 25, 50, 75, 100 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 1 ), QColor( 50, 50, 50, 50 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 2 ), QColor( 25, 50, 75, 100 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 3 ), QColor( 50, 50, 50, 50 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 0 ), QColor( 25, 50, 75, 100 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 1 ), QColor( 50, 50, 50, 50 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 2 ), QColor( 25, 50, 75, 100 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 3 ), QColor( 50, 50, 50, 50 ) );
  style.cellBackgroundColor = QColor( 30, 80, 90, 23 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::EvenColumns, style );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 0 ), QColor( 25, 50, 75, 100 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 1 ), QColor( 30, 80, 90, 23 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 2 ), QColor( 25, 50, 75, 100 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 3 ), QColor( 30, 80, 90, 23 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 0 ), QColor( 25, 50, 75, 100 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 1 ), QColor( 30, 80, 90, 23 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 2 ), QColor( 25, 50, 75, 100 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 3 ), QColor( 30, 80, 90, 23 ) );
  style.cellBackgroundColor = QColor( 111, 112, 113, 114 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::OddRows, style );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 0 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 1 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 2 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 3 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 0 ), QColor( 25, 50, 75, 100 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 1 ), QColor( 30, 80, 90, 23 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 2 ), QColor( 25, 50, 75, 100 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 3 ), QColor( 30, 80, 90, 23 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 0 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 1 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 2 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 3 ), QColor( 111, 112, 113, 114 ) );
  style.cellBackgroundColor = QColor( 222, 223, 224, 225 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::EvenRows, style );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 0 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 1 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 2 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 3 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 0 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 1 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 2 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 3 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 0 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 1 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 2 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 3 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 3, 0 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 3, 1 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 3, 2 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 3, 3 ), QColor( 222, 223, 224, 225 ) );
  style.cellBackgroundColor = QColor( 1, 2, 3, 4 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::FirstColumn, style );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 0 ), QColor( 1, 2, 3, 4 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 1 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 2 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 3 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 0 ), QColor( 1, 2, 3, 4 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 1 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 2 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 3 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 0 ), QColor( 1, 2, 3, 4 ) );
  style.cellBackgroundColor = QColor( 7, 8, 9, 10 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::LastColumn, style );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 0 ), QColor( 1, 2, 3, 4 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 1 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 2 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 5 ), QColor( 7, 8, 9, 10 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 0 ), QColor( 1, 2, 3, 4 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 1 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 2 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 5 ), QColor( 7, 8, 9, 10 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 0 ), QColor( 1, 2, 3, 4 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 5 ), QColor( 7, 8, 9, 10 ) );
  style.cellBackgroundColor = QColor( 87, 88, 89, 90 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::HeaderRow, style );
  QCOMPARE( mComposerAttributeTable->backgroundColor( -1, 0 ), QColor( 87, 88, 89, 90 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( -1, 1 ), QColor( 87, 88, 89, 90 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( -1, 2 ), QColor( 87, 88, 89, 90 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( -1, 5 ), QColor( 87, 88, 89, 90 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 0 ), QColor( 1, 2, 3, 4 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 1 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 2 ), QColor( 111, 112, 113, 114 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 5 ), QColor( 7, 8, 9, 10 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 0 ), QColor( 1, 2, 3, 4 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 1 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 2 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 5 ), QColor( 7, 8, 9, 10 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 0 ), QColor( 1, 2, 3, 4 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 5 ), QColor( 7, 8, 9, 10 ) );
  style.cellBackgroundColor = QColor( 187, 188, 189, 190 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::FirstRow, style );
  QCOMPARE( mComposerAttributeTable->backgroundColor( -1, 0 ), QColor( 87, 88, 89, 90 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( -1, 1 ), QColor( 87, 88, 89, 90 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( -1, 2 ), QColor( 87, 88, 89, 90 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( -1, 5 ), QColor( 87, 88, 89, 90 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 0 ), QColor( 187, 188, 189, 190 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 1 ), QColor( 187, 188, 189, 190 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 2 ), QColor( 187, 188, 189, 190 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 5 ), QColor( 187, 188, 189, 190 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 0 ), QColor( 1, 2, 3, 4 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 1 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 2 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 5 ), QColor( 7, 8, 9, 10 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 0 ), QColor( 1, 2, 3, 4 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 5 ), QColor( 7, 8, 9, 10 ) );
  style.cellBackgroundColor = QColor( 147, 148, 149, 150 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::LastRow, style );
  QCOMPARE( mComposerAttributeTable->backgroundColor( -1, 0 ), QColor( 87, 88, 89, 90 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( -1, 1 ), QColor( 87, 88, 89, 90 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( -1, 2 ), QColor( 87, 88, 89, 90 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( -1, 5 ), QColor( 87, 88, 89, 90 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 0 ), QColor( 187, 188, 189, 190 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 1 ), QColor( 187, 188, 189, 190 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 2 ), QColor( 187, 188, 189, 190 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 0, 5 ), QColor( 187, 188, 189, 190 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 0 ), QColor( 1, 2, 3, 4 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 1 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 2 ), QColor( 222, 223, 224, 225 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 1, 5 ), QColor( 7, 8, 9, 10 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 0 ), QColor( 1, 2, 3, 4 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 2, 5 ), QColor( 7, 8, 9, 10 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 9, 0 ), QColor( 147, 148, 149, 150 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 9, 1 ), QColor( 147, 148, 149, 150 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 9, 2 ), QColor( 147, 148, 149, 150 ) );
  QCOMPARE( mComposerAttributeTable->backgroundColor( 9, 5 ), QColor( 147, 148, 149, 150 ) );

  mComposition->removeMultiFrame( &originalTable );
  mComposition->removeMultiFrame( &tableFromXml );
}

void TestQgsComposerTableV2::cellStylesRender()
{
  mComposerAttributeTable->setMaximumNumberOfFeatures( 3 );
  mComposerAttributeTable->setShowEmptyRows( true );

  QgsComposerTableStyle style;
  style.enabled = true;
  style.cellBackgroundColor = QColor( 25, 50, 75, 100 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::OddColumns, style );
  style.cellBackgroundColor = QColor( 90, 110, 150, 200 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::EvenRows, style );
  style.cellBackgroundColor = QColor( 150, 160, 210, 200 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::HeaderRow, style );
  style.cellBackgroundColor = QColor( 0, 200, 50, 200 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::FirstColumn, style );
  style.cellBackgroundColor = QColor( 200, 50, 0, 200 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::LastColumn, style );
  style.cellBackgroundColor = QColor( 200, 50, 200, 200 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::FirstRow, style );
  style.cellBackgroundColor = QColor( 50, 200, 200, 200 );
  mComposerAttributeTable->setCellStyle( QgsComposerTableV2::LastRow, style );

  QgsCompositionChecker checker( "composerattributetable_cellstyle", mComposition );
  checker.setColorTolerance( 10 );
  checker.setControlPathPrefix( "composer_table" );
  QVERIFY( checker.testComposition( mReport, 0 ) );
  mComposerAttributeTable->setMaximumNumberOfFeatures( 20 );
  mComposerAttributeTable->setShowEmptyRows( false );
}

QTEST_MAIN( TestQgsComposerTableV2 )
#include "testqgscomposertablev2.moc"
