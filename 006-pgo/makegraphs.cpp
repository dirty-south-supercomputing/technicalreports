#include "html.h"
#include <cstdlib>
#include <fstream>
#include <unistd.h>

static void
write_iv_graph(const species &s, const char *leagueshort){
  bool graph = !s.name.contains("G-Max"); // FIXME
  if(graph){
    std::string cmd = "Rscript ../../gmean.R \"" + s.name + "\" " + leagueshort;
    if(system(cmd.c_str())){ // ugh
      std::cerr << "Rscript failed for " << s.name << std::endl;
      throw std::exception();
    }
    std::string radar = "Rscript ../../optstatsradar.R \"" + s.name + "\" " + leagueshort;
    if(system(radar.c_str())){ // ugh
      std::cerr << "Rscript radar failed for " << s.name << std::endl;
      throw std::exception();
    }
  }
}

static void
write_iv_graphs(const species &s){
  write_iv_graph(s, "GL");
  write_iv_graph(s, "UL");
  write_iv_graph(s, "ML");
}

static int
write_mon_graphs(const species &s){
  write_iv_graphs(s);
  return 0;
}

static void
usage(const char *argv0, int ret){
  std::cerr << "usage: " << argv0 << " targetdir" << std::endl;
  exit(ret);
}

// generate a directory full of species graphs, overwriting any that exist.
int main(int argc, const char **argv){
  if(argc != 2){
    usage(argv[0], EXIT_FAILURE);
  }
  if(chdir(argv[1])){
    std::cerr << "error changing directory to " << argv[1] << ": " << strerror(errno) << std::endl;
    usage(argv[0], EXIT_FAILURE);
  }
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(write_mon_graphs(*s)){
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
