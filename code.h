
#include "globals.h"

#ifndef CODE_H
#define CODE_H

#define MAX_LINE_LENGTH 80
#define TABLE_SIZE 10

/**
 * Removes consecutive double whitespaces from the given string.
 *
 * @param str The input string to remove double whitespaces from.
 */
void removeDoubleWhitespaces(char* str);

/**
 * Generates a new filename by combining an existing filename and an extension.
 *
 * @param filename The base filename.
 * @param extension The extension to append to the filename.
 * @return A new filename with the appended extension.
 */
char * giveNameFile(char *filename, char *extension);

/**
 * Splits a string into an array of substrings based on specified operands.
 * Also counts the number of words found during the split.
 *
 * @param input The input string to be split.
 * @param countWords A pointer to an integer where the word count will be stored.
 * @param operands A string containing characters to split the input by.
 * @return An array of substrings resulting from the split.
 */
char** splitString(char* input, int *countWords, char *operands);

/**
 * Checks if the given string contains double comma.
 *
 * @param str The string to check for the presence of a comma.
 * @return True if the string contains a comma, otherwise false.
 */
bool checkComma(char* str);

/**
 * Inserts a new macro into the macro table, updating the current macro pointer.
 *
 * @param macroTable The macro table to insert the macro into.
 * @param name The name of the new macro.
 * @param content The content of the new macro.
 * @param currMacro A pointer to the current macro being processed.
 */
void insertMacro(macroTable *macroTable, char *name, char *content, macro **currMacro);

/**
 * Creates a new macro with the specified name and content.
 *
 * @param macro_name The name of the new macro.
 * @param macro_content The content of the new macro.
 * @return A pointer to the newly created macro.
 */
macro *createNewMacro(const char *macro_name, const char *macro_content);

/**
 * Appends new content to an existing macro.
 *
 * @param currMacro A pointer to the macro to which content will be added.
 * @param newContent The content to add to the macro.
 */
void addContentToMacro(macro *currMacro, char *newContent);

/**
 * Retrieves the content of a macro based on a provided substring.
 *
 * @param macroTable The macro table to search for the macro.
 * @param substring The substring to search for in macro names.
 * @return The content of the found macro, or NULL if not found.
 */
char *getMacroContent(macroTable *macroTable, char *substring);

/**
 * Checks the validity of a macro name within a specified macro table and line number context.
 *
 * @param table The macro table to check for name validity.
 * @param name The name to be validated.
 * @param lineNum The line number context for error reporting.
 * @return True if the name is valid, otherwise false.
 */
bool checkValidMacroName(macroTable *table, char *name, int lineNum);

/**
 * Checks whether a name is not a valid reserved name.
 *
 * @param name The name to be checked for reserved status.
 * @param lineNum The line number context for error reporting.
 * @return True if the name is a valid reserved name, otherwise false.
 */
bool checkValidReservedName(char *name, int lineNum);

#endif
