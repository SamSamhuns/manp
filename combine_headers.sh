#!/bin/bash

# Check number of args supplied to bash script
if [ $# -eq 0 ]
  then
    echo -e >&2 "\033[31;1mERROR:\033[0m Data directory source was not supplied"
    echo -e "       \033[32;1mUsage:\033[0m ./combine_headers.sh [SOURCE_DATA_DIR]"
    exit 1
fi

SOURCE_DATA_DIRECTORY=$1 # first cmd line argument is the source data directory
EXTENSION_TO_SEARCH_FOR='*.txt'
COMBINED_HEADER_FILE=combined_txt_header.h

echo "#ifndef COMBINED_HEADERS_TXT_H" > $COMBINED_HEADER_FILE
echo "#define COMBINED_HEADERS_TXT_H" >> $COMBINED_HEADER_FILE
# loop through the SOURCE_DATA_DIRECTORY finding all files with the EXTENSION_TO_SEARCH_FOR extn
IFS=$'\n' # temporarily setting internal field separator to \n
set -f
for file in $(find $SOURCE_DATA_DIRECTORY -name $EXTENSION_TO_SEARCH_FOR)
do
    echo "#include \"../$file\"" >> $COMBINED_HEADER_FILE
done
unset IFS
set +f
echo "#endif" >> $COMBINED_HEADER_FILE
mv $COMBINED_HEADER_FILE include
