%{
/* Declarations section */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
%}

%%
[cC][hH][aA][rR][uU][sS][aA][tT]    { printf("university"); }
.                                    { printf("%s", yytext); }
\n                                   { 
    printf("\nEnter text (Ctrl+D to exit): "); 
}
%%

int main() {
    printf("=== Word Replacer ===\n");
    printf("This program replaces 'charusat' with 'university' (case-insensitive).\n");
    printf("\nEnter text: ");
    
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
