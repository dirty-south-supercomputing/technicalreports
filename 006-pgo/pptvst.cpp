#include "pgotypes.h"

// produce CSV of power vs turns for fast attacks
int main(void){
  printf("P,T,atype\n");
  for(auto it = attacks_begin() ; it != attacks_end() ; ++it){
    const attack *a = *it;
    if(a->energytrain < 0){
      continue;
    }
    printf("%d,%d,%s\n", a->powertrain, a->turns,
            a->type != TYPECOUNT ? tname_capitalized(a->type) : "Normal");
  }
  return EXIT_SUCCESS;
}
