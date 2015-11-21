#ifndef TERM_LIB_H
#define TERM_LIB_H

#include "Main.h"
#include "Term.h"
#include "Function.h"


class Lib {
	public:
		Lib(Term *const term, const std::string &name = "", const bool delete_tmp = true);

		inline const std::string &print() const { return m_name; }
		inline const std::string &print_file() const { return m_name_file; }
		inline const std::vector<std::string> &print_tmp() const { return m_name_tmp; }

		void create();

	private:
		std::string m_name;
		std::string m_name_file;
		std::vector<std::string> m_name_tmp;

		Term *const m_term;
		std::vector<std::string> m_args;
		bool m_delete;

		void get_names();
		void get_args();
		std::ofstream *create_open(const std::string &file_name) const;
		void create_header(std::ofstream &ofs) const;
		void create_source(std::ofstream &ofs, const std::string &header_name) const;
		void create_evaluate(std::ofstream &ofs, const bool only_head = false) const;
		void create_derivate(std::ofstream &ofs, const bool only_head = false) const;
		void create_lib(const std::string file_out, const std::string source_name) const;
};

void replace_term_symbols(std::string *const expr);
void replace_substring(std::string *const str, const std::string what, const std::string with);

#endif
