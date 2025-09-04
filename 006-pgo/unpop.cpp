#include "pgotypes.cpp"

// list all unpopulated typings. if the number changes, we'll probably want a different
// number of columns, so throw an exception if they're not full.
int main(void){
  std::cout << "\\begin{table}[ht]\\centering\\begin{tabular}{c|c|c|c}" << std::endl;
  int count = 0;
  int column = 0;
  constexpr int COLUMNS = 4;
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    for(int j = i ; j < TYPECOUNT ; ++j){
      bool found = false;
      for(unsigned u = 0 ; !found && u < SPECIESCOUNT ; ++u){
        const species &s = sdex[u];
        if((s.t1 == i && s.t2 == j) || (s.t1 == j && s.t2 == i) || (s.t1 == i && i == j && s.t2 == TYPECOUNT)){
          found = true;
        }
      }
      if(!found){
        std::cout << TNames[i] << "/" << TNames[j];
        ++count;
        if(++column == COLUMNS){
          std::cout << "\\\\" << std::endl;
          column = 0;
        }else{
          std::cout << " & ";
        }
      }
    }
  }
  if(column){
    std::cerr << "column didn't equal 0 following table!" << std::endl;
    return EXIT_FAILURE;
    /* if(column == 1){
      std::cout << " & ";
    }
    std::cout << "\\\\" << std::endl;*/
  }
  std::cout << "\\end{tabular}\\caption{The " << count << " unpopulated typings\\label{table:unpopulated}}\\end{table}" << std::endl;
  std::cout << count << " typings are currently unpopulated (\\autoref{table:unpopulated}), ";
  std::cout << "leaving " << (TYPINGCOUNT - count) << " defender typings to consider." << std::endl;
  return EXIT_SUCCESS;
}
