//-----------------------------------------------------------------------------
/* 
 * OpenDISTest.cpp
 */
//-----------------------------------------------------------------------------

#include "OpenDISTest.h"
#include <DIS/DataStream.h>
#include <DIS/EntityType.h>

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(COpenDISTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void COpenDISTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void COpenDISTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests marshalling and unmarshalling.
 */
void COpenDISTest::testMarshal()
{
    DIS::DataStream dataStream(DIS::BIG);
    DIS::EntityType marshalEntity;
    DIS::EntityType unmarshalEntity;

    // create and marshal an entity
    marshalEntity.setCategory(1);
    marshalEntity.setCountry(222);
    marshalEntity.setDomain(1);
    marshalEntity.setEntityKind(1);
    marshalEntity.setExtra(0);
    marshalEntity.setSpecific(2);
    marshalEntity.setSubcategory(2);
    marshalEntity.marshal(dataStream);

    // unmarshal an entity
    unmarshalEntity.unmarshal(dataStream);
    CPPUNIT_ASSERT_EQUAL( static_cast<unsigned char>(1)    ,
                          unmarshalEntity.getCategory()    );
    CPPUNIT_ASSERT_EQUAL( static_cast<unsigned short>(222) ,
                          unmarshalEntity.getCountry()     );
    CPPUNIT_ASSERT_EQUAL( static_cast<unsigned char>(1)    ,
                          unmarshalEntity.getDomain()      );
    CPPUNIT_ASSERT_EQUAL( static_cast<unsigned char>(1)    ,
                          unmarshalEntity.getEntityKind()  );
    CPPUNIT_ASSERT_EQUAL( static_cast<unsigned char>(0)    ,
                          unmarshalEntity.getExtra()       );
    CPPUNIT_ASSERT_EQUAL( static_cast<unsigned char>(2)    ,
                          unmarshalEntity.getSpecific()    );
    CPPUNIT_ASSERT_EQUAL( static_cast<unsigned char>(2)    ,
                          unmarshalEntity.getSubcategory() );
}
//-----------------------------------------------------------------------------
