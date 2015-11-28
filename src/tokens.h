#ifndef TOKENS_HEADER
#define TOKENS_HEADER

#include <stdio.h>

#define TOKEN_ASSOP      0
#define TOKEN_IDENTIFIER 1
#define TOKEN_LITERAL    2
#define TOKEN_TYPEKEY    3
#define TOKEN_IFKEY      4
#define TOKEN_FORKEY     5
#define TOKEN_COMPOP     6
#define TOKEN_MULOP      7
#define TOKEN_LBRA       8
#define TOKEN_RBRA       9
#define TOKEN_ADDOP      11
#define TOKEN_ERROR      12
#define TOKEN_SCOL       13
#define TOKEN_EOF        14
#define TOKEN_WHILEKEY   15

#define TOKEN_MAX_LENGTH 32

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
 * this is mainly for debugging. final version will probably
 * not print the token list.
 */
extern void printTokenList(token_list *list);

#endif
