#include <stdio.h>
#include "globals.h"
#include "code.h"
#include "lexer.h"
bool firstPass(char *fileAm, macroTable *macroTable,lexTable *lexTable, symbolTable *labelList);
bool lineProcessFirst(FILE *fp, macroTable *macroTable,lexTable *lexTable, symbolTable *labelList);
bool checkForError(char *line, int lineNum) ;
void addBinaryCode(lexStruct *lexStruct);
void getInstructionsAndOperandsBit( lexStruct *lexTree);
int getOpCodeBinary(opCode opCode);
int getAddressingType(operandAddrType arrAddrType[], int index);