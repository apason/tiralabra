#include <stdio.h>

#include "parser.h"
#include "lex.h"

static enum {CONSUME, NO_CONSUME} asdads;

static int match(token_type tt, consumption_type ct);
static int program(void);
static int statement_list(void);
static int statement(void);
static int assignment(void);
static int factor(void);
static int factor_tail(void);
static int term(void);
static int term_tail(void);
static int expression(void);
static int declaration(void);
static int iw(void);
static int for_(void);

static token_list *tl;

/*
 * this is the "interface" of the parses. it is the only non-static
 * function in this file.
 *
 * input parameter tlist is pointer to token list which represents
 * to the program. if the token sequence is compatible with syntax
 * described in doc/kielen_kuvaus.md 1 is returned. 0 otherwise.
 */
		   
int parse(token_list *tlist){
    (void) asdads;
    tl = tlist;
    if(program())
	return 1;

    return 0;
}

/*
 * this parser is so called recursive descent parser. it works
 * producing the syntax tree from root to leaves, from left to right.
 *
 * there is function for every nonterminal in the grammar. every function
 * could call to function match() many times. each of them represents
 * one element in predict set.
 * 
 * because of the LL(1):ness of the grammar, the parser could have been
 * written to work in O(n) and it obviously was. there is no backtracking
 * so the program works in linear time.
 *
 * NOTE! there is "epsilon token" needed in first-sets of some nonterminals.
 * those are actually no tokens. it only indicates that we can continue
 * parsing if the next token is in follow set of that nonterminal.
 */

static int program(void){
    printf("program\n");
    if(match(TOKEN_IWKEY,      NO_CONSUME)  ||
       match(TOKEN_FORKEY,     NO_CONSUME)  ||
       match(TOKEN_TYPEKEY,    NO_CONSUME)  ||
       match(TOKEN_SCOL,       NO_CONSUME)  ||
       match(TOKEN_IDENTIFIER, NO_CONSUME)){
	if(statement_list())
	    return match(TOKEN_EOF, CONSUME);
    }
    return 0;   
}

static int statement_list(void){
    printf("stmt_lst\n");
    if(match(TOKEN_IWKEY,      NO_CONSUME) ||
       match(TOKEN_FORKEY,     NO_CONSUME) ||
       match(TOKEN_TYPEKEY,    NO_CONSUME) ||
       match(TOKEN_SCOL,       NO_CONSUME) ||
       match(TOKEN_IDENTIFIER, NO_CONSUME)){
	if(statement())
	    return statement_list();
    }
    else if(match(TOKEN_EOF, NO_CONSUME))
	return 1;

    return 0;
}

static int statement(void){
    printf("stmt\n");
    if(match(TOKEN_IDENTIFIER,   NO_CONSUME) ||
       match(TOKEN_SCOL,         NO_CONSUME))	
	return assignment();
    else if(match(TOKEN_TYPEKEY, NO_CONSUME))
	return declaration();
    else if(match(TOKEN_IWKEY,   NO_CONSUME))
	return iw();
    else if(match(TOKEN_FORKEY,  NO_CONSUME))
	return for_();
    
    return 0;
}

static int assignment(void){
    printf("ass\n");
    if(match(TOKEN_IDENTIFIER, CONSUME)){
	if(match(TOKEN_ASSOP, CONSUME))
	    if(expression())
		return match(TOKEN_SCOL, CONSUME);
    }
    else if(match(TOKEN_SCOL, CONSUME))
	return 1;
    return 0;
}

static int expression(void){
    printf("exp\n");
    if(match(TOKEN_LBRA,       NO_CONSUME) ||
       match(TOKEN_IDENTIFIER, NO_CONSUME) ||
       match(TOKEN_LITERAL,    NO_CONSUME))
	if(term())
	    return term_tail();
    
    return 0;
}

static int term(void){
    printf("term\n");
    if(match(TOKEN_LBRA,       NO_CONSUME) ||
       match(TOKEN_IDENTIFIER, NO_CONSUME) ||
       match(TOKEN_LITERAL,    NO_CONSUME))
	if(factor())
	    return factor_tail();
    
    return 0;
}

static int term_tail(void){
    printf("term_tail\n");
    if(match(TOKEN_ADDOP, CONSUME)){
	if(term())
	    return term_tail();
    }
    else if(match(TOKEN_RBRA,   NO_CONSUME) ||
	    match(TOKEN_SCOL,   NO_CONSUME) ||
	    match(TOKEN_COMPOP, NO_CONSUME))
	return 1;

    return 0;
}

static int factor(void){
    printf("fac\n");
    if(match(TOKEN_IDENTIFIER, CONSUME) ||
       match(TOKEN_LITERAL,    CONSUME))
	return 1;
    else if(match(TOKEN_LBRA, CONSUME)){
	if(expression())
	    if(match(TOKEN_RBRA, CONSUME))
		return 1;
    }
    
    return 0;
}

static int factor_tail(void){
    printf("fac_tail\n");
    if(match(TOKEN_MULOP, CONSUME)){
	if(factor())
	    return factor_tail();
    }
    else if(match(TOKEN_SCOL,   NO_CONSUME) ||
	    match(TOKEN_ADDOP,  NO_CONSUME) ||
	    match(TOKEN_RBRA,   NO_CONSUME) ||
	    match(TOKEN_COMPOP, NO_CONSUME))
	return 1;
    
    return 0;
}

static int declaration(void){
    printf("dec\n");
    if(match(TOKEN_TYPEKEY, CONSUME)){
	if(match(TOKEN_IDENTIFIER, CONSUME))
	    return match(TOKEN_SCOL, CONSUME);
    }
    else if(match(TOKEN_SCOL, CONSUME))
	return 1;
    
    return 0;
}

static int iw(void){
    printf("iw\n");
    if(match(TOKEN_IWKEY, CONSUME))
	if(match(TOKEN_LBRA, CONSUME))
	    if(expression())
		if(match(TOKEN_COMPOP, CONSUME))
		    if(expression())
			if(match(TOKEN_RBRA, CONSUME))
			    return statement();
    return 0;
}

static int for_(void){
    printf("for\n");
    if(match(TOKEN_FORKEY, CONSUME))
	if(match(TOKEN_LBRA, CONSUME))
	    if(assignment())
		if(expression())
		    if(match(TOKEN_COMPOP, CONSUME))
			if(expression())
			    if(match(TOKEN_SCOL, CONSUME))
				if(match(TOKEN_IDENTIFIER, CONSUME))
				    if(match(TOKEN_ASSOP, CONSUME))
					if(expression())
					    if(match(TOKEN_RBRA, CONSUME))
						return statement();
    return 0;
}

static int match(token_type tt, consumption_type ct){
    if(tl->value->type == tt){
	if(ct == CONSUME){
	    printf("%s\n", tl->value->value);
	    tl = tl->next;
	}
	return 1;
    }
    return 0;
}
