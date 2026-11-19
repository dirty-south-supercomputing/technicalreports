#include "html.h"

/* css
div.speciesivs {
  margin: auto;
}

table.speciesivs {
  margin: auto;
}
*/

int main(int argc, const char **argv){
  if(argc != 2){
    return EXIT_FAILURE;
  }
  const auto *s = lookup_species(argv[1]);
  if(!s){
    std::cerr << "error looking up " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }
  write_family_iv_page(std::cout, *s, 15, 15, 15);
  return EXIT_SUCCESS;
}
