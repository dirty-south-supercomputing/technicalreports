#include <stdio.h>
#include "pgotypes.h"

int main(void){
  printf("\\centering\\begin{tabular}{lcl|lcl}");
  printf("Weather & Icons & Types & Weather & Icons & Types\\\\\n");
  printf("\\Midrule\n");
  for(unsigned w = 0 ; w < static_cast<unsigned>(WEATHERCOUNT) ; ++w){
    printf("%s & \\includegraphics[height=1em,keepaspectratio]{images/", WNames[w]);
    label_string(WNames[w]);
    printf("-icon.png}");
    if(WSNames[w]){
      printf("\\includegraphics[height=1em,keepaspectratio]{images/");
      label_string(WSNames[w]);
      printf("-icon.png}");
    }
    printf(" & ");
    bool printed = false;
    for(pgo_types_e t = TYPESTART ; t < TYPECOUNT ; ++t){
      if(type_weather_boost(t) == w){
        print_type(t);
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
