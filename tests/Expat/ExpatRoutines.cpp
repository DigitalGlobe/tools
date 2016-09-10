#include "ExpatRoutines.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <expat.h>


void start(void *userData, const char *name, const char *args[])
{
	printf("%s : ", name);
}

void end(void *userData, const char *name)
{

}

void value(void *userData, const char *val, int len)
{
	int I;
	char cpy[128] = {};

	/* val is readonly and not NULL terminated but the length *
	* is given in len hence string of length len is copied  *
	* and NULL terminated                                   */
	for (I = 0; I < len; I++)
		cpy[I] = val[I];
	cpy[I] = 0;

	printf("%s", cpy);
}


int ExpatRoutines::parseXMLfile() {

	XML_Parser parser = XML_ParserCreate(NULL);

	XML_SetElementHandler(parser, start, end);

	XML_SetCharacterDataHandler(parser, value);

	int len = 0;
	char val[1024];
	FILE *fh = fopen("Catalog.xml", "r");

	while (len = fread(val, sizeof val, sizeof(char), fh))
	{
		if (0 == XML_Parse(parser, val, len, len < 1024))
		{
			int code = XML_GetErrorCode(parser);
			const char *msg = (const char *)XML_ErrorString((XML_Error)code);
			fprintf(stderr, "Parsing error code %d message %s\n", code, msg);
			break;
		}
	}

	fclose(fh);

	XML_ParserFree(parser);

	return 0;
}