/***************************************************************************************************
 *
 * $Archive: /Eng/Common/Library/Security/Sdt/include/Sdt/SdtMarker.h $     
 *
 * $Revision: 64 $
 *
 * Description:         
 *        
 ***************************************************************************************************
 * Copyright 2005 - 2006 Macrovision Europe Ltd. All Rights Reserved. 
 * 
 * This software has been provided pursuant to a License Agreement containing
 * restrictions on its use. This software contains valuable trade secrets 
 * and proprietary information of Macrovision Europe Ltd. and is protected 
 * by law. It may not be copied or distributed in any form or medium, 
 * disclosed to third parties, reverse engineered or used in any manner not 
 * provided for in said License Agreement except with the prior written 
 * authorization from Macrovision Europe Ltd. 
 ***************************************************************************************************
 *
 * $NoKeywords: $
 *
 **************************************************************************************************/
#ifndef __SDT_MARKER_H__
#define __SDT_MARKER_H__


#define SDT_MARKER_START    SDT_PLATFORM_START
#define SDT_MARKER_END      SDT_PLATFORM_END

#define SDT_MARKER_NULL     SDT_PLATFORM_NULL
#define SDT_MARKER_ERASE    SDT_PLATFORM_ERASE
#define SDT_MARKER_CHECK    SDT_PLATFORM_CHECK

#define SDT_MARKER_DEFINE_START( Attributes  )  SDT_PLATFORM_DEFINE_START( Attributes  ) 

#define SDT_MARKER_DEFINE_END( )                SDT_PLATFORM_DEFINE_END( ) 

#endif 

