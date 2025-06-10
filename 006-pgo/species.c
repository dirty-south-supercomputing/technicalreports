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

static void
print_optimal_latex(const species* sp){
  stats* s = find_optimal_set(sp, 2500, 0);
  while(s){
    stats* tmp = s->next;
    unsigned half;
    unsigned l = halflevel_to_level(s->hlevel, &half);
    printf("%u/%u/%u@%u%s ", s->ia, s->id, s->is, l, half ? ".5" : "");
    free(s);
    s = tmp;
  }
  printf("& ");
  s = find_optimal_set(sp, 1500, 0);
  while(s){
    stats* tmp = s->next;
    unsigned half;
    unsigned l = halflevel_to_level(s->hlevel, &half);
    printf("%u/%u/%u@%u%s ", s->ia, s->id, s->is, l, half ? ".5" : "");
    free(s);
    s = tmp;
  }
  printf("\\\\\n");
}

static void
print_species_latex(const species* s){
  printf("\\begin{tcolorbox}[boxsep=0mm,title=");
  for(const char* curs = s->name ; *curs ; ++curs){
    if(*curs != '%'){
      printf("%c", *curs);
    }else{
      printf("\\%%");
    }
  }
  printf(",sidebyside,lower separated=false,fonttitle=\\bfseries,after title={\\hfill%u %u %u %.2f}]\n",
      s->atk, s->def, s->sta, calc_fit(s->atk, s->def, s->sta));
  if(s->attacks){ // FIXME remove conditional once all attacks are defined
    printf("\\begin{tabular}{lrrr}\n");
    for(const attack** a = s->attacks ; *a ; ++a){
      if((*a)->energytrain < 0){
        printf("%s & & %u & %d\\\\\n", (*a)->name, (*a)->powertrain, (*a)->energytrain);
      }else{
        printf("%s & %u & %u & %d\\\\\n", (*a)->name, (*a)->turns,
                (*a)->powertrain, (*a)->energytrain);
      }
    }
    printf("\\end{tabular}\n");
  }
  printf("\\begin{tabular}{lr}\n");
  print_optimal_latex(s);
  printf("\\end{tabular}\n");
  printf("\\tcblower\n");
  printf("\\hfill\\includegraphics[width=1in,height=1in]{images/pokédex/");
  for(const char* curs = s->name ; *curs ; ++curs){
    if(*curs != '%' && *curs != '\''){
      printf("%c", *curs);
    }
  }
  printf(".png}\n");
  if(s->shadow){
    printf("\\includegraphics[width=1em,height=1em]{images/shadow.png}");
  }
  if(s->shiny){
    printf("\\includegraphics[width=1em,height=1em]{images/shiny.png}");
  }
  print_types(s->t1, s->t2);
  printf("\\end{tcolorbox}\n");
}

// print those entries containing type(s). pass TYPECOUNT for a wildcard on t2.
// pass the same type twice for only that base type. LaTeX output.
static void
filter_by_types(int t1, int t2){
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    bool printit = false;
    if(sdex[i].t1 == t1){
      if(sdex[i].t2 == t2 || t2 == TYPECOUNT || (t1 == t2 && sdex[i].t2 == TYPECOUNT)){
        printit = true;
      }
    }
    if(printit){
      print_species_latex(&sdex[i]);
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
    // we want to retain the order of typings here, so do both ways
    for(int t = TYPESTART ; t < static_cast<int>(TYPECOUNT) ; ++t){
      // we always want the pure typing first
      filter_by_types(t, t);
      for(int t2 = TYPESTART ; t2 < static_cast<int>(TYPECOUNT) ; ++t2){
        if(t != t2){
          filter_by_types(t, t2);
        }
      }
    }
  }
  return EXIT_SUCCESS;
}
