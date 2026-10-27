#include "html.h"
#include <cstdlib>
#include <fstream>
#include <unistd.h>

static int
write_iv_graph(const species &s, const char *leagueshort){
  bool graph = !s.name.contains("G-Max"); // FIXME
  if(graph){
    std::string cmd = "Rscript ../../gmean.R \"" + s.name + "\" " + leagueshort;
    if(system(cmd.c_str())){ // ugh
      std::cerr << "Rscript gmean failed for " << s.name << std::endl;
      return -1;
    }
    std::string radar = "Rscript ../../optstatsradar.R \"" + s.name + "\" " + leagueshort;
    if(system(radar.c_str())){ // ugh
      std::cerr << "Rscript radar failed for " << s.name << std::endl;
      return -1;
    }
  }
  return 0;
}

static int
write_pacing(const species &s){
  std::string pacing = "cd graphs && ../../../out/pacing \"" + s.name + "\"";
  if(system(pacing.c_str())){
    std::cerr << "pacing failed for " << s.name << std::endl;
    return -1;
  }
  return 0;
}

static void
write_iv_graphs(const species &s){
  if(!pvp_banned_p(s)){
    if(write_pacing(s)){
      std::cerr << "pacing failed for " << s.name << std::endl;
      throw std::exception();
    }
    if(write_iv_graph(s, "GL")){
      throw std::exception();
    }
    // FIXME i'd like to check these, as well; we just need know whether we ought
    // be creating ul/ml for a various form (we can always generate GL)
    if(write_iv_graph(s, "UL")){
      std::cerr << "warning: ul generation failed for " << s.name << std::endl;
      return;
    }
    write_iv_graph(s, "ML");
  }
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
  // FIXME parallelize this abortion
  std::vector<species> megas;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(write_mon_graphs(*s)){
      return EXIT_FAILURE;
    }
    for(const auto &m : s->mforms){
      megas.emplace_back(&*s, m);
    }
  }
  for(const auto &speg : megas){
    if(write_mon_graphs(speg)){
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
