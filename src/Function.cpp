#include "Function.hpp"


using namespace std;


string function_derivate_inner(Term &fkt_arg, const Term &var) {
	stringstream deriv_ss;

	Term deriv_arg = fkt_arg.derivate(var);
	if (deriv_arg.is_primitive() || (deriv_arg.fkt()->args().size() == 1L)) {
		deriv_ss << Mul::name << deriv_arg._string();
	} else {
		deriv_ss << Mul::name << term_bracket[0] << deriv_arg._string() << term_bracket[1];
	}

	return deriv_ss.str();
}

string function_evaluate(Term &fkt_arg, const string &fkt_name, const Term &var, const Term &val) {
	stringstream eval_ss;

	eval_ss << fkt_name << function_bracket[0] << fkt_arg.evaluate(var,val)._string() << function_bracket[1];

	return eval_ss.str();
}

string function_simplify(Term &fkt_arg, const string &fkt_name, double (*const fkt)(double)) {
	stringstream simpl_ss;

	if (fkt_arg.is_primitive()) {
		if (fkt_arg.is_numeric()) {
			simpl_ss << fkt(fkt_arg.num());
		} else {
			simpl_ss << fkt_name << function_bracket[0] << fkt_arg._string() << function_bracket[1];
		}
	} else {
		simpl_ss << fkt_name << function_bracket[0] << fkt_arg.simplify()._string() << function_bracket[1];
	}

	return simpl_ss.str();
}


/** Function */
Function::Function(const vector<string> &term_str) {
   for (unsigned long i=0; i<term_str.size(); i++) {
      m_args.push_back(new Term(term_str[i]));
   }
}

Function::Function(const string &term_str) {
   m_args.push_back(new Term(term_str));
}

Function::Function(const Ast *const arg) {
   m_args.push_back(new Term(arg));
}

Function::Function(const Ast *const left, const Ast *const right) {
   m_args.push_back(new Term(left));
   m_args.push_back(new Term(right));
}


/** Exp */
Exp::Exp(const string &term_str) : Function(term_str) {}

Exp::Exp(const Ast *const arg) : Function(arg) {}

const string Exp::name(EXP_NAME);

Term Exp::derivate(const Term &var) {
	stringstream deriv_ss;

	deriv_ss << Exp::name << function_bracket[0] << m_args[0]->_string() << function_bracket[1];
	deriv_ss << function_derivate_inner(*m_args[0], var);

	return Term(deriv_ss.str());
}

Term Exp::evaluate(const Term &var, const Term &val) {
	return Term(function_evaluate(*m_args[0],Exp::name,var,val)).simplify();
}

Term Exp::simplify() {
	return Term(function_simplify(*m_args[0],Exp::name,&exp));
}


/** Sin */
Sin::Sin(const string &term_str) : Function(term_str) {}

Sin::Sin(const Ast *const arg) : Function(arg) {}

const string Sin::name(SIN_NAME);

Term Sin::derivate(const Term &var) {
	stringstream deriv_ss;

	deriv_ss << Cos::name << function_bracket[0] << m_args[0]->_string() << function_bracket[1];
	deriv_ss << function_derivate_inner(*m_args[0], var);

	return Term(deriv_ss.str());
}

Term Sin::evaluate(const Term &var, const Term &val) {
	return Term(function_evaluate(*m_args[0],Sin::name,var,val)).simplify();
}

Term Sin::simplify() {
	return Term(function_simplify(*m_args[0],Sin::name,&sin));
}


/** Cos */
Cos::Cos(const string &term_str) :Function(term_str) {}

Cos::Cos(const Ast *const arg) : Function(arg) {}

const string Cos::name(COS_NAME);

Term Cos::derivate(const Term &var) {
	stringstream deriv_ss;

	deriv_ss << Sub::name << Sin::name << function_bracket[0] << m_args[0]->_string() << function_bracket[1];
	deriv_ss << function_derivate_inner(*m_args[0], var);

	return Term(deriv_ss.str());
}

Term Cos::evaluate(const Term &var, const Term &val) {
	return Term(function_evaluate(*m_args[0],Cos::name,var,val)).simplify();
}

Term Cos::simplify() {
	return Term(function_simplify(*m_args[0],Cos::name,&cos));
}


/** Tan */
Tan::Tan(const string &term_str) : Function(term_str) {}

Tan::Tan(const Ast *const arg) : Function(arg) {}

const string Tan::name(TAN_NAME);

Term Tan::derivate(const Term &var) {
	stringstream deriv_ss;

	deriv_ss << term_bracket[0] << "1" << Add::name << Tan::name << function_bracket[0] << m_args[0]->_string() << function_bracket[1] << Pow::name << "2" << term_bracket[1];
	deriv_ss << function_derivate_inner(*m_args[0], var);

	return Term(deriv_ss.str());
}

Term Tan::evaluate(const Term &var, const Term &val) {
	return Term(function_evaluate(*m_args[0],Tan::name,var,val)).simplify();
}

Term Tan::simplify() {
	return Term(function_simplify(*m_args[0],Tan::name,&tan));
}