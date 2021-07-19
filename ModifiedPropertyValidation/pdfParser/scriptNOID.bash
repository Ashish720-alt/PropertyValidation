#!/bin/bash
rm -f *.json
g++ daedalusToJSON.cpp -o daedalusToJSON
files=(outputselfmadePDF outputempty outputbikes outputnature outputnature2 PDF100pgs PDF200pgs PDF200pgs2 PDF400pgs PDF1000pgs)

for j in "${files[@]}"
do
   echo $j.txt: 
   ./daedalusToJSON $j.txt
   echo "" 
done

rm -f daedalusToJSON  



