#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "code.h"

char** splitString( char* input,int *countWords,char *operands){
    char *str = strdup(input);
    int count = 0;
    char * token;
    char ** substrings = malloc(sizeof(char *) * MAX_LINE_LENGTH);
    str[strlen(str) - 1] = '\n';
    token= strtok(str, operands);

    while (token != NULL) {
        substrings[count] = strdup(token);
        count++;
        token = strtok(NULL, operands);
    }
    substrings[count] = NULL;
    *countWords = count;
    free(str);
    free(token);
    return substrings;
}
void insertMacro(macroTable *macroTable, char *name, char *content,macro **currMacro) {

    int i;
    macro *newMacro;
    for (i = 0; i < macroTable->capacity; ++i) {
        if (strcmp(macroTable->content[i]->name, name) == 0) {
            printf("Error: Macro %s already defined\n", name);
            return;
        }
    }
      if (macroTable->capacity == macroTable->size){
          macroTable= realloc(macroTable, (macroTable->size + TABLE_SIZE ) * sizeof(macroTable));
          macroTable->size += TABLE_SIZE;
      }
      newMacro = createNewMacro(name, content);
      macroTable->content[macroTable->capacity] = newMacro;
      macroTable->capacity++;
      *currMacro = newMacro;

}
char* getMacroContent(macroTable * macroTable, char *substring){
    int i;
    for (i = 0; i < macroTable->capacity; ++i) {
        if (strcmp(macroTable->content[i]->name, substring) == 0) {
            return macroTable->content[i]->content;
        }
    }
    return NULL;
}
bool checkValidMacroName(macroTable *table, char *name, int lineNum){
    int i;
    if (!isalpha(name[0])) {
        printf("Error in lineStr %d: macro name must start with a letter\n",lineNum);
        return false;
    }
    for (i = 0; i < table->capacity; ++i) {
        if (strcmp(table->content[i]->name, name) == 0) {
            printf("Error in lineStr %d: macro name already exists\n",lineNum);
            return false;
        }
    }
    for (i = 1; i < strlen(name); ++i) {
        if (!isalpha(name[i]) && !isdigit(name[i])) {
            printf("Error in lineStr %d: macro name must contain only letters and numbers\n",lineNum);
            return false;
        }
    }
    if (!checkValidReservedName(name,lineNum)) {
        return false;
    }
    return true;
}
bool checkValidReservedName(char *name,int lineNum){
    const char *reservedWords[] = {"@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7",
                                    "mov", "cmp", "add", "sub", "not", "clr", "lea",
                                   "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop",
                                   ".data", ".string", ".entry", ".extern"};
    int i;
    for (i = 0; i < 28; ++i) {
        if (strcmp(name, reservedWords[i]) == 0) {
            printf("Error in lineStr %d: label is a reserved word\n",lineNum);
            return false;
        }
    }
    return true;
}
macro *createNewMacro(const char *macroName, const char *macroContent){
    macro *newMacro;
    ALLOCATE( newMacro, sizeof (macro) );
    ALLOCATE( newMacro->name,strlen(macroName) + 1 );
    ALLOCATE( newMacro->content, strlen(macroContent) + 1 );
    strcpy(newMacro->name, macroName);
    strcpy(newMacro->content , macroContent);
    return newMacro;
}
void addContentToMacro(macro *currMacro, char *newContent){
    newContent[strlen(newContent) - 1] = '\n';
    size_t currLen = strlen(currMacro->content);
    size_t newLen = strlen(newContent);
    char* str ;
    ALLOCATE(str, currLen + newLen + 1);

    strcpy(str, currMacro->content);
    strcat(str, newContent);

    free(currMacro->content);
    currMacro->content = str;

}
bool checkComma(char* str) {
    bool consecutiveCommas = false;
    int i;
    for ( i = 0; str[i] != '\0'; i++) {
        if (str[i] == ',') {
           if (consecutiveCommas) {
               return true;
           }
           consecutiveCommas = true;
        } else if (str[i] != ' ') {
           consecutiveCommas = false;
        }
    }
    return false;
}
void removeDoubleWhitespaces(char* str) {
    int i, j;
    int length = strlen(str);
    int foundDoubleSpace = 0;
    for (i = 0, j = 0; i < length; ++i) {
    if (isspace(str[i])) {
       if(i == 0) {
           continue;
       }
       if (foundDoubleSpace) {
           continue;
       }
       foundDoubleSpace = true;
    } else {
       foundDoubleSpace = false;
    }

    str[j++] = str[i];
    }
    str[j] = '\0';
}
char * giveNameFile(char *filename, char *extension){
    char *newFileName = (char*) malloc((strlen(filename) + strlen(extension) + 1) * sizeof(char));
    if (newFileName == NULL){
        printf("Error: malloc has failed\n");
        free(newFileName);
        return NULL;
    }
    strcpy(newFileName, filename);
    strcat(newFileName, extension);
    return newFileName;
}