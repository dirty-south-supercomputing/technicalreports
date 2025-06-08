#include "pgotypes.c"
#include <stdio.h>
#include <stdlib.h>

// emit table of fast attacks by duration x energy x power
int main(void){
  for(unsigned i = 0 ; i < sizeof(attacks) / sizeof(*attacks) ; ++i){
    const attack* a = attacks[i];
    if(a->energytrain >= 0){
      printf("%s %s %u %d %u\n", a->name, TNames[a->type], a->powertrain, a->energytrain, a->turns);
    }
  }
  return EXIT_SUCCESS;
}
