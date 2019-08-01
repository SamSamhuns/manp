#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <csignal>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <typeinfo>
#include "common.h"

// This is a modified C++ implementation of Felipe Farinon's <felipe.farinon(@)gmail.com> SpellingCorrector (MIT License)
// originally built in python by Peter Norvig <https://norvig.com/spell-correct.html>
#include "SpellingCorrector.h"

/* function to verify the number of cmd line args
    return -1 for failure and 0 for success */
int validate_cmd_line_input(int argc, char const*argv[]);

/* function to generate the query output
   returns 1 if the program is supposed to be terminated after this function call
   returns 0 if the query_output has to be processed through the less command */
int generate_query_output(char const *argv[], std::string &query_output);

/*  function to map the python default module names to their respective documentation
    The module names themselves are the key, the value is the start and end pos of the doc part
    for the respective module in the python_modules_string
    The delimiter for key and values is CPP_MAP_KEY_VALUE_DELIM while
    the delimiter for different modules is CPP_MAP_MODULE_DELIM */
std::map<std::string, std::vector<int> > mappify(std::string const& python_modules_string);

/* Signal Handler for SIGSEGV, segmentation faults */
void sigsegv_handler(int sig_num);

/* Signal Handler for SIGABRT, abort traps */
void sigabrt_handler(int sig_num);

int main(int argc, char const *argv[]) {
	signal(SIGSEGV, sigsegv_handler);  // signal handler for segmentation faults
	signal(SIGABRT, sigabrt_handler);  // signal ahndler for abort traps

	// Check for correct number of arguments used i.e. Only 2 or 3 argc valid
	if (validate_cmd_line_input(argc, argv) == -1) {
		std::cerr << "\033[32;1mUsage:\033[0m manp [ARTIFACT_MODULES_OR_FUNCTION_NAME]" << '\n';
		std::cout << "       For help: manp -h" << '\n';
		return -1;
	}

	std::fstream temp_dump_file_ptr;
	std::string query_output;
	std::string TEMP_DUMP_FILENAME = "query_output_temp.txt";

	//TODO call generate_query_output
	if (generate_query_output(argv, query_output) == 1) {
		return 0; // END the program at this point as no further processing is required
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
	system(("less -R < "+TEMP_DUMP_FILENAME).c_str());
	// remove the temp files
	system(("rm "+TEMP_DUMP_FILENAME).c_str());

	return 0;
}

/* function to generate the query output
   returns 1 if the program is supposed to be terminated after this function call
   returns 0 if the query_output has to be processed through the less command */
int generate_query_output(char const *argv[], std::string &query_output) {
	std::string query_input(argv[1]);

	/* help arg activated */
	if (query_input == "-h") {
		std::cout << "\033[32;1mOptions:\033[0m" << '\n';
		std::cout << "\t-h: Help" << '\n';
		std::cout << "\t-l: List all software artifact documentations available" << "\n";
		std::cout << "\033[32;1mUsage:\033[0m" << '\n';
		std::cout << "\tmanp [ARTIFACT_LIB_FUNCTION_NAME] i.e. manp zlib" << '\n';
		return 1;
	}
	/* listing all the available artifacts which have documentations available */
	else if (query_input == "-l") {
		std::cout << "\033[32;1mAvailable artifacts with documentation:\033[0m";
		std::string artifacts_list =
		#include "doc_artifacts_list.txt"
		;

		/* split the artifacts_list by the newline char which is the delimiter
		   and load it into a string vector to sort and print*/
		std::stringstream str_stream(artifacts_list);
		std::string artifact;
		std::vector<std::string> splitted_artifacts;
		while (std::getline(str_stream, artifact, '\n'))
		{
			splitted_artifacts.push_back(artifact);
		}
		std::sort(splitted_artifacts.begin(), splitted_artifacts.end());
		for (size_t i = 0; i < splitted_artifacts.size(); i++) {
			std::cout << "   "<< splitted_artifacts[i] << '\n';
		}

		return 1;
	}
	/* Python standard library function lookup i.e. manp setattr */
	else {
		/* Loading the entire txt documentation that has been pre-converted to
		   raw string format to the combined_documentation_string */
		std::string combined_documentation_string =
		#include "combined_txt_header.h"
		;

		/* load the combined_documentation_string into the cpp_modules_map
		    key = module/filename, value = start and end string pos of respective documentation */
		std::map<std::string, std::vector<int> > cpp_modules_map = mappify(combined_documentation_string);

		/* load the functions list from the include folder
		   This raw string literal txt file is created by the generate_doc_artifacts_list bash script */
		std::string doc_artifacts_list =
		#include "doc_artifacts_list.txt"
		;
		SpellingCorrector functions_corrector; // creating an instance of the spelling functions_corrector model
		functions_corrector.load(doc_artifacts_list);
		std::string query_module_input(argv[1]);  // num of arg check already done by validate_cmd_line_input()

		if (cpp_modules_map.find(query_module_input) == cpp_modules_map.end()) { // the queried function does not exist in doc
			std::cerr << "\033[31;1mERROR:\033[0m " << query_module_input
			          << " is not documented in the artifacts list. ";
			/* if an unrecognized function/module was entered, attempt to make a guess */
			std::string correct(functions_corrector.correct(query_module_input));
			if (correct != "") std::cout << "Did you mean: \033[32;1m" << correct << "\033[0m";
			std::cout << "\n       For help: manp -h" << '\n';
			return 1;
		} else {                                          // user query exists in doc
			query_output = combined_documentation_string.substr(
				cpp_modules_map.find(query_module_input)->second.at(0),
				cpp_modules_map.find(query_module_input)->second.at(1));
		}

		std::size_t found = query_output.find_first_of('\n')+1; // Trim the "func_name" from func modules
		query_output = query_output.substr(found);

		return 0; // further processing with less required
	}
	std::cerr << "\033[31;1mERROR:\033[0m Control should never reach here\n";
	return 0;
}

/*  function to map the python default module names to their respective documentation
    The module names themselves are the key, the value is the start and end pos of the doc part
    for the respective module in the python_modules_string
    The delimiter for key and values is CPP_MAP_KEY_VALUE_DELIM while
    the delimiter for different modules is CPP_MAP_MODULE_DELIM */
std::map<std::string, std::vector<int> > mappify(std::string const& python_doc_string) {
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

/* function to verify the number of cmd line args
    return -1 for failure and 0 for success */
int validate_cmd_line_input(int argc, char const*argv[]) {
	if (argc != 2) return -1;
	return 0;
}

/* Signal Handler for SIGSEGV, segmentation faults */
void sigsegv_handler(int sig_num) {
	/* Reset handler to catch SIGSEGV next time */
	signal(SIGSEGV, sigsegv_handler);
	std::cerr << "\033[31;1mERROR:\033[0m Invalid Input" << "\n";
	std::cerr << "\033[32;1mUsage:\033[0m manp [PYTHON_STD_FUNCTION_NAME] or manp -m [PYTHON_MODULE_NAME]" << '\n';
	std::cout << "       For help: manp -h" << '\n';
	fflush(stdout);
	exit(1);
}

/* Signal Handler for SIGABRT, abort traps */
void sigabrt_handler(int sig_num) {
	/* Reset handler to catch SIGABRT next time */
	signal(SIGABRT, sigabrt_handler);
	std::cerr << "\033[31;1mERROR:\033[0m Invalid Input" << "\n";
	std::cerr << "\033[32;1mUsage:\033[0m manp [PYTHON_STD_FUNCTION_NAME] or manp -m [PYTHON_MODULE_NAME]" << '\n';
	std::cout << "       For help: manp -h" << '\n';
	fflush(stdout);
	exit(1);
}
