#ifndef TERM_FUNCTION_H
#define TERM_FUNCTION_H

#include "Main.h"
#include "Term.h"


inline std::string function_derivate_inner(Term &fkt_arg, const Term &var);
inline std::string function_evaluate(Term &fkt_arg, const std::string &fkt_name, const Term &var, const Term &val);
inline std::string function_simplify(Term &fkt_arg, const std::string &fkt_name, double (*const fkt)(double));


class Function {
	public:
		virtual Term derivate(const Term &var) = 0;
		virtual Term evaluate(const Term &var, const Term &val) = 0;
		virtual Term simplify() = 0;

		const std::vector<Term *> &term() const { return m_args; }
		virtual const std::string &get_name() const = 0;

	protected:
		Function();
		std::vector<Term *> m_args;
};

class Plus : public Function {
	public:
		Plus(const std::vector<std::string> &term_str);

		static const std::string name;
		virtual inline const std::string &get_name() const { return name; }

		virtual Term derivate(const Term &var);
		virtual Term evaluate(const Term &var, const Term &val);
		virtual Term simplify();
};

class Minus : public Function {
	public:
		Minus(const std::vector<std::string> &term_str);

		static const std::string name;
		virtual inline const std::string &get_name() const { return name; }

		virtual Term derivate(const Term &var);
		virtual Term evaluate(const Term &var, const Term &val);
		virtual Term simplify();
};

class Mult : public Function {
	public:
		Mult(const std::vector<std::string> &term_str);

		static const std::string name;
		virtual inline const std::string &get_name() const { return name; }

		virtual Term derivate(const Term &var);
		virtual Term evaluate(const Term &var, const Term &val);
		virtual Term simplify();
};

class Div : public Function {
	public:
		Div(const std::vector<std::string> &term_str);

		static const std::string name;
		virtual inline const std::string &get_name() const { return name; }

		virtual Term derivate(const Term &var);
		virtual Term evaluate(const Term &var, const Term &val);
		virtual Term simplify();
};

class Pow : public Function {
	public:
		Pow(const std::vector<std::string> &term_str);

		static const std::string name;
		virtual inline const std::string &get_name() const { return name; }

		virtual Term derivate(const Term &var);
		virtual Term evaluate(const Term &var, const Term &val);
		virtual Term simplify();
};

class Exp : public Function {
	public:
		Exp(const std::string &term_str);

		static const std::string name;
		virtual inline const std::string &get_name() const { return name; }

		virtual Term derivate(const Term &var);
		virtual Term evaluate(const Term &var, const Term &val);
		virtual Term simplify();
};

class Sin : public Function {
	public:
		Sin(const std::string &term_str);

		static const std::string name;
		virtual inline const std::string &get_name() const { return name; }

		virtual Term derivate(const Term &var);
		virtual Term evaluate(const Term &var, const Term &val);
		virtual Term simplify();
};

class Cos : public Function {
	public:
		Cos(const std::string &term_str);

		static const std::string name;
		virtual inline const std::string &get_name() const { return name; }

		virtual Term derivate(const Term &var);
		virtual Term evaluate(const Term &var, const Term &val);
		virtual Term simplify();
};

class Tan : public Function {
	public:
		Tan(const std::string &term_str);

		static const std::string name;
		virtual inline const std::string &get_name() const { return name; }

		virtual Term derivate(const Term &var);
		virtual Term evaluate(const Term &var, const Term &val);
		virtual Term simplify();
};



//#undef acos
//#undef asin
//#undef atan
//#undef atan2
//#undef sinh
//#undef cosh
//#undef tanh
//#undef log
//#undef log10
//#undef sqrt



#endif
