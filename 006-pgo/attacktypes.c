#include "pgotypes.c"
#include <memory>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define MAXTURNS 5

// receives TYPECOUNT 5-vectors of attack counts per duration per type
void print_latex_table(const unsigned *acounts){
  printf("\\begin{table}[ht]\n");
  printf("\\begin{center}\n");
  printf("\\footnotesize\n");
  printf("\\begin{tabular}{lrrrrr}\n");
  printf("Type & 1 & 2 & 3 & 4 & 5\\\\\n");
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
    printf("\\\\\n");
    acounts += 5;
  }
  printf("\\end{tabular}\n");
  printf("\\end{center}\n");
  printf("\\caption{Fast attacks by duration by type}\n");
  printf("\\label{table:attacktypes}\n");
  printf("\\end{table}\n");
}

// emit table of fast attacks by duration x energy x power
int main(void){
  static unsigned acounts[TYPECOUNT * MAXTURNS] = {};
  const size_t acount = sizeof(attacks) / sizeof(*attacks);
  for(unsigned i = 0 ; i < acount ; ++i){
    const attack* a = attacks[i];
    if(a->energytrain >= 0){
      ++acounts[a->type * MAXTURNS + (a->turns - 1)];
    }
  }
  print_latex_table(acounts);
  return EXIT_SUCCESS;
}
