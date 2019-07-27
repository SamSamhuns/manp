#include <iostream>
#include <unistd.h>
#include "common.h"


int main(int argc, char const *argv[]) {
	/* Loading the entire txt documentation that has been pre-converted to
	raw string format to char ptr */
	std::string python_doc_string =
	#include "combined_txt_include.h"
	;

	// std::string (python_doc_string);
	std::cout << "NOW PRINTING THE STRING\n";
	std::cout << python_doc_string << std::endl;
	return 0;
}
