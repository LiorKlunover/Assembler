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
void reallocateLexTable(lexTable *lexTable){
    int i;
    lexTable->size +=  TABLE_SIZE;
    REALLOCTION(*lexTable->content, lexTable->size);
    for (i = lexTable->capacity; i < lexTable->size; ++i) {
        lexTable->content[i] = NULL;
    }
}