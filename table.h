#include "lexer.h"
#include "globals.h"
#ifndef TABLE_H
#define TABLE_H
#define TABLE_SIZE 10
#define MAKE_TYPE_CONTENT_TABLE(fileName) { \
    ALLOCATE(fileName, sizeof(*fileName)); \
    ALLOCATE(fileName->content, TABLE_SIZE * sizeof(char*)); \
    fileName->size = TABLE_SIZE; \
    fileName->capacity = 0; \
}
lexTable *createLexStruct();
macroTable *creatMacroTable();
symbolTable *creatSymbolTable();
void reallocateLexTable(lexTable *lexTable);
#endif
