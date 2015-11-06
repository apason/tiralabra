#include <stdio.h>

#ifndef LEX_HEADER
#define LEX_HEADER

#define TOKEN_MAX_LENGTH 32

#define TOKEN_OPERATOR   0
#define TOKEN_KEY_WORD   1
#define TOKEN_IDENTIFIER 2
#define TOKEN_LITERAL    3
#define TOKEN_ERROR      4

#define ungetc(x) 	 fseek(x, -sizeof(char), SEEK_CUR)
#define skipch(x)        fseek(x,  sizeof(char), SEEK_CUR)

typedef unsigned int token_type;
typedef char token_value[TOKEN_MAX_LENGTH +1];

/*
 * structure for token. every token has type of integer value
 * and string containing the actual token. The type is unsigned
 * integer and defines how the token value is used.
 */
typedef struct{
    token_type  type;
    token_value value;
} token;

/*
 * this is data structure type which is returned from lexical
 * analyzer. the list just contains all tokens from the input.
 */
typedef struct TOKEN_LIST{
    token             *value;
    struct TOKEN_LIST *next;
} token_list;

/*
 * the main function of lexical analyzer
 */
extern token_list *lex(FILE *input);
/*
 * this is mainly for debugging. final version will probably
 * not print the token list.
 */
extern void printTokenList(token_list *list);
    
#endif
