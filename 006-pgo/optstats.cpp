#include "pgotypes.h"

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " atk|def|mhp cpbound" << std::endl;
  exit(EXIT_FAILURE);
}

// generate an R-format file with all possible values of a stat bounded by
// some CP for all pokémon
int main(int argc, const char **argv){
  int (*fxn)(const void *, const void *);
  const char *argv0 = argv[0];
  if(argc != 3){
    usage(argv0);
  }
  if(strcmp(argv[1], "atk") == 0){
    fxn = statscmp_atk;
  }else if(strcmp(argv[1], "def") == 0){
    fxn = statscmp_def;
  }else if(strcmp(argv[1], "mhp") == 0){
    fxn = statscmp_mhp;
  }else{
    std::cerr << "invalid stat: " << argv[1] << std::endl;
    usage(argv0);
  }
  int cpceil = atoi(argv[2]);
  if(cpceil <= 0){
    std::cerr << "invalid cpbound: " << argv[2] << std::endl;
    usage(argv0);
  }
  std::vector<stats *> allstats;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const species &s = sdex[u];
    unsigned ivcount;
    allstats.push_back(order_ivs(&s, cpceil, fxn, &ivcount));
  }
  return EXIT_SUCCESS;
}
