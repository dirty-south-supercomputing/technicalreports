#include "pgotypes.cpp"
#include <climits>

static void usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " form cp [iv]" << std::endl;
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
  int cp = lex_cp(argv[2]);
  if(cp < 0){
    usage(argv[0]);
  }
  int ia = -1, id = -1, is = -1;
  if(argc > 3){
    if(lex_iv(argv[3], &ia, &id, &is)){
      usage(argv[0]);
    }
    std::cout << "cp: " << cp << " ia: " << ia << " id: " << id << " is: " << is << std::endl;
  }else{
    std::cout << "cp: " << cp << std::endl;
  }
  auto st = reverse_ivs_level(s, cp, &ia, &id, &is);
  if(!st){
    std::cerr << "couldn't match cp " << cp << std::endl;
    return EXIT_FAILURE;
  }
  // FIXME walk st
  return EXIT_SUCCESS;
}
