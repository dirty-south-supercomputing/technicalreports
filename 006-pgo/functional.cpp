#include "pgotypes.h"

static void
functional_hundos(int cplimit){
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const auto s = sdex[u];
    std::vector<const species*> evols;
    if(get_persistent_evolutions(&s, evols)){ // skip non-terminal evolutions
      continue;
    }
    // find the maximum level subject to cplimit for 15-15-15
    unsigned atk = s.atk + 15;
    unsigned def = s.def + 15;
    unsigned sta = s.sta + 15;
    int cp;
    unsigned maxlevel = maxlevel_cp_bounded(atk, def, sta, cplimit, &cp);
    // get the maximum hp
    unsigned mhp = calc_mhp(sta, maxlevel);
    unsigned fmhp;
    do{
      --sta;
      // dropping the sta might theoretically give us a higher max level
      unsigned fmaxlevel = maxlevel_cp_bounded(atk, def, sta, cplimit, &cp);
      fmhp = calc_mhp(sta, fmaxlevel);
      if(fmhp >= mhp){
        std::cout << s.name << " 15-15-" << (sta - s.sta) << std::endl;
      }
if(fmaxlevel != maxlevel){
  std::cerr << s.name << " FMAXLEVEL " << fmaxlevel << " MAXLEVEL " << maxlevel << std::endl;
  if(fmhp >= mhp){
    std::cerr << s.name << " SUPERFUNCTIONAL! " << (sta - s.sta) << std::endl;
  }
}
    }while(sta != s.sta);
  }
}

// generate list of functional hundos for the (optional) cp limit
int main(int argc, char **argv){
  auto cplimit = 0;
  if(argc >= 2){
    if(argc > 2){
      // FIXME usage();
      return EXIT_FAILURE;
    }
    cplimit = atoi(argv[1]);
    if(cplimit <= 0){
      std::cerr << "illegal cplimit: " << argv[1] << std::endl;
      // FIXME usage();
      return EXIT_FAILURE;
    }
  }
  functional_hundos(cplimit);
  return EXIT_SUCCESS;
}
