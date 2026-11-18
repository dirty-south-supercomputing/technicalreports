#include "pgotypes.h"

// produce data file of energy vs turns
int main(void){
  std::cout << "E,T,atype,name" << std::endl;
  for(auto it = attacks_begin() ; it != attacks_end() ; ++it){
    const attack *a = *it;
    if(a->energytrain <= 0){
      continue;
    }
    std::cout << a->energytrain << ',' << a->turns << ',';
    auto tstr = a->type != TYPECOUNT ? tname_capitalized(a->type) : "Normal";
    std::cout << tstr << ',' << a->name;
    std::cout << std::endl;
  }
  return EXIT_SUCCESS;
}
