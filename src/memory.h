#ifndef MEMORY_HEADER
#define MEMORY_HEADER

#include "tree.h"
#include "tokens.h"
#include "label.h"
/*
 * here is all declarations of memory related functions.
 */

// LEXICAL ANALYSIS ---------------------------------------------

token_list *addToken      (token_list *tl, token_type type, char *buffer, FILE *input);
token      *newToken      (token_type type, char *value);
token_list *newTokenList  (void);
void        freeTokenList (token_list *tl);

// SYNTAX ANALYSIS -----------------------------------------------
program_node   *newProgramNode (void);
stmt_lst_node  *newStmtLstNode (void);
stmt_node      *newStmtNode    (void);
ass_node       *newAssNode     (void);void freeLabelList(label_list *ll);
fac_node       *newFacNode     (void);
fac_tail_node  *newFacTailNode (void);
term_node      *newTermNode    (void);
term_tail_node *newTermTailNode(void);
exp_node       *newExpNode     (void);
dec_node       *newDecNode     (void);
if_node        *newIfNode      (void);
while_node     *newWhileNode   (void);
for_node       *newForNode     (void);
comp_node      *newCompNode    (void);


// SEMANTIC ANALYSIS ---------------------------------------------
label_list *newLabelListNode(label_list **list, label *l, label_type lt);
void freeLabelList (label_list *ll);
void freeSyntaxTree(program_node *pn, void *err);
void freeProgram   (program_node *pn);
void freeStmtLst   (stmt_lst_node *stmtln);
void freeStmt      (stmt_node *stmtn);
void freeIf        (if_node *ifn);
void freeWhile     (while_node *whilen);
void freeFor       (for_node *forn);
void freeComp      (comp_node *compn);
void freeDec       (dec_node *decn);
void freeAss       (ass_node *assn);
void freeExp       (exp_node *expn);
void freeTerm      (term_node *termn);
void freeTermTail  (term_tail_node *termtln);
void freeFac       (fac_node *facn);
void freeFacTail   (fac_tail_node *factln);

#endif
