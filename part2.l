%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "part2.h"  /* Include this first */
#include "part2.tab.h"  /* Include the bison-generated header second */

/* This ensures proper token recognition and handling */
%}

%option noyywrap

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

"print" { return print; }
"scan" { return scan; }
"begin" { return begin; }
"end" {return end;}
"if" {return IF;}
"else" {return ELSE;}
"for" {return FOR;}
"while" {return WHILE;}
"inc" {return INC;}
"dec" {return DEC;}
"do" {return DO;}
"to" {return TO;}

("int"|"char") {
    yylval.str = strdup(yytext);  // Use strdup to allocate memory
    return type;
}
(int|char|while|for|main|program|VarDecl|inc|dec|do) {
    yylval.str = strdup(yytext);  // Use strdup to allocate memory
    return keyword;
}

[a-z]+ {
    yylval.str = strdup(yytext);  // Use strdup to allocate memory
    return ID;
}

[0-9]+ {
    yylval.num = atoi(yytext);
    return digit;
}

[<>] {
    yylval.chr = yytext[0];
    return rel_op;
}

":=" {return EQ;}

"(" {return OB;}

")" {return CB;}

";" {return SC;}

"," {return comma;}

":" {return colon;}

"{" {return OCB;}

"}" {return CCB;}

"[" {return OSB;}

"]" {return CSB;}


\"[^\"]*\" {
    /* Don't strip the quotes, keep them as part of the string */
    yylval.str = strdup(yytext);
    return STRING;
}

"@" {return AT;}

[-+*/%] {
    yylval.chr = yytext[0];
    return arith_op;
}

"'" { return single_quot; }

[ \t\n]+  { /* skip all whitespace & newlines */ }
.  {  }
%%