// class for carrying out a series of Normalized Cross Correlations
// makes use of FFTW plans for speed

#ifndef ossimNCC_FFTW_HEADER
#define ossimNCC_FFTW_HEADER

#include <fftw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

class ossimRunningSum;

class ossimNCC_FFTW
{
public:

   //inner class cMatrix for handling double matrices allocated with FFTW
class cMatrix
{
public:
  cMatrix(int sd, int fd) : _buf(NULL), _fd(fd), _sd(sd)
  {
     _buf = (double*)fftw_malloc(sizeof(double)*_fd*_sd);
  }
  virtual ~cMatrix()
  {
     if (_buf!=NULL) {
        fftw_free(_buf);
        _buf = NULL;
     }     
  }
  inline int      fd()const {return _fd;} //fast dimension (column count)
  inline int      sd()const {return _sd;} //slow dimension
  inline double*       refBuffer()      {return _buf;}
  inline const double* getBuffer()const {return _buf;}

  inline double operator()(int fd, int sd)const { return _buf[sd*_fd + fd]; }

  //fill block within matrix with a specified value
  virtual void fill(double f, int left=0,int top=0, int right=-1, int bottom=-1)
  {
     if (right<0)
     {
        right = _fd;
     }
     if (bottom<0)
     {
        bottom = _sd;
     }
     double* p = _buf + left + top*_fd;
     int i,j;
     for(i=top;i<bottom;++i)
     {
        for (j=left;j<right;++j)
        {
           *(p++) = f;
        }
        p+=left+_fd-right;
     }
  }
  
  //text output to stream
  virtual std::ostream& print(std::ostream& os)const
  {
     const double* p = _buf;
     int i,j;
     for(i=0;i<_sd;++i)
     {
        for (j=0;j<_fd;++j)
        {
           os<<std::setprecision(10)<<std::setw(18)<<*(p++)<<"\t";
        }
        os<<std::endl;
     }
     return os;
  }

  //default is to append matrix
  virtual void write(const char* fname,
                      const char* title=NULL,
                      std::ios_base::openmode mode=std::ios_base::app) const
  {
     std::ofstream fs(fname, mode | std::ios_base::out);
     if (fs.is_open())
     {
        if (fs.good())
        {
           if (title)
           {
              fs<<"cMatrix="<<title<<std::endl;
           }
           //do the actual print
           print(fs);
        } else {
           std::cerr<<"cannot successfully open file "<<fname<<" for matrix output"<<std::endl;
        }
        fs.close();
     } else {
        std::cerr<<"cannot open file "<<fname<<" for matrix output"<<std::endl;
     }
  }

protected:
  double*  _buf;
  int _fd;
  int _sd;
};

//end of inner class cMatrix


//constructor allocates buffers and prepares FFTW plans
//can't change dimensions of NCC
//can re-use existing wisdom string if provided
   ossimNCC_FFTW(int cy, int cx, const char* wisdom=0);
   virtual ~ossimNCC_FFTW();

   bool sameDims(int tcy, int tcx)const; //check dimensions

   void ingestMaster(int my, int mx, const double* master);
   void ingestSlave (int sy, int sx, const double* slave);

   //must ingest master and slave first
   virtual bool calculateNCC();

   //master not valid after calculateNCC
   inline const cMatrix& getPRMaster()const      { return _NCC; } //reversed padded
   inline const cMatrix& getPSlave()const      { return _PS; } //padded

   //must calculateNCC() first
   inline const cMatrix& getNcc()const      { return _NCC; }
   inline double         getMaxCorr()const  { return _maxncc; }
   inline int            getMaxCorrX()const { return _maxx; }
   inline int            getMaxCorrY()const { return _maxy; }

   //optional methods
   inline double         getMasterStd()const { return _mstd; }
   inline double         getSlaveStd()const  { return _sstd; }
   inline double         getSlaveAvg()const  { return _savg; }

   //used to keep FFTW wisdom after object destruction
   std::string getWisdom()const;

protected:
   int _mx;
   int _my;
   int _sx;
   int _sy;
   int _cx;
   int _cy;
   int _pcx; //physical cx (padded for R2C inplace tranform)
   cMatrix _NCC; //used for padded master AND NCC
   cMatrix _PS;  //used for padded slave

   fftw_plan _mplan; //stored plans
   fftw_plan _splan;
   fftw_plan _ccplan;

   ossimRunningSum* _srs;
   double _mavg; //master average
   double _mstd; //master standard deviation
   double _savg;
   double _sstd;
   int _maxy; //position for maximum NCC value in the matrix
   int _maxx;
   double _maxncc; //maximum ncc value

   void moments1(int length, const double* table, double* avg, double* std)const;

};

#endif
