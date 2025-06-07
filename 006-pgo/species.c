#include "pgotypes.c"
#include <memory>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*static void
print_cpms(void){
  printf("cpms:\n");
  for(int i = 1 ; i < 100 ; ++i){
    printf("%2d %f\n", i, cpm(i));
  }
}*/

// print the optimal level/IV combinations bounded by the given cp
static int
print_cp_bounded(const species* s, int cpceil){
  stats* collectopt = find_optimal_set(s, cpceil, 0);
  stats* tmp;
  while( (tmp = collectopt) ){
    unsigned half;
    unsigned l = halflevel_to_level(tmp->hlevel, &half);
    printf("  %u-%u-%u: %2u%s %4u %.3f %.3f %u %.3f\n",
      tmp->ia, tmp->id, tmp->is, l, half ? ".5" : "",
      tmp->cp, tmp->effa, tmp->effd, tmp->mhp, tmp->geommean);
    collectopt = tmp->next;
    free(tmp);
  }
  return 0;
}

static const species*
lookup_species(const char* name){
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    if(strcasecmp(sdex[i].name, name) == 0){
      return &sdex[i];
    }
  }
  return NULL;
}

static void
print_species(const species* s){
  if(s->t2 == TYPECOUNT){
    printf("%u %u %u %u %s %s\n", s->idx, s->atk, s->def, s->sta, TNames[s->t1], s->name);
  }else{
    printf("%u %u %u %u %s+%s %s\n", s->idx, s->atk, s->def, s->sta,
            TNames[s->t1], TNames[s->t2], s->name);
  }
  print_cp_bounded(s, 1500);
  print_cp_bounded(s, 2500);
}

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

static void
print_species_latex(const species* s){
  if(s->t2 == TYPECOUNT){
    printf("%u %u %u %u %s %s\\\\\n", s->idx, s->atk, s->def, s->sta, TNames[s->t1], s->name);
  }else{
    printf("%u %u %u %u %s+%s %s\\\\\n", s->idx, s->atk, s->def, s->sta,
            TNames[s->t1], TNames[s->t2], s->name);
  }
  print_cp_bounded_latex(s, 1500);
  print_cp_bounded_latex(s, 2500);
}

// print those entries containing type(s). pass TYPECOUNT for a wildcard on t2.
// pass the same type twice for only that base type. LaTeX output.
static void
filter_by_types(int t1, int t2){
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    if(sdex[i].t1 == t1){
      if(sdex[i].t2 == t2 || t2 == TYPECOUNT || (t1 == t2 && sdex[i].t2 == TYPECOUNT)){
        print_species_latex(&sdex[i]);
      }
    }
  }
}

int main(int argc, char **argv){
  if(argc > 1){
    while(*++argv){
      const species *s = lookup_species(*argv);
      if(s == NULL){
        fprintf(stderr, "no species named \"%s\", exiting\n", *argv);
        return EXIT_FAILURE;
      }
      print_species(s);
    }
  }else{
    //print_cpms();
    for(int t = TYPESTART ; t < static_cast<int>(TYPECOUNT) ; ++t){
      for(int t2 = t ; t2 < static_cast<int>(TYPECOUNT) ; ++t2){
        printf("\\begin{longtable}{r}\n");
        printf("Species\\\\\n");
        printf("\\Midrule\n");
        printf("\\endhead\n");
        filter_by_types(t, t2);
        printf("\\caption{Species of type %s", TNames[t]);
        if(t != t2){
          printf("+%s", TNames[t2]);
        }
        printf("}\n");
        printf("\\end{longtable}\n");
      }
    }
  }
  return EXIT_SUCCESS;
}
