#include "pgotypes.h"

// build the 171 typesets
static void
build_tsets_full(std::vector<typeset> &tsets){
  for(pgo_types_e t0 = TYPESTART ; t0 < TYPECOUNT ; ++t0){
    for(pgo_types_e t1 = t0 ; t1 < TYPECOUNT ; ++t1){
      build_tset(tsets, t0, t1);
    }
  }
}

int main(){
  std::vector<typeset> tsets;
  build_tsets_full(tsets);
  std::sort(tsets.begin(), tsets.end(), std::greater<typeset>());
  std::cout << std::fixed << std::setprecision(3);
  std::cout << "<table class=\"evenshade\">" << std::endl;
  std::cout << "<tr>";
  std::cout << "<th>T</th>";
  std::cout << "<th>E<sub>-3</sub></th>";
  std::cout << "<th>E<sub>-2</sub></th>";
  std::cout << "<th>E<sub>-1</sub></th>";
  std::cout << "<th>E<sub>0</sub></th>";
  std::cout << "<th>E<sub>1</sub></th>";
  std::cout << "<th>E<sub>2</sub></th>";
  std::cout << "<th>ARA</th>";
  std::cout << "<th style=\"width: 80%\">Pop</th>";
  std::cout << "</tr>" << std::endl;
  for(const auto &ts : tsets){
    std::cout << "<tr><td>";
    html_types(ts.t0, ts.t1);
    std::cout << "</td>";
    for(unsigned i = 0 ; i < sizeof(ts.totals) / sizeof(*ts.totals) ; ++i){
      std::cout << "<td>";
      if(ts.totals[i]){
        std::cout << ts.totals[i];
      }
      std::cout << "</td>";
    }
    std::cout << "<td>" << ts.ara << "</td>";
    std::cout << "<td>" << ts.learnpop.size() << "</td>";
    std::cout << "</tr>" << std::endl;
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
