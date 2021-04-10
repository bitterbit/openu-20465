#include <stdio.h>

size_t readStdin(char* buf, size_t max) {
    size_t count;
    char c;

    count = 0;
    c = getchar();

    while (c != EOF && count < max) {
        if (c == '\n' || c == '\0') {
            c = getchar();
            continue;
        }

        buf[count] = c;
        count++;
        c = getchar();
    }

    return count;
}
