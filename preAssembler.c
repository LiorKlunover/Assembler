#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "preAssembler.h"

bool preProcessFile( char *filename, macroTable *macroTable){
    FILE *fp;
    FILE *newFile;
    bool error = true;
    char  *filenameAs = giveNameFile(filename, ".as");
    char  *filenameAm = giveNameFile(filename, ".am");

    /*fp = fopen(filenameAs, "r");*/
    fp = fopen("C:\\Users\\lior3\\CLionProjects\\Assembler\\test1.as", "r");

    if (fp == NULL) {
        printf("Error: file %s cannot be opened\n", filename);
        free(filenameAs);
        free(filenameAm);
        return false;
    }
    newFile = fopen(filenameAm, "w");

    if (newFile == NULL) {
        printf("Unable to create the file.\n");
        free(filenameAs);
        free(filenameAm);
        fclose(fp);
        return false;
    }

    printf("File %s read in pre processed\n", filename);
    error = lineProcess(fp, newFile, macroTable);

    fclose(newFile);
    fclose(fp);

    free(filenameAs);
    free(filenameAm);
    return error;
}

bool lineProcess(FILE *fp,  FILE *newFile, macroTable *macroTable){
    char *currLine,*content;
    int index ,countWords,lineNum = 0;
    lineStr *lineContent;
    macro **currMakro= malloc(sizeof(macro));
    bool error = true, inMacroMode = false;
    ALLOCATE(lineContent, sizeof(lineStr));
    ALLOCATE(currLine, MAX_LINE_LENGTH);

    if (currLine == NULL){
        printf("Error: malloc has failed\n");
        return false;
    }
    while (fgets(currLine, MAX_LINE_LENGTH, fp) != NULL) {
        index= 0;
        lineNum++;
        SKIP_WHITE(currLine, index)
        lineContent->lineContent = splitString(currLine, &countWords, " \n");
        lineContent->lineNum = lineNum;
        lineContent->size = countWords;

        if (countWords == 1 && (content = getMacroContent(macroTable, lineContent->lineContent[0])) != NULL){
            fputs(content, newFile);
            continue;
        }
        if(inMacroMode){
            if (strstr(currLine,"endmcro") != 0){
                if (countWords != 1 || strcmp(lineContent->lineContent[0],"endmcro") != 0){
                    printf("Error in lineStr %d: invalid macro definition\n",lineContent->lineNum);
                    return false;
                }
                inMacroMode = false;
                continue;
            }
            addContentToMacro(*currMakro, currLine + index);
            continue;
        }

        if (strstr(currLine,"mcro") != 0){

            if(countWords != 2 || strcmp(lineContent->lineContent[0],"mcro") != 0){
                printf("Error in lineStr %d: invalid macro definition\n",lineContent->lineNum);
                return false;
            }
            if (!checkValidMacroName(macroTable, lineContent->lineContent[1], lineContent->lineNum)){
                return false;
            }
            inMacroMode = true;
           insertMacro(macroTable, lineContent->lineContent[1], "", currMakro);
            continue;
        }

        fputs(currLine + index, newFile);
    }
    free(currLine);
    free(lineContent);
    free(currMakro);
    return true;
}