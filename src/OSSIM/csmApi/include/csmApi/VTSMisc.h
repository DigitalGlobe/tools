//#############################################################################
//
//    FILENAME:   VTSMisc.h
//
//    CLASSIFICATION:    Unclassified
//
//    DESCRIPTION:
//
//    Header for the constants and other definitions used by the TSM VTS.
//
//    LIMITATIONS:       None
// 
//                       Date          Author   Comment    
//    SOFTWARE HISTORY:  13-Jan-2004   LMT      Initial version.
//
//    NOTES:
//
//#############################################################################
#ifndef __VTSMISC_H
#define __VTSMISC_H
#ifdef _WIN32
#pragma warning( disable : 4290 )
#endif
#include "csmApiExport.h"
#include "TSM/TSMMisc.h"
#include "TSM/TSMError.h"
#include "TSM/TSMWarning.h"
#include "TSM/TSMISDFilename.h"
#include "TSM/TSMISDByteStream.h"
#include "TSM/TSMISDNITF20.h"
#include "TSM/TSMISDNITF21.h"

#include <vector>
#include <list>
#include <string>

static const int MAX_INPARAM_LENGTH=MAX_NAME_LENGTH;
static const int MAXINPUTPARAMETERS=50;
//      static const int MAXINPUTPARAMETERS=79;
//      static const int MAXOUTPUTPARAMETERS=8;
static const int MAXOUTPUTPARAMETERS=18;
static const int MAX_OUTPARAM_LENGTH=MAX_NAME_LENGTH;
static const int MAX_ACT=MAXOUTPUTPARAMETERS;    // Maximum number of
						 // entries in the
						 // actual value array
						 // (to compare)
static const int MAX_ACT_SIZE=MAX_OUTPARAM_LENGTH;  // Maximum
						    // elemental width
						    // of alpha text
						    // to compare
static const int MAX_PARAM_LENGTH=512;
static const int MAX_COMMAND_LENGTH=512;
static const int MAX_PLUGIN_NUMBER=256;
const int preSet_COMMAND_LENGTH=256;


class CSMAPI_DLL SegmentlengthInfo
{
 public:
  int seghdrlength;
  int segdatalength;
};
    
enum ISDType    
  {  NO_FILE_TYPE,
     NITF_20_TYPE,
     NITF_21_TYPE,
     BYTESTREAM_TYPE,
     FILENAME_TYPE
  }; 

typedef std::list < std::string >      NameList;

CSMAPI_DLL TSMWarning * printList(char *logFile);
// pre: None.
// post: The list of all currently-registered factories has been printed.

CSMAPI_DLL TSMWarning * makeModelListFromISD(const std::string isd_name, const tsm_ISD * isd)
     throw (TSMError);
// pre: None.
// post: A subset of aList has been printed; the subset contains all
//    factories that can support the given isd.


CSMAPI_DLL TSMWarning * listSupportingModels(const std::string isd_name,
				  const tsm_ISD* isd,
				  NameList& names);
// pre: None.
// post: For each model currently registered, the model's name is
//    appended to aNameList if the model supports the given isd.

CSMAPI_DLL void compareParam(char param_array[MAXINPUTPARAMETERS][MAX_PARAM_LENGTH],
                  int param_array_index,
                  int CompNumIndex,
                  double *act_val,
                  double *comp_answer,
                  int *pass_fail);

CSMAPI_DLL void compareParam(char param_array[MAXINPUTPARAMETERS][MAX_PARAM_LENGTH],
		  int param_array_index,
		  int CompNumIndex,
		  char act_val[MAX_ACT][MAX_ACT_SIZE],
		  int *pass_fail);

CSMAPI_DLL void recordLog(char* datafile,
               char* serialNum,
               char* timedata,
               char* comm,
               char* command,         
	       char param_array[MAXINPUTPARAMETERS][MAX_PARAM_LENGTH],
               char out_param_array[MAXOUTPUTPARAMETERS*3][MAX_OUTPARAM_LENGTH]);

CSMAPI_DLL void recordLog(char* datafile,
               char* serialNum,
               char* timedata,
               char* comment,
               char* command,
               char* text);

CSMAPI_DLL void recordLog(char* datafile,
               char* serialNum,
               char* timedata,
               char* comment,
               char* command,
               char param_array[MAXINPUTPARAMETERS][MAX_PARAM_LENGTH],
               char* text);

CSMAPI_DLL void recordLog(char* datafile,
               char* comment,
               char* text);

CSMAPI_DLL void recordLog(char* datafile,
               char* text);

CSMAPI_DLL void reportError (TSMError *err, char *logFile);

// prototypes for ISD "constructors"
CSMAPI_DLL void initFilenameISD (filenameISD *isdfilename, 
		      std::string isd);

CSMAPI_DLL void initBytestreamISD(bytestreamISD *bytestream, std::string filename)  
     throw (TSMError);

CSMAPI_DLL TSMWarning *initNITF20ISD (NITF_2_0_ISD *isd, std::string fname)
     throw (TSMError);

CSMAPI_DLL TSMWarning *initNITF21ISD (NITF_2_1_ISD  *isd, std::string filename)
     throw (TSMError);

CSMAPI_DLL void fillBuff (std::string fname) throw (TSMError);

CSMAPI_DLL TSMWarning *parseFile(NITF_2_0_ISD *isd) throw (TSMError);

CSMAPI_DLL TSMWarning *parseFile(NITF_2_1_ISD *isd) throw (TSMError);

CSMAPI_DLL void parseImages (NITF_2_0_ISD *isd, 
		  const int hl, 
		  const std::vector <int> imagehdrlengths, 
		  const std::vector <int>imagelengths);

CSMAPI_DLL void parseImages (NITF_2_1_ISD *isd, 
		  const int hl, 
		  const std::vector <int> imagehdrlengths, 
		  const std::vector <int>imagelengths);

CSMAPI_DLL void parseTRE(int tlen, // length of tre data
	      std::string treData,  //  input tre data
	      int *numTRE, // output numtres and file/image tre data
	      tre **Tre)   throw (TSMError);

CSMAPI_DLL void parseDES(NITF_2_0_ISD *isd, 
		  const int des_offset, 
		  const std::vector <int> hdrlengths, 
		  const std::vector <int>datalengths);

CSMAPI_DLL void parseDES(NITF_2_1_ISD *isd, 
		  const int des_offset, 
		  const std::vector <int> hdrlengths, 
	      const std::vector <int>datalengths);

CSMAPI_DLL void getSegment( const int offset,
		 const int image_index,
		 const std::vector <int> hdrlengths, 
		 const std::vector <int> datalengths)  throw (TSMError);

CSMAPI_DLL void dumpHdr(NITF_2_0_ISD *isd);

CSMAPI_DLL void dumpHdr(NITF_2_1_ISD *isd);

#endif // VTSMISC_H

     
