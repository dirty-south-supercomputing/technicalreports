#include "pgotypes.h"

// produce data file of DPE vs energy
int main(void){
  printf("P,E,atype\n");
  for(unsigned i = 0 ; i < ATTACKCOUNT ; ++i){
    const attack *a = attacks[i];
    if(a->energytrain >= 0){
      continue;
    }
    printf("%d,%d,%s\n", a->powertrain, -a->energytrain, TNames[a->type]);
  }
  return EXIT_SUCCESS;
}
