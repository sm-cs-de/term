#include "Term.h"


using namespace std;


Term::Term(const string &term_str) : m_fkt(NULL), m_is_primitive(false), m_string(term_str), m_is_numeric(false), m_num(0.0) {
	parse();
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
