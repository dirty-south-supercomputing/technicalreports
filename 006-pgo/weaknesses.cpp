#include "pgotypes.h"

void usage(const char* argv0){
  std::cerr << "usage: " << argv0 << " count" << std::endl;
  exit(EXIT_FAILURE);
}

// emit a table of typings having only n weaknesses.
int main(int argc, const char** argv){
  if(argc != 2){
    usage(argv[0]);
  }
  const int target = atoi(argv[1]);
  if(target < 1 || target > TYPECOUNT){
    usage(argv[0]);
  }
  std::cout << "\\begin{table}\\footnotesize\\centering\\begin{tabular}{lp{.6\\textwidth}}" << std::endl;
  for(pgo_types_e i = TYPESTART ; i < TYPECOUNT ; i = static_cast<pgo_types_e>(i + 1)){
    for(pgo_types_e j = i ; j < TYPECOUNT ; j = static_cast<pgo_types_e>(j + 1)){
      int tarr[TYPECOUNT]{};
      int weak = 0;
      for(int a = TYPESTART ; a < TYPECOUNT ; ++a){
        tarr[a] = typing_relation(static_cast<pgo_types_e>(a), i, j);
        if(tarr[a] > 0){
          ++weak;
        }
      }
      if(weak == target){
        bool firstprint = true;
        for(int a = 0 ; a < TYPECOUNT ; ++a){
          if(tarr[a] > 0){
            if(firstprint){
              firstprint = false;
            }else{
              std::cout << ", ";
            }
            std::cout << tname_capitalized(a);
          }
        }
        std::cout << " → " << tname_capitalized(i);
        if(i != j){
          std::cout << "/" << tname_capitalized(j);
        }
        std::cout << " & ";
        // monotype sets are too large to print
        if(i != j){
          emit_typing_list(i, j);
        }else{
          pgo_types_e t2 = (j == i) ? TYPECOUNT : j;
          std::cout << "\\textit{" << typing_popcount(i, t2) << " forms (page ";
          std::cout << "\\pageref{types:" << tnames[i] << "})}";
        }
        std::cout << "\\\\" << std::endl;
      }
    }
  }
  std::cout << "\\end{tabular}\\caption{Typings weak to only " << target << " type";
  if(target != 1){
    std::cout << "s";
  }
  std::cout << "\\label{table:weak" << target << "}}\\end{table}" << std::endl;
  return EXIT_SUCCESS;
}
