#include "Parse.hpp"


using namespace std;


void yyerror(std::string error) {
   throw new Error(string("Ungültiger Ausdruck '") + COL_HLP + error + COL_CLR + "' !");
}


Ast::Ast(const char *const op_str, Ast *const left, Ast *const right):
   m_type(FCT), m_string(string(op_str)), m_num(0.0), m_arg(NULL), m_left(left), m_right(right) {}


Ast::Ast(const char *const fct_str, Ast *const arg):
   m_type(FCT), m_string(string(fct_str)), m_num(0.0), m_arg(arg), m_left(NULL), m_right(NULL) {}


Ast::Ast(const char *const sym_str):
   m_type(SYM), m_string(string(sym_str)), m_num(0.0), m_arg(NULL), m_left(NULL), m_right(NULL) {}


Ast::Ast(const double num):
   m_type(NUM), m_string(""), m_num(num), m_arg(NULL), m_left(NULL), m_right(NULL) {}


string Ast::_string() const {
   stringstream ss;
   ss << (m_type==NUM ? to_string(m_num) : m_string);

   return ss.str();
}


void Ast::print() const {
   cout << _string() << endl;

   if (m_arg) {
      m_arg->print();
   }
   if (m_left) {
      m_left->print();
   }
   if (m_right) {
      m_right->print();
   }
}


void Term::parse() {	cout << "» " << m_string << endl;

	/* Leer oder ungültige Zeichen, Leerzeichen entfernen */
	parse_initial();

	/* Numerisch ? */
	if (parse_numeric()) {
      m_is_primitive = true;
		return;
	}

	/* Term-Klammersetzung */
	vector<long> level;
	parse_brackets(level, term_bracket);

	/* Absolut-Klammern feststellen und ggf. entfernen */
	if (level.front() && level.back()) {
		bool inner = true;
		for (unsigned long i=1; i<m_string.size()-1; i++) {
			if (!level[i]) {
				inner = false;
				break;
			}
		}
		if (inner) {
			m_string = m_string.substr(1, m_string.length()-2);
			parse();
			return;
		}
	}

	/* Minus am Anfang fixen für interne Rechnungen */
	if ((m_string.size() >= Minus::name.size()) && (m_string.substr(0,Minus::name.size()).compare(Minus::name) == 0L)) {
		m_string = string("0") + m_string;
		parse();
		return;
	}

	/* Gültige Operator-Abfolgen ersetzen */
	if (parse_op_comb()) {
		parse();
		return;
	}

	/* Funktions-Klammersetzung */
	vector<long> function_level;
	parse_brackets(function_level, function_bracket);

	/* Funktions-Klammern in Term-Klammern und andersrum ent-markieren */
	bool in_subterm = false;
	for (unsigned long i=0; i<level.size(); i++) {
		if (level[i]) {
			if (function_level[i] && !in_subterm) {
				in_subterm = false;
			} else {
				in_subterm = true;
			}
		} else {
			in_subterm = false;
		}
		if (function_level[i]) {
			if (in_subterm) {
				function_level[i] = 0;
			} else {
				level[i] = 0;
			}
		}
	}

	/* Subterme und Operator-Verbindungen herausfinden und Operator-Setzung prüfen */
	vector<string> subterms, ops;
	parse_subterms(subterms, ops, level, function_level);

	/* Term bzw. Funktionen anlegen */
	create(subterms, ops);
}

void Term::parse_initial() {
	if (!m_string.size()) {
		throw new Error("Leerer Term !");
	}  else {
//		size_t bad_pos = m_string.find_first_of(bad_keywords);
//		if (bad_pos != string::npos) {
//			throw new Error(string("Ungültiges Zeichen ") + COL_ERR + m_string[bad_pos] + COL_CLR + " im Ausdruck !");
//		}
	}

	stringstream ss;
	for (unsigned int i=0; i<m_string.size(); i++) {
		if (m_string[i] != ' ') {
			ss << m_string[i];
		}
	}
	m_string = ss.str();
}

bool Term::parse_numeric() {

   auto it = constants.find(m_string);
   if (it != constants.end()) {
      m_num = it->second;

   } else {
      string digits = "0123456789";

      size_t pos = m_string.find_first_not_of(string("-.") + digits);
      if (pos != string::npos) {
         return false;
      } else {
         pos = m_string.find(Minus::name);
         if (pos && (pos !=string::npos)) {
            return false;
         }
         pos = m_string.find(".");
         if ((pos != string::npos) && (m_string.find(".",pos+1) != string::npos)) {
            return false;
         }
      }

      stringstream ss(m_string);
      ss >> m_num;
   }
	m_is_numeric = true;

	return true;
}

void Term::parse_brackets(vector<long> &level, const string &bracket_type) const {
	long lev = 0, bra_count = 0, ket_count = 0;

	for (unsigned long i=0; i<m_string.size(); i++) {
		if (m_string[i] == bracket_type[0]) {
			bra_count++;
			lev++;
		}
		level.push_back(lev);
		if (m_string[i] == bracket_type[1]) {
			ket_count++;
			lev--;
		}
	}

	/* Es reicht festzustellen, dass man in Klammer ist (Level 1) */
	for (unsigned long i=0; i<m_string.size(); i++) {
		if (level[i] > 1L) {
			level[i] = 1;
		}
	}

	/* Test auf unterschiedliche Klammeranzahl */
	stringstream ss;
	if (bra_count != ket_count) {
		ss << abs(long(bra_count-ket_count));
		throw new Error(string("Es fehlen ") + COL_HLP + ss.str() + COL_CLR + " Klammern !");
	}

	/* Test auf leere und operatorlose Klammern (vor ungültiger Klammersetzung durchführen) */
	for (unsigned long i=0; i<m_string.size()-1; i++) {
		if ((m_string[i] == bracket_type[0]) && (m_string[i+1] == bracket_type[1])) {
			ss << i;
			throw new Error(string("Leere Klammer nach Position ") + COL_HLP + ss.str() + COL_CLR + " !");
		} else if ((m_string[i] == bracket_type[1]) && (m_string[i+1] == bracket_type[0])) {
			ss << i;
			throw new Error(string("Fehlender Operator zwischen Klammern nach Position ") + COL_HLP + ss.str() + COL_CLR + " !");
		}
	}

	/* Test auf ungültige Klammersetzung */
	for (unsigned long i=0; i<m_string.size(); i++) {
		if (level[i] < 0) {
			ss << i;
			throw new Error(string("Ungültige Klammersetzung vor Position ") + COL_HLP + ss.str() + COL_CLR + " !");
		}
	}
}

bool Term::parse_op_comb() {
   vector<string> variants = { // TODO: andere Kombinationen mit aufnehmen
      Plus::name+Plus::name, Plus::name+Minus::name, Minus::name+Plus::name, Minus::name+Minus::name
   };
   vector<string> equivalent = {
      Plus::name, Minus::name, Minus::name, Plus::name
   };

   size_t pos, len = m_string.size();
   while (true) {
      for (unsigned long i=0; i<variants.size(); i++) {
         pos = m_string.find(variants[i]);
         if (pos != string::npos) {
            m_string = m_string.replace(pos, variants[i].size(), equivalent[i]);
            break;
         }
      }
      if (pos == string::npos) {
         return (len != m_string.size());
      }
   }
}

void Term::parse_subterms(vector<string> &subterms, vector<string> &ops, const vector<long> &level, const vector<long> &function_level) const {
	stringstream ss;
	vector<unsigned long> subterm_positions;

	/* String durchwandern und Operanden/Funktionen/Klammer-Subterme suchen */
	unsigned long idx = 0, idx2, name_size, last_op_idx = 0;
	bool in_operator = false, in_function = false;
	while (idx < m_string.size()) {

		/* In Klammer-Subterm ? */
		if (level[idx]) {
			for(idx2=idx; idx2<m_string.size(); idx2++) {
				if (!level[idx2]) { break; }
			}
			idx2--;
			if (idx && (!subterm_positions.size() || (subterm_positions.back() != idx-2))) { ss << idx; throw new Error(string("Fehlender Operator vor Term an Position ") + COL_HLP + ss.str() + COL_CLR + " !"); }
			subterm_positions.push_back(idx);
			subterm_positions.push_back(idx2);
			idx = idx2 + 1;
			continue;
		}

		/* In Funktions-Subterm ? */
		if (function_level[idx]) {
			if (name_size = Exp::name.size(), (idx >= name_size) && (m_string.substr(idx-name_size,name_size).compare(Exp::name) == 0L)) {
				in_function = true;
				ops.push_back(Exp::name);
			} else if (name_size = Sin::name.size(), (idx >= name_size) && (m_string.substr(idx-name_size,name_size).compare(Sin::name) == 0L)) {
				in_function = true;
				ops.push_back(Sin::name);
			} else if (name_size = Cos::name.size(), (idx >= name_size) && (m_string.substr(idx-name_size,name_size).compare(Cos::name) == 0L)) {
				in_function = true;
				ops.push_back(Cos::name);
			} else if (name_size = Tan::name.size(), (idx >= name_size) && (m_string.substr(idx-name_size,name_size).compare(Tan::name) == 0L)) {
				in_function = true;
				ops.push_back(Tan::name);
			//TODO: funktionen aufnehmen
			}
			if (in_function) {
				in_function = false;
				for(idx2=idx; idx2<m_string.size(); idx2++) {
					if (!function_level[idx2]) { break; }
				}
				idx2--;
				if ((idx-name_size == 0L) && (idx2 == m_string.size()-1)) {
					subterm_positions.push_back(idx+1);
					subterm_positions.push_back(idx2-1);
					break;
				} else {
					if ((idx-name_size > 0L) && (!subterm_positions.size() || (subterm_positions.back() != last_op_idx-1))) { ss << idx; throw new Error(string("Unbekannte Funktion an Stelle ") + COL_HLP + ss.str() + COL_CLR + " !"); }
					subterm_positions.push_back(idx-name_size);
					subterm_positions.push_back(idx2);
					idx = idx2 + 1;
					continue;
				}
			} else {
				if (!idx || (last_op_idx && (last_op_idx == idx-1))) {
					throw new Error("Funktionsklammer ohne Funktionsname !");
				} else {
					ss << idx;
					throw new Error(string("Unbekannte Funktion an Stelle ") + COL_HLP + ss.str() + COL_CLR + " !");
				}
			}
		}

		/* Zwischen Operatoren ? */
		if (name_size = Plus::name.size(), (m_string.size()-idx >= name_size) && (m_string.substr(idx,name_size).compare(Plus::name) == 0L)) {
			in_operator = true;
			ops.push_back(Plus::name);
		} else if (name_size = Minus::name.size(), (m_string.size()-idx >= name_size) && (m_string.substr(idx,name_size).compare(Minus::name) == 0L)) {
			if (!idx && m_is_numeric) { idx += name_size; continue; }				/* Damit - auch am Anfang stehen kann */
			in_operator = true;
			ops.push_back(Minus::name);
		} else if (name_size = Mult::name.size(), (m_string.size()-idx >= name_size) && (m_string.substr(idx,name_size).compare(Mult::name) == 0L)) {
			in_operator = true;
			ops.push_back(Mult::name);
		} else if (name_size = Div::name.size(), (m_string.size()-idx >= name_size) && (m_string.substr(idx,name_size).compare(Div::name) == 0L)) {
			in_operator = true;
			ops.push_back(Div::name);
		} else if (name_size = Pow::name.size(), (m_string.size()-idx >= name_size) && (m_string.substr(idx,name_size).compare(Pow::name) == 0L)) {
			in_operator = true;
			ops.push_back(Pow::name);
		//TODO: operatoren aufnehmen
		}
		if (in_operator) {
			in_operator = false;
			if (!idx || (idx == m_string.size()-1)) { throw new Error("Operator am Anfang oder Ende !"); }
			if (!subterm_positions.size()) {
				subterm_positions.push_back(0);
				subterm_positions.push_back(idx-1);
			} else if (subterm_positions.back() != idx-1) {
				if (last_op_idx == idx-1) { ss << idx2; throw new Error(string("Mehrere Operatoren hintereinander an Position ") + COL_HLP + ss.str() + COL_CLR + " !"); }
				if (last_op_idx < subterm_positions.back()) { ss << subterm_positions.back()+1; throw new Error(string("Fehlender Operator nach Term an Position ") + COL_HLP + ss.str() + COL_CLR + " !"); }
				idx2 = last_op_idx + 1;
				subterm_positions.push_back(idx2);
				subterm_positions.push_back(idx-1);
			}
			last_op_idx = idx + name_size - 1;
			idx += name_size;
			continue;
		}

		/* Am Ende ? */
		if (idx == m_string.size()-1) {
			if (!subterm_positions.size()) {
				subterm_positions.push_back(0);
			} else if (subterm_positions.back() < last_op_idx) {
				subterm_positions.push_back(last_op_idx+1);
			} else {
				ss << subterm_positions.back()+1;
				throw new Error(string("Fehlender Operator nach Term an Position ") + COL_HLP + ss.str() + COL_CLR + " !");
			}
			subterm_positions.push_back(idx);
		}
		idx++;
	}

	/* Subterm-Strings */
	for (unsigned long i=0; i<subterm_positions.size(); i+=2) {
		subterms.push_back(m_string.substr(subterm_positions[i],subterm_positions[i+1]-subterm_positions[i]+1));
	}

//	/* Debug-Ausgabe */
//	for (unsigned long i=0, idx=0; i<m_string.size(); i++) {
//		if ((idx < subterm_positions.size()-1) && (subterm_positions[idx] <= i) && (i <= subterm_positions[idx+1])) {
//			cout << subterm_replace;
//			i += subterm_positions[idx+1] - subterm_positions[idx];
//			idx += 2;
//		} else {	cout << m_string[i];	}
//	}
//	cout << endl;
}

void Term::create(const vector<string> &subterms, const vector<string> &ops) {

	if (ops.size()) {											/* Gibt es einen Operator/Funktion ? */
		if (subterms.size() == 1L) {						/* Nur 1 Subterm? -> Funktion */
			if (ops[0].compare(Exp::name) == 0L) {
				m_fkt = new Exp(subterms[0]);
			} else if (ops[0].compare(Sin::name) == 0L) {
				m_fkt = new Sin(subterms[0]);
			} else if (ops[0].compare(Cos::name) == 0L) {
				m_fkt = new Cos(subterms[0]);
			} else if (ops[0].compare(Tan::name) == 0L) {
				m_fkt = new Tan(subterms[0]);
			}
			//TODO: funktionen aufnehmen
			return;

		} else {													/* Nein -> Mehrere Operanden */
			for (unsigned long i=0; i<ops.size(); i++) {
				if (ops[i].compare(Plus::name) == 0L) {
					m_fkt = new Plus(get_multi_args(subterms,Plus::name));
					return;
				}
			}
			for (unsigned long i=0; i<ops.size(); i++) {
				if (ops[i].compare(Minus::name) == 0L) {
					m_fkt = new Minus(get_multi_args(subterms,Minus::name));
					return;
				}
			}
			for (unsigned long i=0; i<ops.size(); i++) {
				if (ops[i].compare(Mult::name) == 0L) {
					m_fkt = new Mult(get_multi_args(subterms,Mult::name));
					return;
				}
			}
			for (unsigned long i=0; i<ops.size(); i++) {
				if (ops[i].compare(Div::name) == 0L) {
					m_fkt = new Div(get_multi_args(subterms,Div::name));
					return;
				}
			}
			for (unsigned long i=0; i<ops.size(); i++) {
				if (ops[i].compare(Pow::name) == 0L) {
					vector<string> args = get_multi_args(subterms,Pow::name);
					if (args.size() > 2L) { throw new Error("Mehrere Potenzoperatoren in einem Term !"); }
					m_fkt = new Pow(args);
					return;
				}
			}
			//TODO: operatoren aufnehmen
		}
	} else {														/* Nein -> RAW */
		m_is_primitive = true;
      parse_numeric();
	}
}

vector<string> Term::get_multi_args(const vector<string> &subterms, const string &op_name) const {

	vector<string> args;
	long pos = 0, last_pos = -1;
	for (unsigned long i=0; i<subterms.size(); i++) {
		pos = m_string.substr(pos).find(subterms[i]) + subterms[i].size() + pos;
		if ((pos < long(m_string.size())) && (m_string.substr(pos,op_name.size()).compare(op_name) == 0L)) {
			args.push_back(m_string.substr(last_pos+op_name.size(),pos-last_pos-1));
			last_pos = pos;
		}
		if (i == subterms.size()-1) {
			args.push_back(m_string.substr(last_pos+op_name.size()));
		}
	}

	cout << "  ";
	for (unsigned long i=0; i<args.size(); i++) { cout << args[i] << " | "; };	cout << endl;

	return args;
}
