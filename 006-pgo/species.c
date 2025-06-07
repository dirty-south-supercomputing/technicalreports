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
  stats* s2500 = find_optimal_set(s, 2500, 0);
  while(s2500){
    stats* tmp = s2500->next;
    // print s2500
    free(s2500);
    s2500 = tmp;
  }
}

static void
print_species_latex(const species* s){
  printf("\\begin{tcolorbox}[title=");
  for(const char* curs = s->name ; *curs ; ++curs){
    if(*curs != '%'){
      printf("%c", *curs);
    }else{
      printf("\\%%");
    }
  }
  printf(",sidebyside,lower separated=false,fonttitle=\\bfseries,after title={\\hfill%u %u %u}]\n",
      s->atk, s->def, s->sta);
  printf("\\includegraphics[width=1in,height=1in]{images/pokédex/");
  for(const char* curs = s->name ; *curs ; ++curs){
    if(*curs != '%' && *curs != '\''){
      printf("%c", *curs);
    }
  }
  printf(".png}\n\\tcblower\n");
  if(s->t1 != s->t2 && s->t2 != TYPECOUNT){
    printf("%s+%s\\\\\n", TNames[s->t1], TNames[s->t2]);
  }else{
    printf("%s\\\\\n", TNames[s->t1]);
  }
  if(s->shiny){
    printf("Shiny form available\\\\\n");
  }
  /*print_cp_bounded_latex(s, 1500);
  print_cp_bounded_latex(s, 2500);*/
  printf("\\end{tcolorbox}\n");
}

// print those entries containing type(s). pass TYPECOUNT for a wildcard on t2.
// pass the same type twice for only that base type. LaTeX output.
static void
filter_by_types(int t1, int t2){
  unsigned printed = false;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    bool printit = false;
    if(sdex[i].t1 == t1){
      if(sdex[i].t2 == t2 || t2 == TYPECOUNT || (t1 == t2 && sdex[i].t2 == TYPECOUNT)){
        printit = true;
      }
    }else if(sdex[i].t1 == t2 && sdex[i].t2 == t1){
      printit = true;
    }
    if(printit){
      /*if(printed++ == 0){
        printf("\\begin{longtable}{lrrr}\n");
        printf("Species & ATK & DEF & STA\\\\\n");
        printf("\\Midrule\n");
        printf("\\endhead\n");
      }*/
      print_species_latex(&sdex[i]);
    }
  }
  /*if(printed){
    printf("\\caption{%u species of type %s", printed, TNames[t1]);
    if(t1 != t2){
      printf("+%s", TNames[t2]);
    }
    printf("}\n");
    printf("\\end{longtable}\n");
  }*/
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
        filter_by_types(t, t2);
      }
    }
  }
  return EXIT_SUCCESS;
}
