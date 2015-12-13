 /* Parser-Regeln */

%{
#include "Parse.hpp"
#include "Parse.y.hpp"
#include "Parse.l.hpp" // für yylex()
%}

%parse-param {Ast **const ast}


%union {
   Ast *ast;
   char *str;
}

%type<ast> sum
%type<ast> prod
%type<ast> fct


 /* Klammern */
%token TERM_BRA
%token TERM_KET
%token FUNC_BRA
%token FUNC_KET

 /* Operatoren + Funktionen */
%token<str> PLUS
%token<str> MINUS
%token<str> MULT
%token<str> DIV
%token<str> POW

%token<str> FUNC

 /* Konstanten + Symbole */
%token<ast> SYM

 /* Zahlen */
%token<ast> NUM


%%


term:
    | TERM_BRA term TERM_KET
    | sum { *ast = $1; }
    ;

sum : prod
    | prod PLUS  prod { $$ = new Ast($2,$1,$3); }
    | prod MINUS prod { $$ = new Ast($2,$1,$3); }
    ;

prod: fct
    | fct MULT fct { $$ = new Ast($2,$1,$3); }
    | fct DIV  fct { $$ = new Ast($2,$1,$3); }
    ;

fct : NUM
    | SYM
    | FUNC FUNC_BRA sum FUNC_KET { $$ = new Ast($1,$3); }
    ;



%%

