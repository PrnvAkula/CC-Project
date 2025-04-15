%{
#include <string.h>
#include "part2.tab.h"
%}

%%
"begin program:" {
    return begin_prog;
}

"end program" {
    return end_prog;
}

"begin VarDecl:" {
    return begin_vardecl;
}

"end VarDecl" {
    return end_vardecl;
}

(int|char|if|else|while|for|main|begin|end|print|scan|program|VarDecl|inc|dec|do) {
    yylval.str = strdup(yytext);  // Use strdup to allocate memory
    return keyword;
}

[a-z]+ {
    yylval.str = strdup(yytext);  // Use strdup to allocate memory
    return name;
}

[0-9]+ {
    yylval.num = atoi(yytext);
    return digit;
}

[<>] {
    yylval.chr = yytext[0];
    return rel_op;
}

"=" {return EQ;}

"(" {return OB;}

")" {return CB;}

";" {return SC;}

"," {return comma;}

":" {return colon;}

"{" {return OCB;}

"}" {return CCB;}

"[" {return OSB;}

"]" {return CSB;}

\" {return double_quot;}

"@" {return AT;}

[-+*/%] {
    yylval.chr = yytext[0];
    return arith_op;
}

"'" { return single_quot; }
\n  { return NL; }
[ \t]+ {  }
.  {  }

%%