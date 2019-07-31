#!/bin/bash

# Check number of args supplied to bash script
if [ $# -eq 0 ]
  then
    echo -e <&2 "\033[31;1mERROR:\033[0m Data directory source was not supplied"
    echo -e "       \033[32;1mUsage:\033[0m ./generate_doc_artifacts_list.sh [SOURCE_DATA_DIR]"
    exit 1
fi

SOURCE_DATA_DIRECTORY=$1 # first cmd line argument is the source data directory
EXTENSION_TO_SEARCH_FOR='*.txt'
FILE_LIST=doc_artifacts_list.txt

echo "R\"=~=~=(" > $FILE_LIST
IFS=$'\n' # temporarily setting internal field separator to \n
set -f
# loop through the data folder to find all txt files
for file in $(find $SOURCE_DATA_DIRECTORY -name $EXTENSION_TO_SEARCH_FOR)
do
    echo "$(basename ${file%.*} )" >> $FILE_LIST
done
unset IFS
set +f
echo ")=~=~=\"" >> $FILE_LIST
mv $FILE_LIST include           # moves the combined txt header file inside include folder
