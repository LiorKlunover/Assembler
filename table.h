#include "lexer.h"
#include "globals.h"
#ifndef TABLE_H
#define TABLE_H
#define TABLE_SIZE 10


/**
 * Create a new lexeme table.
 *
 * @return A pointer to the newly created lexeme table.
 */
lexTable *createLexStructTable();

/**
 * Create a new macro table.
 *
 * @return A pointer to the newly created macro table.
 */
macroTable *createMacroTable();

/**
 * Create a new symbol table.
 *
 * @return A pointer to the newly created symbol table.
 */
symbolTable *createSymbolTable();

/**
 * Create a new object file content structure.
 *
 * @return A pointer to the newly created object file content structure.
 */
objectFileContent *makeObjectFileContentTable();

/**
 * Create a new extern file content structure.
 *
 * @return A pointer to the newly created extern file content structure.
 */
externFileContent *makeExternFileContentTable();

#endif