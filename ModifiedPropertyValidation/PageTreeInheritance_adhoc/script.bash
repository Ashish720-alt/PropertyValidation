#!/bin/bash
g++ daedalusToJSON.cpp -o daedalusToJSON
g++ PageTreeInheritance.cpp -o PageTreeInheritance
files=(outputselfmadePDF outputempty outputbikes outputnature outputnature2 PDF100pgs PDF200pgs PDF200pgs2 PDF400pgs PDF1000pgs)

for j in "${files[@]}"
do
   echo $j.txt: 
i=1
./daedalusToJSON $j.txt
    while [ $i -le 5 ]
    do
        ./PageTreeInheritance JSONoutput.json  
        echo "" 
        i=`expr $i + 1`
    done
echo "" 
done

rm -f daedalusToJSON PageTreeInheritance JSONoutput.json 



