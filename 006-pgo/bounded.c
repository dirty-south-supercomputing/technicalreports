#include "pgotypes.c"
#include <cstdio>
#include <cstring>
#include <cstdlib>

// print optimal sets bounded by CP of |bound| above and geometric mean of |lbound| below
void print_bounded_table(int bound, float lbound){
  printf("\\begingroup\n");
  printf("\\nohyphenation\n");
  printf("\\footnotesize\n");
  printf("\\setlength{\\tabcolsep}{1pt}\n");
  printf("\\begin{longtable}{lrrrrrrr}\n");
  printf("Species & L & IVs & HP & $Eff_A$ & $Eff_D$ & $\\sqrt[3]{BP}$ & CP \\\\\n");
  printf("\\Midrule\\\\\n");
  printf("\\endhead\n");
  stats *sols = NULL;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    stats *s = find_optimal_set(&sdex[i], bound, lbound);
    while(s){
      stats **prev = &sols;
      stats *tmp;
      while( (tmp = *prev) ){
        if(s->geommean > tmp->geommean){
          break;
        }
        prev = &tmp->next;
      }
      stats* tmps = s->next;
      s->next = tmp;
      *prev = s;
      s = tmps;
    }
  }
  const species* lastspecies = NULL;
  while(sols){
    stats *tmp = sols;
    unsigned half;
    unsigned l = halflevel_to_level(tmp->hlevel, &half);
    if(tmp->s != lastspecies){
      for(const char* curs = tmp->s->name ; *curs ; ++curs){
        if(*curs == '%'){
          putchar('\\');
        }
        putchar(*curs);
      }
      printf(" & %2u%s & %u/%u/%u & %u & %.2f & %.2f & %.2f & %4u\\\\\n",
              l, half ? ".5" : "",
              tmp->ia, tmp->id, tmp->is,
              tmp->mhp, tmp->effa, tmp->effd, tmp->geommean,
              tmp->cp);
    }else{
      printf(" & & %u/%u/%u & & & & & %4u\\\\\n", tmp->ia, tmp->id, tmp->is, tmp->cp);
    }
    lastspecies = tmp->s;
    sols = sols->next;
    free(tmp);
  }
  printf("\\caption{Optimal solutions bounded by %d CP}\n", bound);
  printf("\\label{table:cp%d}\n", bound);
  printf("\\end{longtable}\n");
  printf("\\endgroup\n");
}

// print LaTeX longtables of all species' 2500 and 1500 optimal configurations
int main(int argc, char** argv){
  if(argc != 3){
    fprintf(stderr, "usage: highcp lowGM\n");
    return EXIT_FAILURE;
  }
  int hcp = atoi(argv[1]);
  int lgm = atoi(argv[2]);
  print_bounded_table(hcp, lgm);
  return EXIT_SUCCESS;
}
