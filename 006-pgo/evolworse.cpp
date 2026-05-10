#include "pgotypes.h"

bool check_worse_evol(const species& s, int cpbound){
  bool worse = false;
  std::vector<const species *> evols;
  get_persistent_evolutions(&s, evols);
  unsigned vcount;
  auto opts = order_ivs(&s, cpbound, statscmp_gmean, &vcount);
  const auto &os = opts[vcount - 1];
  const auto gs = os.geommean;
  for(const auto* e : evols){
    unsigned evcount;
    auto eopts = order_ivs(e, cpbound, statscmp_gmean, &evcount);
    const stats &oe = eopts[evcount - 1];
    const auto ge = oe.geommean;
    if(gs > ge){
      std::cout << (gs / ge) << " " << s.name << " " << gs
          << " " << e->name << " " << ge << std::endl;
      worse = true;
    }
  }
  return worse;
}

void usage(const char* argv0){
  std::cerr << "usage: " << argv0 << " cpbound" << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char * const argv[]){
  if(argc != 2){
    usage(argv[0]);
  }
  auto cpb = atoi(argv[1]);
  if(!cpb){
    std::cerr << "invalid cpbound (use -1 for no bound)" << std::endl;
    usage(argv[0]);
  }
  std::cout << std::fixed;
  std::cout.precision(4);
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    check_worse_evol(sdex[i], cpb);
  }
  return EXIT_SUCCESS;
}
