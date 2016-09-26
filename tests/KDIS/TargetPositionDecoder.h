//-----------------------------------------------------------------------------
/*
 * TargetPositionDecoder.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <KDIS/DataTypes/FactoryDecoder.h>
#include <KDIS/DataTypes/VariableDatum.h>
#include <KDIS/Extras/PDU_Factory.h>

//-----------------------------------------------------------------------------
/**
 * The <code>CTargetPositionDecoder</code> class represents decoders that
 * decodes datum.
 */
class CTargetPositionDecoder : public KDIS::DATA_TYPE::FactoryDecoder<KDIS::DATA_TYPE::VariableDatum>
{
    public:

        //---------------------------------------------------------------------
        // public constructors

            CTargetPositionDecoder();

        //---------------------------------------------------------------------
        // destructor

            virtual ~CTargetPositionDecoder();

        //---------------------------------------------------------------------
        // overridden/implemented methods

            virtual KDIS::DATA_TYPE::VariableDatum* FactoryDecode( KDIS::KINT32       enumVal ,
                                                                   KDIS::KDataStream& stream  );

    private:

        //---------------------------------------------------------------------
        // private constructors

            CTargetPositionDecoder(const CTargetPositionDecoder&) = delete;

        //---------------------------------------------------------------------
        // private overridden/implemented methods

            const CTargetPositionDecoder& operator=(const CTargetPositionDecoder&) = delete;

};
//-----------------------------------------------------------------------------
