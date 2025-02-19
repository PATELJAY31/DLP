%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

/* Definitions */
DIGIT       [0-9]
LETTER      [a-zA-Z]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
CONSTANT    {DIGIT}+
STRING      \"([^\"]|\\\")*\"
CHARACTER   \'([^\']|\\\')\'
KEYWORD     int|char|return|if|else|while|for|void
OPERATOR    [=+\-*/%]
PUNCTUATION [(){},;]
COMMENT     \/\*([^*]|(\*+[^*/]))*\*+\/|\/\/.*
WHITESPACE  [ \t\n]+

%%
{KEYWORD}      { printf("Keyword: %s\n", yytext); }
{IDENTIFIER}   { printf("Identifier: %s\n", yytext); }
{CONSTANT}     { printf("Constant: %s\n", yytext); }
{STRING}       { printf("String: %s\n", yytext); }
{CHARACTER}    { printf("String: %s\n", yytext); }
{OPERATOR}     { printf("Operator: %s\n", yytext); }
{PUNCTUATION}  { printf("Punctuation: %s\n", yytext); }
{COMMENT}      { /* Ignore comments */ }
{WHITESPACE}   { /* Ignore whitespace */ }
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
