#include <vector>
#include <string>
#include <cerrno>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <csignal>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common.h"

/* function to convert files of given extension to raw string text literal files
    enclosed by the START_DELIM and END_DELIMs */
int file_to_raw_string_text(std::string &file_extension, std::string &filename);

/* function to recursively find files given their extension */
int recursively_find_files_with_ext(std::string &file_extension, std::string &dirpath);

/* Signal Handler for SIGSEGV, segmentation faults */
void sigsegv_handler(int sig_num);

/* Signal Handler for SIGABRT, abort traps */
void sigabrt_handler(int sig_num);

int main(int argc, const char *argv[]) {
	signal(SIGSEGV, sigsegv_handler);  // signal handler for segmentation faults
	signal(SIGABRT, sigabrt_handler);  // signal ahndler for abort traps

	if (argc != 3) {
		std::cerr <<
		    "\033[32;1mUsage:\033[0m ./file_to_raw_string_text [TARGET_FILE_EXTENSION] [PATH_TO_DIR_OR_FILE]\n";
		return -1;
	}

	std::string file_extension(argv[1]);
	std::string dirpath(argv[2]);
	if (file_extension.front() != '.') {file_extension='.'+file_extension;}
	recursively_find_files_with_ext(file_extension, dirpath);

	return 0;
}

/* function to recursively find files given their extension */
int recursively_find_files_with_ext(std::string &file_extension, std::string &dirpath) {
	struct stat path_stat;

	/* Check if the dirpath is a file or folder */
	if (stat(dirpath.c_str(), &path_stat) == 0) {
		// dirpath is a folder
		if (S_ISDIR(path_stat.st_mode)) {
			/* Add '/' to end of dirpath if not already there */
			if (dirpath[dirpath.length()-1] != '/') dirpath += '/';
			if (DEBUG) std::cout << dirpath << " is a folder\n";

			DIR* dirPtr;
			dirPtr = opendir(dirpath.c_str());
			if (dirPtr == nullptr) {
				std::cerr << "\033[31;1mERROR:\033[0m Unable to open" << dirpath \
				          << " : " << errno << std::endl;
				return -1;
			} else {  // Open the dirpath directory
				struct dirent *dirReadPointer; // Pointer for reading the DIR stream
				dirReadPointer = readdir(dirPtr);

				/* loop through all files inside the directory */
				while (dirReadPointer) {
					std::string filename = dirReadPointer->d_name;
					if (filename != std::string(".") && filename != std::string("..") &&
					    filename != std::string(".DS_Store")) {
						std::string pathToFile = dirpath+filename;

						switch (dirReadPointer->d_type) {
						case DT_REG:     // if dirReadPointer points to a regular file
							file_to_raw_string_text(file_extension, pathToFile);
							break;
						case DT_DIR:     // if dirReadPointer points to a folder
							recursively_find_files_with_ext(file_extension, pathToFile);
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
		// dirpath is a regular file
		else if (S_ISREG(path_stat.st_mode)) {
			if (DEBUG) std::cout << dirpath << " is a file\n";
			file_to_raw_string_text(file_extension, dirpath);
		}
		else {
			if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m " << dirpath << " is not a regular file or folder\n";
			return -1;
		}
	}
	else {
		if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m in opening " << dirpath << " : " \
			                 << errno << '\n';
		return -1;
	}
	return 0;
}

/* function to convert files of given extension to raw string text literal files
    enclosed by the START_DELIM and END_DELIMs */
int file_to_raw_string_text(std::string &file_extension, std::string &filename) {
	std::fstream in_ptr, out_ptr;
	std::vector <std::string> line_string_vector;
	std::string inString, module_name;

	/* Check if the file is a file of the given extension */
	std::size_t dot_found = filename.find_last_of(".");
	if (dot_found == std::string::npos) {
		if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m " << filename << " is not a "<<file_extension<<" file\n";
		return -1;
	}
	// std::cout << "FILENAME IS "<< filename << ",  " << dot_found << '\n';
	if ((filename.substr(dot_found)) != std::string(file_extension) ) {
		if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m " << filename << " is not a "<<file_extension<<" file\n";
		return -1;
	}

	in_ptr.open(filename, std::ios::in);
	if (!in_ptr.is_open()) {
		if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m Cannot open " << filename << "\n";
		return -1;
	}

	/* Get the filename to use the C++ map key data struct
	    the filname will be the unique key for the c++ map */
	std::size_t slash_found = filename.find_last_of("/");
	// if a slash '/' was not found in the filename
	if (slash_found == std::string::npos) {
		module_name = filename;
	}
	// if the file path has a slash '/'
	else {
		module_name = filename.substr(slash_found+1);
	}
	/* remove .txt extension from filename and add the delimiter for C++ map */
	module_name = module_name.substr(0, module_name.size()-4);
	module_name = std::string(START_DELIM) + module_name + std::string(CPP_MAP_KEY_VALUE_DELIM);

	/* Checking if file is already in desired raw string literal format */
	std::getline(in_ptr, inString);
	if (inString == module_name) {
		if (DEBUG) std::cerr << filename << " already in raw literal string format.\n";
		return -2;
	}

	line_string_vector.push_back(module_name);     // first elem of line_string_vector
	line_string_vector.push_back(inString);

	/* Read contents of filename line by line and store in line_string_vector */
	while (std::getline(in_ptr, inString)) {
		line_string_vector.push_back(inString);
	}

	// Add the c++ map delimiter for different modules and custom delimiter for end of raw string literal
	line_string_vector.push_back(std::string(CPP_MAP_MODULE_DELIM)+'\n'+std::string(END_DELIM));

	out_ptr.open(filename, std::ios::out);
	if (!out_ptr.is_open()) {
		if (DEBUG) std::cerr << "\033[31;1mERROR:\033[0m Cannot open " << filename << "\n";
		return -1;
	}

	/* Save contents of line_string_vector to the same filename */
	for (size_t i = 0; i < line_string_vector.size(); i++) {
		out_ptr << line_string_vector[i] << "\n";
	}

	in_ptr.close();
	out_ptr.close();
	if (DEBUG) std::cerr << filename << " successfully converted to raw string literal\n";
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
