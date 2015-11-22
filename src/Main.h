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

#include <map>
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


#endif