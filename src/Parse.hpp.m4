#ifndef TERM_PARSE_HPP
#define TERM_PARSE_HPP

#include "Main.hpp"


 /* Klammern */
static const std::string term_bracket("SYM_TERM_BRA()SYM_TERM_KET()");
static const std::string function_bracket("SYM_FUNC_BRA()SYM_FUNC_KET()");


 /* Makro's für Funktionsnamen, damit nicht die *.cpp mit m4 übersetzt werden muss */
#define ADD_NAME     "SYM_ADD()"
#define SUB_NAME     "SYM_SUB()"
#define MUL_NAME     "SYM_MUL()"
#define DIV_NAME     "SYM_DIV()"
#define POW_NAME     "SYM_POW()"

#define EXP_NAME     "SYM_EXP()"
#define SIN_NAME     "SYM_SIN()"
#define COS_NAME     "SYM_COS()"
#define TAN_NAME     "SYM_TAN()"


 /* Konstanten */
static const std::map<std::string,double> constants = {
   {"SYM_M_E()",        M_E},
   {"SYM_M_LOG2E()",    M_LOG2E},
   {"SYM_M_LOG10E()",   M_LOG10E},
   {"SYM_M_LN2()",      M_LN2},
   {"SYM_M_LN10()",     M_LN10},
   {"SYM_M_PI()",       M_PI},
   {"SYM_M_PI_2()",     M_PI_2},
   {"SYM_M_PI_4()",     M_PI_4},
   {"SYM_M_1_PI()",     M_1_PI},
   {"SYM_M_2_PI()",     M_2_PI},
   {"SYM_M_2_SQRTPI()", M_2_SQRTPI},
   {"SYM_M_SQRT2()",    M_SQRT2},
   {"SYM_M_SQRT1_2()",  M_SQRT1_2}
};


 /* Abstrakter-Syntax-Baum (AST) */
class Ast {
   public:
      enum Type { OP, FUNC, SYM, NUM } m_type;

      Ast(const char *const op_str, Ast *const left, Ast *const right);  // Operatoren
      Ast(const char *const func_str, Ast *const arg);                   // Funktionen
      Ast(const char *const sym_str);                                    // Symbole + Konstanten
      Ast(const char *const num_str, const double num);                  // Zahlen

      static Ast *parse(const std::string &term_str);

      std::string _string() const;
      std::string print() const;
      std::string print_tree() const;

      std::string m_string;
      double m_num;

      class Ast *m_arg;
      class Ast *m_left;
      class Ast *m_right;
};


 /* Linker-Informationen und Hilfsfunktionen für Parser */
void yyerror(Ast **const ast, std::string error);           // Linker-Info; Erstes Argument nur, weil `%parse-param` in `Parse.y` dies so festlegt.
int  yyparse(Ast **const ast);                              // Linker-Info; Bzgl. Argument s.o.
void str_alloc(char **const str, const char *const yytext); // Allokator für `char *str`, siehe `Parse.y`.
void str_free(char **str);                                  // De-Allokator, s.o.


#endif
