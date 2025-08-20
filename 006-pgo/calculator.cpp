#include "pgotypes.cpp"
#include <climits>

static void usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " form iv | cp [iv]" << std::endl;
  std::cerr << " form iv: dump config at all levels for this iv" << std::endl;
  std::cerr << " form cp [iv]: reverse configurations for this cp and optionally iv" << std::endl;
  exit(EXIT_FAILURE);
}

static int lex_cp(const char *arg){
  char *endptr;
  unsigned cp = strtoul(arg, &endptr, 10);
  while(*endptr){
    if(!isspace(*endptr)){
      std::cerr << "invalid characters following cp: " << endptr << std::endl;
      return -1;
    }
    ++endptr;
  }
  if(cp < MINCP || cp > INT_MAX){
    std::cerr << "invalid cp: " << cp << std::endl;
    return -1;
  }
  return cp;
}

static stats *solve_hlevel(const species *s, int cp, int ia, int id, int is){
  unsigned hlevel = 1;
  do{
    int ccp = calccp(s->atk + ia, s->def + id, s->sta + is, hlevel);
    if(ccp == cp){
      unsigned half;
      unsigned l = halflevel_to_level(hlevel, &half);
      std::cout << "matched " << cp << ": " << ia << "/" << id << "/" << is << " " << l;
      if(half){
        std::cout << ".5";
      }
      auto effa = calc_eff_a(s->atk + ia, hlevel, false);
      auto effd = calc_eff_d(s->def + id, hlevel, false);
      auto hp = calc_mhp(s->sta + is, hlevel);
      std::cout << " EffA: " << effa << " EffD: " << effd << " HP: " << hp;
      std::cout << " gm: " << calc_fit(effa, effd, hp);
      std::cout << " am: " << calc_avg(effa, effd, hp);
      std::cout << std::endl;
      break;
    }
  }while(++hlevel <= MAX_HALFLEVEL);
  if(hlevel > MAX_HALFLEVEL){
    return nullptr;
  }
  auto st = new stats(s, hlevel, ia, id, is);
  return st;
}

static stats *reverse_ivs_level(const species *s, int cp, int *ia, int *id, int *is){
  if(cp < 0){
    for(unsigned hlevel = 1 ; hlevel <= MAX_HALFLEVEL ; ++hlevel){
      unsigned half;
      unsigned l = halflevel_to_level(hlevel, &half);
      if(l < 10){
        std::cout << " ";
      }
      if(half){
        std::cout << l;
        std::cout << ".5";
      }else{
        std::cout << "  " << l;
      }
      cp = calccp(s->atk + *ia, s->def + *id, s->sta + *is, hlevel);
      auto gmean = calc_fit(calc_eff_a(s->atk + *ia, hlevel, false),
                            calc_eff_d(s->def + *id, hlevel, false),
                            calc_mhp(s->sta + *is, hlevel));
      std::cout << ": " << cp << " (" << gmean << ") " << "\t";
      if(hlevel % 4 == 0){
        std::cout << std::endl;
      }
    }
    std::cout << std::endl;
    return nullptr;
  }
  if(*ia >= 0 && *id >= 0 && *is >= 0){
    auto st = solve_hlevel(s, cp, *ia, *id, *is);
    return st;
  }
  stats *sols = nullptr;
  for(*ia = 0 ; *ia <= MAXIVELEM ; ++*ia){
    for(*id = 0 ; *id <= MAXIVELEM ; ++*id){
      for(*is = 0 ; *is <= MAXIVELEM ; ++*is){
        auto st = solve_hlevel(s, cp, *ia, *id, *is);
        if(st){
          st->next = sols;
          sols = st;
        }
      }
    }
  }
  return sols;
}

static int lex_iv(const char *arg, int *ia, int *id, int *is){
  int r;
  if((r = sscanf(arg, " %u-%u-%u", ia, id, is)) != 3){
    std::cerr << "expected N-N-N, got " << arg << std::endl;
    return -1;
  }
  return 0;
}

int main(int argc, const char **argv){
  if(argc < 3 || argc > 4){
    usage(argv[0]);
  }
  const species *s = lookup_species(argv[1]);
  if(!s){
    std::cerr << "couldn't look up form " << argv[1] << std::endl;
    usage(argv[0]);
  }
  std::cout << argv[1] << " atk: " << s->atk << " def: " << s->def << " sta: " << s->sta << std::endl;
  int ia = -1, id = -1, is = -1, cp = -1;
  if(argc == 3){
    if(lex_iv(argv[2], &ia, &id, &is)){
      if((cp = lex_cp(argv[2])) < 0){
        usage(argv[0]);
      }
    }
  }else if(argc == 4){
    if((cp = lex_cp(argv[2])) < 0){
      usage(argv[0]);
    }
    if(lex_iv(argv[3], &ia, &id, &is)){
      usage(argv[0]);
    }
  }
  if(cp > 0){
    std::cout << "cp: " << cp << " ";
  }
  std::cout << "ia: " << ia << " id: " << id << " is: " << is << std::endl;
  auto st = reverse_ivs_level(s, cp, &ia, &id, &is);
  if(!st && cp > 0){
    std::cerr << "couldn't match cp " << cp << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
