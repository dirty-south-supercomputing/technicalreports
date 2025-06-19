#include "pgotypes.cpp"

// generate table showing populations of monotypes and types in which they
// have a presence
int main(void){
  puts("\\begin{table}[ht]");
  puts("\\begin{centering}");
  puts("\\begin{tabular}{lrrr}");
  puts("Type & Pop & Dualtypes & Pop\\\\\n");
  puts("\\Midrule\n");
  for(int t = 0 ; t < TYPECOUNT ; ++t){
    unsigned pop = 0;
    unsigned dts = 0;
    unsigned dtpop = 0;
    for(auto &sd : sdexen){
      if(sd.dex){
        for(unsigned i = 0 ; i < sd.dcount ; ++i){
          const species *s = &sd.dex[i];
          if(s->t1 == t && (s->t2 == t || s->t2 == TYPECOUNT)){
            ++pop;
          }else if(s->t1 == t || s->t2 == t){
            // FIXME handle dts
            ++dtpop;
          }
        }
      }
    }
    printf("%s & %u & %u & %u\\\\\n", TNames[t], pop, dts, dtpop);
  }
  puts("\\end{tabular}");
  puts("\\end{centering}");
  puts("\\caption{Population of types}");
  puts("\\label{table:populations}");
  puts("\\end{table}");
  return EXIT_SUCCESS;
}
