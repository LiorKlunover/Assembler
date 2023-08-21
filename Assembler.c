#include <stdio.h>
#include <stdlib.h>
#include "preAssembler.h"
#include "firstPass.h"
#include "secondPass.h"
#include "code.h"
#include "lexer.h"
#include "table.h"
void freeLexTable(lexTable *lexTable);
void freeMacroTable(macroTable *macroTable);
void freeSymbolTable(symbolTable *symbolTable);
int main( int argc, char *argv[] ) {
    int i;
    lexTable *lexTable;
    symbolTable *symbolTable;
    macroTable *macroTable;
    for (i = 1; i < argc; ++i) {
         macroTable = createMacroTable();

       if(!preProcessFile(argv[i], macroTable)){
           freeMacroTable(macroTable);
           printf("File %s pre processed file failed in main\n", argv[i]);
           continue;
       }

        symbolTable = createSymbolTable();
        lexTable = createLexStructTable();

        printf("File %s pre processed succeeded\n\n", argv[i]);

       if(!firstPass(argv[i], macroTable,lexTable,symbolTable)){
           freeSymbolTable(symbolTable);
           freeMacroTable(macroTable);
            printf("File %s  first pass failed in main\n\n", argv[i]);
            continue;
        }

        printf("File %s firstPass succeeded\n\n", argv[i]);

        freeMacroTable(macroTable);

       if(!secondPass(argv[i],lexTable,symbolTable)){
           freeLexTable(lexTable);
           freeSymbolTable(symbolTable);
            printf("File %s Second pass failed in main\n\n", argv[i]);
            continue;
        }

        printf("File %s Second pass succeeded\n\n", argv[i]);

        freeLexTable(lexTable);
        freeSymbolTable(symbolTable);

    }
    return 0;
}

void freeLexTable(lexTable *lexTable){
    int i;
    for(i = 0; i < lexTable->capacity; ++i){
        free(lexTable->content[i]);
    }
    free(lexTable->content);
    free(lexTable);
}

void freeSymbolTable(symbolTable *symbolTable){
    int i;
    for(i = 0; i < symbolTable->capacity; ++i){
        free(symbolTable->content[i]->symbolName);
        free(symbolTable->content[i]);
    }
    free(symbolTable->content);
    free(symbolTable);
}

void freeMacroTable(macroTable *macroTable){
    int i;
    for(i = 0; i < macroTable->capacity; ++i){
        free(macroTable->content[i]->name);
        free(macroTable->content[i]->content);
        free(macroTable->content[i]);
    }
    free(macroTable->content);
    free(macroTable);
}