#!/bin/bash

SOURCE_DATA_DIRECTORY=data
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
