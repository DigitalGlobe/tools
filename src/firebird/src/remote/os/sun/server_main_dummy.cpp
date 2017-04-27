#include "firebird.h"
#include "../jrd/common.h"

extern "C" {
int CLIB_ROUTINE server_main( int argc, char** argv)
{
/*NOOP: this routine is needed to implement
	server_main as it's defined by firebird.vers mapfile
	linking libfbclient with Sun-like LD
*/
	return 0;
}
}
