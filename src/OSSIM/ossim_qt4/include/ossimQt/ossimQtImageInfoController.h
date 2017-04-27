//*******************************************************************
// Copyright (C) 2004 Intelligence Data Systems, Inc.  All rights reserved.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  David Burken
// 
// Description:
//
// Main interface between the image info dialog box and the image from the
// image chain.
//
//*************************************************************************
// $Id: ossimQtImageInfoController.h 9512 2006-09-03 21:08:05Z dburken $
#ifndef ossimQtImageInfoController_HEADER
#define ossimQtImageInfoController_HEADER

#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimObjectDestructingEvent.h>

class ossimQtImageInfoDialog;
class ossimQtScrollingImageWidget;
class ossimImageHandler;
class ossimImageChain;
class ossimKeywordlist;
class ossimMapProjection;
class ossimImageViewTransform;
class ossimImageProjection;
class ossimProjection;
class ossimImageGeometry;

class ossimQtImageInfoController : public ossimConnectableObjectListener
{
public:
   
   ossimQtImageInfoController(ossimQtImageInfoDialog* dialog);
   virtual ~ossimQtImageInfoController();

   virtual void objectDestructingEvent(ossimObjectDestructingEvent& event);

   void setImageWidget(ossimQtScrollingImageWidget* widget);
   void scanForMinMax();
   void scanForValidVertices();
   void layerComboBoxActivated();
   void initializeMinMaxNulls() const;

   void projectionComboBoxActivated();
   void zoneComboBoxActivated()            const;
   void originLineEditActivated()          const;
   void centralMeridianLineEditActivated() const;
   void tiePointX_activated()              const;
   void tiePointY_activated()              const;
   void parallelOneLineEditActivated()     const;
   void parallelTwoLineEditActivated()     const;
   void tabChanged(const QString& qs);
   void customDialogHasClosed();
   
   void saveProjection()                   const;
   void resetProjection();

   /**
    * @note  These methods can return null so caller should always
    *        check the returned pointer.
    */
   ossimImageHandler*       getImageHandler()                   const;
   ossimImageHandler*       getImageHandler(ossim_uint32 layer) const;
   ossimImageChain*         getChain()                          const;
   ossimImageChain*         getChain(ossim_uint32 layer)        const;
   ossimImageViewTransform* getImageViewTransform()             const;
   ossimProjection*         getImageProjection()                const;
   
   enum
   {
      PROJ_TYPE_UNKNOWN   = 0,
      PROJ_TYPE_SENSOR    = 1,
      PROJ_UNITS_DEGREES  = 0,
      PROJ_UNITS_METERS   = 1,
      PIXEL_TYPE_POINT    = 0,
      PIXEL_TYPE_AREA     = 1,
      NORTHERN_HEMISPHERE = 0,
      SOUTHERN_HEMISPHERE = 1
   };
   
private:

   void initializeDialog();
   
   /**
    * Updates the projection panel with info taken from keyword list.
    */
   void updateProjectionPanel(const ossimImageGeometry* geom);

   void updateDialog();
   void updateMenus(const ossimKeywordlist& kwl) const;
   
   void buildLayerMenu()                           const;
   void buildImageGeometryPage(ossim_uint32 layer) const;
   void buildProjectionPage(ossim_uint32 layer);
   void buildValidVerticesPage(ossim_uint32 layer) const;
   
   void buildProjectionMenu() const;
   void buildDatumMenu()      const;
   void buildUnitsMenu()      const;
   void buildPixelTypeMenu()  const;
   void buildZoneMenu()       const;
   void buildHemisphereMenu() const;

   void setup_UTM_Menus() const;
   void setupStatePlaneMenus(const ossimString& name) const;
   
   /**
    * Gets from the dialog box.
    */
   ossimString getProjectionString()      const;
   ossimString getDatumString()           const;
   ossimString getUnitsString()           const;
   ossimString getPixelTypeString()       const;
   ossimString getZoneString()            const;
   ossimString getHemisphereString()      const;
   ossimString getOriginLatitudeString()  const;
   ossimString getCentralMeridianString() const;
   ossimString getFalseEastingString()    const;
   ossimString getFalseNorthingString()   const;
   ossimString getTiePointX_String()      const;
   ossimString getTiePointY_String()      const;
   ossimString getScaleX_String()         const;
   ossimString getScaleY_String()         const;
   ossimString getScaleFactorString()     const;
   ossimString getParallelOneString()     const;
   ossimString getParallelTwoString()     const;

   /**
    * Convenience get methods.
    */
   long                     getZone()                           const;
   ossimString              getDatumCode()                      const;
   double                   getDegrees(const ossimString& s,
                                       bool lat_flag)           const;
   bool                     isGeographic()                      const;
   ossim_uint32             getLayerIndex()                     const;
   ossim_uint32             getBandIndex()                      const;
   
   void setProjectionMenu(const ossimKeywordlist& kwl)         const;

   void setDatumMenu(const ossimKeywordlist& kwl)              const;
   void setDatumMenu(const ossimString& code)                  const;   

   void setUnitsMenu(const ossimMapProjection* proj)           const;
   void setPixelTypeMenu(const ossimKeywordlist& kwl)          const;
   
   void setZoneMenu(const ossimMapProjection* proj)            const;
   void setZoneMenu(const ossimKeywordlist& kwl)               const;
   
   void setHemisphereMenu(const ossimMapProjection* proj)      const;
   void setHemisphereMenu(const ossimKeywordlist& kwl)         const;

   void setOriginLatitude(const ossimMapProjection* proj)      const;
   void setOriginLatitude(const ossimKeywordlist& kwl)         const;
   void setOriginLatitude(const double& origin)                const;

   void setCentralMeridian(const ossimMapProjection* proj)     const;
   void setCentralMeridian(const ossimKeywordlist& kwl)        const;
   void setCentralMeridian(const double& meridian)             const;

   void setFalseEasting(const ossimMapProjection* proj)        const;   
   void setFalseEasting(const ossimKeywordlist& kwl)           const;
   void setFalseEasting(const double& false_easting)           const;

   void setFalseNorthing(const ossimMapProjection* proj)       const;   
   void setFalseNorthing(const ossimKeywordlist& kwl)          const;
   void setFalseNorthing(const double& false_northing)         const;

   void setTiePointX(const ossimMapProjection* proj)           const;
   void setTiePointX(const ossimKeywordlist& kwl)              const;
   void setTiePointX(const double& tie)                        const;

   void setTiePointY(const ossimMapProjection* proj)           const;
   void setTiePointY(const ossimKeywordlist& kwl)              const;
   void setTiePointY(const double& tie)                        const;

   void setScaleX(const ossimMapProjection* proj)              const;
   void setScaleX(const ossimKeywordlist& kwl)                 const;

   void setScaleY(const ossimMapProjection* proj)              const;
   void setScaleY(const ossimKeywordlist& kwl)                 const;

   void setScaleFactor(const ossimKeywordlist& kwl)            const;
   void setScaleFactor(const double& scale)                    const;

   void setParallelOne(const ossimKeywordlist& kwl)            const;
   void setParallelOne(const ossimMapProjection* proj)         const;
   void setParallelOne(const double& degrees)                  const;

   void setParallelTwo(const ossimKeywordlist& kwl)            const;
   void setParallelTwo(const ossimMapProjection* proj)         const;
   void setParallelTwo(const double& degrees)                  const;

   /**
    * Based on current projection, this will turn on and off appropriate
    * fields.
    */
   void enableDisableProjectionPanelFields() const;
   
   /**
    * Enable/disable menu methods.
    *
    * @param flag true flag = enable,  false flag = disable
    */
   void enableDatum(bool flag)                 const;
   void enableUnits(bool flag)                 const;
   void enablePixelType(bool flag)             const;
   void enableZone(bool flag)                  const;
   void enableHemisphere(bool flag)            const;
   void enableOriginLatitude(bool flag)        const;
   void enableCentralMeridian(bool flag)       const;
   void enableFalseEastingNorthing(bool flag)  const;
   void enableTiePoints(bool flag)             const;
   void enableScale(bool flag)                 const;
   void enableScaleFactor(bool flag)           const;
   void enableParallels(bool flag)             const;
   void enableAllProjectionPanel(bool flag)    const;
   
   /**
    * Freeze/unfreeze methods.  Things are frozen if they are predefined,
    * like in the case of UTM.
    *
    * @param flag true flag = freeze, false flag = unfreeze
    */
   void freezeLines(bool flag)                const;
   void freezeSamples(bool flag)              const;
   void freezeBands(bool flag)                const;
   void freezeReducedResLevel(bool flag)      const;
   void freezeMin(bool flag)                  const;
   void freezeMax(bool flag)                  const;
   void freezeNull(bool flag)                 const;
   void freezeFalseEastingNorthing(bool flag) const;
   void freezeOrigin(bool flag)               const; // origin and meridian.
   void freezeScaleFactor(bool flag)          const;
   void freezeParallels(bool flag)            const;
   
   /**
    * Makes projection and performs a loadState.
    */
   ossimRefPtr<ossimMapProjection>  getNewMapProjection(const ossimKeywordlist& kwl) const;

   /**
    * Makes projection.
    */
   ossimRefPtr<ossimMapProjection>  getNewMapProjection(const ossimString& name) const;
   
   // Disallow copy  constructor, assignment operator.
   ossimQtImageInfoController( const ossimQtImageInfoController&);
   ossimQtImageInfoController& operator=( const ossimQtImageInfoController&);
   
   ossimQtImageInfoDialog*      theDialog;
   ossimQtScrollingImageWidget* theWidget;
   QDialog*                     theCustomDialog; 
   mutable bool                 theCallBackDisabled;
};

#endif /* #ifndef ossimQtImageInfoController_HEADER */
