#include "html.h"

// we want all 324 (i.e. duplicate rows for dyadic typings)
static void
gen_te_table(){
  for(pgo_types_e t1 = TYPESTART ; t1 < TYPECOUNT ; ++t1){
    for(pgo_types_e t2 = TYPESTART ; t2 < TYPECOUNT ; ++t2){
      std::cout << "<tr>";
      std::cout << "<td>";
      html_types(t1, t2);
      std::cout << "</td>";
      for(pgo_types_e at = TYPESTART ; at < TYPECOUNT ; ++at){
        std::cout << "<td>";
        auto tr = typing_relation(at, t1, t2);
        if(tr){
          std::cout << tr;
        }
        std::cout << "</td>";
      }
      std::cout << "</tr>" << std::endl;
    }
  }
}

// matrix of attack types and all 324 type effectiveness relations
int main(){
  std::cout << "<table class=\"evenshade\" id=\"teffect\">" << std::endl;
  std::cout << "<tr>";
  std::cout << "<th>T</th>";
  for(pgo_types_e t = TYPESTART ; t < TYPECOUNT ; ++t){
    std::cout << "<th>";
    html_type(t);
    std::cout << "</th>";
  }
  std::cout << "</tr>" << std::endl;
  gen_te_table();
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
