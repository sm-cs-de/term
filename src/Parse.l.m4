 /* Parser-Token */


%option noyywrap nodefault align debug

%{
#include "Parse.hpp"
#include "Parse.y.hpp"
%}

TXT ([a-zA-Z])
EXP ([Ee][-+]?[0-9]+)
DOT "."


%%


 /* Klammern */
"SYM_TERM_BRA()"   { return TERM_BRA; }
"SYM_TERM_KET()"   { return TERM_KET; }
"SYM_FUNC_BRA()"   { return FUNC_BRA; }
"SYM_FUNC_KET()"   { return FUNC_KET; }


 /* Operatoren + Funktionen */
"SYM_ADD()"        { str_alloc(&yylval.str,yytext); return ADD; }
"SYM_SUB()"        { str_alloc(&yylval.str,yytext); return SUB; }
"SYM_MUL()"        { str_alloc(&yylval.str,yytext); return MUL; }
"SYM_DIV()"        { str_alloc(&yylval.str,yytext); return DIV; }
"SYM_POW()"        { str_alloc(&yylval.str,yytext); return POW; }

"SYM_EXP()"        |
"SYM_SIN()"        |
"SYM_COS()"        |
"SYM_TAN()"        { str_alloc(&yylval.str,yytext); return FUNC; }


 /* Konstanten + Symbole*/
"SYM_M_E()"        |
"SYM_M_LOG2E()"    |
"SYM_M_LOG10E()"   |
"SYM_M_LN2()"      |
"SYM_M_LN10()"     |
"SYM_M_PI()"       |
"SYM_M_PI_2()"     |
"SYM_M_PI_4()"     |
"SYM_M_1_PI()"     |
"SYM_M_2_PI()"     |
"SYM_M_2_SQRTPI()" |
"SYM_M_SQRT2()"    |
"SYM_M_SQRT1_2()"  |
({TXT}+[0-9]*)+    { str_alloc(&yylval.str,yytext); return SYM; }


 /* Zahlen */
[0-9]+{DOT}[0-9]*{EXP}? |
{DOT}?[0-9]+{EXP}?      { str_alloc(&yylval.str,yytext); return NUM; }



 /* Sonstige */
[ \t]   {}
.       { yyerror(NULL,yytext); }


%%

