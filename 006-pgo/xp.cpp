#include "pgotypes.h"

static void emitk(uint64_t val){
  if(val < 1000000){
    std::cout << (val / 1000.0) << "k";
  }else{
    std::cout << (val / 1000000.0) << "M";
  }
}

static void level_item_table(int llevel, int hlevel){
  std::cout << "\\begin{table}";
  std::cout << "\\centering";
  std::cout << "\\setlength{\\tabcolsep}{1pt}";
  std::cout << "\\footnotesize";
  std::cout << "\\begin{tabular}{r|g c g c g c g c g c g c g c g c g c g c g c g}";
  std::cout << "&\
\\includegraphics[width=1em]{images/pokeball.png} &\
\\includegraphics[width=1em]{images/greatball.png} &\
\\includegraphics[width=1em]{images/ultraball.png} &\
\\includegraphics[width=1em]{images/nanab.png} &\
\\includegraphics[width=1em]{images/razz.png} &\
\\includegraphics[width=1em]{images/pinap.png} &\
\\includegraphics[width=1em]{images/silverpinap.png} &\
\\includegraphics[width=1em]{images/incense.png} &\
\\includegraphics[width=1em]{images/revive.png} &\
\\includegraphics[width=1em]{images/maxrevive.png} &\
\\includegraphics[width=1em]{images/Potion.png} &\
\\includegraphics[width=1em]{images/superpotion.png} &\
\\includegraphics[width=1em]{images/Hyper_Potion.png} &\
\\includegraphics[width=1em]{images/Max_Potion.png} &\
\\includegraphics[width=1em]{images/lure.png} &\
\\includegraphics[width=1em]{images/luckyegg.png} &\
\\includegraphics[width=1em]{images/incubatorlimited.png} &\
\\includegraphics[width=1em]{images/incubatorsuper.png} &\
\\includegraphics[width=1em]{images/rarecandyxl.png} &\
\\includegraphics[width=1em]{images/elitefasttm.png} &\
\\includegraphics[width=1em]{images/elitechargedtm.png}";
  std::cout << "\\end{tabular}";
  std::cout << "\\caption{Items awarded for reaching Trainer Levels ";
  std::cout << llevel << "--" << hlevel;
  std::cout << "\\label{table:level" << hlevel << "items}}";
  std::cout << "\\end{table}";
}

// write the trainer XP table
int main(int argc, const char **argv){
  if(argc > 1){
    if(argc != 3){
      std::cerr << "usage: " << argv[0] << " [ llevel hlevel ]" << std::endl;
      return EXIT_FAILURE;
    }
    int llevel = atoi(argv[1]);
    int hlevel = atoi(argv[2]);
    if(llevel <= 0 || hlevel <= 0 || llevel > MAXLEVEL || hlevel > MAXLEVEL || llevel > hlevel){
      std::cerr << "usage: " << argv[0] << " [ llevel hlevel ]" << std::endl;
      return EXIT_FAILURE;
    }
    std::cout << "ll: " << llevel << " hl: " << hlevel << std::endl;
    level_item_table(llevel, hlevel);
    return EXIT_SUCCESS;
  }
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
