/*
 * Copyright (C) 2003  Neil Salter
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * (A full copy of the license has is available in the file COPYING in
 * the top directory of this distribution.)
 */

#ifndef ossimPlanetQtActionAdapter_HEADER
#define ossimPlanetQtActionAdapter_HEADER

#include <osgGA/GUIActionAdapter>
#include <ossimPlanetQt/ossimPlanetQtExport.h>

/// QtActionAdapter allows window-system-agnostic osgGA classes to tell
/// the window system what to do

/**
 * QtActionAdapter - a class to adapt osgGA-specified actions into feedback for our app.
 */

class  OSSIMPLANETQT_DLL ossimPlanetQtActionAdapter : public osgGA::GUIActionAdapter
{
    public:

        ossimPlanetQtActionAdapter():theNeedRedraw(false),
           theNeedContinuousUpdate(false),
           theNeedWarpPointer(false),
           theX(-1),theY(-1)
        {};
           void reset()
           {
              theNeedRedraw = false;
              theNeedContinuousUpdate = false;
              theNeedWarpPointer = false;
              theX = -1;
              theY = -1;
           }
        virtual ~ossimPlanetQtActionAdapter() {}

	virtual void requestRedraw()
           {
              theNeedRedraw=true;
           }

	virtual void requestContinuousUpdate(bool needed=true)
           {
              theNeedContinuousUpdate=needed;
           }

	virtual void requestWarpPointer(float x,float y)
           {
              theNeedWarpPointer=true;
              theX=x;
              theY=y;
           }

	virtual void clearWarpRequest()
           {
              theNeedWarpPointer = false;
           }
        void setNeedRedraw(bool flag){ theNeedRedraw = flag;}
        void setNeedContinuousUpdate(bool flag){ theNeedContinuousUpdate = flag;}
	bool getNeedRedraw() const {return theNeedRedraw;}
	bool getNeedContinuousUpdate() const {return theNeedContinuousUpdate;}
	bool getNeedWarpPointer(float &x,float &y) const
           {
              if(theNeedWarpPointer)
              {
                 x=theX;
                 y=theY;
              }
              
              return theNeedWarpPointer;
           }

 private:

	bool theNeedRedraw;
	bool theNeedContinuousUpdate;
	bool theNeedWarpPointer;
	float theX,theY;
};

#endif
