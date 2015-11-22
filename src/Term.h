#ifndef TERM_TERM_H
#define TERM_TERM_H

#include "Main.h"
#include "Function.h"


static const std::string term_bracket("()");										// Einstellig jeweils
static const std::string function_bracket("[]");								// Einstellig jeweils
static const std::string subterm_replace("|");									// Einstellig
static const std::string bad_keywords(subterm_replace + "{}\\=\'\",:;<>");


class Term {
	public:
		Term(const std::string &term_str);

		Term derivate(const Term &var) const;
		Term evaluate(const Term &var, const Term &val) const;
		Term &simplify();

		inline const std::string &print() const { return m_string; }
		inline const Function *fkt() const { return m_fkt; }
		inline bool is_primitive() const { return m_is_primitive; }
		inline bool is_numeric() const { return m_is_numeric; }
		inline double num() const { return m_num; }

	private:
		Function *m_fkt;
		bool m_is_primitive;

		std::string m_string;
		bool m_is_numeric;
		double m_num;

		void parse();
		void parse_initial();
		bool parse_numeric(const bool is_numeric = false);
		void parse_brackets(std::vector<long> &level, const std::string &bracket_type) const;
		bool parse_pm();
		void parse_subterms(std::vector<std::string> &subterms, std::vector<std::string> &ops, const std::vector<long> &level, const std::vector<long> &function_level) const;
		void create(const std::vector<std::string> &subterms, const std::vector<std::string> &ops);
		std::vector<std::string> get_multi_args(const std::vector<std::string> &subterms, const std::string &op_name) const;
		bool parse_constants();
};


static const std::map<std::string,std::string> constants = {
	{"c_e",        "M_E"},
	{"c_lbe",      "M_LOG2E"},
	{"c_lge",      "M_LOG10E"},
	{"c_ln2",      "M_LN2"},
	{"c_ln10",     "M_LN10"},
	{"c_pi",       "M_PI"},
	{"c_pi_2",     "M_PI_2"},
	{"c_pi_4",     "M_PI_4"},
	{"c_1_pi",     "M_1_PI"},
	{"c_2_pi",     "M_2_PI"},
	{"c_2_sqrtpi", "M_2_SQRTPI"},
	{"c_sqrt2",    "M_SQRT2"},
	{"c_1_sqrt2",  "M_SQRT1_2"}
};


#endif
