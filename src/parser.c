#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "lex.h"

static enum {CONSUME, NO_CONSUME} asdads;

static token *match(token_type tt, consumption_type ct);

static program_node *program(void);
static stmt_lst_node *statement_list(void);
static stmt_node *statement(void);
static ass_node *assignment(void);
static fac_node *factor(void);
static fac_tail_node *factor_tail(void);
static term_node *term(void);
static term_tail_node *term_tail(void);
static exp_node *expression(void);
static dec_node *declaration(void);
static if_node *if_(void);
static while_node *while_(void);
static for_node *for_(void);
static comp_node *comparation(void);

static program_node *newProgramNode(void);
static stmt_lst_node *newStmtLstNode(void);
static stmt_node *newStmtNode(void);
static ass_node *newAssNode(void);
static fac_node *newFacNode(void);
static fac_tail_node *newFacTailNode(void);
static term_node *newTermNode(void);
static term_tail_node *newTermTailNode(void);
static exp_node *newExpNode(void);
static dec_node *newDecNode(void);
static if_node *newIfNode(void);
static while_node *newWhileNode(void);
static for_node *newForNode(void);
static comp_node *newCompNode(void);

static token_list *tl;

static int errorv;
static void *error = &errorv;

/*
 * this is the "interface" of the parses. it is the only non-static
 * function in this file.
 *
 * input parameter tlist is pointer to token list which represents
 * to the program. if the token sequence is compatible with syntax
 * described in doc/kielen_kuvaus.md 1 is returned. 0 otherwise.
 */
		   
program_node *parse(token_list *tlist){
    (void) asdads; //for not causing warning about unused variable
    tl = tlist;

    return program();
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

static program_node *program(void){
    program_node *pn = newProgramNode();

    if(match(TOKEN_IFKEY,      NO_CONSUME)  ||
       match(TOKEN_WHILEKEY,   NO_CONSUME)  ||
       match(TOKEN_FORKEY,     NO_CONSUME)  ||
       match(TOKEN_TYPEKEY,    NO_CONSUME)  ||
       match(TOKEN_SCOL,       NO_CONSUME)  ||
       match(TOKEN_IDENTIFIER, NO_CONSUME)){
	if((pn->sln = statement_list()) != error)
	    if((pn->eof = match(TOKEN_EOF, CONSUME)) != NULL)
		return pn;
    }
    return NULL;   
}

static stmt_lst_node *statement_list(void){
    stmt_lst_node *sln = newStmtLstNode();

    if(match(TOKEN_IFKEY,      NO_CONSUME) ||
       match(TOKEN_WHILEKEY,   NO_CONSUME) ||
       match(TOKEN_FORKEY,     NO_CONSUME) ||
       match(TOKEN_TYPEKEY,    NO_CONSUME) ||
       match(TOKEN_SCOL,       NO_CONSUME) ||
       match(TOKEN_IDENTIFIER, NO_CONSUME)){
	if((sln->stmtn = statement()) != NULL)
	    if((sln->sln = statement_list()) != error)
		return sln;
    }
    else if(match(TOKEN_EOF, NO_CONSUME))
	return NULL;

    return error;
}

static stmt_node *statement(void){
    stmt_node *stmtn = newStmtNode();
    
    if(match(TOKEN_IDENTIFIER,   NO_CONSUME) ||
       match(TOKEN_SCOL,         NO_CONSUME)){
	if((stmtn->assn = assignment()) != NULL)
	    return stmtn;
    }
    else if(match(TOKEN_TYPEKEY, NO_CONSUME)){
	if((stmtn->decn = declaration()) != NULL)
	    return stmtn;
    }
    else if(match(TOKEN_IFKEY,   NO_CONSUME)){
	if((stmtn->ifn = if_()) != NULL)
	    return stmtn;
    }
    else if(match(TOKEN_WHILEKEY,NO_CONSUME)){
	if((stmtn->whilen = while_()) != NULL)
	    return stmtn;
    }
    else if(match(TOKEN_FORKEY,  NO_CONSUME))
	if((stmtn->forn = for_()) != NULL)
	    return stmtn;
    
    return NULL;
}

static ass_node *assignment(void){
    ass_node *assn = newAssNode();

    if((assn->id = match(TOKEN_IDENTIFIER, CONSUME)) != NULL){
	if((assn->assOp = match(TOKEN_ASSOP, CONSUME)) != NULL)
	    if((assn->expn = expression()) != NULL)
		if((assn->scol = match(TOKEN_SCOL, CONSUME)) != NULL)
		    return assn;
    }
    else if((assn->scol = match(TOKEN_SCOL, CONSUME)) != NULL)
	return assn;
    
    return NULL;
}

static exp_node *expression(void){
    exp_node *expn = newExpNode();
    
    if(match(TOKEN_LBRA,       NO_CONSUME) ||
       match(TOKEN_IDENTIFIER, NO_CONSUME) ||
       match(TOKEN_LITERAL,    NO_CONSUME))
	if((expn->termn = term()) != NULL)
	    if((expn->termtln = term_tail()) != error)
		return expn;
    
    return NULL;
}

static term_node *term(void){
    term_node *termn = newTermNode();
    
    if(match(TOKEN_LBRA,       NO_CONSUME) ||
       match(TOKEN_IDENTIFIER, NO_CONSUME) ||
       match(TOKEN_LITERAL,    NO_CONSUME))
	if((termn->facn = factor()) != NULL)
	    if((termn->factln = factor_tail()) != error)
		return termn;
    
    return NULL;
}

static term_tail_node *term_tail(void){
    term_tail_node *termtln = newTermTailNode();

    if((termtln->addOp = match(TOKEN_ADDOP, CONSUME)) != NULL){
	if((termtln->termn = term()) != NULL)
	    if((termtln->termtln = term_tail()) != error)
		return termtln;
    }
    else if(match(TOKEN_RBRA,   NO_CONSUME) ||
	    match(TOKEN_SCOL,   NO_CONSUME) ||
	    match(TOKEN_COMPOP, NO_CONSUME))
	return NULL;

    return error;
}

static fac_node *factor(void){
    fac_node *facn = newFacNode();

    if((facn->id = match(TOKEN_IDENTIFIER, CONSUME)) != NULL ||
       (facn->lit = match(TOKEN_LITERAL,    CONSUME)) != NULL)
	return facn;
    else if((facn->lbra = match(TOKEN_LBRA, CONSUME)) != NULL){
	if((facn->expn = expression()) != NULL)
	    if((facn->rbra = match(TOKEN_RBRA, CONSUME)) != NULL)
		return facn;
    }
    
    return NULL;
}

static fac_tail_node *factor_tail(void){
    fac_tail_node *factln = newFacTailNode();

    if((factln->mulOp = match(TOKEN_MULOP, CONSUME)) != NULL){
	if((factln->facn = factor()) != NULL)
	    if((factln->factln = factor_tail()) != error)
		return factln;
    }
    else if(match(TOKEN_SCOL,   NO_CONSUME) ||
	    match(TOKEN_ADDOP,  NO_CONSUME) ||              
	    match(TOKEN_RBRA,   NO_CONSUME) ||
	    match(TOKEN_COMPOP, NO_CONSUME))
	return NULL;
    
    return error;
}

static dec_node *declaration(void){
    dec_node *decn = newDecNode();

    if((decn->typeKey = match(TOKEN_TYPEKEY, CONSUME)) != NULL){
	if((decn->id = match(TOKEN_IDENTIFIER, CONSUME)) != NULL);
	   if((decn->scol = match(TOKEN_SCOL, CONSUME)) != NULL)
	       return decn;
    }
    else if((decn->scol = match(TOKEN_SCOL, CONSUME)) != NULL)
	return decn;
    
    return NULL;
}

static comp_node *comparation(void){
    comp_node *compn = newCompNode();

    if((compn->expn = expression()) != NULL)
	if((compn->compOp = match(TOKEN_COMPOP, CONSUME)) != NULL)
	    if((compn->expn2 = expression()) != NULL)
		return compn;
    
    return NULL;
}

static if_node *if_(void){
    if_node *ifn = newIfNode();

    if((ifn->ifKey = match(TOKEN_IFKEY, CONSUME)) != NULL)
	if((ifn->lbra = match(TOKEN_LBRA, CONSUME)) != NULL)
	    if((ifn->compn = comparation()) != NULL)
		if((ifn->rbra = match(TOKEN_RBRA, CONSUME)) != NULL)
		    if((ifn->stmtn = statement()) != NULL)
			return ifn;
    return NULL;
}

static while_node *while_(void){
    while_node *whilen = newWhileNode();

    if((whilen->whileKey = match(TOKEN_WHILEKEY, CONSUME)) != NULL)
	if((whilen->lbra = match(TOKEN_LBRA, CONSUME)) != NULL)
	    if((whilen->compn = comparation()) != NULL)
		if((whilen->rbra = match(TOKEN_RBRA, CONSUME)) != NULL)
		    if((whilen->stmtn = statement()) != NULL)
			return whilen;
    return NULL;
}

static for_node *for_(void){
    for_node *forn = newForNode();

    if((forn->forKey = match(TOKEN_FORKEY, CONSUME)) != NULL)
	if((forn->lbra = match(TOKEN_LBRA, CONSUME)) != NULL)
	    if((forn->assn = assignment()) != NULL)
		if((forn->compn = comparation()) != NULL)
		    if((forn->scol = match(TOKEN_SCOL, CONSUME)) != NULL)
			if((forn->id = match(TOKEN_IDENTIFIER, CONSUME)) != NULL) 
			    if((forn->assOp = match(TOKEN_ASSOP, CONSUME)) != NULL)
				if((forn->expn = expression()) != NULL)
				    if((forn->rbra = match(TOKEN_RBRA, CONSUME)) != NULL)
					if((forn->stmtn = statement()) != NULL)
					    return forn;
    return NULL;
}
    
static token *match(token_type tt, consumption_type ct){
    token *t;
    if(tl->value->type == tt){
	if(ct == CONSUME){
	    t = tl->value;
	    tl = tl->next;
	}
	return t;
    }
    return NULL;
}

static program_node *newProgramNode(void){
    program_node *r = (program_node*)malloc(sizeof(program_node));

    r->sln = NULL;
    r->eof = NULL;

    return r;
}

static stmt_lst_node *newStmtLstNode(void){
    stmt_lst_node *r = (stmt_lst_node*)malloc(sizeof(stmt_lst_node));

    r->stmtn = NULL;
    r->sln   = NULL;

    return r;
}

static stmt_node *newStmtNode(void){
    stmt_node *r = (stmt_node*)malloc(sizeof(stmt_node));

    r->ifn = NULL;
    r->forn = NULL;
    r->whilen = NULL;
    r->assn = NULL;
    r->decn = NULL;

    return r;
}

static ass_node *newAssNode(void){
    ass_node *r = (ass_node*)malloc(sizeof(ass_node));

    r->id = NULL;
    r->assOp = NULL;
    r->expn = NULL;
    r->scol = NULL;

    return r;
}

static term_node *newTermNode(void){
    term_node *r = (term_node*)malloc(sizeof(term_node));

    r->facn = NULL;
    r->factln = NULL;

    return r;
}

static exp_node *newExpNode(void){
    exp_node *r = (exp_node*)malloc(sizeof(exp_node));

    r->termn = NULL;
    r->termtln = NULL;

    return r;
}

static term_tail_node *newTermTailNode(void){
    term_tail_node *r = (term_tail_node*)malloc(sizeof(term_tail_node));

    r->addOp = NULL;
    r->termn = NULL;
    r->termtln = NULL;

    return r;
}

static fac_node *newFacNode(void){
    fac_node *r = (fac_node*)malloc(sizeof(fac_node));

    r->lbra = NULL;
    r->expn = NULL;
    r->rbra = NULL;
    r->id = NULL;
    r->lit = NULL;

    return r;
}

static fac_tail_node *newFacTailNode(void){
    fac_tail_node *r = (fac_tail_node*)malloc(sizeof(fac_tail_node));

    r->mulOp = NULL;
    r->facn = NULL;
    r->factln = NULL;

    return r;
}

static dec_node *newDecNode(void){
    dec_node *r = (dec_node*)malloc(sizeof(dec_node));

    r->typeKey = NULL;
    r->id = NULL;
    r->scol = NULL;

    return r;
}

static comp_node *newCompNode(void){
    comp_node *r = (comp_node*)malloc(sizeof(comp_node));

    r->expn = NULL;
    r->compOp = NULL;
    r->expn2 = NULL;

    return r;
}

static if_node *newIfNode(void){
    if_node *r = (if_node*)malloc(sizeof(if_node));

    r->ifKey = NULL;
    r->lbra = NULL;
    r->compn = NULL;
    r->rbra = NULL;
    r->stmtn = NULL;

    return r;
}
    
static while_node *newWhileNode(void){
    while_node *r = (while_node*)malloc(sizeof(while_node));

    r->whileKey = NULL;
    r->lbra = NULL;
    r->compn = NULL;
    r->rbra = NULL;
    r->stmtn = NULL;

    return r;
}
    
static for_node *newForNode(void){
    for_node *r = (for_node*)malloc(sizeof(for_node));

    r->forKey = NULL;
    r->lbra = NULL;
    r->assn = NULL;
    r->compn = NULL;
    r->scol = NULL;
    r->id = NULL;
    r->assOp = NULL;
    r->expn = NULL;
    r->rbra = NULL;
    r->stmtn = NULL;

    return r;
}
