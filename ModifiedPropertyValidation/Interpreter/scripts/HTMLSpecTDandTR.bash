#!/bin/bash
rm -f ./compiler > /dev/null
make clean > /dev/null
make > /dev/null 
make clean > /dev/null
files=(floatingimage testing Wikipedia300lines WikipediaAmericanRevolution WikipediaPokemon WikipediaPythagorousTheorem WikipediaVPG)
files2=(WikipediaVPG)

TIMEFORMAT=%R
for j in "${files[@]}"
do
   echo -n $j.json:" " 
   time ./interpreter ./HTMLInput/$j.json ./SurfaceSpec/HTMLSpecTDandTR__Target.txt > ./Output/HTMLSpecTDandTR/$j-output.txt
done

unset TIMEFORMAT


