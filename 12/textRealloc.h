#include "text.h"

#ifndef H_TEXT_REALLOC
#define H_TEXT_REALLOC

int reallocReadText();
void reallocPrintText();

TextImpl TextReallocImpl = {
    .readText = reallocReadText,
    .printText = reallocPrintText,
};

#endif
