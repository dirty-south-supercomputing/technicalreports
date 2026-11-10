#include "html.h"

void usage(const char* argv0, int exitcode){
  std::cerr << "usage: " << argv0 << " form" << std::endl;
  exit(exitcode);
}

// determine the best max attackers against the specified pokémon
int main(int argc, const char** argv){
  if(argc != 2){
    usage(argv[0], EXIT_FAILURE);
  }
  const auto* s = lookup_species(argv[1]);
  if(!s){
    std::cerr << "couldn't find form " << argv[1] << std::endl;
    usage(argv[0], EXIT_FAILURE);
  }
  max_counters(std::cout, *s);
  return EXIT_SUCCESS;
}
