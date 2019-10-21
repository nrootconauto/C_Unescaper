#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "numberParser.h"
int getDecimalDigits(char* text) {
	char* original=text;
	while(*text>='0'&&*text<='9')
		text++;
	return (text-original)/sizeof(char);
}
int getOctalDigits(char* text) {
	char* original=text;
	while(*text>='0'&&*text<='7')
		text++;
	return (text-original)/sizeof(char);
}
int getHexidecimalDigits(char* text) {
	char* original=text;
	for(;;) {
		if(*text>='0'&&*text<='9') {
		} else if(*text>='a'&&*text<='f') {
		} else if(*text>='A'&&*text<='F') {
		} else {
			break;
		}
		text++;
	}
	return (text-original)/sizeof(char);
}
unsigned long int numberParserParseUInt(char* text,int* length) {
	int len=0;
	int offset;
	unsigned long int retVal=0;
	if(0==strncmp(text,"0x",2)) {
		len=getHexidecimalDigits(text+2);
		sscanf(text+2,"%lx",&retVal);
		offset=2;
	} else if(0==strncmp (text,"0b",2)) {
		offset=2;
		unsigned long int value=0;
		while(text[offset]=='0'||'1'==text[offset])
			offset++;
		int count=offset-1;
		while(count!=2-1)
			value|=(text[count--]-'0')<<((offset-count)-2);
		if(length!=NULL)
			*length=offset;
		return value;
	} else if(0==strncmp(text,"0",1)) {
		len=getOctalDigits(text);
		sscanf(text,"%lo",&retVal);
		offset=0;
	} else {
		len=getDecimalDigits(text);
		sscanf(text,"%lu",&retVal);
		offset=0;
	}
	if(length!=NULL)
		*length=offset+len;
	return retVal;
}
signed long int numberParserParseInt(char* text,int* length) {
	signed long int sign=1;
	if(*text=='-') {
		sign=-1;
		text++;
	}
	signed long int value=numberParserParseUInt(text,length);
	value*=sign;
	if(length!=NULL&&sign==-1)
		*length=1+*length;
	
	return value;
}
typedef int(*digitGetter)(char*);
double numberParserParseDouble(char* text,int* length) {
	bool hexOrDex=false;
	int offset=0;
	digitGetter getter;
	if(0==strncmp(text,"0x",2)) {
		hexOrDex=true;
		getter=getHexidecimalDigits;
	} else
		getter=getDecimalDigits;
	int p1;
	numberParserParseUInt(text+offset,&p1);
	offset+=p1;
	bool hasDot=false;
	if(text[offset]=='.') {
		offset++;
		hasDot=true;
		offset+=getter(text+offset);
	}
	char expUpper=(!hexOrDex)?'e':'P';
	char expLower=(!hexOrDex)?'e':'p';
	bool hasExp=false;
	if(expLower==text[offset]||expUpper==text[offset]) {
		hasExp=true;
		offset++;
		if(text[offset]=='-')
			offset++;
		offset+=getter(text+offset);
	}
	if(!hasDot&&!hasExp) {
		//not a float
		if(length!=NULL)
			*length=offset;
		return NAN;
	}
	*length=offset;
	double retVal;
	sscanf(text,"%lf",&retVal);
	return retVal;
}