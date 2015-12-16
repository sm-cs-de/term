#include "Parse.hpp"
#include "Term.hpp"


using namespace std;


void yyerror(Ast **const ast, std::string error) {
   throw new Error(string("Ungültiger Ausdruck '") + COL_HLP + error + COL_CLR + "' !");
}


void str_alloc(char **const str, const char *const yytext) {
   *str = (char *) malloc(strlen(yytext)*sizeof(char));
   strcpy(*str, yytext);
}


void str_free(char **str) {
   if (str && *str) {
      free(*str);
      *str = NULL;
   }
}


Ast::Ast(const char *const op_str, Ast *const left, Ast *const right):
   m_type(FUNC), m_string(string(op_str)), m_num(0.0), m_arg(NULL), m_left(left), m_right(right) {}


Ast::Ast(const char *const func_str, Ast *const arg):
   m_type(FUNC), m_string(string(func_str)), m_num(0.0), m_arg(arg), m_left(NULL), m_right(NULL) {}


Ast::Ast(const char *const sym_str):
   m_type(SYM), m_string(string(sym_str)), m_num(0.0), m_arg(NULL), m_left(NULL), m_right(NULL) {}


Ast::Ast(const double num):
   m_type(NUM), m_string(""), m_num(num), m_arg(NULL), m_left(NULL), m_right(NULL) {}


string Ast::_string() const {
   if (m_type == NUM) {
      stringstream ss;
      ss << to_string(m_num);
      return ss.str();
   } else {
      return m_string;
   }
}


string Ast::print() const {
   stringstream ss;

   if (m_arg) {
      ss << _string() << function_bracket[0] << m_arg->print() << function_bracket[1];
   } else if (m_left && m_right) {
      ss << m_left->print() << _string() << m_right->print();
   } else {
      ss << _string();
   }

   return ss.str();
}


string Ast::print_tree() const {
   stringstream ss;

   const Ast *ast = NULL;
   queue<const Ast *> ast_func;
   queue<const Ast *> ast_args;
   ast_func.push(this);

   while (!ast_func.empty()) {
      ast = ast_func.front();
      ast_func.pop();

      if (ast->m_arg) {
         ast_args.push(ast->m_arg);
      }
      if (ast->m_left) {
         ast_args.push(ast->m_left);
      }
      if (ast->m_right) {
         ast_args.push(ast->m_right);
      }
      ss << ast->_string() << " ";
      if (ast_func.empty() && !ast_args.empty()) {
         ss << endl;
         swap(ast_func, ast_args);
      }
   }

   return ss.str();
}


void Term::create(const Ast *const ast) {
   if (!ast) {
      throw new Error("Kein Syntaxbaum übergeben !");
   }

   cout << ast->print_tree() << endl;

}
