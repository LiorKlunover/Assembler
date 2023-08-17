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
           FREE_STRUCT(macroTable,macroTable);
           printf("File %s pre processed file failed in main\n", argv[i]);
           continue;
       }
        printf("File %s pre processed succeeded\n\n", argv[i]);

       if(!firstPass(argv[i], macroTable,lexTable,symbolTable)){
           FREE_STRUCT(symbolTable,symbolTable);
           FREE_STRUCT(macroTable,macroTable);
            printf("File %s  first pass failed in main\n\n", argv[i]);
            continue;
        }
        printf("File %s firstPass succeeded\n\n", argv[i]);


       if(!secondPass(argv[i],lexTable,symbolTable)){
           for(i = 0; i < lexTable->capacity; ++i){
               free(lexTable->content[i]);
           }
           free(lexTable->content);
           free(lexTable);
           FREE_STRUCT(symbolTable,symbolTable);
           FREE_STRUCT(macroTable,macroTable);
            printf("File %s Second pass failed in main\n\n", argv[i]);
            continue;
        }
        printf("File %s Second pass succeeded\n\n", argv[i]);
        for(i = 0; i < lexTable->capacity; ++i){
            free(lexTable->content[i]);
        }
        free(lexTable->content);
        free(lexTable);
        for(i = 0; i < lexTable->capacity; ++i){
            free(symbolTable->content[i]);
        }
        free(symbolTable->content);
        free(symbolTable);
        for(i = 0; i < macroTable->capacity; ++i){
            free(macroTable->content[i]);
        }
        free(macroTable->content);
        free(macroTable);
    }
    return 0;
}