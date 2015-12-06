#ifndef TERM_LIB_HPP
#define TERM_LIB_HPP

#include "Main.hpp"
#include "Term.hpp"
#include "Function.hpp"
#include "Symbols.hpp"


class Lib {
	public:
		Lib(Term *const term, const std::string &name = "", const bool delete_tmp = true);

		inline const std::string &name() const { return m_name; }
		inline const std::string &name_file() const { return m_name_file; }

	private:
		std::string m_name;                    /* Name der Lib */
		std::string m_name_file;               /* Name der Datei *.so */

		Term *const m_term;
		std::vector<std::string> m_args;       /* Term Variablen */

		std::string get_names();
		void get_args();

		void create(const std::string &basename, const bool delete_tmp);
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
