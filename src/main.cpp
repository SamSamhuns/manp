#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <typeinfo>
#include "common.h"

// This is a C++ implementation of Peter Norvig's SpellingCorrector <https://norvig.com/spell-correct.html>
// from Felipe Farinon <felipe.farinon(@)gmail.com> under an MIT License
#include "SpellingCorrector.h"

/* function to parse the cmd line args
   returns 1 if the program is supposed to be terminated after this function call */
int parse_cmd_line_arg(std::string &cmd_line_arg, std::vector <std::string> &module_vector);

/*  function to map the python default module names to their respective documentations
    The module names themselves are the key, the value is the start and end pos of the doc part
    for the respective module in the python_doc_string
    The delimiter for key and values is CPP_MAP_KEY_VALUE_DELIM while
    the delimiter for different modules is CPP_MAP_MODULE_DELIM
    mappify also loads the module_vector with the names of the available modules
 */
std::map<std::string, std::vector<int> > mappify(std::string const& python_doc_string,
                                                 std::vector<std::string> &module_vector);

int main(int argc, char const *argv[]) {
    /* load the modules list from the include folder
    This raw string literal txt file is created by the generate_modules_list bash script */
    std::string module_list =
    #include "module_list.txt"
    ;
	SpellingCorrector corrector; // creating an instance of the spelling corrector model
	corrector.load(module_list);

	if (argc != 2) {
		std::cerr << "\033[26;1mUsage:\033[0m manp [PYTHON_MODULE_OR_FUNC_NAME]" << '\n';
		std::cerr << "\tFor Help: manp -h" << '\n';
		return -1;
	}


	std::fstream temp_dump_file_ptr;
	std::vector <std::string> module_vector;
	std::string query_input, query_output;
	std::string TEMP_DUMP_FILENAME = "query_output_temp";

	/* Loading the entire txt documentation that has been pre-converted to
	   raw string format to the python_doc_string string */
	std::string python_doc_string =
	#include "combined_txt_include.h"
	;

	/* load the python_doc_string into the cpp_map
	    key = module/filename, value = start and end string pos of respective documentation */
	std::map<std::string, std::vector<int> > cpp_map = mappify(python_doc_string, module_vector);

	query_input = std::string(argv[1]);
	if (parse_cmd_line_arg(query_input, module_vector) == 1) {
		/* user query has been dealt with */
		return 0;
	}

	if (cpp_map.find(query_input) == cpp_map.end()) { // the queried module does not exist in doc
		std::cerr << "\033[31;1mERROR:\033[0m " << query_input << " not recognized as a python standard module or function\n";
		/* if an unrecognized function/module was entered, attempt to make a guess */
		std::string correct(corrector.correct(query_input));
		if (correct != "") std::cout << "Did you mean: " << correct << '\n';
		return -1;
	} else {                                          // user query exists in doc
		query_output = python_doc_string.substr(cpp_map.find(query_input)->second.at(0), cpp_map.find(query_input)->second.at(1));
	}

	/* stream the output of the query to a temp file to use the cmd less
	   Remove the temp file later */
	temp_dump_file_ptr.open(TEMP_DUMP_FILENAME, std::ios::out);
	if (!temp_dump_file_ptr.is_open()) {
		std::cerr << "\033[31;1mERROR:\033[0m Cannot open temp dump file " << TEMP_DUMP_FILENAME << "\n";
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
std::map<std::string, std::vector<int> > mappify(std::string const& python_doc_string, std::vector<std::string> &module_vector) {
	std::string CPP_MAP_MODULE_DELIM_NEWLINE(CPP_MAP_MODULE_DELIM);
	std::string CPP_MAP_KEY_VALUE_DELIM_NEWLINE(CPP_MAP_KEY_VALUE_DELIM);

	/* Add newline chars to the delimiters as the text_to_raw_string functions encode the txt files by adding a newline char */
	CPP_MAP_MODULE_DELIM_NEWLINE = CPP_MAP_MODULE_DELIM_NEWLINE + "\n";
	CPP_MAP_KEY_VALUE_DELIM_NEWLINE = CPP_MAP_KEY_VALUE_DELIM_NEWLINE + "\n";

	std::vector<int> val_start_end;                              // vector to hold the start and end doc locations
	std::map<std::string, std::vector<int> > cpp_module_doc_map; // cpp string map

	/* Setting the start and end pos of key (module name) and val (start end pos of doc in python_doc_string) */
	std::string::size_type key_pos = 0;
	std::string::size_type key_end;
	std::string::size_type val_pos;
	std::string::size_type val_end;

	while((key_end = python_doc_string.find(CPP_MAP_KEY_VALUE_DELIM_NEWLINE, key_pos)) != std::string::npos) {
		if((val_pos = python_doc_string.find_first_not_of(CPP_MAP_KEY_VALUE_DELIM_NEWLINE, key_end)) == std::string::npos)
			break;

		val_end = python_doc_string.find(CPP_MAP_MODULE_DELIM_NEWLINE, val_pos);

		module_vector.push_back(python_doc_string.substr(key_pos, key_end - key_pos));
		val_start_end.push_back(val_pos);
		val_start_end.push_back(val_end-val_pos);

		// The keys for the map will be the filenames of the raw string literal txt files
		cpp_module_doc_map.emplace(python_doc_string.substr(key_pos, key_end - key_pos),
		                           val_start_end);
		val_start_end.clear();
		key_pos = val_end;
		if(key_pos != std::string::npos)
			++key_pos;
		key_pos += CPP_MAP_MODULE_DELIM_NEWLINE.size()-1;         // Skip the CPP_MAP_MODULE_DELIM_NEWLINE part for keys
	}

	return cpp_module_doc_map;
}

/* function to parse the cmd line args
   returns 1 if the program is supposed to be terminated after this function call */
int parse_cmd_line_arg(std::string &cmd_line_arg, std::vector <std::string> &module_vector) {
	if (cmd_line_arg == "-l") {
		std::cout << "\033[26;1mAvailable modules/functions with documentation:\033[0m" << '\n';
		for (size_t i = 0; i < module_vector.size(); i++) {
			std::cout << "  " << module_vector.at(i) << '\n';
		}
		return 1;
	}
	else if (cmd_line_arg == "-h") {
		std::cout << "\033[26;1mOptions:\033[0m" << '\n';
		std::cout << "\t -h: Help" << '\n';
		std::cout << "\t -l: list all python standard library documentations available" << "\n\n";
		std::cout << "\033[26;1mUsage:\033[0m" << '\n';
		std::cout << "\t manp [PYTHON_MODULE_OR_FUNC_NAME]" << '\n';
		return 1;
	}
	return 0;
}
