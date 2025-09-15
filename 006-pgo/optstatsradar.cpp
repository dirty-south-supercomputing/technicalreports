#include "pgotypes.h"

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << std::endl;
  exit(EXIT_FAILURE);
}

static float
statsget_atk(const stats *s){
  return roundf(s->effa);
}

static float
statsget_def(const stats *s){
  return roundf(s->effd);
}

static float
statsget_mhp(const stats *s){
  return s->mhp;
}

static int
write_cpceil(int cpceil, const char *pfx, int(*fxn)(const void*, const void*), float(*getfxn)(const stats *)){
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
      std::cout << ss->s->name << "\t" << getfxn(ss) << "\t" << pfx << cpceil << std::endl;
    }
  }
  return 0;
}

// generate an R-format file with all possible values of a stat bounded by
// some CP for all pokémon
int main(int argc, const char **argv){
  const char *argv0 = argv[0];
  if(argc != 1){
    usage(argv0);
  }
  write_cpceil(GLCPCAP, "atk", statscmp_atk, statsget_atk);
  write_cpceil(ULCPCAP, "atk", statscmp_atk, statsget_atk);
  write_cpceil(GLCPCAP, "def", statscmp_def, statsget_def);
  write_cpceil(ULCPCAP, "def", statscmp_def, statsget_def);
  write_cpceil(GLCPCAP, "mhp", statscmp_mhp, statsget_mhp);
  write_cpceil(ULCPCAP, "mhp", statscmp_mhp, statsget_mhp);
  return EXIT_SUCCESS;
}
