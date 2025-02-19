#include <stdlib.h>
#include <stdio.h>

int main()
{
	char *pathvar;
	pathvar = getenv("USER");
	printf("pathvar=%s\n",pathvar);

	pathvar = getenv("PATH");
	printf("pathvares=%s\n",pathvar);
}