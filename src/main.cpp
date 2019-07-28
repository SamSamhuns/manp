#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <typeinfo>
#include "common.h"

/*  function to map the python default module names to their respective documentations
    The module names themselves are the key, the value is the start and end pos of the doc part
    for the respective module in the python_doc_string
    The delimiter for key and values is CPP_MAP_KEY_VALUE_DELIM while
    the delimiter for different modules is CPP_MAP_MODULE_DELIM */
std::map<std::string, std::vector<int> > mappify(std::string const& python_doc_string);

int main(int argc, char const *argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: manp [PYTHON_MODULE_OR_FUNC_NAME]" << '\n';
		return -1;
	}


	std::fstream temp_dump_file_ptr;
	std::string query_input, query_output;
	std::string TEMP_DUMP_FILENAME = "query_output_temp";

	/* Loading the entire txt documentation that has been pre-converted to
	   raw string format to python_doc_string string */
	std::string python_doc_string =
	#include "combined_txt_include.h"
	;

	/* load the python_doc_string into the cpp_map
	    key = module/filename, value = start and end string pos of respective documentation */
	std::map<std::string, std::vector<int> > cpp_map = mappify(python_doc_string);

	///////////////////// TODO /////////////////////
	query_input = std::string(argv[1]);

	if (cpp_map.find(query_input) == cpp_map.end()) { // the queried module does not exist in doc
		std::cerr << query_input << " not recognized as a python standard module or function.\n";
		return -1;
	} else {                                          // user query exists in doc
		query_output = python_doc_string.substr(cpp_map.find(query_input)->second.at(0), cpp_map.find(query_input)->second.at(1));
	}

	// query_output = std::string(python_doc_string); ///////////// TODO REMOVE THIS STATEMENT LATER
	//
	// // TODO REMOVE LATER, PRINTING CONTENTS OF THE MAP

	// for(auto const& p: cpp_map) {
	//  std::cout << "{KEY***" << p.first << "***}, Value=" << python_doc_string.substr(p.second.at(0), p.second.at(1)) << "\n";
	// }
	//
	// std::string value = cpp_map.at("abc");
	// std::cout << "Value is " << value << " space " << cpp_map.count("abc") ;


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
    The module names themselves are the key, the value is the start and end pos of the doc part
    for the respective module in the python_doc_string
    The delimiter for key and values is CPP_MAP_KEY_VALUE_DELIM while
    the delimiter for different modules is CPP_MAP_MODULE_DELIM */
std::map<std::string, std::vector<int> > mappify(std::string const& python_doc_string) {
	std::string CPP_MAP_MODULE_DELIM_NEWLINE(CPP_MAP_MODULE_DELIM);
	std::string CPP_MAP_KEY_VALUE_DELIM_NEWLINE(CPP_MAP_KEY_VALUE_DELIM);

	CPP_MAP_MODULE_DELIM_NEWLINE = CPP_MAP_MODULE_DELIM_NEWLINE + "\n";
	CPP_MAP_KEY_VALUE_DELIM_NEWLINE = CPP_MAP_KEY_VALUE_DELIM_NEWLINE + "\n";

	std::map<std::string, std::vector<int> > cpp_module_doc_map; // cpp string map

	/* Setting the start and end pos of key (module name) and val (doc info) */
	std::string::size_type key_pos = 0;
	std::string::size_type key_end;
	std::string::size_type val_pos;
	std::string::size_type val_end;
	std::vector<int> val_start_end;

	while((key_end = python_doc_string.find(CPP_MAP_KEY_VALUE_DELIM_NEWLINE, key_pos)) != std::string::npos) {
		if((val_pos = python_doc_string.find_first_not_of(CPP_MAP_KEY_VALUE_DELIM_NEWLINE, key_end)) == std::string::npos)
			break;

		val_end = python_doc_string.find(CPP_MAP_MODULE_DELIM_NEWLINE, val_pos);

		val_start_end.push_back(val_pos);
		val_start_end.push_back(val_end-val_pos);
		cpp_module_doc_map.emplace(python_doc_string.substr(key_pos, key_end - key_pos),
		                           val_start_end);
		val_start_end.clear();
		key_pos = val_end;
		if(key_pos != std::string::npos)
			++key_pos;
		key_pos += CPP_MAP_MODULE_DELIM_NEWLINE.size()-1;
	}

	return cpp_module_doc_map;
}
