#ifndef TERM_TERM_H
#define TERM_TERM_H

#include "Main.h"
#include "Function.h"
#include "Symbols.h"


class Term {
	public:
		Term(const std::string &term_str);

		Term derivate(const Term &var) const;
		Term evaluate(const Term &var, const Term &val) const;
		Term &simplify();

		inline const std::string &_string() const { return m_string; }
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
		bool parse_numeric();
		void parse_brackets(std::vector<long> &level, const std::string &bracket_type) const;
		bool parse_op_comb();
		void parse_subterms(std::vector<std::string> &subterms, std::vector<std::string> &ops, const std::vector<long> &level, const std::vector<long> &function_level) const;
		void create(const std::vector<std::string> &subterms, const std::vector<std::string> &ops);
		std::vector<std::string> get_multi_args(const std::vector<std::string> &subterms, const std::string &op_name) const;
};


#endif
