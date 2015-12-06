#ifndef TERM_PARSE_HPP
#define TERM_PARSE_HPP

#include "Main.hpp"


/* Klammern */
static const std::string term_bracket("SYM_TERM_BRA()SYM_TERM_KET()");
static const std::string function_bracket("SYM_FUNC_BRA()SYM_FUNC_KET()");


/* Makro's für Funktionsnamen, damit nicht die *.cpp mit m4 übersetzt werden muss */
#define PLUS_NAME    "SYM_PLUS()"
#define MINUS_NAME   "SYM_MINUS()"
#define MULT_NAME    "SYM_MULT()"
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
      enum Type { FKT, SYM, NUM } m_type;

      Ast(std::string fkt_str, Ast *left, Ast *right);
      Ast(std::string sym_str);
      Ast(double num);

      std::string _string() const;
      void print() const;

      std::string m_string;
      double m_num;

      class Ast *m_left;
      class Ast *m_right;
};


#endif
