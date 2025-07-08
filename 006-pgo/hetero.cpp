#include "pgotypes.cpp"

static int
print_hetero_evols(const species* dex, unsigned dexcount, unsigned* pcount){
  for(unsigned u = 0 ; u < dexcount ; ++u){
    const species* s = &dex[u];
    if(s->from == NULL){
      continue;
    }
    const species* from = lookup_species(s->from);
    if(from == NULL){
      fprintf(stderr, "Bad ancestor (%s) for %s, exiting\n", s->from, s->name.c_str());
      return -1;
    }
    if(from->t1 == s->t1 && from->t2 == s->t2){
      continue;
    }
    print_types(from->t1, from->t2);
    printf(" %s", from->name.c_str());
    #define GLAR "Galarian"
    #define CROW "Crowned Shield"
    if(!strncmp(from->name.c_str(), GLAR, strlen(GLAR)) || !strncmp(s->name.c_str(), GLAR, strlen(GLAR))
        || !strncmp(s->name.c_str(), CROW, strlen(CROW))){
      printf("\\newline\\hspace{\\parskip}→ ");
    }else{
      printf(" → ");
    }
    #undef CROW
    #undef GLAR
    print_types(s->t1, s->t2);
    printf(" %s ", s->name.c_str());
    if(++*pcount % 2){
      printf(" & ");
    }else{
      printf("\\\\\n");
    }
  }
  return 0;
}

static void type_heterotable(void){
  printf("\\begingroup");
  printf("\\footnotesize");
  printf("\\begin{longtable}{p{.5\\textwidth}|p{.5\\textwidth}}");
  unsigned count = 0;
  puts("Evolution & Evolution\\\\\\Midrule");
  for(const auto &dex : sdexen){
    if(print_hetero_evols(dex.dex, dex.dcount, &count)){
      exit(EXIT_FAILURE);
    }
  }
  if(count % 2){
    printf("\\\\\n");
  }
  printf("\\caption{Type-changing evolutions and form changes}");
  printf("\\label{table:heteroevolve}");
  printf("\\end{longtable}");
  printf("\\endgroup");
}

static int
print_hetero_costs(const species* dex, unsigned dexcount, unsigned* pcount){
  for(unsigned u = 0 ; u < dexcount ; ++u){
    const species* s = &dex[u];
    if(s->from == NULL){
      continue;
    }
    const species* from = lookup_species(s->from);
    if(from == NULL){
      fprintf(stderr, "Bad ancestor (%s) for %s, exiting\n", s->from, s->name.c_str());
      return -1;
    }
    if(from->a2cost == s->a2cost){
      continue;
    }
    printf("%s → %s & %d → %d", from->name.c_str(), s->name.c_str(),
        a2cost_to_cgroup(from->a2cost), a2cost_to_cgroup(s->a2cost));
    if(++*pcount % 2){
      printf(" & ");
    }else{
      printf("\\\\\n");
    }
  }
  return 0;
}

static void cost_heterotable(void){
  printf("\\begin{table}\\centering\\footnotesize");
  printf("\\begin{tabular}{lr|lr}");
  unsigned count = 0;
  puts("Evolution & Change & Evolution & Change\\\\\\Midrule");
  for(const auto &dex : sdexen){
    if(print_hetero_costs(dex.dex, dex.dcount, &count)){
      exit(EXIT_FAILURE);
    }
  }
  if(count % 2){
    printf("&\\\\\n");
  }
  printf("\\end{tabular}");
  printf("\\caption{Evolutions that change cost group}");
  printf("\\label{table:heterocost}");
  printf("\\end{table}");
}

// print a latex table of evolutions which change types
int main(void){
  type_heterotable();
  cost_heterotable();
  return EXIT_SUCCESS;
}
