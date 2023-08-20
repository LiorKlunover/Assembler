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
    lexTable *lexTable;
    symbolTable *symbolTable;
    macroTable *macroTable;
    for (i = 1; i < argc; ++i) {
        lexTable = createLexStruct();
         symbolTable = creatSymbolTable();
         macroTable = creatMacroTable();
       if(!preProcessFile(argv[i], macroTable)){
           for(j = 0; j < macroTable->capacity; ++j){
               free(macroTable->content[j]->name);
               free(macroTable->content[j]->content);
               free(macroTable->content[j]);
           }
           free(macroTable->content);
           free(macroTable);           printf("File %s pre processed file failed in main\n", argv[i]);
           continue;
       }
        printf("File %s pre processed succeeded\n\n", argv[i]);

       if(!firstPass(argv[i], macroTable,lexTable,symbolTable)){
           for(j = 0; j < symbolTable->capacity; ++j){
               free(symbolTable->content[j]->symbolName);
               free(symbolTable->content[j]);
           }
           free(symbolTable->content);
           free(symbolTable);
           for(j = 0; j < macroTable->capacity; ++j){
               free(macroTable->content[j]->name);
               free(macroTable->content[j]->content);
               free(macroTable->content[j]);
           }
           free(macroTable->content);
           free(macroTable);
            printf("File %s  first pass failed in main\n\n", argv[i]);
            continue;
        }
        printf("File %s firstPass succeeded\n\n", argv[i]);
        for(j = 0; j < macroTable->capacity; ++j){
            free(macroTable->content[j]->name);
            free(macroTable->content[j]->content);
            free(macroTable->content[j]);
        }
        free(macroTable->content);
        free(macroTable);

       if(!secondPass(argv[j],lexTable,symbolTable)){
           for(j = 0; j < lexTable->capacity; ++j){
               free(lexTable->content[j]);
           }
           free(lexTable->content);
           free(lexTable);
           for(j = 0; j < symbolTable->capacity; ++j){
               free(symbolTable->content[j]->symbolName);

               free(symbolTable->content[j]);
           }
           free(symbolTable->content);
           free(symbolTable);

            printf("File %s Second pass failed in main\n\n", argv[i]);
            continue;
        }
        printf("File %s Second pass succeeded\n\n", argv[i]);
        for(j = 0; j < lexTable->capacity; ++j){
            free(lexTable->content[j]);
        }
        free(lexTable->content);
        free(lexTable);
        for(j = 0; j < symbolTable->capacity; ++j){
            free(symbolTable->content[j]->symbolName);
            free(symbolTable->content[j]);

        }

        free(symbolTable->content);
        free(symbolTable);

    }
    return 0;
}