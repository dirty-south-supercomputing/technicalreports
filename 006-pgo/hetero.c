#include "pgotypes.c"

// print a latex table of evolutions which change types
int main(void){
  printf("\\begin{table}[ht]\n");
  printf("\\begin{center}\n");
  printf("\\begin{tabular}{ll}\n");
  printf("Original species & Evolved species\\\\\n");
  printf("\\Midrule\\\\\n");
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const species* s = &sdex[u];
    if(s->from == NULL){
      continue;
    }
    const species* from = lookup_species(s->from);
    if(from == NULL){
      fprintf(stderr, "Bad ancestor (%s) for %s, exiting\n", s->from, s->name);
      return EXIT_FAILURE;
    }
    if(from->t1 == s->t1 && from->t2 == s->t2){
      continue;
    }
    print_types(from->t1, from->t2);
    printf(" %s & ", from->name);
    print_types(s->t1, s->t2);
    printf(" %s ", s->name);
    printf("\\\\\n");
  }
  for(unsigned u = 0 ; u < MEGACOUNT ; ++u){
    const species* s = &megasdex[u];
    if(s->from == NULL){
      continue;
    }
    const species* from = lookup_species(s->from);
    if(from == NULL){
      fprintf(stderr, "Bad ancestor (%s) for %s, exiting\n", s->from, s->name);
      return EXIT_FAILURE;
    }
    if(from->t1 == s->t1 && from->t2 == s->t2){
      continue;
    }
    print_types(from->t1, from->t2);
    printf(" %s & ", from->name);
    print_types(s->t1, s->t2);
    printf(" %s ", s->name);
    printf("\\\\\n");
  }
  printf("\\end{tabular}\n");
  printf("\\caption{Evolutions which change type}\n");
  printf("\\label{table:heteroevolve}\n");
  printf("\\end{center}\n");
  printf("\\end{table}\n");
  return EXIT_SUCCESS;
}
