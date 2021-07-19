#!/bin/bash
rm -f ./compiler > /dev/null
make clean > /dev/null
make > /dev/null
make clean > /dev/null
files=(acid cartman digitalCamera gallardo sansBold steps svg2009)
files2=( acid )


TIMEFORMAT=%R
for j in "${files[@]}"
do
   echo -n $j.json:" " 
   time ./interpreter ./SVGInput/$j.json ./SurfaceSpec/SVGSpecUseCircularity__Target.txt > ./Output/SVGSpecUseCircularity/$j-output.txt 
done



unset TIMEFORMAT

# ./SVGInput/acid.json ./SurfaceSpec/SVGSpecUseCircularity__Target.txt > ./Output/SVGSpecUseCircularity/acid-output.txt 
