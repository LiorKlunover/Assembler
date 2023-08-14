#include <stdio.h>
#include <stdlib.h>
#include "preAssembler.h"
#include "firstPass.h"
#include "secondPass.h"
#include "code.h"
#include "lexer.h"
#include "table.h"

int main( int argc, char *argv[] ) {
    int i,j;
    lexTable *lexTable = createLexStruct();
    symbolTable *symbolTable = creatSymbolTable();
    macroTable *macroTable = creatMacroTable();
    for (i = 1; i < argc; ++i) {
       if(!preProcessFile(argv[i], macroTable)){
           printf("File %s pre processed file failed in main\n", argv[i]);
           continue;
       }
        printf("File %s pre processed succeeded\n\n", argv[i]);

       if(!firstPass(argv[i], macroTable,lexTable,symbolTable)){
           FREE_STRUCT(lexTable,lexTable);
           FREE_STRUCT(symbolTable,symbolTable);
            printf("File %s  first pass failed in main\n\n", argv[i]);
            continue;
        }
        printf("File %s firstPass succeeded\n\n", argv[i]);
        FREE_STRUCT(macroTable,macroTable);

       if(!secondPass(argv[i],lexTable,symbolTable)){
            printf("File %s Second pass failed in main\n\n", argv[i]);
            continue;
        }
        printf("File %s Second pass succeeded\n\n", argv[i]);
        for ( j = 0; j < lexTable->capacity; ++j) {
           free(lexTable->content[j]  );
        }
           free(lexTable->content);
        for (j = 0; j < symbolTable->capacity; ++j) {
            free(symbolTable->content[j]);
        }

        free(lexTable);
        free(symbolTable);
    }
    return 0;
}