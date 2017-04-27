// CSMOutput.h
//
// These are some static helper functions for printing out certain TSM data types to a stream.
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef CSM_OUTPUT_H
#define CSM_OUTPUT_H

#include <iostream>
#include "TSM/TSMWarning.h"
#include "TSM/TSMError.h"


class CSMAPI_DLL GSETCSMOutput 
{
	public:

		static void printWarning( std::ostream& /*pOutput*/, TSMWarning* /*pWarning*/ ) {} 

		static void printError( std::ostream& pOutput, TSMError* pError ) {

			pOutput << "ERROR occured: ";
			switch( pError->getError() ) {
				case TSMError::ALGORITHM:                       pOutput << "ALGORITHM -\n";                       break;
				case TSMError::BOUNDS:                          pOutput << "BOUNDS -\n";                          break;
				case TSMError::FILE_READ:                       pOutput << "FILE_READ -\n";                       break;
				case TSMError::FILE_WRITE:                      pOutput << "FILE_WRITE -\n";                      break;
				case TSMError::ILLEGAL_MATH_OPERATION:          pOutput << "ILLEGAL_MATH_OPERATION -\n";          break;
				case TSMError::INDEX_OUT_OF_RANGE:              pOutput << "INDEX_OUT_OF_RANGE -\n";              break;
				case TSMError::INVALID_SENSOR_MODEL_STATE:      pOutput << "INVALID_SENSOR_MODEL_STATE -\n";      break;
				case TSMError::INVALID_USE:                     pOutput << "INVALID_USE -\n";                     break;
				case TSMError::ISD_NOT_SUPPORTED:               pOutput << "ISD_NOT_SUPPORTED -\n";               break;
				case TSMError::MEMORY:                          pOutput << "MEMORY -\n";                          break;
				case TSMError::SENSOR_MODEL_NOT_CONSTRUCTIBLE:  pOutput << "SENSOR_MODEL_NOT_CONSTRUCTIBLE -\n";  break;
				case TSMError::SENSOR_MODEL_NOT_SUPPORTED:      pOutput << "SENSOR_MODEL_NOT_SUPPORTED -\n";      break;
				case TSMError::STRING_TOO_LONG:                 pOutput << "STRING_TOO_LONG -\n";                 break;
				case TSMError::UNKNOWN_ERROR:                   pOutput << "UNKNOWN_ERROR -\n";                   break;
				case TSMError::UNSUPPORTED_FUNCTION:            pOutput << "UNSUPPORTED_FUNCTION -\n";            break;
				case TSMError::UNKNOWN_SUPPORT_DATA:            pOutput << "UNKNOWN_SUPPORT_DATA -\n";            break;
            default:
               break;
			};

			pOutput << "\t" << pError->getFunction() << ": " << pError->getMessage() << std::endl;

		} // printError()

	private:
		GSETCSMOutput();

};


namespace CSMCallWrapper {
  static void call( TSMWarning* pWarning ) {
		if( pWarning ) {
			GSETCSMOutput::printWarning( std::cout, pWarning );
			std::cout << std::endl;
			delete pWarning;
		}
	}
};


#endif

