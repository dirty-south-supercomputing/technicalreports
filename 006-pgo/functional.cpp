#include "pgotypes.h"

static void
functional_hundos(int cplimit){
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const auto& s = sdex[u];
    // find the maximum level subject to cplimit for 15-15-15
    // find the optimal configs subject to cplimit
    auto st = find_optimal_set(&s, cplimit, 0, false, calc_pok_gmean);
    float gmean = st->geommean;
    auto tmp = st->next;
    while(tmp){
      std::cout << s.name << " " << tmp->ia << "-" << tmp->id << "-" << tmp->is << std::endl;
      if(tmp->geommean != gmean){
        break;
      }
      tmp = tmp->next;
    }
    delete[] st;
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
