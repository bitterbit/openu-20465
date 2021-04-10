#include "text.h"

#ifndef H_TEXT_LINKED_LIST
#define H_TEXT_LINKED_LIST

int linkedListReadText();
void linkedListPrintText();

TextImpl TextLinkedListImpl = {
    .readText = linkedListReadText,
    .printText = linkedListPrintText,
};

#endif
