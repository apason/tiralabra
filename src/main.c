#include <stdio.h>
#include "lex.h"
#include "parser.h"

extern int checkSemantics(program_node *pn);

int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");
    if(input == NULL) return -1;
    token_list *tl = lex(input);
    if(tl == NULL){
	fprintf(stderr, "scanning error\n");
	fflush(NULL);
	return 0;
    }
    program_node *pn = parse(tl);

    if(pn)
	return checkSemantics(pn);
    
    fprintf(stderr, "parse error\n");
    fflush(NULL);
    return 0;
}
