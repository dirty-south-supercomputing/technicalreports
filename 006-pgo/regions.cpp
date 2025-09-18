#include "pgotypes.h"

static void
print_first_partners(int region){
    // FIXME print first partners in smaller font
  // first, determine that there *are* first partners from this region
  unsigned lastpdex = 0;
  bool firstfound = false;
  for(int i = 0 ; i < SPECIESCOUNT ; ++i){
    const auto s = &sdex[i];
    if(s->idx == lastpdex){ // skip alternate forms
      continue;
    }
    lastpdex = s->idx;
    const int gen = idx_to_region_int(s->idx);
    if(gen != region){
      continue;
    }
    if(s->category != species::CAT_FPARTNER){
      continue;
    }
    if(!firstfound){
      printf("\\multicolumn{4}{r}{\\footnotesize{}First partners: ");
      firstfound = true;
    }
    printf("%s ", s->name.c_str());
  }
  if(firstfound){
    printf("}\\\\");
  }
}

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
  printf("\\begin{table}\\centering\\begin{tabular}{lcrr}");
  printf("Region & Gen & Range & Pop\\\\\\Midrule\n");
  for(int i = 0 ; i < REGION_COUNT ; ++i){
    int first = region_idx_first(i);
    int last = region_idx_last(i);
    printf("%s & %s & \\#%04d--%04d & %d/%d\\\\\n",
        idx_to_region(first), idx_to_generation(first),
        first, last, regpop[i], last - first + 1);
    print_first_partners(i);
  }
  printf("\\end{tabular}\\caption[Regions of the Pokémon world]{Regions of the Pokémon world (\\textjapanese{ポケモンの世界}). Populations do not count multiple forms.\\label{table:regions}}");
  printf("\\end{table}");
  return EXIT_SUCCESS;
}
