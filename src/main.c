#include <stdio.h>
#include <string.h>

#include "lex.h"
#include "parser.h"
#include "memory.h"

extern int checkSemantics(program_node *pn, FILE *output);

int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");
    FILE *output;
    if(input == NULL) return -1;
    token_list *tl = lex(input);
    fclose(input);
    if(tl == NULL){
	fprintf(stderr, "scanning error\n");
	fflush(NULL);
	return 0;
    }
    program_node *pn = parse(tl);
    freeTokenList(tl);

    if(pn){
	int tmp;
	output = fopen("result.k15", "w");
	tmp =  checkSemantics(pn, output);
	fclose(output);
	return tmp;
    }


    fprintf(stderr, "parse error\n");
    fflush(NULL);
    return 0;
}
