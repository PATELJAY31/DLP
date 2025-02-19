%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

/* Regular definitions for token classes */
KEYWORD      (int|char|return|void|if|else|while|for|break|continue)
IDENTIFIER   [a-zA-Z_][a-zA-Z0-9_]*
CONSTANT     [0-9]+|0[xX][0-9a-fA-F]+
STRING       '([^\\']|\\.)*'|"([^\\"]|\\.)*"
PUNCTUATION  [(){};,]
OPERATOR     [=+\-*/%]
COMMENT      \/\*([^*]|\*+[^/*])*\*+\/|\/\/.*
WHITESPACE   [ \t\n]+

%%
{KEYWORD}      { printf("Keyword: %s\n", yytext); }
{IDENTIFIER}   { printf("Identifier: %s\n", yytext); }
{CONSTANT}     { printf("Constant: %s\n", yytext); }
{STRING}       { printf("String: %s\n", yytext); }
{PUNCTUATION}  { printf("Punctuation: %s\n", yytext); }
{OPERATOR}     { printf("Operator: %s\n", yytext); }
{COMMENT}      { /* Ignore comments */ }
{WHITESPACE}   { /* Ignore whitespaces */ }
.              { printf("Lexical Error: %s\n", yytext); }
%%

int main(int argc, char *argv[]) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("Error opening file");
            return 1;
        }
        yyin = file;
    }
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
