#include "Function.hpp"


using namespace std;


/** Add */
Add::Add(const vector<string> &term_str) : Function(term_str) {}

const string Add::name(ADD_NAME);

Term Add::derivate(const Term &var) {
   stringstream deriv_ss;

   for (unsigned long i=0; i<m_args.size(); i++) {
      deriv_ss << m_args[i]->derivate(var)._string();
      if (i < m_args.size()-1) {
         deriv_ss << Add::name;
      }
   }

   return Term(deriv_ss.str()).simplify();
}

Term Add::evaluate(const Term &var, const Term &val) {
   stringstream eval_ss;

   for (unsigned long i=0; i<m_args.size(); i++) {
      eval_ss << m_args[i]->evaluate(var,val)._string();
      if (i < m_args.size()-1) {
         eval_ss << Add::name;
      }
   }

   return Term(eval_ss.str()).simplify();
}

Term Add::simplify() {
   stringstream simpl_ss;

   double sum = 0.0;
   bool only_numeric = true;
   for (unsigned long i=0; i<m_args.size(); i++) {
      if (m_args[i]->is_numeric()) {
         sum += m_args[i]->num();
         // } else if (!m_args[i]->is_primitive() && (m_args[i]->fkt()->_name().compare(Sub::name) == 0L) && (m_args[i]->fkt()->args()[0]->_string().compare("0") == 0L)) {

      } else {
         if (m_args[i]->is_primitive() || (m_args[i]->fkt()->args().size() == 1L)) {
            simpl_ss << m_args[i]->_string();
         } else {
            Term simpl = m_args[i]->simplify();
            if (simpl.is_numeric()) {
               sum += simpl.num();
               continue;
            } else {
               simpl_ss << simpl._string();
            }
         }
         only_numeric = false;
         simpl_ss << Add::name;
      }
   }

   if (only_numeric || (sum != 0.0)) {
      simpl_ss << sum;
   } else {
      string tmp = simpl_ss.str();
      simpl_ss.str(tmp.substr(0,tmp.size()-Add::name.size()));
   }

   return Term(simpl_ss.str());
}


/** Sub */
Sub::Sub(const vector<string> &term_str) : Function(term_str) {}

const string Sub::name(SUB_NAME);

Term Sub::derivate(const Term &var) {
   stringstream deriv_ss;

   for (unsigned long i=0; i<m_args.size(); i++) {
      Term deriv_arg = m_args[i]->derivate(var);
      if (!i || deriv_arg.is_primitive() || (deriv_arg.fkt()->args().size() == 1L)) {
         if (deriv_arg.is_numeric() && (deriv_arg.num() < 0.0)) {
            deriv_ss << deriv_arg._string().substr(Sub::name.size());
         } else {
            deriv_ss << deriv_arg._string();
         }
      } else {
         deriv_ss << term_bracket[0] << deriv_arg._string() << term_bracket[1];
      }
      if (i < m_args.size()-1) {
         deriv_ss << Sub::name;
      }
   }

   return Term(deriv_ss.str()).simplify();
}

Term Sub::evaluate(const Term &var, const Term &val) {
   stringstream eval_ss;

   for (unsigned long i=0; i<m_args.size(); i++) {
      Term eval_arg = m_args[i]->evaluate(var,val);
      if (!i || eval_arg.is_primitive() || (eval_arg.fkt()->args().size() == 1L)) {
         eval_ss << eval_arg._string();
      } else {
         eval_ss << term_bracket[0] << eval_arg._string() << term_bracket[1];
      }
      if (i < m_args.size()-1) {
         eval_ss << Sub::name;
      }
   }

   return Term(eval_ss.str()).simplify();
}

Term Sub::simplify() {
   stringstream simpl_ss;

   double diff = 0.0;
   bool only_numeric = true;
   for (unsigned long i=0; i<m_args.size(); i++) {
      if (m_args[i]->is_numeric()) {
         if (!i) {
            diff = m_args[i]->num();         /* Falls erstes Argument Zahl ist, hat sie u.U. pos. Vorzeichen */
         } else {
            diff -= m_args[i]->num();
         }
      } else {
         if (i && only_numeric) {            /* Falls erstes Argument Zahl ist, bekommt erste nicht-Zahl sonst kein Sub davor */
            simpl_ss << Sub::name;
         }
         if (m_args[i]->is_primitive() || (m_args[i]->fkt()->args().size() == 1L)) {
            simpl_ss << m_args[i]->_string();
         } else {
            Term simpl = m_args[i]->simplify();
            if (simpl.is_numeric()) {
               if (!i) {
                  diff = simpl.num();
               } else {
                  diff -= simpl.num();
               }
               if (i && only_numeric) {            /* Um das schon geschriebene Sub von oben zu kompensieren */
                  simpl_ss << Sub::name;
               }
               continue;
            } else if (simpl.is_primitive() || (simpl.fkt()->args().size() == 1L)) {
               simpl_ss << simpl._string();
            } else {
               simpl_ss << term_bracket[0] << simpl._string() << term_bracket[1];
            }
         }
         only_numeric = false;
         simpl_ss << Sub::name;
      }
   }

   if (only_numeric) {
      simpl_ss << diff;
   } else if (diff != 0.0) {
      simpl_ss << Sub::name << diff;       /* Um anhängendes Sub der letzten nicht-Zahl zu kompensieren */
   } else {
      string tmp = simpl_ss.str();
      simpl_ss.str(tmp.substr(0,tmp.size()-Sub::name.size()));
   }

   return Term(simpl_ss.str());
}


/** Mul */
Mul::Mul(const vector<string> &term_str) : Function(term_str) {}

const string Mul::name(MUL_NAME);

Term Mul::derivate(const Term &var) {
   stringstream deriv_ss;

   for (unsigned long i=0; i<m_args.size(); i++) {
      Term deriv_arg = m_args[i]->derivate(var);
      if ((deriv_arg.is_primitive() && (!i || (deriv_arg.num() >= 0.0))) || (!deriv_arg.is_primitive() && (deriv_arg.fkt()->args().size() == 1L))) {
         deriv_ss << deriv_arg._string();
      } else {
         deriv_ss << term_bracket[0] << deriv_arg._string() << term_bracket[1];
      }
      for (unsigned long j=0; j<m_args.size(); j++) {
         if (i==j) { continue; }
         if ((m_args[j]->is_primitive() && (m_args[j]->num() >= 0.0)) || (!m_args[j]->is_primitive() && (m_args[j]->fkt()->args().size() == 1L))) {
            deriv_ss << Mul::name << m_args[j]->_string();
         } else {
            deriv_ss << Mul::name << term_bracket[0] << m_args[j]->_string() << term_bracket[1];
         }
      }
      if (i < m_args.size()-1) {
         deriv_ss << Add::name;
      }
   }

   return Term(deriv_ss.str()).simplify();
}

Term Mul::evaluate(const Term &var, const Term &val) {
   stringstream eval_ss;

   for (unsigned long i=0; i<m_args.size(); i++) {
      Term eval_arg = m_args[i]->evaluate(var,val);
      if ((eval_arg.is_primitive() && (!i || (eval_arg.num() >= 0.0))) || (!eval_arg.is_primitive() && (eval_arg.fkt()->args().size() == 1L))) {
         eval_ss << eval_arg._string();
      } else {
         eval_ss << term_bracket[0] << eval_arg._string() << term_bracket[1];
      }
      if (i < m_args.size()-1) {
         eval_ss << Mul::name;
      }
   }

   return Term(eval_ss.str()).simplify();
}

Term Mul::simplify() {
   stringstream simpl_ss;

   double prod = 1.0;
   bool only_numeric = true;
   for (unsigned long i=0; i<m_args.size(); i++) {
      if (m_args[i]->is_numeric()) {
         prod *= m_args[i]->num();
      } else if (m_args[i]->is_primitive() || (m_args[i]->fkt()->args().size() == 1L)) {
         only_numeric = false;
         simpl_ss << m_args[i]->_string() << Mul::name;
      } else {
         Term simpl = m_args[i]->simplify();
         if (simpl.is_numeric()) {
            prod *= simpl.num();
            continue;
         } else if (simpl.is_primitive() || (simpl.fkt()->args().size() == 1L)) {
            simpl_ss << simpl._string() << Mul::name;
         } else {
            simpl_ss << term_bracket[0] << simpl._string() << term_bracket[1] << Mul::name;
         }
         only_numeric = false;
      }
   }

   if (only_numeric || (prod != 1.0)) {
      if (prod == 0.0) {
         simpl_ss.str("0");
      } else {
         simpl_ss << prod;
      }
   } else {
      string tmp = simpl_ss.str();
      simpl_ss.str(tmp.substr(0,tmp.size()-Mul::name.size()));
   }

   return Term(simpl_ss.str());
}


/** Div */
Div::Div(const vector<string> &term_str) : Function(term_str) {}

const string Div::name(DIV_NAME);

Term Div::derivate(const Term &var) {
   stringstream deriv_ss;

   Term simpl_arg = simplify();     /* Ab jetzt gibt es nur noch einen Bruch mit u und v oder garkeinen Bruch mehr */

   if (simpl_arg.is_primitive() || (simpl_arg.fkt()->args().size() == 1L) || (simpl_arg.fkt()->_name().compare(Div::name) != 0L)) {
      deriv_ss << simpl_arg.derivate(var)._string();
   } else {
      Term u = *simpl_arg.fkt()->args()[0];
      Term v = *simpl_arg.fkt()->args()[1];
      Term du = u.derivate(var);
      Term dv = v.derivate(var);

      if (u.is_numeric() && ((u.num() == 0.0) || v.is_numeric())) {
         //if (du.is_numeric() && dv.is_numeric() && (du.num() == 0.0) && (dv.num() == 0.0)) {
         deriv_ss << "0";
         return Term(deriv_ss.str());
      }

      deriv_ss << term_bracket[0];
      deriv_ss << Sub::name;
      if (u.is_primitive() || (u.fkt()->args().size() == 1L)) {
         deriv_ss << u._string();
      } else {
         deriv_ss << term_bracket[0] << u._string() << term_bracket[1];
      }
      deriv_ss << Mul::name;
      if ((dv.is_primitive() && (dv.num() >= 0.0)) || (!dv.is_primitive() && (dv.fkt()->args().size() == 1L))) {
         deriv_ss << dv._string();
      } else {
         deriv_ss << term_bracket[0] << dv._string() << term_bracket[1];
      }
      deriv_ss << Add::name;
      if (du.is_primitive() || (du.fkt()->args().size() == 1L)) {
         deriv_ss << du._string();
      } else {
         deriv_ss << term_bracket[0] << du._string() << term_bracket[1];
      }
      deriv_ss << Mul::name;
      if ((v.is_primitive() && (dv.num() >= 0.0)) || (!v.is_primitive() && (v.fkt()->args().size() == 1L))) {
         deriv_ss << v._string() << term_bracket[1] << Div::name << term_bracket[0] << v._string() << Mul::name << v._string() << term_bracket[1];
      } else {
         deriv_ss << term_bracket[0] << v._string() << term_bracket[1] << term_bracket[1] << Div::name << term_bracket[0] << term_bracket[0] << v._string() << term_bracket[1] << Mul::name << term_bracket[0] << v._string() << term_bracket[1] << term_bracket[1];
      }
   }

   return Term(deriv_ss.str()).simplify();
}

Term Div::evaluate(const Term &var, const Term &val) {
   stringstream eval_ss;

   for (unsigned long i=0; i<m_args.size(); i++) {
      Term eval_arg = m_args[i]->evaluate(var,val);
      if ((eval_arg.is_primitive() && (!i || (eval_arg.num() >= 0.0))) || (!eval_arg.is_primitive() && (eval_arg.fkt()->args().size() == 1L))) {
         eval_ss << eval_arg._string();
      } else {
         eval_ss << term_bracket[0] << eval_arg._string() << term_bracket[1];
      }
      if (i < m_args.size()-1) {
         eval_ss << Div::name;
      }
   }

   return Term(eval_ss.str()).simplify();
}

Term Div::simplify() {
   stringstream simpl_ss, divid_ss, divis_ss;

   double quot = 1.0;
   long divis_count = 0;
   for (unsigned long i=0; i<m_args.size(); i++) {
      if (m_args[i]->is_numeric()) {
         if (!i) {
            quot = m_args[i]->num();         /* Falls erstes Argument Zahl ist, wird sie geteilt */
         } else {
            if (m_args[i]->num() != 0.0) {
               quot /= m_args[i]->num();
            } else {
               throw new Error("Division durch Null !"); // TODO: vernünfige overflow-behandlung, auch bei mult usw.
            }
         }
      } else {
         if (m_args[i]->is_primitive() || (m_args[i]->fkt()->args().size() == 1L)) {
            if (!i) {
               divid_ss << m_args[i]->_string();
            } else {
               divis_ss << m_args[i]->_string();
               divis_count++;
            }
         } else {
            Term simpl = m_args[i]->simplify();
            if (simpl.is_numeric()) {
               if (!i) {
                  quot = simpl.num();
               } else {
                  if (simpl.num() != 0.0) {
                     quot /= simpl.num();
                  } else {
                     throw new Error("Division durch Null !"); // TODO: vernünfige overflow-behandlung, auch bei mult usw.
                  }
               }
               continue;
            } else if (simpl.is_primitive() || (simpl.fkt()->args().size() == 1L)) {
               if (!i) {
                  divid_ss << simpl._string();
               } else {
                  divis_ss << simpl._string();
                  divis_count++;
               }
            } else {
               if (!i) {
                  divid_ss << term_bracket[0] << m_args[i]->simplify()._string() << term_bracket[1];
               } else {
                  divis_ss << term_bracket[0] << m_args[i]->simplify()._string() << term_bracket[1];
                  divis_count++;
               }
            }
         }
         if (divis_count > 0L) {
            divis_ss << Mul::name;
         }
      }
   }

   if (divid_ss.str().size()) {
      if (quot != 1.0) {
         simpl_ss << quot << Mul::name;
      }
      simpl_ss << divid_ss.str();
   } else {
      simpl_ss << quot;
   }
   if (divis_ss.str().size()) {
      string tmp = divis_ss.str();
      divis_ss.str(tmp.substr(0,tmp.size()-Mul::name.size()));

      simpl_ss << Div::name;
      if (divis_count > 1) {
         simpl_ss << term_bracket[0];
      }
      simpl_ss << divis_ss.str();
      if (divis_count > 1) {
         simpl_ss << term_bracket[1];
      }
   }

   return Term(simpl_ss.str());
}


/** Pow */
Pow::Pow(const vector<string> &term_str) : Function(term_str) {}

const string Pow::name(POW_NAME);

Term Pow::derivate(const Term &var) {
   stringstream deriv_ss;

// Term simpl_arg = simplify();     /* Ab jetzt gibt es nur noch x^y */
//
// if (simpl_arg.is_primitive() || (simpl_arg.fkt()->args().size() == 1L) || (simpl_arg.fkt()->_name().compare(Pow::name) != 0L)) {
//    deriv_ss << simpl_arg.derivate(var)._string();
// } else {
//    Term x = *simpl_arg.fkt()->args()[0];
//    Term y = *simpl_arg.fkt()->args()[1];
//    Term du = u.derivate(var);
//    Term dv = v.derivate(var);
//
//    if (u.is_numeric() && ((u.num() == 0.0) || v.is_numeric())) {
//    //if (du.is_numeric() && dv.is_numeric() && (du.num() == 0.0) && (dv.num() == 0.0)) {
//       deriv_ss << "0";
//       return Term(deriv_ss.str());
//    }
//
//    deriv_ss << term_bracket[0];
//    deriv_ss << Sub::name;
//    if (u.is_primitive() || (u.fkt()->args().size() == 1L)) {
//       deriv_ss << u._string();
//    } else {
//       deriv_ss << term_bracket[0] << u._string() << term_bracket[1];
//    }
//    deriv_ss << Mul::name;
//    if ((dv.is_primitive() && (dv.num() >= 0.0)) || (!dv.is_primitive() && (dv.fkt()->args().size() == 1L))) {
//       deriv_ss << dv._string();
//    } else {
//       deriv_ss << term_bracket[0] << dv._string() << term_bracket[1];
//    }
//    deriv_ss << Add::name;
//    if (du.is_primitive() || (du.fkt()->args().size() == 1L)) {
//       deriv_ss << du._string();
//    } else {
//       deriv_ss << term_bracket[0] << du._string() << term_bracket[1];
//    }
//    deriv_ss << Mul::name;
//    if ((v.is_primitive() && (dv.num() >= 0.0)) || (!v.is_primitive() && (v.fkt()->args().size() == 1L))) {
//       deriv_ss << v._string() << term_bracket[1] << Div::name << term_bracket[0] << v._string() << Mul::name << v._string() << term_bracket[1];
//    } else {
//       deriv_ss << term_bracket[0] << v._string() << term_bracket[1] << term_bracket[1] << Div::name << term_bracket[0] << term_bracket[0] << v._string() << term_bracket[1] << Mul::name << term_bracket[0] << v._string() << term_bracket[1] << term_bracket[1];
//    }
// }
//
// for (unsigned long i=0; i<m_args.size(); i++) {
//    Term deriv_arg = m_args[i]->derivate(var);
//    if ((deriv_arg.is_primitive() && (!i || (deriv_arg.num() >= 0.0))) || (!deriv_arg.is_primitive() && (deriv_arg.fkt()->args().size() == 1L))) {
//       deriv_ss << deriv_arg._string();
//    } else {
//       deriv_ss << term_bracket[0] << deriv_arg._string() << term_bracket[1];
//    }
//    for (unsigned long j=0; j<m_args.size(); j++) {
//       if (i==j) { continue; }
//       if ((m_args[j]->is_primitive() && (m_args[j]->num() >= 0.0)) || (!m_args[j]->is_primitive() && (m_args[j]->fkt()->args().size() == 1L))) {
//          deriv_ss << Mul::name << m_args[j]->_string();
//       } else {
//          deriv_ss << Mul::name << term_bracket[0] << m_args[j]->_string() << term_bracket[1];
//       }
//    }
//    if (i < m_args.size()-1) {
//       deriv_ss << Add::name;
//    }
// }

   return Term(deriv_ss.str()).simplify();
}

Term Pow::evaluate(const Term &var, const Term &val) {
   stringstream eval_ss;

// for (unsigned long i=0; i<m_args.size(); i++) {
//    Term eval_arg = m_args[i]->evaluate(var,val);
//    if ((eval_arg.is_primitive() && (!i || (eval_arg.num() >= 0.0))) || (!eval_arg.is_primitive() && (eval_arg.fkt()->args().size() == 1L))) {
//       eval_ss << eval_arg._string();
//    } else {
//       eval_ss << term_bracket[0] << eval_arg._string() << term_bracket[1];
//    }
//    if (i < m_args.size()-1) {
//       eval_ss << Pow::name;
//    }
// }

   return Term(eval_ss.str()).simplify();
}

Term Pow::simplify() {
   stringstream simpl_ss;

// double prod = 1.0;
// bool only_numeric = true;
// for (unsigned long i=0; i<m_args.size(); i++) {
//    if (m_args[i]->is_numeric()) {
//       prod *= m_args[i]->num();
//    } else if (m_args[i]->is_primitive() || (m_args[i]->fkt()->args().size() == 1L)) {
//       only_numeric = false;
//       simpl_ss << m_args[i]->_string() << Mul::name;
//    } else {
//          Term simpl = m_args[i]->simplify();
//          if (simpl.is_numeric()) {
//             prod *= simpl.num();
//             continue;
//          } else if (simpl.is_primitive() || (simpl.fkt()->args().size() == 1L)) {
//             simpl_ss << simpl._string() << Mul::name;
//          } else {
//             simpl_ss << term_bracket[0] << simpl._string() << term_bracket[1] << Mul::name;
//          }
//       only_numeric = false;
//    }
// }
//
// if (only_numeric || (prod != 1.0)) {
//    if (prod == 0.0) {
//       simpl_ss.str("0");
//    } else {
//       simpl_ss << prod;
//    }
// } else {
//    string tmp = simpl_ss.str();
//    simpl_ss.str(tmp.substr(0,tmp.size()-Mul::name.size()));
// }

   return Term(simpl_ss.str());
}
