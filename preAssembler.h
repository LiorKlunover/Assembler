#include <string.h>
#include "globals.h"
#include "code.h"
#define SKIP_WHITE(line, index) \
    removeDoubleWhitespaces(line); \
    for (;line[index] && (isspace(line[index])); (++(index))) \
        ;                  \
        if (line[index] == '\n' || line[index] == EOF || line[index] == ';' || line[index] == '\0')\
             continue;


bool preProcessFile(char *filename, macroTable *macroTable);
bool lineProcess(FILE *fp , FILE *newFile, macroTable *macroTable);
