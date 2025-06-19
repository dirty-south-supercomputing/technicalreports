#include "pgotypes.cpp"

static int
twostagetable(void){
  puts("\\begin{center}");
  puts("\\footnotesize");
  puts("\\begin{longtable}{ll}");
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
  puts("\\begin{longtable}{ll}");
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
