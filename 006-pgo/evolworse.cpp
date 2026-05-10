#include "pgotypes.h"

bool check_worse_evol(const species& s, int cpbound){
  bool worse = false;
  std::vector<const species *> evols;
  const auto os = find_optimal_set(&s, cpbound, 0, false, calc_pok_gmean);
  const auto gs = os->geommean;
  get_persistent_evolutions(&s, evols);
  for(const auto* e : evols){
    const auto oe = find_optimal_set(e, cpbound, 0, false, calc_pok_gmean);
    const auto ge = oe->geommean;
    if(gs > ge){
      std::cout << s.name << " " << gs << " " << e->name << " " << ge << std::endl;
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
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    check_worse_evol(sdex[i], cpb);
  }
  return EXIT_SUCCESS;
}
