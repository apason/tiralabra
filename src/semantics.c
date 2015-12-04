#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"
#include "tree.h"
#include "label.h"
#include "memory.h"


static int         insert          (token *id, label_type lt);
static int         findLabelType   (token *id);
static int         findLiteralType (token *lit);

static int         program         (program_node *pn);
static int         statementList   (stmt_lst_node *sln);
static int         statement       (stmt_node *stmtn);
static int         expression      (exp_node *expn);
static int         termTail        (term_tail_node *termtln);
static int         term            (term_node *termn);
static int         factorTail      (fac_tail_node *factln);
static int         factor          (fac_node *facn);
static int         if_             (if_node *ifn);
static int         while_          (while_node *whilen);
static int         for_            (for_node *forn);
static int         comparation     (comp_node *compn);
static int         declaration     (dec_node *decn);
static int         assignment      (ass_node *assn);

/*
 * to avoid passing head of the label list to every function, 
 * is address is kept in global variable global_list.
 * 
 * variable global_type indicates what type the next literal
 * or variable should be. if the actual type of next token 
 * is not the expected one, error is caused and compilation
 * can not be finished. if the value of global type is 
 * UNDEF, any type is accepted and the expexted value in
 * global_type is set to that type.
 *
 * with that system it is easy to do semantic checks about
 * types: in some cases we know that all types in particular
 * subtree should be same so we can initialize expected value
 * to UNDEF in start of recursion and initialize it again 
 * after that subtree. subtrees that should contain only one
 * type of variables/literals: expression, comparation,
 * assignment. this method should also be used in 
 * id = expression part of for nores
 */

static label_list *global_list;
static label_type global_type;

static FILE *output;

/*
 * main function of semantic analysis. semantics checking
 * includes that there is no use of undefined variables and
 * no variables can ve defined multiple times. also check for
 * type correctness is done. (check the last comment above)
 *
 * input parameter *pn is pointer to syntax tree
 * return 1 if there was no errors, 0 otherwise.
 */
int checkSemantics(program_node *pn, FILE *outputfile){
    int tmp;
    
    global_list = NULL;
    global_type = UNDEF;

    output = outputfile;

    tmp = program(pn);

    freeLabelList(global_list);
    freeSyntaxTree(pn, NULL);

    return tmp;
}

/*
 * here is function for every non-leaf node in the syntax
 * tree, which is executed in the same way as the parsing
 * phase. if there is a semantic error, 0 is returned,
 * 1 otherwise.
 */
static int program(program_node *pn){
    if(statementList(pn->sln) == 0) return 0;

    fprintf(output, "svc sp, =HALT\n");

    return 1;
}

static int statementList(stmt_lst_node *sln){
    if(sln == NULL) return 1;

    return statement(sln->stmtn) && statementList(sln->sln);
}

static int statement(stmt_node *stmtn){
    if( if_(stmtn->ifn)          &&
	while_(stmtn->whilen)    &&
	for_(stmtn->forn)        &&
	declaration(stmtn->decn) &&
	statementList(stmtn->sln)&&
	assignment(stmtn->assn)){

	if(stmtn->ins != NULL)
	    fprintf(output, "%s\n", stmtn->ins->value);

	return 1;
    }
    
    return 0;
}

static int if_(if_node *ifn){
    static int if_nro = 0;
    int        if_cur = if_nro;

    if_nro++;

    if(ifn == NULL) return 1;

    if(comparation(ifn->compn) == 0) return 0;

    fprintf(output, "jzer r0, =_if%d_end\n", if_cur);

    if(statement(ifn->stmtn) == 0) return 0;

    fprintf(output, "_if%d_end nop\n", if_cur);

    return 1;
}

static int while_(while_node *whilen){
    static int while_nbr = 0;
    int        while_cur = while_nbr;

    while_cur++;
    
    if(whilen == NULL) return 1;

    fprintf(output, "_while%d_beginning nop\n", while_cur);

    if(comparation(whilen->compn) == 0) return 0;

    fprintf(output, "jzer r0, =_while%d_end\n", while_cur);
    
    if(statement(whilen->stmtn) == 0) return 0;

    fprintf(output, "jump =_while%d_beginning\n", while_cur);
    fprintf(output, "_while%d_end nop\n", while_cur);

    return 1;
}

/*
 * here we need some type checking because id leaf of for node
 * should has the same type as the following expression subtree
 */
static int for_(for_node *forn){
    static int for_nbr = 0;
    int        for_cur = for_nbr;

    for_nbr++;
    
    if(forn == NULL) return 1;

    if(assignment(forn->assn) == 0) return 0;

    fprintf(output, "_for%d_beginning nop\n", for_cur);

    if(comparation(forn->compn) == 0) return 0;

    fprintf(output, "jzer r0, =_for%d_end\n", for_cur);

    if(statement(forn->stmtn) == 0) return 0;
    
    if((global_type = findLabelType(forn->id)) == 0)
	return 0;


    if(expression(forn->expn) == 0) return 0;

    fprintf(output, "pop sp, r3\nstore r3, =%s\n", forn->id->value);
    fprintf(output, "jump =_for%d_beginning\n", for_cur);
    fprintf(output, "_for%d_end nop\n", for_cur);
	    
    global_type = UNDEF;


    return 1;
}

/*
 * comparation is also subtree that must have only one
 * kind of tokens. after checks global_type is reseted
 */
static int comparation(comp_node *compn){
    static int comp_nbr = 0;
    
    if(expression(compn->expn) == 0 || expression(compn->expn2) == 0)
	return 0;
    
    fprintf(output, "load r0, =0\npop sp, r2\npop sp, r1\ncomp r1, r2\n");

    if(     (strncmp(compn->compOp->value, "==", TOKEN_MAX_LENGTH) == 0))
	fprintf(output, "jequ =_comp%d_true\n",  comp_nbr);
    else if((strncmp(compn->compOp->value, "!=", TOKEN_MAX_LENGTH) == 0))
	fprintf(output, "jnequ =_comp%d_true\n", comp_nbr);
    else if((strncmp(compn->compOp->value, "<=", TOKEN_MAX_LENGTH) == 0))
	fprintf(output, "jngre =_comp%d_true\n", comp_nbr);
    else if((strncmp(compn->compOp->value, ">=", TOKEN_MAX_LENGTH) == 0))
	fprintf(output, "jnles =_comp%d_true\n", comp_nbr);
    else if((strncmp(compn->compOp->value, "<",  TOKEN_MAX_LENGTH) == 0))
	fprintf(output, "jles =_comp%d_true\n",  comp_nbr);
    else if((strncmp(compn->compOp->value, ">",  TOKEN_MAX_LENGTH) == 0))
	fprintf(output, "jgre =_comp%d_true\n",  comp_nbr);

    fprintf(output, "jump =_comp%d_end\n",        comp_nbr);
    fprintf(output, "_comp%d_true load r0, =1\n", comp_nbr);
    fprintf(output, "_comp%d_end nop\n",          comp_nbr);
	
    comp_nbr++;
    global_type = UNDEF;
	
    return 1;
}

/*
 * declaration node should allways check for duplicate
 * definitions.
 */
static int declaration(dec_node *decn){
    label_type lt;

    if(decn == NULL)          return 1;

    if(strncmp(decn->typeKey->value, "int", TOKEN_MAX_LENGTH) == 0)
	lt = INT;
    else
	lt = FLOAT;
    
    if(insert(decn->id, lt) == 0) return 0;                    //duplicate definitio

    fprintf(output, "%s dc 0\nnop\n", decn->id->value);

    return 1;
}

/*
 * assignment subtrees can also operat with only one 
 * kind of types.
 */
static int assignment(ass_node *assn){
    if(assn == NULL)        return 1;
    if(assn->id == NULL)    return 1;                          //empty statement
    if((global_type = findLabelType(assn->id)) == 0) return 0; //undefined variable


    if(expression(assn->expn)){
	fprintf(output, "pop sp, r3\nstore r3, =%s\n", assn->id->value);
	global_type = UNDEF;
	return 1;
    }
    
    return 0;
}

static int expression(exp_node *expn){
    if(expn == NULL) return 1;

    return term(expn->termn) && termTail(expn->termtln);
}

static int termTail(term_tail_node *termtln){
    if(termtln == NULL) return 1;
    if(term(termtln->termn) == 0)
	return 0;

    if(global_type == INT){
	if(termtln->addOp->value[0] == '+')
	    fprintf(output, "pop sp, r2\npop sp, r1\nadd r1, r2\npush sp, r1\n");
	else
	    fprintf(output, "pop sp, r2\npop sp, r1\nsub r1, r2\npush sp, r1\n");
    }
    else{
	if(termtln->addOp->value[0] == '+')
	    fprintf(output, "pop sp, r2\npop sp, r1\nfadd r1, r2\npush sp, r1\n");
	else
	    fprintf(output, "pop sp, r2\npop sp, r1\nfsub r1, r2\npush sp, r1\n");
    }

    return termTail(termtln->termtln);
}

static int term(term_node *termn){
    return factor(termn->facn) && factorTail(termn->factln);
}

static int factorTail(fac_tail_node *factln){
    if(factln == NULL) return 1;
    if(factor(factln->facn) == 0)
	return 0;
    if(global_type == INT){
	if(factln->mulOp->value[0] == '*')
	    fprintf(output, "pop sp, r2\npop sp, r1\nmul r1, r2\npush sp, r1\n");
	else
	    fprintf(output, "pop sp, r2\npop sp, r1\ndiv r1, r2\npush sp, r1\n");
    }
    else{
	if(factln->mulOp->value[0] == '*')
	    fprintf(output, "pop sp, r2\npop sp, r1\nfmul r1, r2\npush sp, r1\n");
	else
	    fprintf(output, "pop sp, r2\npop sp, r1\nfdiv r1, r2\npush sp, r1\n");
    }
		    
    return factorTail(factln->factln);
}

/*
 * factor nodes are the latest non-leaf nodes in the tree so
 * checks for compatible type operations has to be done here.
 */
static int factor(fac_node *facn){

    if(facn->id != NULL){

	if(global_type == UNDEF) {
	     if((global_type = findLabelType(facn->id)) == 0) return 0;
	     fprintf(output, "push sp, %s\n", facn->id->value);
	     return 1;
	}

	else{
	    if(global_type != findLabelType(facn->id)){
		fprintf(stderr, "Error: Mixing int and float types id\n");
	       return 0;
	    }
	    else{
		fprintf(output, "push sp, %s\n", facn->id->value);
		return 1;
	    }
	}
    }
    else if(facn->lit != NULL){

	if(global_type == UNDEF){
	    global_type = findLiteralType(facn->lit);

	    if(global_type == FLOAT)
		fprintf(output, "fload r3, =%s\n", facn->lit->value);
	    else
		fprintf(output, "load r3, =%s\n", facn->lit->value);
	    fprintf(output, "push sp, r3\n");

	    return 1;
	}
	else{
	    if(global_type != findLiteralType(facn->lit)){
		fprintf(stderr, "Error: Mixing int and float types. expected value: %d\n", global_type);
		return 0;
	    }
	    else{
		//push sp, factn->lit->value huom tyyppi!

		if(global_type == FLOAT)
		    fprintf(output, "fload r3, =%s\n", facn->lit->value);
		else
		    fprintf(output, "load r3, =%s\n", facn->lit->value);
		fprintf(output, "push sp, r3\n");

		return 1;
	    }
	}
    }

    return expression(facn->expn);
}

/*
 * this function inserts new label "id" to beginning of the list.
 * if the list already contains that label, duplicate is removed
 * and 0 is returned. in succes 1 is returned
 */

static int insert(token *id, label_type lt){
    label_list *tmp = newLabelListNode(&global_list, (label*)id->value, lt);
    label_list *prev;

    global_list = tmp;

    //check for duplicate (start from second node
    for(tmp  = global_list->next, prev = global_list;
	tmp != NULL;
	tmp  = tmp->next,         prev = prev->next)
	
	if(strncmp((char*)tmp->l, id->value, TOKEN_MAX_LENGTH +1) == 0){
	    prev->next = tmp->next;
	    free(tmp);
	    fprintf(stderr, "duplicate definition of variable %s\n", id->value);
	    return 0;
	}
    
    return 1;
}

/*
 * search value of *id from the variable list.
 * return value is its type. UNDEF, FLOAT or INT.
 */
static int findLabelType(token *id){
    label_list *tmp;

    for(tmp = global_list; tmp != NULL; tmp = tmp->next)
	if(strncmp(id->value, (char*)tmp->l, TOKEN_MAX_LENGTH +1) == 0)
	    return tmp->lt;

    fprintf(stderr, "undefined variable %s\n", id->value);
    return UNDEF;
}

/*
 * checks whether the string has '.' inside.
 * if it has, the literal is treated as a
 * float and value FLOAT is returned. INT
 * otherwise 
 */
static int findLiteralType(token *lit){
    if(strchr(lit->value, '.') == NULL)
	return INT;
    
    return FLOAT;
}
