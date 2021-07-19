#!/bin/bash
rm -f ./parser ./checkopentags
make clean
make parser
make checkopentags
make clean
htmlfiles=(floatingimage testing Wikipedia300lines WikipediaAmericanRevolution WikipediaPokemon WikipediaPythagorousTheorem WikipediaVPG)
htmlfiles2=( floatingimage )
for j in "${htmlfiles[@]}"
do
   echo $j.json: 
   echo ""
   ./checkopentags ./HTMLfiles/$j.html 	
   ./parser ./HTMLfiles/$j.html > ./OutputHTMLJSONfiles/$j.json
   echo "" 
done
svgfiles=(acid cartman digitalCamera gallardo sansBold steps svg2009)
svgfiles2=(  )
for j in "${svgfiles[@]}"
do
   echo $j.json: 
   echo ""
   ./checkopentags ./SVGfiles/$j.svg	
   ./parser ./SVGfiles/$j.svg > ./OutputSVGJSONfiles/$j.json 
   echo "" 
done



