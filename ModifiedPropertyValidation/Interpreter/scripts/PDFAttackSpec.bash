#!/bin/bash
#rm -f ./interpreter > /dev/null
make clean  > /dev/null
make   > /dev/null
make clean  > /dev/null
files=(11 12 13)

TIMEFORMAT=%R
for j in "${files[@]}"
do
   echo -n $j.json:" " 
   time  ./interpreter ./pdfAttackInput/$j.json ./SurfaceSpec/PDFAttackSpec1.txt > ./Output/PDFAttackSpec/$j-output.txt  
   #time  ./interpreter ./pdfInput/$j.json ./SurfaceSpec/PDFAttackSpec2.txt > ./Output/PDFAttackSpec/$j-output.txt  
done

unset TIMEFORMAT




#./pdfAttackInput/11.json ./SurfaceSpec/PDFAttackSpec1.txt > ./Output/PDFAttackSpec/11-output.txt  
