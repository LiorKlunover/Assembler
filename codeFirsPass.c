#include "codeFirsPass.h"
#include "code.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
char** splitStringForFirstpass( char* input,int *countWords){
    char *str = strdup(input);
    int count = 0;
    char* token= NULL;
    char** substrings = NULL;

    token= strtok(str, ",");

    while (token != NULL) {
        substrings = realloc(substrings, (count + 1) * sizeof(char*));
        REALLOCTION(substrings, (count + 1) * sizeof(char*));
        substrings[count] = strdup(token);
        count++;
        token = strtok(NULL, ",");
    }
    REALLOCTION(substrings, (count + 1) * sizeof(char*));
    substrings[count] = NULL;
    *countWords = count;
    free(str);
    return substrings;
}