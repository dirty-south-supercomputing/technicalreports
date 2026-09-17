#include "pgotypes.h"
#include <memory>
#include <cstring>
#include <cstdio>
#include <cstdlib>

static bool cmpatk(const attack* a1, const attack* a2){
  // remember, energytrain is negative
  float ppere1 = ((float)a1->powertrain) / a1->energytrain;
  float ppere2 = ((float)a2->powertrain) / a2->energytrain;
  if(ppere1 > ppere2){
    return true;
  }else if(ppere1 == ppere2){
    if(a1->energytrain > a2->energytrain){
      return true;
    }
  }
  return false;
}

void print_latex_table(const std::vector<const attack*>& as){
  printf("\\begin{center}\n");
  printf("\\footnotesize\n");
  // we removed raw P and P * 6/5 because that can be derived from P/E and E
  printf("\\begin{longtable}{lrrrrr}\n");
  printf("Attack & E & $\\frac{P}{E}$ & $\\cdot\\frac{6}{5}$ & Buff & Pop(STAB)\\\\\n");
  printf("\\Midrule\n");
  printf("\\endhead\n");
  unsigned shadnormals; // number of shadows with normal type
  auto shadows = shadow_count(&shadnormals); // number of shadows
  for(const auto a : as){
    if(a->type != TYPECOUNT){
      print_type(a->type);
    }
    printf(" %s & %d & %.3g & %.3g &", a->name,
            //a->powertrain, (a->powertrain * 6.0) / 5,
            -a->energytrain,
            a->powertrain / (float)-a->energytrain,
            (a->powertrain * 6.0) / (-a->energytrain * 5.0));
    summarize_buffs(a);
    printf(" & ");
    // we should still print numbers for these, based on the total number of shadows FIXME
    if(!strcmp(a->name, "Frustration")){
      printf("%u(%u)", shadows, shadnormals);
      printf("\\footnote{Frustration is known by default to all Shadow Pokémon (unfortunately).}");
    }else if(!strcmp(a->name, "Return")){
      printf("%u(%u)", shadows, shadnormals);
      printf("\\footnote{Return is known by default to all Purified Pokémon (it's not terrible with STAB).}");
    }else{
      unsigned popstab;
      auto pop = learner_count(a, &popstab);
      printf("%u(%u)", pop, popstab);
    }
    puts("\\\\");
  }
  printf("\\caption{Charged attacks and power-per-energy (3x3 stats)\\label{table:chargedattacks}}\n");
  printf("\\end{longtable}\n");
  printf("\\end{center}\n");
}

int main(void){
  std::vector<const attack*> charged{};
  for(auto it = attacks_begin() ; it != attacks_end() ; ++it){
    const attack* a = *it;
    if(a->energytrain < 0){
      charged.push_back(a);
    }
  }
  std::sort(charged.begin(), charged.end(), cmpatk);
  print_latex_table(charged);
  return EXIT_SUCCESS;
}
