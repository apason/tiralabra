#ifndef parser_header
#define parser_header

#include "lex.h"

typedef unsigned int           consumption_type;

typedef struct EXP_NODE        exp_node;
typedef struct TERM_TAIL_NODE  term_tail_node;
typedef struct TERM_NODE       term_node;
typedef struct FAC_TAIL_NODE   fac_tail_node;
typedef struct FAC_NODE        fac_node;
typedef struct DEC_NODE        dec_node;
typedef struct ASS_NODE        ass_node;
typedef struct IF_NODE         if_node;
typedef struct WHILE_NODE      while_node;
typedef struct FOR_NODE        for_node;
typedef struct PROGRAM_NODE    program_node;
typedef struct STMT_LST_NODE   stmt_lst_node;
typedef struct STMT_NODE       stmt_node;
typedef struct COMP_NODE       comp_node;

extern program_node *parse(token_list *tlist);

struct EXP_NODE{
     term_node           *termn;
     term_tail_node      *termtln;
    
};

struct TERM_TAIL_NODE{
     token               *addOp;
     term_node           *termn;
     term_tail_node      *termtln;
    
};

struct TERM_NODE{
     fac_node            *facn;
     fac_tail_node       *factln;
    
};

struct FAC_TAIL_NODE{
     token               *mulOp;
     fac_node            *facn;
     fac_tail_node       *factln;
};

struct FAC_NODE{
     token               *lbra;
     exp_node            *expn;
     token               *rbra;
     token               *id;
     token               *lit;
    
};

struct COMP_NODE{
    exp_node            *expn;
    token               *compOp;
    exp_node            *expn2;
};

struct IF_NODE{
    token               *ifKey;
    token               *lbra;
    comp_node           *compn;
    token               *rbra;
    stmt_node           *stmtn;

};

struct WHILE_NODE{
     token               *whileKey;
     token               *lbra;
    comp_node           *compn;
     token               *rbra;
     stmt_node           *stmtn;

};

struct FOR_NODE{
     token               *forKey;
     token               *lbra;
     ass_node            *assn;
    comp_node           *compn;
     token               *scol;
     token               *id;
     token               *assOp;
     exp_node            *expn;
     token               *rbra;
     stmt_node           *stmtn;

};

struct DEC_NODE{
     token               *typeKey;
     token               *id;
     token               *scol;
    
};

struct ASS_NODE{
     token               *id;
     token               *assOp;
     exp_node            *expn;
     token               *scol;

};

struct STMT_NODE{
     if_node             *ifn;
     while_node          *whilen;
     for_node            *forn;
     dec_node            *decn;
     ass_node            *assn;
    
};

struct STMT_LST_NODE{
    stmt_node           *stmtn;
    stmt_lst_node       *sln;
    
};

struct PROGRAM_NODE{
     stmt_lst_node       *sln;
     token               *eof;
    
};



#endif
