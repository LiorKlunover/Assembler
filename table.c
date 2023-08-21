#include "table.h"
#include <stdio.h>
#include <stdlib.h>

macroTable *createMacroTable(){
    int i;
    macroTable *macroTable =  NULL;
    ALLOCATE(macroTable, sizeof(macroTable));
    ALLOCATE(macroTable->content, TABLE_SIZE * sizeof(macro*));
    macroTable->size = TABLE_SIZE;
    macroTable->capacity = 0;
    for (i = 0; i < TABLE_SIZE; ++i) {
        macroTable->content[i] = NULL;
    }
    return macroTable;
}
symbolTable *createSymbolTable(){
    int i;
    symbolTable *symbolTable = NULL;
    ALLOCATE(symbolTable, sizeof(symbolTable));
    ALLOCATE(symbolTable->content, (TABLE_SIZE * sizeof(label*)));
    symbolTable->size = TABLE_SIZE;
    symbolTable->capacity = 0;
    for (i = 0; i < TABLE_SIZE; ++i) {
        symbolTable->content[i] = NULL;
    }
    return symbolTable;
}
lexTable *createLexStructTable(){
    int i;
    lexTable *lexTable = NULL;
    ALLOCATE(lexTable, sizeof(lexTable));
    ALLOCATE(lexTable->content, TABLE_SIZE * sizeof(lexStruct*));
    lexTable->size = TABLE_SIZE;
    lexTable->capacity = 0;
    for (i = 0; i < TABLE_SIZE; ++i) {
        lexTable->content[i] = NULL;
    }
    return lexTable;
}
objectFileContent * makeObjectFileContentTable(){
    int i;
    objectFileContent *objectFileContent = malloc(sizeof(objectFileContent));
    objectFileContent->content = malloc(TABLE_SIZE * sizeof(char*));
    objectFileContent->size = TABLE_SIZE;
    objectFileContent->capacity = 0;
    if(objectFileContent->content == NULL || objectFileContent == NULL){
        printf("malloc failed in makeObjectFileContent\n");
        exit(0);
    }

    for (i = 0; i < TABLE_SIZE; ++i) {
        objectFileContent->content[i] = NULL;
    }
    return objectFileContent;
}
externFileContent * makeExternFileContentTable(){
    int i;
    externFileContent *externFileContent = malloc(sizeof(externFileContent));
    externFileContent->content = malloc(TABLE_SIZE * sizeof(char*));
    externFileContent->size = TABLE_SIZE;
    externFileContent->capacity = 0;
    for (i = 0; i < TABLE_SIZE; ++i) {
        externFileContent->content[i] = NULL;
    }
    return externFileContent;
}

