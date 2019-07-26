#include <iostream>
#include <unistd.h>
#include "common.h"


int main(int argc, char const *argv[]) {

	std::cout << "Lets Begin" << std::endl;
	const char *s =
	#include "../data/abc.txt"
	;

	std::string test(s);
	std::cout << "NOW PRINTING THE STRING\n";
	std::cout << test << std::endl;
	return 0;
}
