#ifndef SEMANTICS_HEADER
#define SEMANTICS_HEADER

#include "tree.h"

typedef char label[TOKEN_MAX_LENGTH +1];
typedef enum {UNDEF = 0, INT, FLOAT} label_type;

typedef struct LABEL_LIST{
    label *l;
    label_type lt;
    struct LABEL_LIST *next;

}label_list;

extern label_list *global_list;

#endif
