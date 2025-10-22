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
  unsigned prempass;
  // storage upgrades
  unsigned bagup;
  unsigned pokup;
  unsigned postup;
  unsigned giftup;
};

static TrainerLevel levels[MAXLEVEL + 1] = {
  {}, // 0
                                       // potion
  { 50,  0,  0, 0,  0,  0, 0, 2,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 1
  { 10,  0,  0, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 2
  { 10,  0,  0, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 3
  { 10,  0,  0, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 4
  { 15,  0,  0, 0,  0,  0, 0, 1,  5,  0, 10,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 5
  { 10,  0,  0, 0,  0,  0, 0, 0,  0,  0,  5,  0,  0,  0, 1, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 6
  { 10,  0,  0, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 7
  { 10,  0,  0, 0,  0,  0, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 8
  { 10,  0,  0, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 9
  { 20,  0,  0, 0, 10,  0, 0, 0, 10,  0, 10,  0,  0,  0, 0, 1, 0, 1, 0, 0, 0, 25,  0,  0,  0,  }, // 10
  { 10,  0,  0, 5,  0,  0, 0, 0,  0,  0,  5,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 11
  { 10,  0,  0, 0,  5,  0, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 12
  { 10,  0,  0, 5,  0,  0, 1, 0,  0,  0,  5,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 13
  {  0, 10,  0, 0,  5,  0, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 14
  {  0, 15,  0, 0,  5,  0, 0, 0,  5,  0, 10,  0,  0,  0, 1, 0, 1, 1, 0, 0, 0,  0,  0,  0,  0,  }, // 15
  {  0, 10,  0, 0,  5,  0, 0, 0,  0,  0,  0, 10,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 16
  {  0, 10,  0, 0,  0,  5, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 17
  {  0, 10,  0, 5,  0,  0, 0, 0,  0,  0,  0, 10,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 18
  {  0, 10,  0, 0,  5,  0, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 19 FIXME fast tm
  {  0,  0, 20, 0,  0, 10, 0, 1, 10,  0,  0, 20,  0,  0, 0, 1, 0, 1, 0, 0, 0,  0, 25,  0,  0,  }, // 20
  {  0,  0, 10, 5,  0,  0, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 21
  {  0,  0, 10, 0,  5,  0, 0, 0,  0,  0,  0,  0, 10,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 22
  {  0,  0, 10, 0,  0,  5, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 23
  {  0,  0, 10, 5,  0,  0, 0, 0,  0,  0,  0,  0, 10,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 24
  {  0,  0, 15, 0, 10,  0, 0, 0, 10,  0,  0,  0,  0, 20, 1, 0, 1, 1, 0, 0, 0,  0,  0,  0,  0,  }, // 25
  {  0,  0, 10, 0,  0,  5, 0, 0,  0,  0,  0,  0,  0, 10, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 26
  {  0,  0, 10, 5,  0,  0, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 27
  {  0,  0, 10, 0,  5,  0, 0, 0,  0,  0,  0,  0,  0, 10, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 28
  {  0,  0, 10, 0,  0,  5, 0, 0,  5,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 29
  {  0,  0, 30, 0,  0, 20, 0, 2,  0, 15,  0,  0,  0, 30, 0, 2, 0, 2, 0, 0, 0, 50,  0,  0,  0,  }, // 30
  {  0,  0, 10, 5,  0,  0, 0, 0,  0,  0,  0,  0,  0, 10, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 31
  {  0,  0, 10, 0,  5,  0, 0, 0,  0,  5,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 32
  {  0,  0, 10, 0,  0,  5, 0, 0,  0,  0,  0,  0,  0, 10, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 33
  {  0,  0, 10, 5,  0,  0, 0, 0,  0,  5,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 34
  {  0,  0, 15, 0, 10,  0, 0, 0,  0, 10,  0,  0,  0, 20, 1, 0, 1, 1, 0, 0, 0,  0,  0,  0,  0,  }, // 35
  {  0,  0, 10, 0,  0,  5, 0, 0,  0,  0,  0,  0,  0, 10, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 36
  {  0,  0, 10, 5,  0,  0, 0, 0,  0,  5,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 37
  {  0,  0, 10, 0,  5,  0, 0, 0,  0,  0,  0,  0,  0, 10, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 38
  {  0,  0, 10, 0,  0,  5, 0, 0,  0,  5,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 39
  {  0,  0, 40, 0,  0, 30, 0, 2,  0, 20,  0,  0,  0, 40, 0, 2, 0, 2, 0, 0, 0,  0, 50,  0,  0,  }, // 40
  {  0,  0, 15,10,  0,  0, 0, 0,  0,  0,  0,  0,  0, 20, 0, 1, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 41
  {  0,  0, 15, 0, 10,  0, 0, 0,  0, 10,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, 0,  0,  0,  0,  0,  }, // 42
  {  0,  0, 15, 0,  0, 10, 0, 0,  0,  0,  0,  0,  0, 20, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  5,  }, // 43
  {  0,  0, 15,10,  0,  0, 0, 1,  0, 10,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 44
  {  0,  0, 25, 0, 15,  0, 0, 0,  0, 15,  0,  0,  0, 30, 1, 0, 1, 1, 0, 0, 0, 25,  0,  0,  0,  }, // 45
  {  0,  0, 15, 0,  0, 10, 0, 0,  0,  0,  0,  0,  0, 20, 0, 0, 1, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 46
  {  0,  0, 15,10,  0,  0, 0, 0,  0, 10,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0, 50,  0,  }, // 47
  {  0,  0, 15, 0, 10,  0, 0, 0,  0,  0,  0,  0,  0, 20, 0, 0, 0, 1, 0, 0, 0,  0,  0,  0,  0,  }, // 48
  {  0,  0, 15, 0,  0, 10, 0, 0,  0, 10,  0,  0,  0,  0, 1, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 49
  {  0,  0, 50, 0,  0, 40, 0, 3,  0, 25,  0,  0,  0, 50, 0, 3, 0, 3, 0, 0, 0, 50,  0,  0,  0,  }, // 50
  {  0,  0, 20,15,  0,  0, 0, 0,  0,  0,  0,  0,  0, 20, 0, 1, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 51
  {  0,  0, 20, 0, 15,  0, 0, 0,  0, 10,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, 0,  0,  0,  0,  0,  }, // 52
  {  0,  0, 20, 0,  0, 15, 0, 0,  0,  0,  0,  0,  0, 20, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  5,  }, // 53
  {  0,  0, 20,15,  0,  0, 0, 1,  0, 10,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 54
  {  0,  0, 30, 0, 20,  0, 0, 0,  0, 15,  0,  0,  0, 30, 2, 0, 2, 2, 0, 0, 0,  0, 25,  0,  0,  }, // 55
  {  0,  0, 20, 0,  0, 15, 0, 0,  0,  0,  0,  0,  0, 20, 0, 0, 1, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 56
  {  0,  0, 20,15,  0,  0, 0, 0,  0, 10,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0, 50,  0,  }, // 57
  {  0,  0, 20, 0, 15,  0, 0, 0,  0,  0,  0,  0,  0, 20, 0, 0, 0, 1, 0, 0, 0,  0,  0,  0,  0,  }, // 58
  {  0,  0, 20, 0,  0, 15, 0, 0,  0, 10,  0,  0,  0,  0, 1, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 59
  {  0,  0, 60, 0,  0, 50, 0, 4,  0, 25,  0,  0,  0, 50, 0, 4, 0, 4, 0, 0, 0,  0, 50,  0,  0,  }, // 60
  {  0,  0, 20,15,  0,  0, 0, 0,  0,  0,  0,  0,  0, 20, 0, 1, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 61
  {  0,  0, 20, 0, 15,  0, 0, 0,  0, 10,  0,  0,  0,  0, 0, 0, 0, 1, 0, 0, 0,  0,  0,  0,  0,  }, // 62
  {  0,  0, 20, 0,  0, 15, 0, 0,  0,  0,  0,  0,  0, 20, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  5,  }, // 63
  {  0,  0, 20,15,  0,  0, 0, 1,  0, 10,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 64
  {  0,  0, 30, 0, 15,  0, 0, 0,  0, 15,  0,  0,  0, 30, 3, 0, 3, 3, 0, 0, 0, 25,  0,  0,  0,  }, // 65
  {  0,  0, 20, 0,  0, 15, 0, 0,  0,  0,  0,  0,  0, 20, 0, 0, 1, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 66
  {  0,  0, 20,15,  0,  0, 0, 0,  0, 10,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0, 50,  0,  }, // 67
  {  0,  0, 20, 0, 15,  0, 0, 0,  0,  0,  0,  0,  0, 20, 0, 0, 0, 1, 0, 0, 0,  0,  0,  0,  0,  }, // 68
  {  0,  0, 20, 0,  0, 15, 0, 0,  0, 10,  0,  0,  0,  0, 1, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  }, // 69
  {  0,  0, 70, 0,  0,  0, 0, 6,  0, 25,  0,  0,  0, 50, 0, 6, 0, 0, 3, 0, 0, 75,  0,  0,  0,  }, // 70
  {  0,  0, 30,25,  0,  0, 0, 0,  0,  0,  0,  0,  0, 20, 0, 0, 0, 0, 0, 1, 1,  0,  0,  0,  0,  }, // 71
  {  0,  0, 30, 0, 25,  0, 0, 0,  0, 10,  0,  0,  0,  0, 0, 1, 0, 0, 0, 1, 1,  0,  0,  0,  0,  }, // 72
  {  0,  0, 30, 0,  0, 25, 0, 0,  0,  0,  0,  0,  0, 20, 0, 0, 0, 0, 0, 1, 1,  0,  0,  0,  5,  }, // 73
  {  0,  0, 30,25,  0,  0, 0, 0,  0, 10,  0,  0,  0,  0, 0, 0, 0, 1, 0, 1, 1,  0,  0, 50,  0,  }, // 74
  {  0,  0, 50, 0, 35,  0, 0, 0,  0, 15,  0,  0,  0, 30, 5, 0, 5, 0, 2, 0, 0,  0, 25,  0,  0,  }, // 75
  {  0,  0, 30, 0,  0, 25, 0, 0,  0,  0,  0,  0,  0, 20, 0, 1, 0, 0, 0, 1, 1,  0,  0,  0,  0,  }, // 76
  {  0,  0, 30,25,  0,  0, 0, 0,  0, 10,  0,  0,  0,  0, 0, 0, 0, 1, 0, 1, 1,  0,  0, 50,  0,  }, // 77
  {  0,  0, 30, 0, 25,  0, 0, 0,  0,  0,  0,  0,  0, 20, 0, 0, 0, 0, 0, 1, 1,  0, 75,  0,  0,  }, // 78
  {  0,  0, 30, 0,  0, 25, 0, 0,  0, 10,  0,  0,  0,  0, 0, 0, 0, 0, 0, 1, 1,  0,  0,  0,  0,  }, // 79
  {  0,  0, 80, 0,  0, 70, 0, 7,  0, 25,  0,  0,  0, 50, 0, 7, 0, 0, 5, 0, 0,100,100,  0,  0,  }, // 80
};

static void emitk(uint64_t val){
  if(val < 1000000){
    std::cout << (val / 1000.0) << "k";
  }else{
    std::cout << (val / 1000000.0) << "M";
  }
}

static void emitv(unsigned val){
  if(val){
    std::cout << val;
  }
  std::cout << "&";
}

static void level_item_table(int llevel, int hlevel){
  std::cout << "\\begin{table}";
  std::cout << "\\centering";
  std::cout << "\\setlength{\\tabcolsep}{2pt}";
  std::cout << "\\footnotesize";
  std::cout << "\\begin{tabular}{r|g c g c g c g c g c g c g c g c g c g c g c}";
  std::cout << "&\
\\includegraphics[width=1em]{images/pokeball.png}&\
\\includegraphics[width=1em]{images/greatball.png}&\
\\includegraphics[width=1em]{images/ultraball.png}&\
\\includegraphics[width=1em]{images/nanab.png}&\
\\includegraphics[width=1em]{images/razz.png}&\
\\includegraphics[width=1em]{images/pinap.png}&\
\\includegraphics[width=1em]{images/incense.png}&\
\\includegraphics[width=1em]{images/revive.png}&\
\\includegraphics[width=1em]{images/maxrevive.png}&\
\\includegraphics[width=1em]{images/Potion.png}&\
\\includegraphics[width=1em]{images/superpotion.png}&\
\\includegraphics[width=1em]{images/Hyper_Potion.png}&\
\\includegraphics[width=1em]{images/Max_Potion.png}&\
\\includegraphics[width=1em]{images/lure.png}&\
\\includegraphics[width=1em]{images/luckyegg.png}&\
\\includegraphics[width=1em]{images/starpiece.png}&\
\\includegraphics[width=1em]{images/incubatorlimited.png}&\
\\includegraphics[width=1em]{images/incubatorsuper.png}&\
\\includegraphics[width=1em]{images/rarecandyxl.png}&\
\\includegraphics[width=1em]{images/premiumbattlepass.png}";
  std::cout << "\\\\";
  for(int x = llevel ; x <= hlevel ; ++x){
    const TrainerLevel &l = levels[x];
    std::cout << x << "&";
    emitv(l.pokeball);
    emitv(l.greatball);
    emitv(l.ultraball);
    emitv(l.nanab);
    emitv(l.razz);
    emitv(l.pinap);
    emitv(l.incense);
    emitv(l.revive);
    emitv(l.maxrevive);
    emitv(l.potion);
    emitv(l.superpotion);
    emitv(l.hyperpotion);
    emitv(l.maxpotion);
    emitv(l.lure);
    emitv(l.luckyegg);
    emitv(l.starpiece);
    emitv(l.incubator);
    emitv(l.superincubator);
    emitv(l.rarexl);
    if(l.prempass){
      std::cout << l.prempass;
    }
    std::cout << "\\\\" << std::endl;
  }
  std::cout << "\\end{tabular}";
  std::cout << "\\caption{Items awarded for reaching Trainer Levels ";
  std::cout << llevel << "--" << hlevel;
  std::cout << "\\label{table:level" << hlevel << "items}}";
  std::cout << "\\end{table}";
}

static bool
hasupgrade(const TrainerLevel &l){
  if(l.bagup || l.pokup || l.postup || l.giftup){
    return true;
  }
  return false;
}

static void
level_upgrade_table(void){
  std::cout << "\\begin{table}";
  std::cout << "\\centering";
  std::cout << "\\setlength{\\tabcolsep}{2pt}";
  std::cout << "\\footnotesize";
  std::cout << "\\begin{tabular}{r|g c g c}";
  std::cout << "&\
\\includegraphics[width=1em]{images/bagupgrade.png}&\
\\includegraphics[width=1em]{images/pokemonupgrade.png}&\
\\includegraphics[width=1em]{images/postcardupgrade.png}&\
\\includegraphics[width=1em]{images/gift.png}";
  std::cout << "\\\\" << std::endl;
  for(int x = 2 ; x <= MAXLEVEL ; ++x){
    const TrainerLevel &l = levels[x];
    if(!hasupgrade(l)){
      continue;
    }
    std::cout << x << "&";
    emitv(l.bagup);
    emitv(l.pokup);
    emitv(l.postup);
    if(l.giftup){
      std::cout << l.giftup;
    }
    std::cout << "\\\\" << std::endl;
  }
  std::cout << "\\end{tabular}";
  std::cout << "\\caption{Upgrades awarded for reading Trainer Levels\\label{table:levelupgrades}}";
  std::cout << "\\end{table}";
}

static void usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " [ upgrades | llevel hlevel ]" << std::endl;
  exit(EXIT_FAILURE);
}

// write the trainer XP table
int main(int argc, const char **argv){
  if(argc > 1){
    if(argc > 3){
      usage(argv[0]);
    }
    if(argc == 2){
      if(strcmp(argv[1], "upgrades")){
        usage(argv[0]);
      }
      level_upgrade_table();
      return EXIT_SUCCESS;
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
