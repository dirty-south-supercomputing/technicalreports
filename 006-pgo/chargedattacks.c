#include "pgotypes.c"
#include <memory>
#include <cstring>
#include <cstdio>
#include <cstdlib>

static int cmpatk(const void* va1, const void* va2){
  auto a1 = static_cast<const attack*>(va1);
  auto a2 = static_cast<const attack*>(va2);
  return a1->energytrain < a2->energytrain ? -1 : a1->energytrain == a2->energytrain ? 0 : 1;
}

void print_latex_table(const attack* as, unsigned ccount){
  printf("\\begin{center}\n");
  printf("\\begin{longtable}{lrrrrrr}\n");
  printf("Attack & P & $\\times\\frac{6}{5}$ & E & $\\frac{P}{E}$ & $\\times\\frac{6}{5}$ & Pop\\\\\n");
  printf("\\Midrule\\\\\n");
  int c = ccount;
  while(--c >= 0){
    const attack* a = &as[c];
    if(a->type != TYPECOUNT){
      printf("\\includegraphics[width=1em,height=1em]{images/%s.png} ", TNames[a->type]);
    }
    printf("%s & %u & %g & %d & %.2f & %.3g & %u\\\\\n", a->name,
           a->powertrain, (a->powertrain * 6.0) / 5,
            -a->energytrain,
            a->powertrain / (float)-a->energytrain,
            (a->powertrain * 6.0) / (-a->energytrain * 5.0),
            learner_count(a));
  }
  printf("\\caption{Charged attacks and power-per-energy}\n");
  printf("\\label{table:chargedattacks}\n");
  printf("\\end{longtable}\n");
  printf("\\end{center}\n");
}

// emit table of fast attacks by duration x energy x power
int main(void){
  const size_t acount = sizeof(attacks) / sizeof(*attacks);
  auto charged = std::make_unique<attack[]>(acount);
  unsigned ccount = 0;
  for(unsigned i = 0 ; i < acount ; ++i){
    const attack* a = attacks[i];
    if(a->energytrain < 0){
      memcpy(&charged[ccount], a, sizeof(*a));
      ++ccount;
    }
  }
  qsort(charged.get(), ccount, sizeof(*charged.get()), cmpatk);
  print_latex_table(charged.get(), ccount);
  return EXIT_SUCCESS;
}
