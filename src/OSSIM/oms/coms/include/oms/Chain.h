/******************************
 *
 * License: LGPL
 * Author:  Garrett Potts
 *
 *****************************/

#ifndef omsChain_HEADER
#define omsChain_HEADER
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimGrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/projection/ossimImageViewTransform.h>
#include <oms/Constants.h>
#include <oms/Object.h>

class ossimImageSource;

namespace oms
{
   class OMSDLL Chain : public oms::Object
	{
	public:
		Chain();
		virtual ~Chain();
		
      bool loadChainKwl(const ossimKeywordlist& kwl);
      bool loadChainKwlString(const std::string& kwlString);
      std::string toString();
      
      ossimConnectableObject* getChain();
      const ossimConnectableObject* getChain()const;
      ossimImageSource* getChainAsImageSource();
      const ossimImageSource* getChainAsImageSource()const;
      bool executeChain();
      void deleteChain();
      void initialize();
      void connectMyInputTo(Chain* obj);
      void print()const;
   private:
      
      ossimRefPtr<ossimConnectableObject>      m_rootChain;
      
      /**
       * We will allow the filter to be sharpen in image or view space
       * depending on how it is set and used.
       */
	};
}
#endif
