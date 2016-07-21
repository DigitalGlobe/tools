/***************************************************************************
     testqgsdatasourceuri.cpp
     --------------------------------------
    Date                 : Thu Apr 16 2015
    Copyright            : (C) 2015 by Sandro Mani
    Email                : manisandro@gmail.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <QtTest/QtTest>
#include <QObject>
#include <QString>
//header for class being tested
#include <qgsdatasourceuri.h>

Q_DECLARE_METATYPE( QgsWKBTypes::Type )
Q_DECLARE_METATYPE( QgsDataSourceURI::SSLmode )

class TestQgsDataSourceUri: public QObject
{
    Q_OBJECT
  private slots:
    void checkparser();
    void checkparser_data();
};

void TestQgsDataSourceUri::checkparser_data()
{
  QTest::addColumn<QString>( "uri" );
  QTest::addColumn<QString>( "table" );
  QTest::addColumn<QString>( "geometrycolumn" );
  QTest::addColumn<QString>( "key" );
  QTest::addColumn<bool>( "estimatedmetadata" );
  QTest::addColumn<QString>( "srid" );
  QTest::addColumn<QgsWKBTypes::Type>( "type" );
  QTest::addColumn<bool>( "selectatid" );
  QTest::addColumn<QString>( "service" );
  QTest::addColumn<QString>( "user" );
  QTest::addColumn<QString>( "password" );
  QTest::addColumn<QString>( "dbname" );
  QTest::addColumn<QString>( "host" );
  QTest::addColumn<QString>( "port" );
  QTest::addColumn<QString>( "driver" );
  QTest::addColumn<QgsDataSourceURI::SSLmode>( "sslmode" );
  QTest::addColumn<QString>( "sql" );
  QTest::addColumn<QString>( "myparam" );


  QTest::newRow( "oci" )
  << "host=myhost port=1234 user='myname' password='mypasswd' estimatedmetadata=true srid=1000003007 table=\"myschema\".\"mytable\" (GEOM) myparam='myvalue' sql="
  << "mytable" // table
  << "GEOM" // geometrycolumn
  << "" // key
  << true // estimatedmetadata
  << "1000003007" // srid
  << QgsWKBTypes::Unknown // type
  << false // selectatid
  << "" // service
  << "myname" // user
  << "mypasswd" // password
  << "" // dbname
  << "myhost" // host
  << "1234" // port
  << "" // driver
  << QgsDataSourceURI::SSLprefer // sslmode
  << "" // sql
  << "myvalue" // myparam
  ;

  QTest::newRow( "pgrast" )
  << "PG: dbname=mydb host=myhost user=myname password=mypasswd port=5432 mode=2 schema=public column=geom table=mytable"
  << "mytable" // table
  << "" // geometrycolumn
  << "" // key
  << false // estimatedmetadata
  << "" // srid
  << QgsWKBTypes::Unknown // type
  << false // selectatid
  << "" // service
  << "myname" // user
  << "mypasswd" // password
  << "mydb" // dbname
  << "myhost" // host
  << "5432" // port
  << "" // driver
  << QgsDataSourceURI::SSLprefer // sslmode
  << "" // sql
  << "" // myparam
  ;

  QTest::newRow( "pgmlsz" )
  << "PG: dbname=mydb host=myhost user=myname password=mypasswd port=5432 mode=2 schema=public column=geom table=mytable type=MultiLineStringZ"
  << "mytable" // table
  << "" // geometrycolumn
  << "" // key
  << false // estimatedmetadata
  << "" // srid
  << QgsWKBTypes::MultiLineStringZ // type
  << false // selectatid
  << "" // service
  << "myname" // user
  << "mypasswd" // password
  << "mydb" // dbname
  << "myhost" // host
  << "5432" // port
  << "" // driver
  << QgsDataSourceURI::SSLprefer // sslmode
  << "" // sql
  << "" // myparam
  ;

  QTest::newRow( "DB2" )
  << "host=localhost port=50000 dbname=OSTEST user='osuser' password='osuserpw' estimatedmetadata=true srid=4326 key=OBJECTID table=TEST.ZIPPOINT (GEOM) myparam='myvalue' driver='IBM DB2 ODBC DRIVER' sql="
  << "TEST.ZIPPOINT" // table
  << "GEOM" // geometrycolumn
  << "OBJECTID" // key
  << true // estimatedmetadata
  << "4326" // srid
  << QgsWKBTypes::Unknown // type
  << false // selectatid
  << "" // service
  << "osuser" // user
  << "osuserpw" // password
  << "OSTEST" // dbname
  << "localhost" // host
  << "50000" // port
  << "IBM DB2 ODBC DRIVER" // driver
  << QgsDataSourceURI::SSLprefer // sslmode
  << "" // sql
  << "myvalue" // myparam
  ;
}

void TestQgsDataSourceUri::checkparser()
{
  QFETCH( QString, uri );
  QFETCH( QString, table );
  QFETCH( QString, geometrycolumn );
  QFETCH( QString, key );
  QFETCH( bool, estimatedmetadata );
  QFETCH( QString, srid );
  QFETCH( QgsWKBTypes::Type, type );
  QFETCH( bool, selectatid );
  QFETCH( QString, service );
  QFETCH( QString, user );
  QFETCH( QString, password );
  QFETCH( QString, dbname );
  QFETCH( QString, host );
  QFETCH( QString, port );
  QFETCH( QString, driver );
  QFETCH( QgsDataSourceURI::SSLmode, sslmode );
  QFETCH( QString, sql );
  QFETCH( QString, myparam );

  QgsDataSourceURI ds( uri );
  QCOMPARE( ds.table(), table );
  QCOMPARE( ds.geometryColumn(), geometrycolumn );
  QCOMPARE( ds.keyColumn(), key );
  QCOMPARE( ds.useEstimatedMetadata(), estimatedmetadata );
  QCOMPARE( ds.srid(), srid );
  QCOMPARE( ds.newWkbType(), type );
  QCOMPARE( ds.selectAtIdDisabled(), selectatid );
  QCOMPARE( ds.service(), service );
  QCOMPARE( ds.username(), user );
  QCOMPARE( ds.password(), password );
  QCOMPARE( ds.database(), dbname );
  QCOMPARE( ds.host(), host );
  QCOMPARE( ds.port(), port );
  QCOMPARE( ds.driver(), driver );
  QCOMPARE( ds.sslMode(), sslmode );
  QCOMPARE( ds.sql(), sql );
  QCOMPARE( ds.param( "myparam" ), myparam );
}

QTEST_MAIN( TestQgsDataSourceUri )
#include "testqgsdatasourceuri.moc"
