
#ifndef UNTITLED5_GLOBALS_H
#define UNTITLED5_GLOBALS_H
#define MAX_LABEL_SIZE 31

#define ALLOCATE(ptr, size) do {\
    ptr = malloc(sizeof(*(ptr)) * (size)); \
    if (!ptr) { \
        printf("Memory allocation failed"); \
        exit(0); \
    } \
} while (0)
#define REALLOCTION(ptr, size)                         \
    do {                                           \
        void *temp_ptr = realloc(ptr,sizeof(*(ptr)) *(size));      \
        if (temp_ptr != NULL) {                     \
            ptr = temp_ptr;                         \
        } else {                                   \
            /* Handle reallocation failure here */  \
            printf("Memory reallocation failed!"); \
            exit(EXIT_FAILURE);                    \
        }                                          \
    } while (0)


typedef enum booleans {
    false = 0, true = 1
} bool;

typedef enum registers{
    r0,
    r1,
    r2,
    r3,
    r5,
    r6,
    r7,
    NOT_REG = -1
} reg;
typedef enum opCode{
    mov,
    cmp,
    add,
    sub,
    not,
    clr,
    lea,
    inc,
    dec,
    jmp,
    bne,
    red,
    prn,
    jsr,
    rts,
    stop,
    error = -1
} opCode;
typedef enum labelDefineType{
    entryLabel,
    externLabel,
    notEntryOrExtern,
    operandDefined,
    notDefined = -1
} lDType;

typedef struct label{
    char *symbolName;
    lDType defineType;
    bool defined;
    int address;
    int data;
}label;
typedef struct symbolTable{
    label **labels;
    int size;
    int capacity;
} symbolTable;

typedef struct first_w {
    unsigned int ARE: 2;
    unsigned int dest_op_addr: 2;
    unsigned int src_op_addr: 2;
    unsigned int op_code: 4;
    unsigned int second_param: 2;
    unsigned int first_param: 2;
} first_w;
typedef struct line{
    char **lineContent;
    int size;
    int lineNum;
} lineStr;
typedef struct macro {
    char *name;
    char *content;
} macro;
typedef struct macroTable {
    macro **tableList;
    int size;
    int count;
} macroTable;

typedef struct objectContent{
    char **content;
    int size;
    int capacity;
}objectFileContent;

typedef struct externContent{
    char **content;
    int size;
    int capacity;
}externFileContent;

typedef struct entryContent{
    char **content;
    int size;
    int capacity;
}entryFileContent;
#endif
