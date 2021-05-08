#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "set.h"

Set SETA;
Set SETB;
Set SETC;
Set SETD;
Set SETE;
Set SETF;

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

struct CmdLine {
    char* cmd;
    char* original_params_line;
    size_t nb_params;
    char** params;
};

typedef struct CmdLine CmdLine;

void print_err(int err) {
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

bool isNumber(char *s) {
    size_t len = strlen(s);
    for (int i=0; i<len; i++) {
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

int readSet(Set *set, CmdLine *line) {
    reset_set(set);
    char *token = NULL;
    int num = 0;

    /* make sure to skip first param as it is the name of the set */
    for(size_t i=1; i<line->nb_params; i++) {
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

    char c = *s;
    while(c != '\0' && isspace(c)) {
        s++;
        c = *s;
    }

    return s;
}

void nullifyTrailingSpace(char* s) {
    if (isEmpty(s)) {
        return;
    }

    size_t len = strlen(s);
    char* cursor = s + len - 1;

    while(cursor > s && isspace(*cursor)) {
        *cursor = '\0';
        cursor--;
    }
}



/* 
 * CMD OP_1, OP_2, OP_3,...
 * CMD OP_1,OP_2,OP_3,...
 * CMD OP_1, OP_2 ,OP_3,...
 * OP can be
 *   1) name of set
 *   2) number
 */
CmdLine* parseLine(char *line) {
    char cmd_delim[2] = " ";
    char param_delim[2] = ",";

    char* line_duplicate = strdup(line);
    CmdLine *cmd_line = malloc(sizeof(CmdLine));

    char *cmd = strtok(line, cmd_delim);
    char *params = lineSkipToParams(line_duplicate);

    cmd_line->cmd = cmd == NULL ? NULL : strdup(cmd);
    cmd_line->params = NULL;
    cmd_line->nb_params = 0;
    cmd_line->original_params_line = params == NULL ? NULL : strdup(params);

    if (params == NULL) {
        free(line_duplicate);
        return cmd_line;
    }


    char* param = strtok(params, param_delim);
    while(param != NULL) {
        size_t i = cmd_line->nb_params;
        cmd_line->nb_params += 1;
        cmd_line->params = realloc(cmd_line->params, sizeof(*cmd_line->params) * cmd_line->nb_params);

        /* 
         * skip leading space before duplicating it so it is simple to deallocate later on 
         * remove trailing space with nulls after duplicating to avoid affecting next strtok calls
         */
        param = skipLeadingSpace(param);
        cmd_line->params[i] = strdup(param);
        nullifyTrailingSpace(cmd_line->params[i]);

        param = strtok(NULL, param_delim);
    }

    free(line_duplicate);
    return cmd_line;
}

void freeLine(CmdLine *line) {
    free(line->cmd);
    free(line->original_params_line);
    for (size_t i=0; i<line->nb_params; i++) {
        free(line->params[i]);
    }

    line->params = NULL;
    line->nb_params = 0;
}

void debugLine(CmdLine *line) {
    printf("$ CMD: %s\n", line->cmd);
    for (size_t i=0; i<line->nb_params; i++) {
        printf("$ param[%lu] = '%s'\n", i, line->params[i]);
    }
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
    int err = OK;
    if (cmd_line->cmd == NULL) {
        return ERR_UNDEF_CMD_NAME;
    }

    /* spec says no commas allowed in command name */
    if (strchr(cmd_line->cmd, ',') != NULL) {
        return ERR_ILLEGAL_COMMA;
    }

    for(size_t i=0; i<cmd_line->nb_params; i++) {
        if (isEmpty(cmd_line->params[i])) {
            return ERR_MULTIPLE_CONSECUTIVE_COMMAS;
        }
    }

    int min_nb_params = getMinNumberOfParams(cmd_line->cmd, &err);
    int max_nb_params = getMaxNumberOfParams(cmd_line->cmd);
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

int handleLine(char *line) {
    int err = OK;

    /* empty lines are OK by spec */
    if (isEmpty(line)) {
        return OK;
    }

    nullifyTrailingSpace(line);
    size_t line_len = strlen(line);

    /* can't be a valid line if it ends with a comma */
    if (line[line_len-1] == ',') {
        return ERR_EXTRANEOUS_TEXT_AFTER_CMD;
    }

    CmdLine* cmd_line = parseLine(line);
    /* debugLine(cmd_line); */

    err = checkCmdLineValid(cmd_line);
    CHECK_ERR(err);

    if (strcmp(cmd_line->cmd, "stop") == 0) {
        return STOP;
    }

    if (cmd_line->nb_params < 1) {
        return ERR_MISSING_PARAM;
    }

    Set *set = getSet(cmd_line->params[0], &err);
    CHECK_ERR(err);

    if (strcmp(cmd_line->cmd, "print_set") == 0) {
        print_set(set);
        printf("\n");
        return OK;
    }

    if (strcmp(cmd_line->cmd, "read_set") == 0) {
        err = readSet(set, cmd_line);
        CHECK_ERR(err);
        return OK;
    }

    /* All commands here have three set operands. 
     * Should have been checked before but just to make sure */
    if (cmd_line->nb_params < 3) {
        return ERR_MISSING_PARAM;
    }

    char *set_b_name = cmd_line->params[1];
    char *set_c_name = cmd_line->params[2]; 
    if (set_b_name == NULL || set_c_name == NULL) {
        return ERR_MISSING_PARAM;
    }

    Set *set_b = getSet(set_b_name, &err);
    CHECK_ERR(err);
    Set *set_c = getSet(set_c_name, &err);
    CHECK_ERR(err);

    if (strcmp(cmd_line->cmd, "union_set") == 0) {
        union_set(set, set_b, set_c);
    }

    if (strcmp(cmd_line->cmd, "intersect_set") == 0) {
        intersect_set(set, set_b, set_c);
    }

    if (strcmp(cmd_line->cmd, "sub_set") == 0) {
        sub_set(set, set_b, set_c);
    }

    if (strcmp(cmd_line->cmd, "symdiff_set") == 0) {
        symdiff_set(set, set_b, set_c);
    }

    return OK;
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

    int err = OK;
    while ((read = getline(&line, &len, stdin)) != -1) {
        line[read - 1] = '\0'; /* Null terminate instead of newline */
        printf(">> %s\n", line);

        /* spec states we should continue to next cmd after error */
        err = handleLine(line);
        if (err == STOP) {
            break;
        }
        if (err != OK) {
            print_err(err);
        }
    }

    free(line);
}
