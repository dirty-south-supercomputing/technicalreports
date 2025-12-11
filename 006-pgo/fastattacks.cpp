#include "pgotypes.h"
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

static int cmpatkraid(const void* va1, const void* va2){
  auto a1 = static_cast<const attack*>(va1);
  auto a2 = static_cast<const attack*>(va2);
  return a1->animdur < a2->animdur ? -1
    : a1->animdur > a2->animdur ? 1
    : a1->powerraid < a2->powerraid ? -1
    : a1->powerraid > a2->powerraid ? 1
    : a1->energyraid < a2->energyraid ? -1
    : a1->energyraid > a2->energyraid ? 1
    : 0;
}

void print_latex_table(const attack* as, unsigned ccount, bool raidvalues){
  printf("\\begin{center}\n");
  printf("\\footnotesize\n");
  printf("\\begin{longtable}{lrrrrrrrrr}\n");
  printf("Attack & T & E & \\EPT{} & P & $\\cdot\\frac{6}{5}$ & \\PPT{} & $\\cdot\\frac{6}{5}$ & Pop(STAB)\\\\\n");
  printf("\\Midrule\n");
  printf("\\endhead\n");
  for(unsigned c = 0 ; c < ccount ; ++c){
    const attack* a = &as[c];
    float ppt = a->powertrain / (float)a->turns;
    float ept = a->energytrain / (float)a->turns;
    print_fast_attack_rowcolor(a);
    if(a->type != TYPECOUNT){
      print_type(a->type);
    }else{
      printf("\\hspace{1em}");
    }
    unsigned pop, popstab;
    pop = learner_count(a, &popstab);
    printf(" %s & %u & %d & %.3g & %u & %g & %.3g & %g & %u(%u)\\\\\n",
           a->name, a->turns, a->energytrain, ept,
           a->powertrain, (a->powertrain * 6.0) / 5,
           ppt, (a->powertrain * 6.0) / (a->turns * 5.0),
           pop, popstab);
  }
  const char* typestr = raidvalues ? "Mx1" : "3x3";
  printf("\\caption[Fast attacks, \\PPT{}, and \\EPT{} (%s stats)]", typestr);
  printf("{Fast attacks, \\PPT{}, and \\EPT{} (%s stats. Attacks with \\EPT{}·\\PPT{} ≥ 9 are highlighted in green)\\label{table:fastattacks%s}}\n", typestr, typestr);
  printf("\\end{longtable}\n");
  printf("\\end{center}\n");
}

static void
usage(const char* argv0){
  std::cerr << "usage: " << argv0 << " [ -r ]" << std::endl;
  std::cerr << " -r: use raid values, not trainer battles" << std::endl;
  exit(EXIT_FAILURE);
}

// emit table of fast attacks by duration x energy x power
int main(int argc, const char** argv){
  bool raidvalues = false; // use Mx1 rather than 3x3 values
  if(argc != 1){
    if(argc != 2){
      usage(argv[0]);
    }
    if(strcmp(argv[1], "-r")){
      std::cerr << "invalid argument: " << argv[1] << std::endl;
      usage(argv[0]);
    }
    raidvalues = true;
  }
  const size_t acount = sizeof(attacks) / sizeof(*attacks);
  auto fast = std::make_unique<attack[]>(acount);
  unsigned fcount = 0;
  for(unsigned i = 0 ; i < acount ; ++i){
    const attack* a = attacks[i];
    if(a->energytrain > 0){
      memcpy(&fast[fcount], a, sizeof(*a));
      ++fcount;
    }
  }
  if(raidvalues){
    qsort(fast.get(), fcount, sizeof(*fast.get()), cmpatkraid);
  }else{
    qsort(fast.get(), fcount, sizeof(*fast.get()), cmpatk);
  }
  print_latex_table(fast.get(), fcount, raidvalues);
  return EXIT_SUCCESS;
}
