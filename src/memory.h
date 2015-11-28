#ifndef MEMORY_HEADER
#define MEMORY_HEADER

#include "tree.h"
#include "tokens.h"
#include "semantics.h"

/*
 * here is all declarations of memory related functions.
 */

// LEXICAL ANALYSIS ---------------------------------------------

token_list *addToken      (token_list *tl, token_type type, char *buffer, FILE *input);
token      *newToken      (token_type type, char *value);
token_list *newTokenList  (void);


// SYNTAX ANALYSIS -----------------------------------------------
program_node   *newProgramNode (void);
stmt_lst_node  *newStmtLstNode (void);
stmt_node      *newStmtNode    (void);
ass_node       *newAssNode     (void);
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
label_list *newLabelListNode(label *l, label_type lt);

#endif
