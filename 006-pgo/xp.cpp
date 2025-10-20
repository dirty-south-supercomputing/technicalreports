#include "pgotypes.h"

struct TrainerLevel {
  unsigned pokeball;
  unsigned greatball;
  unsigned ultraball;
  unsigned nanab;
  unsigned razz;
  unsigned incense;
  unsigned revive;
  unsigned potion;
  unsigned lure;
  unsigned luckyegg;
};

static TrainerLevel levels[MAXLEVEL] = {
  {}, // 0
  { 50, 0, 0, 0,  0, 1,  0,  0, 0, 0, }, // 1
  { 10, 0, 0, 5,  0, 0,  0,  0, 0, 0, }, // 2
  { 10, 0, 0, 5,  0, 0,  0,  0, 0, 0, }, // 3
  { 10, 0, 0, 5,  0, 0,  0,  0, 0, 0, }, // 4
  { 15, 0, 0, 0,  0, 1,  5, 10, 0, 0, }, // 5
  { 10, 0, 0, 0,  0, 0,  0,  5, 1, 0, }, // 6
  { 10, 0, 0, 5,  0, 0,  0,  0, 0, 0, }, // 7
  { 10, 0, 0, 0,  0, 0,  5,  0, 0, 0, }, // 8
  { 10, 0, 0, 5,  0, 0,  0,  0, 0, 0, }, // 9
  { 20, 0, 0, 0, 10, 0, 10, 10, 0, 1, }, // 10
 // FIXME
};

/*
\begin{table}
\centering
\setlength{\tabcolsep}{1pt}
\footnotesize
\begin{tabular}{r|g c g c g c g c g c g c g c g c g c g c g c g}
&
\includegraphics[width=1em]{images/pokeball.png} &
\includegraphics[width=1em]{images/greatball.png} &
\includegraphics[width=1em]{images/ultraball.png} &
\includegraphics[width=1em]{images/nanab.png} &
\includegraphics[width=1em]{images/razz.png} &
\includegraphics[width=1em]{images/pinap.png} &
\includegraphics[width=1em]{images/silverpinap.png} &
\includegraphics[width=1em]{images/incense.png} &
\includegraphics[width=1em]{images/revive.png} &
\includegraphics[width=1em]{images/maxrevive.png} &
\includegraphics[width=1em]{images/Potion.png} &
\includegraphics[width=1em]{images/superpotion.png} &
\includegraphics[width=1em]{images/Hyper_Potion.png} &
\includegraphics[width=1em]{images/Max_Potion.png} &
\includegraphics[width=1em]{images/lure.png} &
\includegraphics[width=1em]{images/luckyegg.png} &
\includegraphics[width=1em]{images/incubatorlimited.png} &
\includegraphics[width=1em]{images/incubatorsuper.png} &
\includegraphics[width=1em]{images/rarecandyxl.png} &
\includegraphics[width=1em]{images/elitefasttm.png} &
\includegraphics[width=1em]{images/elitechargedtm.png}
  \\
  2 & 10 &    &    &  5 &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
  3 & 10 &    &    &  5 &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
  4 & 10 &    &    &  5 &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
  5 & 15 &    &    &    &    &    &    & 1 &  5 &    & 10 &    &    &    &   &   &   &   &   &   &   \\
  6 & 10 &    &    &    &    &    &    &   &    &    &  5 &    &    &    &   &   & 1 &   &   &   &   \\
  7 & 10 &    &    &  5 &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
  8 & 10 &    &    &    &    &    &    &   &  5 &    &    &    &    &    &   &   &   &   &   &   &   \\
  9 & 10 &    &    &    &  5 &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
  10 & 20 &    &    &    & 10 &    &    &   & 10 &    & 10 &    &    &    &   & 1 & 1 &   &   &   &   \\%item storage upgrade
  11 & 10 &    &    &    &  5 &    &    &   &    &    &  5 &    &    &    &   &   &   &   &   &   &   \\
  12 & 10 &    &    &    &  5 &    &    &   &  5 &    &    &    &    &    &   &   &   &   &   &   &   \\
  13 & 10 &    &    &  5 &    &    &    &   &    &    &  5 &    &    &    &   &   &   &   &   &   &   \\%1x golden razz
  14 &    & 10 &    &    &  5 &    &    &   &  5 &    &    &    &    &    &   &   &   &   &   &   &   \\
  15 &    & 15 &    &    & 10 &    &    & 1 & 10 &    &    &    & 20 &    & 1 & 1 & 1 &   &   &   &   \\
  16 &    & 10 &    &  5 &    &    &    &   &  5 &    &    &    & 10 &    &   &   &   &   &   &   &   \\
  17 &    & 10 &    &    &  5 &    &    &   &  5 &    &    &    & 10 &    &   &   &   &   &   &   &   \\
  18 &    & 10 &    &    &    &  5 &    &   &  5 &    &    &    & 10 &    &   &   &   &   &   &   &   \\
  19 &    & 15 &    &    &  5 &    &    &   &  5 &    &    &    & 10 &    &   &   &   &   &   &   &   \\
  20 &    &    & 20 & 20 &    &    &    & 2 & 20 &    &    &    & 20 &    & 2 & 2 & 2 &   &   &   &   \\
  21 &    &    & 10 &    &    & 10 &    &   & 10 &    &    &    & 10 &    &   &   &   &   &   &   &   \\
  22 &    &    & 10 &    & 10 &    &    &   & 10 &    &    &    & 10 &    &   &   &   &   &   &   &   \\
  23 &    &    & 10 & 10 &    &    &    &   & 10 &    &    &    & 10 &    &   &   &   &   &   &   &   \\
  24 &    &    & 15 &    & 10 &    &    &   & 10 &    &    &    & 10 &    &   &   &   &   &   &   &   \\
  25 &    &    & 25 &    &    & 15 &    & 1 & 15 &    &    &    &    & 20 & 1 & 1 & 1 &   &   &   &   \\
  26 &    &    & 10 &    & 15 &    &    &   & 10 &    &    &    &    & 15 &   &   &   &   &   &   &   \\
  27 &    &    & 10 & 15 &    &    &    &   & 10 &    &    &    &    & 15 &   &   &   &   &   &   &   \\
  28 &    &    & 10 &    & 15 &    &    &   & 10 &    &    &    &    & 15 &   &   &   &   &   &   &   \\
  29 &    &    & 10 &    &    & 15 &    &   & 10 &    &    &    &    & 15 &   &   &   &   &   &   &   \\
  30 &    &    & 30 &    & 20 &    &    & 3 &    & 20 &    &    &    & 20 & 3 & 3 & 3 &   &   &   &   \\
  31 &    &    & 10 & 15 &    &    &    &   &    & 10 &    &    &    & 15 &   &   &   &   &   &   &   \\
  32 &    &    & 10 &    & 15 &    &    &   &    & 10 &    &    &    & 15 &   &   &   &   &   &   &   \\
  33 &    &    & 10 &    &    & 15 &    &   &    & 10 &    &    &    & 15 &   &   &   &   &   &   &   \\
  34 &    &    & 10 &    & 15 &    &    &   &    & 10 &    &    &    & 15 &   &   &   &   &   &   &   \\
  35 &    &    & 30 & 20 &    &    &    & 2 &    & 20 &    &    &    & 20 & 1 & 1 & 1 &   &   &   &   \\
  36 &    &    & 20 &    & 20 &    &    &   &    & 10 &    &    &    & 20 &   &   &   &   &   &   &   \\
  37 &    &    & 20 &    &    & 20 &    &   &    & 10 &    &    &    & 20 &   &   &   &   &   &   &   \\
  38 &    &    & 20 &    & 20 &    &    &   &    & 10 &    &    &    & 20 &   &   &   &   &   &   &   \\
  39 &    &    & 20 & 20 &    &    &    &   &    & 10 &    &    &    & 20 &   &   &   &   &   &   &   \\
  40 &    &    & 40 &    & 40 &    &    & 4 &    & 40 &    &    &    & 40 & 4 & 4 & 4 &   &   &   &   \\
\end{tabular}
\caption{Items awarded for reaching Trainer Levels 2--40\label{table:levelitems}}
\end{table}
\begin{table}
\centering
\setlength{\tabcolsep}{1pt}
\footnotesize
\begin{tabular}{r|g c g c g c g c g c g c g c g c g c g c g c g}
&
\includegraphics[width=1em]{images/pokeball.png} &
\includegraphics[width=1em]{images/greatball.png} &
\includegraphics[width=1em]{images/ultraball.png} &
\includegraphics[width=1em]{images/nanab.png} &
\includegraphics[width=1em]{images/razz.png} &
\includegraphics[width=1em]{images/pinap.png} &
\includegraphics[width=1em]{images/silverpinap.png} &
\includegraphics[width=1em]{images/incense.png} &
\includegraphics[width=1em]{images/revive.png} &
\includegraphics[width=1em]{images/maxrevive.png} &
\includegraphics[width=1em]{images/Potion.png} &
\includegraphics[width=1em]{images/superpotion.png} &
\includegraphics[width=1em]{images/Hyper_Potion.png} &
\includegraphics[width=1em]{images/Max_Potion.png} &
\includegraphics[width=1em]{images/lure.png} &
\includegraphics[width=1em]{images/luckyegg.png} &
\includegraphics[width=1em]{images/incubatorlimited.png} &
\includegraphics[width=1em]{images/incubatorsuper.png} &
\includegraphics[width=1em]{images/rarecandyxl.png} &
\includegraphics[width=1em]{images/elitefasttm.png} &
\includegraphics[width=1em]{images/elitechargedtm.png}
  \\
41 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
42 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
43 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
44 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
45 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
46 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
47 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
48 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
49 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
50 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
51 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
52 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
53 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
54 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
55 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
56 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
57 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
58 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
59 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
60 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
61 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
62 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
63 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
64 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
65 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
66 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
67 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
68 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
69 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
70 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
71 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
72 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
73 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
74 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
75 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
76 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
77 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
78 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
79 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\
80 &    &    &    &    &    &    &    &   &    &    &    &    &    &    &   &   &   &   &   &   &   \\

\end{tabular}
\caption{Items awarded for reaching Trainer Levels 41--80\label{table:levelitems41}}
\end{table}
*/

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
\\includegraphics[width=1em]{images/elitechargedtm.png}" << "\\\\";
  for(int x = llevel ; x <= hlevel ; ++x){
    const TrainerLevel &l = levels[x];
    std::cout << x << "&";
    std::cout << l.pokeball << "&";
    std::cout << l.greatball << "&";
    std::cout << l.ultraball << "&";
    std::cout << l.nanab << "&";
    std::cout << l.razz << "&";
    std::cout << "\\\\" << std::endl;
  }
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
