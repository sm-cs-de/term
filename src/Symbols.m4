divert(-1)
changecom(//)

// Hinzufügen eines neuen Symbols / Operators / Funktionsnamens:
// 1. Hier ein geeignetes m4-Macro mit entsprechendem Text definieren.
// 2. Mit diesem Macro in `Parse.hpp.m4` einen entsprechenden Namen bzw. eine Konstante definieren.
// 3. Mit diesem Macro in `Parse.l.m4` ein entsprechendes Token anlegen.
// 4. Das Token in `Parse.y` aufführen und die Gramatik festlegen.
// 5. Anschließend das Funktions"= oder Operator"=Klassenobjekt deklarieren bzw. das Symbol entsprechend verwenden.

// Build-Ablauf:
// - Hier werden die Symbole für den Lexer und den Code definiert.
// - m4 erzeugt dann das entsprechende Lexer-File (`Parse.l`) und den Code-Header (`Parse.hpp`).
// - Aus `Parse.l` und `Parse.y` wird dann der Parser erzeugt.
// - `Parse.hpp` dient zur Verwendung im Code.
// - Die Erzeugung des Parser-Code's wird durch das Makefile gesteuert.


define(`SYM_TERM_BRA',  `(')dnl
define(`SYM_TERM_KET',  `)')dnl
define(`SYM_FUNC_BRA',  `[')dnl
define(`SYM_FUNC_KET',  `]')dnl

define(`SYM_ADD',       `+')dnl
define(`SYM_SUB',       `-')dnl
define(`SYM_MUL',       `*')dnl
define(`SYM_DIV',       `/')dnl
define(`SYM_POW',       `^')dnl

define(`SYM_EXP',       `Exp')dnl
define(`SYM_SIN',       `Sin')dnl
define(`SYM_COS',       `Cos')dnl
define(`SYM_TAN',       `Tan')dnl

define(`SYM_M_E',       `E')dnl
define(`SYM_M_LOG2E',   `LOG2E')dnl
define(`SYM_M_LOG10E',  `LOG10E')dnl
define(`SYM_M_LN2',     `LN2')dnl
define(`SYM_M_LN10',    `LN10')dnl
define(`SYM_M_PI',      `PI')dnl
define(`SYM_M_PI_2',    `PI_2')dnl
define(`SYM_M_PI_4',    `PI_4')dnl
define(`SYM_M_1_PI',    `1_PI')dnl
define(`SYM_M_2_PI',    `2_PI')dnl
define(`SYM_M_2_SQRTPI',`2_SQRTPI')dnl
define(`SYM_M_SQRT2',   `SQRT2')dnl
define(`SYM_M_SQRT1_2', `SQRT1_2')dnl


divert(1)
