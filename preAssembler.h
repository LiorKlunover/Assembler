#include <string.h>
#include "globals.h"
#include "code.h"
#define SKIP_WHITE(line, index) \
    removeDoubleWhitespaces(line); \
    for (;line[index] && (isspace(line[index])); (++(index))) \
        ;                  \
        if (line[index] == '\n' || line[index] == EOF || line[index] == ';' || line[index] == '\0')\
             continue;


/**
 * Pre-processes the input file to expand macros and generate a modified output file.
 *
 * @param filename The name of the input file to preprocess.
 * @param macroTable The macro table containing defined macros.
 * @return True if preprocessing was successful, false otherwise.
 */
bool preProcessFile(char *filename, macroTable *macroTable);

/**
 * Process each line of the input file, expanding macros and writing to the new file.
 *
 * @param fp The input file pointer.
 * @param newFile The output file pointer.
 * @param macroTable The macro table containing defined macros.
 * @return True if processing was successful, false otherwise.
 */
bool lineProcess(FILE *fp, FILE *newFile, macroTable *macroTable);