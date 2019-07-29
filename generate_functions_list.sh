#!/bin/bash
TEXTS=functions_list.txt

echo "R\"=~=~=(" > $TEXTS
# loop through the data folder to find all txt files
for file in data/std_functions/*.txt
do
    echo "$(basename ${file%.*} )" >> $TEXTS
done
echo ")=~=~=\"" >> $TEXTS
mv $TEXTS include           # moves the combined txt header file inside include folder
