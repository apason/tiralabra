#include "lex.h"
#include "parser.h"

int main(int argc, char *argv[]){
    FILE *input = fopen(argv[1], "r");
    if(input == NULL) return -1;
    token_list *tl = lex(input);
    return parse(tl) != NULL ? 1 : 0;
}
