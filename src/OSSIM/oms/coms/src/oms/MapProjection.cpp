#include <oms/MapProjection.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimTransMercatorProjection.h>
#include <ossim/base/ossimEllipsoidFactory.h>

class oms::MapProjection::PrivateData
{
public:
   virtual ~PrivateData()
   {
   	m_mapProjection = 0;
   }
   ossimRefPtr<ossimMapProjection> m_mapProjection;
};


oms::MapProjection::MapProjection()
:m_privateData(new PrivateData())
{

}
oms::MapProjection::~MapProjection()
{
	if(m_privateData)
	{
		delete m_privateData;
		m_privateData = 0;
	}
}

void oms::MapProjection::createUtmProjection(const ossimGpt& worldPt)
{
	ossimRefPtr<ossimUtmProjection> utmProj = new ossimUtmProjection();
	utmProj->setZone(worldPt);

	m_privateData->m_mapProjection = utmProj.get();
}
void oms::MapProjection::createTransMercProjection(const ossimGpt& worldPt)
{
	ossimRefPtr<ossimTransMercatorProjection> proj = new ossimTransMercatorProjection(*ossimEllipsoidFactory::instance()->wgs84(),
																										   worldPt);

	m_privateData->m_mapProjection = proj.get();
}

bool oms::MapProjection::worldToLocal(const ossimGpt& worldPt, ossimDpt& localPt)const
{
	bool result = false;
	
	if(m_privateData->m_mapProjection.valid())
	{
		localPt = m_privateData->m_mapProjection->forward(worldPt);

		result = true;
	}

	return result;
}

bool oms::MapProjection::localToWorld(const ossimDpt& localPt, ossimGpt& worldPt)const
{
	bool result = false;
	
	if(m_privateData->m_mapProjection.valid())
	{
		worldPt = m_privateData->m_mapProjection->inverse(localPt);

		result = true;
	}

	return result;
}
