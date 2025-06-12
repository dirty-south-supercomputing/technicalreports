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

static void
print_species(const species* s){
  if(s->t2 == TYPECOUNT){
    printf("%u %u %u %u %s %s\n", s->idx, s->atk, s->def, s->sta, TNames[s->t1], s->name);
  }else{
    printf("%u %u %u %u %s+%s %s\n", s->idx, s->atk, s->def, s->sta,
            TNames[s->t1], TNames[s->t2], s->name);
  }
  stats* s2500 = find_optimal_set(s, 2500, 0);
  while(s2500){
    stats* tmp = s2500->next;
    // print s2500
    free(s2500);
    s2500 = tmp;
  }
}

int main(int argc, char **argv){
  if(argc != 2){
    fprintf(stderr, "usage: %s mega|primal|crowned|dynamax|gigantamax\n", argv[0]);
    return EXIT_FAILURE;
  }
  const species* dex;
  unsigned count;
  if(strcasecmp(argv[1], "mega") == 0){
    dex = megasdex;
    count = MEGACOUNT;
  }else if(strcasecmp(argv[1], "primal") == 0){
    dex = primalsdex;
    count = PRIMALCOUNT;
  }else if(strcasecmp(argv[1], "crowned") == 0){
    dex = crownedsdex;
    count = CROWNEDCOUNT;
  }else if(strcasecmp(argv[1], "dynamax") == 0){
    dex = dynadex;
    count = DYNACOUNT;
  }else if(strcasecmp(argv[1], "gigantamax") == 0){
    dex = gigantasdex;
    count = GIGANTACOUNT;
  }else{
    fprintf(stderr, "usage: %s mega|primal|crowned|dynamax|gigantamax\n", argv[0]);
    return EXIT_FAILURE;
  }
  // we want to retain the order of typings here, so do both ways
  for(int t = TYPESTART ; t < static_cast<int>(TYPECOUNT) ; ++t){
    // we always want the pure typing first
    filter_by_types(t, t, dex, count);
    for(int t2 = TYPESTART ; t2 < static_cast<int>(TYPECOUNT) ; ++t2){
      if(t != t2){
        filter_by_types(t, t2, dex, count);
      }
    }
  }
  return EXIT_SUCCESS;
}
