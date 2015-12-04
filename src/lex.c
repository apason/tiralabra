#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lex.h"
#include "tokens.h"
#include "memory.h"

static token_list *handleOthers     (token_list *tl, char *buffer, FILE *input);
static token_list *handleLiterals   (token_list *tl, char *buffer, FILE *input);
static token_list *handleComps      (token_list *tl, char *buffer, FILE *input);
static token_list *handleSlash      (token_list *tl, char *buffer, FILE *input);
static token_list *handleErrors     (token_list *tl, char *buffer, FILE *input);
static token_list *handleNO         (token_list *tl, char *buffer, FILE *input);
static token_list *handleInstruction(token_list *tl, char *buffer, FILE *input);
    

static int        isTypeKey      (char *word);
static int        isIfKey        (char *word);
static int        isWhileKey     (char *word);
static int        isForKey       (char *word);

static void       addEOF         (token_list *tl);


static int        line_number;
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
    token_list *tmp, *head, *tl = newTokenList();
    char c, buffer[TOKEN_MAX_LENGTH +1];
    line_number = 1;

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
	     * this and next 6 groups are the only groups of items that can
	     * be returned directly after founding. there is no need to read next
	     * characters and thus no need to ungetc it.
	     */

	    //some kind of map system would be useful here?
	case '+': case '-': 
	    buffer[0] = c;
	    tl = addToken(tl, TOKEN_ADDOP, buffer, line_number, input);
	    break;

	case '*':
	    buffer[0] = c;
	    tl = addToken(tl, TOKEN_MULOP, buffer, line_number, input);
	    break;

	case '(':
	    buffer[0] = c;
	    tl = addToken(tl, TOKEN_LBRA, buffer, line_number, input);
	    break;

	case ')':
	    buffer[0] = c;
	    tl = addToken(tl, TOKEN_RBRA, buffer, line_number, input);
	    break;

	case '{':
	    buffer[0] = c;
	    tl = addToken(tl, TOKEN_LCUR, buffer, line_number, input);
	    break;

	case '}':
	    buffer[0] = c;
	    tl = addToken(tl, TOKEN_RCUR, buffer, line_number, input);
	    break;

	case ';':
	    buffer[0] = c;
	    tl = addToken(tl, TOKEN_SCOL, buffer, line_number, input);
	    break;


	case '$':
	    tl = handleInstruction(tl, buffer, input);
	    break;

	    /*
	     * lets handle out 3 possibilities with handleSlash()
	     */
	case '/':
	    buffer[0] = c;
	    if((tmp = handleSlash(tl, buffer, input)) != NULL)
	       tl = tmp;
	    break;

	    /*
	     * cases for all comparation excluded not equals token
	     */
	case '>': case '<': case '=':
	    buffer[0] = c;
	    tl = handleComps(tl, buffer, input);
	    break;

	    /*
	     * case for not equals operator
	     */
	case '!':
	    buffer[0] = c;
	    tl = handleNO(tl, buffer, input);
	    break;
	    
	    /* here we skip all whitespaces */
	case ' ': case '\n': case '\t':
	    if(c == '\n')
		line_number++;
	    break;

	    /* now the token is keyword, literal, variable or error token */
	default:
	    
	    /*
	     * next token is now key word or identifier
	     */
	    if(isalpha(c))
		tl = handleOthers(tl, buffer, input);
	    
	    /*
	     * next token is now integer literal or
	     * floating point literal
	     */
	    else if(isdigit(c))
		tl = handleLiterals(tl, buffer, input);

	    /*
	     * everything else is error
	     */
	    else{
		ungetc(input);
		tl = handleErrors(tl, buffer, input);
	    }

	    break;
	}
    }
    
    /*
     * we had all along one extra slot for new tokens.
     * now we can mark it as EOF
     */
    addEOF(head);
    
    return head;
}

/*
 * here we have three possibilities. the token may be a
 * division operator '/' or it can be a start of comment of
 * each type //one line comment or / * multiline comment
 */
static token_list *handleSlash(token_list *tl, char *buffer, FILE *input){
    char c;

    /* check for eof */
    if(fread(&c, sizeof(char), 1, input) != sizeof(char)){
	skipch(input);
	return addToken(tl, TOKEN_MULOP, buffer, line_number, input);
    }
    
    /* 
     * one line comment detected!
     */
    else if(c == '/'){
	while((c = fgetc(input)) != '\n' && c != EOF);
	if(c == '\n')
	    line_number++;
    }
    
    /* 
     * start of multiline comment
     */
    else if(c == '*'){

	while((c = fgetc(input))){
	    if(c == EOF)
		return addToken(tl, TOKEN_ERROR, buffer, line_number, input);

	    else if( c != '*')
		continue;
	    
	    else if(fread(&c, sizeof(char), 1, input) != sizeof(char))
		return addToken(tl, TOKEN_ERROR, buffer, line_number, input);

	    else if(c == '/') break;

	    else
		ungetc(input);
	}
    }
    
    /* it was just a division operator */
    else
	return addToken(tl, TOKEN_MULOP, buffer, line_number, input);

    return NULL;
}

static token_list *handleInstruction(token_list *tl, char *buffer, FILE *input){
    char c;
    int i = 0;

    while((c = fgetc(input)) && i < TOKEN_MAX_LENGTH){
	if(c == EOF || c == '\n')
	    return addToken(tl, TOKEN_INSTRUCTION, buffer, line_number++, input);

	buffer[i++] = c;
    }

    fprintf(stderr, "warning: ASM line is over %d characters\n", TOKEN_MAX_LENGTH);
    return addToken(tl, TOKEN_INSTRUCTION, buffer, line_number, input);
}

/*
 * here we have operator types that cannot be returned only
 * after 1 char is read. that is because there is possibly
 * == <= >= operators. so after returning operator from this
 * casee, the ungetch() macro should be called
 */
static token_list *handleComps(token_list *tl, char *buffer, FILE *input){
    char c;
    
    if(fread(&c, sizeof(char), 1, input) != sizeof(char))
	skipch(input);
    else if(c == '=')
	buffer[1] = c;

    if(strncmp(buffer, "=", TOKEN_MAX_LENGTH) == 0)
	return addToken(tl, TOKEN_ASSOP, buffer, line_number, input);
    
    else{

	if(strlen(buffer) > 1)
	    skipch(input);
	
	return addToken(tl, TOKEN_COMPOP, buffer, line_number, input);
    }
}

/*
 * case for not equals operator
 */
static token_list *handleNO(token_list *tl, char *buffer, FILE *input){
    char c;
    
    if(fread(&c, sizeof(char), 1, input) != sizeof(char))
	tl = addToken(tl, TOKEN_ERROR, buffer, line_number, input);
    
    if(c == '='){
	buffer[1] = c;
	skipch(input);
	return addToken(tl, TOKEN_COMPOP, buffer, line_number, input);
    }
    else{
	ungetc(input);
	return addToken(tl, TOKEN_ERROR, buffer, line_number, input);
    }
}

/*
 * here we have two possibilities. next token can be identifier
 * or language key word. 
 */
static token_list *handleOthers(token_list *tl, char *buffer, FILE *input){
    int  i;
    char c;
    
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

    if(isTypeKey(buffer))
	return addToken(tl, TOKEN_TYPEKEY, buffer, line_number, input);
    else if(isIfKey(buffer))
	return addToken(tl, TOKEN_IFKEY, buffer, line_number, input);
    else if(isWhileKey(buffer))
	return addToken(tl, TOKEN_WHILEKEY, buffer, line_number, input);
    else if(isForKey(buffer))
	return addToken(tl, TOKEN_FORKEY, buffer, line_number, input);
    else
	return addToken(tl, TOKEN_IDENTIFIER, buffer, line_number, input);

}

/*
 * literals can be either integer or floating point literals
 */
static token_list *handleLiterals(token_list *tl, char *buffer, FILE *input){
    int  i;
    char c;
    
    ungetc(input);
    
    for(i = 0; i <= TOKEN_MAX_LENGTH; i++){

	if(fread(&c, sizeof(char), 1, input) != sizeof(char)){
	    skipch(input);
	    return addToken(tl, TOKEN_LITERAL, buffer, line_number, input);
	}
		
	if(isdigit(c))
	    buffer[i] = c;

	/*
	 * floating point literal detected
	 */
	else if(c == '.'){
	    buffer[i] = c;

	    for(i++; i <= TOKEN_MAX_LENGTH; i++){

		if(fread(&c, sizeof(char), 1, input) != sizeof(char)){
		    skipch(input);
		    return addToken(tl, TOKEN_LITERAL, buffer, line_number, input);
		}

		if(isdigit(c))
		    buffer[i] = c;
		else
		    goto breakpoint;
	    }
	}

	/*
	 * end of the literal
	 */
	else
	    goto breakpoint;
    }
		
 breakpoint:
    return addToken(tl, TOKEN_LITERAL, buffer, line_number, input);
}

/*
 * error tokens here! lets scan to next whitespace and
 * treat the whole word as an error token
 */
static token_list *handleErrors(token_list *tl, char *buffer, FILE *input){
    int  i, tmp = line_number;
    char c;

    for(i = 0; i <= TOKEN_MAX_LENGTH; i++){

  	if(fread(&c, sizeof(char), 1, input) != sizeof(char))
	    break;

	if(c == ' ' || c == '\n' || c == '\t'){
	    if(c == '\n')
		line_number++;
	    break;
	}

	buffer[i] = c;
    }

    return addToken(tl, TOKEN_ERROR, buffer, tmp, input);
}

/*
 * Following functions are used to check if the buffer
 * contains one of the reserved key words in the language.
 *
 * input parameter word is pointer to buffer in switch statement
 * return 1 if it is reserved key we are looking for, 0 otherwise
 */
static int isIfKey(char *word){

    if(strncmp(word, "if", TOKEN_MAX_LENGTH)    == 0)
	return 1;
    
    return 0;
}     
static int isTypeKey(char *word){

    if(strncmp(word, "int", TOKEN_MAX_LENGTH)   == 0 ||
       strncmp(word, "float", TOKEN_MAX_LENGTH) == 0 )
	return 1;
    
    return 0;
}

static int isForKey(char *word){

    if(strncmp(word, "for", TOKEN_MAX_LENGTH)   == 0 )
	return 1;

    return 0;
}

static int isWhileKey(char *word){

    if(strncmp(word, "while", TOKEN_MAX_LENGTH) == 0 )
	return 1;

    return 0;
}

/*
 * this function adds special EOF token
 * to the end of the token list
 */
static void addEOF(token_list *tl){
    for(; tl->next; tl=tl->next);

    tl->value = newToken(TOKEN_EOF, "EOF", line_number);
}

/*
 * this function is mainly for debugging. it takes token_list *
 * parameter and prints its all nodes.
 *
 * no return value
 */
void printTokenList(token_list *list){
    for(; list; list = list->next)
	printf("%s\t%d\n", list->value->value, list->value->type);
}


int correctTokenList(token_list *list){
    int ret = 1;
    
    for(; list; list = list->next)
	if(list->value->type == TOKEN_ERROR){
	    fprintf(stderr, "error in line %d: invalid character sequence %s\n", list->value->line_number, list->value->value);
	    ret = 0;
	}
    
    return ret;
}

