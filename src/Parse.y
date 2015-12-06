 /* Parser-Regeln */

%{
#include "Parse.y.hpp"
#include "Parse.l.hpp"
%}


 /* Klammern */
%token TERM_BRA
%token TERM_KET
%token FUNC_BRA
%token FUNC_KET


 /* Operatoren + Funktionen */
%token PLUS
%token MINUS
%token MULT
%token DIV
%token POW

%token EXP
%token SIN
%token COS
%token TAN


 /* Konstanten */
%token M_E
%token M_LOG2E
%token M_LOG10E
%token M_LN2
%token M_LN10
%token M_PI
%token M_PI_2
%token M_PI_4
%token M_1_PI
%token M_2_PI
%token M_2_SQRTPI
%token M_SQRT2
%token M_SQRT1_2


 /* Zahlen + Symbole */
%token NUM
%token SYM


%%


list: {}
    ;


%%



