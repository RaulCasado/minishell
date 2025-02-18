#include <stdlib.h>
#include <stdio.h>

int main()
{
	char *pathvar;
	pathvar = getenv("USER");
	printf("pathvar=%s",pathvar);
}