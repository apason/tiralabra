#include <stdio.h>

#include "lex.h"
#include "parser.h"

extern int checkSemantics(program_node *pn, FILE *output);

int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen("result.k15", "w");
    if(input == NULL) return -1;
    if(output == NULL) return -1;
    return checkSemantics(parse(lex(input)), output) > 0 ? 1 : 0;
}
