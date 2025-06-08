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
  printf("\\begin{table}\n");
  printf("\\begin{center}\n");
  printf("\\begin{tabular}{llrrr}\n");
  printf("Attack & Type & Power & Energy\\\\\n");
  printf("\\Midrule\\\\\n");
  int c = ccount;
  while(--c >= 0){
    const attack* a = &as[c];
    printf("%s & %s & %u & %d\\\\\n", a->name, TNames[a->type], a->powertrain, -a->energytrain);
  }
  printf("\\end{tabular}\n");
  printf("\\end{center}\n");
  printf("\\caption{Charged attacks}\n");
  printf("\\label{table:charged}\n");
  printf("\\end{table}\n");
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
