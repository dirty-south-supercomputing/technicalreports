#include "html.h"

[[noreturn]] void usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " [ -s ]" << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, const char **argv){
  if(argc > 2){
    usage(argv[0]);
  }else if(argc == 2){
    if(strcmp(argv[1], "-s")){
      usage(argv[0]);
    }
    write_navbar_selectopts(std::cout);
    return EXIT_SUCCESS;
  }
  std::cout << "<body>" << std::endl;
  write_navbar(std::cout);
  return EXIT_SUCCESS;
}
