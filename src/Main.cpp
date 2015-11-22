#include "Main.h"
#include "Term.h"
#include "Lib.h"


using namespace std;


int main(int argc, char *argv[]) {

	try {
		if (argc != 2) {
			throw new Error("Bitte genau einen Term angeben!");
		}

		extern bool replace_constants;
		replace_constants = false;

		string in(argv[1]);
		Term input(in);

		cout << COL_HLP << "Lib:" << COL_CLR << endl;
		Lib *lib = new Lib(&input, "term", false);
		cout << COL_HLP << lib->name_file() << COL_CLR << endl;

		cout << COL_HLP << "var, val:" << COL_CLR << endl;
		Term var("x"), val("1.1");

//		cout << COL_HLP << "simpl:" << COL_CLR << endl;
//		cout << COL_HLP << input.simplify().print() << COL_CLR << endl;

//		cout << COL_HLP << "eval:" << COL_CLR << endl;
//		cout << COL_HLP << input.evaluate(var, val).print() << COL_CLR << endl;

		cout << COL_HLP << "deriv:" << COL_CLR << endl;
		cout << COL_HLP << input.derivate(var).print() << COL_CLR << endl;


	} catch (Error *exc) {
		exc->print_error();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
