#include "simul.h"

static void
autofight(const species &su, const species &sopp){
  simulstate sstate = {};
  // FIXME select attacks
  // FIXME set up pmons
  results r;
  r.wins[0] = r.wins[1] = r.ties = 0;
  simul(&sstate, &r);
}

int main(void){
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const species &su = sdex[u];
    for(unsigned opp = 0 ; opp < SPECIESCOUNT ; ++opp){
      const species &sopp = sdex[opp];
      autofight(su, sopp);
    }
  }
  return EXIT_SUCCESS;
}
