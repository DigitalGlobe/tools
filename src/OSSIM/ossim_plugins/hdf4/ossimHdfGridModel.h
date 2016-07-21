//*****************************************************************************
// FILE: ossimHdfGridModel.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// AUTHOR: Mingjie Su
//
// DESCRIPTION:
//   Contains declaration of class ossimHdfGridModel. This is an
//   implementation of an interpolation sensor model.
//
//*****************************************************************************
//  $Id: ossimHdfGridModel.h 2645 2011-05-26 15:21:34Z oscar.kramer $

#ifndef ossimHdfGridModel_HEADER
#define ossimHdfGridModel_HEADER 1

#include <ossim/projection/ossimCoarseGridModel.h>
#include "../ossimPluginConstants.h"

#include <string>

/******************************************************************************
 *
 * CLASS:  ossimHdfGridModel
 *
 *****************************************************************************/



class OSSIM_PLUGINS_DLL ossimHdfGridModel : public ossimCoarseGridModel
{
public:

   /** @brief default constructor. */
   ossimHdfGridModel();
   
    //! CONSTRUCTOR (filename) Accepts name of hdf file and the index of SDS data
   ossimHdfGridModel(const ossimFilename& file, 
                    const ossimDrect& imageRect,
                    ossimString latGridIndexOrName, 
                    ossimString lonGridIndexOrName, 
                    const ossimIpt& gridSpacing);

   /** @brief virtual destructor */
   virtual ~ossimHdfGridModel();

   /**
    * @brief saveState Saves state of object to a keyword list.
    * @param kwl Initialized by this.
    * @param prefix E.g. "image0.geometry.projection."
    * @return true on success, false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0) const;

   /**
    * @brief loadState Loads state of object from a keyword list.
    * @param kwl Keyword list to initialize from.
    * @param prefix E.g. "image0.geometry.projection."
    * @return true on success, false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   
private:
   
   void setGridNodes(ossimDblGrid& grid, ossim_int32 sds_id, const ossimIpt& spacing);

   bool getWktFootprint( std::string& s ) const;
   
   void debugDump();

   TYPE_DATA
};

#endif

