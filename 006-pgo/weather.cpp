#include <stdio.h>
#include "pgotypes.cpp"

int main(void){
  printf("\\centering\\begin{tabular}{lcl}");
  printf("Weather & Icon & Types\\\\\n");
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
        printf("%s%s", printed ? ", " : "", TNames[t]);
        printed = true;
      }
    }
    if(!printed){
      printf("None");
    }
    printf("\\\\\n");
  }
  printf("\\end{tabular}\\caption{Weather-boosted types}\\label{table:weather}");
}
