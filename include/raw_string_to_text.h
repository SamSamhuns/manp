#ifndef RAW_STRING_TO_FILE_H // Makes sure that raw_string_to_text.h is not included twice
#define RAW_STRING_TO_FILE_H

#include <vector>
#include <string>
#include <cerrno>
#include <fstream>
#include <stdio.h>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "common.h"

/* function to convert raw string literal files to original files of file_extension
    enclosed by the START_DELIMs and END_DELIMs */
int raw_string_text_to_file(std::string &file_extension, std::string &filename);

/* function to recursively find files with file_extension */
int recursively_find_files_with_ext(std::string &file_extension, std::string &dirpath);

#endif
