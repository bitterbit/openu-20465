#include <stdio.h>

#ifndef H_TEXT
#define H_TEXT

struct TextImpl {
    int (*readText)();
    int (*printText)();
};

typedef struct TextImpl TextImpl;

size_t readStdin(char* buf, size_t max);

#endif
