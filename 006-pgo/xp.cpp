#include "pgotypes.h"

static void emitk(uint64_t val){
  if(val < 1000000){
    std::cout << (val / 1000.0) << "k";
  }else{
    std::cout << (val / 1000000.0) << "M";
  }
}

// write the trainer XP table
int main(void){
  std::cout << "\\begin{table}";
  std::cout << "\\centering\\footnotesize";
  std::cout << "\\setlength{\\tabcolsep}{2pt}";
  std::cout << "\\begin{tabular}{rrr|rrr|rrr|rrr}" << std::endl;
  std::cout << "L & XP & ΔXP & L & XP & ΔXP & L & XP & ΔXP & L & XP & ΔXP\\\\" << std::endl;
  std::cout << "\\Midrule" << std::endl;
  constexpr int cols = 4;
  constexpr int percol = MAXLEVEL / cols;
  static_assert(MAXLEVEL == cols * percol);
  static_assert(MAXLEVEL == sizeof(LEVELREQS) / sizeof(*LEVELREQS));
  unsigned collast[cols] = {};
  for(int l = 0 ; l < percol ; ++l){
    for(int c = 0 ; c < cols ; ++c){
      auto level = c * percol + l + 1;
      std::cout << level << " & "; // level
      auto totalxp = LEVELREQS[level - 1];
      emitk(totalxp);
      std::cout << " & "; // total xp for level
      auto xpdiff = totalxp - collast[c];
      if(l == 0){ // we're the top of the column
        if(c == 0){
          xpdiff = 0;
        }else{
          xpdiff = totalxp - LEVELREQS[level - 2];
        }
      }
      emitk(xpdiff);
      if(c + 1 < cols){
        std::cout << " & ";
      }
      collast[c] = totalxp;
    }
    std::cout << "\\\\" << std::endl;
  }
  std::cout << "\\end{tabular}";
  std::cout << "\\caption{XP requirements for Trainer Levels 1--" << MAXLEVEL << "\\label{table:xp40}}";
  std::cout << "\\end{table}";
  return EXIT_SUCCESS;
}
