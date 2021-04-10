#include "text.h"

#ifndef H_TEXT_REALLOC
#define H_TEXT_REALLOC

int reallocReadText();
int reallocPrintText();

TextImpl TextReallocImpl = {
    .readText = reallocReadText,
    .printText = reallocPrintText,
};

#endif
