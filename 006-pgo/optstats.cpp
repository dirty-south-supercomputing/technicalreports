#include "pgotypes.h"

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " atk|def|mhp cpbound" << std::endl;
  exit(EXIT_FAILURE);
}

static float
statsget_atk(const stats *s){
  return s->effa;
}

static float
statsget_def(const stats *s){
  return s->effd;
}

static float
statsget_mhp(const stats *s){
  return s->mhp;
}

// generate an R-format file with all possible values of a stat bounded by
// some CP for all pokémon
int main(int argc, const char **argv){
  int (*fxn)(const void *, const void *);
  float (*getfxn)(const stats *);
  const char *argv0 = argv[0];
  if(argc != 3){
    usage(argv0);
  }
  if(strcmp(argv[1], "atk") == 0){
    fxn = statscmp_atk;
    getfxn = statsget_atk;
  }else if(strcmp(argv[1], "def") == 0){
    fxn = statscmp_def;
    getfxn = statsget_def;
  }else if(strcmp(argv[1], "mhp") == 0){
    fxn = statscmp_mhp;
    getfxn = statsget_mhp;
  }else{
    std::cerr << "invalid stat: " << argv[1] << std::endl;
    usage(argv0);
  }
  int cpceil = atoi(argv[2]);
  if(cpceil <= 0){
    std::cerr << "invalid cpbound: " << argv[2] << std::endl;
    usage(argv0);
  }
  std::vector<stats*> allstats;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const species &s = sdex[u];
    if(strchr(s.name.c_str(), '\'')){ // FIXME messes up R's geom_density_ridges()/aes()
      continue;
    }
    if(get_persistent_evolution(&s)){
      continue;
    }
    unsigned ivcount;
    allstats.push_back(order_ivs(&s, cpceil, fxn, &ivcount));
    if(ivcount % IVLEVVEC){
      std::cerr << "unexpected vector count " << ivcount << std::endl;
      throw std::exception();
    }
  }
  // FIXME for now, sort by lowest value. ought use median probably.
  /*std::sort(allstats.begin(), allstats.end(), [](const stats *lhs, const stats *rhs){
        return std::min_element(lhs.begin(), lhs.end(), fxn)
            //[](const stats *lhs, const stats *rhs){ return lhs->mhp < rhs->mhp; })
               < std::min_element(rhs.begin(), lhs.begin(), fxn);
                   //[](const stats *lhs, const stats *rhs){ });
      });*/
  for(const stats *s : allstats){
    for(unsigned u = 0 ; u < IVLEVVEC ; ++u){
      const stats *ss = &s[u];
      std::cout << ss->s->name << "\t" << getfxn(ss) << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
