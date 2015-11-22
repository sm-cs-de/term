#include "Lib.h"


using namespace std;


Lib::Lib(Term *const term, const string &name, const bool delete_tmp) : m_name(name), m_name_file(""), m_term(term), m_args() {
   if (!term) {
      throw new Error("Kein Term übergeben !");
   }

   string basename = get_names();
   get_args();

   create(basename, delete_tmp);
}

void Lib::create(const string &basename, const bool delete_tmp) {

   ofstream *ofs;
   string name_hpp = basename + ".hpp";
   string name_cpp = basename + ".cpp";

   ofs = create_open(name_hpp);
   create_header(*ofs);
   ofs->close();
   delete ofs;

   ofs = create_open(name_cpp);
   create_source(*ofs, name_hpp);
   ofs->close();
   delete ofs;

   create_lib(m_name_file, name_cpp);

   if (delete_tmp) {
      if (remove(name_hpp.c_str()) || remove(name_cpp.c_str())) {
         throw new Error("Konnte temporäre Deteien nicht löschen !");
      }
   }
}

string Lib::get_names() {
   if (!m_name.size()) {
      m_name = m_term->print();
      replace_substring(&m_name,"/",":");
   }

   string basename = "./lib_" + m_name;
   m_name_file = basename + ".so";

   return basename;
}

void Lib::get_args() {

   vector<Term *> terms;
   terms.push_back(m_term);

   unsigned long idx = 0;
   do {
      Term *term = terms[idx];
      if (term->is_primitive()) {
         if (!term->is_numeric() && (constants.find(term->print())==constants.end())) {
            m_args.push_back(term->print());
         }
      } else {
         for (unsigned long i=0; i<term->fkt()->term().size(); i++) {
            terms.push_back(term->fkt()->term()[i]);
         }
      }

      idx++;
   } while (idx < terms.size());

   sort(m_args.begin(), m_args.end());
   m_args.erase(unique(m_args.begin(), m_args.end()), m_args.end());    // doppelte Einträge raus
}

ofstream *Lib::create_open(const string &file_name) const {

   ofstream *ofs = new ofstream();
   if (!ofs) { throw new Error("Konnte ofstream nicht anlegen !"); }

   ofs->open(file_name.c_str(), ios::out);
   if (!ofs->is_open()) {
      throw new Error(string("Datei ") + file_name + "konnte nicht geöffnet werden !");
   }

   return ofs;
}

void Lib::create_header(ofstream &ofs) const {

   string allowed = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
   string guard = "TERM_LIB_" + m_name;

   unsigned long pos = guard.find_first_not_of(allowed);
   while (pos != string::npos) {
      guard[pos] = '_';
      pos = guard.find_first_not_of(allowed,pos+1);
   }

   ofs << "#ifndef " << guard << endl;
   ofs << "#define " << guard << endl;

   ofs << "#include <cstdlib>" << endl;
   ofs << "#include <cstdio>" << endl;
   ofs << "#include <cmath>" << endl;

   ofs << "#ifdef __cplusplus" << endl;
   ofs << "extern \"C\" {" << endl;
   ofs << "#endif" << endl;

   create_evaluate(ofs, true);
   create_derivate(ofs, true);

   ofs << "#ifdef __cplusplus" << endl;
   ofs << "}" << endl;
   ofs << "#endif" << endl;
   ofs << "#endif" << endl;
   ofs << flush;
}

void Lib::create_source(ofstream &ofs, const string &header_name) const {

   ofs << "#include \"" << header_name << "\"" << endl;
   create_evaluate(ofs);
   create_derivate(ofs);
   ofs << flush;
}

void Lib::create_evaluate(ofstream &ofs, const bool only_head) const {

   ofs << "double evaluate(";
   for (unsigned long i=0; i<m_args.size(); i++) {
      ofs << "double " << m_args[i];
      if (i < m_args.size()-1) {
         ofs << ",";
      }
   }
   ofs << ")";

   if (only_head) {
      ofs << ";" << endl;
      return;
   }
   ofs << "{" << endl;

   string expr = m_term->print();
   replace_term_symbols(&expr);
   ofs << "return (" << expr << ");" << endl;

   ofs << "}" << endl;
}

void Lib::create_derivate(ofstream &ofs, const bool only_head) const {

   ofs << "double derivate(";
   if (m_args.size() > 1L) {
      ofs << "unsigned long arg_num,";
   }
   for (unsigned long i=0; i<m_args.size(); i++) {
      if (i) {
         ofs << ",";
      }
      ofs << "double " << m_args[i];
   }
   ofs << ")";

   if (only_head) {
      ofs << ";" << endl;
      return;
   }
   ofs << "{" << endl;

   if (!m_args.size()) {         // falls Term nur Zahl ist
      ofs << "return 0.0;" << endl;
   } else {
      if (m_args.size() > 1L) {
         ofs << "switch (arg_num) {" << endl;
      }
      string expr;
      for (unsigned long i=0; i<m_args.size(); i++) {
         expr = m_term->derivate(Term(m_args[i])).print();
         replace_term_symbols(&expr);

         if (m_args.size() > 1L) {
            ofs << "case " << i << ": ";
         }
         ofs << "return (" << expr << ");" << endl;
      }
      if (m_args.size() > 1L) {
         ofs << "default: exit(EXIT_FAILURE);" << endl;
         ofs << "}" << endl;
      }
   }

   ofs << "}" << endl;
}

void Lib::create_lib(const string file_out, const string source_name) const {

   string cmd = string("g++ \"") + source_name + "\" -o \"" + file_out + "\" -fPIC -lm -shared -O3 -march=native -s";

   if (system(cmd.c_str())) {
      throw new Error("Fehler beim Kompilieren !");
   }
}

void replace_term_symbols(string *const expr) {
   if (!expr) {
      throw new Error("Kein String !");
   }

   for (auto it=constants.begin(); it!=constants.end(); ++it) {
      replace_substring(expr, it->first, it->second);
   }

   replace_substring(expr, function_bracket.substr(0,1), "(");
   replace_substring(expr, function_bracket.substr(1,1), ")");
   replace_substring(expr, Exp::name, "exp");
   replace_substring(expr, Sin::name, "sin");
   replace_substring(expr, Cos::name, "cos");
   replace_substring(expr, Tan::name, "tan");
   //TODO: funktionen aufnehmen
}

void replace_substring(string *const str, const string what, const string with) {
   if (!str) {
      throw new Error("Kein String !");
   }

   unsigned long pos = str->find(what);
   while (pos != string::npos) {
      str->replace(pos, what.length(), with);
      pos = str->find(what, pos+1);
   }
}



