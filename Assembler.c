#include <stdio.h>
#include <stdlib.h>
#include "preAssembler.h"
#include "firstPass.h"
#include "secondPass.h"
#include "code.h"
#include "lexer.h"
#include "table.h"
int main( int argc, char *argv[] ) {
    int i;
    macroTable *macroTable = creatMacroTable();
    lexTable *lexTable = createLexStruct();
    symbolTable *symbolTable = creatSymbolTable();
    for (i = 1; i < argc; ++i) {
       if(!preProcessFile(argv[i], macroTable)){
           printf("File %s pre processed file failed in main\n", argv[i]);
           continue;
       }
        printf("File %s pre processed succeeded\n\n", argv[i]);
       if(!firstPass(argv[i], macroTable,lexTable,symbolTable)){
            printf("File %s  first pass failed in main\n\n", argv[i]);
            continue;
        }
        printf("File %s firstPass succeeded\n\n", argv[i]);
        if(!secondPass(argv[i],lexTable,symbolTable)){
            printf("File %s Second pass failed in main\n\n", argv[i]);
            continue;
        }
    }
    return 0;
}