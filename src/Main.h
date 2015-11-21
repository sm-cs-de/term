#ifndef TERM_MAIN_H
#define TERM_MAIN_H

#include <cstdlib>
#include <cerrno>
#include <cmath>
#include <cstring>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <algorithm>
#include <iomanip>
#include <limits>



class Term;
class Function;

#define COL_ERR		"\033[1;31m"
#define COL_HLP		"\033[1;33m"
#define COL_CLR		"\033[m"


class Error {
	public:
		Error(const std::string &exc) : m_error(exc) {};

		void print_error() const {
			std::cout << COL_ERR << "#FEHLER" << COL_CLR << ":   " << m_error << std::endl;
		};

	private:
		std::string m_error;
};

//namespace std {
//// This class exists solely to "trick" the compiler into
//// considering this allocator a new, different type
//class newallocator : public allocator<char> {};
//
//// template specialize std::stringstream to inherit from basic_stringstream
//// using our newallocator in place of std::allocator<char>
//template <> class basic_stringstream<char,char_traits<char>,allocator<char> > : public basic_stringstream <char,char_traits<char>,newallocator> {
//	public:
//		basic_stringstream() {
//			precision(16);
//		}
//};
//}
//
//namespace std {
//	class precision_ss : public stringstream {
//		public:
//			precision_ss(ios_base::openmode which = ios_base::in) : stringstream(which)  {
//				precision(3);
//			}
//			precision_ss(const string& str, ios_base::openmode which = ios_base::in | ios_base::out) : stringstream(str,which) {
//				precision(3);
//			}
//	};
//}
//#define stringstream precision_ss


#endif