#!/bin/bash
TEXTS=modules_list.txt

echo "R\"=~=~=(" > $TEXTS
# loop through the data folder to find all txt files
for file in data/modules/*.txt
do
    echo "$(basename ${file%.*} )" >> $TEXTS
done
echo ")=~=~=\"" >> $TEXTS
mv $TEXTS include           # moves the combined txt header file inside include folder
