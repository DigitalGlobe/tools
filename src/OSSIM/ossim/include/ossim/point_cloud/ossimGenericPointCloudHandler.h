//**************************************************************************************************
//
// OSSIM (http://trac.osgeo.org/ossim/)
//
// License: MIT
//
//**************************************************************************************************
// $Id: ossimGenericPointCloudHandler.h 23664 2015-12-14 14:17:27Z dburken $

#ifndef ossimGenericPointCloudHandler_HEADER
#define ossimGenericPointCloudHandler_HEADER 1

#include <ossim/point_cloud/ossimPointCloudHandler.h>
#include <ossim/point_cloud/ossimPointBlock.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimGrect.h>
#include <vector>

using namespace std;

class OSSIM_DLL ossimGenericPointCloudHandler : public ossimPointCloudHandler
{
public:
   ossimGenericPointCloudHandler(vector<ossimEcefPoint>& ecef_points);
   ossimGenericPointCloudHandler(vector<ossimGpt>& ground_points);
   virtual ~ossimGenericPointCloudHandler();
   virtual ossim_uint32 getNumPoints() const;
   virtual void getFileBlock(ossim_uint32 offset,
                             ossimPointBlock& block,
                             ossim_uint32 maxNumPoints=0xFFFFFFFF)const;
   virtual ossim_uint32 getFieldCode() const;
   virtual bool open(const ossimFilename& pointsFile);
   virtual void close();

protected:
   ossimGenericPointCloudHandler() {}
   ossimPointBlock m_pointBlock;
};

#endif /* #ifndef ossimPdalReader_HEADER */

