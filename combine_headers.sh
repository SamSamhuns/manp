#!/bin/bash
MODULES_HEADER=combined_modules_txt_include.h
FUNCTIONS_HEADER=combined_std_functions_txt_include.h

#################################
# For combined_modules_txt_include
#################################
echo "#ifndef COMBINED_MODULES_H" > $MODULES_HEADER
echo "#define COMBINED_MODULES_H" >> $MODULES_HEADER

# loop through the data folder to find all txt files
for file in data/modules/*.txt
do
    echo "#include \"../$file\"" >> $MODULES_HEADER

done
echo "#endif" >> $MODULES_HEADER
mv $MODULES_HEADER include           # moves the combined txt header file inside include folder

#################################
# For combined_std_functions_txt_include
#################################
echo "#ifndef COMBINED_STD_FUNCTIONS_H" > $FUNCTIONS_HEADER
echo "#define COMBINED_STD_FUNCTIONS_H" >> $FUNCTIONS_HEADER

# loop through the data folder to find all txt files
for file in data/std_functions/*.txt
do
    echo "#include \"../$file\"" >> $FUNCTIONS_HEADER

done
echo "#endif" >> $FUNCTIONS_HEADER
mv $FUNCTIONS_HEADER include           # moves the combined txt header file inside include folder
