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
      fprintf(stderr, "Bad ancestor (%s) for %s, exiting\n", s->from, s->name);
      return -1;
    }
    if(from->t1 == s->t1 && from->t2 == s->t2){
      continue;
    }
    print_types(from->t1, from->t2);
    printf(" %s", from->name);
    #define GLAR "Galarian"
    if(!strncmp(from->name, GLAR, strlen(GLAR)) || !strncmp(s->name, GLAR, strlen(GLAR))){
      // FIXME need indentation here
      printf("\\newline → ");
    }else{
      printf(" → ");
    }
    #undef GLAR
    print_types(s->t1, s->t2);
    printf(" %s ", s->name);
    if(++*pcount % 2){
      printf(" & ");
    }else{
      printf("\\\\\n");
    }
  }
  return 0;
}

// print a latex table of evolutions which change types
int main(void){
  printf("\\begin{table}[ht]\n");
  printf("\\footnotesize\n");
  printf("\\begin{center}\n");
  printf("\\begin{tabular}{p{.5\\textwidth}p{.5\\textwidth}}\n");
  unsigned count = 0;
  print_hetero_evols(sdex, SPECIESCOUNT, &count);
  print_hetero_evols(megasdex, MEGACOUNT, &count);
  print_hetero_evols(primalsdex, PRIMALCOUNT, &count);
  printf("\\end{tabular}\n");
  printf("\\caption{Type-changing evolutions and form changes}\n");
  printf("\\label{table:heteroevolve}\n");
  printf("\\end{center}\n");
  printf("\\end{table}\n");
  return EXIT_SUCCESS;
}
