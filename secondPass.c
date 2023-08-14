#include "secondPass.h"
#include "globals.h"
#include "table.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
static int count = 0;
bool secondPass(char *fileName, lexTable *lexList, symbolTable *labelList){
     int lineBit;
    objectFileContent *objectFileContentList =NULL;
    externFileContent *externFileContentList=NULL;
    entryFileContent *entryFileContentList = NULL;
    lexStruct *currLex = NULL;
    int i=0,j;
    ALLOCATE(currLex, sizeof(lexStruct));
    MAKE_TYPE_CONTENT_TABLE(objectFileContentList)
    MAKE_TYPE_CONTENT_TABLE(externFileContentList)
    MAKE_TYPE_CONTENT_TABLE(entryFileContentList)

    while(i < lexList->capacity) {
        currLex = lexList->content[i++];
        if (currLex->label != NULL) {
            if (currLex->label->defineType == notEntryOrExtern || currLex->label->defineType == operandDefined || !currLex->label->defined ) {
                printf("Error in line %d: label \"%s\" is not defined\n", i, currLex->label->symbolName);
                return false;
            }
        }
        if(currLex->lineType == lexInst){
            lineBit = (currLex->lexType.instType.lineByteCodeData.lineByteCode);
            insertObjectFileContent(objectFileContentList, lineBit);
           if (!getOperandByte(currLex, objectFileContentList, 0, i, labelList, externFileContentList)){
               return false;
           }
            if (!getOperandByte(currLex, objectFileContentList, 1, i, labelList, externFileContentList)){
                return false;
            }
        }else if(currLex->lineType == lexDir){
            if (currLex->lexType.dirType.lexDirType == lexDirData) {
                for ( j = 0; j < currLex->lexType.dirType.dirUnionContent.dataType.numCount; ++j) {
                    insertObjectFileContent(objectFileContentList, currLex->lexType.dirType.dirUnionContent.dataType.numArray[j]);
                }
            } else if( currLex->lexType.dirType.lexDirType == lexDirString){
                j=0;
                while(j<(strlen(currLex->lexType.dirType.dirUnionContent.stringType)+1)){
                    insertObjectFileContent(objectFileContentList, currLex->lexType.dirType.dirUnionContent.stringType[j]);
                    j++;
                }
            }
        }
    }
    makeObjectFile(objectFileContentList, fileName);
    makeExternFile(externFileContentList, fileName);
    makeEntryFile(labelList, fileName);
   free(currLex);
    free(objectFileContentList->content);
    free(objectFileContentList);
    FREE_STRUCT(externFileContentList, externFileContentList);
    FREE_STRUCT(entryFileContentList, entryFileContentList);

return true;
}
bool getOperandByte(lexStruct *currLex,objectFileContent *objectFileContent,int operandNumber,int lineNum,symbolTable *labelList,externFileContent *externFileContentList) {
    int bit;
    if (currLex->lexType.instType.OpeInstTypes.arrOpType[operandNumber] != -1 ) {
        if(currLex->lexType.instType.OpeInstTypes.arrOpType[operandNumber] == labelType) {
            if ((bit = currLex->lexType.instType.OpeInstTypes.arrOpName[operandNumber].label->address) == 0) {
                if ((bit = getLabelAddress(currLex->lexType.instType.OpeInstTypes.arrOpName[operandNumber].label->symbolName, labelList)) == -1){
                    printf("Error in line %d: label \"%s\" is not defined\n", lineNum, currLex->lexType.instType.OpeInstTypes.arrOpName[operandNumber].label->symbolName);
                    return false;
                }
            }
            if (currLex->lexType.instType.OpeInstTypes.arrOpName[operandNumber].label->defineType == externLabel ){
                insertToExternContent(externFileContentList, currLex->lexType.instType.OpeInstTypes.arrOpName[operandNumber].label->symbolName, count);
            }
            if (operandNumber == 0) {
                bit <<= 2;
                bit |=2;
                currLex->lexType.instType.lineByteCodeData.sourceOpByteCode = bit;
            } else if (operandNumber == 1) {
                bit <<= 2;
                bit |=2;
                currLex->lexType.instType.lineByteCodeData.targetOpByteCode = bit;
            }
        }if (operandNumber == 1 ){
            insertObjectFileContent(objectFileContent, currLex->lexType.instType.lineByteCodeData.targetOpByteCode);
        }else if (operandNumber == 0 && !(currLex->lexType.instType.OpeInstTypes.arrOpType[0] == registerType && currLex->lexType.instType.OpeInstTypes.arrOpType[1] == registerType)){
            insertObjectFileContent(objectFileContent, currLex->lexType.instType.lineByteCodeData.sourceOpByteCode);
        }
    }
    return true;
}
int getLabelAddress(char *label,symbolTable *symbolTable){
    int i=0;
    while(i <= symbolTable->capacity){
        if(strcmp(symbolTable->content[i]->symbolName, label) == 0){
            if (symbolTable->content[i]->address != 0){
                return symbolTable->content[i]->address;
            }else{
                printf("Error: label %s Adress not found` is not defined\n",label);
                break;
            }
        }
        i++;
    }
    return -1;
}

void insertObjectFileContent(objectFileContent *objectContent, int bit){
    int *num = malloc(sizeof(int));
    *num = bit;
    if (objectContent->capacity == objectContent->size) {
        objectContent->size *= 2;
        REALLOCTION(objectContent->content, sizeof(char) * objectContent->size);
    }
    count++;
    objectContent->content[objectContent->capacity++] = decimal12ToBase64(*num);
}

void print(int bit){
    char str[] ="000000000000";
    int i,num = 1;
    for ( i = 0; i < 12; ++i) {
        if (num & bit) {
            str[11 - i] = '1';
        } else {
            str[11 - i] = '0';
        }
        num <<= 1;
    }
    printf("%d: %s - %d\n",  (100+count),str, bit);
    count++;
}
void insertToExternContent(externFileContent *externContent, char *label, int lineNum){
    char *str = malloc(sizeof(char) * 100);
    sprintf(str, "%s %d", label, (lineNum+100));
    if (externContent->capacity == externContent->size) {
        externContent->size *= 2;
        REALLOCTION(externContent->content, sizeof(char) * externContent->size);
    }
    externContent->content[externContent->capacity++] = str;

}
void makeObjectFile(objectFileContent *objectContent, char *fileName){
    int i;
    FILE *fp = NULL;
    fileName = giveNameFile(fileName, ".ob");
    fp = fopen(fileName, "w");
    if (fp == NULL){
        printf("Error: can't open file %s\n",fileName);
        return;
    }
    fprintf(fp, "%d %d\n", 100, objectContent->capacity);
    for (i = 0; i < objectContent->capacity; ++i) {
        fprintf(fp, "%d: %s\n",i, objectContent->content[i]);
    }
    fclose(fp);
    free(fileName);

}
void makeExternFile(externFileContent *externContent, char *fileName){
    int i;
    FILE *fp = NULL;
    fileName = giveNameFile(fileName, ".txt");
    fp = fopen(fileName, "w");
    if (fp == NULL){
        printf("Error: can't open file %s\n",fileName);
        return;
    }
    for (i = 0; i < externContent->capacity; ++i) {
        fprintf(fp, "%s\n", externContent->content[i]);
    }
    fclose(fp);
    free(fileName);
}
void makeEntryFile(symbolTable *labelList, char *fileName){
    int i;
    FILE *fp = NULL;
    char *str = malloc(sizeof(char) * 40);
    fileName = giveNameFile(fileName, ".ent");
    fp = fopen(fileName, "w");
    if (fp == NULL){
        printf("Error: can't open file %s\n",fileName);
        return;
    }
    for (i = 0; i < labelList->capacity; ++i) {
        if (labelList->content[i]->defineType == entryLabel) {
            sprintf(str, "%s %d", labelList->content[i]->symbolName, labelList->content[i]->address);
            fprintf(fp, "%s\n", str);
        }
    }
    fclose(fp);
    free(fileName);
}
const char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char* decimal12ToBase64(int n) {
    char *result = malloc(sizeof(char) * 3);
    int group1 = (n >> 6) & 0x3F;
    int group2 = n & 0x3F;
    result[0] = base64Chars[group1];
    result[1] = base64Chars[group2];
    result[2] = '\0';
    return result;
}


