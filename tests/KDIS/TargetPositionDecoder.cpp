//-----------------------------------------------------------------------------
/*
 * TargetPositionDecoder.cpp
 */
//-----------------------------------------------------------------------------

#include "TargetPosition.h"
#include "TargetPositionDecoder.h"
#include <assert.h>

using namespace KDIS;
using namespace KDIS::DATA_TYPE;

//-----------------------------------------------------------------------------
/**
 * Constructs this decoder as a default decoder.
 */
CTargetPositionDecoder::CTargetPositionDecoder() : FactoryDecoder<VariableDatum>()
{
}
//-----------------------------------------------------------------------------
/**
 * Destructs this decoder.
 */
CTargetPositionDecoder::~CTargetPositionDecoder()
{
}
//-----------------------------------------------------------------------------
/**
 * Do KDIS decoding.
 *
 * @param   enumVal
 * @param   stream
 * @return 
 */
VariableDatum* CTargetPositionDecoder::FactoryDecode( KINT32       enumVal ,
                                                      KDataStream& stream  )
{
    VariableDatum* pTargetPosition = nullptr;

    if (enumVal == CTargetPosition::DEFAULT_DATUM_ID)
    {
        pTargetPosition = new CTargetPosition(stream);
        assert(pTargetPosition != nullptr);
    }

    return pTargetPosition;
}
//-----------------------------------------------------------------------------
