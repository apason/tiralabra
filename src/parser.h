#ifndef parser_header
#define parser_header

/*
 * this is needed for type token_list
 */
#include "tokens.h"

/*
 * tree.h contains definition of program_node
 */
#include "tree.h"

/*
 * consumption type can be either CONSUME or
 * NO_CONSUME. that value is used in function 
 * match() to determine whether the token is
 * consumed after it was checked.
 */
typedef unsigned int consumption_type;

/*
 * main function of syntax analysis.
 */
extern program_node *parse(token_list *tlist);

#endif
