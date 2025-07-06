#include "pgotypes.cpp"

static void
print_attack_users_sdex(const attack *a, const species *dex, unsigned dcount,
                        bool *printed){
  for(unsigned i = 0 ; i < dcount ; ++i){
    const species *s = &dex[i];
    for(const attack **sa = s->attacks ; *sa ; ++sa){
      if(strcmp((*sa)->name, a->name) == 0){
        if(*printed){
          printf(", ");
        }
        bool stab = has_stab_p(s, *sa);
        bool elite = exclusive_attack_p(s, *sa);
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
  if(a->type != TYPECOUNT){
    printf("\\pagecolor{%s!50}", TNames[a->type]);
  }
  bool printed = false;
  printf("\\begin{tcolorbox}[enhanced,title=");
  escape_string(a->name);
  printf(",colbacktitle=%s,fonttitle=\\bfseries,before title={",
          a->type == TYPECOUNT ? "White" : TNames[a->type]);
  print_type(a->type);
  printf(" ");
  printf("},after title={");
  if(a->turns){
    printf("\\hfill T%u P%u E%d", a->turns, a->powertrain, a->energytrain);
  }else{
    printf("\\hfill P%u E%d", a->powertrain, -a->energytrain);
  }
  printf("}]\n");
  if(!strcmp(a->name, "Return")){
    printf("Return replaces the Frustration charged attack whenever a Shadow Pokémon is purified. "
        "It cannot be reacquired if replaced via Charged TM.");
  }else if(!strcmp(a->name, "Frustration")){
    printf("All Shadow Pokémon know Frustration (and only Frustration) when captured. "
        "It cannot generally be replaced using a Charged TM, though this is not true during certain events. "
        "Purifying the Shadow Pokémon replaces this attack with Return.");
  }else{
    for(auto dex : sdexen){
      print_attack_users_sdex(a, dex.dex, dex.dcount, &printed);
    }
  }
  if(a->chance_user_attack){
    printf("\\\\\n\\textbf{%g\\%% chance of attack %d}\n",
          a->chance_user_attack / 10.0, a->user_attack);
  }
  if(a->chance_user_defense){
    printf("\\\\\n\\textbf{%g\\%% chance of defense %d}\n",
          a->chance_user_defense / 10.0, a->user_defense);
  }
  if(a->chance_opp_attack){
    printf("\\\\\n\\textbf{%g\\%% chance of opponent attack %d}\n",
          a->chance_opp_attack / 10.0, a->opp_attack);
  }
  if(a->chance_opp_defense){
    printf("\\\\\n\\textbf{%g\\%% chance of opponent defense %d}\n",
          a->chance_opp_defense / 10.0, a->opp_defense);
  }
  printf("\n\\end{tcolorbox}\n");
}

static void
usage(const char *argv0){
  fprintf(stderr, "usage: %s fast|charged\n", argv0);
  exit(EXIT_FAILURE);
}

static int acmp(const void *va1, const void *va2){
  const attack *a1 = static_cast<const attack*>(va1);
  const attack *a2 = static_cast<const attack*>(va2);
  if(a1->turns != a2->turns){
    return (int)a1->turns - a2->turns;
  }
  if(a1->energytrain < 0){ // charged, compare ppe
    float p1 = calc_ppe(a1);
    float p2 = calc_ppe(a2);
    return p1 < p2 ? -1 : p2 < p1 ? 1 : 0;
  }
  // fast, compare EPT*PPT. we know they're the same number of turns,
  // so we can just compare the products.
  int p1 = a1->powertrain * a1->energytrain;
  int p2 = a2->powertrain * a2->energytrain;
  return p1 - p2;
}

int main(int argc, char **argv){
  static attack acopies[ATTACKCOUNT];
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
    unsigned acount = 0;
    for(unsigned aidx = 0 ; aidx < ATTACKCOUNT ; ++aidx){
      const attack *a = attacks[aidx];
      if(a->type == t){
        if(fast && a->energytrain >= 0){
          memcpy(&acopies[acount], a, sizeof(*a));
          ++acount;
        }else if(!fast && a->energytrain < 0){
          memcpy(&acopies[acount], a, sizeof(*a));
          ++acount;
        }
      }
    }
    qsort(acopies, acount, sizeof(*acopies), acmp);
    for(unsigned u = 0 ; u < acount ; ++u){
      print_attack_users(&acopies[u]);
    }
  }
  return EXIT_SUCCESS;
}
