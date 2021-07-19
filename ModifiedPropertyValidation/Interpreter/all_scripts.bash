#!/bin/bash
scripts=(HTMLSpecHeadelmrefonce.bash  HTMLSpecIDUnique.bash  HTMLSpec_Pcannotnest.bash  HTMLSpecTDandTR.bash  PageTreeSpec.bash  SVGOrderedSpec.bash  SVGSpecRefsinDefs.bash SVGSpecUseCircularity.bash)
remainingHTMLscripts=(HTMLSpec_Pcannotnest.bash  HTMLSpecTDandTR.bash)

for j in "${remainingHTMLscripts[@]}"
do
   echo $j; 
   ./scripts/$j
   echo "" 
done
