#include "pgotypes.h"

bool check_worse_evol(const species& s, int cpbound){
  bool worse = false;
  unsigned vcount;
  auto opts = order_ivs(&s, cpbound, statscmp_gmean, &vcount);
  const auto gs = opts[vcount - 1].geommean;
  const auto gsworst = opts[0].geommean;
  std::vector<const species *> evols;
  get_persistent_evolutions(&s, evols);
  for(const auto* e : evols){
    // first, compare the best of s to the best of es. if better, compare the worst of s to
    // the best of es. if also better, s is purely better. otherwise, there is overlap.
    // otherwise, compare the best of s to the worst of es. if better, there is overlap.
    // otherwise, es is purely better.
    unsigned evcount;
    auto eopts = order_ivs(e, cpbound, statscmp_gmean, &evcount);
    const stats &oe = eopts[evcount - 1];
    const auto ge = oe.geommean;
    const auto geworst = eopts[0].geommean;
    if(gs > ge){
      std::cout << (gs / ge) << " " << s.name << " " << gsworst << "–" << gs
          << " " << e->name << " " << geworst << "–" << ge;
      if(gsworst > ge){
        std::cout << " pure";
      }else{
        std::cout << " partial";
      }
      std::cout << std::endl;
      worse = true;
    }else{
      if(gs > geworst){
        std::cout << (gs / ge) << " " << s.name << " " << gsworst << "–" << gs
            << " " << e->name << " " << geworst << "–" << ge;
        std::cout << " partial" << std::endl;
        worse = true;
      }else{
        //std::cout << "ALL WORSE! ";
      }
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
  std::cout.precision(3);
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    check_worse_evol(sdex[i], cpb);
  }
  return EXIT_SUCCESS;
}
