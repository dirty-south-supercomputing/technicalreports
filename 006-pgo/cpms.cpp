#include "pgotypes.cpp"

static void
print_cpms(void){
  std::cout.precision(4);
  std::cout << "\\begin{table}\\centering\\begin{tabular}{ll|ll|ll}" << std::endl;
  std::cout << "Level & \\CPM & Level & \\CPM & Level & \\CPM\\\\" << std::endl;
  std::cout << "\\Midrule" << std::endl;
  for(int i = 1 ; i < 34 ; ++i){
    unsigned half;
    auto level = halflevel_to_level(i, &half);
    std::cout << level << '.';
    if(half){
      std::cout << "5";
    }else{
      std::cout << "0";
    }
    std::cout << " & " << cpm(i) << " & ";
    level = halflevel_to_level(i + 33, &half);
    if(half){
      std::cout << level << '.';
      std::cout << "5";
    }else{
      std::cout << level << '.';
      std::cout << "0";
    }
    std::cout << " & " << cpm(i + 33) << " & ";
    level = halflevel_to_level(i + 66, &half);
    std::cout << level << '.';
    if(half){
      std::cout << "5";
    }else{
      std::cout << "0";
    }
    std::cout << " & " << cpm(i + 66) << "\\\\" << std::endl;
  }
  std::cout << "\\end{tabular}\\caption{Idealized combat power multipliers\\label{table:cpm}}\\end{table}" << std::endl;
}

int main(void){
  print_cpms();
}
