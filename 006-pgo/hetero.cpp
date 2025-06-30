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
    if(!strncmp(from->name.c_str(), GLAR, strlen(GLAR)) || !strncmp(s->name.c_str(), GLAR, strlen(GLAR))){
      // FIXME need indentation here
      printf("\\newline → ");
    }else{
      printf(" → ");
    }
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

// print a latex table of evolutions which change types
int main(void){
  printf("\\begingroup");
  printf("\\footnotesize");
  printf("\\begin{longtable}{p{.5\\textwidth}|p{.5\\textwidth}}");
  unsigned count = 0;
  puts("Evolution & Evolution\\\\\\Midrule");
  for(const auto &dex : sdexen){
    if(print_hetero_evols(dex.dex, dex.dcount, &count)){
      return EXIT_FAILURE;
    }
  }
  printf("\\caption{Type-changing evolutions and form changes}");
  printf("\\label{table:heteroevolve}");
  printf("\\end{longtable}");
  printf("\\endgroup");
  return EXIT_SUCCESS;
}
