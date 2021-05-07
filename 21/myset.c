#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"

Set SETA;
Set SETB;
Set SETC;
Set SETD;
Set SETE;
Set SETF;


Set* getSet(char* set_name) {
    if (set_name == NULL) {
        return NULL;
    }

    /* printf("getset(%s);\n", set_name); */

    if (strcmp(set_name, "SETA") == 0) {
        return &SETA;
    }
    if (strcmp(set_name, "SETB") == 0) {
        return &SETB;
    }
    if (strcmp(set_name, "SETC") == 0) {
        return &SETC;
    }
    if (strcmp(set_name, "SETD") == 0) {
        return &SETD;
    }
    if (strcmp(set_name, "SETE") == 0) {
        return &SETE;
    }
    if (strcmp(set_name, "SETF") == 0) {
        return &SETF;
    }

    return NULL;
}

char cmd_delim[3] = " ,";

void readSet(Set *set) {
    reset_set(set);
    char *token = NULL;
    do {
        token = strtok(NULL, cmd_delim);
        if (token == NULL) {
            break;
        }

        int num = atoi(token);
        if (num == -1) {
            break;
        }

        // don't insert invalid values
        if (num < 0 || num > 127) {
            continue;
        }

        put_set(set, num);
    } while(token != NULL);
}

bool handleLine(char *line) {
    char *cmd = strtok(line, cmd_delim);

    if (cmd == NULL) {
        return false;
    }

    if (strcmp(cmd, "stop") == 0) {
        return false; /* signal to caller that processing of commands should stop */
    }

    char *set_name = strtok(NULL, cmd_delim);
    Set *set = getSet(set_name);

    if (set_name == NULL || set == NULL) {
        printf("Undefined set\n");
        return false;
    }

    if (strcmp(cmd, "read_set") == 0) {
        readSet(set);
        return true;
    }

    if (strcmp(cmd, "print_set") == 0) {
        print_set(set);
        printf("\n");
        return true;
    }

    if ( strcmp(cmd, "union_set") & 
         strcmp(cmd, "intersect_set") & 
         strcmp(cmd, "sub_set") & 
         strcmp(cmd, "symdiff_set") == 0) {
        printf("Invalid Command\n");
        return false;
    }

    // All commands here have three set operands
    char *set_b_name = strtok(NULL, cmd_delim);
    Set *set_b = getSet(set_b_name);
    if (set_b_name == NULL || set_b == NULL) {
        printf("Undefined set\n");
        return false;
    }

    char *set_c_name = strtok(NULL, cmd_delim);
    Set *set_c = getSet(set_c_name);

    if (set_c_name == NULL || set_c == NULL) {
        printf("Undefined set\n");
        return false;
    }

    if (strcmp(cmd, "union_set") == 0) {
        union_set(set, set_b, set_c);
        return true;
    }

    if (strcmp(cmd, "intersect_set") == 0) {
        intersect_set(set, set_b, set_c);
        return true;
    }

    if (strcmp(cmd, "sub_set") == 0) {
        sub_set(set, set_b, set_c);
        return true;
    }

    if (strcmp(cmd, "symdiff_set") == 0) {
        symdiff_set(set, set_b, set_c);
        return true;
    }

    return true;
}

int main(int argc, char **argv) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    reset_set(&SETA);
    reset_set(&SETB);
    reset_set(&SETC);
    reset_set(&SETD);
    reset_set(&SETE);
    reset_set(&SETF);

    bool should_continue = true;
    while ((read = getline(&line, &len, stdin)) != -1 && should_continue) {
        line[read - 1] = '\0'; /* Null terminate instead of newline */
        printf(">> %s\n", line);
        should_continue = handleLine(line);
    }

    free(line);
}
