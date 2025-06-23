#include "pgotypes.cpp"
#include <memory>
#include <iostream>

static int
scmp(const void *va1, const void *va2){
  const species *s1 = static_cast<const species*>(va1);
  const species *s2 = static_cast<const species*>(va2);
  return strcmp(s1->name.c_str(), s2->name.c_str());
}

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
