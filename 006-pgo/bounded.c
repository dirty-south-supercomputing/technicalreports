#include "pgotypes.c"
#include <stdio.h>
#include <stdlib.h>

// merge with print_cp_bounded(), ugh
static stats *
find_optimal_set(const species* s, int cpceil){
  stats* optsets = NULL;
  for(int iva = 0 ; iva < 16 ; ++iva){
    for(int ivd = 0 ; ivd < 16 ; ++ivd){
      for(int ivs = 0 ; ivs < 16 ; ++ivs){
        int cp;
        unsigned hl = maxlevel_cp_bounded(s->atk + iva, s->def + ivd, s->sta + ivs, cpceil, &cp);
        if(update_optset(&optsets, s, iva, ivd, ivs, hl) < 0){
          return NULL;
        }
      }
    }
  }
  stats* collectopt = NULL;
  stats** qopt = &collectopt;
  float maxmean = -1;
  // print the optimal frontier (large), and extract the truly optimal sets (small)
  while(optsets){
    stats* cur;
    cur = optsets;
    optsets = cur->next;
    cur->s = s;
    //printf(" %u-%u-%u: %2u %4u %.3f %.3f %u %.3f\n", cur->ia, cur->id, cur->is,
    //    cur->hlevel, cur->cp, cur->effa, cur->effd, cur->mhp, cur->geommean);
    if(cur->geommean > maxmean){ // new optimal
      stats* c;
      // clean out existing true optimals
      while( (c = collectopt) ){
        collectopt = c->next;
        free(c);
      }
      collectopt = cur;
      qopt = &cur->next;
      cur->next = NULL;
      maxmean = cur->geommean;
    }else if(cur->geommean == maxmean){ // FIXME unsafe FP comparison
      *qopt = cur;
      qopt = &cur->next;
      cur->next = NULL;
    }else{
      free(cur);
    }
  }
  return collectopt;
}

void print_bounded_table(int bound){
  printf("\\begingroup\n");
  printf("\\nohyphenation\n");
  printf("\\setlength{\\tabcolsep}{1pt}\n");
  printf("\\begin{longtable}{p{6em}m{3em}m{5em}m{3em}m{3em}m{3em}m{3em}m{3em}}\n");
  printf("Species & L & IVs & MHP & $Eff_A$ & $Eff_D$ & $\\sqrt[3]{BP}$ & CP \\\\\n");
  printf("\\Midrule\\\\\n");
  printf("\\endhead\n");
  stats *sols = NULL;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    stats *s = find_optimal_set(&sdex[i], bound);
    stats **prev = &sols;
    stats *tmp;
    while( (tmp = *prev) ){
      if(s->geommean > tmp->geommean){
        break;
      }
      prev = &tmp->next;
    }
    s->next = tmp;
    *prev = s;
  }
  while(sols){
    stats *tmp = sols;
    unsigned half;
    unsigned l = halflevel_to_level(tmp->hlevel, &half);
    printf("%s & %2u%s & %u-%u-%u & %u & %.2f & %.2f & %.2f & %4u\\\\\n",
            tmp->s->name,
            l, half ? ".5" : "",
            tmp->ia, tmp->id, tmp->is,
            tmp->mhp, tmp->effa, tmp->effd, tmp->geommean,
            tmp->cp);
    sols = sols->next;
    free(tmp);
  }
  printf("\\caption{Optimal solutions bounded by %d CP}\n", bound);
  printf("\\end{longtable}\n");
  printf("\\endgroup\n");
}

// print LaTeX longtables of all species' 2500 and 1500 optimal configurations
int main(void){
  print_bounded_table(2500);
  print_bounded_table(1500);
  return EXIT_SUCCESS;
}
