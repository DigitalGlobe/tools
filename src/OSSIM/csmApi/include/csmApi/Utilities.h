#ifndef UTILS_H
#define UTILS_H
#include "csmApiExport.h"

#pragma warning(disable: 4786)
using namespace std;

#ifdef WIN32
std::string separator = "\\";
bool isSolaris = false;
#else
std::string separator = "/";
bool isSolaris = true;
#endif

bool debug = false;

//
CSMAPI_DLL bool stripPixels(FILE* fp, const char* filename, const char* newFilename);
bool createMetaDataFile(const char* filename, const char* newfilename);

// structure to hold a directory and all its filenames.
struct CSMAPI_DLL FILELIST
{
	string path;
	vector<string> theList;
};

CSMAPI_DLL void cleanTempDirectory(string path, list<FILELIST>& theList);

#endif