#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"
#include "tree.h"
#include "lex.h"
#include "label.h"


/*
 * LEXICAL ANALYSIS ---------------------------------------------------------
 * return new token initialized by parameters.
 * this function expects that type and value are correct!
 */
token *newToken(token_type type, char *value){
    token *t = (token *)malloc(sizeof(token));
    
    t->type = type;
    strncpy(t->value, value, TOKEN_MAX_LENGTH +1);

    return t;
}

/*
 * returns new token_list node all values initialized to NULL
 */
token_list *newTokenList(void){
    token_list *tl = (token_list *)malloc(sizeof(token_list));

    tl->value = NULL;
    tl->next  = NULL;
    
    return tl;
}


/*
 * this function creates and adds new token to the end of
 * the token list. 
 * 
 * parameters:
 *   tl is pointer to the end of the list
 *   type is token type, one of the macros defined in lex.h
 *   buffer is pointer to the token value of new token
 *   input is pointer to input stream
 *
 * pointer to new node is returned
 */
token_list *addToken(token_list *tl, token_type type, char *buffer, FILE *input){
    tl->next  = (token_list *)malloc(sizeof(token_list));
    tl->value = newToken(type, buffer);
    tl        = tl->next;
    tl->next  = NULL;
    tl->value = NULL;

    /*
     * here we have all tokens that can be added 
     * without any backtracking. that means we do
     * not need to ungetc the input stream
     */
    if(type == TOKEN_ERROR                         ||
       type == TOKEN_ADDOP                         ||
       type == TOKEN_LBRA                          ||
       type == TOKEN_RBRA                          ||
       type == TOKEN_SCOL                          ||
       
       strncmp(buffer, "*", TOKEN_MAX_LENGTH) == 0)
	;
    else
	ungetc(input);
    
    return tl;
}


/*
 * SYNTACTIC ANALYSIS --------------------------------------------------------
 *
 * the following functions just creates and initializes
 * nodes of the syntax tree. all nonterminals have own
 * function. note that memory area can not be set with
 * memset() because NULL is not guaranteed to be 0
 */
program_node *newProgramNode(void){
    program_node *r = (program_node*)malloc(sizeof(program_node));

    r->sln = NULL;
    r->eof = NULL;

    return r;
}

stmt_lst_node *newStmtLstNode(void){
    stmt_lst_node *r = (stmt_lst_node*)malloc(sizeof(stmt_lst_node));

    r->stmtn = NULL;
    r->sln   = NULL;

    return r;
}

stmt_node *newStmtNode(void){
    stmt_node *r = (stmt_node*)malloc(sizeof(stmt_node));

    r->ifn    = NULL;
    r->forn   = NULL;
    r->whilen = NULL;
    r->assn   = NULL;
    r->decn   = NULL;

    return r;
}

ass_node *newAssNode(void){
    ass_node *r = (ass_node*)malloc(sizeof(ass_node));

    r->id    = NULL;
    r->assOp = NULL;
    r->expn  = NULL;
    r->scol  = NULL;

    return r;
}

term_node *newTermNode(void){
    term_node *r = (term_node*)malloc(sizeof(term_node));

    r->facn   = NULL;
    r->factln = NULL;

    return r;
}

exp_node *newExpNode(void){
    exp_node *r = (exp_node*)malloc(sizeof(exp_node));

    r->termn   = NULL;
    r->termtln = NULL;

    return r;
}

term_tail_node *newTermTailNode(void){
    term_tail_node *r = (term_tail_node*)malloc(sizeof(term_tail_node));

    r->addOp   = NULL;
    r->termn   = NULL;
    r->termtln = NULL;

    return r;
}

fac_node *newFacNode(void){
    fac_node *r = (fac_node*)malloc(sizeof(fac_node));

    r->lbra = NULL;
    r->expn = NULL;
    r->rbra = NULL;
    r->id   = NULL;
    r->lit  = NULL;

    return r;
}

fac_tail_node *newFacTailNode(void){
    fac_tail_node *r = (fac_tail_node*)malloc(sizeof(fac_tail_node));

    r->mulOp  = NULL;
    r->facn   = NULL;
    r->factln = NULL;

    return r;
}

dec_node *newDecNode(void){
    dec_node *r = (dec_node*)malloc(sizeof(dec_node));

    r->typeKey = NULL;
    r->id      = NULL;
    r->scol    = NULL;

    return r;
}

comp_node *newCompNode(void){
    comp_node *r = (comp_node*)malloc(sizeof(comp_node));

    r->expn   = NULL;
    r->compOp = NULL;
    r->expn2  = NULL;

    return r;
}

if_node *newIfNode(void){
    if_node *r = (if_node*)malloc(sizeof(if_node));

    r->ifKey = NULL;
    r->lbra  = NULL;
    r->compn = NULL;
    r->rbra  = NULL;
    r->stmtn = NULL;

    return r;
}
    
while_node *newWhileNode(void){
    while_node *r = (while_node*)malloc(sizeof(while_node));

    r->whileKey = NULL;
    r->lbra     = NULL;
    r->compn    = NULL;
    r->rbra     = NULL;
    r->stmtn    = NULL;

    return r;
}
    
for_node *newForNode(void){
    for_node *r = (for_node*)malloc(sizeof(for_node));

    r->forKey = NULL;
    r->lbra   = NULL;
    r->assn   = NULL;
    r->compn  = NULL;
    r->scol   = NULL;
    r->id     = NULL;
    r->assOp  = NULL;
    r->expn   = NULL;
    r->rbra   = NULL;
    r->stmtn  = NULL;

    return r;
}




// SEMANTIC ANALYSIS -----------------------------------------------------
label_list *newLabelListNode(label_list **list, label *l, label_type lt){
    label_list *new = (label_list*)malloc(sizeof(label_list));

    new->l    = l;
    new->lt   = lt; 
    new->next = *list; //head of the label list

    return new;
}
