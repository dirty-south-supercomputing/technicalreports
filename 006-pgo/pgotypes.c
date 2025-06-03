#include <math.h>
#include <stdio.h>

typedef enum {
  TYPE_BUG,
  TYPE_DARK,
  TYPE_DRAGON,
  TYPE_ELECTRIC,
  TYPE_FAIRY,
  TYPE_FIGHTING,
  TYPE_FIRE,
  TYPE_FLYING,
  TYPE_GHOST,
  TYPE_GRASS,
  TYPE_GROUND,
  TYPE_ICE,
  TYPE_NORMAL,
  TYPE_POISON,
  TYPE_PSYCHIC,
  TYPE_ROCK,
  TYPE_STEEL,
  TYPE_WATER,
  TYPECOUNT
} pgo_types_e;

static const char* tnames[TYPECOUNT] = {
  "bug",
  "dark",
  "dragon",
  "electric",
  "fairy",
  "fighting",
  "fire",
  "flying",
  "ghost",
  "grass",
  "ground",
  "ice",
  "normal",
  "poison",
  "psychic",
  "rock",
  "steel",
  "water"
};

/*
float nextcpm(float cpm, float step){
  return sqrt(pow(cpm, 2) + step);
}

// halflevel: positive integer (usually less than 100) equal to L * 2 - 1
float cpm(int halflevel){
  float step;
  if(halflevel >= 79){ // Levels 40 and above are computed directly
    return 0.0025 * (halflevel - 79) + 0.7903;
  }else if(halflevel >= 59){ // Levels 30..39.5
    step = 0.00445946079;
  }else if(halflevel >= 39){ // Levels 20..29.5
    step = 0.008924905903;
    // FIXME errors start here
  }else if(halflevel >= 19){ // Levels 10..19.5
    step = 0.008919025675;
  }else if(halflevel > 1){ //mLevels 1.5..9.5
    step = 0.009426125469;
  }else{
    return 0.094; // Level 1 (or invalid low value)
  }
  return nextcpm(cpm(halflevel - 1), step);
}
*/

float cpm(int halflevel){
  static const float cpms[] = {
    0.094, 0.1351374318, 0.16639787, 0.192650919, 0.21573247, 0.2365726613,
    0.25572005, 0.2735303812, 0.29024988, 0.3060573775, 0.3210876,
    0.3354450362, 0.34921268, 0.3624577511, 0.3752356, 0.387592416, 0.39956728,
    0.4111935514, 0.4225, 0.4329264091, 0.44310755, 0.4530599591, 0.4627984,
    0.472336093, 0.48168495, 0.4908558003, 0.49985844, 0.508701765, 0.51739395,
    0.5259425113, 0.5343543, 0.5426357375, 0.5507927, 0.5588305862, 0.5667545,
    0.5745691333, 0.5822789, 0.5898879072, 0.5974, 0.6048236651, 0.6121573,
    0.6194041216, 0.6265671, 0.6336491432, 0.64065295, 0.6475809666,
    0.65443563, 0.6612192524, 0.667934, 0.6745818959, 0.6811649, 0.6876849038,
    0.69414365, 0.70054287, 0.7068842, 0.7131691091, 0.7193991, 0.7255756136,
    0.7317, 0.7347410093, 0.7377695, 0.7407855938, 0.74378943, 0.7467812109,
    0.74976104, 0.7527290867, 0.7556855, 0.7586303683, 0.76156384,
    0.7644860647, 0.76739717, 0.7702972656, 0.7731865, 0.7760649616,
    0.77893275, 0.7817900548, 0.784637, 0.7874736075, 0.7903, 0.792803968,
    0.79530001, 0.797800015, 0.8003, 0.802799995, 0.8053, 0.8078, 0.81029999,
    0.812799985, 0.81529999, 0.81779999, 0.82029999, 0.82279999, 0.82529999,
    0.82779999, 0.83029999, 0.83279999, 0.83529999, 0.83779999, 0.84029999,
    0.84279999, 0.84529999, };
  if(halflevel <= 0 || (unsigned)halflevel > sizeof(cpms) / sizeof(*cpms)){
    fprintf(stderr, "INVALID HALFLEVEL %d\n", halflevel);
    return NAN;
  }
  return cpms[halflevel - 1];
}
