#include "UriParseRoutines.h"
#include "stdio.h"


unsigned char UriParseRoutines::uriGetOctetValue(const unsigned char * digits, int digitCount)
{
	switch (digitCount) {
	case 1:
		return digits[0];

	case 2:
		return 10 * digits[0] + digits[1];

	case 3:
	default:
		return 100 * digits[0] + 10 * digits[1] + digits[2];

	}

	
}

unsigned char UriParseRoutines::uriPushToStack(UriIp4Parser * parser, unsigned char digit) {

	switch (parser->stackCount) {
	case 0:
		parser->stackOne = digit;
		parser->stackCount = 1;
		break;

	case 1:
		parser->stackTwo = digit;
		parser->stackCount = 2;
		break;

	case 2:
		parser->stackThree = digit;
		parser->stackCount = 3;
		break;

	default:
		;
	}

	return parser->stackOne;
}

