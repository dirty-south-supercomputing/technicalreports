#include "pgotypes.cpp"

static int
print_evolutions(int depth){
  for(auto &dex : sdexen){
    for(unsigned u = 0 ; u < dex.dcount ; ++u){
      const species *s = &dex.dex[u];
      if(s->from){
        const species *s1 = lookup_species(s->from);
        if(s1 == NULL){
          return -1;
        }
        if(depth == 2){
          if(s1->from){
            continue;
          }
          // FIXME ensure nothing links to s
        }else if(depth == 3){
          if(!s1->from){
            continue;
          }
          const species *s2 = lookup_species(s1->from);
          if(s2 == NULL){
            return -1;
          }
          if(s2->from){
            continue;
          }
          escape_string(s2->name.c_str());
          puts(" & ");
        }else{
          return -1;
        }
        escape_string(s1->name.c_str());
        puts(" & ");
        escape_string(s->name.c_str());
        puts("\\\\");
      }
    }
  }
  return 0;
}

static int
twostagetable(void){
  puts("\\begin{center}");
  puts("\\footnotesize");
  puts("\\begin{longtable}{lcl}");
  puts("Stage 1 & Requirements & Stage 2\\\\");
  puts("\\Midrule");
  if(print_evolutions(2)){
    return -1;
  }
  puts("\\caption{Two-stage evolutionary paths}");
  puts("\\label{table:twostage}");
  puts("\\end{longtable}");
  puts("\\end{center}");
  return 0;
}

static int
threestagetable(void){
  puts("\\begin{center}");
  puts("\\footnotesize");
  puts("\\begin{longtable}{lclcl}");
  puts("Stage 1 & Requirements & Stage 2 & Requirements & Stage 3\\\\");
  puts("\\Midrule");
  if(print_evolutions(3)){
    return -1;
  }
  puts("\\caption{Three-stage evolutionary paths}");
  puts("\\label{table:threestage}");
  puts("\\end{longtable}");
  puts("\\end{center}");
  return 0;
}

// table of all evolutions
int main(void){
  twostagetable();
  threestagetable();
  return EXIT_SUCCESS;
}
