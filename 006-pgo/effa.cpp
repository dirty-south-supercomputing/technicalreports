#include "html.h"

[[noreturn]] static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " GL|UL|ML" << std::endl;
  exit(EXIT_FAILURE);
}

// generate SVG CMP graph for a league
int main(int argc, const char **argv){
  if(argc != 2){
    usage(argv[0]);
  }
  std::vector<cmprange> effas;
  if(strcmp(argv[1], "GL") == 0){
    calc_cmp_graph(effas, 0, GLCPCAP);
  }else if(strcmp(argv[1], "UL") == 0){
    calc_cmp_graph(effas, GLCPCAP, ULCPCAP);
  }else if(strcmp(argv[1], "ML") == 0){
    calc_cmp_graph(effas, ULCPCAP, 0);
  }else{
    usage(argv[0]);
  }
  emit_cmp_graph(std::cout, effas);
  return EXIT_SUCCESS;
}
