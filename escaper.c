#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "escaper.h"
#include "numberParser.h"
#include <stdbool.h>

//ENDS AT '"'
char* unescapeString(uint8_t* str,uint8_t* where) {
	while(*str!=0) {
		//sorry for long code
		if(*str=='\\') {
			memcpy(where,"\\\\",2);
			where+=2;
			str++;
			continue;
		}
		if(*str=='\a') {
			memcpy(where,"\\a",2);
			where+=2;
			str++;
			continue;
		}
		if(*str=='\b') {
			memcpy(where,"\\b",2);
			where+=2;
			str++;
			continue;
		}
		if(*str=='\f') {
			memcpy(where,"\\f",2);
			str++;
			where+=2;
			continue;
		}
		if(*str=='\n') {
			memcpy(where,"\\n",2);
			str++;
			where+=2;
			continue;
		}
		if(*str=='\r') {
			memcpy(where,"\\r",2);
			str++;
			where+=2;
			continue;
		}
		if(*str=='\t') {
			memcpy(where,"\\t",2);
			str++;
			where+=2;
			continue;
		}
		if(*str=='\v') {
			memcpy(where,"\\v",2);
			str++;
			where+=2;
			continue;
		}
		if(*str=='\?') {
			memcpy(where,"\\?",2);
			where+=2;
			str++;
			continue;
		}
		if(*str=='\"') {
			memcpy(where,"\\\"",2);
			where+=2;
			str++;
			continue;
		}
		//\uHHHH
		const uint8_t twoByteUTF8=0|0b11000000;
		const uint8_t threeByteUTF8=0|0b11100000;
		if((str[0]&0b11100000)==twoByteUTF8||(str[0]&0b11110000)==threeByteUTF8) {
			uint64_t value=0;
			bool flag=false; //if passes 3 bytes
			if((str[0]&0b11110000)==threeByteUTF8) {
				value=str[0]&~0b11110000;
				value<<=6;
				str++;
				value|=str[0]&0b00111111;
				value<<=6;
				str++;
				value|=str[0]&0b00111111;
				flag=true;
			} else {
				value|=str[0]&0b00011111;
				str++;
				value<<=6;//shift for next byte
				value=str[0]&0b00111111;
			}
			uint8_t temp[7];
			sprintf(temp,"\\u%x%x%x%x",
					 (int)((value>>12)&0xf),
					 (int)((value>>8)&0xf),
					 (int)((value>>4)&0xf),
					 (int)((value>>0)&0xf)
					 );
			memcpy(where,temp,6);
			where+=6;
			str++;
			continue;
		}
		//\UNNNNnnnn
		const uint8_t fourByteUTF8=0b11110000;
		if((str[0]&0b11111000)==fourByteUTF8) {
			uint64_t value=0;
			value=str[0]&~fourByteUTF8;
			for(int i=1;i!=4;i++) {
				value<<=6;
				value|=str[i]&0b00111111;
			}
			char temp[2+8+1]; //[\][U][H]*8[null]
			sprintf(temp,"\\U%x%x%x%x%x%x%x%x\x00",
					 (int)((value>>28)&0xf),
					 (int)((value>>24)&0xf),
					 (int)((value>>20)&0xf),
					 (int)((value>>16)&0xf), //4
					 (int)((value>>12)&0xf),
					 (int)((value>>8)&0xf),
					 (int)((value>>3)&0xf),
					 (int)((value>>0)&0xf)//8
					 );
			memcpy(where,temp,2+8);
			where+=2+8;
			str+=4;
			continue;
		}
		//if cant be inputed with a (us) keyboard,escape
		
		const char* valids=" ~!@#$%^&*()_+|}{[]\\;':\",./<>?";
		bool isValid=false;
		if(*str>='a'&&'z'>=*str)
			isValid=true;
		else if(*str>='A'&&'Z'>=*str)
			isValid=true;
		else if(*str>='0'&&*str<='9')
			isValid=true;
		else if(strchr(valids,*str)!=NULL)
			isValid=true;
		if(!isValid) {
			char temp[5];
			sprintf(temp,"%o",str[0]);
			int len=strlen(temp);
			memmove(temp+3-len, temp, len);
			memset(temp, '0', 3-len);
			where[0]='\\';
			memcpy(where+1,temp,4);
			str++;
			where+=4;
			continue;
		}
		*where=*str;
		str++;
		where++;
	}
	return where;
}
