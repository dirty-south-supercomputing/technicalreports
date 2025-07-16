#include "pgotypes.cpp"

int main(void){
  int genpop[GENERATION_COUNT] = {};
  unsigned lastpdex = 0;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const auto s = &sdex[i];
    if(s->idx == lastpdex){ // skip alternate forms
      continue;
    }
    lastpdex = s->idx;
    const int gen = idx_to_generation_int(s->idx);
    if(gen < 0){
      std::cerr << "couldn't get generation for " << s->idx << std::endl;
      return EXIT_FAILURE;
    }
    ++genpop[gen];
  }
  printf("\\begin{table}\\centering\\begin{tabular}{lcrr}");
  printf("Region & Generation & Start & End\\\\\\Midrule");
  for(int i = 0 ; i < GENERATION_COUNT ; ++i){
    int first = generation_idx_first(i);
    int last = generation_idx_last(i);
    printf("%s & %s & \\#%04d & \\#%04d & %d\\\\",
        idx_to_region(first), idx_to_generation(first), first, last, genpop[i]);
  }
  printf("\\end{tabular}\\caption[Regions of the Pokémon world]{Regions of the Pokémon world (\\textjapanese{ポケモンの世界})\\label{table:regions}}");
  printf("\\end{table}");
  return EXIT_SUCCESS;
}
