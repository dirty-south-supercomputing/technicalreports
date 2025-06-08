#include "pgotypes.c"
#include <memory>
#include <cstdio>
#include <cstring>
#include <cstdlib>

static int cmpatk(const void* va1, const void* va2){
  auto a1 = static_cast<const attack*>(va1);
  auto a2 = static_cast<const attack*>(va2);
  return a1->turns < a2->turns ? -1
    : a1->turns > a2->turns ? 1
    : a1->powertrain < a2->powertrain ? -1
    : a1->powertrain > a2->powertrain ? 1
    : a1->energytrain < a2->energytrain ? -1
    : a1->energytrain > a2->energytrain ? 1
    : 0;
}

void print_latex_table(const attack* as, unsigned ccount){
  printf("\\begin{center}\n");
  printf("\\begin{longtable}{llrrrr}\n");
  printf("Attack & Type & Power & Energy & Turns\\\\\n");
  printf("\\Midrule\\\\\n");
  for(unsigned c = 0 ; c < ccount ; ++c){
    const attack* a = &as[c];
    printf("%s & %s & %u & %d & %u\\\\\n",
        a->name, TNames[a->type], a->powertrain, a->energytrain, a->turns);
  }
  printf("\\caption{Fast attacks}\n");
  printf("\\label{table:fastattacks}\n");
  printf("\\end{longtable}\n");
  printf("\\end{center}\n");
}

// emit table of fast attacks by duration x energy x power
int main(void){
  const size_t acount = sizeof(attacks) / sizeof(*attacks);
  auto fast = std::make_unique<attack[]>(acount);
  unsigned fcount = 0;
  for(unsigned i = 0 ; i < sizeof(attacks) / sizeof(*attacks) ; ++i){
    const attack* a = attacks[i];
    if(a->energytrain >= 0){
      memcpy(&fast[fcount], a, sizeof(*a));
      ++fcount;
    }
  }
  qsort(fast.get(), fcount, sizeof(*fast.get()), cmpatk);
  print_latex_table(fast.get(), fcount);
  return EXIT_SUCCESS;
}
