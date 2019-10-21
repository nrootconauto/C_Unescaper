#include <stdio.h>
#include <stdlib.h>
#include "numberParser.h"
#include "escaper.h"
int main() {
	//\n works
	char* text="\U0010FFFF";
	char output[256];
	*unescapeString(text,output)=0;
	printf("%s",output);
	return EXIT_SUCCESS;
}