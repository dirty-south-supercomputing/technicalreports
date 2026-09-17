#include "pgotypes.h"

static void
print_attack_users_sdex(const attack *a, const species *dex, unsigned dcount,
                        bool *printed){
  for(unsigned i = 0 ; i < dcount ; ++i){
    const species *s = &dex[i];
    for(const auto &sa : s->attacks){
      if(strcmp(sa->name, a->name) == 0){
        if(*printed){
          printf(", ");
        }
        bool stab = has_stab_p(s, sa);
        bool elite = exclusive_attack_p(s, sa);
        if(elite){
          printf("\\textbf{");
        }
        if(!stab){
          printf("\\textit{");
        }
        escape_string(s->name.c_str());
        if(!stab){
          printf("}");
        }
        if(elite){
          printf("}");
        }
        *printed = true;
      }
    }
  }
}

static void
print_attack_users(const attack *a){
  bool printed = false;
  printf("\\begin{attackbox}[enhanced,title=");
  escape_string(a->name);
  printf(",colbacktitle=%s,before title={",
          a->type == TYPECOUNT ? "White" : tname_capitalized(a->type));
  print_type(a->type);
  printf("},after title={");
  if(a->turns){
    printf("\\hfill{}%.1gs P%u E%u\\hfill{}T%u P%u E%d",
        a->animdur / 2.0, a->powerraid, a->energyraid,
        a->turns, a->powertrain, a->energytrain);
  }else{
    printf("\\hfill{}%.1gs P%u E%u\\hfill{}P%u E%d ",
        a->animdur / 2.0, a->powerraid, a->energyraid,
        a->powertrain, -a->energytrain);
    summarize_buffs(a);
  }
  printf("}]\n");
  if(!strcmp(a->name, "Return")){
    printf("Purification replaces the Frustration charged attack with Return. "
        "It cannot be reacquired if replaced via Charged TM.");
  }else if(!strcmp(a->name, "Frustration")){
    printf("All Shadow Pokémon know Frustration (and only Frustration) when captured. "
        "It cannot generally be replaced using a Charged TM, though this is not true during certain events. "
        "Purifying the Shadow Pokémon replaces this attack with Return.");
  }else{
    // we don't want to list mega/primal forms
    print_attack_users_sdex(a, sdex, SPECIESCOUNT, &printed);
  }
  // FIXME improve subtitle or use something else
  if(a->adveffect){
    printf("\\tcbsubtitle{Adventure Effect}");
  }
  printf("\n\\end{attackbox}\n");
  if(a->type != TYPECOUNT){
    printf("\\pagecolor{%s!25!white}", tname_capitalized(a->type));
  }
}

static void
usage(const char *argv0){
  fprintf(stderr, "usage: %s fast|charged\n", argv0);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
  if(argc != 2){
    usage(argv[0]);
  }
  bool fast;
  if(strcmp(argv[1], "fast") == 0){
    fast = true;
  }else if(strcmp(argv[1], "charged") == 0){
    fast = false;
  }else{
    usage(argv[0]);
  }
  for(int t = 0 ; t <= TYPECOUNT ; ++t){
    std::vector<const attack*> acopies{};
    for(auto it = attacks_begin() ; it != attacks_end() ; ++it){
      const attack *a = *it;
      if(a->type == t){
        if(fast && fast_attack_p(a)){
          acopies.push_back(a);
        }else if(!fast && charged_attack_p(a)){
          acopies.push_back(a);
        }
      }
    }
    std::cout << std::endl;
    std::sort(acopies.begin(), acopies.end(), [](const attack* a1, const attack* a2){
          if(a1->turns != a2->turns){
            if(a1->turns < a2->turns){
              return true;
            }
            return false;
          }
          if(a1->energytrain < 0){ // charged, compare ppe
            float p1 = calc_ppe(a1);
            float p2 = calc_ppe(a2);
            if(p1 < p2){
              return true;
            }
            return false;
          }
          // fast, compare EPT*PPT. we know they're the same number of turns,
          // so we can just compare the products.
          int p1 = a1->powertrain * a1->energytrain;
          int p2 = a2->powertrain * a2->energytrain;
          if(p1 < p2){
            return true;
          }
          return false;
        });
    for(const auto& a : acopies){
      print_attack_users(a);
    }
  }
  return EXIT_SUCCESS;
}
