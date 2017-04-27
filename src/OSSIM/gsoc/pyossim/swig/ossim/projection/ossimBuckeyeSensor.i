/*-----------------------------------------------------------------------------
 * Filename        : ossimBuckeyeSensor.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjection
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimSensorModel.h>
#include <ossim/projection/ossimSmacCallibrationSystem.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimBuckeyeSensor.h>
%}


#ifndef TYPE_DATA
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"


/**
 * The Buckey sensor has a set of simple orientation paramters.  For this model we are using the
 * orientation paramters found in the MetaData_GSTI.txt file typically found in the directory where the
 * imagery sits.
 *
 * There are 2 forms the class takes for keyword lists.  The first allows one to point
 * to a file containing the frame numbers and all the orientation paramters. You will still
 * need to define the Camera information and callibrartion paramters.  The pixel_size, 
 * focal_length and principal point are all in millimeters. 
 *
 * <pre>
 * EXAMPLE 1:
 *
 * type: ossimBuckeyeSensor
 * framemeta_gsti: /data/buckey/01-31@09/01-31@09/10-39-48/FrameMeta_GSTI.txt
 * frame_number: 10001
 * pixel_size: (0.0068,0.0068)
 * focal_length: 211.0211
 * principal_point: (-0.1853,1.2428)
 * smac_radial: -.2165e-3 .4230e-7 -.1652e-11 .2860e-19 .5690e-26
 * smac_decent: -.1483e-6 .1558e-6 -.1464e-18 .1233e-38
 * rect: 0 0 7239 5432
 *
 *
 * EXAMPLE 2:
 * roll: 0.074405
 * pitch: 2.849851
 * yaw: 269.517649
 * platform_position: (34.927014,69.353684,3757.12,WGE)
 * pixel_size: (0.0068,0.0068)
 * focal_length: 211.0211
 * principal_point: (-0.1853,1.2428)
 * smac_radial: -.2165e-3 .4230e-7 -.1652e-11 .2860e-19 .5690e-26
 * smac_decent: -.1483e-6 .1558e-6 -.1464e-18 .1233e-38
 * rect: 0 0 7239 5432
 */
%include "ossim/projection/ossimBuckeyeSensor.h"

#endif
