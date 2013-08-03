/*| Submitted by:
|*| Matt Cotter (might be a walrus)
|*| Greg Fournier
|*| Joy Hill
|*/

/* Tokenizer for the Scheme interpreter. */

%{

/* DEFINITIONS - This section contains C code to be included before
   yylex(). You can use it to include libraries, define helper
   functions, etc. */

#include <stdio.h>

#include "common.h"

ConsCell *tokens = NULL;
ConsCell *tokenTail = NULL;
Value currentToken;
Value casualToken;

void addCurrentToken() {
    ConsCell *toBeAdded = (ConsCell *) malloc(sizeof(ConsCell));
    toBeAdded->car = currentToken;
    toBeAdded->cdr.type = consType;
    toBeAdded->cdr.consValue = NULL;
    if (tokens){        
        tokenTail->cdr.consValue = toBeAdded;
    }else{
        tokens = toBeAdded;
    }
    tokenTail = toBeAdded;
}

bool hasPeriod(char* string){
    while(*string){
        if((*string) == '.'){ return true; }
        string++;
    }
    return false;
}

 /* 1: paren 2: newline 0: whitespace */
int checkParen(char toHandle){
    switch(toHandle){
        case('('):
            casualToken.type = openType;
            casualToken.parenValue = parenPType;
            break;
        case('['):
            casualToken.type = openType;
            casualToken.parenValue = squarePType;
            break;
        case('{'):
            casualToken.type = openType;
            casualToken.parenValue = curlyPType;
            break;
        case(')'):
            casualToken.type = closeType;
            casualToken.parenValue = parenPType;
            break;
        case(']'):
            casualToken.type = closeType;
            casualToken.parenValue = squarePType;
            break;
        case('}'):
            casualToken.type = closeType;
            casualToken.parenValue = curlyPType;
            break;
        case('\n'):
            return 2;
        default:
            return 0;
    }
    return 1;
}

%}

/* OPTIONS - Comment these lines out in order to get the yywrap, input
   or unput functions. */
%option noyywrap
%option nounput
%option noinput

whitespace " "|\t
whitespaces {whitespace}+
commentBl ("#|"([^\|]|(\|[^\#]))*"|#")
commentSi (\;[^\n]*\n)

paren \(|\[|\{|\)|\]|\}

ending {whitespace}|{paren}|\n

sign \+|\-
digit [0-9]
uinteger {digit}+
udecimal (\.{digit}+)|({digit}+\.{digit}*)
ureal {udecimal}|{uinteger}
number (({sign}{ureal})|{ureal}){ending}

letter [A-Z]|[a-z]
initial {letter}|\!|\$|\%|\&|\*|\/|\:|\<|\=|\>|\?|\~|\_|\^
subseq {initial}|{digit}|\.|\+|\-
id (({initial}{subseq}*)|\+|\-){ending}


%%

 /* RULES */

 /* Some flavor of paren */
{paren} {
    checkParen(*yytext);
    currentToken = casualToken;
    addCurrentToken();
}

 /* Cymbal // clang // its funny because music and CS */
{id} {
    //printf("Symbol........ ");
    //printf("%s\n", yytext);
    int len = strlen(yytext);
    int casual = checkParen(yytext[len-1]);
    yytext[len-1]='\0';
    currentToken.type = symbolType;
    currentToken.symbolValue = strdup(yytext);
    addCurrentToken();
    if(casual==1){
        currentToken = casualToken;
        addCurrentToken();
    }else if(casual==2){
        return 1;
    }
}

 /* Numbers */
{number} {
    int len = strlen(yytext);
    int casual = checkParen(yytext[len-1]);
    yytext[len-1]='\0';
    int intval = atoi(yytext);
    double realval = atof(yytext);
    if( !hasPeriod(yytext)){
        //Integer
        //printf("Integer....... ");
        //printf("%s\n", yytext);
        currentToken.type = integerType;
        currentToken.integerValue = intval;
        addCurrentToken();
        if(casual==1){
            currentToken = casualToken;
            addCurrentToken();
        }else if(casual==2){
            return 1;
        }
    }else{
        //Non-int
        //printf("Real number... ");
        //printf("%s\n", yytext);
        currentToken.type = realType;
        currentToken.realValue = realval;
        addCurrentToken();
        if(casual==1){
            currentToken = casualToken;
            addCurrentToken();
        }else if(casual==2){
            return 1;
        }
    }
}

 /* True */
\#(t|T) {
    //printf("TRUE.......... ");
    //printf("%s\n", yytext);
    currentToken.type = booleanType;
    currentToken.booleanValue = true;
    addCurrentToken();
}

 /* False */
\#(f|F) {
    //printf("FALSE......... ");
    //printf("%s\n", yytext);
    currentToken.type = booleanType;
    currentToken.booleanValue = false;
    addCurrentToken();
}

 /* Strings */
\"([^\"]|(\\\"))*\" {
    //printf("String........ ");
    //printf("%s\n", yytext);
    currentToken.type = stringType;
    int len = strlen(yytext);
    char* currentString = (char*) malloc(sizeof(char) * (len - 1));
    strncpy(currentString, (yytext+1), len-2);
    currentString[len-2] = '\0';
    currentToken.stringValue = currentString;
    addCurrentToken();
}

 /* New Line or Single Line Comments */
\n|{commentSi} {
    return 1;
}

 /* Whitespace or Block Comments */
{whitespaces}|{commentBl} ;

 /* Other */
. {
    printf("syntax error\n");
    fprintf(stderr,"Untokenizeable input.\n");
    exit(1);
}

