#include "pgotypes.h"

void usage(const char* argv0, int ret){
  std::cerr << "usage: " << argv0 << " type" << std::endl;
  exit(ret);
}

int emit_dynamax_table(pgo_types_e t){
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const auto s = &sdex[u];
    if(has_dmax(s)){
      for(const auto a : s->attacks){
        if(fast_attack_p(a)){
          auto at = dmax_attack_type(a);
          if(at == t){
            std::cout << "dmax " << s->name << " (" << a->name
              << " -> Max " << max_attack_name(at) << ")" << std::endl;
          }
        }
      }
    }
    auto gma = lookup_gmax_attack(s);
    if(gma){
      if(gma->type == t){
        std::cout << "gmax " << s->name << " (G-Max " << s->gmax << ")" << std::endl;
      }
    }
  }
  return 0;
}

// emit tables of best max attackers of (attack) type
int main(int argc, char ** argv){
  if(argc != 2){
    usage(*argv, EXIT_FAILURE);
  }
  pgo_types_e lext = lookup_type(argv[1]);
  if(lext == TYPECOUNT){
    usage(*argv, EXIT_FAILURE);
  }
  return emit_dynamax_table(lext);
}
