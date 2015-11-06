#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex.h"

static int isKeyWord(char *word);
static token *newToken(token_type type, char *value);
static token_list *addToken(token_list *tl, token_type type, char *buffer, FILE *input);
static token_list *newTokenList(void);
static void delLast(token_list *tl);

/*
 * this is the main function of lexical analyzer. it is called
 * from the main function of a program.
 *
 * variable *input is the FILE pointer which contains the input
 * file. the file should be already correctly opened so the main
 * function should handle possible error situations.
 *
 * return value is type of token_list *. it contains all text
 * from the file (excluding comments) divided into tokens of
 * 5 type. if there is text sequence that does not match with 
 * any token type, the sequence is returned with type TOKEN_ERROR
 */
token_list *lex(FILE *input){
    int i;
    char c, buffer[TOKEN_MAX_LENGTH +1] = {'\0'};
    token_list *head, *tl = newTokenList();

    head = tl;

    /*
     * this is the main loop of lexer. all tokening happens here.
     * there is cases for every element group which is handled 
     * similarly. 
     */
    while(fread(&c, sizeof(char), 1, input) == sizeof(char)){

	memset(buffer, '\0', TOKEN_MAX_LENGTH +1);

	switch (c){
	    
	    /*
	     * this is the only group of items that can be returned
	     * directly after founding. there is no need to read next
	     * characters and thus no need to ungetc it.
	     */
	case '+': case '-': case '*':
	    buffer[0] = c;
	    tl = addToken(tl, TOKEN_OPERATOR, buffer, input);
	    break;

	    /*
	     * here we have three possibilities. the token may be a
	     * division operator '/' or it can be a start of comment of
	     * each type //one line comment or / * multiline comment
	     */
	case '/':
	    buffer[0] = c;
	    if(fread(&c, sizeof(char), 1, input) != sizeof(char)){
		skipch(input);
		tl = addToken(tl, TOKEN_OPERATOR, buffer, input);
	    }
	    else if(c == '/')
		while((c = fgetc(input)) != '\n' && c != EOF);
	    else if(c == '*'){
		while((c = fgetc(input))){
		    if(c == EOF){
			skipch(input);
			tl = addToken(tl, TOKEN_ERROR, buffer, input);
			break;
		    }
		    else if( c != '*')
			continue;
		    else if(fread(&c, sizeof(char), 1, input) != sizeof(char))
			tl = addToken(tl, TOKEN_ERROR, buffer, input);
		    else if(c == '/') break;
		    else
			ungetc(input);
		}
	    }
	    else
		tl = addToken(tl, TOKEN_OPERATOR, buffer, input);

	    break;

	    /*
	     * here we have operator types that cannot be returned only
	     * after 1 char is read. that is because there is possibly
	     * == <= >= operators. so after returning operator from this
	     * casee, the ungetch() macro should be called
	     */
	case '=': case '>': case '<':
	    buffer[0] = c;
	    if(fread(&c, sizeof(char), 1, input) != sizeof(char))
		skipch(input);
	    else if(c == '=')
		buffer[1] = c;

	    tl = addToken(tl, TOKEN_OPERATOR, buffer, input);

	    break;

	    /* here we skip all whitespaces */
	case ' ': case '\n': case '\t':
	    break;

	    /* now the token is keyword, literal, variable or error token */
	default:
	    //next token is now key word or identifier
	    if(isalpha(c)){                                       
		ungetc(input);
		//ota nyt tuo limitti huomioon!!!
		for(i = 0; i <= TOKEN_MAX_LENGTH; i++){
		    if(fread(&c, sizeof(char), 1, input) != sizeof(char)){
			skipch(input);
			break;
		    }
		    else if(isalnum(c) || c == '_')
			buffer[i] = c;
		    else
			break;
		}

		if(isKeyWord(buffer))
		    tl = addToken(tl, TOKEN_KEY_WORD, buffer, input);
		else
		    tl = addToken(tl, TOKEN_IDENTIFIER, buffer, input);

	    }
	    //next token is now integer literal or floating point literal
	    else if(isdigit(c)){
		ungetc(input);
		for(i = 0; i <= TOKEN_MAX_LENGTH; i++){
		    if(fread(&c, sizeof(char), 1, input) != sizeof(char))
			;//tee jotain
		    if(isdigit(c))
			buffer[i] = c;
		    //liukulukuliteraali
		    else if(c == '.'){
			buffer[i] = c;
			for(i++; i <= TOKEN_MAX_LENGTH; i++){
			    if(fread(&c, sizeof(char), 1, input) != sizeof(char))
			       ;//tee jotain
			    if(isdigit(c))
				buffer[i] = c;
			    else
				goto breakpoint;
			}
		    }
		    //kokonaislukuliteraali
		    else
			goto breakpoint;
		}
		
	    breakpoint:
		tl = addToken(tl, TOKEN_LITERAL, buffer, input);
	    }
	    //error tokens here! not implemented yet!
	    else
		;
	    break;
	}
    }
    
    /*
     * we had all along one extra slot for new tokens
     * and it is now completely redundant
     */
    delLast(head);
    
    return head;
}

static token_list *addToken(token_list *tl, token_type type, char *buffer, FILE *input){
    tl->value = newToken(type, buffer);
    tl->next = (token_list *)malloc(sizeof(token_list));
    tl = tl->next;
    tl->next = NULL;
    tl->value = NULL;

    if(type == TOKEN_OPERATOR){
	if(strncmp(buffer, "=", TOKEN_MAX_LENGTH) == 0 ||
	   strncmp(buffer, "<", TOKEN_MAX_LENGTH) == 0 ||
	   strncmp(buffer, "/", TOKEN_MAX_LENGTH) == 0 ||
	   strncmp(buffer, ">", TOKEN_MAX_LENGTH) == 0)
	    ungetc(input);
    }
    else
	ungetc(input);
    
    return tl;
}

/*
 * this function expects that type and value are correct!
 */
static token *newToken(token_type type, char *value){
    token *t = (token *)malloc(sizeof(token));
    
    t->type = type;
    strncpy(t->value, value, TOKEN_MAX_LENGTH +1);

    return t;
}

static int isKeyWord(char *word){
    if(strncmp(word, "int", TOKEN_MAX_LENGTH)   == 0 ||
       strncmp(word, "float", TOKEN_MAX_LENGTH) == 0 ||
       strncmp(word, "if", TOKEN_MAX_LENGTH)    == 0 ||
       strncmp(word, "while", TOKEN_MAX_LENGTH) == 0 ||
       strncmp(word, "for", TOKEN_MAX_LENGTH)   == 0 )
	return 1;
    
    return 0;
}
/*
 * this function is mainly for debugging. it takes token_list *
 * parameter and prints its all nodes.
 *
 * no return value
 */
void printTokenList(token_list *list){
    for(; list; list = list->next){
	printf("%s\t%d\n", list->value->value, list->value->type);
	fflush(NULL);
    }

}

static token_list *newTokenList(void){
    token_list *tl = (token_list *)malloc(sizeof(token_list));

    tl->value = NULL;
    tl->next  = NULL;
    
    return tl;
}

static void delLast(token_list *tl){
    token_list *prev;

    for(; tl->next; prev = tl, tl=tl->next);

    free(prev->next);
    prev->next = NULL;
}
