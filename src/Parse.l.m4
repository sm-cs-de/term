 /* Parser-Token */


%option noyywrap nodefault yylineno c++

%{
#include "Parse.y.hpp"
%}

TXT ([a-zA-Z])
EXP ([Ee][-+]?[0-9]+)
DOT "."


%%


 /* Klammern */
"SYM_TERM_BRA()"    { return TERM_BRA;  }
"SYM_TERM_KET()"    { return TERM_KET;  }
"SYM_FUNC_BRA()"    { return FUNC_BRA;  }
"SYM_FUNC_KET()"    { return FUNC_KET;  }


 /* Operatoren + Funktionen */
"SYM_PLUS()"        { return PLUS;      }
"SYM_MINUS()"       { return MINUS;     }
"SYM_MULT()"        { return MULT;      }
"SYM_DIV()"         { return DIV;       }
"SYM_POW()"         { return POW;       }

"SYM_EXP()"         { return EXP;       }
"SYM_SIN()"         { return SIN;       }
"SYM_COS()"         { return COS;       }
"SYM_TAN()"         { return TAN;       }


 /* Konstanten */
"SYM_M_E()"         { return M_E;       }
"SYM_M_LOG2E()"     { return M_LOG2E;   }
"SYM_M_LOG10E()"    { return M_LOG10E;  }
"SYM_M_LN2()"       { return M_LN2;     }
"SYM_M_LN10()"      { return M_LN10;    }
"SYM_M_PI()"        { return M_PI;      }
"SYM_M_PI_2()"      { return M_PI_2;    }
"SYM_M_PI_4()"      { return M_PI_4;    }
"SYM_M_1_PI()"      { return M_1_PI;    }
"SYM_M_2_PI()"      { return M_2_PI;    }
"SYM_M_2_SQRTPI()"  { return M_2_SQRTPI;}
"SYM_M_SQRT2()"     { return M_SQRT2;   }
"SYM_M_SQRT1_2()"   { return M_SQRT1_2; }


 /* Zahlen + Symbole */
[0-9]+{DOT}[0-9]*{EXP}? | 
{DOT}?[0-9]+{EXP}?      { return NUM; }
({TXT}+[0-9]*)+         { return SYM; }


 /* Sonstige */
[ \t]   {}
.       { printf("Unknown character %s\n", *yytext); abort(); }


%%

