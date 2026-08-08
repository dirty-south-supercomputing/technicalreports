#include "pgotypes.h"

void usage(const char* argv0, int ret){
  std::cerr << "usage: " << argv0 << " type" << std::endl;
  exit(ret);
}

struct candidate {
  const species* s;         // species
  const std::string aname;  // attack name
  bool gmaxpower;           // gmax/eternatus power scale? if not, dmax/crowned.
};

int emit_dynamax_table(pgo_types_e t){
  std::vector<candidate> cands;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const auto s = &sdex[u];
    if(has_dmax(s)){
      for(const auto a : s->attacks){
        if(fast_attack_p(a)){
          auto at = dmax_attack_type(a);
          if(at == t){
            cands.emplace_back(s, max_attack_name(at), false);
            break; // don't handle multiple fast attacks of the same type
          }
        }
      }
    }
    auto gma = lookup_gmax_attack(s);
    if(gma){
      if(gma->type == t){
        cands.emplace_back(s, s->gmax, true);
      }
    }
  }
  // we hard code the crowned forms and eternatus, yuck. we don't want to
  // generally mark them as dmax/gmax, as they're technically not.
  if(t == TYPE_STEEL){
    const auto zac = lookup_species("Crowned Sword Zacian");
    if(!zac){
      return -1;
    }
    cands.emplace_back(zac, "Behemoth Blade", false);
    const auto zam = lookup_species("Crowned Shield Zamazenta");
    if(!zam){
      return -1;
    }
    cands.emplace_back(zam, "Behemoth Bash", false);
  }else if(t == TYPE_DRAGON){
    const auto e = lookup_species("Eternatus");
    if(!e){
      return -1;
    }
    cands.emplace_back(e, "Dynamax Cannon", true);
  }
  for(const auto &c : cands){
    unsigned p = c.gmaxpower ? 350 : 250; // FIXME
    std::cout << c.s->name << " (" << c.aname << ") " << p << std::endl;
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
