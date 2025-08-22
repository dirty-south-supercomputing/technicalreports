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
      std::cout << " gm: " << calc_gmean(effa, effd, hp);
      std::cout << " am: " << calc_amean(effa, effd, hp);
      std::cout << std::endl;
      break;
    }
  }while(++hlevel <= MAX_HALFLEVEL);
  if(hlevel > MAX_HALFLEVEL){
    return nullptr;
  }
  auto st = new stats(s, hlevel, ia, id, is, false);
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
      auto gmean = calc_gmean(calc_eff_a(s->atk + *ia, hlevel, false),
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

static int
lex_iv(const char *arg, int *ia, int *id, int *is){
  int r;
  if((r = sscanf(arg, " %u-%u-%u", ia, id, is)) != 3){
    std::cerr << "expected N-N-N, got " << arg << std::endl;
    return -1;
  }
  return 0;
}

static constexpr unsigned IVLEVVEC =
  (MAXIVELEM + 1) * (MAXIVELEM + 1) * (MAXIVELEM + 1);

static void
order_ivs_internal(const species *s, int cpceil, stats *svec, bool shadow){
  unsigned idx = 0;
  for(int iva = 0 ; iva < 16 ; ++iva){
    for(int ivd = 0 ; ivd < 16 ; ++ivd){
      for(int ivs = 0 ; ivs < 16 ; ++ivs){
        auto &st = svec[idx];
        const unsigned moda = s->atk + iva;
        const unsigned modd = s->def + ivd;
        const unsigned mods = s->sta + ivs;
        st.hlevel = maxlevel_cp_bounded(moda, modd, mods, cpceil, &st.cp);
        st.s = s;
        st.effa = calc_eff_a(moda, st.hlevel, shadow);
        st.effd = calc_eff_d(modd, st.hlevel, shadow);
        st.mhp = calc_mhp(mods, st.hlevel);
        st.ia = iva;
        st.id = ivd;
        st.is = ivs;
        st.shadow = shadow;
        st.geommean = calc_pok_gmean(&st);
        st.average = calc_pok_amean(&st);
        ++idx;
      }
    }
  }
}

// generate the stats for each of 4,096 possible IVs at their maximum level
// subject to the cpceiling (-1 for no ceiling), ordered according to fitfxn.
static stats *
order_ivs(const species *s, int cpceil, int(*cmpfxn)(const void*, const void*),
          unsigned *vcount){
  *vcount = IVLEVVEC;
  if(s->shadow){
    *vcount *= 2;
  }
  stats *svec = new stats[*vcount];
  order_ivs_internal(s, cpceil, svec, false);
  if(s->shadow){
    order_ivs_internal(s, cpceil, svec + IVLEVVEC, true);
  }
  qsort(svec, *vcount, sizeof(*svec), cmpfxn);
  return svec;
}

static int
statscmp_gmean(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  return st1->geommean < st2->geommean ? -1 :
          st1->geommean > st2->geommean ? 1 : 0;
}

static int
statscmp_amean(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  return st1->average < st2->average ? -1 :
          st1->average > st2->average ? 1 : 0;
}

static int
statscmp_atk(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  return st1->effa < st2->effa ? -1 :
          st1->effa > st2->effa ? 1 : 0;
}

static int
statscmp_def(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  return st1->effd < st2->effd ? -1 :
          st1->effd > st2->effd ? 1 : 0;
}

static int
statscmp_mhp(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  return st1->mhp < st2->mhp ? -1 :
          st1->mhp > st2->mhp ? 1 : 0;
}

static int
statscmp_bulk(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  const float b1 = st1->mhp * st1->effd;
  const float b2 = st2->mhp * st2->effd;
  return b1 < b2 ? -1 : b1 > b2 ? 1 : 0;
}

static void
summarize_stat(const stats &st){
  std::cout << st.geommean << " " << st.average << " ";
  std::cout << st.cp;
  std::cout << " " << st.effa << " " << st.effd << " " << st.mhp << " ";
  unsigned half;
  unsigned l = halflevel_to_level(st.hlevel, &half);
  std::cout << "\\ivlev{" << st.ia << "}{" << st.id << "}{" << st.is << "}{" << l;
  if(half){
    std::cout << ".5";
  }
  std::cout << "}";
  if(st.shadow){
    std::cout << " \\shadow";
  }
  std::cout << std::endl;
}

static void
summarize_fxn(const species *s, int cpceil, const char *str,
              int(*cmpfxn)(const void*, const void*), int items){
  unsigned vcount;
  auto opts = order_ivs(s, cpceil, cmpfxn, &vcount);
  std::cout << str << std::endl;
  for(int i = 0 ; i < items ; ++i){
    const stats &st = opts[vcount - i - 1];
    summarize_stat(st);
  }
  std::cout << "..." << std::endl;
  for(int i = 0 ; i < items ; ++i){
    const stats &st = opts[items - i];
    summarize_stat(st);
  }
  std::cout << std::endl;
  delete[] opts;
}

// top n + bottom n for various fitness functions
static void
summarize(const species *s, int cpceil){
  constexpr int items = 4;
  summarize_fxn(s, cpceil, "Geometric mean", statscmp_gmean, items);
  summarize_fxn(s, cpceil, "Arithmetic mean", statscmp_amean, items);
  summarize_fxn(s, cpceil, "Attack", statscmp_atk, items);
  summarize_fxn(s, cpceil, "Defense", statscmp_def, items);
  summarize_fxn(s, cpceil, "MHP", statscmp_mhp, items);
  summarize_fxn(s, cpceil, "Bulk", statscmp_bulk, items);
}

int main(int argc, const char **argv){
  if(argc < 2 || argc > 4){
    usage(argv[0]);
  }
  const species *s = lookup_species(argv[1]);
  if(!s){
    std::cerr << "couldn't look up form " << argv[1] << std::endl;
    usage(argv[0]);
  }
  std::cout << argv[1] << " atk: " << s->atk << " def: " << s->def << " sta: " << s->sta << std::endl;
  int ia = -1, id = -1, is = -1, cp = -1;
  if(argc == 2){
    summarize(s, 1500);
    summarize(s, 2500);
    return EXIT_SUCCESS;
  }else if(argc == 3){
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
  for(const species *evol = s ; (s = get_persistent_evolution(evol)) ; evol = s){
    std::cout << "evolution: " << s->name << std::endl;
  }
  return EXIT_SUCCESS;
}
