#!/bin/bash
rm -f ./compiler > /dev/null
make clean > /dev/null
make > /dev/null
make clean > /dev/null
files=(acid cartman digitalCamera gallardo sansBold steps svg2009)
files2=( svg2009 )

TIMEFORMAT=%R
for j in "${files[@]}"
do
   echo -n $j.json:" " 
   time ./interpreter ./SVGInput/$j.json ./SurfaceSpec/SVGOrderedSpec__Target.txt > ./Output/SVGOrderedSpec/$j-output.txt
done

unset TIMEFORMAT

