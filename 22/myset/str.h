#include "bool.h"

#ifndef _H_STR_
#define _H_STR_
bool isNumber(char *s);
char* skipSpace(char *s);
bool hasComma(char *s);
bool isEmpty(char *s);
char* lineSkipToParams(char *line);
char* skipLeadingSpace(char* s);
void nullifyTrailingSpace(char* s);
#endif
