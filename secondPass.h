
#include "globals.h"
#include "code.h"
#include "lexer.h"

#ifndef UNTITLED5_SECONDPASS_H
#define UNTITLED5_SECONDPASS_H


/**
 * Executes the second pass of the assembly process.
 *
 * @param fileName The name of the output file.
 * @param lexList The lexical table containing parsed instructions and directives.
 * @param labelList The symbol table containing label information.
 * @return True if the second pass is successful, false otherwise.
 */
bool secondPass(char *fileName, lexTable *lexList, symbolTable *labelList);

/**
 * Retrieves the byte value of the specified operand in the instruction and inserts it into the object file content.
 *
 * @param currLex The current lexical structure representing the instruction.
 * @param objectFileContent The object file content structure to insert the byte into.
 * @param operandNumber The index of the operand being processed.
 * @param lineNum The line number of the current instruction.
 * @param labelList The symbol table containing label information.
 * @param externFileContentList The structure to store external file content.
 * @return True if operand processing is successful, false otherwise.
 */
bool getOperandByte(lexStruct *currLex, objectFileContent *objectFileContent, int operandNumber, int lineNum, symbolTable *labelList, externFileContent *externFileContentList);

/**
 * Retrieves the address of the specified label from the symbol table.
 *
 * @param label The label name to look up.
 * @param symbolTable The symbol table containing label information.
 * @return The address of the label if found, or -1 if not found.
 */
int getLabelAddress(char *label, symbolTable *symbolTable);

/**
 * Inserts a value into the object file content structure.
 *
 * @param objectContent The object file content structure to insert the value into.
 * @param bit The value to insert.
 */
void insertObjectFileContent(objectFileContent *objectContent, int bit);

/**
 * Inserts a label into the external file content structure.
 *
 * @param externContent The external file content structure to insert the label into.
 * @param label The label name.
 * @param lineNum The line number of the instruction.
 */
void insertToExternContent(externFileContent *externContent, char *label, int lineNum);

/**
 * Generates the object file with the provided object content.
 *
 * @param objectContent The object file content structure.
 * @param fileName The name of the output object file.
 */
void makeObjectFile(objectFileContent *objectContent, char *fileName);

/**
 * Generates the external file with the provided external content.
 *
 * @param externContent The external file content structure.
 * @param fileName The name of the output external file.
 */
void makeExternFile(externFileContent *externContent, char *fileName);

/**
 * Generates the entry file with the provided label list.
 *
 * @param labelList The symbol table containing label information.
 * @param fileName The name of the output entry file.
 */
void makeEntryFile(symbolTable *labelList, char *fileName);

/**
 * Converts a decimal value (12-bit) to a base64 representation.
 *
 * @param n The decimal value to convert.
 * @return The base64 representation of the decimal value.
 */
char *decimal12ToBase64(int n);

#endif