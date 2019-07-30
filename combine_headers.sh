#!/bin/bash

SOURCE_DATA_DIRECTORY=data
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
