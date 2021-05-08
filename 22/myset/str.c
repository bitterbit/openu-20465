#include <string.h>
#include <ctype.h>
#include "bool.h"
#include "str.h"

bool isNumber(char *s) {
    int i;
    size_t len = strlen(s);
    for (i=0; i<len; i++) {
        char c = s[i];

        if (isdigit(c) || c == '-' || isspace(c)) {
            continue;
        } else {
            return false;
        }
    }

    return true;
}

char* skipSpace(char *s)  {
    char c = *s;
    while (c != '\0' && isspace(c)) {
        s++;
        c = *s;
    }

    return s;
}

bool hasComma(char *s) {
    return strchr(s, ',') != NULL;
}

bool isEmpty(char *s) {
    while(*s != '\0') {
        if (!isspace(*s)) {
            return false;
        }
        s++;
    }

    return true;
}

char* lineSkipToParams(char *line) {
    line = strchr(line, ' ');
    if (line == NULL) {
        return NULL;
    }

    /* skip to after the space */
    line += 1;
    /* printf("skipped to params: '%s'\n", line); */

    return line;
}

char* skipLeadingSpace(char* s) {
    if (isEmpty(s)) { 
        return s;
    }

    while(*s != '\0' && isspace(*s)) {
        s++;
    }

    return s;
}

void nullifyTrailingSpace(char* s) {
    size_t len;
    char* cursor;

    if (isEmpty(s)) {
        return;
    }

    len = strlen(s);
    cursor = s + len - 1;
    while(cursor > s && isspace(*cursor)) {
        *cursor = '\0';
        cursor--;
    }
}
