#include "table.h"
#include <stdio.h>
#include <stdlib.h>

macroTable *creatMacroTable(){
    int i;
    macroTable *macroTable =  malloc(sizeof(*macroTable));
    macroTable->tableList = malloc(TABLE_SIZE * sizeof(macro*));
    macroTable->size = TABLE_SIZE;
    macroTable->count = 0;
    for (i = 0; i < TABLE_SIZE; ++i) {
        macroTable->tableList[i] = NULL;
    }
    return macroTable;
}
symbolTable *creatSymbolTable(){
    int i;
    symbolTable *symbolTable = NULL;
    ALLOCATE(symbolTable, sizeof(symbolTable));
    ALLOCATE(symbolTable->labels, 10 * sizeof(label*));
    symbolTable->size = 10;
    symbolTable->capacity = 0;
    for (i = 0; i < TABLE_SIZE; ++i) {
        symbolTable->labels[i] = NULL;
    }
    return symbolTable;
}
lexTable *createLexStruct(){
    int i;
    lexTable *lexTable = NULL;
    ALLOCATE(lexTable, sizeof(lexTable));
    ALLOCATE(lexTable->lexStructList,TABLE_SIZE * sizeof(lexStruct*));
    lexTable->size = TABLE_SIZE;
    lexTable->capacity = 0;
    for (i = 0; i < TABLE_SIZE; ++i) {
        lexTable->lexStructList[i] = NULL;
    }
    return lexTable;
}
void reallocateLexTable(lexTable *lexTable){
    int i;
    lexTable->size +=  TABLE_SIZE;
    REALLOCTION(*lexTable->lexStructList, lexTable->size);
    for (i = lexTable->capacity; i < lexTable->size; ++i) {
        lexTable->lexStructList[i] = NULL;
    }
}