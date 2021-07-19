#!/bin/bash
rm -f ./parser ./checkopentags > /dev/null
make clean  > /dev/null
make   > /dev/null
make clean  > /dev/null
files=(outputbikes outputempty outputnature outputnature2 outputselfmadePDF PDF100pgs PDF200pgs PDF200pgs2 PDF400pgs PDF1000pgs)
files2=(outputbikes outputempty outputnature outputnature2 outputselfmadePDF PDF100pgs)

TIMEFORMAT=%R
for j in "${files[@]}"
do
   echo -n $j.json:" " 
   time  ./interpreter ./pdfInput/$j.json ./SurfaceSpec/PageTreeSpec__Target.txt > ./Output/PageTreeSpec/$j-output.txt  
done

unset TIMEFORMAT



# ./pdfInput/outputbikes.json ./SurfaceSpec/PageTreeSpec__Target.txt > ./Output/PageTreeSpec/outputbikes-output.txt  

