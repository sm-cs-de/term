#ifndef TERM_FUNCTION_HPP
#define TERM_FUNCTION_HPP

#include "Main.hpp"
#include "Term.hpp"
#include "Parse.hpp"


inline std::string function_derivate_inner(Term &fkt_arg, const Term &var);
inline std::string function_evaluate(Term &fkt_arg, const std::string &fkt_name, const Term &var, const Term &val);
inline std::string function_simplify(Term &fkt_arg, const std::string &fkt_name, double (*const fkt)(double));


class Function {
   public:
      virtual Term derivate(const Term &var) = 0;
      virtual Term evaluate(const Term &var, const Term &val) = 0;
      virtual Term simplify() = 0;

      inline const std::vector<Term *> &args() const { return m_args; }
      virtual inline const std::string &_name() const = 0;

   protected:
      Function(const std::vector<std::string> &term_str);
      Function(const std::string &term_str);
      Function(const Ast *const arg);
      Function(const Ast *const left, const Ast *const right);

      std::vector<Term *> m_args;
};


/** Operatoren */

class Add : public Function {
   public:
      Add(const std::vector<std::string> &term_str);

      static const std::string name;
      virtual inline const std::string &_name() const { return name; }

      virtual Term derivate(const Term &var);
      virtual Term evaluate(const Term &var, const Term &val);
      virtual Term simplify();
};

class Sub : public Function {
   public:
      Sub(const std::vector<std::string> &term_str);

      static const std::string name;
      virtual inline const std::string &_name() const { return name; }

      virtual Term derivate(const Term &var);
      virtual Term evaluate(const Term &var, const Term &val);
      virtual Term simplify();
};

class Mul : public Function {
   public:
      Mul(const std::vector<std::string> &term_str);

      static const std::string name;
      virtual inline const std::string &_name() const { return name; }

      virtual Term derivate(const Term &var);
      virtual Term evaluate(const Term &var, const Term &val);
      virtual Term simplify();
};

class Div : public Function {
   public:
      Div(const std::vector<std::string> &term_str);

      static const std::string name;
      virtual inline const std::string &_name() const { return name; }

      virtual Term derivate(const Term &var);
      virtual Term evaluate(const Term &var, const Term &val);
      virtual Term simplify();
};

class Pow : public Function {
   public:
      Pow(const std::vector<std::string> &term_str);

      static const std::string name;
      virtual inline const std::string &_name() const { return name; }

      virtual Term derivate(const Term &var);
      virtual Term evaluate(const Term &var, const Term &val);
      virtual Term simplify();
};


/** Funktionen (1 Parameter) */

class Exp : public Function {
   public:
      Exp(const std::string &term_str);

      static const std::string name;
      virtual inline const std::string &_name() const { return name; }

      virtual Term derivate(const Term &var);
      virtual Term evaluate(const Term &var, const Term &val);
      virtual Term simplify();
};

class Sin : public Function {
   public:
      Sin(const std::string &term_str);

      static const std::string name;
      virtual inline const std::string &_name() const { return name; }

      virtual Term derivate(const Term &var);
      virtual Term evaluate(const Term &var, const Term &val);
      virtual Term simplify();
};

class Cos : public Function {
   public:
      Cos(const std::string &term_str);

      static const std::string name;
      virtual inline const std::string &_name() const { return name; }

      virtual Term derivate(const Term &var);
      virtual Term evaluate(const Term &var, const Term &val);
      virtual Term simplify();
};

class Tan : public Function {
   public:
      Tan(const std::string &term_str);

      static const std::string name;
      virtual inline const std::string &_name() const { return name; }

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
