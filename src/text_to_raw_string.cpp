#include <vector>
#include <string>
#include <cerrno>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <csignal>
#include <iostream>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common.h"

/* function to convert files of given extension to raw string text literal files
    enclosed by the START_DELIM and END_DELIMs */
int file_to_raw_string_text(std::string &file_extension, std::string &file_path, std::string &target_path);

/* function to recursively find files given their extension */
int recursively_find_files_with_ext(std::string &file_extension, std::string &source_path, std::string &target_path);

/* function to write the contents of the line_string_vector to the write_target_path
    On success returns 0, if failed returns -1 */
int write_string_vector_to_file(std::string write_target_path,
                                std::vector<std::string> &line_string_vector);

/* Signal Handler for SIGSEGV, segmentation faults */
void sigsegv_handler(int sig_num);

/* Signal Handler for SIGABRT, abort traps */
void sigabrt_handler(int sig_num);

int main(int argc, const char *argv[]) {
	signal(SIGSEGV, sigsegv_handler);  // signal handler for segmentation faults
	signal(SIGABRT, sigabrt_handler);  // signal ahndler for abort traps

	if (argc < 4) {
		std::cerr << "\033[32;1mUsage:\033[0m";
		std::cerr << "./file_to_raw_string_text [FILE_EXTENSIONS_SEP_BY_SPACE...] [SOURCE_PATH] [TARGET_PATH]\n";
		return -1;
	}

	std::string source_path(argv[argc-2]); // second last arg = source_path
	std::string target_path(argv[argc-1]); // final arg = target_path
	system(("mkdir -p "+target_path).c_str());

	// loop through all the file extensions mentioned (starting from 1st not the oth argument)
	for (size_t i = 1; i < argc-2; i++) {
		std::string file_extension(argv[i]);
		if (file_extension.front() != '.') {file_extension='.'+file_extension;}
		recursively_find_files_with_ext(file_extension, source_path, target_path);
	}

	return 0;
}

/* function to recursively find files given their extension */
int recursively_find_files_with_ext(std::string &file_extension, std::string &source_path, std::string &target_path) {
	struct stat path_stat;

	/* Check if the source_path is a file or folder */
	if (stat(source_path.c_str(), &path_stat) == 0) {
		// source_path is a folder
		if (S_ISDIR(path_stat.st_mode)) {
			/* Add '/' to end of source_path if not already there */
			if (source_path[source_path.length()-1] != '/') source_path += '/';
			if (DEBUG) std::cout << source_path << " is a folder\n";

			DIR* dirPtr;
			dirPtr = opendir(source_path.c_str());
			if (dirPtr == nullptr) {
				std::cerr << "\033[31;1mERROR:\033[0m Unable to open" << source_path \
				          << " : " << errno << std::endl;
				return -1;
			} else {  // Open the source_path directory
				struct dirent *dirReadPointer; // Pointer for reading the DIR stream
				dirReadPointer = readdir(dirPtr);

				/* loop through all files inside the directory */
				while (dirReadPointer) {
					std::string file_path = dirReadPointer->d_name;
					if (file_path != std::string(".") && file_path != std::string("..") &&
					    file_path != std::string(".DS_Store")) {
						std::string pathToFile = source_path+file_path;

						switch (dirReadPointer->d_type) {
						case DT_REG:     // if dirReadPointer points to a regular file
							file_to_raw_string_text(file_extension, pathToFile, target_path);
							break;
						case DT_DIR:     // if dirReadPointer points to a folder
							recursively_find_files_with_ext(file_extension, pathToFile, target_path);
							break;
						default:         // if dirReadPointer is neither file/folder
							if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m " << pathToFile << \
								    " is not a regular file or folder\n";
							break;
						}
					}
					dirReadPointer = readdir(dirPtr);
				}
				closedir(dirPtr);
			}
		}
		// source_path is a regular file
		else if (S_ISREG(path_stat.st_mode)) {
			if (DEBUG) std::cout << source_path << " is a file\n";
			file_to_raw_string_text(file_extension, source_path, target_path);
		}
		else {
			if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m " << source_path
				                 << " is not a regular file or folder\n";
			return -1;
		}
	}
	else {
		if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m in opening " << source_path << " : " \
			                 << errno << '\n';
		return -1;
	}
	return 0;
}

/* function to convert files of given extension to raw string text literal files
    enclosed by the START_DELIM and END_DELIMs */
int file_to_raw_string_text(std::string &file_extension, std::string &file_path, std::string &target_path) {
	std::fstream in_ptr;
	std::vector <std::string> line_string_vector;
	std::string inString, module_name;

	/* Check if the file is a file of the given extension */
	std::size_t dot_found = file_path.find_last_of(".");
	if (dot_found == std::string::npos) {
		if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m " << file_path << " is not a "<<file_extension<<" file\n";
		return -1;
	}

	if ((file_path.substr(dot_found)) != std::string(file_extension) ) {
		if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m " << file_path << " is not a "<<file_extension<<" file\n";
		return -1;
	}

	in_ptr.open(file_path, std::ios::in);
	if (!in_ptr.is_open()) {
		if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m Cannot open " << file_path << "\n";
		return -1;
	}

	/* Get the file_path to use the C++ map key data struct
	    the filname will be the unique key for the c++ map */
	std::size_t slash_found = file_path.find_last_of("/");
	// if a slash '/' was not found in the file_path
	if (slash_found == std::string::npos) {
		module_name = file_path;
	}
	// if the file path has a slash '/'
	else {
		module_name = file_path.substr(slash_found+1);
	}

	if (target_path.back() != '/') {target_path += '/';}
	std::string write_target_path = target_path + module_name;
	/* remove file extension from file_path and add the delimiter for C++ map */
	module_name = module_name.substr(0, module_name.size()-file_extension.size());
	std::string module_name_delimited = std::string(START_DELIM) + module_name + std::string(CPP_MAP_KEY_VALUE_DELIM);

	/* Checking if file is already in desired raw string literal format */
	std::getline(in_ptr, inString);
	if (inString == module_name_delimited) {
		if (DEBUG) std::cout << file_path << " already in raw literal string format.\n";

		/* The source file will be written to write_target_path without any modifications */
		line_string_vector.push_back(inString);
		/* Read contents of file_path line by line and store in line_string_vector */
		while (std::getline(in_ptr, inString)) {
			line_string_vector.push_back(inString);
		}

		// the write_string_vector_to_file() func writes to the write_target_path and returns 0 on success
		if (write_string_vector_to_file(write_target_path, line_string_vector) == -1) {
			if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m Cannot write to "
				                 << write_target_path << "\n";
			return -2;
		}

		if (DEBUG) std::cout << file_path << " successfully written to "
			                 << write_target_path << " in a raw string literal format\n";
		in_ptr.close();
		return 0;
	}

	line_string_vector.push_back(module_name_delimited);     // first elem of line_string_vector
    line_string_vector.push_back("\""+module_name+"\"");     // Add module_name with quotes to file start
	line_string_vector.push_back(inString);

	/* Read contents of file_path line by line and store in line_string_vector */
	while (std::getline(in_ptr, inString)) {
		line_string_vector.push_back(inString);
	}
	in_ptr.close();

	// Add the c++ map delimiter for different modules and custom delimiter for end of raw string literal
	line_string_vector.push_back(std::string(CPP_MAP_MODULE_DELIM)+'\n'+std::string(END_DELIM));

	// the write_string_vector_to_file() func writes to the write_target_path and returns 0 on success
	if (write_string_vector_to_file(write_target_path, line_string_vector) == -1) {
		if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m Cannot write to "
			                 << write_target_path << "\n";
		return -2;
	}

	if (DEBUG) std::cout << file_path << " successfully written to "
		                 << write_target_path << " in a raw string literal format\n";
	return 0;
}

/* function to write the contents of the line_string_vector to the write_target_path
    On success returns 0, if failed returns -1 */
int write_string_vector_to_file(std::string write_target_path,
                                std::vector<std::string> &line_string_vector) {
	std::fstream out_ptr;

	/* Open the output write_target_path file */
	out_ptr.open(write_target_path, std::ios::out);
	if (!out_ptr.is_open()) {
		if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m Cannot open "
			                 << write_target_path << "\n";
		return -1;
	}

	/* Save contents of line_string_vector to the same file_path */
	for (size_t i = 0; i < line_string_vector.size(); i++) {
		out_ptr << line_string_vector[i] << "\n";
	}

	out_ptr.close();
	return 0;
}

/* Signal Handler for SIGSEGV, segmentation faults */
void sigsegv_handler(int sig_num) {
	/* Reset handler to catch SIGSEGV next time */
	signal(SIGSEGV, sigsegv_handler);
	std::cerr << "\033[31;1mERROR:\033[0m segmentation fault" << "\n";
	fflush(stdout);
	exit(1);
}

/* Signal Handler for SIGABRT, abort traps */
void sigabrt_handler(int sig_num) {
	/* Reset handler to catch SIGABRT next time */
	signal(SIGABRT, sigabrt_handler);
	std::cerr << "\033[31;1mERROR:\033[0m abort trap" << "\n";
	fflush(stdout);
	exit(1);
}
