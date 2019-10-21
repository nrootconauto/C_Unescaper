#ifndef NUMBERPARSER_H
#define NUMBERPARSER_H
#include <stdint.h>
signed long int numberParserParseInt(char* text,int* length);
unsigned long int numberParserParseUInt(char* text,int* length);
double numberParserParseDouble(char* text,int* length);
#endif