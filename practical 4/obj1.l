%{
/* Declarations section */
#include <stdio.h>
#include <stdlib.h>

/* Keep track of line state */
int numbers_in_line = 0;
%}

/* Rule section */
%%
[0-9]+  { 
    printf("%s\n", yytext);  /* Print each number on its own line */
    numbers_in_line++;
}
\n      {
    if (numbers_in_line == 0) {
        printf("No numbers found in input\n");
    }
    printf("\nEnter text (Ctrl+D to exit): ");
    numbers_in_line = 0;
}
.       ;  /* Skip non-numeric characters */
%%

/* Main program section */
int main() {
    printf("=== Number Extractor ===\n");
    printf("This program extracts numbers from text.\n");
    yylex();
    return 0;
}

/* Required for standalone Lex program */
int yywrap() {
    return 1;
}