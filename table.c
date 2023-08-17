#include "table.h"
#include <stdio.h>
#include <stdlib.h>

macroTable *creatMacroTable(){
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
symbolTable *creatSymbolTable(){
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
lexTable *createLexStruct(){
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
objectFileContent * makeObjectFileContent(){
    int i;
    objectFileContent *objectFileContent = malloc(sizeof(objectFileContent));
    objectFileContent->content = malloc(TABLE_SIZE * sizeof(char*));
    objectFileContent->size = TABLE_SIZE;
    objectFileContent->capacity = 0;
    for (i = 0; i < TABLE_SIZE; ++i) {
        objectFileContent->content[i] = NULL;
    }
    return objectFileContent;
}
externFileContent * makeExternFileContent(){
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
entryFileContent * makeEntryFileContent(){
    int i;
    entryFileContent *entryFileContent = malloc(sizeof(entryFileContent));
    entryFileContent->content = malloc(TABLE_SIZE * sizeof(char*));
    entryFileContent->size = TABLE_SIZE;
    entryFileContent->capacity = 0;
    for (i = 0; i < TABLE_SIZE; ++i) {
        entryFileContent->content[i] = NULL;
    }
    return entryFileContent;
}
