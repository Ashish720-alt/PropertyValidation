#!/bin/bash
rm -f specConverter
make clean
make
make clean
files=(HTMLSpecHeadelmrefonce HTMLSpecIDUnique  HTMLSpec_Pcannotnest  HTMLSpecTDandTR  PageTreeSpec  SVGOrderedSpec  SVGSpecRefsinDefs  SVGSpecUseCircularity )
files2=(HTMLSpecHeadelmrefonce)
for j in "${files[@]}"
do
   echo $j: 
   echo ""
   ./specConverter ./SurfaceSpec/${j}__Target.txt  
   # diff ./SurfaceSpec/${j}__Target__internal.txt ./InternalSpec/${j}__internal.txt
   echo "" 
done


# ./SurfaceSpec/SVGOrderedSpec__Target.txt  