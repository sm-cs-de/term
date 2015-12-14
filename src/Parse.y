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

%type<ast> expr


 /* Klammern */
%token TERM_BRA
%token TERM_KET
%token FUNC_BRA
%token FUNC_KET

 /* Operatoren + Funktionen */
%left<str>  ADD SUB
%left<str>  MUL DIV
%right<str> POW
//%nonassoc

%token<str> FUNC

 /* Konstanten + Symbole */
%token<ast> SYM

 /* Zahlen */
%token<ast> NUM


%%


term: %empty
    | expr { *ast = $1; }
    ;


expr: TERM_BRA expr TERM_KET      { $$ = $2; }
    | expr ADD expr               { $$ = new Ast($2,$1,$3); }
    | expr SUB expr               { $$ = new Ast($2,$1,$3); }
    | expr MUL expr               { $$ = new Ast($2,$1,$3); }
    | expr DIV expr               { $$ = new Ast($2,$1,$3); }
    | expr POW expr               { $$ = new Ast($2,$1,$3); }
    | FUNC FUNC_BRA expr FUNC_KET { $$ = new Ast($1,$3);    }
    | SYM
    | NUM
//    | OP_UN expr {}
    ;


%%

