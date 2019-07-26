#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cerrno>
#include <stdio.h>
#include <cstddef>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common.h"
#define START_DELIMITER "R\"========("
#define END_DELIMITER ")========\""

/* function to convert raw string literal files to txt enclosed by the
    START_DELIMITERs and END_DELIMITERs */
int raw_string_to_text(std::string &filename);

/* function to recursively find txt files */
int recursively_find_txt_files(std::string &dirpath);

int main(int argc, const char *argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: ./raw_string_to_text <PATH_TO_DIR_OR_TXT_FILE>\n";
		return -1;
	}

	std::string dirpath(argv[1]);
	recursively_find_txt_files(dirpath);

	return 0;
}

/* function to recursively find txt files */
int recursively_find_txt_files(std::string &dirpath) {
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
				std::cerr << "ERROR: Unable to open" << dirpath \
				          << " : " << std::strerror(errno) << std::endl;
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
							raw_string_to_text(pathToFile);
							break;
						case DT_DIR:     // if dirReadPointer points to a folder
							recursively_find_txt_files(pathToFile);
							break;
						default:         // if dirReadPointer is neither file/folder
							if (DEBUG) std::cerr << pathToFile << \
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
			raw_string_to_text(dirpath);
		}
		else {
			if (DEBUG) std::cerr << dirpath << " is not a regular file or folder\n";
			return -1;
		}
	}
	else {
		if (DEBUG) std::cerr << "ERROR in opening " << dirpath << " : " \
			                 << std::strerror(errno) << '\n';
		return -1;
	}
	return 0;
}

/* function to convert raw string literal files to txt enclosed by the
    START_DELIMITERs and END_DELIMITERs */
int raw_string_to_text(std::string &filename) {
	std::fstream in_ptr, out_ptr;
	std::vector <std::string> line_string_vector;
	std::string inString;

	/* Check if the file is a txt file */
	std::size_t found = filename.find_last_of(".");
	if ((filename.substr(found+1)) != std::string("txt") ) {
		if (DEBUG) std::cerr << "ERROR: " << filename << " is not a txt file\n";
		return -1;
	}

	in_ptr.open(filename, std::ios::in);
	if (!in_ptr.is_open()) {
		if (DEBUG) std::cerr << "ERROR: Cannot open " << filename << "\n";
		return -1;
	}

	/* Checking if file is already in a normal text format */
	std::getline(in_ptr, inString);
	if (inString != START_DELIMITER) {
		if (DEBUG) std::cerr << filename << " not in raw string literal text format.\n";
		return -2;
	}
	int line_count = 1; // to count number of lines in filename
	line_string_vector.push_back(inString);

	/* Read contents of filename line by line and store in line_string_vector */
	while (std::getline(in_ptr, inString)) {
		line_string_vector.push_back(inString);
		line_count += 1;
	}

	if (line_string_vector[line_count-1] != END_DELIMITER) {
		if (DEBUG) std::cerr << filename << " not in raw string literal text format.\n";
		return -2;
	}

	out_ptr.open(filename, std::ios::out);
	if (!out_ptr.is_open()) {
		if (DEBUG) std::cerr << "ERROR: Cannot open " << filename << "\n";
		return -1;
	}

	/* Save contents of line_string_vector to the same filename */
	/* avodi adding the first and last strings which are the DELMITERS */
	for (size_t i = 1; i < (line_string_vector.size()-2); i++) {
		out_ptr << line_string_vector[i] << "\n";
	}

	in_ptr.close();
	out_ptr.close();
	if (DEBUG) std::cerr << filename << " successfully converted to normal text format.\n";
	return 0;
}
