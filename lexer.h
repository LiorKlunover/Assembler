#include "globals.h"
#include "code.h"
#ifndef UNTITLED5_LEXER_H
#define UNTITLED5_LEXER_H

#define MAX_LABEL_SIZE 31
#define MAX_STRING_LEN 80
#define MAX_NUMBER_SIZE 80
#define MAX_DATA_VALUE 511
#define MIN_DATA_VALUE -512

typedef enum operandAddrType {
    immediate,
    labelType,
    registerType,
    errorType = -1
}operandAddrType;

typedef union operandAddrTypeUnion {
    label *label;
    char reg;
    int immed;
}operandAddrTypeName;
typedef struct bitArrayDir {
    unsigned int lineByteCode : 12;
}bitLine;

typedef struct lexStruct {
    label *label;
    enum {
        lexInst,
        lexDir
    }lineType;
    union {
        struct {
            int **contentByteCode;
            enum {
                lexDirString,
                lexDirData,
                lexDirEntry,
                lexDirExtern
            }lexDirType;

            union {
                char stringType[MAX_STRING_LEN];
                struct {
                    int numArray[MAX_NUMBER_SIZE];
                    int numCount;
                }dataType;
            }dirUnionContent;
            bitLine dirBitArray [MAX_STRING_LEN+1];
        }dirType;

        struct {
            opCode instName;
            int numOfOperands;
            struct {
                 int lineByteCode : 12;
                 int sourceOpByteCode : 12;
                 int targetOpByteCode : 12;
            }lineByteCodeData;
            union {
                struct {
                    operandAddrType arrOpType[2];
                    operandAddrTypeName arrOpName[2];
                }twoOpeInstType;

                struct {
                    operandAddrType oat;
                    union {
                        struct {
                            operandAddrType oat[2];
                            operandAddrTypeName oaTu[2];
                        }setB_param_label;
                        operandAddrTypeName oatu;
                    }setB_operands;
                }asm_inst_setB;
            }asm_inst_sets;
        }instType;
    }lexType;
}lexStruct;

typedef struct lexTable{
    lexStruct **lexStructList;
    int size;
    int capacity;
} lexTable;

lexStruct * getLexTreePosition(lineStr *line, symbolTable *labelTable, int *IC, int* DC, macroTable *macroTable);
bool isValidLabel (char *label, int linNum, symbolTable *list, macroTable *macroTable) ;
void insertLabelTable(char *l, symbolTable *labelTable, int lineNum, label **currentLabel, lDType type,bool defined);
bool getData(const char *inputString, int *numArray, int *numCount,int *lineNum,label *label);
label *getLabel(char *label, symbolTable *labelList);
int getOpCode(char *str,int *lineNum,int *numOfOperands);
bool getOperands(char *str , lexStruct *lexTree, int *lineNum, symbolTable *labelTable, macroTable *macroTable);
bool getNumber(char *str, int *num);
bool checkOperandOrder(opCode opCode, operandAddrType arrOpType[], int *lineNum );
void makeStringBitArray(char *str ,bitLine *stringBitArray);
#endif