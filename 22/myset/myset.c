#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "bool.h"
#include "str.h"
#include "set.h"
#include "cmdline.h"

#define STOP -1
#define OK 0
#define ERR_UNDEF_SET_NAME 1
#define ERR_UNDEF_CMD_NAME 2
#define ERR_SET_MEMBER_OUT_OF_RANGE 3
#define ERR_SET_MEMBERS_NOT_TERMINATED 4
#define ERR_SET_MEMBER_NOT_INT 5
#define ERR_MISSING_PARAM 6
#define ERR_EXTRANEOUS_TEXT_AFTER_CMD 7
#define ERR_MULTIPLE_CONSECUTIVE_COMMAS 8
#define ERR_MISSING_COMMA 9
#define ERR_ILLEGAL_COMMA 10

void printErr(int err) {
    switch(err) {
        case ERR_UNDEF_SET_NAME:
            printf("Undefined set name\n");
            break;
        case ERR_UNDEF_CMD_NAME:
            printf("Undefined command name\n");
            break;
        case ERR_SET_MEMBER_OUT_OF_RANGE:
            printf("Invalid set member – value out of range\n");
            break;
        case ERR_SET_MEMBERS_NOT_TERMINATED:
            printf("List of set members is not terminated correctly\n");
            break;
        case ERR_SET_MEMBER_NOT_INT:
            printf("Invalid set member – not an integer\n");
            break;
        case ERR_MISSING_PARAM:
            printf("Missing parameter\n");
            break;
        case ERR_EXTRANEOUS_TEXT_AFTER_CMD:
            printf("Extraneous text after end of command\n");
            break;
        case ERR_MULTIPLE_CONSECUTIVE_COMMAS:
            printf("Multiple consecutive commas\n");
            break;
        case ERR_MISSING_COMMA:
            printf("Missing comma\n");
            break;
        case ERR_ILLEGAL_COMMA:
            printf("Illegal comma\n");
            break;
    }
}

Set SETA;
Set SETB;
Set SETC;
Set SETD;
Set SETE;
Set SETF;

Set* getSet(char* set_name, int *err) {
    if (set_name == NULL) {
        *err = ERR_UNDEF_SET_NAME;
        return NULL;
    }

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

    *err = ERR_UNDEF_SET_NAME;
    return NULL;
}


int readSet(Set *set, CmdLine *line) {
    int num = 0;
    size_t i;
    reset_set(set);

    /* make sure to skip first param as it is the name of the set */
    for(i=1; i<line->nb_params; i++) {
        char *token = line->params[i];
        if (isNumber(token) == false) {
            return ERR_SET_MEMBER_NOT_INT;
        }

        num = atoi(token);

        /* terminating character, 
         * continue parsing to catch errors where there is more data after terminating */
        if (num == -1) {
            continue;
        }

        /* don't insert invalid values */
        if (num < 0 || num > 127) {
            return ERR_SET_MEMBER_OUT_OF_RANGE;
        }

        put_set(set, num);
    }

    /* last item should be -1 */
    if (num != -1) {
        return ERR_SET_MEMBERS_NOT_TERMINATED;
    }

    return OK;
}


int getMaxNumberOfParams(char *cmd) {
    if (strcmp(cmd, "stop") == 0) {
        return 0;
    }

    if (strcmp(cmd, "print_set") == 0)  {
        return 1;
    }

    if (strcmp(cmd, "union_set") == 0 || strcmp(cmd, "intersect_set") == 0 ||
        strcmp(cmd, "sub_set") == 0 || strcmp(cmd, "symdiff_set") == 0) {
        return 3;
    }

    return -1;
}

int getMinNumberOfParams(char *cmd, int *err) {
    if (strcmp(cmd, "stop") == 0) {
        return 0;
    }

    if (strcmp(cmd, "print_set") == 0 || strcmp(cmd, "read_set") == 0)  {
        return 1;
    }

    if (strcmp(cmd, "union_set") == 0 || strcmp(cmd, "intersect_set") == 0 ||
        strcmp(cmd, "sub_set") == 0 || strcmp(cmd, "symdiff_set") == 0) {
        return 3;
    }

    *err = ERR_UNDEF_CMD_NAME;
    return -1;
}

#define CHECK_ERR(e) if (e != OK) { return e; }
int checkCmdLineValid(CmdLine *cmd_line) {
    int err;
    size_t i;
    int min_nb_params, max_nb_params;

    err = OK;
    if (cmd_line->cmd == NULL) {
        return ERR_UNDEF_CMD_NAME;
    }

    /* spec says no commas allowed in command name */
    if (strchr(cmd_line->cmd, ',') != NULL) {
        return ERR_ILLEGAL_COMMA;
    }

    for(i=0; i<cmd_line->nb_params; i++) {
        if (isEmpty(cmd_line->params[i])) {
            return ERR_MULTIPLE_CONSECUTIVE_COMMAS;
        }
    }

    min_nb_params = getMinNumberOfParams(cmd_line->cmd, &err);
    max_nb_params = getMaxNumberOfParams(cmd_line->cmd);
    CHECK_ERR(err);

    if (cmd_line->nb_params < min_nb_params) {
        if (!hasComma(cmd_line->original_params_line)) {
            return ERR_MISSING_COMMA;
        }

        return ERR_MISSING_PARAM;
    }

    if (max_nb_params != -1 && cmd_line->nb_params > max_nb_params) {
        return ERR_EXTRANEOUS_TEXT_AFTER_CMD;
    }

    return OK;
}

#define FREE_CMD_IF_ERR(e) if (e != OK) { freeLine(cmd_line); }
int handleLine(char *line) {
    Set* set_a;
    Set* set_b;
    Set* set_c;
    char *set_b_name = NULL;
    char *set_c_name = NULL;

    CmdLine *cmd_line = NULL;
    int err = OK;
    size_t line_len;

    /* empty lines are OK by spec */
    if (isEmpty(line)) {
        return OK;
    }

    nullifyTrailingSpace(line);
    line_len = strlen(line);

    /* can't be a valid line if it ends with a comma */
    if (line[line_len-1] == ',') {
        return ERR_EXTRANEOUS_TEXT_AFTER_CMD;
    }

    cmd_line = parseLine(line);
    err = checkCmdLineValid(cmd_line);
    FREE_CMD_IF_ERR(err);
    CHECK_ERR(err);

    if (strcmp(cmd_line->cmd, "stop") == 0) {
        freeLine(cmd_line);
        return STOP;
    }

    if (cmd_line->nb_params < 1) {
        freeLine(cmd_line);
        return ERR_MISSING_PARAM;
    }

    set_a = getSet(cmd_line->params[0], &err);
    FREE_CMD_IF_ERR(err);
    CHECK_ERR(err);

    if (strcmp(cmd_line->cmd, "print_set") == 0) {
        print_set(set_a);
        freeLine(cmd_line);
        return OK;
    }

    if (strcmp(cmd_line->cmd, "read_set") == 0) {
        err = readSet(set_a, cmd_line);
        FREE_CMD_IF_ERR(err);
        CHECK_ERR(err);

        freeLine(cmd_line);
        return OK;
    }

    /* All commands here have three set operands. 
     * Should have been checked before but just to make sure */
    if (cmd_line->nb_params < 3) {
        freeLine(cmd_line);
        return ERR_MISSING_PARAM;
    }

    set_b_name = cmd_line->params[1];
    set_c_name = cmd_line->params[2]; 
    if (set_b_name == NULL || set_c_name == NULL) {
        freeLine(cmd_line);
        return ERR_MISSING_PARAM;
    }

    set_b = getSet(set_b_name, &err);
    FREE_CMD_IF_ERR(err);
    CHECK_ERR(err);

    set_c = getSet(set_c_name, &err);
    FREE_CMD_IF_ERR(err);
    CHECK_ERR(err);

    if (strcmp(cmd_line->cmd, "union_set") == 0) {
        union_set(set_a, set_b, set_c);
    }

    if (strcmp(cmd_line->cmd, "intersect_set") == 0) {
        intersect_set(set_a, set_b, set_c);
    }

    if (strcmp(cmd_line->cmd, "sub_set") == 0) {
        sub_set(set_a, set_b, set_c);
    }

    if (strcmp(cmd_line->cmd, "symdiff_set") == 0) {
        symdiff_set(set_a, set_b, set_c);
    }

    freeLine(cmd_line);
    return OK;
}

#define CHUNK_SIZE 0x1000

char* readLine(char* prompt)
{
    int count = 0;
    char* buf;
    char c;

    printf("%s", prompt);
    buf = malloc(CHUNK_SIZE);

    c = fgetc(stdin);
    while (c != '\n' && c != EOF) {
        /* skip null's to avoid problems with
           string functions such as strlen */
        if (c == '\0') {
            continue;
        }

        buf[count++] = c;

        if (count % CHUNK_SIZE == 0) {
            size_t chunks = count / CHUNK_SIZE;
            buf = realloc(buf, (chunks+1) * CHUNK_SIZE);
        }

        c = fgetc(stdin);
    }

    if (c == EOF) {
        free(buf);
        return NULL;
    }

    buf[count] = '\0';
    return buf;
}

int main(int argc, char **argv) {
    char *line = NULL;
    int err;
    err = OK;

    reset_set(&SETA);
    reset_set(&SETB);
    reset_set(&SETC);
    reset_set(&SETD);
    reset_set(&SETE);
    reset_set(&SETF);

    line = readLine(">> ");
    while (line != NULL) {
        printf("%s\n", line);

        /* spec states we should continue to next cmd after error */
        err = handleLine(line);
        if (err == STOP) {
            free(line);
            break;
        }
        if (err != OK) {
            printErr(err);
        }

        free(line);
        line = readLine(">> ");
    }

    if (err != STOP) {
        printf("\nReached end of commands without a stop command\n");
    }

    return 0;
}
