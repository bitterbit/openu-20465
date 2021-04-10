#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "text.h"

char* singleBuffer = NULL;
size_t chunks = 0;

#define CHUNK_SIZE 60
#define LINE_WIDTH "60"

int reallocReadText() {
    size_t actualSize;

    singleBuffer = calloc(CHUNK_SIZE, sizeof(char));

    while (singleBuffer != NULL && (actualSize == CHUNK_SIZE || chunks == 0)){
        actualSize = readStdin(singleBuffer + (chunks*CHUNK_SIZE), CHUNK_SIZE);
        chunks++;

        /* allocate space for next round in case we need it */
        singleBuffer = realloc(singleBuffer, (chunks+1)*CHUNK_SIZE);
    }

    if (singleBuffer == NULL) {
        printf("Error: memory allocation failed\n");
        return -1;
    }

    printf("realloc read %lu chunks\n", chunks);
    return 0;
}

int reallocPrintText() {
    size_t i;
    for(i=0; i<chunks; i++) {
        char *c = singleBuffer + (i * CHUNK_SIZE);
        printf("%." LINE_WIDTH "s\n", c);
    }

    free(singleBuffer);
    return 0;
}
