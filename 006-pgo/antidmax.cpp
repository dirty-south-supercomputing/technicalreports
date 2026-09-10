#include "pgotypes.h"

void usage(const char* argv0, int exitcode){
  std::cerr << "usage: " << argv0 << " form" << std::endl;
  exit(exitcode);
}

// build up candidates for the given species attacking something having typing
// ttype1+ttype2
void handle_species(const species *s, pgo_types_e ttype1, pgo_types_e ttype2,
                    std::vector<candidate>& cands){
  if(has_dmax(s)){
    bool ts[TYPECOUNT]{};
    for(const auto a : s->attacks){
      if(fast_attack_p(a)){
        auto at = dmax_attack_type(a);
        if(ts[at]){
          continue;
        }
        ts[at] = true;
        bool stab = has_stab_raw_p(s, at);
        float teffect = type_effectiveness(at, ttype1, ttype2);
        add_candidate(cands, s, max_attack_name(at), false, stab, at, teffect);
      }
    }
  }
  auto gma = lookup_gmax_attack(s);
  if(gma){
    bool stab = has_stab_raw_p(s, gma->type);
    float teffect = type_effectiveness(gma->type, ttype1, ttype2);
    add_candidate(cands, s, gma->name.c_str(), true, stab, gma->type, teffect);
  }
}

// determine the best max attackers against the specified pokémon
int main(int argc, const char** argv){
  if(argc != 2){
    usage(argv[0], EXIT_FAILURE);
  }
  const auto* s = lookup_species(argv[1]);
  if(!s){
    std::cerr << "couldn't find form " << argv[1] << std::endl;
    usage(argv[0], EXIT_FAILURE);
  }
  std::vector<candidate> cands;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const auto a = &sdex[u];
    handle_species(a, s->t1, s->t2, cands);
  }
  float teffect = type_effectiveness(TYPE_STEEL, s->t1, s->t2);
  const auto zac = lookup_species("Zacian Crowned Sword");
  if(!zac){
    return -1;
  }
  add_candidate(cands, zac, "Behemoth Blade", false, true, TYPE_STEEL, teffect);
  const auto zam = lookup_species("Zamazenta Crowned Shield");
  if(!zam){
    return -1;
  }
  add_candidate(cands, zam, "Behemoth Bash", false, true, TYPE_STEEL, teffect);
  const auto e = lookup_species("Eternatus");
  if(!e){
    return -1;
  }
  teffect = type_effectiveness(TYPE_DRAGON, s->t1, s->t2);
  add_candidate(cands, e, "Dynamax Cannon", true, true, TYPE_DRAGON, teffect);
  std::sort(cands.begin(), cands.end(), std::greater<>());
  auto maxp = cands.begin()->powprod();
  for(const auto& c : cands){
    emit_cand(c, maxp);
  }
  return EXIT_SUCCESS;
}
