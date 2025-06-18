#include "pgotypes.cpp"

// generate table showing populations of monotypes and types in which they
// have a presence
int main(void){
  puts("\\begin{table}[ht]");
  puts("\\begin{centering}");
  puts("\\begin{tabular}{lrrr}");
  for(int t = 0 ; t < TYPECOUNT ; ++t){
    puts("Type & Pop & Dualtypes & Pop\\\\\n");
    puts("\\Midrule\n");
    unsigned pop = 0;
    unsigned dts = 0;
    unsigned dtpop = 0;
    printf("%s & %u & %u & %u\\\\\n", TNames[t], pop, dts, dtpop);
  }
  puts("\\end{tabular}");
  puts("\\end{centering}");
  puts("\\caption{Population of types}");
  puts("\\label{table:populations}");
  puts("\\end{table}");
  return EXIT_SUCCESS;
}
