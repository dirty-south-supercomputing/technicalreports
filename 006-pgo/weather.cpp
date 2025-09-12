#include <stdio.h>
#include "pgotypes.h"

int main(void){
  printf("\\centering\\begin{tabular}{lcl|lcl}");
  printf("Weather & Icons & Types & Weather & Icons & Types\\\\\n");
  printf("\\Midrule\n");
  for(unsigned w = 0 ; w < static_cast<unsigned>(WEATHERCOUNT) ; ++w){
    printf("%s & \\includegraphics[height=1em,keepaspectratio]{images/%s.png}", WNames[w], WNames[w]);
    if(WSNames[w]){
      printf("\\includegraphics[height=1em,keepaspectratio]{images/%s.png}", WSNames[w]);
    }
    printf(" & ");
    bool printed = false;
    for(unsigned t = 0 ; t < static_cast<unsigned>(TYPECOUNT) ; ++t){
      if(wboosts[t] == w){
        print_type(static_cast<pgo_types_e>(t));
        printf(" ");
        printed = true;
      }
    }
    if(!printed){
      printf("None");
    }
    if(w % 2){
      printf("\\\\\n");
    }else{
      printf(" & ");
    }
  }
  printf("\\end{tabular}\\caption{Weather-boosted types\\label{table:weather}}");
}
