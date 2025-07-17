#include "pgotypes.cpp"

int main(void){
  int regpop[REGION_COUNT] = {};
  unsigned lastpdex = 0;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const auto s = &sdex[i];
    if(s->idx == lastpdex){ // skip alternate forms
      continue;
    }
    lastpdex = s->idx;
    const int gen = idx_to_region_int(s->idx);
    if(gen < 0){
      std::cerr << "couldn't get region for " << s->idx << std::endl;
      return EXIT_FAILURE;
    }
    ++regpop[gen];
  }
  printf("\\begin{table}\\centering\\begin{tabular}{lcrrr}");
  printf("Region & Generation & Start & End & Pop\\\\\\Midrule\n");
  for(int i = 0 ; i < REGION_COUNT ; ++i){
    int first = region_idx_first(i);
    int last = region_idx_last(i);
    printf("%s & %s & \\#%04d & \\#%04d & %d\\\\\n",
        idx_to_region(first), idx_to_generation(first), first, last, regpop[i]);
  }
  printf("\\end{tabular}\\caption[Regions of the Pokémon world]{Regions of the Pokémon world (\\textjapanese{ポケモンの世界}). Populations do not count multiple forms.\\label{table:regions}}");
  printf("\\end{table}");
  return EXIT_SUCCESS;
}
