#pragma once

typedef struct UriIp4ParserStruct {
	unsigned char stackCount;
	unsigned char stackOne;
	unsigned char stackTwo;
	unsigned char stackThree;
} UriIp4Parser;

class UriParseRoutines
{
public:
	unsigned char uriGetOctetValue(const unsigned char * digits, int digitCount);

	unsigned char uriPushToStack(UriIp4Parser * parser, unsigned char digit);

};