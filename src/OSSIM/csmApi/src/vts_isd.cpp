//#############################################################################
//
//    FILENAME:   vts_isd.cpp
//
//
//    DESCRIPTION:
//	This module contains functions to support TSMISD classes
//	These classes are bytestream, filename, NITF 2.0 and NITF 2.1    
//
//    NOTES:
//
//
//#############################################################################
#ifdef _WIN32
#pragma warning( disable : 4290 )
#endif
#include <csmApi/TSM/TSMError.h>
#include <csmApi/TSM/TSMWarning.h>
#include <csmApi/VTSMisc.h>
#include <csmApi/TSM/TSMImageSupportData.h>
#include <csmApi/TSM/TSMISDNITF20.h>
#include <csmApi/TSM/TSMISDNITF21.h>
#include <csmApi/TSM/TSMISDByteStream.h>
#include <csmApi/TSM/TSMISDFilename.h>

#ifdef _WIN32
# include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#ifdef _WIN32
#include <sys\stat.h>
#else
#include <sys/stat.h>
#endif
#include <string.h>

void initFilenameISD (filenameISD *isdfilename, std::string filename)
{
  isdfilename->_filename = filename;
}

void initBytestreamISD(bytestreamISD *bytestream, std::string filename)
  throw (TSMError)
{
	TSMError err;
	FILE * ifp = fopen (filename.c_str(), "rb");
	if (ifp == NULL)
  {
    std::cerr << "Unable to open bytestream " << filename << '\n';
    return;
  }
  const size_t bufflen = 1048576;
  char *buff = new char[bufflen];
  if (!buff)
  {
	  err.setTSMError( TSMError::MEMORY, 
			   "Unable to allocate bytestream buffer", 
			   "initBytestreamISD" );
	  throw err;
  }
  bytestream->_isd = ""; // empty the string
  int numread = 0, totread = 0;
  while ((numread = fread ( buff, 1, bufflen, ifp)) == bufflen)
  {
	totread += numread;
	bytestream->_isd += std::string(buff, bufflen);
  }
  if (numread > 0)
  {
    totread += numread;
    bytestream->_isd += std::string(buff, numread);
  }
  
  bytestream->_isd.erase (totread);
  delete [] buff;
  return;
}

void dumpbytestream (bytestreamISD &bytestream, int len)
{
	std::cout << "\nLength of bytestream: " 
		  << bytestream._isd.length() << '\n';
  std::cout << "First " << len << " characters of ISD Bytestream are \n " 
	    <<  bytestream._isd.substr(1,len) << '\n';
}


//////////////////////
// NITF Section //////
//////////////////////
double dummy;
char *buff;
std::string filename;
FILE *ifile;
struct stat statbuf; // to check for file presence and size

// Fill an existing NITF_2_0_ISD object with data from the specified
// file.  A nearly identical copy of this method will be used to init
// NITF21 objects.

TSMWarning *initNITF20ISD (NITF_2_0_ISD *isd, std::string fname)
  throw (TSMError)
{
  std::string ftype ("NITF20");
  TSMWarning *warn = NULL;

  try {
    fillBuff(fname);
  } catch (TSMError err) {
    switch (err.getError())
    { 
      case TSMError::FILE_READ:
	std::cerr << "Unable to open " << ftype << fname << '\n';
	break;
      case TSMError::MEMORY:
	std::cerr << "Unable to Allocate memory for " 
		  << ftype << " ISD\n";
	break;
      case TSMError::ISD_NOT_SUPPORTED:
	std::cerr << "Failure parsing " 
		  << ftype << " file " 
		  << fname << '\n';
	break;
      default:
	break;
    }
    return NULL;
  }
  try 
  {
    warn = parseFile(isd);
  } 
  catch (TSMError err) 
  {
    switch (err.getError())
    { 
		case TSMError::FILE_READ:
			std::cerr << "Unable to open " << ftype << " file " << fname << '\n';
			break;
		case TSMError::MEMORY:
			std::cerr << "Unable to Allocate memory for " << ftype << " ISD\n";
			break;
		case TSMError::ISD_NOT_SUPPORTED:
			std::cerr << "Failure parsing " << ftype << " file " << fname << '\n';
			break;
		default:
			break;
    }
  }
  if (warn->getMessage().length() > 0) 
    return warn;
  else
    return NULL;
};

TSMWarning *initNITF21ISD (NITF_2_1_ISD *isd, std::string fname)
   throw (TSMError)
{
   std::string ftype ("NITF21");
   TSMWarning *warn = NULL;
   try 
   {
      fillBuff(fname);
   } 
   catch (TSMError err) 
   {
      switch (err.getError())
      { 
		 case TSMError::FILE_READ:
			std::cerr << "Unable to open " << ftype << fname << '\n';
			break;
		 case TSMError::MEMORY:
			std::cerr << "Unable to Allocate memory for " << ftype << " ISD\n";
			break;
		 case TSMError::ISD_NOT_SUPPORTED:
			std::cerr << "Failure parsing " << ftype << " file " << fname << '\n';
			break;
		 default:
			break;
      }
      return NULL;
   }
   try 
   {
      warn = parseFile(isd);
   } 
   catch (TSMError err) 
   {
      switch (err.getError())
      { 
		 case TSMError::FILE_READ:
			std::cerr << "Unable to open " << ftype << " file " << fname << '\n';
			break;
		 case TSMError::MEMORY:
			std::cerr << "Unable to Allocate memory for "<< ftype << " ISD\n";
			break;
		 case TSMError::ISD_NOT_SUPPORTED:
			std::cerr << "Failure parsing " << ftype << " file " << fname << '\n';
			break;
		 default:
			break;
      }
   }
   if (warn->getMessage().length() > 0) 
     return warn;
   else
     return NULL;
};


// Service functions for NITF isd data

// reads an entire image file or the file header (whichever is
// shorter) into a memory buffer
void fillBuff (std::string fname) throw (TSMError)
{
  const int MAXNITFFILEHDRLEN = 999999;

  TSMError tsmerr;
  int buffsize;

  if (stat(fname.c_str(), &statbuf))
  {
    tsmerr.setTSMError (TSMError::FILE_READ, 
			"Unable to locate input file", 
			"TSMNITF20::fillBuff");
    throw (tsmerr);
  }
  buffsize = (MAXNITFFILEHDRLEN > statbuf.st_size) ? 
    statbuf.st_size : MAXNITFFILEHDRLEN;
  buff = (char*) calloc (buffsize, sizeof (char));
  if (buff == NULL)
  {
    tsmerr.setTSMError (TSMError::MEMORY, 
			"Unable to allocate file buffer", 
			"TSMNITF20::fillBuff");
    throw (tsmerr);
  }
  // malloc ok
  ifile = fopen (fname.c_str(), "rb");
  if (! ifile)
  {
    std::string errstr ("Unable to open input file " + fname);
    tsmerr.setTSMError (TSMError::FILE_READ, 
			errstr, 
			"TSMNITF20::fillBuff");
    throw (tsmerr);
  }
  // open ok
  off_t size = fread (buff, 1, buffsize, ifile);
  if (size != buffsize)
  {
    std::string errstr ("failure reading input file " + fname);
    tsmerr.setTSMError (TSMError::FILE_READ, 
			errstr, 
			"TSMNITF20::fillBuff");
    throw (tsmerr);
  }
};

///////////////////////////////////////////////////////////////////////////
// This method parses the data in the file header to derive sizes and
// numbers of each of the file component segments.  It also processes
// any file tre's.   This function is derived from information in
// MIL-STD-2500A, release 3
//
// this function will have to be overloaded for NITF21 isd's
//////////////////////////////////////////////////////////////////////////
TSMWarning *parseFile (NITF_2_0_ISD *isd) throw (TSMError)
{
  TSMError err;
  TSMWarning *warning = new TSMWarning (TSMWarning::UNKNOWN_WARNING, "", "");
  if (!buff) // fillBuff failed -- should never get here
  {
    err.setTSMError (TSMError::INVALID_USE,
		     "No ISD data buffer: fillBuff failed",
		     "parseFile");
    throw (err);
  }
    
  char pbuff[4096];	// parser buffer
  int fl,		// file length
    hl;		        // file header length
  int crypt_offset;
  int class_offset = 119; // location of classification indicator in
			  // file header

  // check for conditional fields
  // if file is classified at any level the class info is 40 bytes longer
  if (buff[class_offset] == 'U')
    crypt_offset = 296;
  else
    crypt_offset = 336;
  // location of file length field in 2.0 file 
  int fl_offset = crypt_offset + 46;
  memcpy (pbuff, &buff[fl_offset], 12); pbuff[12] = '\0';
  fl = atoi(pbuff);
  if (statbuf.st_size != fl)
    warning->setTSMWarning( TSMWarning::UNKNOWN_WARNING, 
			"File length field does not match actual file length", 
			"parseFile(nitf2.0)");
  // get 2.0 file header length 
  int hl_offset = fl_offset + 12;
  memcpy (pbuff, &buff[hl_offset], 6); pbuff[6] = '\0';
  hl = atoi(pbuff);
  if (statbuf.st_size < hl)
  {
    // choke
     err.setTSMError(TSMError::DATA_NOT_AVAILABLE, 
		     "ISD file smaller than indicated header length",
		     "parseFile(nitf2.0)");
     throw err;
  }
  // Copy the file header  
  isd->fileHeader.assign (buff, hl);

////////////////////////////////////////////////////////////////
//////  This section goes through the file 2.0 header      /////
//////  accumulating size information for the various      /////
//////  segments of the file.  Althought at the present    /////
//////  time only image info, DES info and User Defined    /////
//////  Header data (tre) are required for the class, all  /////
//////  of the section length info is retained for         /////
//////  possible future use.                               /////
////////////////////////////////////////////////////////////////
  int seg_index;
  unsigned int des_offset = hl; // This value will be used to point to
				// the location in the 2.0 file of the
				// DES data
  ////////////////////////////////////////////
  // process image segments length info  /////
  ////////////////////////////////////////////

  int numi_offset = hl_offset + 6;
  memcpy (pbuff, &buff[numi_offset], 3); pbuff[3] = '\0';
  int NUMI =  atoi (pbuff);
  isd->numImages = NUMI;
  // std::cout << "num images = " << isd->numImages << '\n';
  int seg_offset = numi_offset + 3;
  
  if (NUMI > 0)
  {
     std::vector <int> imagehdrlengths (NUMI);
     std::vector <int> imagelengths (NUMI);
     for (seg_index = 0; seg_index < isd->numImages; seg_index++)
     {
		memcpy (pbuff, &buff[seg_offset], 6); pbuff[6] = '\0';
		imagehdrlengths.at(seg_index) = atoi (pbuff);
		des_offset += imagehdrlengths.at(seg_index);
		seg_offset += 6;
		memcpy (pbuff, &buff[seg_offset], 10); pbuff[10] = '\0';
		imagelengths[seg_index] = atoi (pbuff);
		des_offset += imagelengths[seg_index];
		seg_offset += 10;
     }

  //////////////////////////////////////////////////
  //// process 2.0 Symbols segments length info ////
  //////////////////////////////////////////////////

  memcpy (pbuff, &buff[seg_offset], 3); pbuff[3] = '\0';
  int NUMS = atoi (pbuff);
  seg_offset += 3;
  if (NUMS > 0)
  {
     std::vector <int> numshdrlengths (NUMS);
     std::vector <int> numslengths (NUMS);
     for (seg_index = 0; seg_index < NUMS; seg_index++)
     {
		memcpy (pbuff, &buff[seg_offset], 4); pbuff[4] = '\0';
		numshdrlengths.at (seg_index) = atoi (pbuff);
		des_offset += numshdrlengths.at (seg_index);
		seg_offset += 4;
		memcpy (pbuff, &buff[seg_offset], 6);  pbuff[6] = '\0';
		numslengths.at (seg_index) = atoi (pbuff);
		des_offset += numslengths.at (seg_index);
		seg_offset += 6;
     }
  }
  ///////////////////////////////////////////////////
  //// process 2.0  Label segments length info   ////
  ///////////////////////////////////////////////////
  memcpy (pbuff, &buff[seg_offset], 3);  pbuff[3] = '\0';
  int NUML = atoi (pbuff);
  seg_offset += 3;
  if (NUML > 0)
  {
     std::vector <int> numLhdrlengths (NUML);
     std::vector <int> numLlengths (NUML);
     for (seg_index = 0; seg_index < NUML; seg_index++)
     {
		memcpy (pbuff, &buff[seg_offset], 4);  pbuff[4] = '\0';
		numLhdrlengths.at (seg_index) = atoi (pbuff);
		des_offset += numLhdrlengths.at (seg_index);
		seg_offset += 4;
		memcpy (pbuff, &buff[seg_offset], 3);  pbuff[3] = '\0';
		numLlengths.at (seg_index) = atoi (pbuff);
		des_offset += numLlengths.at (seg_index);
		seg_offset += 3;
     }
  }
  ////////////////////////////////////////////////////////////
  //// process 2.0 Text Segments length info              ////
  ////////////////////////////////////////////////////////////
  memcpy (pbuff, &buff[seg_offset], 3);  pbuff[3] = '\0';
  int NUMT = atoi (pbuff);
  seg_offset += 3;
  if (NUMT > 0 )
  {
     std::vector <int> numThdrlengths (NUMT);
     std::vector <int> numTlengths (NUMT);
     for (int t_index = 0; t_index < NUMT; t_index++)
     {
		memcpy (pbuff, &buff[seg_offset], 4);  pbuff[4] = '\0';
		numThdrlengths [t_index] = atoi (pbuff);
		des_offset += numThdrlengths [t_index];
		seg_offset += 4;
		memcpy (pbuff, &buff[seg_offset], 5);  pbuff[5] = '\0';
		numTlengths [t_index] = atoi (pbuff);
		des_offset += numTlengths [t_index];
		seg_offset += 5;
     }
  }
  ////////////////////////////////////////////////////////////
  //// process 2.0 Data Extension Segments length info    ////
  ////////////////////////////////////////////////////////////

  memcpy (pbuff, &buff[seg_offset], 3);  pbuff[3] = '\0';
  int NUMD = atoi (pbuff);
  seg_offset += 3;
  std::vector <int> DEShdrlengths (NUMD);
  std::vector <int> DESlengths (NUMD);
  if (NUMD > 0)
     for (seg_index = 0; seg_index < NUMD; seg_index++)
     {
		memcpy (pbuff, &buff[seg_offset], 4);  pbuff[4] = '\0';
		DEShdrlengths [seg_index] = atoi (pbuff);
		seg_offset += 4;
		memcpy (pbuff, &buff[seg_offset], 9);  pbuff[9] = '\0';
		DESlengths [seg_index] = atoi (pbuff);
		seg_offset += 9;
     }
 
  /////////////////////////////////////////////////////////////
  //// process 2.0 Reserved Extension Segments length info ////
  /////////////////////////////////////////////////////////////
  memcpy (pbuff, &buff[seg_offset], 3);  pbuff[3] = '\0';
  int numR = atoi (pbuff);
  seg_offset += 3;
  std::vector <int> numRhdrlengths (numR);
  std::vector <int> numRlengths (numR);
  for (seg_index = 0; seg_index < numR; seg_index++)
  {
    memcpy (pbuff, &buff[seg_offset], 4);  pbuff[4] = '\0';
    numRhdrlengths [seg_index] = atoi (pbuff);
    seg_offset += 4;
    memcpy (pbuff, &buff[seg_offset], 7);  pbuff[7] = '\0';
    numRlengths [seg_index] = atoi (pbuff);
    seg_offset += 7;
  }

  ////////////////////////////////////////////////////////////
  //// process  2.0 UDHD/XHD info                         ////
  ////////////////////////////////////////////////////////////
  //std::cout << "seg_offset " << seg_offset << '\n';
  memcpy (pbuff, &buff[seg_offset], 5);  pbuff[5] = '\0';
  int lenUDHD = atoi (pbuff);
  seg_offset += 5;
  seg_offset += 3; // added to get past currently unused UDHOFL field
  // store file header UD tre's in string
  std::string UDHD;
  if (lenUDHD > 0)
    UDHD.assign (&buff[seg_offset], lenUDHD);

  seg_offset += lenUDHD -3;
  memcpy (pbuff, &buff[seg_offset], 5);  pbuff[5] = '\0';
  int lenXHD = atoi (pbuff);
  seg_offset += 5;
  // store file header UD tre's in string
  std::string XHD;
  if (lenXHD > 0)
    XHD.assign (&buff[seg_offset], lenXHD);
  
  std::string HD = UDHD + XHD;
  unsigned int treLen = lenUDHD + lenXHD;
  // parse file tre's
  if (treLen)
    parseTRE (treLen,  HD, &isd->numTREs, &isd->fileTREs);

  ////////////////////////////////////////////
  /////// End of fileheader code  ////////////
  ////////////////////////////////////////////
  seg_offset += treLen;  // cue up to begining of first image group

  if (seg_offset != hl)
    {
      std::stringstream s;
      s << "Parser error in file header -- hl = " << hl << " offset = " << seg_offset << '\n';
//      exit (666);
          err.setTSMError (TSMError::INVALID_USE,
         s.str().c_str(),
         "parseFile");
        throw (err);

    }
  /////// Begin 2.0 image sub-header parsing
  if (NUMI > 0)
     parseImages (isd, hl, imagehdrlengths, imagelengths);
  if (NUMD > 0)
     parseDES(isd, des_offset, DEShdrlengths, DESlengths);
  }
  return warning;
}

// the above mentioned overloaded method -- file syntax is derived
// from MIL-STD-2500B release 2
TSMWarning *parseFile(NITF_2_1_ISD *isd) throw (TSMError)
{
  TSMError err;
  TSMWarning *warn = new TSMWarning(TSMWarning::UNKNOWN_WARNING, "", "");
  char parseBuff [13];

  if (!buff) // fillBuff() failed or somebody called me before calling fillBuff() -- should never get here
  {
    err.setTSMError (TSMError::INVALID_USE,
		     "No ISD data buffer: fillBuff failed",
		     "parseFile");
    throw (err);
  }
    
  char pbuff[4096];	// parser buffer
  int fl,		// file length
	  hl;       // file header length.  Required to locate
				// beginning of first image data.

  // locations and field lengths of the FL, HL, and NUMI fields are
  // all at fixed offsets in the 2.1 header.  These values are taken
  // from MIL-STD-2500B Notice 2
  const int FLOFFSET = 342;
  const int FLLEN = 12;
  const int HLOFFSET = 354;
  const int HLLEN = 6;
  const int NUMIOFFSET = 360;
  const int NUMILEN = 3;

  memcpy (parseBuff, &buff[FLOFFSET], FLLEN);
  parseBuff[FLLEN] = '\0';
  fl = atoi (parseBuff);
  if (statbuf.st_size != fl)
    warn->setTSMWarning( TSMWarning::UNKNOWN_WARNING, 
			"File length field does not match actual file length", 
			"parseFile(nitf2.1)");

  memcpy (pbuff, &buff[HLOFFSET], 6); pbuff[6] = '\0';
  hl = atoi(pbuff);
  if (statbuf.st_size < hl)
  {
    // choke
     err.setTSMError(TSMError::DATA_NOT_AVAILABLE, 
		     "ISD file smaller than indicated header length",
		     "parseFile(nitf2.1)");
     throw err;
  }

  memcpy (parseBuff, &buff[HLOFFSET],HLLEN );
  parseBuff[HLLEN] = '\0';
  hl = atoi (parseBuff);
   // Copy the file header  
  isd->fileHeader.assign (buff, hl);

  unsigned int des_offset = hl;  // This value will be used to locate
				 // the DES data in the 2.1 file
//////////////////////////////////////////////////////////////////
////////  This section goes through the 2.1 file header      /////
////////  accumulating size information for the various      /////
////////  segments of the file.  Althought at the present    /////
////////  time only image info, DES info and User Defined    /////
////////  Header data (tre) are required for the class, all  /////
////////  of the section length info is retained for         /////
////////  possible future use.                               /////
//////////////////////////////////////////////////////////////////
  int seg_index;
  int seg_offset = NUMIOFFSET;
  ////////////////////////////////////////////////
  // process 2.1 image segments length info  /////
  ////////////////////////////////////////////////

  memcpy (parseBuff, &buff[seg_offset],NUMILEN );
  parseBuff[NUMILEN] = '\0';
  int NUMI = atoi (parseBuff);
  // std::cout << "**** NUMI = " << NUMI << '\n'; 
  isd->numImages = NUMI;
  std::vector <int> imagehdrlengths (NUMI);
  std::vector <int> imagelengths (NUMI);

  seg_offset = seg_offset + 3;
  for (seg_index = 0; seg_index < isd->numImages; seg_index++)
  {
    memcpy (parseBuff, &buff[seg_offset], 6); parseBuff[6] = '\0';
    imagehdrlengths.at(seg_index) = atoi (parseBuff);
    des_offset += imagehdrlengths.at(seg_index);
    seg_offset += 6;
    memcpy (parseBuff, &buff[seg_offset], 10); parseBuff[10] = '\0';
    imagelengths[seg_index] = atoi (parseBuff);
    des_offset += imagelengths[seg_index];
    seg_offset += 10;
  }

  // Because the number of entries in each of the 2.1 header/data
  // segment length fields varies as the number of segments there are
  // no fixed offsets.
  //////////////////////////////////////////////////
  //// process 2.1 Graphic segments length info ////
  //////////////////////////////////////////////////
  memcpy (parseBuff, &buff[seg_offset], 3);  parseBuff[3] = '\0';
  int NUMS = atoi (parseBuff);
  // std::cout << "**** NUMS = " << NUMS << '\n'; 
  seg_offset += 3;
  if (NUMS > 0)
  {
     std::vector <int> numShdrlengths (NUMS);
     std::vector <int> numSlengths (NUMS);
     for (seg_index = 0; seg_index < NUMS; seg_index++)
     {
		memcpy (parseBuff, &buff[seg_offset], 4);  parseBuff[4] = '\0';
		numShdrlengths.at (seg_index) = atoi (parseBuff);
		des_offset += numShdrlengths.at (seg_index);
		seg_offset += 4;
		memcpy (parseBuff, &buff[seg_offset], 3);  parseBuff[3] = '\0';
		numSlengths.at (seg_index) = atoi (parseBuff);
		seg_offset += 6;
     }
  }
  // There is a 3 space field called NUMX whixh is not currently used
  // by the 2500B standard
  seg_offset += 3;

   ////////////////////////////////////////////////////////////
  //// process 2.1 Text Segments length info               ////
  /////////////////////////////////////////////////////////////
  memcpy (parseBuff, &buff[seg_offset], 3);  parseBuff[3] = '\0';
  int NUMT = atoi (parseBuff);
  // std::cout << "**** NUMT = " << NUMT << '\n'; 
  seg_offset += 3;
  if (NUMT > 0)
  {
     std::vector <int> numThdrlengths (NUMT);
     std::vector <int> numTlengths (NUMT);
     for (int t_index = 0; t_index < NUMT; t_index++)
     {
		memcpy (parseBuff, &buff[seg_offset], 4);  parseBuff[4] = '\0';
		numThdrlengths [t_index] = atoi (parseBuff);
		des_offset += numThdrlengths [t_index];
		seg_offset += 4;
		memcpy (parseBuff, &buff[seg_offset], 5);  parseBuff[5] = '\0';
		numTlengths [t_index] = atoi (parseBuff);
		des_offset += numTlengths [t_index];
		seg_offset += 5;
     }
  }

  ////////////////////////////////////////////////////////////
  //// process 2.1 Data Extension Segments length info    ////
  ////////////////////////////////////////////////////////////

  memcpy (parseBuff, &buff[seg_offset], 3);  parseBuff[3] = '\0';
  int NUMD = atoi (parseBuff);
  // std::cout << "**** NUMD = " << NUMD << '\n'; 
  isd->numDESs = NUMD;
  seg_offset += 3;
  std::vector <int> Dhdrlengths (NUMD);
  std::vector <int> Dlengths (NUMD);
  if (NUMD > 0)
  {
     for (seg_index = 0; seg_index < NUMD; seg_index++)
     {
		memcpy (parseBuff, &buff[seg_offset], 4);  parseBuff[4] = '\0';
		Dhdrlengths [seg_index] = atoi (parseBuff);
		seg_offset += 4;
		memcpy (parseBuff, &buff[seg_offset], 9);  parseBuff[9] = '\0';
		Dlengths [seg_index] = atoi (parseBuff);
		seg_offset += 9;
     }
  }

  ////////////////////////////////////////////////////////////////
  //// process 2.1 Reserved Extension Segments length info    ////
  ////////////////////////////////////////////////////////////////
  memcpy (parseBuff, &buff[seg_offset], 3);  parseBuff[3] = '\0';
  int numR = atoi (parseBuff);
  seg_offset += 3;
  std::vector <int> numRhdrlengths (numR);
  std::vector <int> numRlengths (numR);
  for (seg_index = 0; seg_index < numR; seg_index++)
  {
    memcpy (parseBuff, &buff[seg_offset], 4);  parseBuff[4] = '\0';
    numRhdrlengths [seg_index] = atoi (parseBuff);
    seg_offset += 4;
    memcpy (parseBuff, &buff[seg_offset], 7);  parseBuff[7] = '\0';
    numRlengths [seg_index] = atoi (parseBuff);
    seg_offset += 7;
  }

  ////////////////////////////////////////////////////////////
  //// process  2.1 UDHD/XHD (file tre) info              ////
  ////////////////////////////////////////////////////////////
  // this 5 byte adjustment is also different from the book to the data
  seg_offset += 5;
  // std::cout << "<<<<< seg_offset " << seg_offset << '\n';
  memcpy (parseBuff, &buff[seg_offset], 5);  parseBuff[5] = '\0';
  int lenUDHD = atoi (parseBuff);
  seg_offset += 5;
  seg_offset += 3; // added to get past currently unused UDHOFL field
  // store file header UD tre's in string
  std::string UDHD;
  if ((lenUDHD - 3) > 0)
    UDHD.assign (&buff[seg_offset], lenUDHD - 3);

  seg_offset += lenUDHD - 3 ;
  memcpy (parseBuff, &buff[seg_offset], 5);  parseBuff[5] = '\0';
  int XHDL = atoi (parseBuff);
  seg_offset += 5;

  // store file header X UD tre's in string
  std::string XHD;
  if (XHDL > 0)
    XHD.assign (&buff[seg_offset], XHDL);
  std::string HD;
  if ((XHDL + lenUDHD - 3) > 0)
    HD.assign (UDHD + XHD);
  
  int udidl = XHDL + lenUDHD - 3;
  // if any file tre data parse it
  if (udidl > 0)
    parseTRE (udidl, HD, &isd->numTREs, &(isd->fileTREs));
  if (NUMI > 0)
     parseImages(isd, hl, imagehdrlengths, imagelengths);
  if (NUMD > 0)
     parseDES(isd, des_offset, Dhdrlengths, Dlengths);

  return warn;
};

/////////////////////////////////////////////////////////////////////////
// For each image this method will copy the entire image sub-header
// including the image tre's into the current image header object.
// Then it will find the tre data for the current image and store them
// in the image tre's.  This code is based on MIL-STD-2500A defining
// the NITF 2.0 file format.
//
//  This method will have to be overloaded to process NITF 2.1 data
/////////////////////////////////////////////////////////////////////////
void parseImages (NITF_2_0_ISD * isd, 
		  const int hl, 
		  const std::vector <int> imagehdrlengths, 
		  const std::vector <int> imagelengths)
{
   int numComments;
   int numBands;
   int numLUTS;
   int numLUTEnt;
   int imgptr = hl;
   int buffptr;
   std::string pbuff;
   isd->images = new image[isd->numImages];
   for (int index = 0; index < isd->numImages; index ++)
   {     

      getSegment (imgptr, index, imagehdrlengths, imagelengths);
      // copy image header including image tre's

      int len = imagehdrlengths.at(index);

      isd->images[index].imageSubHeader.assign(buff,len);
						
      // find image tre data The first conditional field switch is the
      // Classification code (at 123).  If not 'U' then there are an
      // additional 206 characters of classification information.
      buffptr = 123;
      //std::cout <<  isd->images[index].imageSubHeader[buffptr] << '\n';
      if (isd->images[index].imageSubHeader[buffptr] != 'U')
      {
		 // there is classification info present. If present, there is
		 // a conditional 40 byte downgrading event field present
		 // depending on what value is in the ISDWNG field
		 pbuff.assign (buff [284], 6);
		 if (!strncmp ( pbuff.c_str(), "999998", 6))
			buffptr += 40;
      }
      buffptr += 167;
      
      // get by next block of unconditional data to Img coord system 
      buffptr += 81; 
      if (isd->images[index].imageSubHeader[buffptr] != 'N')
		 // There are image coordinates
		 buffptr += 60;
      buffptr++; // slide up to number of comments
      numComments = isd->images[index].imageSubHeader[buffptr] - '0';
      int CommentLen = 80;
      buffptr += (numComments * CommentLen + 1);
      // image compression present?
      if (isd->images[index].imageSubHeader[buffptr] != 'N')
		// image compression field required
		buffptr += 4;
      buffptr += 2; // Number of bands
      numBands =  isd->images[index].imageSubHeader[buffptr] - '0';
      buffptr++;
      // accumulate all of the per/band information
      for (int bands = 0; bands < numBands; bands++)
      {
		 // skip first 12 required bytes
		 buffptr += 12;
		 // pass by LUT data for each band
		 numLUTS = isd->images[index].imageSubHeader[buffptr] - '0';
		 buffptr++;
		 if (numLUTS)
		 {
			pbuff = isd->images[index].imageSubHeader.substr(buffptr, 5);
			pbuff.at(5) = 0;
			numLUTEnt = atoi (pbuff.c_str());
			buffptr += numLUTEnt * numLUTS + 5;
		 }
      }
      // bypass more unconditional image info
      buffptr += 45;

      // parse image tre data
      pbuff = isd->images[index].imageSubHeader.substr(buffptr, 5);
      int udidl =  atoi (pbuff.c_str()) - 3;
      int udid_offset = buffptr + 8;
      // if image tre data available parse it
      if (udidl > 0)
		parseTRE (udidl, &buff [udid_offset], &isd->images[index].numTREs, &(isd->images[index].imageTREs));
   }
};

/////////////////////////////////////////////////////////////////////////////
// This parser can be used for both NITF2.0 and NITF2.1 isd's and both
// file and image tre's because at the level that tre's are defined for CSM
// the parser rules are identical.
/////////////////////////////////////////////////////////////////////////////

void parseTRE(int tlen, // length of tre data
	      std::string treData,  //  tre data
	      int *numTRE,
	      tre **Tre)  // output tre objects
   throw (TSMError)
{
   unsigned int parselen = 0;        // index location of next tre
   std::vector <int> trelens (100);  // stores the full length of each tre
   std::string len;
   int treNum = 0;
   // two loops required by class definition -- requiring number of
   // tres before they can be allocated and set
   int trehdrlen = 6 + 5; // tre header length = 6 label chars + 5 len chars
   while (parselen < (unsigned)tlen)
   {
      len = treData.substr (parselen + 6, 5);
      trelens [treNum] = atoi(len.c_str()) + trehdrlen;
      parselen += trelens[treNum];
      treNum++;
   }
   *numTRE = treNum;
   *Tre = new tre [treNum];
   parselen = 0;
   for (int i = 0; i < treNum; i++)
   {
      char *workbuff = (char *) malloc (trelens[i]);     
      std::string str = treData.substr(parselen, trelens [i]).c_str();
      str.copy (workbuff,trelens [i]);
      (*Tre)[i].setTRE (workbuff);
      free (workbuff);
      parselen += trelens[i];
   }
};

void dumpHdr(NITF_2_0_ISD *isd)
{
  int i = 0;
  std::cout << isd->fileHeader << '\n';
  std::cout << "***************** END OF FILE HEADER *************\n";
  std::cout << isd->numTREs << " fileTRE records\n";
  for (i = 0; i < isd->numTREs; i++)
  {
     if ( isd->fileTREs != NULL)
     {
		std::cout << "\trecord " << isd->fileTREs[i].record << '\n';
		std::cout << "\tname   " << isd->fileTREs[i].name << '\n';
		std::cout << "\tlength " << isd->fileTREs[i].length << "\n\n";
     }
  }
  std::cout << isd->numImages << " image(s)\n";
  for (i = 0; i < isd->numImages; i++)
  {
    std::cout << "image sub-header for image " << i + 1 << '\n';
    std::cout << isd->images[i].imageSubHeader << '\n';
    std::cout << "***************** END OF IMAGE HEADER *************\n";
    std::cout  << isd->images[i].numTREs << " TREs in image " 
	       << i + 1 << '\n';
    for (int j = 0; j < isd->images[i].numTREs; j++)
    {
       if (isd->images[i].imageTREs != NULL)
       {
		  std::cout << "\tname   " << isd->images[i].imageTREs[j].name << '\n';
		  std::cout << "\tlength " << isd->images[i].imageTREs[j].length << '\n';
		  std::cout << "\trecord " << isd->images[i].imageTREs[j].record << "**\n";
		  std::cout << "\trecordlen " << strlen(isd->images[i].imageTREs[j].record) << "\n\n";
       }
    }
  }
  for (i = 0; i < isd->numDESs; i ++)
  {
     std::cout << "\nDES hdr " << i + 1 << '\n';
     std::cout << "header length " << isd->fileDESs[i].desShLength << '\n';
     std::cout << "header: " << isd->fileDESs[i].desSh << "\n\n";
     std::cout << "data length " << isd->fileDESs[i].desDataLength << '\n';
     std::cout << "data: " << isd->fileDESs[i].desData << "\n\n";
  }
};

void parseImages (NITF_2_1_ISD * isd, 
		  const int hl, 
		  const std::vector <int> imagehdrlengths, 
		  const std::vector <int> imagelengths)
{
   const int ICORDS_OFFSET = 371;
   const int ICORDS_LEN = 60;
   const int ICOM_LEN = 80;

   // NITF data
   int numComments;
   int numBands;
   int numLUTS;
   int numLUTEnt;
   int UDIDL;
   int UDOFL;
   int IXSHDL;
   int IXSOFL;

   int imgptr = hl;
   char pbuff [10];
   std::string treData;
   int buffptr = ICORDS_OFFSET + 1;
   isd->images = new image[isd->numImages];

   for (int index = 0; index < isd->numImages; index ++)
   {
      getSegment (imgptr, index, imagehdrlengths, imagelengths);
      // copy image header including image tre's
      isd->images[index].imageSubHeader.assign 
	 (buff,imagehdrlengths.at(index));

      // now parse image looking for tre's
      if (buff[ICORDS_OFFSET] != ' ') // 60 byte ICOORDS field present
		buffptr += ICORDS_LEN; 
      // get number of image comments;
      numComments = (int) buff[buffptr] - '0'; buffptr++;
      buffptr += numComments *  ICOM_LEN;
      // evalute image compression type (IC) field to determine
      // presence of ICOM field
      if ((strncmp(&buff[buffptr], "NM", 2)) && (strncmp(&buff[buffptr], "NC", 2)))
		buffptr += 4; // ICOM field is present if IC != NM or NC
      
	  // check if XBANDS field is present
      buffptr += 2;
      if (buff[buffptr] != '0')  // no XBANDS use NBANDS
      {
		numBands = (int) buff[buffptr] - '0';
		buffptr++;
      }
      else // use num XBANDS
      {
		memcpy (pbuff, &buff[buffptr], 5); pbuff[5] = 0;
		numBands = atoi (pbuff);
		buffptr += 5;
      }
      // image band parsing each image band contains required fields
      // plus an optional varying length LUT table.  The LUT
      // information can vary in length from band to band.
      int band_total_length = 0;
      for (int band_index = 0; band_index < numBands; band_index++)
      {
		band_total_length += 13;
		buffptr += 13;
		numLUTS = buff[buffptr] - '0';
		if (numLUTS) // there are LUTS
		{
		  band_total_length += 5;
		  buffptr += 5;
		  memcpy (pbuff, &buff[buffptr], 5); buff[5] = 0;
		  numLUTEnt = atoi (pbuff);
		  buffptr += (numLUTEnt * numLUTS);
		}
      }
      buffptr += 40; // length of all required fields between last of
					 // the band info and the UDIDL field

      // This adjustment is made to make the spec fit the data.  Not a good idea but there it is
      buffptr+= 5;
      memcpy (pbuff, &buff[buffptr], 5); pbuff[5] = 0;
      UDIDL = atoi (pbuff) - 3;   // image tre's
      if (UDIDL > 0) // tre's are present
      {
		 buffptr += 5;
		 memcpy (pbuff, &buff[buffptr], 3); pbuff[3] = 0;
		 UDOFL = atoi (pbuff);
		 buffptr += 3;
		 treData.assign (&buff[buffptr], UDIDL);
		 // buffptr += UDIDL - 3;
		 parseTRE (UDIDL, treData, &isd->images[index].numTREs, &isd->images[index].imageTREs);
      }
      
      // shift file pointer to next image header/data segment
      imgptr += imagehdrlengths.at(index) + imagelengths.at(index);
   } // end of image loop
};

void parseDES (NITF_2_0_ISD * isd, 
		const int offset, // file offset to first DES data
		const std::vector <int> hdrlengths, 
		const std::vector <int> datalengths)

{
  int fptr = offset;
  isd->fileDESs = new des[isd->numDESs];
  char *hdr, *data;
  for (int index = 0; index < isd->numDESs; index++)
  {
    if ((hdr = (char *) calloc (hdrlengths.at(index), 1)) == NULL)
       ; // return error
    if ((data = (char *) calloc (datalengths.at(index), 1)) == NULL)
       ; // return error
    getSegment (fptr, index, hdrlengths, datalengths);
    memcpy (hdr, &buff[0], hdrlengths.at(index));
    memcpy (data, &buff[hdrlengths.at(index)], datalengths.at(index));
    isd->fileDESs[index].setDES (hdrlengths.at(index), hdr, datalengths.at(index), data);
    fptr += hdrlengths.at(index) + datalengths.at(index);
    free (hdr); free (data);
  }
}
void parseDES(NITF_2_1_ISD *isd, 
	      const int des_offset, 
	      const std::vector <int> hdrlengths, 
	      const std::vector <int>datalengths)
{
  parseDES((NITF_2_0_ISD *)isd, des_offset, hdrlengths, datalengths);
};

// fills a buffer with n segments currently image or DES
void getSegment( const int offset,  // where the next seegment is located
		 const int index,
		 const std::vector <int> hdrlengths, 
		 const std::vector <int> datalengths)  throw (TSMError)
{
   TSMError tsmerr;
   int buffsize = hdrlengths[index] + datalengths[index];
   
   if (buff) 
	   free (buff);
   buff = (char *)  malloc(buffsize);
   if (!buff)
      ; // handle error
   // std::cout << buffsize << '\n';
   fseek (ifile, (long) offset, SEEK_SET);
   off_t size = fread (buff, 1, buffsize, ifile);
   if (size != buffsize)
   {
      std::string errstr ("failure reading input file ");
      tsmerr.setTSMError (TSMError::FILE_READ, errstr, "getSegment");
      throw (tsmerr);
  }
}
void dumpHdr(NITF_2_1_ISD *isd)
{
  dumpHdr ((NITF_2_0_ISD *) isd);
}
