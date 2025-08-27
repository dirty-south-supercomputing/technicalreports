#include "pgotypes.cpp"

// list all unpopulated typings
int main(void){
  std::cout << "\\begin{table}[ht]\\centering\\begin{tabular}{c|c}" << std::endl;
  int count = 0;
  bool lhs = true;
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
        if( (lhs = !lhs) ){
          std::cout << "\\\\" << std::endl;
        }else{
          std::cout << " & ";
        }
      }
    }
  }
  std::cout << "\\end{tabular}\\caption{The " << count << " unpopulated typings\\label{table:unpopulated}}\\end{table}" << std::endl;
  std::cout << count << " typings are currently unpopulated (\\autoref{table:unpopulated}), ";
  std::cout << "leaving " << (TYPINGCOUNT - count) << " defender typings to consider." << std::endl;
  return EXIT_SUCCESS;
}
