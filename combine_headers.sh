#!/bin/bash
HEADER=combined_txt_include.h
echo "#ifndef COMBINED_H" > $HEADER
echo "#define COMBINED_H" >> $HEADER

# loop through the data folder to find all txt files
for file in data/modules/*.txt
do
    echo "#include \"../$file\"" >> $HEADER

done
echo "#endif" >> $HEADER
mv $HEADER include           # moves the combined txt header file inside include folder
