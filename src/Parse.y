 /* Parser-Regeln */

%{
#include "Parse.hpp"
#include "Parse.y.hpp"
#include "Parse.l.hpp" // für yylex()
%}


%union {
   Ast *ast;
}

%type <ast> sum
//%type <ast> prod


 /* Klammern */
%token _TERM_BRA
%token _TERM_KET
%token _FUNC_BRA
%token _FUNC_KET


 /* Operatoren + Funktionen */
%token <ast> _PLUS
%token _MINUS
%token _MULT
%token _DIV
%token _POW

%token _EXP
%token _SIN
%token _COS
%token _TAN


 /* Konstanten */
%token _M_E
%token _M_LOG2E
%token _M_LOG10E
%token _M_LN2
%token _M_LN10
%token _M_PI
%token _M_PI_2
%token _M_PI_4
%token _M_1_PI
%token _M_2_PI
%token _M_2_SQRTPI
%token _M_SQRT2
%token _M_SQRT1_2


 /* Zahlen + Symbole */
%token<ast> _NUM
%token _SYM


%%


term:
    | term sum { $2->print(); }
    ;

sum : _NUM _PLUS _NUM { $$ = new Ast("+",$1,$2); }



%%

