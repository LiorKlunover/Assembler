#include "globals.h"
#include "code.h"

#ifndef UNTITLED5_LEXER_H
#define UNTITLED5_LEXER_H

#define MAX_LABEL_SIZE 31
#define MAX_STRING_LEN 80
#define MAX_NUMBER_SIZE 80
#define MAX_DATA_VALUE 511
#define MIN_DATA_VALUE (-512)

typedef enum operandAddrType {
    immediate,
    labelType,
    registerType,
    errorType = -1
} operandAddrType;

typedef union operandAddrTypeUnion {
    label *label;
    char reg;
    int immed;
} operandAddrTypeName;

typedef struct bitArrayDir {
    unsigned int lineByteCode : 12;
} bitLine;

typedef struct lexStruct {
    label *label;
    enum {
        lexInst,
        lexDir
    } lineType;
    union {
        struct {
            int **contentByteCode;
            enum {
                lexDirString,
                lexDirData,
                lexDirEntry,
                lexDirExtern
            } lexDirType;

            union {
                char stringType[MAX_STRING_LEN];
                struct {
                    int numArray[MAX_NUMBER_SIZE];
                    int numCount;
                } dataType;
            } dirUnionContent;
            bitLine dirBitArray[MAX_STRING_LEN + 1];
        } dirType;

        struct {
            opCode instName;
            int numOfOperands;
            struct {
                int lineByteCode : 12;
                int sourceOpByteCode : 12;
                int targetOpByteCode : 12;
            } lineByteCodeData;
            struct {
                operandAddrType arrOpType[2];
                operandAddrTypeName arrOpName[2];
            } OpeInstTypes;
        } instType;
    } lexType;
} lexStruct;

typedef struct lexTable {
    lexStruct **content;
    int size;
    int capacity;
} lexTable;

/**
 * Get the lexical tree position for a given line of code.
 *
 * @param line The line of code to analyze.
 * @param labelTable The symbol table for labels.
 * @param IC The instruction counter.
 * @param DC The data counter.
 * @param macroTable The table for macros.
 * @return A pointer to the generated lexStruct, or NULL on error.
 */
lexStruct *getLexTreePosition(lineStr *line, symbolTable *labelTable, int *IC, int *DC, macroTable *macroTable);

/**
 * Check if a label is valid.
 *
 * @param label The label to check.
 * @param linNum The line number of the label.
 * @param list The symbol table to check against.
 * @param macroTable The table for macros.
 * @return True if the label is valid, false otherwise.
 */
bool isValidLabel(char *label, int linNum, symbolTable *list, macroTable *macroTable);

/**
 * Insert a label into the symbol table.
 *
 * @param l The label to insert.
 * @param labelTable The symbol table.
 * @param currentLabel A pointer to the current label being processed.
 * @param type The type of label.
 * @param defined Whether the label is defined.
 */
void insertLabelTable(char *l, symbolTable *labelTable, label **currentLabel, lDType type, bool defined);

/**
 * Parse data from an input string.
 *
 * @param inputString The input string containing data values.
 * @param numArray An array to store parsed data values.
 * @param numCount The number of parsed data values.
 * @param lineNum The line number of the input.
 * @return True if successful, false otherwise.
 */
bool getData(const char *inputString, int *numArray, int *numCount, int *lineNum);

/**
 * Get a label from the symbol table by its name.
 *
 * @param label The label name to search for.
 * @param labelList The symbol table containing labels.
 * @return A pointer to the label if found, NULL otherwise.
 */
label *getLabel(char *label, symbolTable *labelList);

/**
 * Get the opcode of an instruction.
 *
 * @param str The instruction mnemonic.
 * @param lineNum The line number of the instruction.
 * @param numOfOperands The number of operands the instruction has.
 * @return The opcode if the instruction is recognized, -1 otherwise.
 */
int getOpCode(char *str, int *lineNum, int *numOfOperands);

/**
 * Parse operands from a string and populate the lexStruct with operand information.
 *
 * @param str The string containing operands.
 * @param lexTree The lexStruct to populate.
 * @param lineNum The line number of the instruction.
 * @param labelTable The symbol table for labels.
 * @param macroTable The table for macros.
 * @return True if successful, false otherwise.
 */
bool getOperands(char *str, lexStruct *lexTree, int *lineNum, symbolTable *labelTable, macroTable *macroTable);

/**
 * Check if a string represents a valid number.
 *
 * @param str The string to check.
 * @param num A pointer to store the parsed number if valid.
 * @return True if the string is a valid number, false otherwise.
 */
bool getNumber(char *str, int *num);

/**
 * Check the validity of operand order for a given opcode.
 *
 * @param opCode The opcode to check.
 * @param arrOpType An array of operand address types.
 * @param lineNum The line number of the instruction.
 * @return True if the operand order is valid, false otherwise.
 */
bool checkOperandOrder(opCode opCode, operandAddrType arrOpType[], int *lineNum);

/**
 * Create a bit array from a string to store in the lexStruct.
 *
 * @param str The input string.
 * @param stringBitArray The bitLine array to store the bit data.
 */
void makeStringBitArray(char *str, bitLine *stringBitArray);

/**
 * Check if a string represents a valid number.
 *
 * @param str The string to check.
 * @return True if the string is a valid number, false otherwise.
 */
int isValidNumber(char *str);

/**
 * Check if a string represents a valid string enclosed in double quotes.
 *
 * @param str The string to check.
 * @return A pointer to the valid string if present, NULL otherwise.
 */
char *isValidString(char *str);

/**
 * Check for an invalid comma on the left side of a string.
 *
 * @param str The string to check.
 * @return True if there is no invalid comma, false otherwise.
 */
bool checkInvalidLeftComma(char *str);

/**
 * Check for an invalid comma on the right side of a string.
 *
 * @param str The string to check.
 * @return True if there is no invalid comma, false otherwise.
 */
bool checkInvalidRightComma(char *str);

#endif
