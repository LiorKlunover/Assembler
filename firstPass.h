#include <stdio.h>
#include "globals.h"
#include "code.h"
#include "lexer.h"


#ifndef FIRSTPASS_H
#define FIRSTPASS_H

/**
 * Conducts the first pass of the assembly code, processing each line
 * to build the lexical table and symbol table.
 *
 * @param fileAm The input filename.
 * @param macroTable A pointer to the macro table.
 * @param lexTable A pointer to the lexical table.
 * @param labelList A pointer to the symbol table.
 * @return True if the first pass is successful, otherwise false.
 */
bool firstPass(char *fileAm, macroTable *macroTable, lexTable *lexTable, symbolTable *labelList);

/**
 * Processes a line of assembly code during the first pass.
 *
 * @param fp A pointer to the input file.
 * @param macroTable A pointer to the macro table.
 * @param lexTable A pointer to the lexical table.
 * @param labelList A pointer to the symbol table.
 * @return True if the line processing is successful, otherwise false.
 */
bool lineProcessFirst(FILE *fp, macroTable *macroTable, lexTable *lexTable, symbolTable *labelList);

/**
 * Checks for errors in a line of code, specifically consecutive commas.
 *
 * @param line The line of code to check.
 * @param lineNum The line number for error reporting.
 * @return True if there are no errors, otherwise false.
 */
bool checkForError(char *line, int lineNum);

/**
 * Adds binary code to a lexical structure for instructions.
 *
 * @param lexStruct A pointer to the lexical structure to modify.
 */
void addBinaryCode(lexStruct *lexStruct);

/**
 * Computes the binary code for instruction and operand types.
 *
 * @param lexTree A pointer to the lexical structure containing instruction and operand information.
 */
void getInstructionsAndOperandsBit(lexStruct *lexTree);

/**
 * Retrieves the binary opcode for a given operation code.
 *
 * @param opCode The operation code.
 * @return The binary representation of the opcode.
 */
int getOpCodeBinary(opCode opCode);

/**
 * Determines the binary representation of an addressing type.
 *
 * @param arrAddrType An array of operand addressing types.
 * @param index The index of the operand.
 * @return The binary representation of the addressing type.
 */
int getAddressingType(operandAddrType arrAddrType[], int index);

/**
 * Inserts a lexical structure into the lexical table.
 *
 * @param lexTable A pointer to the lexical table.
 * @param lexTree The lexical structure to insert.
 */
void insertLexTable(lexTable *lexTable, lexStruct lexTree);

/**
 * Frees memory associated with a line structure.
 *
 * @param line A pointer to the line structure to free.
 */
void freeLineStr(lineStr *line);

#endif
