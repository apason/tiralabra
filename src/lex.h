#ifndef LEX_HEADER
#define LEX_HEADER

#include <stdio.h>

#include "tokens.h"

// these function calls are so frequently used that I made them a macros
#define ungetc(x) 	 fseek(x, -sizeof(char), SEEK_CUR)
#define skipch(x)        fseek(x,  sizeof(char), SEEK_CUR)

/*
 * the main function of lexical analyzer
 */
extern token_list *lex(FILE *input);
    
#endif
