#include "pgotypes.cpp"
#include <memory>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define MAXTURNS 5

// receives TYPECOUNT 5-vectors of attack counts per duration per type
// gooddpt and goodept are counts of strong moves, vectors of TYPECOUNT size
void print_latex_table(const unsigned *acounts, const unsigned *gooddpt, const unsigned *goodept){
  printf("\\begin{table}[ht]\n");
  printf("\\centering\\footnotesize");
  printf("\\begin{tabular}{lrrrrrrr}\n");
  printf("Type & 1 & 2 & 3 & 4 & 5 & PPT>3 & EPT>3\\\\\n");
  printf("\\Midrule\n");
  for(unsigned e = 0 ; e < TYPECOUNT ; ++e){
    printf("%s", TNames[e]);
    for(int i = 0 ; i < MAXTURNS ; ++i){
      if(acounts[i]){
        printf(" & %u", acounts[i]);
      }else{
        printf(" &");
      }
    }
    printf(" & %u & %u", gooddpt[e], goodept[e]);
    printf("\\\\\n");
    acounts += 5;
  }
  printf("\\end{tabular}\n");
  printf("\\caption{Fast attacks by duration by type}\n");
  printf("\\label{table:attacktypes}\n");
  printf("\\end{table}\n");
}

// emit table of fast attacks by duration x energy x power
int main(void){
  static unsigned acounts[TYPECOUNT * MAXTURNS] = {};
  static unsigned gooddpt[TYPECOUNT] = {};
  static unsigned goodept[TYPECOUNT] = {};
  for(unsigned i = 0 ; i < ATTACKCOUNT ; ++i){
    const attack* a = attacks[i];
    if(a->energytrain >= 0){
      ++acounts[a->type * MAXTURNS + (a->turns - 1)];
      if(a->energytrain / a->turns >= 3){
        ++goodept[a->type];
      }
      if(a->powertrain / a->turns >= 3){
        ++gooddpt[a->type];
      }
    }
  }
  print_latex_table(acounts, gooddpt, goodept);
  return EXIT_SUCCESS;
}
