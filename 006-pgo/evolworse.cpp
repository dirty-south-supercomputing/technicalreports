#include "pgotypes.h"
#include <cfloat>

bool check_worse_evol(const species& s, int cpbound){
  bool ret = false;
  stats svec[IVLEVVEC];
  // generate the level we can hit for each of 4k iv configurations given cpbound
  order_ivs_internal(&s, cpbound, svec, false);
  float gs = 0;
  float gsworst = FLT_MAX;
  std::vector<const species *> evols;
  get_persistent_evolutions(&s, evols);
  for(const auto* e : evols){
    unsigned worse = 0;
    float ge = 0;
    float geworst = FLT_MAX;
    // generate the levels for the evol, as we did the base
    stats evec[sizeof(svec) / sizeof(*svec)];
    order_ivs_internal(e, cpbound, evec, false);
    // now, compare each of the 4k configs (geometric mean)
    for(unsigned i = 0 ; i < sizeof(svec) / sizeof(*svec) ; ++i){
      if(evec[i].geommean < svec[i].geommean){
        ++worse;
      }
      if(svec[i].geommean < gsworst){
        gsworst = svec[i].geommean;
      }
      if(svec[i].geommean > gs){
        gs = svec[i].geommean;
      }
      if(evec[i].geommean < geworst){
        geworst = evec[i].geommean;
      }
      if(evec[i].geommean > ge){
        ge = evec[i].geommean;
      }
    }
    if(worse){
      std::cout << "<tr>";
      std::cout << "<td>" << s.name << "</td>";
      std::cout << "<td>" << gsworst << "–" << gs << "</td>";
      std::cout << "<td>" << e->name << "</td>";
      std::cout << "<td>" << geworst << "–" << ge << "</td>";
      if(worse == sizeof(svec) / sizeof(*svec)){
        if(gsworst > ge){
          std::cout << "<td>absolute</td>";
        }else{
          std::cout << "<td>pure</td>";
        }
      }else{
        std::cout << "<td>partial (" << worse << ")</td>";
      }
      std::cout << "</tr>" << std::endl;
      ret = true;
    }
  }
  return ret;
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
  std::cout << "<table>" << std::endl;
  std::cout << "<tr><th>Pokémon</th><th>Gmean range</th><th>Evolution</th><th>Gmean range</th><th>Relationship</th></tr>" << std::endl;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    check_worse_evol(sdex[i], cpb);
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
