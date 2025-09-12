#include "pgotypes.h"

// generate table with the typings having two extra weaknesses
int main(void){
  std::cout << "\\begin{table}\\footnotesize\\centering\\begin{tabular}{lp{.6\\textwidth}}" << std::endl;
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    for(int j = i ; j < TYPECOUNT ; ++j){
      // test t on i+j
      int firstdouble = TYPECOUNT;
      for(int t = 0 ; t < TYPECOUNT ; ++t){
        if(typing_relation(static_cast<pgo_types_e>(t), static_cast<pgo_types_e>(i), static_cast<pgo_types_e>(j)) == 2){
          if(firstdouble == TYPECOUNT){
            firstdouble = t;
          }else{
            std::cout << TNames[firstdouble] << ", " << TNames[t] << " → " << TNames[i] << "/" << TNames[j];
            std::cout << " & ";
            bool firstprint = true;
            for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
              const auto &s = sdex[u];
              if(s.t1 == i || s.t2 == i){
                if(s.t1 == j || s.t2 == j){
                  if(firstprint){
                    firstprint = false;
                  }else{
                    std::cout << ", ";
                  }
                  escape_cpp_string(s.name);
                }
              }
            }
            // FIXME list mega mons
            std::cout << "\\\\" << std::endl;
          }
        }
      }
    }
  }
  std::cout << "\\end{tabular}\\caption{The eight bidoubly weak typings\\label{table:doubledoubles}}\\end{table}" << std::endl;
}
