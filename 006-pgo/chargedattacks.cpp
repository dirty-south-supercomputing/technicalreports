#include "pgotypes.cpp"
#include <memory>
#include <cstring>
#include <cstdio>
#include <cstdlib>

static int cmpatk(const void* va1, const void* va2){
  auto a1 = static_cast<const attack*>(va1);
  auto a2 = static_cast<const attack*>(va2);
  // remember, energytrain is negative
  float ppere1 = ((float)a1->powertrain) / a1->energytrain;
  float ppere2 = ((float)a2->powertrain) / a2->energytrain;
  return ppere1 < ppere2 ? -1
         : ppere1 > ppere2 ? 1
         : a1->energytrain < a2->energytrain ? -1
         : a1->energytrain > a2->energytrain ? 1
         : 0;
}

void print_latex_table(const attack* as, unsigned ccount){
  printf("\\begin{center}\n");
  printf("\\footnotesize\n");
  printf("\\begin{longtable}{lrrrrrrr}\n");
  printf("Attack & P & $\\cdot\\frac{6}{5}$ & E & $\\frac{P}{E}$ & $\\cdot\\frac{6}{5}$ & Buff & Pop\\\\\n");
  printf("\\Midrule\n");
  printf("\\endhead\n");
  int c = ccount;
  while(--c >= 0){
    const attack* a = &as[c];
    if(a->type != TYPECOUNT){
      print_type(a->type);
    }
    printf(" %s & %u & %g & %d & %.2f & %.3g &", a->name,
            a->powertrain, (a->powertrain * 6.0) / 5,
            -a->energytrain,
            a->powertrain / (float)-a->energytrain,
            (a->powertrain * 6.0) / (-a->energytrain * 5.0));
    summarize_buffs(a);
    printf(" & ");
    if(!strcmp(a->name, "Frustration")){
      printf("*\\footnote{Frustration is known by default to all Shadow Pokémon (unfortunately).}");
    }else if(!strcmp(a->name, "Return")){
      printf("*\\footnote{Return is known by default to all Purified Pokémon (it's mid at best).}");
    }else{
      printf("%u", learner_count(a));
    }
    puts("\\\\");
  }
  printf("\\caption{Charged attacks and power-per-energy (3x3 stats)\\label{table:chargedattacks}}\n");
  printf("\\end{longtable}\n");
  printf("\\end{center}\n");
}

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
