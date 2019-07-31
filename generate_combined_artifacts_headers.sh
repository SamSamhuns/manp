#!/bin/bash

# Check number of args supplied to bash script
if [ $# -lt 2 ]
  then
    echo -e <&2 "\033[31;1mERROR:\033[0m Data dir or Target file extension(s) were not supplied"
    echo -e "       \033[32;1mUsage:\033[0m ./generate_doc_artifacts_list.sh [DATA_DIR] [FILE_EXTENSIONS_SEP_BY_SPACE...]"
    exit 1
fi

ARTIFACTS_LIST_FILE='doc_artifacts_list.txt'
COMBINED_HEADER_FILE='combined_txt_header.h'
DATA_DIR_TO_CHK=$1           # first cmd line argument is the source data directory
if [ "${DATA_DIR_TO_CHK: -1}" == "/" ]
    then
        DATA_DIR_TO_CHK="${DATA_DIR_TO_CHK%?}"
fi

echo "R\"=~=~=(" > $ARTIFACTS_LIST_FILE

echo "#ifndef COMBINED_HEADERS_TXT_H" > $COMBINED_HEADER_FILE
echo "#define COMBINED_HEADERS_TXT_H" >> $COMBINED_HEADER_FILE

IFS=$'\n' # temporarily setting internal field separator to \n
set -f

for ((i = 2; i <= $#; i++ )) # loop starting from 2nd cmd arg as the 1st arg is the DATA_DIR
do
  if [ "${!i:0:1}" != "." ]  # if the first char is not a .
    then
        EXTENSION_TO_SEARCH_FOR='*.'${!i}
    else
        EXTENSION_TO_SEARCH_FOR='*'${!i}
  fi
  # echo "THE EXTENSION IS " $EXTENSION_TO_SEARCH_FOR
  # loop through the data folder to find all files specified by the extension for multiple file types
  for file in $(find $DATA_DIR_TO_CHK -name $EXTENSION_TO_SEARCH_FOR)
  do
      echo "$(basename ${file%.*} )" >> $ARTIFACTS_LIST_FILE
      echo "#include \"../$file\"" >> $COMBINED_HEADER_FILE
  done
done

unset IFS
set +f

echo "#endif" >> $COMBINED_HEADER_FILE
echo ")=~=~=\"" >> $ARTIFACTS_LIST_FILE

# move both files to the include folder
mv $COMBINED_HEADER_FILE include
mv $ARTIFACTS_LIST_FILE include
