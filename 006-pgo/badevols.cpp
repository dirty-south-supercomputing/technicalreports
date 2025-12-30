#include "pgotypes.h"

static int
check_bad_evol(const species& s){
  // FIXME
  return 0;
}

// list undesirable evolutions
int main(void){
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    check_bad_evol(sdex[u]);
  }
  return EXIT_SUCCESS;
}
