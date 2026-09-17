#include "pgotypes.h"

// produce data file of energy vs turns
int main(void){
  printf("E,T,atype\n");
  for(auto it = attacks_begin() ; it != attacks_end() ; ++it){
    const attack *a = *it;
    if(a->energytrain < 0){
      continue;
    }
    printf("%d,%d,%s\n", a->energytrain, a->turns, a->type != TYPECOUNT ? tname_capitalized(a->type) : "Normal");
  }
  return EXIT_SUCCESS;
}
