#include <string.h>
#include <ctype.h>
#include <stdio.h>

int main()
{
	char s[1024];
	while (fgets(s, sizeof(s), stdin))
	{
		s[sizeof(s) - 1] = 0;
		char* p = s;
		while (isspace(*p))
			++p;
		if (strncmp(p, "isc_", 4) == 0)
			fputs(s, stdout);
	}

	return 0;
}

