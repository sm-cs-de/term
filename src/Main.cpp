#include "Main.hpp"
#include "Term.hpp"
#include "Lib.hpp"


using namespace std;


int main(int argc, char *argv[]) {

	try {
		if (argc != 2) {
			throw new Error("Bitte genau einen Term angeben!");
		}

		string in(argv[1]);

      Ast *ast = Ast::parse(in);
      cout << ast->print_tree() << endl;

		Term input(ast);
		cout << input._string() << endl;

//		cout << COL_HLP << "Lib:" << COL_CLR << endl;
//		Lib *lib = new Lib(&input, "term", false);
//		cout << COL_HLP << lib->name_file() << COL_CLR << endl;

//		cout << COL_HLP << "var, val:" << COL_CLR << endl;
//		Term var("x"), val("1.1");

//		cout << COL_HLP << "simpl:" << COL_CLR << endl;
//		cout << COL_HLP << input.simplify()._string() << COL_CLR << endl;

//		cout << COL_HLP << "eval:" << COL_CLR << endl;
//		cout << COL_HLP << input.evaluate(var, val)._string() << COL_CLR << endl;

//		cout << COL_HLP << "deriv:" << COL_CLR << endl;
//		cout << COL_HLP << input.derivate(var)._string() << COL_CLR << endl;


	} catch (Error *exc) {
		exc->print_error();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
