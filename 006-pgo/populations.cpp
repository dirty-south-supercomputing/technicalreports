#include "pgotypes.cpp"

// generate table showing populations of monotypes and types in which they
// have a presence
int main(void){
  puts("\\begin{table}[ht]");
  puts("\\begin{center}");
  puts("\\begin{tabular}{lrrrr}");
  puts("Type & Pop & Dualtypes & Pop & Total\\\\\n");
  puts("\\Midrule\n");
  for(int t = 0 ; t < TYPECOUNT ; ++t){
    bool duals[TYPECOUNT] = {};
    unsigned pop = 0;
    unsigned dts = 0;
    unsigned dtpop = 0;
    for(auto &sd : sdexen){
      if(sd.dex){
        for(unsigned i = 0 ; i < sd.dcount ; ++i){
          const species *s = &sd.dex[i];
          if(s->t1 == t && (s->t2 == t || s->t2 == TYPECOUNT)){
            ++pop;
          }else if(s->t1 == t){
            if(!duals[s->t2]){
              ++dts;
            }
            duals[s->t2] = true;
            ++dtpop;
          }else if(s->t2 == t){
            if(!duals[s->t1]){
              ++dts;
            }
            duals[s->t1] = true;
            ++dtpop;
          }
        }
      }
    }
    printf("%s & %u & %u & %u & %u\\\\\n",
        TNames[t], pop, dts, dtpop, pop + dtpop);
  }
  puts("\\end{tabular}");
  puts("\\end{center}");
  puts("\\caption{Population of types}");
  puts("\\label{table:populations}");
  puts("\\end{table}");
  return EXIT_SUCCESS;
}
