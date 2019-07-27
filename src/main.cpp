#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include "common.h"

/*  function to map the python default module names to their respective documentations
    The module names themselves are the key, the value is the respective doc
    The delimiter for key and values is CPP_MAP_KEY_VALUE_DELIM while
    the delimiter for different modules is CPP_MAP_MODULE_DELIM */
std::map<std::string, std::string> mappify(std::string const& python_doc_string);

int main(int argc, char const *argv[]) {
	std::fstream temp_dump_file_ptr;
	std::string query_output;
	std::string TEMP_DUMP_FILENAME = "query_output_temp";

	/* Loading the entire txt documentation that has been pre-converted to
	   raw string format to python_doc_string string */
	std::string python_doc_string =
	#include "combined_txt_include.h"
	;

	/* load the python_doc_string into the cpp_map
	    key = module/filename, value = respective documentation*/
	auto cpp_map = mappify(python_doc_string);

	///////////////////// TODO /////////////////////


	query_output = std::string(python_doc_string); ///////////// TODO REMOVE THIS STATEMENT LATER

	// TODO REMOVE LATER, PRINTING CONTENTS OF THE MAP
	for(auto const& p: cpp_map) {
		std::cout << '{' << p.first << " => " << p.second << '}' << '\n';
	}


	//
	//
	// Process user query through another function
	//
	//


	///////////////// END OF TODO ///////////////////

	/* stream the output of the query to a temp file to use the cmd less
	   Remove the temp file later */
	temp_dump_file_ptr.open(TEMP_DUMP_FILENAME, std::ios::out);
	if (!temp_dump_file_ptr.is_open()) {
		std::cerr << "ERROR: Cannot open temp dump file " << TEMP_DUMP_FILENAME << "\n";
		return -1;
	}

	temp_dump_file_ptr << query_output; // save the contents of the query output to temp file
	temp_dump_file_ptr.close();         // close the file ptr before passing the temp file to less

	// print the contents of the temp output file
	system(("less < "+TEMP_DUMP_FILENAME).c_str());
	// remove the temp files
	system(("rm "+TEMP_DUMP_FILENAME).c_str());


	return 0;
}

/*  function to map the python default module names to their respective documentations
    The module names themselves are the key, the value is the respective doc
    The delimiter for key and values is CPP_MAP_KEY_VALUE_DELIM while
    the delimiter for different modules is CPP_MAP_MODULE_DELIM
    mapify function based on Galik's stackoverflow answer to
    https://stackoverflow.com/questions/38812780/split-string-into-key-value-pairs-using-c */
std::map<std::string, std::string> mappify(std::string const& python_doc_string) {
	std::string CPP_MAP_MODULE_DELIM_NEWLINE(CPP_MAP_MODULE_DELIM);
	std::string CPP_MAP_KEY_VALUE_DELIM_NEWLINE(CPP_MAP_KEY_VALUE_DELIM);

	CPP_MAP_MODULE_DELIM_NEWLINE += "\n";
	CPP_MAP_KEY_VALUE_DELIM_NEWLINE += "\n";

	std::map<std::string, std::string> cpp_module_doc_map; // cpp string map

	/* Setting the start and end pos of key (module name) and val (doc info) */
	std::string::size_type key_pos = 0;
	std::string::size_type key_end;
	std::string::size_type val_pos;
	std::string::size_type val_end;

	while((key_end = python_doc_string.find(CPP_MAP_KEY_VALUE_DELIM, key_pos)) != std::string::npos) {
		if((val_pos = python_doc_string.find_first_not_of(CPP_MAP_KEY_VALUE_DELIM_NEWLINE, key_end)) == std::string::npos)
			break;

		val_end = python_doc_string.find(CPP_MAP_MODULE_DELIM_NEWLINE, val_pos);
		cpp_module_doc_map.emplace(python_doc_string.substr(key_pos, key_end - key_pos),
		                           python_doc_string.substr(val_pos, val_end - val_pos));

		key_pos = val_end;
		if(key_pos != std::string::npos)
			++key_pos;
	}

	return cpp_module_doc_map;
}
