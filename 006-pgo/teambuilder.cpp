#include "pgotypes.cpp"
#include <getopt.h>

static constexpr auto GLCPCAP = 1500;
static constexpr auto ULCPCAP = 1500;

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " pokéspec [ -g | -u ] [ pokéspec... ]" << std::endl;
  std::cerr << "supply up to three pokéspexen" << std::endl;
  std::cerr << "-g: CP cap for Great League (" << GLCPCAP << ")" << std::endl;
  std::cerr << "-u: CP cap for Ultra League (" << ULCPCAP << ")" << std::endl;
  exit(EXIT_FAILURE);
}

struct Teambuild {
  pmon pmons[TEAMSIZE];
  unsigned cpbound;

  Teambuild() :
    cpbound(0)
  {
    for(unsigned i = 0 ; sizeof(pmons) < sizeof(*pmons) ; ++i){
      pmon &p = pmons[i];
      p.s.s = nullptr; // FIXME initialize remainder?
      p.fa = nullptr;
      p.ca1 = nullptr;
      p.ca2 = nullptr;
    }
  }

  Teambuild(unsigned Cpbound) :
    Teambuild()
  {
    cpbound = Cpbound;
  }

};

static void
summarize_ivlev(const stats &st){
  unsigned half;
  unsigned l = halflevel_to_level(st.hlevel, &half);
  std::cout << st.ia << "-" << st.id << "-" << st.is << " (" << l;
  if(half){
    std::cout << ".5";
  }
  std::cout << ") " << st.cp;
}

static float
statsget_gmean(const stats &st){
  return st.geommean;
}

static float
statsget_amean(const stats &st){
  return st.average;
}

static void
summarize_fxn(const pmon &p, int cpcap, const char *name,
              int(*cmpfxn)(const void*, const void*),
              float(*getfxn)(const stats &)){
  unsigned vcount;
  auto opts = order_ivs(p.s.s, cpcap, cmpfxn, &vcount);
  const stats &st = opts[vcount - 1];
  const auto max = getfxn(st);
  std::cout << " Top " << name << ": " << max << " ";
  summarize_ivlev(st);
  const auto ours = getfxn(p.s);
  std::cout << " ours: " << ours << " (" << (100 * ours / max) << "%)";
  std::cout << std::endl;
  delete[] opts;
}

static void
summarize_pmon(const Teambuild &tb, const pmon &p){
  std::cout << p.s.s->name;
  // FIXME if no IVs supplied, isolate best for CP limit (we currently require IVs+level spec)
  std::cout << " ATK: " << p.s.s->atk;
  std::cout << " DEF: " << p.s.s->def;
  std::cout << " STA: " << p.s.s->sta;
  std::cout << " EffA: " << p.s.effa;
  std::cout << " EffD: " << p.s.effd;
  std::cout << " MHP: " << p.s.mhp;
  std::cout << " Gmean: " << p.s.geommean;
  std::cout << " Amean: " << p.s.average;
  std::cout << std::endl;
  summarize_fxn(p, tb.cpbound, "Gmean", statscmp_gmean, statsget_gmean);
  summarize_fxn(p, tb.cpbound, "Amean", statscmp_amean, statsget_amean);
  // FIXME show effective stats relative to max configs for this mon+cpcap
  // FIXME run through all attack sets, profiling turns, target-independent power,
  //  and power delivered to typings
}

static void
set_cpcap(int *cpcap, unsigned newcap, const char *argv0){
  if(*cpcap >= 0){
    std::cerr << "can't provide cp cap twice" << std::endl;
    usage(argv0);
  }
  *cpcap = newcap;
}

int main(int argc, char **argv){
  setlocale(LC_ALL, "");
  const char *argv0 = *argv;
  int cpcap = -1;
  // FIXME allow a -c + arg to specify an arbitrary cp limit
  while(int gopt = getopt(argc, argv, "gu")){
    if(gopt == -1){
      break;
    }
    // extract cpbound using getopt FIXME
    switch(gopt){
      case 'g':
        set_cpcap(&cpcap, GLCPCAP, argv0);
        break;
      case 'u':
        set_cpcap(&cpcap, ULCPCAP, argv0);
        break;
      default:
        std::cerr << "unknown option " << static_cast<char>(gopt) << std::endl;
        usage(argv[0]);
    }
  }
  if(cpcap < 0){ // no cpcap was provided; don't enforce one
    cpcap = 0;
  }
  argc -= optind;
  argv += optind;
  Teambuild tb(cpcap);
  unsigned pcount = 0;
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    if(!argc && i){
      pcount = i;
      break;
    }
    uint16_t mhp;
    if(lex_pmon(&tb.pmons[i], &mhp, &argc, &argv)){
      usage(argv0);
    }
  }
  for(unsigned i = 0 ; i < pcount ; ++i){
    summarize_pmon(tb, tb.pmons[i]);
  }
  return EXIT_SUCCESS;
}
