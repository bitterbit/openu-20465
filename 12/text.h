#include <stdio.h>

#ifndef H_TEXT
#define H_TEXT

#define CHUNK_SIZE 60
#define LINE_WIDTH "60"

#define ERR_MEM -1

struct TextImpl {
    int (*readText)();
    void (*printText)();
};

typedef struct TextImpl TextImpl;

size_t readStdin(char* buf, size_t max);

#endif
