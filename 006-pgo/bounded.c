#include "pgotypes.c"
#include <stdio.h>
#include <stdlib.h>

// merge with print_cp_bounded(), ugh
static int
print_cp_bounded_latex(const species* s, int cpceil){
  stats* optsets = NULL;
  printf(" %d-bounded set for %s:\\\\\n", cpceil, s->name);
  for(int iva = 0 ; iva < 16 ; ++iva){
    for(int ivd = 0 ; ivd < 16 ; ++ivd){
      for(int ivs = 0 ; ivs < 16 ; ++ivs){
        int cp;
        unsigned hl = maxlevel_cp_bounded(s->atk + iva, s->def + ivd, s->sta + ivs, cpceil, &cp);
        if(update_optset(&optsets, s, iva, ivd, ivs, hl) < 0){
          return -1;
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
  stats* tmp;
  while( (tmp = collectopt) ){
    unsigned half;
    unsigned l = halflevel_to_level(tmp->hlevel, &half);
    printf("  %u-%u-%u: %2u%s %4u %.3f %.3f %u %.3f\\\\\n",
      tmp->ia, tmp->id, tmp->is, l, half ? ".5" : "",
      tmp->cp, tmp->effa, tmp->effd, tmp->mhp, tmp->geommean);
    collectopt = tmp->next;
    free(tmp);
  }
  return 0;
}

void print_bounded_table(int bound){
  printf("\\begin{longtable}{rrrrrrrrrr}\n");
  printf("Species & Level & ATK & DEF & STA & IVs & $Eff_A$ & $Eff_D$ & $MHP$ & CP \\\\\n");
  printf("\\Midrule\\\\\n");
  printf("\\endhead\n");
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    // FIXME generate list
  }
  // FIXME sort list
  // FIXME print list
  printf("\\caption{Optimal solutions bounded by %d CP}\n", bound);
  printf("\\end{longtable}\n");
}

// print LaTeX longtables of all species' 2500 and 1500 optimal configurations
int main(void){
  print_bounded_table(2500);
  print_bounded_table(1500);
  return EXIT_SUCCESS;
}
