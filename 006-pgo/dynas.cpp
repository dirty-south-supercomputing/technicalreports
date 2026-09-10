#include "pgotypes.h"

void usage(const char* argv0, int ret){
  std::cerr << "usage: " << argv0 << " [ type ]" << std::endl;
  exit(ret);
}

void handle_species(const species *s, pgo_types_e t, std::vector<candidate>& cands){
  bool stab = has_stab_raw_p(s, t);
  if(has_dmax(s)){
    for(const auto a : s->attacks){
      if(fast_attack_p(a)){
        auto at = dmax_attack_type(a);
        if(at == t){
          add_candidate(cands, s, max_attack_name(at), false, stab, t, 1.0);
          break; // don't handle multiple fast attacks of the same type
        }
      }
    }
  }
  auto gma = lookup_gmax_attack(s);
  if(gma){
    if(gma->type == t){
      add_candidate(cands, s, gma->name.c_str(), true, stab, t, 1.0);
    }
  }
}

// build a sorted vector of all *t*-type Max attackers
int build_type_vec(pgo_types_e t, std::vector<candidate>& cands){
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const auto s = &sdex[u];
    handle_species(s, t, cands);
  }
  // we hard code the crowned forms and eternatus, yuck. we don't want to
  // generally mark them as dmax/gmax, as they're technically not.
  if(t == TYPE_STEEL){
    // crowned forms always use behemoth attacks, even with non-steel fast attacks
    const auto zac = lookup_species("Zacian Crowned Sword");
    if(!zac){
      return -1;
    }
    add_candidate(cands, zac, "Behemoth Blade", false, true, t, 1.0);
    const auto zam = lookup_species("Zamazenta Crowned Shield");
    if(!zam){
      return -1;
    }
    add_candidate(cands, zam, "Behemoth Bash", false, true, t, 1.0);
  }else if(t == TYPE_DRAGON){
    // eternatus always uses dynamax cannon, even if its fast move is non-dragon
    const auto e = lookup_species("Eternatus");
    if(!e){
      return -1;
    }
    add_candidate(cands, e, "Dynamax Cannon", true, true, t, 1.0);
  }
  std::sort(cands.begin(), cands.end(), std::greater<>());
  return cands.size();
}

// top *count* attackers for each attack type, unified.
// 0 gets all possible max attackers.
int emit_dynamax_unified_table(int count){
  std::cout << "\\begin{table}\\centering\\footnotesize";
  std::cout << "\\begin{tabular}{cllrr}";
  std::cout << "Type & Pokémon & Attack & Relative & Absolute\\\\";
  std::cout << "\\Midrule" << std::endl;
  std::vector<candidate> cands;
  for(int t = 0 ; t < TYPECOUNT ; ++t){
    std::vector<candidate> tcands;
    if(build_type_vec(static_cast<pgo_types_e>(t), tcands) <= 0){
      return -1;
    }
    int emit = 0;
    for(const auto& c : tcands){
      cands.emplace_back(c);
      if(++emit == count){
        break;
      }
    }
  }
  std::sort(cands.begin(), cands.end(), std::greater<>());
  auto maxp = cands.begin()->powprod();
  for(const auto& c : cands){
    emit_cand(c, maxp);
  }
  std::cout << "\\end{tabular}\\caption{Top Max attackers";
  if(count){
    std::cout << " (" << count << " per attack type, unified)";
  }
  std::cout << "\\label{table:maxranked}}\\end{table}";
  return 0;
}

// top *count* attackers throwing max attack type *t*.
// 0 for complete list.
int emit_dynamax_typed_table(pgo_types_e t, int count){
  std::cout << "\\begin{table}[hb]\\centering\\footnotesize";
  std::cout << "\\begin{tabular}{cllrr}";
  std::cout << "Type & Pokémon & Attack & Relative & Absolute\\\\";
  std::cout << "\\Midrule" << std::endl;
  std::vector<candidate> cands;
  if(build_type_vec(t, cands) <= 0){
    return -1;
  }
  auto maxp = cands.begin()->powprod();
  int emits = 0;
  for(const auto &c : cands){
    emit_cand(c, maxp);
    if(++emits == count){
      break;
    }
  }
  std::cout << "\\end{tabular}\\caption{Top ";
  if(count){
    std::cout << emits << " ";
  }
  std::cout << tname_capitalized(t) << " Max attackers"
            << "\\label{table:maxranked" << tnames[t] << "}}\\end{table}";
  return 0;
}

// emit tables of best max attackers of (attack) type
int main(int argc, char ** argv){
  if(argc > 2){
    usage(*argv, EXIT_FAILURE);
  }
  if(argc == 1){
    return emit_dynamax_unified_table(3);
  }
  pgo_types_e lext = lookup_type(argv[1]);
  if(lext == TYPECOUNT){
    usage(*argv, EXIT_FAILURE);
  }
  return emit_dynamax_typed_table(lext, 0);
}
