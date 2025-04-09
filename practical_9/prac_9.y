%{
    #include <cstdio>
    #include <cstdlib>
    int yylex(void);
    void yyerror(const char *s);
    %}
    
    %token I B T E A
    
    %%
    S : I E T S S1
      | A
      ;
    
    S1 : E S
       | /* empty */
       ;
    
    E : B
      ;
    %%
    
    void yyerror(const char *s) {
        printf("Invalid string\n");
        exit(0);
    }
    
    int main() {
        if (!yyparse())
            printf("Valid string\n");
        return 0;
    }
    