//*******************************************************************
// Copyright (C) 2003 ImageLinks Inc. 
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
// Author:  David Burken <dburken@imagelinks.com>
// 
// Description:
// 
// Class declaration for ossimQtHistogramWidget.
// 
// Derived from QWidget.
// 
// A QT Widget for displaying histograms.
//
//*************************************************************************
// $Id: ossimQtHistogramWidget.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef ossimQtHistogramWidget_HEADER
#define ossimQtHistogramWidget_HEADER

#include <QtGui/QWidget>
  // #include <qwidgetplugin.h>
#include <ossim/base/ossimConstants.h>

class QPaintEvent;
class ossimMultiBandHistogram;
class ossimHistogramRemapper;

class ossimQtHistogramWidget : public QWidget
{
   Q_OBJECT
      
public:

   enum PaintMode
   {
      PAINT_BAND = 0,
      PAINT_AVG  = 1
   };
   
   ossimQtHistogramWidget(QWidget* parent = 0,
                          const char* name = 0,
                          Qt::WFlags f = 0);

   virtual ~ossimQtHistogramWidget();

   /*!
    * We need a pointer to the remapper as it's the only one who can map
    * it's bands to the histogram file bands.
    */
   void setHistogramRemapper(ossimHistogramRemapper* remapper);


   /*!
    * Band is zero based.
    */
   // void paintHistogram(ossim_uint32 zero_based_band);

   /*!
    * Averages all bands.
    */
   // void paintHistogram();

   /*!
    * A band of zero tells paintHistogram to paint average of all bands...
    */
   void setBand(ossim_uint32 one_based_band);

   void setPaintMode(PaintMode mode);

protected:

   virtual void paintEvent(QPaintEvent* event);
   
private:

   /*!
    * Band is zero based.
    */
   void paintBand();

   /*!
    * Averages all bands.
    */
   void paintAvg();

   double getMaxBinCount();

   // Hide from use...
   ossimQtHistogramWidget(const ossimQtHistogramWidget& rhs);
   ossimQtHistogramWidget& operator=(const ossimQtHistogramWidget& rhs);

   ossimHistogramRemapper* theRemapper;

   PaintMode               thePaintMode;

   // Zero based band to paint if paint mode = PAINT_BAND
   ossim_uint32            theBand;
   
};

#endif /* #ifndef ossimQtHistogramWidget_HEADER */
