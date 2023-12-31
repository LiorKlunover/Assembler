#include "lexer.h"
#include "firstPass.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

lexStruct * getLexTreePosition(lineStr *line, symbolTable *labelTable, int * IC, int * DC, macroTable *macroTable){
    lexStruct *lexTree = NULL;
    opCode instName;
    bool labelFlag = false, dirFlag = false, instFlag = false;
    char **lineC = line->lineContent;
    label *currLabel = NULL;
    char *token =NULL,*ptr = NULL,*temp;
    int len = 0,index = 0;
    lexTree = malloc(sizeof(lexStruct));
    lexTree->label = NULL;
    ptr = malloc(sizeof(char) * MAX_LINE_LENGTH);
    if (ptr == NULL|| lexTree == NULL) {
        printf("Error: malloc has failed\n");
        free(ptr);
        free(lexTree);
        return NULL;
    }
    temp = ptr;

    for(index = 0; index < line->size; index++) {

        ptr = strcpy(ptr,lineC[index]);
        len = (int) strlen(ptr);
        ptr[len] = '\0';
        /** check if ptr is label **/
        if (len > 0 && ptr[len-1]  == ':') {
            ptr[len-1] = '\0';
            if (!isValidLabel(ptr, line->lineNum, labelTable, macroTable)) {
                free(temp);
                free(lexTree);
                return NULL;
            }

            if (strlen(*lineC)>index+1 && (strstr(lineC[index+1],"extern")  || strstr(lineC[index+1],"entry"))){
                printf("Warning label was define before entry\\extern in line: %d \n", line->lineNum);
                continue;
            }

            if((currLabel = getLabel(ptr, labelTable)) == NULL){
                insertLabelTable(ptr, labelTable, &lexTree->label, notEntryOrExtern,true);

            }else if (currLabel->defineType == notEntryOrExtern ){
                printf("Error in line: %d , label was define before \n", line->lineNum);
                free(temp);
                free(lexTree);
                return NULL;

            } else if(currLabel->defineType == externLabel){
                printf("Error in line: %d ,can't define extern label  \n", line->lineNum);
                free(temp);
                free(lexTree);
                return NULL;

            }else if(currLabel->defineType == operandDefined){
                currLabel->defineType = notEntryOrExtern;

            }else if(currLabel->address != 0){
                printf("Error in line: %d ,label was define before \n", line->lineNum);
                free(temp);
                free(lexTree);
                return NULL;
            }
            if(currLabel != NULL && !currLabel->defined){
                currLabel->defined = true;
            }
            labelFlag = true;
            continue;
        }
        /** check if ptr is directive **/
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
                free(temp);
                free(lexTree);
                return NULL;
            }
            lexTree->lineType = lexDir;
            dirFlag = true;
            continue;
        }
        /*get directive data*/
        else if (dirFlag) {

            while (++index<line->size){
                strcat(ptr, " ");
                strcat(ptr, lineC[index]);
            }

            if (lexTree->lexType.dirType.lexDirType == lexDirData){
                if(!getData(ptr, lexTree->lexType.dirType.dirUnionContent.dataType.numArray,
                        &lexTree->lexType.dirType.dirUnionContent.dataType.numCount, &line->lineNum)){
                    free(temp);
                    free(lexTree);
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

               if ((ptr = isValidString(ptr)) == NULL) {

                   printf("Error in lineStr %d: invalid string\n", line->lineNum);
                   free(temp);
                   free(lexTree);
                   return NULL;
               }

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
                    insertLabelTable(ptr, labelTable,&lexTree->label,entryLabel,false);

                }else if(currLabel->defineType == notEntryOrExtern ||currLabel->defineType == operandDefined){
                        currLabel->defineType = entryLabel;

                }else if(currLabel->defineType == entryLabel){
                    printf("Warning in line: %d , label was define before \n", line->lineNum);

                }else {
                    printf("Error in line: %d , label was define as extern \n", line->lineNum);
                    free(temp);
                    free(lexTree);
                    return NULL;
                }

            }else if (lexTree->lexType.dirType.lexDirType == lexDirExtern) {
                token = strtok(ptr, " ,");
                while (token != NULL) {
                    if ((currLabel = getLabel(token, labelTable)) == NULL) {
                        insertLabelTable(token, labelTable, &lexTree->label, externLabel,false);
                        currLabel = getLabel(token, labelTable);

                    }else if(currLabel->defined){
                        printf("Error in line: %d , label was define as entry \n", line->lineNum);
                        free(temp);
                        free(lexTree);
                        return NULL;

                    } else if (currLabel->defineType == notEntryOrExtern || currLabel->defineType == operandDefined) {
                        currLabel->defineType = externLabel;

                    } else if (currLabel->defineType == externLabel) {
                        printf("Warning in line: %d , label was define before \n", line->lineNum);
                    }

                    currLabel->address = 1;
                    token = strtok(NULL, " ,");
                }

            }else {

                printf("Error in lineStr %d: invalid directive\n", line->lineNum);
                free(ptr);
                free(lexTree);
                return NULL;
            }
         /*get instruction operands*/
        } else if (( instName = (opCode)getOpCode(ptr, &line->lineNum,&lexTree->lexType.instType.numOfOperands)) != -1) {
                lexTree->lexType.instType.instName = instName;
                lexTree->lexType.instType.OpeInstTypes.arrOpType[0] = -1;
                lexTree->lexType.instType.OpeInstTypes.arrOpType[1] = -1;
                instFlag = true;

                if (labelFlag) {
                    if(lexTree->label == NULL)
                        lexTree->label = currLabel;
                    lexTree->label->address = (*DC)+100+(*IC);
                }

                *IC += 1 + lexTree->lexType.instType.numOfOperands;
                lexTree->lineType = lexInst;

        }else if(instFlag){

            while (++index<line->size){
                strcat(ptr, lineC[index]);
            }

            if (!getOperands(ptr,lexTree ,&line->lineNum, labelTable, macroTable)){
                free(temp);
                free(lexTree);
                return NULL;
            }

            if(lexTree->lexType.instType.OpeInstTypes.arrOpType[0] == registerType && lexTree->lexType.instType.OpeInstTypes.arrOpType[1] == registerType){
                --(*IC);
            }

        } else{

            printf("Error in line: %d , invalid input \n", line->lineNum);
            free(temp);
            free(lexTree);
            return NULL;
        }
    }

    if (instFlag){
        if (!checkOperandOrder(lexTree->lexType.instType.instName,lexTree->lexType.instType.OpeInstTypes.arrOpType,
                               &line->lineNum)){
            printf("Error in line: %d , invalid operand \n", line->lineNum);
            free(temp);
            free(lexTree);
            return NULL;
        }
    }

    free(temp);
    return lexTree;
}

char *isValidString(char *str) {
    char *newStr;
    char *result = (char *) malloc(sizeof(char) * (strlen(str) + 1));
    int len,left = 0, right;
   if (result == NULL) {
        printf("Error: malloc has failed\n");
        return NULL;
    }
    strcpy(result, str);
    len = strlen(result);
    right = len - 1;
    if (result[left++] != '\"' || result[right--] != '\"'){
        free(result);
        return NULL;
    }
    if (left > right) {
        free(result);
        return NULL;
    }
    if (left < len && result[left] == ' ')
        left++;
    if (right >= 0 && result[right] == ' ')
        right--;
    result[right + 1] = '\0';
    newStr= (char *) malloc(sizeof(char) * (right - left + 2));
    if (newStr == NULL) {
        printf("Error: malloc has failed\n");
        free(result);
        return NULL;
    }
    strcpy(newStr, result + left);
    free(result);
    return newStr;
}

bool getOperands(char *str , lexStruct *lexTree, int *lineNum, symbolTable *labelTable, macroTable *macroTable){
    int num;
    int len =strlen(str)+1;
    operandAddrTypeName tempArrOpName;
    operandAddrType tempArrOpType = -1;
    char *copy = (char *)malloc(len * sizeof (char));
    char * token;
    if (copy == NULL) {
        printf("Error: malloc has failed\n");
        return false;
    }

    strcpy(copy, str);
    token = strtok(copy, ",");
    while (token != NULL) {
        if (lexTree->lexType.instType.OpeInstTypes.arrOpType[0] != -1){
            printf("Error in line: %d , too many operands \n", *lineNum);
            free(copy);
            return false;
        }

        else if (token[0] == '@'){
            if (strlen(token) == 3 && token[1] == 'r' && token[2]<='7' && token[2] >= '0'){
                tempArrOpType= registerType;
                tempArrOpName.reg = token[2]-'0';
            }else{
                printf("Error in line: %d , invalid register \n", *lineNum);
                free(copy);
                return false;
            }
        }

        else if(getNumber(token, &num)){
            if (num > MAX_DATA_VALUE || num < MIN_DATA_VALUE) {
                printf("Error in line: %d , immediate value is out of range \n", *lineNum);
                free(copy);
                return false;
            }
            tempArrOpType = immediate;
            tempArrOpName.immed = num;
        }

        else if(isValidLabel(token,*lineNum,labelTable,macroTable)){
            tempArrOpType = labelType;
            if ((tempArrOpName.label = getLabel(token, labelTable)) == NULL) {
                insertLabelTable(token, labelTable, &tempArrOpName.label, operandDefined,false);
            }
        }

        else{
            printf("Error in line: %d , invalid operand \n", *lineNum);
            free(copy);
            return false;
        }

        if (lexTree->lexType.instType.OpeInstTypes.arrOpType[1] != -1){
            lexTree->lexType.instType.OpeInstTypes.arrOpType[0] = lexTree->lexType.instType.OpeInstTypes.arrOpType[1];
            lexTree->lexType.instType.OpeInstTypes.arrOpName[0] = lexTree->lexType.instType.OpeInstTypes.arrOpName[1] ;
        }

        lexTree->lexType.instType.OpeInstTypes.arrOpType[1] = tempArrOpType;
        lexTree->lexType.instType.OpeInstTypes.arrOpName[1] = tempArrOpName;

        token = strtok(NULL, ",");
    }

    free(copy);
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
bool getData(const char *inputString, int *numArray, int *numCount,int *lineNum) {
    char *token;
    int len = strlen(inputString) + 1;
    char *copy = (char *) malloc(len * sizeof (char));
    strcpy(copy, inputString);
    if(!checkInvalidLeftComma(copy) || !checkInvalidRightComma(copy)){
        printf("Error in line %d: Invalid comma\n", *lineNum);
        free(copy);
        return false;
    }
    token = strtok(copy, ",");

    while (token != NULL) {
        if (*numCount >= MAX_DATA_VALUE) {
            printf("Error in line %d: Exceeded maximum number of values\n", *lineNum);
            if (copy != NULL) {
                free(copy);
            }
            return false;
        }
        if (!isValidNumber(token)) {
            printf("Error in line %d: Not a valid number\n", *lineNum);
            if (copy != NULL) {
                free(copy);
            }
            return false;
        }
        numArray[*numCount] = atoi(token);
        if (numArray[*numCount] > MAX_DATA_VALUE || numArray[*numCount] < MIN_DATA_VALUE) {
            printf("Error in line %d: number is out of range\n", *lineNum);
            if (copy != NULL) {
                free(copy);
            }
            return false;
        }
        (*numCount)++;
        token = strtok(NULL, ",");
    }
    if (numCount == 0) {
        printf("Error in line %d: No valid numbers found\n", *lineNum);

    }

    if (copy != NULL) {
        free(copy);
    }
    return true;
}
bool checkInvalidLeftComma(char *str) {
    int i;
    int len = strlen(str);
    for (i = 0; i < len; ++i) {
        if (str[i] == ' ' || str[i] == '\t') {
            continue;
        }
        if (str[i] != ',') {
            return true;
        }else

            return false;
    }
    return false;
}
bool checkInvalidRightComma(char *str) {
    int i;
    int len = strlen(str);
    for (i = len-1; i >=0; --i) {
        if (str[i] == ' ' || str[i] == '\t') {
            continue;
        }
        if (str[i] != ',') {
            return true;
        }else
            return false;
    }
    return false;
}
int isValidNumber(char *str) {
    if (*str == ' ') {
        str++;
    }
    if (str[strlen(str) - 1] == ' ') {
        str[strlen(str) - 1] = '\0';
    }

    while (*str != '\0') {
        if (!isdigit(*str) && *str != '-') {
            return 0;
        }
        str++;
    }
    return 1;
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
        temp = labelList->content[i]->symbolName;
        if (strcmp(label, temp) == 0) {
            return labelList->content[i];
        }
    }
    return NULL;
}

void insertLabelTable(char *l, symbolTable *labelTable, label **currentLabel, lDType type,bool defined){
    label *label = NULL;
    int len = strlen(l);
    ALLOCATE(label, sizeof(label));
    ALLOCATE(label->symbolName, sizeof(char) * len+1);
    strncpy(label->symbolName, l, len);
    label->symbolName[len] = '\0';
    label->defineType = type;
    label->address = 0;
    label->defined = defined;
    if (labelTable->size == labelTable->capacity) {
        labelTable->size += 10;
        REALLOCTION(*labelTable->content, labelTable->size);
    }
    *currentLabel = label;
    labelTable->content[labelTable->capacity++] = label;
}
