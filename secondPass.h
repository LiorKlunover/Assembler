#include "globals.h"
#include "code.h"
#include "lexer.h"
#ifndef UNTITLED5_SECONDPASS_H
#define UNTITLED5_SECONDPASS_H

typedef struct opCodeBinary{
    char *content[16];
    int codes[16];
}opCodeBinary;
#endif
bool secondPass(char *fileName, lexTable *lexList, symbolTable *symbolTable);
bool getOperandByte(lexStruct *currLex,objectFileContent *objectFileContent,int operandNumber,int lineNum,symbolTable *labelList,externFileContent *externFileContentList   );
int getLabelAddress(char *label,symbolTable *symbolTable);
void insertObjectFileContent(objectFileContent *objectContent, int bit);
void print(int bit);
char* decimal12ToBase64(int n) ;
void makeObjectFile(objectFileContent *objectContent, char *fileName);
void insertToExternContent(externFileContent *externContent, char *label, int bit);
void makeExternFile(externFileContent *externContent, char *fileName);
void makeEntryFile(symbolTable *labelList, char *fileName);