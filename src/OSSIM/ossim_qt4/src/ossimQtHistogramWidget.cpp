//----------------------------------------------------------------------------
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author:  David Burken
// 
// Description:
// 
// Class definition for ossimQtHistogramWidget.
// 
// Derived from QWidget.
// 
// A QT Widget for displaying histograms.
//
// $Id: ossimQtHistogramWidget.cpp 12563 2008-03-20 11:46:29Z gpotts $
//----------------------------------------------------------------------------

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
//Added by qt3to4:
#include <QtGui/QPixmap>
#include <ossimQt/ossimQtHistogramWidget.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimHistogram.h>
#include <ossim/imaging/ossimHistogramRemapper.h>

const int WIDTH  = 258;
const int HEIGHT = 130;

ossimQtHistogramWidget::ossimQtHistogramWidget(QWidget* parent,
                                               const char* name,
                                               Qt::WFlags f)
   :
      QWidget(parent, name, f), // base class.
      theRemapper(0),
      thePaintMode(PAINT_AVG),
      theBand(0)
{
   
   setMinimumWidth(WIDTH);
   setMinimumHeight(HEIGHT);
   setMaximumWidth(WIDTH);
   setMaximumHeight(HEIGHT);
   // setBackgroundColor(Qt::white);
   // setBackgroundRole(QPalette::Base);
   setBackgroundRole(QPalette::Background);
   // paintHistogram(0);
}

ossimQtHistogramWidget::~ossimQtHistogramWidget()
{
}

void ossimQtHistogramWidget::setBand(ossim_uint32 one_based_band)
{
   theBand = one_based_band;
}

void ossimQtHistogramWidget::setPaintMode(PaintMode mode)
{
   thePaintMode = mode;
   update();
}

void ossimQtHistogramWidget::paintEvent(QPaintEvent* /* event */ )
{
   if (!theRemapper)
   {
      return;
   }

   if (thePaintMode == PAINT_AVG)
   {
      paintAvg();
   }
   else
   {
      paintBand();
   }
}

void ossimQtHistogramWidget::paintBand()
{
   if (!theRemapper)
   {
      return;
   }

   const int H = HEIGHT - 2;

   QPainter p(this);
   QPen p1(Qt::black, 1);
   p.setPen(p1);

   ossimRefPtr<ossimHistogram> h = theRemapper->getHistogram(theBand);
   if (h.valid())
   {
      float min = floor(h->GetMinVal());
      float max = ceil(h->GetMaxVal());
      float maxBins = h->GetRes();
      ossimRefPtr<ossimHistogram> scaledHisto = new ossimHistogram(WIDTH, min, max);
      ossim_uint32 binIdx = 0;
      float delta = (max-min)/maxBins;
      float* sumCounts = scaledHisto->GetCounts();
      ossim_int32 sumIndex = 0;
      float value = 0.0;
      memset(sumCounts, '\0', sizeof(float)*WIDTH);
      
      for(binIdx = 0; binIdx < maxBins;++binIdx)
      {
         value = min+delta*binIdx;
         sumIndex = scaledHisto->GetIndex(value);
         if(sumIndex >= 0)
         {
            sumCounts[sumIndex] += h->GetCount(value);
         }
      }
      float sumMaxCount   = scaledHisto->GetMaxCount();
      int height = 0;
      if(sumMaxCount > 0.0)
      {
         for (int col = 1; col < WIDTH; ++col)
         {
            height = ossim::round<int>((sumCounts[col-1]/sumMaxCount)*H);
            
            p.drawLine(col, H, col, H-height);
         }
      }
   }
}

void ossimQtHistogramWidget::paintAvg()
{
   // This paints an average of all bands it more than one.
   if (!theRemapper)
   {
      return;
   }

   const int H = HEIGHT - 2;

   QPainter p(this);
   QPen p1(Qt::black, 1);
   p.setPen(p1);

   ossim_uint32 band = 0;
   const ossim_uint32 BANDS = theRemapper->getNumberOfOutputBands();
   std::vector<ossimRefPtr<ossimHistogram> > h;
   if(BANDS<1) return;
   h.resize(BANDS);

   if(!h.size()) return;
   bool validDrawFlag = true;
   for(band = 0; band < BANDS; ++ band)
   {
      h[band] = theRemapper->getHistogram(band);
      if(!h[band].valid()) validDrawFlag = false;
   }

   if(!validDrawFlag)
   {
      return;
   }

   float min = 99999.0;
   float max = -99999.0;
   float max_count = 0.0;
   ossim_uint32 maxBins   = 0;   
   for (band = 0; band < BANDS; ++band)
   {
      if(h[band].valid())
      {
         float hmin       = floor(h[band]->GetMinVal());
         float hmax       = ceil(h[band]->GetMaxVal());
         float hmax_count = h[band]->GetMaxCount();
         ossim_uint32 res = h[band]->GetRes();
         
         maxBins = res > maxBins?res:maxBins;
         min = hmin < min ? hmin : min;
         max = hmax > max ? hmax : max;
         // max_count = hmax_count > max_count ? hmax_count : max_count;
         max_count += hmax_count;
      }
   }
   ossimRefPtr<ossimHistogram> scaledHisto = new ossimHistogram(WIDTH, min, max);
   ossim_uint32 binIdx = 0;
   float delta = (max-min)/maxBins;
   float* sumCounts = scaledHisto->GetCounts();
   ossim_int32 sumIndex = 0;
   float value = 0.0;
   memset(sumCounts, '\0', sizeof(float)*WIDTH);

   for(binIdx = 0; binIdx < maxBins;++binIdx)
   {
      value = min+delta*binIdx;
      sumIndex = scaledHisto->GetIndex(value);
      if(sumIndex >= 0)
      {
         for(band = 0; band < BANDS; ++band)
         {
            sumCounts[sumIndex] += h[band]->GetCount(value);
         }
      }
   }

   float sumMaxCount   = scaledHisto->GetMaxCount();
   int height = 0;
   if(sumMaxCount > 0.0)
   {
      for (int col = 1; col < WIDTH; ++col)
      {
         height = ossim::round<int>((sumCounts[col-1]/sumMaxCount)*H);
         
         p.drawLine(col, H, col, H-height);
      }
   }
}

void
ossimQtHistogramWidget::setHistogramRemapper(ossimHistogramRemapper* remapper)
{
   theRemapper = remapper;
}

double getMaxBinCount()
{
   return 0.0;
}

// private copy constructor.
ossimQtHistogramWidget::ossimQtHistogramWidget(const ossimQtHistogramWidget& /* rhs */)
   :
      QWidget(NULL, NULL, 0),
      theRemapper(NULL)
{}

// private operator=
ossimQtHistogramWidget& ossimQtHistogramWidget::operator=(const ossimQtHistogramWidget& /* rhs */)
{
   return *this;
}
