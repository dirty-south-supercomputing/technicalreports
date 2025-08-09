#include "pgotypes.cpp"

static void elite_fast_table(const species *dex, unsigned scount){
  std::cout << "\\begin{table}\\centering";
  std::cout << "\\begin{tabular}{ll}";
  std::cout << "Pokémon & Attack\\\\\\Midrule" << std::endl;
  for(unsigned i = 0 ; i < scount ; ++i){
    const species *s = &dex[i];
    for(const auto *a : s->attacks){
      if(a->energytrain < 0){
        continue;
      }
      for(const auto *e : s->elite){
        if(a == e){
          std::cout << s->name << " & " << a->name << "\\\\" << std::endl;
        }
      }
    }
  }
  std::cout << "\\end{tabular}";
  std::cout << "\\caption{Elite fast moves\\label{table:elitefast}}";
  std::cout << "\\end{table}" << std::endl;
}

static void elite_charged_table(const species *dex, unsigned scount){
  std::cout << "\\begin{table}\\centering";
  std::cout << "\\begin{tabular}{ll}";
  std::cout << "Pokémon & Attack\\\\\\Midrule" << std::endl;
  for(unsigned i = 0 ; i < scount ; ++i){
    const species *s = &dex[i];
    for(const auto *a : s->attacks){
      if(a->energytrain >= 0){
        continue;
      }
      for(const auto *e : s->elite){
        if(a == e){
          std::cout << s->name << " & " << a->name << "\\\\" << std::endl;
        }
      }
    }
  }
  std::cout << "\\end{tabular}";
  std::cout << "\\caption{Elite charged moves\\label{table:elitecharged}}";
  std::cout << "\\end{table}" << std::endl;
}

int main(void){
  elite_fast_table(sdex, SPECIESCOUNT);
  elite_charged_table(sdex, SPECIESCOUNT);
  return EXIT_SUCCESS;
}
