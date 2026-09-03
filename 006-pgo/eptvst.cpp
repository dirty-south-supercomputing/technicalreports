#include "pgotypes.h"

// produce data file of energy vs turns
int main(void){
  printf("E,T,atype\n");
  for(unsigned i = 0 ; i < ATTACKCOUNT ; ++i){
    const attack *a = attacks[i];
    if(a->energytrain < 0){
      continue;
    }
    printf("%d,%d,%s\n", a->energytrain, a->turns, a->type != TYPECOUNT ? tname_capitalized(a->type) : "Normal");
  }
  return EXIT_SUCCESS;
}
