 /* Parser-Token */


%option noyywrap nodefault yylineno c++

%{
#include "Main.hpp"
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
"SYM_PLUS()"        { return _PLUS;      }
"SYM_MINUS()"       { return _MINUS;     }
"SYM_MULT()"        { return _MULT;      }
"SYM_DIV()"         { return _DIV;       }
"SYM_POW()"         { return _POW;       }

"SYM_EXP()"         { return _EXP;       }
"SYM_SIN()"         { return _SIN;       }
"SYM_COS()"         { return _COS;       }
"SYM_TAN()"         { return _TAN;       }


 /* Konstanten */
"SYM_M_E()"         { return _M_E;       }
"SYM_M_LOG2E()"     { return _M_LOG2E;   }
"SYM_M_LOG10E()"    { return _M_LOG10E;  }
"SYM_M_LN2()"       { return _M_LN2;     }
"SYM_M_LN10()"      { return _M_LN10;    }
"SYM_M_PI()"        { return _M_PI;      }
"SYM_M_PI_2()"      { return _M_PI_2;    }
"SYM_M_PI_4()"      { return _M_PI_4;    }
"SYM_M_1_PI()"      { return _M_1_PI;    }
"SYM_M_2_PI()"      { return _M_2_PI;    }
"SYM_M_2_SQRTPI()"  { return _M_2_SQRTPI;}
"SYM_M_SQRT2()"     { return _M_SQRT2;   }
"SYM_M_SQRT1_2()"   { return _M_SQRT1_2; }


 /* Zahlen + Symbole */
[0-9]+{DOT}[0-9]*{EXP}? |
{DOT}?[0-9]+{EXP}?      { return _NUM; }
({TXT}+[0-9]*)+         { return _SYM; }


 /* Sonstige */
[ \t]   {}
.       { throw new Error(std::string(*yytext)); }


%%

