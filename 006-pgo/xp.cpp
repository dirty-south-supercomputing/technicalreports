#include "pgotypes.h"

struct TrainerLevel {
  unsigned pokeball;
  unsigned greatball;
  unsigned ultraball;
  unsigned nanab;
  unsigned razz;
  unsigned pinap;
  unsigned goldenrazz;
  unsigned incense;
  unsigned revive;
  unsigned maxrevive;
  unsigned potion;
  unsigned superpotion;
  unsigned hyperpotion;
  unsigned maxpotion;
  unsigned lure;
  unsigned luckyegg;
  unsigned starpiece;
  unsigned incubator;
  unsigned superincubator;
  unsigned rarexl;
};

static TrainerLevel levels[MAXLEVEL + 1] = {
  {}, // 0
  { 50,  0,  0, 0,  0,  0, 0, 1,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 1
  { 10,  0,  0, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 2
  { 10,  0,  0, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 3
  { 10,  0,  0, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 4
  { 15,  0,  0, 0,  0,  0, 0, 1,  5,  0, 10,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 5
  { 10,  0,  0, 0,  0,  0, 0, 0,  0,  0,  5,  0,  0,  0, 1, 0, 0, 0, 0, 0, }, // 6
  { 10,  0,  0, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 7
  { 10,  0,  0, 0,  0,  0, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 8
  { 10,  0,  0, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 9
  { 20,  0,  0, 0, 10,  0, 0, 0, 10,  0, 10,  0,  0,  0, 0, 0, 0, 1, 0, 0, }, // 10
  { 10,  0,  0, 5,  0,  0, 0, 0,  0,  0,  5,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 11
  { 10,  0,  0, 0,  5,  0, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 12
  { 10,  0,  0, 5,  0,  0, 1, 0,  0,  0,  5,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 13
  {  0, 10,  0, 0,  5,  0, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 14
  {  0, 15,  0, 0,  5,  0, 0, 0,  5,  0, 10,  0,  0,  0, 1, 0, 1, 1, 0, 0, }, // 15
  {  0, 10,  0, 0,  5,  0, 0, 0,  0,  0,  0, 10,  0,  0, 0, 0, 0, 0, 0, 0, }, // 16
  {  0, 10,  0, 0,  0,  5, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 17
  {  0, 10,  0, 5,  0,  0, 0, 0,  0,  0,  0, 10,  0,  0, 0, 0, 0, 0, 0, 0, }, // 18
  {  0, 10,  0, 0,  5,  0, 0, 0,  5,  0,  0, 10,  0,  0, 0, 0, 0, 0, 0, 0, }, // 19 FIXME fast tm
  {  0,  0, 20, 0,  0, 10, 0, 1, 10,  0,  0, 20,  0,  0, 0, 1, 0, 1, 0, 0, }, // 20
  {  0,  0, 10, 5,  0,  0, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 21
  {  0,  0, 10, 0,  5,  0, 0, 0,  0,  0,  0,  0, 10,  0, 0, 0, 0, 0, 0, 0, }, // 22
  {  0,  0, 10, 0,  0,  5, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 23
  {  0,  0, 10, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 15, 0, 10,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, }, // 25
  {  0,  0, 10, 0,  0,  5, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 10, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 10, 0,  5,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 10, 0,  0,  5, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 30, 0,  0, 20, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 2, 0, 0, }, // 30
  {  0,  0, 10, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 10, 0,  5,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 10, 0,  0,  5, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 10, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 15, 0, 10,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, }, // 35
  {  0,  0, 10, 0,  0,  5, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 10, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 10, 0,  5,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 10, 0,  0,  5, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 40, 0,  0, 30, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 2, 0, 0, }, // 40
  {  0,  0, 15,10,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 15, 0, 10,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, }, // 42
  {  0,  0, 15, 0,  0, 10, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 15,10,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 25, 0, 15,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, }, // 45
  {  0,  0, 15, 0,  0, 10, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 15,10,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 15, 0, 10,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, }, // 48
  {  0,  0, 15, 0,  0, 10, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, },
  {  0,  0, 50, 0,  0, 40, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 3, 0, 0, }, // 50
  {  0,  0, 20,15,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 51
  {  0,  0, 20, 0, 15,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, }, // 52
  {  0,  0, 20, 0,  0, 15, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 53
  {  0,  0, 20,15,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 54
  {  0,  0, 30, 0, 20,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 2, 0, 0, }, // 55
  {  0,  0, 20, 0,  0, 15, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 56
  {  0,  0, 20,15,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 57
  {  0,  0, 20, 0, 15,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, }, // 58
  {  0,  0, 20, 0,  0, 15, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 59
  {  0,  0, 60, 0,  0, 50, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 4, 0, 0, }, // 60
  {  0,  0, 20,15,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 61
  {  0,  0, 20, 0, 15,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, }, // 62
  {  0,  0, 20, 0,  0, 15, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 63
  {  0,  0, 20,15,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 64
  {  0,  0, 30, 0, 15,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 3, 0, 0, }, // 65
  {  0,  0, 20, 0,  0, 15, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 66
  {  0,  0, 20,15,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 67
  {  0,  0, 20, 0, 15,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, }, // 68
  {  0,  0, 20, 0,  0, 15, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, }, // 69
  {  0,  0, 70, 0,  0, 60, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 3, 0, }, // 70
  {  0,  0, 30,25,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 1, }, // 71
  {  0,  0, 30, 0, 25,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 1, }, // 72
  {  0,  0, 30, 0,  0, 25, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 1, }, // 73
  {  0,  0, 30,25,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 1, }, // 74
  {  0,  0, 50, 0, 35,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 2, 0, }, // 75
  {  0,  0, 30, 0,  0, 25, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 1, }, // 76
  {  0,  0, 30,25,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 1, 0, 1, }, // 77
  {  0,  0, 30, 0, 25,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 1, }, // 78
  {  0,  0, 30, 0,  0, 25, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 1, }, // 79
  {  0,  0, 80, 0,  0, 70, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 5, 0, }, // 80
};

/*
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
  std::cout << "\\begin{tabular}{r|g c g c g c g c g c g c g c g c g c g c g c g c}";
  std::cout << "&\
\\includegraphics[width=1em]{images/pokeball.png} &\
\\includegraphics[width=1em]{images/greatball.png} &\
\\includegraphics[width=1em]{images/ultraball.png} &\
\\includegraphics[width=1em]{images/nanab.png} &\
\\includegraphics[width=1em]{images/razz.png} &\
\\includegraphics[width=1em]{images/pinap.png} &\
\\includegraphics[width=1em]{images/goldenrazz.png} &\
\\includegraphics[width=1em]{images/incense.png} &\
\\includegraphics[width=1em]{images/revive.png} &\
\\includegraphics[width=1em]{images/maxrevive.png} &\
\\includegraphics[width=1em]{images/Potion.png} &\
\\includegraphics[width=1em]{images/superpotion.png} &\
\\includegraphics[width=1em]{images/Hyper_Potion.png} &\
\\includegraphics[width=1em]{images/Max_Potion.png} &\
\\includegraphics[width=1em]{images/lure.png} &\
\\includegraphics[width=1em]{images/luckyegg.png} &\
\\includegraphics[width=1em]{images/starpiece.png} &\
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
    std::cout << l.pinap << "&";
    std::cout << l.goldenrazz << "&";
    std::cout << l.incense << "&";
    std::cout << l.revive << "&";
    std::cout << l.maxrevive << "&";
    std::cout << l.potion << "&";
    std::cout << l.superpotion << "&";
    std::cout << l.hyperpotion << "&";
    std::cout << l.maxpotion << "&";
    std::cout << l.lure << "&";
    std::cout << l.luckyegg << "&";
    std::cout << l.starpiece << "&";
    std::cout << l.incubator << "&";
    std::cout << l.superincubator << "&";
    std::cout << l.rarexl << "&";
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
