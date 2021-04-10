#include <stdio.h>
#include <stdlib.h>
#include "text.h"

struct LinkedBuffer {
    char *data;
    struct LinkedBuffer *next;
};

typedef struct LinkedBuffer LinkedBuffer;

/**
 * allocate a node with a buffer owned by him
 * caller should free when done by calling `deleteNode`
 */
LinkedBuffer* newNode(size_t dataSize) {
    LinkedBuffer* node = calloc(1, sizeof(LinkedBuffer));
    node->data = malloc(dataSize);
    if (!node->data) {
        return NULL;
    }
    return node;
}

/**
 * deletes both the memory of the node and the buffer owned by it
 */
void deleteNode(LinkedBuffer *node) {
    if (node == NULL) {
        return;
    }

    if (node->data) {
        free(node->data);
        node->data = NULL;
    }

    free(node);
}

LinkedBuffer *rootNode = NULL;

int linkedListReadText() {
    size_t sizeRead = 0;

    LinkedBuffer *prev = NULL;

    do {
        LinkedBuffer *node = newNode(CHUNK_SIZE);

        if (node == NULL) {
            return ERR_MEM;
        }

        /* first iteration of our loop, make this node the first of this 
         * linked list 
         */
        if (rootNode == NULL) {
            rootNode = node;
        }

        if (prev != NULL) {
            prev->next = node;
        }

        sizeRead = readStdin(node->data, CHUNK_SIZE);
        prev = node;
    } while (sizeRead == CHUNK_SIZE);

    return 0;
}

void linkedListPrintText() {
    LinkedBuffer *oldNode = NULL;
    LinkedBuffer *node = rootNode;
    while (node != NULL) {
        printf("%." LINE_WIDTH "s\n", node->data);
        oldNode = node;
        node = node->next;
        deleteNode(oldNode);
    }
}
