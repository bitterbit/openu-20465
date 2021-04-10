#include <stdio.h>
#include "text.h"
#include "textRealloc.h"
#include "textLinkedList.h"

#define SINGLE_BUF_BACKEND 1
#define LINKED_LIST_BACKEND 2


int promtBackendSelection() {
    char choice;
    printf("1) Single Backing Buffer\n");
    printf("2) LinkedListed backed buffer\n");
    printf("Your Choice:");
    if (1 != scanf(" %c", &choice)) {
        printf(">> Error while reading selection\n");
        return -1;
    }

    switch(choice) {
        case '1':
            return SINGLE_BUF_BACKEND;

        case '2':
            return LINKED_LIST_BACKEND;

        default:
            printf(">> Invalid option selected: %c\n", choice);
            return -1;
    }
}

int main() {
    int res;
    TextImpl *textBackend;
    printf("Hello World\n");

    do {
        res = promtBackendSelection();
    } while(res < 0);

    if (res == SINGLE_BUF_BACKEND) {
        textBackend = &TextReallocImpl;
    } else if (res == LINKED_LIST_BACKEND) {
        textBackend = &TextLinkedListImpl;
    } else {
        return -1;
    }

    textBackend->readText();
    textBackend->printText();
}
