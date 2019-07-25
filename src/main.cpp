#include <iostream>
#include <unistd.h>
#include "common.h"


int main(int argc, char const *argv[]) {

	std::cout << "Lets Begin" << std::endl;
	const char *s =
	#include "../data/abc.txt"
	;

	std::cout << s << std::endl;
	return 0;
}
