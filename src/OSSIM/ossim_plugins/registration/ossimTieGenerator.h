// class TieGenerator
//
// features:
// execution gets all tie points using a ossimChipMatch object, given
// -a specific resoultionLevel
// -a specific rectangular ROI (default : everything in bounds)
// => it is a global operator
//
// can export tie points to a file (optional)
// tie points are provided by a ossimChipMatch object, using tiled method getFeatures()
//
// created by Frederic Claudel, CSIR - Aug 2005 - using ossimVertexExtractor as a model
//
// TBD: parallelize : use a sequencer (for vector data as well)
//

#ifndef ossimTieGenerator_HEADER
#define ossimTieGenerator_HEADER

#include <fstream>

#include <ossim/base/ossimOutputSource.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimProcessProgressEvent.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimTDpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimImageSource.h>
#include "ossimChipMatch.h"
#include "ossimRegistrationExports.h"
class ossimImageSource;

//! Class ossimTieGenerator

/*!
 *  get all tie points from ossimChipMatch using a tiled vector method : getFeatures()
 */
class OSSIM_REGISTRATION_DLL ossimTieGenerator : 
    public ossimOutputSource,
    public ossimProcessInterface
{
public:
   ossimTieGenerator(ossimImageSource* inputSource=NULL);
   virtual ~ossimTieGenerator();

   inline bool getStoreFlag()const   { return theStoreFlag; }
   inline void setStoreFlag(bool sf) { theStoreFlag = sf; }

   virtual       ossimObject* getObject()      { return this; }
   virtual const ossimObject* getObject()const { return this; }
   virtual       ossimObject* getObjectInterface() { return this; }

   void setAreaOfInterest(const ossimIrect& rect);
   virtual void setOutputName(const ossimString& filename);
   virtual bool isOpen() const;
   virtual bool open();
   virtual void close();

   virtual bool execute();

   virtual ossimListenerManager* getListenerManagerInterface() { return this; }   
   
/*   virtual void setPercentComplete(double percentComplete)
      {
         ossimProcessInterface::setPercentComplete(percentComplete);
         ossimProcessProgressEvent event(this,
                                         percentComplete);
         fireEvent(event);      
      }
*/
   bool canConnectMyInputTo(ossim_int32 inputIndex,
                            const ossimConnectableObject* object)const
      {         
         return (object&& PTR_CAST(ossimChipMatch, object));
      }

   const vector<ossimTDpt>& getTiePoints() { return theTiePoints; }

protected:
   bool getAllFeatures();
   void writeTiePoints(const vector<ossimTDpt>& tp);

private:
   ossimIrect        theAreaOfInterest;
   ossimFilename     theFilename;
   std::ofstream     theFileStream;
   vector<ossimTDpt> theTiePoints;
   bool              theStoreFlag;

   //! Disallow copy constructor and operator=
   ossimTieGenerator(const ossimTieGenerator&) {}
   const ossimTieGenerator& operator=(const ossimTieGenerator& rhs)
      {return rhs;}

TYPE_DATA
};

#endif
