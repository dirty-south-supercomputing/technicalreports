#include "html.h"

static void
functional_hundos(int cplimit){
  std::cout << "<table class=\"evenshade\" id=\"functional\">" << std::endl;
  std::cout << "<tr><th>Pokémon</th><th>Optimum</th><th>Functional optima</th></tr>" << std::endl;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    // find the optimal configs subject to cplimit
    auto st = find_optimal_set(*s, cplimit, 0, false, calc_pok_gmean);
    float gmean = st->geommean;
    auto tmp = st->next;
    if(tmp){
      std::cout << "<tr><td>" << st->s->name << "</td><td>" << st->ia << "-" << st->id << "-" << st->is << ' ';
      emit_halflevel_as_level(std::cout, st->hlevel);
      std::cout << " (" << calc_pok_cp(st) << ")" << "</td><td>";
      while(tmp){
        std::cout << tmp->ia << "-" << tmp->id << "-" << tmp->is << ' ';
        emit_halflevel_as_level(std::cout, tmp->hlevel);
        std::cout << " (" << calc_pok_cp(tmp) << ")<br/>";
        if(tmp->geommean != gmean){
          break;
        }
        tmp = tmp->next;
      }
      std::cout << "</td></tr>" << std::endl;
    }
    delete[] st;
  }
  std::cout << "</table>" << std::endl;
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
