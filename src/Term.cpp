#include "Term.hpp"


using namespace std;


Term::Term(const string &term_str) : Term(Ast::parse(term_str)) {}


Term::Term(const Ast *const ast) : m_string(""), m_fkt(NULL), m_is_primitive(false), m_is_numeric(false), m_num(0.0) {
   create(ast);
}


Term Term::derivate(const Term &var) const {
	stringstream ss;

	if (!var.is_primitive()) {
		ss << var._string();
		throw new Error(string("Kann nicht nach Term ") + COL_ERR + ss.str() + COL_CLR + " ableiten !");
	} else if (var.is_numeric()) {
		ss << var.num();
		throw new Error(string("Kann nicht nach Zahl ") + COL_ERR + ss.str() + COL_CLR + " ableiten !");
	}

	if (m_is_primitive) {
		if (m_string.compare(var._string()) == 0L) {
			ss << 1;
		} else {
			ss << 0;
		}
		return Term(ss.str());
	} else {
		return m_fkt->derivate(var);
	}
}


Term Term::evaluate(const Term &var, const Term &val) const {
	stringstream ss;

	if (!var.is_primitive()) {
		ss << var._string();
		throw new Error(string("Kann nicht Term ") + COL_ERR + ss.str() + COL_CLR + " mit Wert ersetzen !");
	} else if (var.is_numeric()) {
		ss << var.num();
		throw new Error(string("Kann nicht Zahl ") + COL_ERR + ss.str() + COL_CLR + " mit Wert ersetzen !");
	}

	if (m_is_primitive) {
		if (m_is_numeric) {
			ss << m_num;
		} else if (m_string.compare(var._string()) == 0L) {
			ss << val._string();
		} else {
			ss << m_string;
		}
		return Term(ss.str());
	} else {
		return m_fkt->evaluate(var, val);
	}
}


Term &Term::simplify() {
	string current = m_string;

	if (!m_is_primitive) {
		*this = m_fkt->simplify();
	}

	if (m_string.compare(current) != 0L) {
		simplify();
	}

	return *this;
}


void Term::create(const Ast *const ast) {
   if (!ast) { throw new Error(string("Kein AST übergeben !")); }

   m_string = ast->print();

   Ast::Type type = ast->m_type;
   if (type == Ast::OP) {
      string op_str = ast->m_string;
      Ast *left = ast->m_left;
      Ast *right = ast->m_right;

      if      (op_str == Add::name) { m_fkt = new Add(left, right); }
      else if (op_str == Sub::name) { m_fkt = new Sub(left, right); }
      else if (op_str == Mul::name) { m_fkt = new Mul(left, right); }
      else if (op_str == Div::name) { m_fkt = new Div(left, right); }
      else if (op_str == Pow::name) { m_fkt = new Pow(left, right); }
      // ...

   } else if (type == Ast::FUNC) {
      string func_str = ast->m_string;
      Ast *arg = ast->m_arg;

      if      (func_str == Exp::name) { m_fkt = new Exp(arg); }
      else if (func_str == Sin::name) { m_fkt = new Sin(arg); }
      else if (func_str == Cos::name) { m_fkt = new Cos(arg); }
      else if (func_str == Tan::name) { m_fkt = new Tan(arg); }
      // ...

   } else if (type == Ast::SYM) {
      m_is_primitive = true;

   } else if (type == Ast::NUM) {
      m_is_primitive = true;
      m_is_numeric = true;
      m_num = ast->m_num;
   }
}