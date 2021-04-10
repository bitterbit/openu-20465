#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum State {
    START = 0,
    IN_SENTENCE,
    IN_QUOTES
};

typedef enum State State;


State global_state = START;

/*  takes a letter as input, and transforms it according to 
    rules defined the maman.
    might return 0 if letter should not be outputed at all */
char handle_letter(char l) {
    /* ignore all digits */
    if (isdigit(l)) {
        return 0;
    }

    if (l == '"') {
        if (global_state != IN_QUOTES) {
            global_state = IN_QUOTES;
        } else {
            global_state = IN_SENTENCE;
        }
        /* no need to continue processing qoute characters */
        return l;
    }

    if (global_state == START) {
        /* A new line begins with the first character that is not a white space */
        if (isspace(l)) {
            return l;
        }

        /* first letter of sentence should be with upper case */
        global_state = IN_SENTENCE;
        return toupper(l);
    }


    /* Any dot that is not inside a quote starts a new sentence */
    if (l == '.' && global_state != IN_QUOTES) {
        global_state = START;
    }

    /* All characters that are not the first of a sentence should be with
       lower case */
    if (global_state == IN_SENTENCE) {
        return tolower(l);
    }

    /* All characters inside quotation marks should be uppercased */
    if (global_state == IN_QUOTES) {
        return toupper(l);
    }

    /* If no special case, default to returning the exact same letter */
    return l;
}

int main() {
    size_t char_count = 0;
    char c;

    printf("Please enter a story here:\n");
    do {
        c = getchar();
        char_count++;
        c = handle_letter(c);

        if (c != 0) {
            putchar(c);
        }
    } while(c != EOF);

    return 0;
}
