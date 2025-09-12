#include "pgotypes.h"
#include <memory>
#include <iostream>

static int
index_sdex(const species *dex, unsigned dcount, int cols){
  int total = 0;
  for(unsigned i = 0 ; i < dcount ; ++i){
    const species *s = &dex[i];
    std::cout << s->name << " & ";
    std::cout << "\\pageref{species:" << s->name << "}";
    if(++total == cols){
      std::cout << "\\\\" << std::endl;
      total = 0;
    }else{
      std::cout << " & ";
    }
  }
  return 0;
}

// index for the species appendix
int main(void){
  std::cout << "\\begin{longtable}{lr|lr}";
  index_sdex(sdex, SPECIESCOUNT, 2);
  std::cout << "\\end{longtable}" << std::endl;
  return EXIT_SUCCESS;
}
