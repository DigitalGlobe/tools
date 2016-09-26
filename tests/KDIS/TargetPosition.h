//-----------------------------------------------------------------------------
/*
 * TargetPosition.h
 */
//-----------------------------------------------------------------------------

#pragma once

#include <KDIS/DataTypes/EntityIdentifier.h>
#include <KDIS/DataTypes/VariableDatum.h>
#include <KDIS/DataTypes/WorldCoordinates.h>

//-----------------------------------------------------------------------------
/**
 * The <code>CTargetPosition</code> class represents target-position datums.
 */
class CTargetPosition : public KDIS::DATA_TYPE::VariableDatum
{
    public:

        //---------------------------------------------------------------------
        // public constants

            //-----------------------------------------------------------------
            /** the default datum ID */
            static const KDIS::KUINT32 DEFAULT_DATUM_ID;
            //-----------------------------------------------------------------

        //---------------------------------------------------------------------
        // public constructors

            CTargetPosition();
            CTargetPosition( KDIS::DATA_TYPE::EntityIdentifier entityID    ,
                             KDIS::DATA_TYPE::WorldCoordinates coordinates );
            CTargetPosition(KDIS::KDataStream& stream);

        //---------------------------------------------------------------------
        // destructor

            virtual ~CTargetPosition();

        //---------------------------------------------------------------------
        // public methods

            void Decode(KDIS::KDataStream& stream) throw (KDIS::KException);
            void Encode(KDIS::KDataStream& stream) const;

    private:

        //---------------------------------------------------------------------
        // private constructors

            CTargetPosition(const CTargetPosition &) = delete;

        //---------------------------------------------------------------------
        // private overridden/implemented methods

            const CTargetPosition& operator=(const CTargetPosition &) = delete;

        //---------------------------------------------------------------------
        // fields

            //-----------------------------------------------------------------
            /** the coordinates of this position */
            KDIS::DATA_TYPE::WorldCoordinates m_coordinates;
            //-----------------------------------------------------------------
            /** the entity identifier of this position */
            KDIS::DATA_TYPE::EntityIdentifier m_entityID;
            //-----------------------------------------------------------------
            /** the padding  */
            KDIS::KUINT16 m_uiPadding;
            //-----------------------------------------------------------------

};
//-----------------------------------------------------------------------------
