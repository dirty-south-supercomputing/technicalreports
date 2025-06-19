#include "pgotypes.cpp"

static void
print_attack_users_sdex(const attack *a, const species *dex, unsigned dcount,
                        bool *printed){
  for(unsigned i = 0 ; i < dcount ; ++i){
    const species *s = &dex[i];
    for(const attack **sa = s->attacks ; *sa ; ++sa){
      if(*sa == a){
        if(*printed){
          printf(", ");
        }
        escape_string(s->name.c_str());
        if(!has_stab_p(s, *sa)){
          printf("⁻");
        }
        *printed = true;
      }
    }
  }
}

static void
print_attack_users(const attack *a){
  bool printed = false;
  printf("\\begin{tcolorbox}[enhanced,title=");
  escape_string(a->name);
  printf(",colbacktitle=%s,before title={",
          a->type == TYPECOUNT ? "White" : TNames[a->type]);
  print_type(a->type);
  printf(" ");
  printf("},after title={");
  if(a->turns){
    printf("\\hfill %u %u %d", a->turns, a->powertrain, a->energytrain);
  }else{
    printf("\\hfill %u %d", a->powertrain, -a->energytrain);
  }
  printf("}]\n");
  print_attack_users_sdex(a, sdex, SPECIESCOUNT, &printed);
  // FIXME handle other sdexen?
  printf("\n\\end{tcolorbox}\n");
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
  for(int t = 0 ; t < TYPECOUNT ; ++t){
    for(unsigned aidx = 0 ; aidx < ATTACKCOUNT ; ++aidx){
      const attack *a = attacks[aidx];
      if(a->type == t){
        if(fast && a->energytrain >= 0){
          print_attack_users(a);
        }else if(!fast && a->energytrain < 0){
          print_attack_users(a);
        }
      }
    }
  }
  return EXIT_SUCCESS;
}
