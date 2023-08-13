#include "firstPass.h"
#include <stdlib.h>
#include "codeFirsPass.h"
#include "table.h"
bool firstPass(char *fileAm, macroTable *macroTable,lexTable *lexTable, symbolTable *labelList) {
    FILE *fp;
    /*fileAm = strcat(fileAm, ".am");
    fp = fopen(fileAm, "r");*/
    fp = fopen("C:\\Users\\lior3\\CLionProjects\\Assembler\\test1.am", "r");
    if (fp == NULL) {
        printf("Error: file %s cannot be opened\n", fileAm);
        return false;
    }
    if (lineProcessFirst(fp, macroTable, lexTable, labelList)) {
        fclose(fp);
        return true;
    }
    fclose(fp);
    return false;
}

bool lineProcessFirst(FILE *fp, macroTable *macroTable,lexTable *lexTable, symbolTable *labelList) {
    char *currLine;
    lineStr * line;
    int index=0, countWords,*IC,*DC;
    lexStruct *lexTree = NULL;
    bool error = true;
    ALLOCATE(currLine, MAX_LINE_LENGTH);
    ALLOCATE(line, sizeof(line));
    ALLOCATE(IC, sizeof(int));
    ALLOCATE(DC, sizeof(int));
    *IC=0;
    *DC=0;
    while (fgets(currLine, MAX_LINE_LENGTH, fp) != NULL) {
        if(!checkForError(currLine, index)){
            error = false;
        }
        line->lineContent = splitString(currLine, &countWords, " \n");
        line->lineNum = ++index;
        line->size = countWords;
        if (lexTable->capacity == lexTable->size) {
            reallocateLexTable(lexTable);
        }
        lexTree = getLexTreePosition(line, labelList,IC,DC,macroTable);
        if (lexTree == NULL) {
            printf("error in first pass: %d\n", line->lineNum);
            error = false;
        }else{
            if (lexTree->lineType == lexInst)
                addBinaryCode(lexTree);
        }

        lexTable->lexStructList[lexTable->capacity++] = lexTree;

    }
    return error;
}

bool checkForError(char *line, int lineNum) {
    if (checkComma(line)) {
        printf("Error in lineStr %d: Consecutive commas\n", lineNum);
        return false;
    }
    return true;
}


void addBinaryCode(lexStruct *lexStruct){
    int bit =0;
    if (lexStruct->lineType == lexInst && lexStruct->lexType.instType.instName != error) {
        getInstructionsAndOperandsBit(lexStruct);

    }
}

void getInstructionsAndOperandsBit( lexStruct *lexTree){
    operandAddrTypeName *arrAddName = lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpName;
    opCode opCode = lexTree->lexType.instType.instName;
    operandAddrType *arrAddrType = lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpType;
    int bitLine =0,bitFirst=0,bitSecond=0,temp=0;
    int bitSourceOperand=0;
    int birTargetOperand = 0;
    bitLine |= getOpCodeBinary(opCode);
    if (lexTree->lexType.instType.numOfOperands > 0){
        bitFirst = getAddressingType(arrAddrType, 1);
        if (bitFirst == 1) {
            temp = arrAddName[1].immed;
            temp <<= 2;
            temp  &= 4095;
            birTargetOperand = temp;

        }else if(bitFirst == 3){
            if (arrAddName[1].label->address != 0) {
                temp = arrAddName[1].label->address;
                temp <<= 2;
                birTargetOperand |= temp;
                birTargetOperand |= 2;
            }
        }else if(bitFirst == 5){
            temp = (arrAddName[1].reg);
            temp <<= 2;
            birTargetOperand |= temp;
        }
        if (lexTree->lexType.instType.numOfOperands > 1){
            bitSecond = getAddressingType(arrAddrType, 0);
            if(bitSecond == 5){
                temp = (arrAddName[0].reg);
                temp <<= 7;
                if(arrAddrType[1] != registerType){
                    bitSourceOperand |= temp;
                } else{
                    birTargetOperand |= temp;
                }
            }else if(bitSecond == 3){
                if (arrAddName[0].label->address != 0){
                    temp = arrAddName[0].label->address;
                    temp <<= 2;
                    bitSourceOperand |= temp;
                    bitSourceOperand |= 2;
                }
            }else if(bitSecond==1){
                temp = arrAddName[0].immed;
                temp = ~temp;
                temp++;
                bitSourceOperand = temp;
            }
            bitFirst <<= 2;
            bitSecond <<= 9;
            bitLine |= bitFirst;
            bitLine |= bitSecond;
        } else{
            bitFirst <<= 2;
            bitLine |= bitFirst;
        }
    }
    lexTree->lexType.instType.lineByteCodeData.lineByteCode = bitLine;
    lexTree->lexType.instType.lineByteCodeData.sourceOpByteCode = bitSourceOperand;
    lexTree->lexType.instType.lineByteCodeData.targetOpByteCode = birTargetOperand;

}
int getOpCodeBinary(opCode opCode){
    int codes[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int index = opCode;
    int byteCode = codes[index] << 5;
    return byteCode;
}
int getAddressingType(operandAddrType arrAddrType[], int index){
    if(arrAddrType[index] == immediate){
        return 1;
    }
    if(arrAddrType[index] == labelType){
        return 3;
    }
    if(arrAddrType[index] == registerType){
        return 5;
    }
    return 0;
}