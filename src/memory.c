#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"
#include "tree.h"
#include "lex.h"
#include "label.h"
#include "memory.h"

static void *error;

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
 * deletes the token list structure. note that actual tokens
 * are not deleted but they are not lost either. in parse()
 * funciton tokens are linked to the syntax tree and is deleted
 * after code generation
 */
void freeTokenList(token_list *tl){
    token_list *tmp;
    
    while(tl != NULL){
	tmp = tl->next;
	free(tl);
	tl = tmp;
    }
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

void freeLabelList(label_list *ll){
    label_list *tmp;

    while(ll != NULL){
	tmp = ll->next;
	free(ll);
	ll = tmp;
    }
}

void freeSyntaxTree(program_node *pn, void *err){
    if(err != NULL)
	error = err;
    if(pn == NULL) return;
    freeProgram(pn);
}
void freeProgram   (program_node *pn){
    if(pn == NULL || pn == error) return;

    free(pn->eof);
    freeStmtLst(pn->sln);
    free(pn);
}
void freeStmtLst   (stmt_lst_node *stmtln){
    if(stmtln == NULL || stmtln == error) return;

    freeStmt(stmtln->stmtn);
    freeStmtLst(stmtln->sln);
    free(stmtln);
}
void freeStmt      (stmt_node *stmtn){
    if(stmtn == NULL || stmtn == error) return;

    freeIf(stmtn->ifn);
    freeWhile(stmtn->whilen);
    freeFor(stmtn->forn);
    freeDec(stmtn->decn);
    freeAss(stmtn->assn);
    free(stmtn);
}

void freeIf        (if_node *ifn){
    if(ifn == NULL || ifn == error) return;

    free(ifn->ifKey);
    free(ifn->lbra);
    freeComp(ifn->compn);
    free(ifn->rbra);
    freeStmt(ifn->stmtn);
    free(ifn);
}
void freeWhile     (while_node *whilen){
    if(whilen == NULL || whilen == error) return;

    free(whilen->whileKey);
    free(whilen->lbra);
    freeComp(whilen->compn);
    free(whilen->rbra);
    freeStmt(whilen->stmtn);
    free(whilen);
}
    
void freeFor       (for_node *forn){
    if(forn == NULL || forn == error) return;

    free(forn->forKey);
    free(forn->lbra);
    freeAss(forn->assn);
    freeComp(forn->compn);
    free(forn->scol);
    free(forn->id);
    free(forn->assOp);
    freeExp(forn->expn);
    free(forn->rbra);
    freeStmt(forn->stmtn);
    free(forn);
}
    
void freeComp      (comp_node *compn){
    if(compn == NULL || compn == error) return;

    freeExp(compn->expn);
    free(compn->compOp);
    freeExp(compn->expn2);
    free(compn);
}

void freeDec       (dec_node *decn){
    if(decn == NULL || decn == error) return;

    free(decn->typeKey);
    free(decn->id);
    free(decn->scol);
    free(decn);
}

void freeAss       (ass_node *assn){
    if(assn == NULL || assn == error) return;

    free(assn->id);
    free(assn->assOp);
    freeExp(assn->expn);
    free(assn->scol);
    free(assn);
}

void freeExp       (exp_node *expn){
    if(expn == NULL || expn == error) return;

    freeTerm(expn->termn);
    freeTermTail(expn->termtln);
    free(expn);
}

void freeTerm      (term_node *termn){
    if(termn == NULL || termn == error) return;

    freeFac(termn->facn);
    freeFacTail(termn->factln);
    free(termn);
}

void freeTermTail  (term_tail_node *termtln){
    if(termtln == NULL || termtln == error) return;

    free(termtln->addOp);
    freeTerm(termtln->termn);
    freeTermTail(termtln->termtln);
    free(termtln);
}

void freeFac       (fac_node *facn){
    if(facn == NULL || facn == error) return;

    free(facn->lbra);
    freeExp(facn->expn);
    free(facn->rbra);
    free(facn->id);
    free(facn->lit);
    free(facn);
}
void freeFacTail   (fac_tail_node *factln){
    if(factln == NULL || factln == error) return;

    free(factln->mulOp);
    freeFac(factln->facn);
    freeFacTail(factln->factln);
    free(factln);
}
