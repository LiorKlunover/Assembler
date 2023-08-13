
#include "globals.h"

#ifndef CODE_H
#define CODE_H
#define MAX_LINE_LENGTH 80
#define TABLE_SIZE 10

void removeDoubleWhitespaces(char* str);
char * giveNameFile(char *filename, char *extension);
char** splitString( char* input,int *countWords,char *operands);
bool checkComma(char* str);
void insertMacro(macroTable *macroTable, char *name, char *content,macro **currMacro) ;
macro *createNewMacro(const char *macro_name, const char *macro_content);
void addContentToMacro(macro *currMacro, char *newContent);
char * getMacroContent(macroTable * macroTable, char *substring);
bool checkValidMacroName(macroTable *table, char *name, int lineNum);
bool checkValidReservedName(char *name,int lineNum);
#endif