#!/bin/bash
rm -f *.json
rm -f  partial_output.txt

g++ daedalusToJSONID.cpp -g -o daedalusToJSONID
files=(outputselfmadePDF outputempty outputbikes outputnature outputnature2 PDF100pgs PDF200pgs PDF200pgs2 PDF400pgs PDF1000pgs)
pdf_attack_files=(11)
for j in "${pdf_attack_files[@]}"
do
   echo $j: 
   #./daedalusToJSONID $j.txt #for files
   ./daedalusToJSONID PDF_attack_input/$j.txt 
   echo "" 
done


