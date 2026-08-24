#include "pgotypes.h"

void usage(const char* argv0, int exitcode){
  std::cerr << "usage: " << argv0 << " form" << std::endl;
  exit(exitcode);
}

struct candidate {
  const species* s;   // species
  std::string aname;  // attack name
  unsigned hlevel;    // halflevel
  bool gmaxpower;     // gmax/eternatus power scale? if not, dmax/crowned.
  bool hasstab;       // do we have stab for the attack?
  unsigned iva;       // attack iv 0..15
  float teffective;   // type effectiveness
  pgo_types_e atype;  // attack type


  float powprod(void) const {
    unsigned p = gmaxpower ? GMAX_POWER_BASE : DMAX_POWER_BASE;
    float rp = hasstab ? calc_stab(p) : p;
    rp *= teffective;
    return rp * calc_eff_a(s->atk + iva, hlevel, false);
  }

  bool operator<(const candidate& r) const {
    if(powprod() < r.powprod()){
      return true;
    }
    return false;
  }

  bool operator>(const candidate& r) const {
    if(powprod() > r.powprod()){
      return true;
    }
    return false;
  }
};

void add_candidate(std::vector<candidate>& cands, const species* s,
                   const char* aname, bool gmaxpower, bool stab,
                   pgo_types_e atype,
                   pgo_types_e ttype1, pgo_types_e ttype2){
  const unsigned highiv = 15;
  const auto teffective = type_effectiveness(atype, ttype1, ttype2);
  cands.emplace_back(s, aname, MAX_HALFLEVEL_BASIC, gmaxpower, stab,
                      highiv, teffective, atype); // level 50
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
        add_candidate(cands, s, max_attack_name(at), false, stab, at, ttype1, ttype2);
      }
    }
  }
  auto gma = lookup_gmax_attack(s);
  if(gma){
    bool stab = has_stab_raw_p(s, gma->type);
    add_candidate(cands, s, gma->name.c_str(), true, stab, gma->type, ttype1, ttype2);
  }
}

void emit_cand(const candidate& c, unsigned maxp){
  auto rp = c.powprod();
  //unsigned hhalf;
  print_types(c.s->t1, c.s->t2);
  std::cout << " & " << c.s->name;
  //std::cout << " & " << c.iva << " & " << halflevel_to_level(c.hlevel, &hhalf);
  std::cout << " & ";
  print_type(c.atype);
  std::cout << c.aname << " & ";
  std::cout << std::setprecision(2) << std::fixed << (rp * 100.0 / maxp) << "\\% & ";
  std::cout << std::setprecision(0) << std::fixed << rp << " \\\\" << std::endl;
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
  // FIXME handle crowned dogs / eternatus
  std::sort(cands.begin(), cands.end(), std::greater<>());
  auto maxp = cands.begin()->powprod();
  for(const auto& c : cands){
    emit_cand(c, maxp);
  }
  return EXIT_SUCCESS;
}
