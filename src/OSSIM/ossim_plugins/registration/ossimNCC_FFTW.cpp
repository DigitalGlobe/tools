#include "ossimNCC_FFTW.h"
#include "ossimRunningSum.h"

#include <cmath>

ossimNCC_FFTW::ossimNCC_FFTW(int cy, int cx, const char* wisdom)
 : _mx(0),
   _my(0),
   _sx(0),
   _sy(0),
   _cx(cx),
   _cy(cy),
   _pcx(2*(cx/2+1)),
   _NCC(cy,2*(cx/2+1)),
   _PS(cy,2*(cx/2+1)),
   _srs(NULL),
   _mavg(0.0),
   _mstd(0.0),
   _savg(0.0),
   _sstd(0.0),
   _maxy(0),
   _maxx(0),
   _maxncc(-2)
{
   //re-use wisdom (if provided) : IMPORTANT don't exchange wisdom strings across different systems !
   if (wisdom)
   {
      fftw_import_wisdom_from_string(wisdom);
   }


   //create plans : TBC TBD use different planner flags?
   _splan = fftw_plan_dft_r2c_2d(_cy, 
                                 _cx,
                                 _PS.refBuffer(),
                                 (double(*)[2])_PS.refBuffer(),
                                 FFTW_MEASURE); //note: ESTIMATE flag does nor erase input/output buffer
   _mplan = fftw_plan_dft_r2c_2d(_cy, 
                                 _cx,
                                 _NCC.refBuffer(),
                                 (double(*)[2])_NCC.refBuffer(),
                                 FFTW_MEASURE);
   //inverse FFT (inplace)
   _ccplan = fftw_plan_dft_c2r_2d(_cy, 
                                  _cx,
                                  (double(*)[2])_NCC.refBuffer(),
                                  _NCC.refBuffer(),
                                  FFTW_MEASURE);
}

ossimNCC_FFTW::~ossimNCC_FFTW()
{
   //delete plans (matrices are automatically discarded)
   fftw_destroy_plan(_ccplan);
   fftw_destroy_plan(_mplan);
   fftw_destroy_plan(_splan);
   //delete running sum
   if (_srs!=NULL)
   {
      delete _srs;
      _srs=NULL;
   }
}

bool
ossimNCC_FFTW::sameDims(int tcy, int tcx)const
{
   return ((tcx==_cx)&&(tcy==_cy));
}

void
ossimNCC_FFTW::ingestMaster(int my,int mx, const double* master)
{
   //IMPORTANT NOTE this will overwrite the stored NCC
   if ((my>_cy) || (mx>_cx) || (master==NULL))
   {
      //TBD error mngt
      std::cerr<<"ingestMaster input error"<<std::endl;
      return;
   }
   //store master dims
   _my=my;
   _mx=mx;

   //calculate stats
   moments1(_mx*_my, master, &_mavg, &_mstd);

   //rotate180 master & remove mean, then pad to correlation size   
   double* pm = _NCC.refBuffer();
   const double* rm = master + (_mx*_my-1); //last position
   int i,j;
   for(i=0;i<_my;++i)
   {
      for(j=0;j<_mx;++j)
      {
         *(pm++) = *(rm--) - _mavg; // master minus average, reversed pm
      }
      for(;j<_pcx;++j) *(pm++)=0.0;
   }
   for(;i<_cy;++i) for(j=0;j<_pcx;++j) *(pm++)=0.0;
}

void
ossimNCC_FFTW::ingestSlave(int sy, int sx, const double* slave)
{   
   if ((sy>_cy) || (sx>_cx) || (slave==NULL))
   {
      //TBD error mngt
      std::cerr<<"ingestSlave input error"<<std::endl;
      return;
   }
   //store slave dims
   _sy=sy;
   _sx=sx;

   //calculate running sums
   if (_srs != NULL) delete _srs;
   _srs = new ossimRunningSum(_sy, _sx, slave);
   _savg = _srs->getAverage();
   _sstd = _srs->getStd();

   //pad slave
   double* ps= _PS.refBuffer();
   const double* fs = slave; //first position

   int i,j;
   for(i=0;i<_sy;++i)
   {
      for(j=0;j<_sx;++j)
      {
         *(ps++) = *(fs++) - _savg; //remove average value
      }
      for(;j<_pcx;++j) *(ps++)=0.0;
   }
   for(;i<_cy;++i) for(j=0;j<_pcx;++j) *(ps++)=0.0;
}

//calculate NCC based on master & slave
//also stores the maximum NCC position
bool
ossimNCC_FFTW::calculateNCC()
{
   //check master & slave dimensions
   if ((_cx != _sx+_mx-1) || (_cy != _sy+_my-1))
   {
      //TBD err mngt
      std::cerr<<"calculateNCC input error"<<std::endl;
      return false;
   }

   //transform padded reversed master & padded slave to freq. space
   fftw_execute(_splan);
   fftw_execute(_mplan);

   //multiply master by slave in freq. space (half transform only)
   const double* s = _PS.getBuffer();
   double*       m = _NCC.refBuffer();
   double rr;
   int i,j;
   for(i=0;i<_cy;++i) 
   {
      for(j=0;j<_cx/2+1;++j) //number of complex coeffs (halfed because of real transform)
      {
          //own complex multiplication - could be faster(can't use latest complex type)
         rr   = s[0] * m[0] - s[1] * m[1] ; //real part
         m[1] = s[1] * m[0] + s[0] * m[1] ; //imaginary part
         m[0] = rr;
         m+=2;
         s+=2;
      }
      //note: no jump over padding
   }

   //transform back to image space : correlation
   fftw_execute(_ccplan);

   //normalize correlation (for unnormalized FFT in FFTW + local variance of slave)
   // MASTER cannot be flat because of feature detection (std=0)
   // use only parts where MASTER is fully included in SLAVE
   double invmpix= 1.0 / ((double)_mx*_my);
   double cnorm  = invmpix / (_mstd * (double)_cy*_cx); //cx*cy is due to unnormalized FFT in FFTW //TBC
   int psi,pei,psj,pej;
   double wsum,wstd;
   double maxncor=-2000000;
   int maxy=-1;
   int maxx=-1;
   int startx = _mx-1;
   int starty = _my-1;
   int endx   = _sx-1;
   int endy   = _sy-1;
   double* pcc   = _NCC.refBuffer() + startx + starty * _pcx;

   for(i=starty;i<endy;++i) 
   {
      for(j=startx;j<endx;++j)
      {
         //get std & average for master window over PADDED slave
         psi=i-_my; 
         pei=i; //master position start& end relatively to slave
         psj=j-_mx; 
         pej=j;         
         wsum =           (_srs->getRS (pei,pej) + _srs->getRS (psi,psj) - _srs->getRS (pei,psj) - _srs->getRS (psi,pej)) * invmpix;
         wstd = std::sqrt((_srs->getRS2(pei,pej) + _srs->getRS2(psi,psj) - _srs->getRS2(pei,psj) - _srs->getRS2(psi,pej)) * invmpix - wsum*wsum);

         if (wstd > 1e-13)
         {
            *pcc = *pcc * cnorm / wstd;
         } else {
            *pcc = 0.0;
         }
         //keep max index
         if (maxncor<*pcc) 
         {
            maxncor=*pcc;
            maxy=i; 
            maxx=j;
         }
         ++pcc;
      }

      pcc+=_pcx-(_sx-_mx);
   }
   _maxy   = maxy;
   _maxx   = maxx;
   _maxncc = maxncor;
   return true;
}

std::string
ossimNCC_FFTW::getWisdom()const
{
   char* ws=fftw_export_wisdom_to_string();
   if (ws)
   {
      std::string s(ws);
      fftw_free(ws);
      return s;
   }
   return std::string();
}

void
ossimNCC_FFTW::moments1(int length, const double* table, double* avg, double* std)const
{
   if ((length<=0) || (table==NULL)) return;

   int i;
   double t;
   double sum  = 0.0;
   double sum2 = 0.0;
   for (i=0;i<length;++i)
   {
      t=*(table++);
      sum  += t;
      sum2 += t*t;
   }
   sum  /= length;
   sum2 /= length;

   //update output parms
   if (avg)
   {
     *avg = sum;
   }
   if (std)
   {
      *std = std::sqrt(sum2-sum*sum);
   }
}
