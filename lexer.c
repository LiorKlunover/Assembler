#include "lexer.h"
#include "firstPass.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

lexStruct * getLexTreePosition(lineStr *line, symbolTable *labelTable, int * IC, int * DC, macroTable *macroTable){
    lexStruct *lexTree=NULL;
    opCode instName;
    bool labelFlag = false, dirFlag = false, instFlag = false;
    char **lineC = line->lineContent;
    label *currLabel = NULL;
    char *token =NULL,*ptr = NULL;
    int len = 0,index = 0;
    ALLOCATE(lexTree, sizeof(lexStruct));
    ALLOCATE(ptr, MAX_LINE_LENGTH);
    lexTree->label = NULL;

    for(index = 0; index < line->size; index++) {

        ptr =   strcpy(ptr,lineC[index]);
        len = (int) strlen(ptr);
        ptr[len] = '\0';

        if (len > 0 && ptr[len-1]  == ':') {
            ptr[len-1] = '\0';
            if (!isValidLabel(ptr, line->lineNum, labelTable, macroTable)) {
                return NULL;
            }
            if (strlen(*lineC)>index+1 && (strstr(lineC[index+1],"extern")  || strstr(lineC[index+1],"entry"))){
                printf("Warning label was define before entry\\extern in line: %d \n", line->lineNum);
                continue;
            }
            if((currLabel = getLabel(ptr, labelTable)) == NULL){
                insertLabelTable(ptr, labelTable, line->lineNum, &lexTree->label, notEntryOrExtern,true);
            }else if (currLabel->defineType == notEntryOrExtern ){
                printf("Error in line: %d , label was define before \n", line->lineNum);
                return NULL;
            } else if(currLabel->defineType == externLabel){
                printf("Error in line: %d ,can't define extern label  \n", line->lineNum);
                return NULL;
            }else if(currLabel->defineType == operandDefined){
                currLabel->defineType = notEntryOrExtern;
            }else if(currLabel->address != 0){
                printf("Error in line: %d ,label was define before \n", line->lineNum);
                return NULL;
            }
            if(currLabel != NULL && !currLabel->defined){
                currLabel->defined = true;
            }

            labelFlag = true;
            continue;
        }
        else if (strchr(ptr, '.') != NULL && dirFlag == false) {
            strcpy(ptr, lineC[index]);
            if (strcmp(ptr, ".data") == 0) {
                lexTree->lexType.dirType.lexDirType = lexDirData;
            } else if (strcmp(ptr, ".string")== 0) {
                lexTree->lexType.dirType.lexDirType = lexDirString;
            } else if (strcmp(ptr, ".entry")== 0) {
                lexTree->lexType.dirType.lexDirType = lexDirEntry;
            } else if (strcmp(ptr, ".extern")== 0) {
                lexTree->lexType.dirType.lexDirType = lexDirExtern;
            } else {
                printf("Error in lineStr %d: invalid directive\n", line->lineNum);
                return NULL;
            }
            lexTree->lineType = lexDir;
            dirFlag = true;
            continue;
        }
        else if (dirFlag) {
            if (lexTree->lexType.dirType.lexDirType == lexDirData){
                if(!getData(ptr, lexTree->lexType.dirType.dirUnionContent.dataType.numArray,
                        &lexTree->lexType.dirType.dirUnionContent.dataType.numCount, &line->lineNum,lexTree->label)){
                    return NULL;
                }
                if (labelFlag) {
                    if (lexTree->label == NULL) {
                        lexTree->label = currLabel;
                    }
                    lexTree->label->address = *DC+100+*IC;
                }
                *DC += lexTree->lexType.dirType.dirUnionContent.dataType.numCount;
            } else if (lexTree->lexType.dirType.lexDirType == lexDirString) {
                if (ptr[0] != '"'|| ptr[len-1] != '"') {
                    printf("Error in lineStr %d: invalid string\n", line->lineNum);
                    return NULL;
                }
                ptr[len-1] = '\0';
                ptr++;
                strcpy(lexTree->lexType.dirType.dirUnionContent.stringType,ptr );
                makeStringBitArray( ptr,lexTree->lexType.dirType.dirBitArray);
                if (labelFlag) {
                    if (lexTree->label == NULL) {
                        lexTree->label = currLabel;
                    }
                    lexTree->label->address = *DC+100 + *IC;
                }
                *DC += strlen(ptr)+1;
            }else if (lexTree->lexType.dirType.lexDirType == lexDirEntry) {
                if((currLabel = getLabel(ptr, labelTable)) == NULL){
                    insertLabelTable(ptr, labelTable, line->lineNum,&lexTree->label,entryLabel,false);
                }else if(currLabel->defineType == notEntryOrExtern ||currLabel->defineType == operandDefined){
                        currLabel->defineType = entryLabel;
                }else if(currLabel->defineType == entryLabel){
                    printf("Warning in line: %d , label was define before \n", line->lineNum);
                }else {
                    printf("Error in line: %d , label was define as extern \n", line->lineNum);
                    return NULL;
                }
            }else if (lexTree->lexType.dirType.lexDirType == lexDirExtern) {
                token = strtok(ptr, " ,");
                while (token != NULL) {
                    if ((currLabel = getLabel(token, labelTable)) == NULL) {
                        insertLabelTable(token, labelTable, line->lineNum, &lexTree->label, externLabel,false);
                        currLabel = getLabel(token, labelTable);
                    } else if (currLabel->defineType == notEntryOrExtern || currLabel->defineType == operandDefined) {
                        currLabel->defineType = externLabel;
                    } else if (currLabel->defineType == externLabel) {
                        printf("Warning in line: %d , label was define before \n", line->lineNum);
                    } else {
                        printf("Error in line: %d , label was define as entry \n", line->lineNum);
                        return NULL;
                    }
                    currLabel->address = 1;
                    token = strtok(NULL, " ,");
                }
            }else {
                printf("Error in lineStr %d: invalid directive\n", line->lineNum);
                return NULL;
            }
        } else if (( instName = (opCode)getOpCode(ptr, &line->lineNum,&lexTree->lexType.instType.numOfOperands)) != -1) {
                lexTree->lexType.instType.instName = instName;
                lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpType[0] = -1;
                lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpType[1] = -1;
                instFlag = true;
                if (labelFlag) {
                    if(lexTree->label == NULL)
                        lexTree->label = currLabel;
                    lexTree->label->address = (*DC)+100+(*IC);
                }
                *IC += 1 + lexTree->lexType.instType.numOfOperands;
                lexTree->lineType = lexInst;
        }else if(instFlag){
            if (!getOperands(ptr,lexTree ,&line->lineNum, labelTable, macroTable)){
                return NULL;
            }
            if(lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpType[0] == registerType && lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpType[1] == registerType){
                --(*IC);
            }
        } else
            return NULL;
    }
    if (instFlag){
        if (!checkOperandOrder(lexTree->lexType.instType.instName,lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpType,
                               &line->lineNum)){
            printf("Error in line: %d , invalid operand \n", line->lineNum);
            return NULL;
        }
    }
    return lexTree;
}

bool getOperands(char *str , lexStruct *lexTree, int *lineNum, symbolTable *labelTable, macroTable *macroTable){
    operandAddrTypeName tempArrOpName;
    char *token = strtok(str, ",");
    int num;
    operandAddrType  tempArrOpType=-1;

    while (token != NULL) {
        if (lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpType[0] != -1){
            printf("Error in line: %d , too many operands \n", *lineNum);
            return false;
        }
        else if (token[0] == '@'){
            if (strlen(token) == 3 && token[1] == 'r' && token[2]<='7' && token[2] >= '0'){
                tempArrOpType= registerType;
                tempArrOpName.reg = token[2]-'0';
            }else{
                printf("Error in line: %d , invalid register \n", *lineNum);
                return false;
            }
        }
        else if(getNumber(token,&num)){
            if (num > MAX_DATA_VALUE || num < MIN_DATA_VALUE) {
                printf("Error in line: %d , immediate value is out of range \n", *lineNum);
                return false;
            }
            tempArrOpType = immediate;
            tempArrOpName.immed = num;
        }
        else if(isValidLabel(token,*lineNum,labelTable,macroTable)){
            tempArrOpType = labelType;
            if ((tempArrOpName.label = getLabel(token, labelTable)) == NULL) {
                insertLabelTable(token, labelTable, *lineNum, &tempArrOpName.label, operandDefined,false);
            }
        }
        else{
            printf("Error in line: %d , invalid operand \n", *lineNum);
            return false;
        }
        if ( lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpType[1]!= -1){
            lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpType[0] = lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpType[1];
            lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpName[0] = lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpName[1] ;
        }
        lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpType[1] = tempArrOpType;
        lexTree->lexType.instType.asm_inst_sets.twoOpeInstType.arrOpName[1] = tempArrOpName;
        token = strtok(NULL, ",");
    }
    return true;

}
bool checkOperandOrder(opCode opCode, operandAddrType arrOpType[], int *lineNum){
    if (opCode == 6){
        if (arrOpType[0] != labelType){
            printf("Error in line: %d , invalid operand \n", *lineNum);
            return false;
        }
    }
    if (opCode >= 0 && opCode  <=3 ){
        if (arrOpType[0] == -1){
            printf("Error in line: %d , invalid operand \n", *lineNum);
            return false;
        }
    }
    if (opCode == 1 || opCode == 12){
        if (arrOpType[1] == -1){
            printf("Error in line: %d , invalid operand \n", *lineNum);
            return false;
        }
    }
    if (opCode == 4 || opCode == 5 ||(opCode >=7 && opCode<=15)){
        if (arrOpType[0] != -1){
            printf("Error in line: %d , invalid operand \n", *lineNum);
            return false;
        }
    }
    if (opCode == 0 || (opCode >=2 && opCode <= 11) || opCode == 13){
        if (arrOpType[1] == immediate){
            return false;
        }
    }
    if (opCode == 14 || opCode == 15){
        if (arrOpType[0] != -1 || arrOpType[1] != -1){
            return false;
        }
    }
    return true;
}
bool getNumber(char *str, int *num){
    int i=0;
    if (str[0] == '-') {
        i=1;
    }
    for (; i < strlen(str); ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    *num = atoi(str);
    return true;
}
int getOpCode(char *str,int *lineNum,int *numOfOperands){
         int i;
        char *opCode[] = {"mov", "cmp", "add", "sub", "not","clr","lea", "inc",
                          "dec", "jmp", "bne", "red", "prn", "jsr","rts", "stop"};
        char *opCode1[] = {"mov", "cmp", "add", "sub", "lea"};
        char *opCode2[] = { "not", "clr","inc", "dec", "jmp", "bne", "red", "prn", "jsr"};
        char *opCode3[] = { "rts", "stop"};

        for (i = 0; i < 5; ++i) {
                if (strcmp(opCode1[i], str) == 0){
                    *numOfOperands=2;
                    break;
                }
        }
        for (i = 0; i < 9; ++i) {
            if (strcmp(opCode2[i], str) == 0){
                *numOfOperands = 1;
                 break;
           }
         }
        for (i = 0; i < 2; ++i) {
            if (strcmp(opCode3[i], str) == 0){
                *numOfOperands = 0;
                break;
            }
        }
        for (i = 0; i < 16; ++i) {
            if (strcmp(opCode[i], str) == 0){
              return  i;
        }
        }
        return -1;
}
bool getData(const char *inputString, int *numArray, int *numCount,int *lineNum,label *label) {
        char buffer[100];
        char *token=NULL;
        int count;
        strcpy(buffer, inputString);
        if (!isalnum(inputString[0]) || !isalnum(inputString[strlen(inputString) - 1] )) {
            printf("Error in lineStr %d: invalid data\n", *lineNum);
            return false;
        }
        token = strtok(buffer, ",");
        count = 0;

        while (token != NULL && count < 3) {
            if ((count > 0 && token == inputString) || (count > 0 && token[0] == '\0')) {
                return false;
            }
            numArray[count] = atoi(token);

            if (numArray[count] > MAX_DATA_VALUE || numArray[count] < MIN_DATA_VALUE) {
                printf("Error in lineStr %d: data value is out of range\n", *lineNum);
                return false;
            }
            if (count == 0 && label != NULL){
                label->data = numArray[count];
            }
            token = strtok(NULL, ",");
            count++;
        }
        *numCount = count;
    return true;
}
void makeStringBitArray(char *str ,bitLine *stringBitArray){
    int i;
    for (i = 0; i < strlen(str); ++i) {
        stringBitArray[i].lineByteCode = str[i];
    }
    stringBitArray[i].lineByteCode = 0;
}
bool isValidLabel (char *label, int linNum, symbolTable *labelList, macroTable *macroTable) {
    int i = 0;
    if (strlen(label) > MAX_LABEL_SIZE) {
        printf("Error in lineStr %d: label is too long\n", linNum);
        return false;
    }
    if (!isalpha(label[0])) {
        printf("Error in lineStr %d: label must start with a letter\n", linNum);
        return false;
    }
    for ( i = 0;i<strlen(label);i++){
        if (!isalnum(label[i])){
            printf("Error in lineStr %d: label must contain only letters and numbers\n", linNum);
            return false;
        }
    }
    if(!checkValidMacroName(macroTable,label,linNum)){
        return false;
    }
return true;
}
label *getLabel(char *label, symbolTable *labelList){
    char *temp = NULL ;
    int i = 0;
    for (i = 0; i < labelList->capacity; ++i) {
        temp = labelList->labels[i]->symbolName;
        if (strcmp(label, temp) == 0) {
            return labelList->labels[i];
        }
    }
    return NULL;
}

void insertLabelTable(char *l, symbolTable *labelTable, int lineNum, label **currentLabel, lDType type,bool defined){
    label *label = NULL;
    ALLOCATE(label, sizeof(label));
    ALLOCATE(label->symbolName, sizeof(char)*strlen(l));
    strncpy(label->symbolName, l, strlen(l));
    label->symbolName[strlen(l)] = '\0';
    label->defineType = type;
    label->address = 0;
    label->defined = defined;
    if (labelTable->size == labelTable->capacity) {
        labelTable->size += 10;
        REALLOCTION(*labelTable->labels, labelTable->size);
    }
    *currentLabel = label;
    labelTable->labels[labelTable->capacity++] = label;
}
