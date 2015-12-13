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
"SYM_TERM_BRA()"    { return _TERM_BRA;  }
"SYM_TERM_KET()"    { return _TERM_KET;  }
"SYM_FUNC_BRA()"    { return _FUNC_BRA;  }
"SYM_FUNC_KET()"    { return _FUNC_KET;  }


 /* Operatoren + Funktionen */
"SYM_PLUS()"        { str_alloc(&yylval.str,yytext); return _PLUS;  }
"SYM_MINUS()"       { str_alloc(&yylval.str,yytext); return _MINUS; }
"SYM_MULT()"        { str_alloc(&yylval.str,yytext); return _MULT;  }
"SYM_DIV()"         { str_alloc(&yylval.str,yytext); return _DIV;   }
"SYM_POW()"         { str_alloc(&yylval.str,yytext); return _POW;   }

"SYM_EXP()"         |
"SYM_SIN()"         |
"SYM_COS()"         |
"SYM_TAN()"         { str_alloc(&yylval.str,yytext); return _FUNC;  }


 /* Konstanten + Symbole*/
"SYM_M_E()"         |
"SYM_M_LOG2E()"     |
"SYM_M_LOG10E()"    |
"SYM_M_LN2()"       |
"SYM_M_LN10()"      |
"SYM_M_PI()"        |
"SYM_M_PI_2()"      |
"SYM_M_PI_4()"      |
"SYM_M_1_PI()"      |
"SYM_M_2_PI()"      |
"SYM_M_2_SQRTPI()"  |
"SYM_M_SQRT2()"     |
"SYM_M_SQRT1_2()"   |
({TXT}+[0-9]*)+     { yylval.ast = new Ast(yytext); return _SYM; }


 /* Zahlen */
[0-9]+{DOT}[0-9]*{EXP}? |
{DOT}?[0-9]+{EXP}?      { yylval.ast = new Ast(atof(yytext)); return _NUM; }



 /* Sonstige */
[ \t]   {}
.       { yyerror(NULL,yytext); }


%%

