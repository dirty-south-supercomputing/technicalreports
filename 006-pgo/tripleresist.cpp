#include "pgotypes.cpp"

// generate table with the typings having a triple resistance
int main(void){
  std::cout << "\\begin{table}\\begin{raggedright}\\centering\\begin{tabular}{lp{.6\\textwidth}}" << std::endl;
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    for(int j = i ; j < TYPECOUNT ; ++j){
      // test t on i+j
      for(int t = 0 ; t < TYPECOUNT ; ++t){
        if(typing_relation(static_cast<pgo_types_e>(t), static_cast<pgo_types_e>(i), static_cast<pgo_types_e>(j)) == -3){
          std::cout << TNames[t] << " → " << TNames[i] << "/" << TNames[j];
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
          if(firstprint){
            std::cout << "Unpopulated";
          }
          // FIXME list mega mons?
          std::cout << "\\\\" << std::endl;
        }
      }
    }
  }
  std::cout << "\\end{tabular}\\caption{The twenty triple resistances\\label{table:triples}}\\end{raggedright}\\end{table}" << std::endl;
}
