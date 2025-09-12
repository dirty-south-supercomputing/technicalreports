#include "pgotypes.h"
#include <memory>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
print_species(const species* s){
  if(s->t2 == TYPECOUNT){
    printf("%u %u %u %u %s %s\n", s->idx, s->atk, s->def, s->sta, TNames[s->t1], s->name.c_str());
  }else{
    printf("%u %u %u %u %s+%s %s\n", s->idx, s->atk, s->def, s->sta,
            TNames[s->t1], TNames[s->t2], s->name.c_str());
  }
  stats* s2500 = find_optimal_set(s, 2500, 0, false, calc_pok_gmean);
  while(s2500){
    stats* tmp = s2500->next;
    // print s2500
    delete s2500;
    s2500 = tmp;
  }
  stats* s1500 = find_optimal_set(s, 1500, 0, false, calc_pok_gmean);
  while(s1500){
    stats* tmp = s1500->next;
    // print s2500
    delete s1500;
    s1500 = tmp;
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
    // we want to retain the order of typings here, so do both ways
    for(int t = TYPESTART ; t < static_cast<int>(TYPECOUNT) ; ++t){
      // we always want the pure typing first
      printf("\\label{types:%s}\n", tnames[t]);
      filter_by_types(t, t, sdex, SPECIESCOUNT, true, true);
      for(int t2 = TYPESTART ; t2 < static_cast<int>(TYPECOUNT) ; ++t2){
        if(t != t2){
          printf("\\label{types:%s%s}\n", tnames[t], tnames[t2]);
          filter_by_types(t, t2, sdex, SPECIESCOUNT, true, true);
        }
      }
    }
  }
  return EXIT_SUCCESS;
}
