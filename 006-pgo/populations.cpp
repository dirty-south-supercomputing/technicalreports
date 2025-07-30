#include "pgotypes.cpp"

static int
print_populations(const char **types){
  int t[TYPECOUNT] = {};
  std::cout << "Looking at types:";
  while(*types){
    std::cout << "*****" << *types << "*****";
    pgo_types_e e = lookup_type(*types);
    if(e == TYPECOUNT){
      std::cerr << "couldn't find type " << *types << std::endl;
      return -1;
    }
    if(t[e]){
      std::cerr << "provided type twice: " << *types << std::endl;
      return -1;
    }
    t[e] = 1;
    ++types;
    std::cout << " " << tnames[e];
  }
  std::cout << std::endl;
  int total = 0;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const species &s = sdex[i];
    if(t[s.t1] || (s.t2 != TYPECOUNT && t[s.t2])){
      std::cout << s.name << std::endl;
      ++total;
    }
  }
  std::cout << total << " total" << std::endl;
  return 0;
}

// generate table showing populations of monotypes and types in which they
// have a presence, or if provided arguments, print numbers of typings
// including those types.
int main(int argc, const char **argv){
  if(argc >= 2){
    if(print_populations(argv + 1)){
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
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
