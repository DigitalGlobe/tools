//-----------------------------------------------------------------------------
/*
 * TargetPosition.cpp
 */
//-----------------------------------------------------------------------------

#include "TargetPosition.h"

using namespace KDIS;
using namespace KDIS::DATA_TYPE;

//---------------------------------------------------------------------------
// class constants

    const KUINT32 CTargetPosition::DEFAULT_DATUM_ID = 12345;

//-----------------------------------------------------------------------------
/**
 * Constructs this position as a default position.
 */
CTargetPosition::CTargetPosition() : VariableDatum() ,
                                     m_uiPadding(0)
{
}
//-----------------------------------------------------------------------------
/**
 * Constructs this position using specified parameters.
 *
 * @param   entityID     the entity identifier to use
 * @param   coordinates  the coordinates to use
 */
CTargetPosition::CTargetPosition( EntityIdentifier entityID    ,
                                  WorldCoordinates coordinates )
                                  : VariableDatum()            ,
                                    m_coordinates(coordinates) ,
                                    m_entityID(entityID)       ,
                                    m_uiPadding(0)
{
    m_ui32DatumID      = CTargetPosition::DEFAULT_DATUM_ID;
    m_ui32DatumLength  = (EntityIdentifier::ENTITY_IDENTIFER_SIZE  * 8);
    m_ui32DatumLength += (WorldCoordinates::WORLD_COORDINATES_SIZE * 8);
}
//-----------------------------------------------------------------------------
/**
 * Constructs this position using a specified stream.
 *
 * @param   stream  the stream to use
 */
CTargetPosition::CTargetPosition(KDataStream& stream)
                                 : m_uiPadding(0)
{
    this->Decode(stream);
}
//-----------------------------------------------------------------------------
/**
 * Destructs this position.
 */
CTargetPosition::~CTargetPosition()
{
}
//-----------------------------------------------------------------------------
/**
 * Decodes from a specified stream.
 *
 * @param   stream      the stream from which to decode
 * @throws  KException  if this method failed to decode from the specified
 *                      stream
 */
void CTargetPosition::Decode(KDataStream& stream) throw (KException)
{
    stream >> m_ui32DatumID
           >> m_ui32DatumLength;
    stream >> KDIS_STREAM m_entityID
           >> KDIS_STREAM m_coordinates
           >> m_uiPadding;
}
//-----------------------------------------------------------------------------
/**
 * Encodes to a specified stream.
 *
 * @param   stream  the stream to which to encode
 */
void CTargetPosition::Encode(KDataStream& stream) const
{
    stream << m_ui32DatumID
           << m_ui32DatumLength;
    stream << KDIS_STREAM m_entityID
           << KDIS_STREAM m_coordinates
           << m_uiPadding;
}
//-----------------------------------------------------------------------------
