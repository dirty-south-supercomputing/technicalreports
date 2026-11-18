#include "pgotypes.h"

// produce CSV of power vs turns for fast attacks
int main(void){
  printf("P,T,atype,name\n");
  for(auto it = attacks_begin() ; it != attacks_end() ; ++it){
    const attack *a = *it;
    if(a->energytrain < 0){
      continue;
    }
    printf("%d,%d,%s,%s\n", a->powertrain, a->turns,
            a->type != TYPECOUNT ? tname_capitalized(a->type) : "Normal",
            a->name.c_str());
  }
  return EXIT_SUCCESS;
}
