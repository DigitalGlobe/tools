//-----------------------------------------------------------------------------
/* 
 * HDF5Test.cpp
 */
//-----------------------------------------------------------------------------

#include "HDF5Test.h"
#include "hdf5.h"

//-----------------------------------------------------------------------------
// class constants

    const char* CHDF5Test::DATA_SET_NAME   = "DS1";
    const char* CHDF5Test::FILE_NAME_INPUT = "HDF5\\data\\h5ex_t_array.h5";

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CHDF5Test);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CHDF5Test::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CHDF5Test::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests reading an HDF5 file.
 */
void CHDF5Test::testRead()
{
    const int DIMENSION_COUNT = 2;
    hid_t     hDataSet   = 0;
    hid_t     hDataSpace = 0;
    hid_t     hDataType  = 0;
    hid_t     hFile      = 0;
    hsize_t   aullDimensions[DIMENSION_COUNT];
    int       iRank      = 0;
    int       iStatus    = 0;
    size_t    uiSize     = 0;

    // open the file and data set
    hFile = ::H5Fopen( CHDF5Test::FILE_NAME_INPUT ,
                       H5F_ACC_RDONLY             ,
                       H5P_DEFAULT                );
    CPPUNIT_ASSERT(0 != hFile);
    hDataSet = ::H5Dopen2( hFile                    ,
                           CHDF5Test::DATA_SET_NAME ,
                           H5P_DEFAULT              );
    CPPUNIT_ASSERT(0 != hDataSet);

    // get the data type and data space
    hDataType = ::H5Dget_type(hDataSet);
    CPPUNIT_ASSERT(0 != hDataType);
    uiSize = ::H5Tget_size(hDataType);
    CPPUNIT_ASSERT_EQUAL( static_cast<unsigned>(120U  ) ,
                          static_cast<unsigned>(uiSize) );
    hDataSpace = ::H5Dget_space(hDataSet);
    CPPUNIT_ASSERT(0 != hDataSpace);
    iRank = ::H5Sget_simple_extent_ndims(hDataSpace);
    CPPUNIT_ASSERT_EQUAL( 1     ,
                          iRank );
    iStatus = ::H5Sget_simple_extent_dims( hDataSpace     ,
                                           aullDimensions ,
                                           NULL           );
    CPPUNIT_ASSERT_EQUAL( 1                 ,
                          iStatus           );
    CPPUNIT_ASSERT_EQUAL( 4ULL              ,
                          aullDimensions[0] );

    // close the file and data set
    ::H5Dclose(hDataSet);
    hDataSet = 0;
    ::H5Fclose(hFile);
    hFile = 0;
}
//-----------------------------------------------------------------------------
