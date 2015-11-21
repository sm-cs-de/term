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


static const std::string const_e("e");
static const std::string const_log2e("lbe");
static const std::string const_log10e("lge");
static const std::string const_ln2("ln2");
static const std::string const_ln10("ln10");
static const std::string const_pi("pi");
static const std::string const_pi_2("pi_2");
static const std::string const_pi_4("pi_4");
static const std::string const_1_pi("1_pi");
static const std::string const_2_pi("2_pi");
static const std::string const_2_sqrtpi("2_sqrtpi");
static const std::string const_sqrt2("sqrt2");
static const std::string const_sqrt1_2("1_sqrt2");

#endif