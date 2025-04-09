%{
#include "y.tab.h"
%}

%%
i     return I;
b     return B;
t     return T;
e     return E;
a     return A;
[ \t\n]+ ;
.     return -1;
%%

int yywrap() {
    return 1;
}
