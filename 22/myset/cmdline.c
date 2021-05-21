#include <string.h>

#include "cmdline.h"
#include "str.h"

char* strDup(char* s) {
    char *dup;
    size_t len;

    if (s == NULL) {
        return NULL;
    }

    len = strlen(s);
    dup = malloc(len+1);
    memcpy(dup, s, len);
    dup[len] = '\0';

    return dup;
}

CmdLine* parseLine(char *line) {
    char cmd_delim[2] = " ";
    char param_delim[2] = ",";

    char* line_duplicate;
    char* cmd;
    char* params;
    char* param; /* currently parsed param */
    CmdLine *cmd_line;

    line_duplicate = strDup(line);
    cmd_line = malloc(sizeof(CmdLine));

    cmd = strtok(line, cmd_delim);
    params = lineSkipToParams(line_duplicate);

    cmd_line->cmd = cmd == NULL ? NULL : strDup(cmd);
    cmd_line->params = NULL;
    cmd_line->nb_params = 0;
    cmd_line->original_params_line = params == NULL ? NULL : strDup(params);

    if (params == NULL) {
        free(line_duplicate);
        return cmd_line;
    }


    param = strtok(params, param_delim);
    while(param != NULL) {
        size_t i;
        i = cmd_line->nb_params;

        cmd_line->nb_params += 1;
        cmd_line->params = realloc(cmd_line->params, sizeof(*cmd_line->params) * cmd_line->nb_params);

        /* 
         * skip leading space before duplicating it so it is simple to deallocate later on 
         * remove trailing space with nulls after duplicating to avoid affecting next strtok calls
         */
        param = skipLeadingSpace(param);
        cmd_line->params[i] = strDup(param);
        nullifyTrailingSpace(cmd_line->params[i]);

        param = strtok(NULL, param_delim);
    }

    free(line_duplicate);
    return cmd_line;
}

void freeLine(CmdLine *line) {
    size_t i;
    free(line->cmd);
    free(line->original_params_line);
    for (i=0; i<line->nb_params; i++) {
        free(line->params[i]);
    }

    line->params = NULL;
    line->nb_params = 0;
}
