#include <stdlib.h>

#ifndef _H_CMDLINE_
#define _H_CMDLINE_
struct CmdLine {
    char* cmd;
    char* original_params_line;
    size_t nb_params;
    char** params;
};

typedef struct CmdLine CmdLine;

CmdLine* parseLine(char *line);
void freeLine(CmdLine *line);

#endif

/* 
 * Valid Command Line Exmples:
 * CMD OP_1, OP_2, OP_3,...
 * CMD OP_1,OP_2,OP_3,...
 * CMD OP_1, OP_2 ,OP_3,...
 * OP can be
 *   1) name of set
 *   2) number
 */

