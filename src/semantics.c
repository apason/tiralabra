#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex.h"
#include "parser.h"

typedef char label[TOKEN_MAX_LENGTH +1];
typedef enum {UNDEF = 0, INT, FLOAT} label_type;

typedef struct LABEL_LIST{
    label *l;
    label_type lt;
    struct LABEL_LIST *next;

}label_list;

label_list *newLabelListNode(label *l, label_type lt);
int insert(token *id, label_type lt);

int find(token *id);

int program(program_node *pn);
int statementList(stmt_lst_node *sln);
int statement(stmt_node *stmtn);
int expression(exp_node *expn);
int termTail(term_tail_node *termtln);
int term(term_node *termn);
int factorTail(fac_tail_node *factln);
int factor(fac_node *facn);
int if_(if_node *ifn);
int while_(while_node *whilen);
int for_(for_node *forn);
int comparation(comp_node *compn);
int declaration(dec_node *decn);
int assignment(ass_node *assn);

label_list *global_list;
label_type global_type;

int checkSemantics(program_node *pn){
    global_list = NULL;

    return program(pn);
}

int program(program_node *pn){
    return statementList(pn->sln);
}

int statementList(stmt_lst_node *sln){
    if(sln == NULL) return 1;

    return statement(sln->stmtn) && statementList(sln->sln);
}

int statement(stmt_node *stmtn){
    return if_(stmtn->ifn)       &&
	while_(stmtn->whilen)    &&
	for_(stmtn->forn)        &&
	declaration(stmtn->decn) &&
	assignment(stmtn->assn);
}

int expression(exp_node *expn){
    if(expn == NULL) return 1;

    return term(expn->termn) && termTail(expn->termtln);
}

int termTail(term_tail_node *termtln){
    if(termtln == NULL) return 1;

    return term(termtln->termn) && termTail(termtln->termtln);
}

int term(term_node *termn){
    return factor(termn->facn) && factorTail(termn->factln);
}

int factorTail(fac_tail_node *factln){
    if(factln == NULL) return 1;

    return factor(factln->facn) && factorTail(factln->factln);
}

int factor(fac_node *facn){


    if(facn->id != NULL)
	if(global_type == UNDEF) 
	    return (global_type = find(facn->id));
	else
	    if(global_type != find(facn->id)){
	       fprintf(stderr, "Error: Mixing int and float types id\n");
	       return 0;
	    }
	    else
		return 1;
    
    else if(facn->lit != NULL)
	if(global_type == UNDEF)
	    return (global_type = findType(facn->lit));
	else
	    if(global_type != findType(facn->lit)){
		fprintf(stderr, "Error: Mixing int and float types lit expected value: %d\n", global_type);
		return 0;
	    }
	    else return 1;

    return expression(facn->expn);
}

int if_(if_node *ifn){
    if(ifn == NULL) return 1;
    
    return comparation(ifn->compn) && statement(ifn->stmtn);
}

int while_(while_node *whilen){
    if(whilen == NULL) return 1;
    
    return comparation(whilen->compn) && statement(whilen->stmtn);
}

int for_(for_node *forn){
    if(forn == NULL) return 1;
    
    if( assignment(forn->assn) && comparation(forn->compn)){
	if((global_type = find(forn->id)) == 0)
	    return 0;
	if(expression(forn->expn)){
	   global_type = UNDEF;
	   return statement(forn->stmtn);
	}
    }
    return 0;
}

int comparation(comp_node *compn){
    if(expression(compn->expn) && expression(compn->expn2)){
	global_type = UNDEF;
	return 1;
    }
    return 0;
}

int declaration(dec_node *decn){
    label_type lt;

    if(decn == NULL)          return 1;

    if(strncmp(decn->typeKey->value, "int", TOKEN_MAX_LENGTH) == 0)
	lt = INT;
    else
	lt = FLOAT;
    
    if(insert(decn->id, lt) == 0) return 0; //duplicate definition
    
    return 1;
}

int assignment(ass_node *assn){
    if(assn == NULL)        return 1;
    if(assn->id == NULL)    return 1;                 //empty statement detected
    if((global_type = find(assn->id)) == 0) return 0; //undefined variable


    if(expression(assn->expn)){
	global_type = UNDEF;
	return 1;
    }
    
    return 0;
}

label_list *newLabelListNode(label *l, label_type lt){
    label_list *new = (label_list*)malloc(sizeof(label_list));

    new->l    = l;
    new->lt   = lt; 
    new->next = global_list; //head of the label list

    return new;
}

/*
 * this function inserts new label "id" to endbeginning of the list.
 * if the list already contains that label, nothing is done 
 * and 0 is returned. in succes 1 is returned
 */

int insert(token *id, label_type lt){
    label_list *tmp = newLabelListNode((label*)id->value, lt);
    label_list *prev;

    global_list = tmp; //move new node to first 

    //check for duplicate (start from second node
    for(tmp = global_list->next, prev = global_list; tmp != NULL; tmp = tmp->next, prev = prev->next)
	if(strncmp((char*)tmp->l, id->value, TOKEN_MAX_LENGTH +1) == 0){ //pointer??
	    prev->next = tmp->next;
	    free(tmp);
	    fprintf(stderr, "duplicate definition of variable %s\n", id->value);
	    return 0;
	}
    return 1;
}

int find(token *id){
    label_list *tmp;

    for(tmp = global_list; tmp != NULL; tmp = tmp->next)
	if(strncmp(id->value, (char*)tmp->l, TOKEN_MAX_LENGTH +1) == 0)
	    return tmp->lt;

    fprintf(stderr, "undefined variable %s\n", id->value);
    return UNDEF;
}

int findType(token *lit){
    if(strchr(lit->value, '.') == NULL)
	return INT;
    
    return FLOAT;
}
