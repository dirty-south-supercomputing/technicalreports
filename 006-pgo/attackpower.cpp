#include "pgotypes.h"

// table of all attacks at all possible strengths (-3..2, as applicable, with
// and without STAB)
int main(void){
  for(unsigned i = 0 ; i < ATTACKCOUNT ; ++i){
    const auto* a = attacks[i];
    auto p = a->powertrain;
    auto pstab = calc_stab(p);
    std::cout << a->name << " ";
    for(int te = -3 ; te < 3 ; ++te){
      // FIXME only show values which can apply
      auto pte = p * type_effectiveness_mult(te);
      auto ptestab = pstab * type_effectiveness_mult(te);
      std::cout << pte << " " << ptestab << " ";
    }
    std::cout << std::endl;
  }
  return EXIT_SUCCESS;
}
