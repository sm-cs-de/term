 /* Parser-Regeln */

%{
#include "Parse.hpp"
#include "Parse.y.hpp"
#include "Parse.l.hpp" // für yylex()
%}


%union {
   Ast *ast;
   char *str;
}

%type<ast> sum
%type<ast> prod
%type<ast> fct


 /* Klammern */
%token _TERM_BRA
%token _TERM_KET
%token _FUNC_BRA
%token _FUNC_KET

 /* Operatoren + Funktionen */
%token<str> _PLUS
%token<str> _MINUS
%token<str> _MULT
%token<str> _DIV
%token<str> _POW

%token<str> _FUNC

 /* Konstanten + Symbole */
%token<ast> _SYM

 /* Zahlen */
%token<ast> _NUM


%%


term:
    | _TERM_BRA term _TERM_KET
    | sum { $1->print(); }
    ;

sum : prod
    | prod _PLUS  prod { $$ = new Ast($2,$1,$3); }
    | prod _MINUS prod { $$ = new Ast($2,$1,$3); }
    ;

prod: fct
    | fct _MULT fct { $$ = new Ast($2,$1,$3); }
    | fct _DIV  fct { $$ = new Ast($2,$1,$3); }
    ;

fct : _NUM
    | _SYM
    | _FUNC _FUNC_BRA sum _FUNC_KET { $$ = new Ast($1,$3); }
    ;



%%

