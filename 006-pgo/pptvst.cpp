#include "pgotypes.h"

// produce CSV of power vs turns for fast attacks
int main(void){
  printf("P,T,type\n");
  for(unsigned i = 0 ; i < ATTACKCOUNT ; ++i){
    const attack *a = attacks[i];
    if(a->energytrain < 0){
      continue;
    }
    printf("%d,%d,%d\n", a->powertrain, a->turns, a->type);
  }
  return EXIT_SUCCESS;
}
