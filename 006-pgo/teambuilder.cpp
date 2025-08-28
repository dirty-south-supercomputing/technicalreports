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
summarize_pmon(const Teambuild &tb, const pmon &p){
  std::cout << p.s.s->name;
  // FIXME if no IVs supplied, isolate best for CP limit
  std::cout << std::endl;
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
  --argc;
  ++argv;
  int cpcap = -1;
  // FIXME allow a -c + arg to specify an arbitrary cp limit
  while(int gopt = getopt(argc, argv, "")){
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
  argc -= (optind - 1);
  unsigned pcount = 0;
  Teambuild tb;
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
