#ifndef FILE_TO_RAW_STRING_H // Makes sure that text_to_raw_string.h is not included twice
#define FILE_TO_RAW_STRING_H

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

/* function to convert files of given extension to raw string text literal files
    enclosed by the START_DELIM and END_DELIMs */
int file_to_raw_string_text(std::string &file_extension, std::string &filename);

/* function to recursively find files given their extension */
int recursively_find_files_with_ext(std::string &file_extension, std::string &dirpath);

#endif
