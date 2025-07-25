#include <map>
#include <cmath>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <iostream>

#define TYPESTART TYPE_BUG

constexpr unsigned MAXIVELEM = 15;
constexpr unsigned MAX_HALFLEVEL = 99;

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
  TYPECOUNT = 18
} pgo_types_e;

// there are 171 distinct species types (18 + C(18, 2))
#define TYPINGCOUNT 171
// but there are 324 if one considers ordering, which one generally oughtn't
#define TYPECOUNTSQUARED 324

typedef enum {
  WEATHER_CLEAR,
  WEATHER_RAIN,
  WEATHER_PARTLY_CLOUDY,
  WEATHER_CLOUDY,
  WEATHER_WINDY,
  WEATHER_SNOW,
  WEATHER_FOG,
  WEATHER_EXTREME,
  WEATHERCOUNT
} pgo_weather_t;

static const char* WNames[WEATHERCOUNT] = {
  "Clear",
  "Rainy",
  "Partly Cloudy",
  "Cloudy",
  "Windy",
  "Snow",
  "Fog",
  "Extreme"
};

// secondary names
static const char* WSNames[WEATHERCOUNT] = {
  "Clear Night",
  NULL,
  "Partly Cloudy Night",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

static pgo_weather_t wboosts[TYPECOUNT] = {
  WEATHER_RAIN,   // bug
  WEATHER_FOG,    // dark
  WEATHER_WINDY,  // dragon
  WEATHER_RAIN,   // electric
  WEATHER_CLOUDY, // fairy
  WEATHER_CLOUDY, // fighting
  WEATHER_CLEAR,  // fire
  WEATHER_WINDY,  // flying
  WEATHER_FOG,    // ghost
  WEATHER_CLEAR,  // grass
  WEATHER_CLEAR,  // ground
  WEATHER_SNOW,   // ice
  WEATHER_PARTLY_CLOUDY,  // normal
  WEATHER_CLOUDY, // poison
  WEATHER_WINDY,  // psychic
  WEATHER_PARTLY_CLOUDY,  // rock
  WEATHER_SNOW,   // steel
  WEATHER_RAIN,   // water
};

const uint32_t TColors[TYPECOUNT] = {
  0xA6B91A, 0x575063, 0x6F35FC, 0xF7D02C, 0xD685AD, 0xC22E28, 0xEE8130, 0x8D87DB, 0x5066A3,
  0x7AC74C, 0xD97A4B, 0x96D9D6, 0x9CA5AB, 0xA33EA1, 0xF95587, 0xB6A136, 0x5E91A4, 0x6390F0,
};

// each row is an attacking Type
static const int trelations[TYPECOUNT][TYPECOUNT] = {
  // bug     dragon  fairy   fire    ghost   ground  normal  psychic steel
  //   dark      elec    fight   fly     grass   ice     poison  rock    water
  {  0,  1,  0,  0, -1, -1, -1, -1, -1,  1,  0,  0,  0, -1,  1,  0, -1,  0 }, // bug
  {  0, -1,  0,  0, -1, -1,  0,  0,  1,  0,  0,  0,  0,  0,  1,  0,  0,  0 }, // dark
  {  0,  0,  1,  0, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,  0 }, // dragon
  {  0,  0, -1, -1,  0,  0,  0,  1,  0, -1, -2,  0,  0,  0,  0,  0,  0,  1 }, // electric
  {  0,  1,  1,  0,  0,  1, -1,  0,  0,  0,  0,  0,  0, -1,  0,  0, -1,  0 }, // fairy
  { -1,  1,  0,  0, -1,  0,  0, -1, -2,  0,  0,  1,  1, -1, -1,  1,  1,  0 }, // fighting
  {  1,  0, -1,  0,  0,  0, -1,  0,  0,  1,  0,  1,  0,  0,  0, -1,  1, -1 }, // fire
  {  1,  0,  0, -1,  0,  1,  0,  0,  0,  1,  0,  0,  0,  0,  0, -1, -1,  0 }, // flying
  {  0, -1,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0, -2,  0,  1,  0,  0,  0 }, // ghost
  { -1,  0, -1,  0,  0,  0, -1, -1,  0, -1,  1,  0,  0, -1,  0,  1, -1,  1 }, // grass
  { -1,  0,  0,  1,  0,  0,  1, -2,  0, -1,  0,  0,  0,  1,  0,  1,  1,  0 }, // ground
  {  0,  0,  1,  0,  0,  0, -1,  1,  0,  1,  1, -1,  0,  0,  0,  0, -1, -1 }, // ice
  {  0,  0,  0,  0,  0,  0,  0,  0, -2,  0,  0,  0,  0,  0,  0, -1, -1,  0 }, // normal
  {  0,  0,  0,  0,  1,  0,  0,  0, -1,  1, -1,  0,  0, -1,  0, -1, -2,  0 }, // poison
  {  0, -2,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  1, -1,  0, -1,  0 }, // psychic
  {  1,  0,  0,  0,  0, -1,  1,  1,  0,  0, -1,  1,  0,  0,  0,  0, -1,  0 }, // rock
  {  0,  0,  0, -1,  1,  0, -1,  0,  0,  0,  0,  1,  0,  0,  0,  1, -1, -1 }, // steel
  {  0,  0, -1,  0,  0,  0,  1,  0,  0, -1,  1,  0,  0,  0,  0,  1,  0, -1 }  // water
};

// look up the type relation of atype upon ttype
static inline int
type_relation(pgo_types_e atype, pgo_types_e ttype){
  return trelations[atype][ttype];
}

static inline int
typing_relation(pgo_types_e atype, pgo_types_e ttype1, pgo_types_e ttype2){
  int r1 = type_relation(atype, ttype1);
  if(ttype2 == ttype1 || ttype2 == TYPECOUNT){
    return r1;
  }
  int r2 = type_relation(atype, ttype2);
  return r1 + r2;
}

const char* tnames[TYPECOUNT] = {
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

const char* TNames[TYPECOUNT] = {
  "Bug",
  "Dark",
  "Dragon",
  "Electric",
  "Fairy",
  "Fighting",
  "Fire",
  "Flying",
  "Ghost",
  "Grass",
  "Ground",
  "Ice",
  "Normal",
  "Poison",
  "Psychic",
  "Rock",
  "Steel",
  "Water"
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
    0.79530001, 0.797800015, 0.8003, 0.802799995,
    0.8053, 0.8078, 0.8103, 0.8128,
    0.8153, 0.8178, 0.8203, 0.8228,
    0.8253, 0.8278, 0.8303, 0.8328,
    0.8353, 0.8378, 0.8403, 0.8428,
    0.8453,
    // levels "50.5/51" are 49.5/50 for *active buddy* with the Best Buddy bonus
    0.8478, 0.8503
  };
  if(halflevel <= 0 || (unsigned)halflevel > sizeof(cpms) / sizeof(*cpms)){
    fprintf(stderr, "INVALID HALFLEVEL %d\n", halflevel);
    return NAN;
  }
  return cpms[halflevel - 1];
}

// currently only have trainer battle stats; need to add raid
typedef struct attack {
  const char *name;
  pgo_types_e type;
  // 3x3 context
  unsigned powertrain;   // power in 3x3 battle context
  int energytrain;       // energy generated/consumed in trainer battle context
  unsigned turns;
  // chance (out of 1000) of having any of four buffing/debuffing effects
  unsigned chance_user_attack;
  unsigned chance_user_defense;
  unsigned chance_opp_attack;
  unsigned chance_opp_defense;
  // four possible effects (out of [-2, 2])
  int user_attack;
  int user_defense;
  int opp_attack;
  int opp_defense;
  // nx1 context
  int powerraid;         // power in nx1 battle context
  int energyraid;
  int animdur;           // nx1 animation duration in half-seconds
  bool adveffect;        // does it have an Adventure Effect?
} attack;

// either a fast attack with all charged attacks it can be paired with (on some
// form or another), or a charged attack with all fast attacks yadda yadda.
class attackset {
 public:
  attackset(const attack *a) :
   A(a) {}

  // add if not already present
  void add(const attack *paired) {
    As.try_emplace(paired->name, paired);
  }

  const attack *A;
  std::map<std::string, const attack *> As;
};

static inline bool
fast_attack_p(const attack *a){
  return a->energytrain >= 0; // need 0 to pick up Transform
}

static inline bool
charged_attack_p(const attack *a){
  return a->energytrain < 0;
}

using pairmap = std::map<std::string, attackset>;

static const attack ATK_Acid = { "Acid", TYPE_POISON, 6, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 10, 2, false, };
static const attack ATK_Air_Slash = { "Air Slash", TYPE_FLYING, 9, 9, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 8, 2, false, };
static const attack ATK_Astonish = { "Astonish", TYPE_GHOST, 12, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	7, 13, 2, false, };
static const attack ATK_Bite = { "Bite", TYPE_DARK, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 4, 1, false, };
static const attack ATK_Bubble = { "Bubble", TYPE_WATER, 8, 11, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 12, 2, false, };
static const attack ATK_Bug_Bite = { "Bug Bite", TYPE_BUG, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 6, 1, false, };
static const attack ATK_Bullet_Punch = { "Bullet Punch", TYPE_STEEL, 6, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 11, 2, false, };
static const attack ATK_Bullet_Seed = { "Bullet Seed", TYPE_GRASS, 5, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	7, 13, 2, false, };
static const attack ATK_Charge_Beam = { "Charge Beam", TYPE_ELECTRIC, 5, 11, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	7, 14, 2, false, };
static const attack ATK_Charm = { "Charm", TYPE_FAIRY, 15, 6, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	20, 11, 3, false, };
static const attack ATK_Confusion = { "Confusion", TYPE_PSYCHIC, 16, 12, 4, 0, 0, 0, 0, 0, 0, 0, 0,
	19, 14, 3, false, };
static const attack ATK_Counter = { "Counter", TYPE_FIGHTING, 8, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 9, 2, false, };
static const attack ATK_Cut = { "Cut", TYPE_NORMAL, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 1, false, };
static const attack ATK_Double_Kick = { "Double Kick", TYPE_FIGHTING, 8, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 13, 2, false, };
static const attack ATK_Dragon_Breath = { "Dragon Breath", TYPE_DRAGON, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 4, 1, false, };
static const attack ATK_Dragon_Tail = { "Dragon Tail", TYPE_DRAGON, 13, 9, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	14, 8, 2, false, };
static const attack ATK_Ember = { "Ember", TYPE_FIRE, 7, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 10, 2, false, };
static const attack ATK_Extrasensory = { "Extrasensory", TYPE_PSYCHIC, 8, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 11, 2, false, };
static const attack ATK_Feint_Attack = { "Feint Attack", TYPE_DARK, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 10, 2, false, };
static const attack ATK_Fire_Fang = { "Fire Fang", TYPE_FIRE, 8, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 9, 2, false, };
static const attack ATK_Fairy_Wind = { "Fairy Wind", TYPE_FAIRY, 4, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	9, 13, 2, false, };
static const attack ATK_Fire_Spin = { "Fire Spin", TYPE_FIRE, 11, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 9, 2, false, };
static const attack ATK_Force_Palm = { "Force Palm", TYPE_FIGHTING, 13, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 16, 2, false, };
static const attack ATK_Frost_Breath = { "Frost Breath", TYPE_ICE, 7, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 9, 2, false, };
static const attack ATK_Fury_Cutter = { "Fury Cutter", TYPE_BUG, 2, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 8, 1, false, };
static const attack ATK_Geomancy = { "Geomancy", TYPE_FAIRY, 4, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	20, 14, 3, false, };
static const attack ATK_Gust = { "Gust", TYPE_FLYING, 16, 12, 4, 0, 0, 0, 0, 0, 0, 0, 0,
	25, 20, 4, false, };
static const attack ATK_Hex = { "Hex", TYPE_GHOST, 7, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	8, 13, 2, false, };
static const attack ATK_Hidden_Power = { "Hidden Power", TYPECOUNT, 9, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	15, 15, 3, false, };
static const attack ATK_Ice_Fang = { "Ice Fang", TYPE_ICE, 8, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 20, 3, false, };
static const attack ATK_Ice_Shard = { "Ice Shard", TYPE_ICE, 9, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 10, 2, false, };
static const attack ATK_Incinerate = { "Incinerate", TYPE_FIRE, 20, 20, 5, 0, 0, 0, 0, 0, 0, 0, 0,
	32, 22, 5, false, };
static const attack ATK_Infestation = { "Infestation", TYPE_BUG, 6, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	9, 13, 2, false, };
static const attack ATK_Iron_Tail = { "Iron Tail", TYPE_STEEL, 10, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	14, 6, 2, false, };
static const attack ATK_Karate_Chop = { "Karate Chop", TYPE_FIGHTING, 5, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 13, 2, false, };
static const attack ATK_Leafage = { "Leafage", TYPE_GRASS, 6, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 4, 1, false, };
static const attack ATK_Lick = { "Lick", TYPE_GHOST, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 6, 1, false, };
static const attack ATK_Lock_On = { "Lock On", TYPE_NORMAL, 1, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	2, 10, 1, false, };
static const attack ATK_Low_Kick = { "Low Kick", TYPE_FIGHTING, 4, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 1, false, };
static const attack ATK_Magical_Leaf = { "Magical Leaf", TYPE_GRASS, 10, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	17, 17, 3, false, };
static const attack ATK_Metal_Claw = { "Metal Claw", TYPE_STEEL, 5, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 5, 1, false, };
static const attack ATK_Metal_Sound = { "Metal Sound", TYPE_STEEL, 3, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Mud_Shot = { "Mud Shot", TYPE_GROUND, 4, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Mud_Slap = { "Mud Slap", TYPE_GROUND, 12, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	19, 13, 3, false, };
static const attack ATK_Peck = { "Peck", TYPE_FLYING, 6, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 10, 2, false, };
static const attack ATK_Poison_Jab = { "Poison Jab", TYPE_POISON, 7, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 9, 2, false, };
static const attack ATK_Poison_Sting = { "Poison Sting", TYPE_POISON, 4, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Pound = { "Pound", TYPE_NORMAL, 4, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 5, 1, false, };
static const attack ATK_Powder_Snow = { "Powder Snow", TYPE_ICE, 5, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 15, 2, false, };
static const attack ATK_Present = { "Present", TYPE_NORMAL, 3, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 23, 3, false, };
static const attack ATK_Psycho_Cut = { "Psycho Cut", TYPE_PSYCHIC, 3, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 7, 1, false, };
static const attack ATK_Psywave = { "Psywave", TYPE_PSYCHIC, 3, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 7, 1, false, };
static const attack ATK_Quick_Attack = { "Quick Attack", TYPE_NORMAL, 5, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 13, 2, false, };
static const attack ATK_Razor_Leaf = { "Razor Leaf", TYPE_GRASS, 9, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 7, 2, false, };
static const attack ATK_Rock_Smash = { "Rock Smash", TYPE_FIGHTING, 9, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	17, 12, 3, false, };
static const attack ATK_Rock_Throw = { "Rock Throw", TYPE_ROCK, 8, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 8, 2, false, };
static const attack ATK_Rollout = { "Rollout", TYPE_ROCK, 7, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	15, 19, 3, false, };
static const attack ATK_Sand_Attack = { "Sand Attack", TYPE_GROUND, 2, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 7, 1, false, };
static const attack ATK_Scratch = { "Scratch", TYPE_NORMAL, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 4, 1, false, };
static const attack ATK_Shadow_Claw = { "Shadow Claw", TYPE_GHOST, 6, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 4, 1, false, };
static const attack ATK_Smack_Down = { "Smack Down", TYPE_ROCK, 11, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 7, 2, false, };
static const attack ATK_Snarl = { "Snarl", TYPE_DARK, 5, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 13, 2, false, };
static const attack ATK_Spark = { "Spark", TYPE_ELECTRIC, 5, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Splash = { "Splash", TYPE_WATER, 0, 12, 4, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 17, 3, false, };
static const attack ATK_Steel_Wing = { "Steel Wing", TYPE_STEEL, 7, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	14, 8, 2, false, };
static const attack ATK_Struggle_Bug = { "Struggle Bug", TYPE_BUG, 9, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	15, 15, 3, false, };
static const attack ATK_Sucker_Punch = { "Sucker Punch", TYPE_DARK, 8, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 6, 1, false, };
static const attack ATK_Tackle = { "Tackle", TYPE_NORMAL, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 1, false, };
static const attack ATK_Take_Down = { "Take Down", TYPE_NORMAL, 5, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	7, 8, 2, false, };
static const attack ATK_Thunder_Fang = { "Thunder Fang", TYPE_ELECTRIC, 8, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 13, 2, false, };
static const attack ATK_Thunder_Shock = { "Thunder Shock", TYPE_ELECTRIC, 4, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 7, 1, false, };
static const attack ATK_Transform = { "Transform", TYPE_NORMAL, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 4, false, };
static const attack ATK_Vine_Whip = { "Vine Whip", TYPE_GRASS, 5, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 5, 1, false, };
static const attack ATK_Volt_Switch = { "Volt Switch", TYPE_ELECTRIC, 12, 16, 4, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 20, 3, false, };
static const attack ATK_Water_Gun = { "Water Gun", TYPE_WATER, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 1, false, };
static const attack ATK_Waterfall = { "Waterfall", TYPE_WATER, 12, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 7, 2, false, };
static const attack ATK_Water_Shuriken = { "Water Shuriken", TYPE_WATER, 6, 14, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	9, 14, 2, false, };
static const attack ATK_Wing_Attack = { "Wing Attack", TYPE_FLYING, 5, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 11, 2, false, };
static const attack ATK_Yawn = { "Yawn", TYPE_NORMAL, 0, 12, 4, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 13, 3, false, };
static const attack ATK_Zen_Headbutt = { "Zen Headbutt", TYPE_PSYCHIC, 8, 6, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 9, 2, false, };
static const attack ATK_Acid_Spray = { "Acid Spray", TYPE_POISON, 20, -45, 0, 0, 0, 0, 1000, 0, 0, 0, -2,
	20, 50, 6, false, };
static const attack ATK_Acrobatics = { "Acrobatics", TYPE_FLYING, 110, -55, 0, 125, 0, 0, 0, 2, 0, 0, 0,
	100, 100, 4, false, };
static const attack ATK_Aerial_Ace = { "Aerial Ace", TYPE_FLYING, 55, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 5, false, };
static const attack ATK_Aeroblast = { "Aeroblast", TYPE_FLYING, 170, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	200, 100, 7, false, };
static const attack ATK_Air_Cutter = { "Air Cutter", TYPE_FLYING, 45, -35, 0, 300, 0, 0, 0, 1, 0, 0, 0,
	55, 50, 5, false, };
static const attack ATK_Ancient_Power = { "Ancient Power", TYPE_ROCK, 60, -45, 0, 100, 0, 0, 0, 1, 0, 0, 0,
	70, 33, 7, false, };
static const attack ATK_Aqua_Jet = { "Aqua Jet", TYPE_WATER, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 5, false, };
static const attack ATK_Aqua_Step = { "Aqua Step", TYPE_WATER, 55, -40, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	55, -40, 7, false, };
static const attack ATK_Aqua_Tail = { "Aqua Tail", TYPE_WATER, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Aura_Sphere = { "Aura Sphere", TYPE_FIGHTING, 100, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 4, false, };
static const attack ATK_Aura_Wheel = { "Aura Wheel", TYPE_ELECTRIC, 100, -45, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	100, 45, 5, false, };
static const attack ATK_Aurora_Beam = { "Aurora Beam", TYPE_ICE, 80, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 7, false, };
static const attack ATK_Avalanche = { "Avalanche", TYPE_ICE, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 5, false, };
static const attack ATK_Behemoth_Bash = { "Behemoth Bash", TYPE_STEEL, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	125, 50, 3, true, };
static const attack ATK_Behemoth_Blade = { "Behemoth Blade", TYPE_STEEL, 100, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	200, 100, 7, true, };
static const attack ATK_Blast_Burn = { "Blast Burn", TYPE_FIRE, 110, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 50, 7, false, };
static const attack ATK_Blaze_Kick = { "Blaze Kick", TYPE_FIRE, 60, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	40, 33, 2, false, };
static const attack ATK_Bleakwind_Storm = { "Bleakwind Storm", TYPE_FLYING, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	150, 100, 5, false, };
static const attack ATK_Blizzard = { "Blizzard", TYPE_ICE, 140, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	130, 100, 6, false, };
static const attack ATK_Body_Slam = { "Body Slam", TYPE_NORMAL, 50, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Boomburst = { "Boomburst", TYPE_NORMAL, 150, -70, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	150, 100, 5, false, };
static const attack ATK_Bone_Club = { "Bone Club", TYPE_GROUND, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	40, 33, 3, false, };
static const attack ATK_Brave_Bird = { "Brave Bird", TYPE_FLYING, 130, -55, 0, 0, 1000, 0, 0, 0, -3, 0, 0,
	130, 100, 4, false, };
static const attack ATK_Breaking_Swipe = { "Breaking Swipe", TYPE_DRAGON, 50, -35, 0, 0, 0, 500, 0, 0, 0, -1, 0,
	45, 33, 2, false, };
static const attack ATK_Brick_Break = { "Brick Break", TYPE_FIGHTING, 40, -40, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	40, 33, 3, false, };
static const attack ATK_Brutal_Swing = { "Brutal Swing", TYPE_DARK, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 33, 4, false, };
static const attack ATK_Brine = { "Brine", TYPE_WATER, 60, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 5, false, };
static const attack ATK_Bubble_Beam = { "Bubble Beam", TYPE_WATER, 25, -40, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	45, 33, 4, false, };
static const attack ATK_Bug_Buzz = { "Bug Buzz", TYPE_BUG, 100, -60, 0, 0, 0, 0, 300, 0, 0, 0, -1,
	95, 50, 7, false, };
static const attack ATK_Bulldoze = { "Bulldoze", TYPE_GROUND, 45, -45, 0, 0, 0, 0, 500, 0, 0, 0, -1,
	80, 50, 7, false, };
static const attack ATK_Clanging_Scales = { "Clanging Scales", TYPE_DRAGON, 120, -45, 0, 0, 1000, 0, 0, 0, -1, 0, 0,
	120, 100, 7, false, };
static const attack ATK_Close_Combat = { "Close Combat", TYPE_FIGHTING, 100, -45, 0, 0, 1000, 0, 0, 0, -2, 0, 0,
	105, 100, 5, false, };
static const attack ATK_Crabhammer = { "Crabhammer", TYPE_WATER, 85, -50, 0, 125, 0, 0, 0, 2, 0, 0, 0,
	85, 50, 4, false, };
static const attack ATK_Cross_Chop = { "Cross Chop", TYPE_FIGHTING, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 50, 3, false, };
static const attack ATK_Cross_Poison = { "Cross Poison", TYPE_POISON, 50, -35, 0, 125, 0, 0, 0, 2, 0, 0, 0,
	40, 33, 3, false, };
static const attack ATK_Crunch = { "Crunch", TYPE_DARK, 70, -45, 0, 0, 0, 0, 200, 0, 0, 0, -1,
	65, 33, 6, false, };
static const attack ATK_Darkest_Lariat = { "Darkest Lariat", TYPE_DARK, 120, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 4, false, };
static const attack ATK_Dark_Pulse = { "Dark Pulse", TYPE_DARK, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 6, false, };
static const attack ATK_Dazzling_Gleam = { "Dazzling Gleam", TYPE_FAIRY, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  100, 50, 7, false, };
static const attack ATK_Dig = { "Dig", TYPE_GROUND, 70, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 9, false, };
static const attack ATK_Disarming_Voice = { "Disarming Voice", TYPE_FAIRY, 70, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 8, false, };
static const attack ATK_Discharge = { "Discharge", TYPE_ELECTRIC, 55, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 33, 5, false, };
static const attack ATK_Doom_Desire = { "Doom Desire", TYPE_STEEL, 75, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 33, 3, false, };
static const attack ATK_Double_Iron_Bash = { "Double Iron Bash", TYPE_STEEL, 50, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 4, false, };
static const attack ATK_Draco_Meteor = { "Draco Meteor", TYPE_DRAGON, 150, -65, 0, 1000, 0, 0, 0, -2, 0, 0, 0,
	150, 100, 7, false, };
static const attack ATK_Dragon_Ascent = { "Dragon Ascent", TYPE_FLYING, 150, -70, 0, 0, 1000, 0, 0, 0, -1, 0, 0,
	140, 50, 7, false, };
static const attack ATK_Dragon_Claw = { "Dragon Claw", TYPE_DRAGON, 50, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 3, false, };
static const attack ATK_Dragon_Energy = { "Dragon Energy", TYPE_DRAGON, 100, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	155, 50, 7, false, };
static const attack ATK_Dragon_Pulse = { "Dragon Pulse", TYPE_DRAGON, 90, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 7, false, };
static const attack ATK_Drain_Punch = { "Drain Punch", TYPE_FIGHTING, 20, -40, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	50, 33, 5, false, };
static const attack ATK_Draining_Kiss = { "Draining Kiss", TYPE_FAIRY, 60, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 5, false, };
static const attack ATK_Drill_Peck = { "Drill Peck", TYPE_FLYING, 65, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 5, false, };
static const attack ATK_Drill_Run = { "Drill Run", TYPE_GROUND, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 6, false, };
static const attack ATK_Dynamax_Cannon = { "Dynamax Cannon", TYPE_DRAGON, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	215, 100, 3, true, };
static const attack ATK_Dynamic_Punch = { "Dynamic Punch", TYPE_FIGHTING, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 5, false, };
static const attack ATK_Earthquake = { "Earthquake", TYPE_GROUND, 110, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 7, false, };
static const attack ATK_Earth_Power = { "Earth Power", TYPE_GROUND, 90, -55, 0, 0, 0, 0, 100, 0, 0, 0, -1,
	100, 50, 7, false, };
static const attack ATK_Energy_Ball = { "Energy Ball", TYPE_GRASS, 90, -55, 0, 0, 0, 0, 100, 0, 0, 0, -1,
	90, 50, 8, false, };
static const attack ATK_Feather_Dance = { "Feather Dance", TYPE_FLYING, 35, -50, 0, 0, 0, 1000, 0, 0, 0, -2, 0,
	35, 50, 6, false, };
static const attack ATK_Fell_Stinger = { "Fell Stinger", TYPE_BUG, 20, -35, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	45, 33, 4, false, };
static const attack ATK_Fire_Blast = { "Fire Blast", TYPE_FIRE, 140, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 8, false, };
static const attack ATK_Fire_Punch = { "Fire Punch", TYPE_FIRE, 60, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Flame_Burst = { "Flame Burst", TYPE_FIRE, 70, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 50, 5, false, };
static const attack ATK_Flame_Charge = { "Flame Charge", TYPE_FIRE, 65, -50, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	70, 33, 8, false, };
static const attack ATK_Flame_Wheel = { "Flame Wheel", TYPE_FIRE, 60, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 50, 5, false, };
static const attack ATK_Flamethrower = { "Flamethrower", TYPE_FIRE, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 4, false, };
static const attack ATK_Flash_Cannon = { "Flash Cannon", TYPE_STEEL, 110, -70, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 100, 5, false, };
static const attack ATK_Flower_Trick = { "Flower Trick", TYPE_GRASS, 30, -35, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	75, 33, 5, false, };
static const attack ATK_Fly = { "Fly", TYPE_FLYING, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 4, false, };
static const attack ATK_Flying_Press = { "Flying Press", TYPE_FIGHTING, 90, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	115, 50, 5, false, };
static const attack ATK_Focus_Blast = { "Focus Blast", TYPE_FIGHTING, 150, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 7, false, };
static const attack ATK_Foul_Play = { "Foul Play", TYPE_DARK, 60, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 50, 4, false, };
static const attack ATK_Freeze_Shock = { "Freeze Shock", TYPE_ICE, 120, -60, 0, 0, 0, 300, 0, 0, 0, -1, 0,
	160, 100, 3, true, };
static const attack ATK_Frenzy_Plant = { "Frenzy Plant", TYPE_GRASS, 100, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 5, false, };
static const attack ATK_Frustration = { "Frustration", TYPE_NORMAL, 10, -70, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 33, 4, false, };
static const attack ATK_Fusion_Bolt = { "Fusion Bolt", TYPE_ELECTRIC, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 4, false, };
static const attack ATK_Fusion_Flare = { "Fusion Flare", TYPE_FIRE, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 4, false, };
static const attack ATK_Future_Sight = { "Future Sight", TYPE_PSYCHIC, 110, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	115, 100, 5, false, };
static const attack ATK_Giga_Impact = { "Giga Impact", TYPE_NORMAL, 150, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	200, 100, 9, false, };
static const attack ATK_Glaciate = { "Glaciate", TYPE_ICE, 60, -40, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	160, 100, 5, false, };
static const attack ATK_Grass_Knot = { "Grass Knot", TYPE_GRASS, 90, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 5, false, };
static const attack ATK_Gunk_Shot = { "Gunk Shot", TYPE_POISON, 130, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	130, 100, 6, false, };
static const attack ATK_Gyro_Ball = { "Gyro Ball", TYPE_STEEL, 80, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 7, false, };
static const attack ATK_Heat_Wave = { "Heat Wave", TYPE_FIRE, 95, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	95, 100, 6, false, };
static const attack ATK_Heavy_Slam = { "Heavy Slam", TYPE_STEEL, 70, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 50, 4, false, };
static const attack ATK_High_Horsepower = { "High Horsepower", TYPE_GROUND, 100, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	105, 100, 3, false, };
static const attack ATK_High_Jump_Kick = { "High Jump Kick", TYPE_FIGHTING, 110, -55, 0, 0, 100, 0, 0, 0, -4, 0, 0,
	90, 100, 3, false, };
static const attack ATK_Horn_Attack = { "Horn Attack", TYPE_NORMAL, 40, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 4, false, };
static const attack ATK_Hurricane = { "Hurricane", TYPE_FLYING, 110, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	105, 100, 5, false, };
static const attack ATK_Hydro_Cannon = { "Hydro Cannon", TYPE_WATER, 80, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 4, false, };
// FIXME there appear to be two hydro pumps?
// https://db.pokemongohub.net/move/135
// https://db.pokemongohub.net/move/107
static const attack ATK_Hydro_Pump = { "Hydro Pump", TYPE_WATER, 130, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	135, 100, 7, false, };
static const attack ATK_Hyper_Beam = { "Hyper Beam", TYPE_NORMAL, 150, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	150, 100, 8, false, };
static const attack ATK_Hyper_Fang = { "Hyper Fang", TYPE_NORMAL, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 5, false, };
static const attack ATK_Ice_Beam = { "Ice Beam", TYPE_ICE, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	95, 50, 7, false, };
static const attack ATK_Ice_Burn = { "Ice Burn", TYPE_ICE, 120, -60, 0, 0, 0, 0, 300, 0, 0, 0, -1,
	90, 50, 4, true, };
static const attack ATK_Ice_Punch = { "Ice Punch", TYPE_ICE, 60, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Icicle_Spear = { "Icicle Spear", TYPE_ICE, 65, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 4, false, };
static const attack ATK_Icy_Wind = { "Icy Wind", TYPE_ICE, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	65, 33, 7, false, };
static const attack ATK_Iron_Head = { "Iron Head", TYPE_STEEL, 70, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 4, false, };
static const attack ATK_Last_Resort = { "Last Resort", TYPE_NORMAL, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 6, false, };
static const attack ATK_Leaf_Blade = { "Leaf Blade", TYPE_GRASS, 70, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 5, false, };
static const attack ATK_Leaf_Storm = { "Leaf Storm", TYPE_GRASS, 130, -55, 0, 1000, 0, 0, 0, -2, 0, 0, 0,
	130, 100, 5, false, };
static const attack ATK_Leaf_Tornado = { "Leaf Tornado", TYPE_GRASS, 45, -40, 0, 0, 0, 500, 0, 0, 0, -2, 0,
	45, 33, 6, false, };
static const attack ATK_Liquidation = { "Liquidation", TYPE_WATER, 70, -45, 0, 0, 0, 0, 300, 0, 0, 0, -1,
	70, 33, 6, false, };
static const attack ATK_Low_Sweep = { "Low Sweep", TYPE_FIGHTING, 40, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	40, 33, 4, false, };
static const attack ATK_Lunge = { "Lunge", TYPE_BUG, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	55, 33, 6, false, };
static const attack ATK_Luster_Purge = { "Luster Purge", TYPE_PSYCHIC, 120, -60, 0, 0, 0, 0, 500, 0, 0, 0, -1,
	100, 100, 3, false, };
static const attack ATK_Magma_Storm = { "Magma Storm", TYPE_FIRE, 65, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	75, 33, 5, false, };
static const attack ATK_Magnet_Bomb = { "Magnet Bomb", TYPE_STEEL, 70, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	75, 33, 6, false, };
static const attack ATK_Megahorn = { "Megahorn", TYPE_BUG, 110, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	105, 100, 4, false, };
static const attack ATK_Meteor_Beam = { "Meteor Beam", TYPE_ROCK, 120, -60, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	140, 100, 4, false, };
static const attack ATK_Meteor_Mash = { "Meteor Mash", TYPE_STEEL, 100, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 5, false, };
static const attack ATK_Mirror_Coat = { "Mirror Coat", TYPE_PSYCHIC, 60, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 5, false, };
static const attack ATK_Mirror_Shot = { "Mirror Shot", TYPE_STEEL, 35, -35, 0, 0, 0, 300, 0, 0, 0, -1, 0,
	50, 33, 5, false, };
static const attack ATK_Mist_Ball = { "Mist Ball", TYPE_PSYCHIC, 120, -60, 0, 0, 0, 500, 0, 0, 0, -1, 0,
	105, 100, 4, false, };
static const attack ATK_Moonblast = { "Moonblast", TYPE_FAIRY, 110, -60, 0, 0, 0, 100, 0, 0, 0, -1, 0,
	130, 100, 8, false, };
static const attack ATK_Moongeist_Beam = { "Moongeist Beam", TYPE_GHOST, 135, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	230, 100, 6, true, };
static const attack ATK_Mud_Bomb = { "Mud Bomb", TYPE_GROUND, 60, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 5, false, };
static const attack ATK_Muddy_Water = { "Muddy Water", TYPE_WATER, 35, -35, 0, 0, 0, 300, 0, 0, 0, -1, 0,
	45, 33, 4, false, };
static const attack ATK_Mystical_Fire = { "Mystical Fire", TYPE_FIRE, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	60, 33, 4, false, };
static const attack ATK_Natures_Madness = { "Nature's Madness", TYPE_FAIRY, 80, -50, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	90, 55, 4, false, };
static const attack ATK_Night_Shade = { "Night Shade", TYPE_GHOST, 70, -45, 0, 125, 0, 0, 0, 2, 0, 0, 0,
	60, 50, 5, false, };
static const attack ATK_Night_Slash = { "Night Slash", TYPE_DARK, 50, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 4, false, };
static const attack ATK_Oblivion_Wing = { "Oblivion Wing", TYPE_FLYING, 85, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 4, false, };
static const attack ATK_Obstruct = { "Obstruct", TYPE_DARK, 15, -40, 0, 0, 1000, 0, 1000, 0, 1, 0, -1,
	20, 33, 3, false, };
static const attack ATK_Octazooka = { "Octazooka", TYPE_WATER, 50, -50, 0, 0, 0, 500, 0, 0, 0, -2, 0,
	55, 50, 5, false, };
static const attack ATK_Ominous_Wind = { "Ominous Wind", TYPE_GHOST, 45, -45, 0, 100, 100, 0, 0, 1, 1, 0, 0,
	55, 33, 5, false, };
static const attack ATK_Origin_Pulse = { "Origin Pulse", TYPE_WATER, 130, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 3, false, };
static const attack ATK_Outrage = { "Outrage", TYPE_DRAGON, 110, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	110, 50, 8, false, };
static const attack ATK_Overheat = { "Overheat", TYPE_FIRE, 130, -55, 0, 1000, 0, 0, 0, -2, 0, 0, 0,
	160, 100, 8, false, };
static const attack ATK_Parabolic_Charge = { "Parabolic Charge", TYPE_ELECTRIC, 70, -50, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	70, 50, 6, false, };
static const attack ATK_Payback = { "Payback", TYPE_DARK, 110, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	95, 100, 4, false, };
static const attack ATK_Petal_Blizzard = { "Petal Blizzard", TYPE_GRASS, 110, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	110, 100, 5, false, };
static const attack ATK_Play_Rough = { "Play Rough", TYPE_FAIRY, 90, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 6, false, };
static const attack ATK_Poison_Fang = { "Poison Fang", TYPE_POISON, 45, -40, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	30, 33, 3, false, };
static const attack ATK_Poltergeist = { "Poltergeist", TYPE_GHOST, 150, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 7, false, };
static const attack ATK_Power_Gem = { "Power Gem", TYPE_ROCK, 85, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 6, false, };
static const attack ATK_Power_Up_Punch = { "Power-Up Punch", TYPE_FIGHTING, 20, -35, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Power_Whip = { "Power Whip", TYPE_GRASS, 90, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 5, false, };
static const attack ATK_Precipice_Blades = { "Precipice Blades", TYPE_GROUND, 130, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 3, false, };
static const attack ATK_Psybeam = { "Psybeam", TYPE_PSYCHIC, 70, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 6, false, };
static const attack ATK_Psychic = { "Psychic", TYPE_PSYCHIC, 75, -55, 0, 0, 0, 0, 100, 0, 0, 0, -1,
	95, 50, 7, false, };
static const attack ATK_Psychic_Fangs = { "Psychic Fangs", TYPE_PSYCHIC, 40, -35, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	25, 33, 2, false, };
static const attack ATK_Psycho_Boost = { "Psycho Boost", TYPE_PSYCHIC, 70, -35, 0, 1000, 0, 0, 0, -2, 0, 0, 0,
	70, 50, 8, false, };
static const attack ATK_Psyshock = { "Psyshock", TYPE_PSYCHIC, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 5, false, };
static const attack ATK_Psystrike = { "Psystrike", TYPE_PSYCHIC, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	95, 50, 5, false, };
static const attack ATK_Rage_Fist = { "Rage Fist", TYPE_GHOST, 50, -35, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	100, 50, 6, false, };
static const attack ATK_Razor_Shell = { "Razor Shell", TYPE_WATER, 35, -35, 0, 0, 0, 0, 500, 0, 0, 0, -1,
	55, 33, 3, false, };
static const attack ATK_Return = { "Return", TYPE_NORMAL, 130, -70, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	25, 33, 1, false, };
static const attack ATK_Roar_of_Time = { "Roar of Time", TYPE_DRAGON, 150, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	160, 100, 4, true, };
static const attack ATK_Rock_Blast = { "Rock Blast", TYPE_ROCK, 50, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Rock_Slide = { "Rock Slide", TYPE_ROCK, 65, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	75, 50, 5, false, };
static const attack ATK_Rock_Tomb = { "Rock Tomb", TYPE_ROCK, 80, -50, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	65, 50, 6, false, };
static const attack ATK_Rock_Wrecker = { "Rock Wrecker", TYPE_ROCK, 110, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	110, 50, 7, false, };
static const attack ATK_Sacred_Fire = { "Sacred Fire", TYPE_FIRE, 130, -65, 0, 0, 0, 500, 0, 0, 0, -1, 0,
	120, 100, 5, false, };
static const attack ATK_Sacred_Sword = { "Sacred Sword", TYPE_FIGHTING, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 2, false, };
static const attack ATK_Sand_Tomb = { "Sand Tomb", TYPE_GROUND, 25, -40, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	60, 33, 8, false, };
static const attack ATK_Sandsear_Storm = { "Sandsear Storm", TYPE_GROUND, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	150, 100, 5, false, };
// FIXME there appear to be two scalds?
// https://db.pokemongohub.net/move/134
// https://db.pokemongohub.net/move/106
static const attack ATK_Scald = { "Scald", TYPE_WATER, 85, -50, 0, 0, 0, 300, 0, 0, 0, -1, 0,
	75, 50, 7, false, };
static const attack ATK_Scorching_Sands = { "Scorching Sands", TYPE_GROUND, 80, -50, 0, 0, 0, 300, 0, 0, 0, -1, 0,
	90, 50, 6, false, };
static const attack ATK_Seed_Bomb = { "Seed Bomb", TYPE_GRASS, 65, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 4, false, };
static const attack ATK_Seed_Flare = { "Seed Flare", TYPE_GRASS, 130, -75, 0, 0, 0, 0, 400, 0, 0, 0, -2,
	115, 100, 5, false, };
static const attack ATK_Shadow_Ball = { "Shadow Ball", TYPE_GHOST, 100, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 6, false, };
static const attack ATK_Shadow_Bone = { "Shadow Bone", TYPE_GHOST, 80, -45, 0, 0, 0, 0, 200, 0, 0, 0, -1,
	85, 50, 6, false, };
static const attack ATK_Shadow_Force = { "Shadow Force", TYPE_GHOST, 120, -90, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, -4, false, };
static const attack ATK_Shadow_Punch = { "Shadow Punch", TYPE_GHOST, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	35, 33, 3, false, };
static const attack ATK_Shadow_Sneak = { "Shadow Sneak", TYPE_GHOST, 50, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 6, false, };
static const attack ATK_Signal_Beam = { "Signal Beam", TYPE_BUG, 75, -55, 0, 0, 0, 200, 200, 0, 0, -1, -1,
	75, 50, 6, false, };
static const attack ATK_Silver_Wind = { "Silver Wind", TYPE_BUG, 60, -45, 0, 100, 100, 0, 0, 1, 1, 0, 0,
	65, 33, 7, false, };
static const attack ATK_Skull_Bash = { "Skull Bash", TYPE_NORMAL, 130, -75, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	130, 100, 6, false, };
static const attack ATK_Sky_Attack = { "Sky Attack", TYPE_FLYING, 85, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 4, false, };
static const attack ATK_Sludge = { "Sludge", TYPE_POISON, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Sludge_Bomb = { "Sludge Bomb", TYPE_POISON, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 5, false, };
static const attack ATK_Sludge_Wave = { "Sludge Wave", TYPE_POISON, 110, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	105, 100, 6, false, };
static const attack ATK_Solar_Beam = { "Solar Beam", TYPE_GRASS, 150, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	180, 100, 10, false, };
static const attack ATK_Spacial_Rend = { "Spacial Rend", TYPE_DRAGON, 95, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	160, 100, 5, true, };
static const attack ATK_Sparkling_Aria = { "Sparkling Aria", TYPE_WATER, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 33, 6, false, };
static const attack ATK_Spirit_Shackle = { "Spirit Shackle", TYPE_GHOST, 50, -40, 0, 0, 0, 0, 330, 0, 0, 0, -1,
	70, 33, 5, false, };
static const attack ATK_Stomp = { "Stomp", TYPE_NORMAL, 55, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 50, 3, false, };
static const attack ATK_Stone_Edge = { "Stone Edge", TYPE_ROCK, 100, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	105, 100, 5, false, };
static const attack ATK_Struggle = { "Struggle", TYPE_NORMAL, 35, -100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	35, 0, 4, false, };
static const attack ATK_Submission = { "Submission", TYPE_FIGHTING, 60, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 50, 4, false, };
static const attack ATK_Sunsteel_Strike = { "Sunsteel Strike", TYPE_STEEL, 135, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	230, 100, 6, true, };
static const attack ATK_Superpower = { "Superpower", TYPE_FIGHTING, 85, -40, 0, 1000, 1000, 0, 0, -1, -1, 0, 0,
	85, 50, 6, false, };
static const attack ATK_Surf = { "Surf", TYPE_WATER, 75, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 3, false, };
static const attack ATK_Swift = { "Swift", TYPE_NORMAL, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 6, false, };
static const attack ATK_Synchronoise = { "Synchronoise", TYPE_PSYCHIC, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 5, false, };
static const attack ATK_Techno_Blast = { "Techno Blast", TYPE_NORMAL, 120, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 4, false, };
static const attack ATK_Thunder = { "Thunder", TYPE_ELECTRIC, 100, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 100, 7, false, };
static const attack ATK_Thunder_Punch = { "Thunder Punch", TYPE_ELECTRIC, 60, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Thunderbolt = { "Thunderbolt", TYPE_ELECTRIC, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 5, false, };
static const attack ATK_Torch_Song = { "Torch Song", TYPE_FIRE, 70, -45, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	100, 50, 7, false, };
static const attack ATK_Trailblaze = { "Trailblaze", TYPE_GRASS, 65, -45, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	65, 50, 4, false, };
static const attack ATK_Tri_Attack = { "Tri Attack", TYPE_NORMAL, 65, -50, 0, 0, 0, 500, 500, 0, 0, -1, -1,
	75, 50, 5, false, };
static const attack ATK_Triple_Axel = { "Triple Axel", TYPE_ICE, 60, -45, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	60, 33, 4, false, };
static const attack ATK_Twister = { "Twister", TYPE_DRAGON, 45, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 6, false, };
static const attack ATK_Upper_Hand = { "Upper Hand", TYPE_FIGHTING, 70, -40, 0, 0, 0, 0, 300, 0, 0, 0, -1,
	50, 33, 4, false, };
static const attack ATK_V_Create = { "V-Create", TYPE_FIRE, 95, -40, 0, 0, 1000, 0, 0, 0, -3, 0, 0,
	105, 33, 6, false, };
static const attack ATK_Vise_Grip = { "Vise Grip", TYPE_NORMAL, 40, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	35, 33, 4, false, };
static const attack ATK_Volt_Tackle = { "Volt Tackle", TYPE_ELECTRIC, 90, -50, 0, 0, 1000, 0, 0, 0, -1, 0, 0,
	90, 33, 7, false, };
static const attack ATK_Water_Pulse = { "Water Pulse", TYPE_WATER, 80, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 6, false, };
static const attack ATK_Weather_Ball_Fire = { "Weather Ball", TYPE_FIRE, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 3, false, };
static const attack ATK_Weather_Ball_Ice = { "Weather Ball", TYPE_ICE, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 3, false, };
static const attack ATK_Weather_Ball_Rock = { "Weather Ball", TYPE_ROCK, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 4, false, };
static const attack ATK_Weather_Ball_Normal = { "Weather Ball", TYPE_NORMAL, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 3, false, };
static const attack ATK_Weather_Ball_Water = { "Weather Ball", TYPE_WATER, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 3, false, };
static const attack ATK_Wild_Charge = { "Wild Charge", TYPE_ELECTRIC, 100, -45, 0, 0, 1000, 0, 0, 0, -2, 0, 0,
	90, 50, 5, false, };
static const attack ATK_Wildbolt_Storm = { "Wildbolt Storm", TYPE_ELECTRIC, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	150, 100, 5, false, };
static const attack ATK_Wrap = { "Wrap", TYPE_NORMAL, 60, -45, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	25, 33, 6, false, };
static const attack ATK_XScissor = { "X-Scissor", TYPE_BUG, 65, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 3, false, };
static const attack ATK_Zap_Cannon = { "Zap Cannon", TYPE_ELECTRIC, 150, -80, 0, 0, 0, 660, 0, 0, 0, -1, 0,
	140, 100, 7, false, };

static const attack* const attacks[] = {
  &ATK_Acid,
  &ATK_Air_Slash,
  &ATK_Astonish,
  &ATK_Bite,
  &ATK_Bubble,
  &ATK_Bug_Bite,
  &ATK_Bullet_Punch,
  &ATK_Bullet_Seed,
  &ATK_Charge_Beam,
  &ATK_Charm,
  &ATK_Confusion,
  &ATK_Counter,
  &ATK_Cut,
  &ATK_Double_Kick,
  &ATK_Dragon_Breath,
  &ATK_Dragon_Tail,
  &ATK_Ember,
  &ATK_Extrasensory,
  &ATK_Feint_Attack,
  &ATK_Fire_Fang,
  &ATK_Fairy_Wind,
  &ATK_Fire_Spin,
  &ATK_Force_Palm,
  &ATK_Frost_Breath,
  &ATK_Fury_Cutter,
  &ATK_Geomancy,
  &ATK_Gust,
  &ATK_Hex,
  &ATK_Hidden_Power,
  &ATK_Ice_Fang,
  &ATK_Ice_Shard,
  &ATK_Incinerate,
  &ATK_Infestation,
  &ATK_Iron_Tail,
  &ATK_Karate_Chop,
  &ATK_Leafage,
  &ATK_Lick,
  &ATK_Lock_On,
  &ATK_Low_Kick,
  &ATK_Magical_Leaf,
  &ATK_Metal_Claw,
  &ATK_Metal_Sound,
  &ATK_Mud_Shot,
  &ATK_Mud_Slap,
  &ATK_Peck,
  &ATK_Poison_Jab,
  &ATK_Poison_Sting,
  &ATK_Pound,
  &ATK_Powder_Snow,
  &ATK_Present,
  &ATK_Psycho_Cut,
  &ATK_Psywave,
  &ATK_Quick_Attack,
  &ATK_Razor_Leaf,
  &ATK_Rock_Smash,
  &ATK_Rock_Throw,
  &ATK_Rollout,
  &ATK_Sand_Attack,
  &ATK_Scratch,
  &ATK_Shadow_Claw,
  &ATK_Smack_Down,
  &ATK_Snarl,
  &ATK_Spark,
  &ATK_Splash,
  &ATK_Steel_Wing,
  &ATK_Struggle_Bug,
  &ATK_Sucker_Punch,
  &ATK_Tackle,
  &ATK_Take_Down,
  &ATK_Thunder_Fang,
  &ATK_Thunder_Shock,
  &ATK_Transform,
  &ATK_Vine_Whip,
  &ATK_Volt_Switch,
  &ATK_Water_Gun,
  &ATK_Waterfall,
  &ATK_Water_Shuriken,
  &ATK_Wing_Attack,
  &ATK_Yawn,
  &ATK_Zen_Headbutt,
  &ATK_Acid_Spray,
  &ATK_Acrobatics,
  &ATK_Aerial_Ace,
  &ATK_Aeroblast,
  &ATK_Air_Cutter,
  &ATK_Ancient_Power,
  &ATK_Aqua_Jet,
  &ATK_Aqua_Step,
  &ATK_Aqua_Tail,
  &ATK_Aura_Sphere,
  &ATK_Aura_Wheel,
  &ATK_Aurora_Beam,
  &ATK_Avalanche,
  &ATK_Behemoth_Bash,
  &ATK_Behemoth_Blade,
  &ATK_Blast_Burn,
  &ATK_Bleakwind_Storm,
  &ATK_Blizzard,
  &ATK_Body_Slam,
  &ATK_Boomburst,
  &ATK_Bone_Club,
  &ATK_Brave_Bird,
  &ATK_Breaking_Swipe,
  &ATK_Brick_Break,
  &ATK_Brutal_Swing,
  &ATK_Brine,
  &ATK_Bubble_Beam,
  &ATK_Bug_Buzz,
  &ATK_Bulldoze,
  &ATK_Close_Combat,
  &ATK_Crabhammer,
  &ATK_Cross_Chop,
  &ATK_Cross_Poison,
  &ATK_Crunch,
  &ATK_Darkest_Lariat,
  &ATK_Dark_Pulse,
  &ATK_Dazzling_Gleam,
  &ATK_Dig,
  &ATK_Disarming_Voice,
  &ATK_Discharge,
  &ATK_Doom_Desire,
  &ATK_Double_Iron_Bash,
  &ATK_Draco_Meteor,
  &ATK_Dragon_Ascent,
  &ATK_Dragon_Claw,
  &ATK_Dragon_Energy,
  &ATK_Dragon_Pulse,
  &ATK_Drain_Punch,
  &ATK_Draining_Kiss,
  &ATK_Drill_Peck,
  &ATK_Drill_Run,
  &ATK_Dynamic_Punch,
  &ATK_Dynamax_Cannon,
  &ATK_Earthquake,
  &ATK_Earth_Power,
  &ATK_Energy_Ball,
  &ATK_Fell_Stinger,
  &ATK_Fire_Blast,
  &ATK_Fire_Punch,
  &ATK_Blaze_Kick,
  &ATK_Feather_Dance,
  &ATK_Flame_Burst,
  &ATK_Flame_Charge,
  &ATK_Flame_Wheel,
  &ATK_Flamethrower,
  &ATK_Flash_Cannon,
  &ATK_Flower_Trick,
  &ATK_Fly,
  &ATK_Flying_Press,
  &ATK_Focus_Blast,
  &ATK_Foul_Play,
  &ATK_Freeze_Shock,
  &ATK_Frenzy_Plant,
  &ATK_Frustration,
  &ATK_Fusion_Bolt,
  &ATK_Fusion_Flare,
  &ATK_Future_Sight,
  &ATK_Giga_Impact,
  &ATK_Glaciate,
  &ATK_Grass_Knot,
  &ATK_Gunk_Shot,
  &ATK_Gyro_Ball,
  &ATK_Heat_Wave,
  &ATK_Heavy_Slam,
  &ATK_High_Horsepower,
  &ATK_High_Jump_Kick,
  &ATK_Horn_Attack,
  &ATK_Hurricane,
  &ATK_Hydro_Cannon,
  &ATK_Hydro_Pump,
  &ATK_Hyper_Beam,
  &ATK_Hyper_Fang,
  &ATK_Ice_Beam,
  &ATK_Ice_Burn,
  &ATK_Ice_Punch,
  &ATK_Icicle_Spear,
  &ATK_Icy_Wind,
  &ATK_Iron_Head,
  &ATK_Last_Resort,
  &ATK_Leaf_Blade,
  &ATK_Leaf_Tornado,
  &ATK_Leaf_Storm,
  &ATK_Liquidation,
  &ATK_Low_Sweep,
  &ATK_Lunge,
  &ATK_Luster_Purge,
  &ATK_Magma_Storm,
  &ATK_Magnet_Bomb,
  &ATK_Megahorn,
  &ATK_Meteor_Beam,
  &ATK_Meteor_Mash,
  &ATK_Mirror_Coat,
  &ATK_Mirror_Shot,
  &ATK_Mist_Ball,
  &ATK_Moonblast,
  &ATK_Moongeist_Beam,
  &ATK_Mud_Bomb,
  &ATK_Muddy_Water,
  &ATK_Mystical_Fire,
  &ATK_Natures_Madness,
  &ATK_Night_Shade,
  &ATK_Night_Slash,
  &ATK_Oblivion_Wing,
  &ATK_Obstruct,
  &ATK_Octazooka,
  &ATK_Ominous_Wind,
  &ATK_Origin_Pulse,
  &ATK_Outrage,
  &ATK_Overheat,
  &ATK_Parabolic_Charge,
  &ATK_Payback,
  &ATK_Petal_Blizzard,
  &ATK_Play_Rough,
  &ATK_Poison_Fang,
  &ATK_Poltergeist,
  &ATK_Power_Gem,
  &ATK_Power_Up_Punch,
  &ATK_Power_Whip,
  &ATK_Precipice_Blades,
  &ATK_Psybeam,
  &ATK_Psychic,
  &ATK_Psychic_Fangs,
  &ATK_Psycho_Boost,
  &ATK_Psyshock,
  &ATK_Psystrike,
  &ATK_Rage_Fist,
  &ATK_Razor_Shell,
  &ATK_Return,
  &ATK_Roar_of_Time,
  &ATK_Rock_Blast,
  &ATK_Rock_Slide,
  &ATK_Rock_Tomb,
  &ATK_Rock_Wrecker,
  &ATK_Sacred_Fire,
  &ATK_Sacred_Sword,
  &ATK_Sand_Tomb,
  &ATK_Sandsear_Storm,
  &ATK_Scald,
  &ATK_Scorching_Sands,
  &ATK_Seed_Bomb,
  &ATK_Seed_Flare,
  &ATK_Shadow_Ball,
  &ATK_Shadow_Bone,
  &ATK_Shadow_Force,
  &ATK_Shadow_Punch,
  &ATK_Shadow_Sneak,
  &ATK_Signal_Beam,
  &ATK_Silver_Wind,
  &ATK_Skull_Bash,
  &ATK_Sky_Attack,
  &ATK_Sludge,
  &ATK_Sludge_Bomb,
  &ATK_Sludge_Wave,
  &ATK_Solar_Beam,
  &ATK_Spacial_Rend,
  &ATK_Sparkling_Aria,
  &ATK_Spirit_Shackle,
  &ATK_Stomp,
  &ATK_Stone_Edge,
  &ATK_Struggle,
  &ATK_Submission,
  &ATK_Sunsteel_Strike,
  &ATK_Superpower,
  &ATK_Surf,
  &ATK_Swift,
  &ATK_Synchronoise,
  &ATK_Techno_Blast,
  &ATK_Thunder,
  &ATK_Thunder_Punch,
  &ATK_Thunderbolt,
  &ATK_Torch_Song,
  &ATK_Trailblaze,
  &ATK_Tri_Attack,
  &ATK_Triple_Axel,
  &ATK_Twister,
  &ATK_Upper_Hand,
  &ATK_V_Create,
  &ATK_Vise_Grip,
  &ATK_Volt_Tackle,
  &ATK_Water_Pulse,
  &ATK_Weather_Ball_Fire,
  &ATK_Weather_Ball_Ice,
  &ATK_Weather_Ball_Rock,
  &ATK_Weather_Ball_Normal,
  &ATK_Weather_Ball_Water,
  &ATK_Wildbolt_Storm,
  &ATK_Wild_Charge,
  &ATK_Wrap,
  &ATK_XScissor,
  &ATK_Zap_Cannon,
};

const size_t ATTACKCOUNT = sizeof(attacks) / sizeof(*attacks);

static const attack* WIGLETT_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Mud_Slap,
  &ATK_Dig,
  &ATK_Surf,
  &ATK_Liquidation,
  NULL
};

static const attack* WUGTRIO_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Mud_Slap,
  &ATK_Dig,
  &ATK_Surf,
  &ATK_Liquidation,
  NULL
};

static const attack* DITTO_ATKS[] = {
  &ATK_Transform,
  &ATK_Struggle,
  NULL
};

static const attack* UNOWN_ATKS[] = {
  &ATK_Hidden_Power,
  &ATK_Struggle,
  NULL
};

static const attack* DEERLING_ATKS[] = {
  &ATK_Tackle,
  &ATK_Take_Down,
  &ATK_Seed_Bomb,
  &ATK_Wild_Charge,
  &ATK_Energy_Ball,
  &ATK_Trailblaze,
  NULL
};

static const attack* SAWSBUCK_ATKS[] = {
  &ATK_Feint_Attack,
  &ATK_Take_Down,
  &ATK_Hyper_Beam,
  &ATK_Megahorn,
  &ATK_Solar_Beam,
  &ATK_Wild_Charge,
  &ATK_Trailblaze,
  NULL
};

static const attack* TAROUNTULA_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Struggle_Bug,
  &ATK_XScissor,
  &ATK_Grass_Knot,
  &ATK_Lunge,
  NULL
};

static const attack* SPIDOPS_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Counter,
  &ATK_Rock_Tomb,
  &ATK_XScissor,
  &ATK_Grass_Knot,
  &ATK_Lunge,
  NULL
};

static const attack* SNUBBULL_ATKS[] = {
  &ATK_Bite,
  &ATK_Tackle,
  &ATK_Dazzling_Gleam,
  &ATK_Brick_Break,
  &ATK_Crunch,
  NULL
};

static const attack* GRANBULL_ATKS[] = {
  &ATK_Bite,
  &ATK_Snarl,
  &ATK_Charm,
  &ATK_Play_Rough,
  &ATK_Close_Combat,
  &ATK_Crunch,
  NULL
};

static const attack* TAPUBULU_ATKS[] = {
  &ATK_Rock_Smash,
  &ATK_Bullet_Seed,
  &ATK_Megahorn,
  &ATK_Dazzling_Gleam,
  &ATK_Solar_Beam,
  &ATK_Grass_Knot,
  &ATK_Natures_Madness,
  NULL
};

static const attack* TAPUFINI_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Hidden_Power,
  &ATK_Ice_Beam,
  &ATK_Moonblast,
  &ATK_Hydro_Pump,
  &ATK_Surf,
  &ATK_Natures_Madness,
  NULL
};

static const attack* TAPULELE_ATKS[] = {
  &ATK_Confusion,
  &ATK_Astonish,
  &ATK_Psyshock,
  &ATK_Draining_Kiss,
  &ATK_Moonblast,
  &ATK_Focus_Blast,
  &ATK_Future_Sight,
  &ATK_Natures_Madness,
  NULL
};

static const attack* PLUSLE_ATKS[] = {
  &ATK_Spark,
  &ATK_Quick_Attack,
  &ATK_Discharge,
  &ATK_Thunderbolt,
  &ATK_Swift,
  &ATK_Grass_Knot,
  NULL
};

static const attack* MINUN_ATKS[] = {
  &ATK_Spark,
  &ATK_Quick_Attack,
  &ATK_Discharge,
  &ATK_Thunderbolt,
  &ATK_Swift,
  &ATK_Grass_Knot,
  NULL
};

static const attack* TAPUKOKO_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Volt_Switch,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  &ATK_Dazzling_Gleam,
  &ATK_Brave_Bird,
  &ATK_Natures_Madness,
  NULL
};

static const attack* PACHIRISU_ATKS[] = {
  &ATK_Spark,
  &ATK_Volt_Switch,
  &ATK_Thunder_Punch,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  NULL
};

static const attack* GIMMIGHOUL_ATKS[] = {
  &ATK_Astonish,
  &ATK_Shadow_Ball,
  NULL
};

static const attack* GHOLDENGO_ATKS[] = {
  &ATK_Astonish,
  &ATK_Hex,
  &ATK_Power_Gem,
  &ATK_Shadow_Ball,
  &ATK_Dazzling_Gleam,
  &ATK_Focus_Blast,
  NULL
};

static const attack* IGGLYBUFF_ATKS[] = {
  &ATK_Pound,
  &ATK_Feint_Attack,
  &ATK_Shadow_Ball,
  &ATK_Psychic,
  &ATK_Swift,
  &ATK_Body_Slam,
  &ATK_Wild_Charge,
  NULL
};

static const attack* MISDREAVUS_ATKS[] = {
  &ATK_Astonish,
  &ATK_Hex,
  &ATK_Psywave,
  &ATK_Dark_Pulse,
  &ATK_Shadow_Sneak,
  &ATK_Ominous_Wind,
  NULL
};

static const attack* MISMAGIUS_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Hex,
  &ATK_Magical_Leaf,
  &ATK_Psywave,
  &ATK_Dark_Pulse,
  &ATK_Shadow_Ball,
  &ATK_Dazzling_Gleam,
  NULL
};

static const attack* FURFROU_ATKS[] = {
  &ATK_Bite,
  &ATK_Sucker_Punch,
  &ATK_Take_Down,
  &ATK_Sand_Attack,
  &ATK_Dark_Pulse,
  &ATK_Grass_Knot,
  &ATK_Surf,
  NULL
};

static const attack* MORELULL_ATKS[] = {
  &ATK_Astonish,
  &ATK_Seed_Bomb,
  &ATK_Dazzling_Gleam,
  NULL
};

static const attack* SHIINOTIC_ATKS[] = {
  &ATK_Astonish,
  &ATK_Seed_Bomb,
  &ATK_Moonblast,
  &ATK_Sludge_Bomb,
  NULL
};

static const attack* SKORUPI_ATKS[] = {
  &ATK_Poison_Sting,
  &ATK_Infestation,
  &ATK_Aqua_Tail,
  &ATK_Cross_Poison,
  &ATK_Sludge_Bomb,
  NULL
};

static const attack* CROAGUNK_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Poison_Jab,
  &ATK_Poison_Sting,
  &ATK_Low_Sweep,
  &ATK_Sludge_Bomb,
  &ATK_Brick_Break,
  NULL
};

static const attack* TOXICROAK_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Poison_Jab,
  &ATK_Poison_Sting,
  &ATK_Counter,
  &ATK_Shadow_Ball,
  &ATK_Sludge_Bomb,
  &ATK_Mud_Bomb,
  &ATK_Dynamic_Punch,
  NULL
};

static const attack* DRAPION_ATKS[] = {
  &ATK_Bite,
  &ATK_Poison_Sting,
  &ATK_Infestation,
  &ATK_Ice_Fang,
  &ATK_Aqua_Tail,
  &ATK_Sludge_Bomb,
  &ATK_Crunch,
  &ATK_Fell_Stinger,
  NULL
};

static const attack* TOGEDEMARU_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Spark,
  &ATK_Wild_Charge,
  &ATK_Gyro_Ball,
  &ATK_Fell_Stinger,
  NULL
};

static const attack* ROCKRUFF_ATKS[] = {
  &ATK_Tackle,
  &ATK_Rock_Throw,
  &ATK_Stone_Edge,
  &ATK_Rock_Tomb,
  &ATK_Crunch,
  &ATK_Trailblaze,
  NULL
};

static const attack* LYCANROC_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Rock_Throw,
  &ATK_Stone_Edge,
  &ATK_Drill_Run,
  &ATK_Crunch,
  &ATK_Trailblaze,
  NULL
};

static const attack* D_LYCANROC_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Rock_Throw,
  &ATK_Counter,
  &ATK_Stone_Edge,
  &ATK_Iron_Head,
  &ATK_Crunch,
  &ATK_Trailblaze,
  NULL
};

static const attack* N_LYCANROC_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Counter,
  &ATK_Stone_Edge,
  &ATK_Crunch,
  &ATK_Psychic_Fangs,
  &ATK_Trailblaze,
  NULL
};

static const attack* FIDOUGH_ATKS[] = {
  &ATK_Bite,
  &ATK_Charm,
  &ATK_Play_Rough,
  &ATK_Body_Slam,
  &ATK_Psychic_Fangs,
  NULL
};

static const attack* DACHSBUN_ATKS[] = {
  &ATK_Bite,
  &ATK_Charm,
  &ATK_Play_Rough,
  &ATK_Body_Slam,
  &ATK_Psychic_Fangs,
  NULL
};

static const attack* WYRDEER_ATKS[] = {
  &ATK_Tackle,
  &ATK_Zen_Headbutt,
  &ATK_Confusion,
  &ATK_Megahorn,
  &ATK_Psychic,
  &ATK_Stomp,
  &ATK_Wild_Charge,
  NULL
};

static const attack* SOLGALEO_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Fire_Spin,
  &ATK_Flamethrower,
  &ATK_Iron_Head,
  &ATK_Solar_Beam,
  &ATK_Psychic_Fangs,
  NULL
};

static const attack* CYNDAQUIL_ATKS[] = {
  &ATK_Ember,
  &ATK_Tackle,
  &ATK_Flamethrower,
  &ATK_Flame_Charge,
  &ATK_Swift,
  NULL
};

static const attack* QUILAVA_ATKS[] = {
  &ATK_Ember,
  &ATK_Tackle,
  &ATK_Flamethrower,
  &ATK_Dig,
  &ATK_Flame_Charge,
  NULL
};

static const attack* TYPHLOSION_ATKS[] = {
  &ATK_Ember,
  &ATK_Shadow_Claw,
  &ATK_Incinerate,
  &ATK_Thunder_Punch,
  &ATK_Fire_Blast,
  &ATK_Solar_Beam,
  &ATK_Overheat,
  &ATK_Blast_Burn,
  NULL
};

static const attack* H_TYPHLOSION_ATKS[] = {
  &ATK_Ember,
  &ATK_Hex,
  &ATK_Shadow_Ball,
  &ATK_Fire_Punch,
  &ATK_Wild_Charge,
  &ATK_Night_Shade,
  &ATK_Overheat,
  NULL
};

static const attack* LUNALA_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Confusion,
  &ATK_Air_Slash,
  &ATK_Shadow_Ball,
  &ATK_Moonblast,
  &ATK_Psychic,
  &ATK_Future_Sight,
  NULL
};

static const attack* MIMIKYU_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Charm,
  &ATK_Shadow_Sneak,
  &ATK_Shadow_Ball,
  &ATK_Play_Rough,
  NULL
};

static const attack* CACNEA_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Poison_Sting,
  &ATK_Sand_Attack,
  &ATK_Seed_Bomb,
  &ATK_Brick_Break,
  &ATK_Grass_Knot,
  &ATK_Payback,
  &ATK_Trailblaze,
  NULL
};

static const attack* CACTURNE_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Poison_Jab,
  &ATK_Sand_Attack,
  &ATK_Dark_Pulse,
  &ATK_Dynamic_Punch,
  &ATK_Grass_Knot,
  &ATK_Payback,
  &ATK_Trailblaze,
  NULL
};

static const attack* BLACEPHALON_ATKS[] = {
  &ATK_Astonish,
  &ATK_Incinerate,
  &ATK_Shadow_Ball,
  &ATK_Overheat,
  &ATK_Mystical_Fire,
  NULL
};

static const attack* COSMOG_ATKS[] = {
  &ATK_Splash,
  &ATK_Struggle,
  NULL
};

static const attack* COSMOEM_ATKS[] = {
  &ATK_Splash,
  &ATK_Struggle,
  NULL
};

static const attack* WIMPOD_ATKS[] = {
  &ATK_Struggle_Bug,
  &ATK_Bug_Buzz,
  NULL
};

static const attack* GOLISOPOD_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Shadow_Claw,
  &ATK_Metal_Claw,
  &ATK_Waterfall,
  &ATK_Aerial_Ace,
  &ATK_Aqua_Jet,
  &ATK_XScissor,
  &ATK_Razor_Shell,
  &ATK_Liquidation,
  NULL
};

static const attack* GLAMEOW_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Scratch,
  &ATK_Aerial_Ace,
  &ATK_Thunderbolt,
  &ATK_Play_Rough,
  NULL
};

static const attack* PURUGLY_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Scratch,
  &ATK_Aerial_Ace,
  &ATK_Thunder,
  &ATK_Play_Rough,
  NULL
};

static const attack* APPLIN_ATKS[] = {
  &ATK_Astonish,
  &ATK_Struggle,
  NULL
};

static const attack* APPLETUN_ATKS[] = {
  &ATK_Astonish,
  &ATK_Bullet_Seed,
  &ATK_Seed_Bomb,
  &ATK_Dragon_Pulse,
  &ATK_Energy_Ball,
  &ATK_Outrage,
  NULL
};

static const attack* FLAPPLE_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Bullet_Seed,
  &ATK_Seed_Bomb,
  &ATK_Dragon_Pulse,
  &ATK_Outrage,
  &ATK_Fly,
  NULL
};

static const attack* TATSUGIRI_ATKS[] = {
  &ATK_Take_Down,
  &ATK_Water_Gun,
  &ATK_Outrage,
  &ATK_Surf,
  &ATK_Hydro_Pump,
  &ATK_Muddy_Water,
  NULL
};

static const attack* DRAMPA_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Extrasensory,
  &ATK_Dragon_Pulse,
  &ATK_Swift,
  &ATK_Outrage,
  &ATK_Fly,
  NULL
};

static const attack* ESPURR_ATKS[] = {
  &ATK_Scratch,
  &ATK_Confusion,
  &ATK_Psyshock,
  &ATK_Psychic,
  &ATK_Energy_Ball,
  NULL
};

// FIXME male and female have different moves
static const attack* MEOWSTIC_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Confusion,
  &ATK_Thunderbolt,
  &ATK_Psychic,
  &ATK_Energy_Ball,
  NULL
};

static const attack* HONEDGE_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Tackle,
  &ATK_Iron_Head,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* DOUBLADE_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Psycho_Cut,
  &ATK_Iron_Head,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* AEGISLASH_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Psycho_Cut,
  &ATK_Flash_Cannon,
  &ATK_Shadow_Ball,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* SALANDIT_ATKS[] = {
  &ATK_Ember,
  &ATK_Poison_Jab,
  &ATK_Flamethrower,
  &ATK_Poison_Fang,
  &ATK_Dragon_Pulse,
  NULL
};

static const attack* SALAZZLE_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Incinerate,
  &ATK_Poison_Fang,
  &ATK_Dragon_Pulse,
  &ATK_Sludge_Wave,
  &ATK_Fire_Blast,
  NULL
};

static const attack* TOXAPEX_ATKS[] = {
  &ATK_Bite,
  &ATK_Poison_Jab,
  &ATK_Sludge_Wave,
  &ATK_Gunk_Shot,
  &ATK_Brine,
  NULL
};

static const attack* SEEDOT_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Bullet_Seed,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  &ATK_Foul_Play,
  NULL
};

static const attack* NUZLEAF_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Feint_Attack,
  &ATK_Leaf_Blade,
  &ATK_Grass_Knot,
  &ATK_Foul_Play,
  NULL
};

static const attack* SHIFTRY_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Feint_Attack,
  &ATK_Bullet_Seed,
  &ATK_Snarl,
  &ATK_Leaf_Blade,
  &ATK_Hurricane,
  &ATK_Foul_Play,
  &ATK_Leaf_Tornado,
  NULL
};

static const attack* PANSAGE_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Scratch,
  &ATK_Seed_Bomb,
  &ATK_Energy_Ball,
  &ATK_Crunch,
  NULL
};

static const attack* SIMISAGE_ATKS[] = {
  &ATK_Bite,
  &ATK_Vine_Whip,
  &ATK_Solar_Beam,
  &ATK_Grass_Knot,
  &ATK_Crunch,
  NULL
};

static const attack* PANSEAR_ATKS[] = {
  &ATK_Scratch,
  &ATK_Fire_Spin,
  &ATK_Flame_Charge,
  &ATK_Flame_Burst,
  &ATK_Crunch,
  NULL
};

static const attack* SIMISEAR_ATKS[] = {
  &ATK_Bite,
  &ATK_Fire_Spin,
  &ATK_Flamethrower,
  &ATK_Fire_Blast,
  &ATK_Crunch,
  NULL
};

static const attack* PANPOUR_ATKS[] = {
  &ATK_Scratch,
  &ATK_Water_Gun,
  &ATK_Water_Pulse,
  &ATK_Crunch,
  &ATK_Surf,
  NULL
};

static const attack* SIMIPOUR_ATKS[] = {
  &ATK_Bite,
  &ATK_Water_Gun,
  &ATK_Hydro_Pump,
  &ATK_Crunch,
  &ATK_Surf,
  NULL
};

static const attack* CHINCHOU_ATKS[] = {
  &ATK_Spark,
  &ATK_Bubble,
  &ATK_Bubble_Beam,
  &ATK_Thunderbolt,
  &ATK_Water_Pulse,
  NULL
};

static const attack* LANTURN_ATKS[] = {
  &ATK_Spark,
  &ATK_Water_Gun,
  &ATK_Charge_Beam,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  &ATK_Hydro_Pump,
  &ATK_Surf,
  NULL
};

static const attack* SWIRLIX_ATKS[] = {
  &ATK_Tackle,
  &ATK_Draining_Kiss,
  &ATK_Energy_Ball,
  NULL
};

static const attack* SLURPUFF_ATKS[] = {
  &ATK_Tackle,
  &ATK_Charm,
  &ATK_Fairy_Wind,
  &ATK_Flamethrower,
  &ATK_Draining_Kiss,
  &ATK_Play_Rough,
  &ATK_Energy_Ball,
  NULL
};

static const attack* SPRITZEE_ATKS[] = {
  &ATK_Charge_Beam,
  &ATK_Charm,
  &ATK_Thunderbolt,
  &ATK_Draining_Kiss,
  NULL
};

static const attack* AROMATISSE_ATKS[] = {
  &ATK_Charge_Beam,
  &ATK_Charm,
  &ATK_Thunderbolt,
  &ATK_Draining_Kiss,
  &ATK_Moonblast,
  &ATK_Psychic,
  NULL
};

static const attack* YAMPER_ATKS[] = {
  &ATK_Bite,
  &ATK_Spark,
  &ATK_Thunder,
  &ATK_Crunch,
  NULL
};

static const attack* BOLTUND_ATKS[] = {
  &ATK_Bite,
  &ATK_Spark,
  &ATK_Thunder,
  &ATK_Crunch,
  NULL
};

static const attack* SHINX_ATKS[] = {
  &ATK_Spark,
  &ATK_Tackle,
  &ATK_Discharge,
  &ATK_Thunderbolt,
  &ATK_Swift,
  NULL
};

static const attack* LUXIO_ATKS[] = {
  &ATK_Bite,
  &ATK_Spark,
  &ATK_Thunderbolt,
  &ATK_Wild_Charge,
  &ATK_Crunch,
  NULL
};

static const attack* LUXRAY_ATKS[] = {
  &ATK_Spark,
  &ATK_Snarl,
  &ATK_Hidden_Power,
  &ATK_Hyper_Beam,
  &ATK_Wild_Charge,
  &ATK_Crunch,
  &ATK_Psychic_Fangs,
  NULL
};

static const attack* TYNAMO_ATKS[] = {
  &ATK_Spark,
  &ATK_Tackle,
  &ATK_Struggle,
  NULL
};

static const attack* EELEKTRIK_ATKS[] = {
  &ATK_Spark,
  &ATK_Acid,
  &ATK_Discharge,
  &ATK_Thunderbolt,
  &ATK_Crunch,
  NULL
};

static const attack* EELEKTROSS_ATKS[] = {
  &ATK_Spark,
  &ATK_Acid,
  &ATK_Volt_Switch,
  &ATK_Thunderbolt,
  &ATK_Dragon_Claw,
  &ATK_Crunch,
  &ATK_Acid_Spray,
  &ATK_Liquidation,
  NULL
};

static const attack* A_GIRATINA_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Shadow_Claw,
  &ATK_Ancient_Power,
  &ATK_Shadow_Sneak,
  &ATK_Dragon_Claw,
  &ATK_Shadow_Force,
  NULL
};

static const attack* O_GIRATINA_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Dragon_Tail,
  &ATK_Ominous_Wind,
  &ATK_Shadow_Ball,
  &ATK_Dragon_Pulse,
  &ATK_Shadow_Force,
  NULL
};

static const attack* BLITZLE_ATKS[] = {
  &ATK_Spark,
  &ATK_Low_Kick,
  &ATK_Discharge,
  &ATK_Flame_Charge,
  &ATK_Wild_Charge,
  NULL
};

static const attack* ZEBSTRIKA_ATKS[] = {
  &ATK_Spark,
  &ATK_Quick_Attack,
  &ATK_Discharge,
  &ATK_Thunderbolt,
  &ATK_Flame_Charge,
  NULL
};

static const attack* TAILLOW_ATKS[] = {
  &ATK_Peck,
  &ATK_Quick_Attack,
  &ATK_Aerial_Ace,
  NULL
};

static const attack* SWELLOW_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Steel_Wing,
  &ATK_Aerial_Ace,
  &ATK_Brave_Bird,
  &ATK_Sky_Attack,
  NULL
};

static const attack* RUFFLET_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Peck,
  &ATK_Aerial_Ace,
  &ATK_Rock_Tomb,
  &ATK_Brave_Bird,
  &ATK_Fly,
  NULL
};

static const attack* BRAVIARY_ATKS[] = {
  &ATK_Steel_Wing,
  &ATK_Air_Slash,
  &ATK_Heat_Wave,
  &ATK_Rock_Slide,
  &ATK_Close_Combat,
  &ATK_Brave_Bird,
  &ATK_Fly,
  NULL
};

static const attack* H_BRAVIARY_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Air_Slash,
  &ATK_Ominous_Wind,
  &ATK_Dazzling_Gleam,
  &ATK_Psychic,
  &ATK_Brave_Bird,
  &ATK_Fly,
  NULL
};

static const attack* QUAXLY_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Water_Gun,
  &ATK_Aerial_Ace,
  &ATK_Aqua_Jet,
  &ATK_Liquidation,
  NULL
};

static const attack* QUAQUAVAL_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Water_Gun,
  &ATK_Aerial_Ace,
  &ATK_Aqua_Jet,
  &ATK_Aqua_Step,
  &ATK_Close_Combat,
  &ATK_Liquidation,
  &ATK_Hydro_Cannon,
  NULL
};

static const attack* KELDEO_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Poison_Jab,
  &ATK_Aqua_Jet,
  &ATK_XScissor,
  &ATK_Hydro_Pump,
  &ATK_Close_Combat,
  &ATK_Sacred_Sword,
  NULL
};

static const attack* MELOETTA_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Quick_Attack,
  &ATK_Hyper_Beam,
  &ATK_Ice_Punch,
  &ATK_Fire_Punch,
  &ATK_Close_Combat,
  NULL
};

static const attack* QUAXWELL_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Water_Gun,
  &ATK_Aerial_Ace,
  &ATK_Aqua_Jet,
  &ATK_Liquidation,
  NULL
};

static const attack* SKRELP_ATKS[] = {
  &ATK_Acid,
  &ATK_Water_Gun,
  &ATK_Aqua_Tail,
  &ATK_Twister,
  &ATK_Sludge_Bomb,
  &ATK_Water_Pulse,
  NULL
};

static const attack* DRAGALGE_ATKS[] = {
  &ATK_Acid,
  &ATK_Water_Gun,
  &ATK_Dragon_Tail,
  &ATK_Aqua_Tail,
  &ATK_Sludge_Bomb,
  &ATK_Gunk_Shot,
  &ATK_Hydro_Pump,
  &ATK_Focus_Blast,
  &ATK_Outrage,
  NULL
};

static const attack* MAREANIE_ATKS[] = {
  &ATK_Bite,
  &ATK_Poison_Sting,
  &ATK_Sludge_Wave,
  &ATK_Gunk_Shot,
  &ATK_Brine,
  NULL
};

static const attack* LUVDISC_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Splash,
  &ATK_Aqua_Jet,
  &ATK_Draining_Kiss,
  &ATK_Water_Pulse,
  NULL
};

static const attack* WISHIWASHI_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Iron_Tail,
  &ATK_Waterfall,
  &ATK_Aqua_Tail,
  &ATK_Brine,
  &ATK_Surf,
  NULL
};

static const attack* ALOMOMOLA_ATKS[] = {
  &ATK_Hidden_Power,
  &ATK_Waterfall,
  &ATK_Blizzard,
  &ATK_Hydro_Pump,
  &ATK_Psychic,
  NULL
};

static const attack* WINGULL_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Water_Gun,
  &ATK_Ice_Beam,
  &ATK_Water_Pulse,
  &ATK_Air_Cutter,
  NULL
};

static const attack* PELIPPER_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Water_Gun,
  &ATK_Blizzard,
  &ATK_Hydro_Pump,
  &ATK_Hurricane,
  &ATK_Weather_Ball_Water,
  NULL
};

static const attack* POPPLIO_ATKS[] = {
  &ATK_Pound,
  &ATK_Water_Gun,
  &ATK_Aqua_Jet,
  &ATK_Aqua_Tail,
  &ATK_Water_Pulse,
  NULL
};

static const attack* MINCCINO_ATKS[] = {
  &ATK_Pound,
  &ATK_Charm,
  &ATK_Aqua_Tail,
  &ATK_Thunderbolt,
  &ATK_Swift,
  NULL
};

static const attack* CINCCINO_ATKS[] = {
  &ATK_Pound,
  &ATK_Charm,
  &ATK_Hyper_Beam,
  &ATK_Aqua_Tail,
  &ATK_Thunderbolt,
  NULL
};

static const attack* BOUFFALANT_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Zen_Headbutt,
  &ATK_Megahorn,
  &ATK_Earthquake,
  &ATK_Stomp,
  &ATK_Skull_Bash,
  NULL
};

static const attack* BRIONNE_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Charm,
  &ATK_Aqua_Jet,
  &ATK_Disarming_Voice,
  &ATK_Water_Pulse,
  NULL
};

static const attack* PRIMARINA_ATKS[] = {
  &ATK_Waterfall,
  &ATK_Charm,
  &ATK_Disarming_Voice,
  &ATK_Moonblast,
  &ATK_Hydro_Pump,
  &ATK_Psychic,
  &ATK_Hydro_Cannon,
  &ATK_Sparkling_Aria,
  NULL
};

static const attack* AZURILL_ATKS[] = {
  &ATK_Splash,
  &ATK_Bubble,
  &ATK_Ice_Beam,
  &ATK_Bubble_Beam,
  &ATK_Body_Slam,
  NULL
};

static const attack* MARILL_ATKS[] = {
  &ATK_Tackle,
  &ATK_Bubble,
  &ATK_Bubble_Beam,
  &ATK_Aqua_Tail,
  &ATK_Body_Slam,
  NULL
};

static const attack* AZUMARILL_ATKS[] = {
  &ATK_Bubble,
  &ATK_Rock_Smash,
  &ATK_Ice_Beam,
  &ATK_Play_Rough,
  &ATK_Hydro_Pump,
  NULL
};

static const attack* CLAMPERL_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Ice_Beam,
  &ATK_Water_Pulse,
  &ATK_Body_Slam,
  NULL
};

static const attack* HUNTAIL_ATKS[] = {
  &ATK_Bite,
  &ATK_Water_Gun,
  &ATK_Ice_Beam,
  &ATK_Aqua_Tail,
  &ATK_Crunch,
  NULL
};

static const attack* GOREBYSS_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Confusion,
  &ATK_Draining_Kiss,
  &ATK_Water_Pulse,
  &ATK_Psychic,
  NULL
};

static const attack* BRUXISH_ATKS[] = {
  &ATK_Bite,
  &ATK_Water_Gun,
  &ATK_Confusion,
  &ATK_Aqua_Tail,
  &ATK_Psyshock,
  &ATK_Crunch,
  &ATK_Psychic_Fangs,
  NULL
};

static const attack* PANCHAM_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Tackle,
  &ATK_Low_Sweep,
  &ATK_Body_Slam,
  &ATK_Crunch,
  NULL
};

static const attack* PANGORO_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Karate_Chop,
  &ATK_Bullet_Punch,
  &ATK_Snarl,
  &ATK_Night_Slash,
  &ATK_Rock_Slide,
  &ATK_Iron_Head,
  &ATK_Close_Combat,
  NULL
};

static const attack* ZANGOOSE_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Shadow_Claw,
  &ATK_Dig,
  &ATK_Night_Slash,
  &ATK_Close_Combat,
  NULL
};

static const attack* VIRIZION_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Zen_Headbutt,
  &ATK_Double_Kick,
  &ATK_Stone_Edge,
  &ATK_Leaf_Blade,
  &ATK_Close_Combat,
  &ATK_Sacred_Sword,
  NULL
};

static const attack* FARFETCHD_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Cut,
  &ATK_Air_Slash,
  &ATK_Aerial_Ace,
  &ATK_Leaf_Blade,
  &ATK_Air_Cutter,
  NULL
};

static const attack* SIRFETCHD_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Counter,
  &ATK_Night_Slash,
  &ATK_Leaf_Blade,
  &ATK_Close_Combat,
  &ATK_Brave_Bird,
  NULL
};

static const attack* G_FARFETCHD_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Rock_Smash,
  &ATK_Leaf_Blade,
  &ATK_Brick_Break,
  &ATK_Brave_Bird,
  NULL
};

static const attack* GIRAFARIG_ATKS[] = {
  &ATK_Tackle,
  &ATK_Confusion,
  &ATK_Double_Kick,
  &ATK_Thunderbolt,
  &ATK_Psychic,
  &ATK_Mirror_Coat,
  &ATK_Psychic_Fangs,
  &ATK_Trailblaze,
  NULL
};

static const attack* C_TAUROS_ATKS[] = {
  &ATK_Tackle,
  &ATK_Zen_Headbutt,
  &ATK_Double_Kick,
  &ATK_Earthquake,
  &ATK_Iron_Head,
  &ATK_Close_Combat,
  &ATK_Trailblaze,
  NULL
};

static const attack* B_TAUROS_ATKS[] = {
  &ATK_Tackle,
  &ATK_Zen_Headbutt,
  &ATK_Double_Kick,
  &ATK_Earthquake,
  &ATK_Iron_Head,
  &ATK_Flame_Charge,
  &ATK_Trailblaze,
  NULL
};

static const attack* A_TAUROS_ATKS[] = {
  &ATK_Tackle,
  &ATK_Zen_Headbutt,
  &ATK_Double_Kick,
  &ATK_Earthquake,
  &ATK_Aqua_Jet,
  &ATK_Iron_Head,
  &ATK_Horn_Attack,
  &ATK_Trailblaze,
  NULL
};

static const attack* TAUROS_ATKS[] = {
  &ATK_Tackle,
  &ATK_Zen_Headbutt,
  &ATK_Earthquake,
  &ATK_Iron_Head,
  &ATK_Horn_Attack,
  &ATK_Trailblaze,
  NULL
};

static const attack* GOLDEEN_ATKS[] = {
  &ATK_Peck,
  &ATK_Mud_Shot,
  &ATK_Aqua_Tail,
  &ATK_Water_Pulse,
  &ATK_Horn_Attack,
  NULL
};

static const attack* SEAKING_ATKS[] = {
  &ATK_Peck,
  &ATK_Poison_Jab,
  &ATK_Waterfall,
  &ATK_Megahorn,
  &ATK_Ice_Beam,
  &ATK_Drill_Run,
  &ATK_Water_Pulse,
  &ATK_Icy_Wind,
  NULL
};

static const attack* RELICANTH_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Zen_Headbutt,
  &ATK_Aqua_Tail,
  &ATK_Ancient_Power,
  &ATK_Hydro_Pump,
  NULL
};

static const attack* OSHAWOTT_ATKS[] = {
  &ATK_Tackle,
  &ATK_Water_Gun,
  &ATK_Night_Slash,
  &ATK_Aqua_Tail,
  &ATK_Water_Pulse,
  NULL
};

static const attack* DEWOTT_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Water_Gun,
  &ATK_Aqua_Tail,
  &ATK_XScissor,
  &ATK_Water_Pulse,
  NULL
};

static const attack* SAMUROTT_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Waterfall,
  &ATK_Megahorn,
  &ATK_Blizzard,
  &ATK_Hydro_Pump,
  &ATK_Hydro_Cannon,
  &ATK_Razor_Shell,
  &ATK_Liquidation,
  NULL
};

static const attack* H_SAMUROTT_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Snarl,
  &ATK_Waterfall,
  &ATK_Dark_Pulse,
  &ATK_XScissor,
  &ATK_Icy_Wind,
  &ATK_Razor_Shell,
  NULL
};

static const attack* GLIGAR_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Wing_Attack,
  &ATK_Dig,
  &ATK_Aerial_Ace,
  &ATK_Night_Slash,
  NULL
};

static const attack* GLISCOR_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Wing_Attack,
  &ATK_Sand_Attack,
  &ATK_Earthquake,
  &ATK_Aerial_Ace,
  &ATK_Night_Slash,
  &ATK_Sand_Tomb,
  NULL
};

static const attack* TOTODILE_ATKS[] = {
  &ATK_Scratch,
  &ATK_Water_Gun,
  &ATK_Aqua_Jet,
  &ATK_Water_Pulse,
  &ATK_Crunch,
  NULL
};

static const attack* CROCONAW_ATKS[] = {
  &ATK_Scratch,
  &ATK_Water_Gun,
  &ATK_Ice_Punch,
  &ATK_Water_Pulse,
  &ATK_Crunch,
  NULL
};

static const attack* FERALIGATR_ATKS[] = {
  &ATK_Bite,
  &ATK_Shadow_Claw,
  &ATK_Water_Gun,
  &ATK_Waterfall,
  &ATK_Ice_Fang,
  &ATK_Ice_Beam,
  &ATK_Hydro_Pump,
  &ATK_Crunch,
  &ATK_Hydro_Cannon,
  NULL
};

static const attack* SOBBLE_ATKS[] = {
  &ATK_Pound,
  &ATK_Water_Gun,
  &ATK_Water_Pulse,
  &ATK_Surf,
  NULL
};

static const attack* DRIZZILE_ATKS[] = {
  &ATK_Pound,
  &ATK_Water_Gun,
  &ATK_Water_Pulse,
  &ATK_Surf,
  NULL
};

static const attack* INTELEON_ATKS[] = {
  &ATK_Pound,
  &ATK_Water_Gun,
  &ATK_Shadow_Ball,
  &ATK_Water_Pulse,
  &ATK_Surf,
  NULL
};

static const attack* HORSEA_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Bubble,
  &ATK_Flash_Cannon,
  &ATK_Bubble_Beam,
  &ATK_Dragon_Pulse,
  NULL
};

static const attack* SEADRA_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Water_Gun,
  &ATK_Blizzard,
  &ATK_Dragon_Pulse,
  &ATK_Hydro_Pump,
  &ATK_Aurora_Beam,
  NULL
};

static const attack* KINGDRA_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Water_Gun,
  &ATK_Waterfall,
  &ATK_Blizzard,
  &ATK_Hydro_Pump,
  &ATK_Outrage,
  &ATK_Octazooka,
  NULL
};

static const attack* STARYU_ATKS[] = {
  &ATK_Tackle,
  &ATK_Water_Gun,
  &ATK_Bubble_Beam,
  &ATK_Power_Gem,
  &ATK_Swift,
  NULL
};

static const attack* STARMIE_ATKS[] = {
  &ATK_Tackle,
  &ATK_Water_Gun,
  &ATK_Hidden_Power,
  &ATK_Psywave,
  &ATK_Psybeam,
  &ATK_Ice_Beam,
  &ATK_Power_Gem,
  &ATK_Thunder,
  &ATK_Hydro_Pump,
  &ATK_Psychic,
  &ATK_Surf,
  NULL
};

static const attack* DHELMISE_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Astonish,
  &ATK_Metal_Sound,
  &ATK_Wrap,
  &ATK_Shadow_Ball,
  &ATK_Power_Whip,
  &ATK_Heavy_Slam,
  NULL
};

static const attack* ROTOM_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Astonish,
  &ATK_Ominous_Wind,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  NULL
};

static const attack* FAN_ROTOM_ATKS[] = {
  &ATK_Air_Slash,
  &ATK_Astonish,
  &ATK_Ominous_Wind,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  NULL
};

static const attack* FROST_ROTOM_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Astonish,
  &ATK_Blizzard,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  NULL
};

static const attack* MOW_ROTOM_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Astonish,
  &ATK_Ominous_Wind,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  NULL
};

static const attack* WASH_ROTOM_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Astonish,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  &ATK_Hydro_Pump,
  NULL
};

static const attack* HEAT_ROTOM_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Astonish,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  &ATK_Overheat,
  NULL
};

static const attack* ELGYEM_ATKS[] = {
  &ATK_Confusion,
  &ATK_Astonish,
  &ATK_Dark_Pulse,
  &ATK_Psybeam,
  &ATK_Psychic,
  NULL
};

static const attack* BEHEEYEM_ATKS[] = {
  &ATK_Confusion,
  &ATK_Astonish,
  &ATK_Dark_Pulse,
  &ATK_Rock_Slide,
  &ATK_Psychic,
  NULL
};

static const attack* SEEL_ATKS[] = {
  &ATK_Lick,
  &ATK_Ice_Shard,
  &ATK_Water_Gun,
  &ATK_Aqua_Jet,
  &ATK_Aqua_Tail,
  &ATK_Icy_Wind,
  &ATK_Aurora_Beam,
  NULL
};

static const attack* DEWGONG_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Frost_Breath,
  &ATK_Iron_Tail,
  &ATK_Blizzard,
  &ATK_Drill_Run,
  &ATK_Aqua_Jet,
  &ATK_Water_Pulse,
  &ATK_Icy_Wind,
  &ATK_Aurora_Beam,
  &ATK_Liquidation,
  NULL
};

static const attack* KLEAVOR_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Quick_Attack,
  &ATK_Air_Slash,
  &ATK_Stone_Edge,
  &ATK_Rock_Slide,
  &ATK_XScissor,
  &ATK_Trailblaze,
  NULL
};

static const attack* DURANT_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Metal_Claw,
  &ATK_Stone_Edge,
  &ATK_Iron_Head,
  &ATK_XScissor,
  NULL
};

static const attack* REGIDRAGO_ATKS[] = {
  &ATK_Bite,
  &ATK_Dragon_Breath,
  &ATK_Hyper_Beam,
  &ATK_Dragon_Pulse,
  &ATK_Dragon_Energy,
  &ATK_Outrage,
  &ATK_Breaking_Swipe,
  NULL
};

static const attack* GENESECT_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Metal_Claw,
  &ATK_Hyper_Beam,
  &ATK_Magnet_Bomb,
  &ATK_XScissor,
  &ATK_Techno_Blast,
  NULL
};

static const attack* FERROSEED_ATKS[] = {
  &ATK_Tackle,
  &ATK_Metal_Claw,
  &ATK_Flash_Cannon,
  &ATK_Iron_Head,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* FERROTHORN_ATKS[] = {
  &ATK_Metal_Claw,
  &ATK_Bullet_Seed,
  &ATK_Flash_Cannon,
  &ATK_Thunder,
  &ATK_Power_Whip,
  &ATK_Acid_Spray,
  &ATK_Mirror_Shot,
  NULL
};

static const attack* DUCKLETT_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Water_Gun,
  &ATK_Aerial_Ace,
  &ATK_Bubble_Beam,
  &ATK_Brave_Bird,
  NULL
};

static const attack* SWANNA_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Air_Slash,
  &ATK_Ice_Beam,
  &ATK_Bubble_Beam,
  &ATK_Hurricane,
  &ATK_Fly,
  NULL
};

static const attack* LOTAD_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Water_Gun,
  &ATK_Bubble_Beam,
  &ATK_Scald,
  &ATK_Energy_Ball,
  NULL
};

static const attack* LOMBRE_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Bubble,
  &ATK_Ice_Beam,
  &ATK_Bubble_Beam,
  &ATK_Scald,
  &ATK_Grass_Knot,
  NULL
};

static const attack* LUDICOLO_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Bubble,
  &ATK_Ice_Beam,
  &ATK_Blizzard,
  &ATK_Scald,
  &ATK_Hydro_Pump,
  &ATK_Solar_Beam,
  &ATK_Energy_Ball,
  &ATK_Leaf_Storm,
  NULL
};

static const attack* KARTANA_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Air_Slash,
  &ATK_Aerial_Ace,
  &ATK_Night_Slash,
  &ATK_XScissor,
  &ATK_Leaf_Blade,
  NULL
};

static const attack* SPINDA_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Psycho_Cut,
  &ATK_Dig,
  &ATK_Rock_Tomb,
  &ATK_Icy_Wind,
  NULL
};

static const attack* NECROZMA_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Psycho_Cut,
  &ATK_Metal_Claw,
  &ATK_Dark_Pulse,
  &ATK_Iron_Head,
  &ATK_Future_Sight,
  &ATK_Outrage,
  NULL
};

static const attack* DUSK_NECROZMA_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Psycho_Cut,
  &ATK_Metal_Claw,
  &ATK_Dark_Pulse,
  &ATK_Iron_Head,
  &ATK_Future_Sight,
  &ATK_Outrage,
  &ATK_Sunsteel_Strike,
  NULL
};

static const attack* DAWN_NECROZMA_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Psycho_Cut,
  &ATK_Metal_Claw,
  &ATK_Dark_Pulse,
  &ATK_Iron_Head,
  &ATK_Future_Sight,
  &ATK_Outrage,
  &ATK_Moongeist_Beam,
  NULL
};

static const attack* CELESTEELA_ATKS[] = {
  &ATK_Air_Slash,
  &ATK_Smack_Down,
  &ATK_Iron_Head,
  &ATK_Bulldoze,
  &ATK_Body_Slam,
  &ATK_Heavy_Slam,
  NULL
};

static const attack* BUNNELBY_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Mud_Slap,
  &ATK_Dig,
  &ATK_Earthquake,
  &ATK_Bulldoze,
  NULL
};

static const attack* DIGGERSBY_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Quick_Attack,
  &ATK_Hyper_Beam,
  &ATK_Dig,
  &ATK_Earthquake,
  &ATK_Fire_Punch,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* BINACLE_ATKS[] = {
  &ATK_Scratch,
  &ATK_Mud_Slap,
  &ATK_Dig,
  &ATK_Cross_Chop,
  &ATK_Ancient_Power,
  NULL
};

static const attack* BARBARACLE_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Water_Gun,
  &ATK_Mud_Slap,
  &ATK_Cross_Chop,
  &ATK_Stone_Edge,
  &ATK_Grass_Knot,
  &ATK_Skull_Bash,
  &ATK_Razor_Shell,
  NULL
};

static const attack* SANDILE_ATKS[] = {
  &ATK_Bite,
  &ATK_Mud_Slap,
  &ATK_Dig,
  &ATK_Bulldoze,
  &ATK_Crunch,
  NULL
};

static const attack* KROKOROK_ATKS[] = {
  &ATK_Bite,
  &ATK_Mud_Slap,
  &ATK_Earthquake,
  &ATK_Bulldoze,
  &ATK_Crunch,
  NULL
};

static const attack* KROOKODILE_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Snarl,
  &ATK_Earthquake,
  &ATK_Brick_Break,
  &ATK_Outrage,
  &ATK_Crunch,
  NULL
};

static const attack* SENTRET_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Scratch,
  &ATK_Dig,
  &ATK_Brick_Break,
  &ATK_Grass_Knot,
  NULL
};

static const attack* FURRET_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Quick_Attack,
  &ATK_Hyper_Beam,
  &ATK_Dig,
  &ATK_Brick_Break,
  &ATK_Swift,
  &ATK_Trailblaze,
  NULL
};

static const attack* TINKATINK_ATKS[] = {
  &ATK_Rock_Smash,
  &ATK_Fairy_Wind,
  &ATK_Flash_Cannon,
  &ATK_Play_Rough,
  NULL
};

static const attack* TINKATUFF_ATKS[] = {
  &ATK_Rock_Smash,
  &ATK_Fairy_Wind,
  &ATK_Flash_Cannon,
  &ATK_Play_Rough,
  NULL
};

static const attack* TINKATON_ATKS[] = {
  &ATK_Rock_Smash,
  &ATK_Fairy_Wind,
  &ATK_Flash_Cannon,
  &ATK_Play_Rough,
  &ATK_Bulldoze,
  &ATK_Heavy_Slam,
  NULL
};

static const attack* CHINGLING_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Astonish,
  &ATK_Wrap,
  &ATK_Psyshock,
  &ATK_Shadow_Ball,
  NULL
};

static const attack* CHIMECHO_ATKS[] = {
  &ATK_Astonish,
  &ATK_Extrasensory,
  &ATK_Psyshock,
  &ATK_Shadow_Ball,
  &ATK_Energy_Ball,
  NULL
};

static const attack* I_ENAMORUS_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Astonish,
  &ATK_Fairy_Wind,
  &ATK_Dazzling_Gleam,
  &ATK_Grass_Knot,
  &ATK_Fly,
  NULL
};

static const attack* EXEGGCUTE_ATKS[] = {
  &ATK_Confusion,
  &ATK_Bullet_Seed,
  &ATK_Seed_Bomb,
  &ATK_Ancient_Power,
  &ATK_Psychic,
  NULL
};

static const attack* EXEGGUTOR_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Confusion,
  &ATK_Bullet_Seed,
  &ATK_Extrasensory,
  &ATK_Seed_Bomb,
  &ATK_Psychic,
  &ATK_Solar_Beam,
  NULL
};

static const attack* A_EXEGGUTOR_ATKS[] = {
  &ATK_Dragon_Tail,
  &ATK_Bullet_Seed,
  &ATK_Seed_Bomb,
  &ATK_Dragon_Pulse,
  &ATK_Solar_Beam,
  &ATK_Draco_Meteor,
  NULL
};

static const attack* PALKIA_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Dragon_Tail,
  &ATK_Aqua_Tail,
  &ATK_Fire_Blast,
  &ATK_Hydro_Pump,
  &ATK_Draco_Meteor,
  NULL
};

static const attack* O_PALKIA_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Dragon_Tail,
  &ATK_Aqua_Tail,
  &ATK_Fire_Blast,
  &ATK_Hydro_Pump,
  &ATK_Draco_Meteor,
  &ATK_Spacial_Rend,
  NULL
};

static const attack* SIGILYPH_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Air_Slash,
  &ATK_Psybeam,
  &ATK_Ancient_Power,
  &ATK_Signal_Beam,
  &ATK_Air_Cutter,
  NULL
};

static const attack* BALTOY_ATKS[] = {
  &ATK_Confusion,
  &ATK_Extrasensory,
  &ATK_Dig,
  &ATK_Psybeam,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* CLAYDOL_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Confusion,
  &ATK_Extrasensory,
  &ATK_Earthquake,
  &ATK_Ice_Beam,
  &ATK_Rock_Tomb,
  &ATK_Shadow_Ball,
  &ATK_Psychic,
  &ATK_Gyro_Ball,
  &ATK_Earth_Power,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* UXIE_ATKS[] = {
  &ATK_Confusion,
  &ATK_Extrasensory,
  &ATK_Thunder,
  &ATK_Swift,
  &ATK_Future_Sight,
  NULL
};

static const attack* MESPRIT_ATKS[] = {
  &ATK_Confusion,
  &ATK_Extrasensory,
  &ATK_Blizzard,
  &ATK_Swift,
  &ATK_Future_Sight,
  NULL
};

static const attack* AZELF_ATKS[] = {
  &ATK_Confusion,
  &ATK_Extrasensory,
  &ATK_Fire_Blast,
  &ATK_Swift,
  &ATK_Future_Sight,
  NULL
};

static const attack* TYRUNT_ATKS[] = {
  &ATK_Tackle,
  &ATK_Dragon_Tail,
  &ATK_Ancient_Power,
  &ATK_Dragon_Claw,
  &ATK_Stomp,
  NULL
};

static const attack* TYRANTRUM_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Dragon_Tail,
  &ATK_Charm,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Outrage,
  &ATK_Crunch,
  &ATK_Meteor_Beam,
  NULL
};

static const attack* INKAY_ATKS[] = {
  &ATK_Peck,
  &ATK_Tackle,
  &ATK_Psywave,
  &ATK_Psybeam,
  &ATK_Night_Slash,
  NULL
};

static const attack* MALAMAR_ATKS[] = {
  &ATK_Peck,
  &ATK_Psycho_Cut,
  &ATK_Psywave,
  &ATK_Hyper_Beam,
  &ATK_Psybeam,
  &ATK_Foul_Play,
  &ATK_Superpower,
  NULL
};

static const attack* LUNATONE_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Confusion,
  &ATK_Psywave,
  &ATK_Rock_Slide,
  &ATK_Moonblast,
  &ATK_Psychic,
  NULL
};

static const attack* SOLROCK_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Confusion,
  &ATK_Psywave,
  &ATK_Rock_Slide,
  &ATK_Psychic,
  &ATK_Solar_Beam,
  NULL
};

static const attack* CRANIDOS_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Take_Down,
  &ATK_Ancient_Power,
  &ATK_Rock_Tomb,
  &ATK_Bulldoze,
  NULL
};

static const attack* RAMPARDOS_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Smack_Down,
  &ATK_Flamethrower,
  &ATK_Rock_Slide,
  &ATK_Outrage,
  NULL
};

static const attack* ROGGENROLA_ATKS[] = {
  &ATK_Tackle,
  &ATK_Smack_Down,
  &ATK_Stone_Edge,
  &ATK_Bulldoze,
  &ATK_Rock_Blast,
  NULL
};

static const attack* BOLDORE_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Smack_Down,
  &ATK_Stone_Edge,
  &ATK_Rock_Slide,
  &ATK_Bulldoze,
  NULL
};

static const attack* GIGALITH_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Smack_Down,
  &ATK_Rock_Slide,
  &ATK_Solar_Beam,
  &ATK_Heavy_Slam,
  &ATK_Superpower,
  &ATK_Meteor_Beam,
  NULL
};

static const attack* NOSEPASS_ATKS[] = {
  &ATK_Spark,
  &ATK_Rock_Throw,
  &ATK_Rock_Slide,
  &ATK_Thunderbolt,
  &ATK_Rock_Blast,
  NULL
};

static const attack* PROBOPASS_ATKS[] = {
  &ATK_Spark,
  &ATK_Rock_Throw,
  &ATK_Rock_Slide,
  &ATK_Magnet_Bomb,
  &ATK_Thunderbolt,
  &ATK_Zap_Cannon,
  NULL
};

static const attack* STAKATAKA_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Take_Down,
  &ATK_Stone_Edge,
  &ATK_Flash_Cannon,
  &ATK_Bulldoze,
  NULL
};

static const attack* BONSLY_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Counter,
  &ATK_Earthquake,
  &ATK_Rock_Tomb,
  &ATK_Rock_Slide,
  NULL
};

static const attack* SUDOWOODO_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Counter,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Rock_Tomb,
  &ATK_Rock_Slide,
  &ATK_Meteor_Beam,
  &ATK_Trailblaze,
  NULL
};

static const attack* G_YAMASK_ATKS[] = {
  &ATK_Astonish,
  &ATK_Rock_Tomb,
  &ATK_Night_Shade,
  NULL
};

static const attack* CHATOT_ATKS[] = {
  &ATK_Peck,
  &ATK_Steel_Wing,
  &ATK_Heat_Wave,
  &ATK_Sky_Attack,
  &ATK_Night_Shade,
  NULL
};

static const attack* FLETCHLING_ATKS[] = {
  &ATK_Peck,
  &ATK_Quick_Attack,
  &ATK_Heat_Wave,
  &ATK_Aerial_Ace,
  &ATK_Swift,
  &ATK_Fly,
  NULL
};

static const attack* FLETCHINDER_ATKS[] = {
  &ATK_Ember,
  &ATK_Peck,
  &ATK_Steel_Wing,
  &ATK_Heat_Wave,
  &ATK_Aerial_Ace,
  &ATK_Flame_Charge,
  &ATK_Fly,
  NULL
};

static const attack* TALONFLAME_ATKS[] = {
  &ATK_Peck,
  &ATK_Steel_Wing,
  &ATK_Fire_Spin,
  &ATK_Incinerate,
  &ATK_Flame_Charge,
  &ATK_Fire_Blast,
  &ATK_Hurricane,
  &ATK_Brave_Bird,
  &ATK_Fly,
  NULL
};

static const attack* SOLOSIS_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Hidden_Power,
  &ATK_Psyshock,
  &ATK_Thunder,
  &ATK_Night_Shade,
  NULL
};

static const attack* DUOSION_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Hidden_Power,
  &ATK_Psyshock,
  &ATK_Thunder,
  &ATK_Night_Shade,
  NULL
};

static const attack* REUNICLUS_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Hidden_Power,
  &ATK_Psyshock,
  &ATK_Shadow_Ball,
  &ATK_Thunder,
  &ATK_Future_Sight,
  NULL
};

static const attack* RUNERIGUS_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Astonish,
  &ATK_Rock_Smash,
  &ATK_Shadow_Ball,
  &ATK_Sand_Tomb,
  &ATK_Brutal_Swing,
  NULL
};

static const attack* ZORUA_ATKS[] = {
  &ATK_Scratch,
  &ATK_Feint_Attack,
  &ATK_Dark_Pulse,
  &ATK_Aerial_Ace,
  &ATK_Night_Shade,
  &ATK_Foul_Play,
  NULL
};

static const attack* HZORUA_ATKS[] = {
  &ATK_Scratch,
  &ATK_Snarl,
  &ATK_Dark_Pulse,
  &ATK_Shadow_Ball,
  &ATK_Night_Shade,
  &ATK_Foul_Play,
  NULL
};

static const attack* ZOROARK_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Snarl,
  &ATK_Flamethrower,
  &ATK_Sludge_Bomb,
  &ATK_Night_Shade,
  &ATK_Foul_Play,
  NULL
};

static const attack* HZOROARK_ATKS[] = {
  &ATK_Scratch,
  &ATK_Snarl,
  &ATK_Dark_Pulse,
  &ATK_Shadow_Ball,
  &ATK_Night_Shade,
  &ATK_Foul_Play,
  NULL
};

static const attack* STUNKY_ATKS[] = {
  &ATK_Bite,
  &ATK_Scratch,
  &ATK_Flamethrower,
  &ATK_Sludge_Bomb,
  &ATK_Crunch,
  &ATK_Trailblaze,
  NULL
};

static const attack* SKUNTANK_ATKS[] = {
  &ATK_Bite,
  &ATK_Poison_Jab,
  &ATK_Flamethrower,
  &ATK_Sludge_Bomb,
  &ATK_Crunch,
  &ATK_Trailblaze,
  NULL
};

static const attack* MAKUHITA_ATKS[] = {
  &ATK_Tackle,
  &ATK_Rock_Smash,
  &ATK_Cross_Chop,
  &ATK_Low_Sweep,
  &ATK_Heavy_Slam,
  &ATK_Upper_Hand,
  NULL
};

static const attack* HARIYAMA_ATKS[] = {
  &ATK_Bullet_Punch,
  &ATK_Counter,
  &ATK_Force_Palm,
  &ATK_Close_Combat,
  &ATK_Dynamic_Punch,
  &ATK_Heavy_Slam,
  &ATK_Superpower,
  &ATK_Upper_Hand,
  NULL
};

static const attack* SCRAGGY_ATKS[] = {
  &ATK_Feint_Attack,
  &ATK_Rock_Smash,
  &ATK_Thunder_Punch,
  &ATK_Brick_Break,
  &ATK_Foul_Play,
  &ATK_Acid_Spray,
  &ATK_Upper_Hand,
  NULL
};

static const attack* SCRAFTY_ATKS[] = {
  &ATK_Counter,
  &ATK_Snarl,
  &ATK_Thunder_Punch,
  &ATK_Foul_Play,
  &ATK_Acid_Spray,
  &ATK_Power_Up_Punch,
  &ATK_Upper_Hand,
  NULL
};

static const attack* DARKRAI_ATKS[] = {
  &ATK_Feint_Attack,
  &ATK_Snarl,
  &ATK_Dark_Pulse,
  &ATK_Shadow_Ball,
  &ATK_Sludge_Bomb,
  &ATK_Focus_Blast,
  NULL
};

static const attack* NICKIT_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Quick_Attack,
  &ATK_Snarl,
  &ATK_Night_Slash,
  &ATK_Play_Rough,
  NULL
};

static const attack* THIEVUL_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Quick_Attack,
  &ATK_Snarl,
  &ATK_Night_Slash,
  &ATK_Play_Rough,
  NULL
};

static const attack* PURRLOIN_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Scratch,
  &ATK_Dark_Pulse,
  &ATK_Night_Slash,
  &ATK_Play_Rough,
  NULL
};

static const attack* LIEPARD_ATKS[] = {
  &ATK_Snarl,
  &ATK_Charm,
  &ATK_Dark_Pulse,
  &ATK_Play_Rough,
  &ATK_Gunk_Shot,
  &ATK_Payback,
  NULL
};

static const attack* VANILLITE_ATKS[] = {
  &ATK_Powder_Snow,
  &ATK_Astonish,
  &ATK_Ice_Beam,
  &ATK_Signal_Beam,
  &ATK_Icy_Wind,
  NULL
};

static const attack* VANILLISH_ATKS[] = {
  &ATK_Frost_Breath,
  &ATK_Astonish,
  &ATK_Ice_Beam,
  &ATK_Signal_Beam,
  &ATK_Icy_Wind,
  NULL
};

static const attack* VANILLUXE_ATKS[] = {
  &ATK_Frost_Breath,
  &ATK_Astonish,
  &ATK_Flash_Cannon,
  &ATK_Blizzard,
  &ATK_Signal_Beam,
  &ATK_Avalanche,
  NULL
};

static const attack* CUBCHOO_ATKS[] = {
  &ATK_Powder_Snow,
  &ATK_Charm,
  &ATK_Ice_Punch,
  &ATK_Play_Rough,
  &ATK_Icy_Wind,
  NULL
};

static const attack* BEARTIC_ATKS[] = {
  &ATK_Powder_Snow,
  &ATK_Charm,
  &ATK_Ice_Punch,
  &ATK_Play_Rough,
  &ATK_Surf,
  &ATK_Liquidation,
  NULL
};

static const attack* AXEW_ATKS[] = {
  &ATK_Scratch,
  &ATK_Iron_Tail,
  &ATK_Aqua_Tail,
  &ATK_Dragon_Pulse,
  &ATK_Dragon_Claw,
  NULL
};

static const attack* FRAXURE_ATKS[] = {
  &ATK_Dragon_Tail,
  &ATK_Iron_Tail,
  &ATK_Night_Slash,
  &ATK_Aqua_Tail,
  &ATK_Dragon_Claw,
  NULL
};

static const attack* HAXORUS_ATKS[] = {
  &ATK_Counter,
  &ATK_Dragon_Tail,
  &ATK_Earthquake,
  &ATK_Night_Slash,
  &ATK_Dragon_Claw,
  &ATK_Surf,
  &ATK_Breaking_Swipe,
  NULL
};

static const attack* DRUDDIGON_ATKS[] = {
  &ATK_Bite,
  &ATK_Dragon_Tail,
  &ATK_Hyper_Beam,
  &ATK_Night_Slash,
  &ATK_Dragon_Claw,
  NULL
};

static const attack* TYMPOLE_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Bubble,
  &ATK_Sludge_Wave,
  &ATK_Mud_Bomb,
  &ATK_Water_Pulse,
  NULL
};

static const attack* PALPITOAD_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Bubble,
  &ATK_Sludge_Wave,
  &ATK_Water_Pulse,
  &ATK_Earth_Power,
  NULL
};

static const attack* SEISMITOAD_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Bubble,
  &ATK_Sludge_Bomb,
  &ATK_Earth_Power,
  &ATK_Muddy_Water,
  NULL
};

static const attack* BASCULIN_ATKS[] = {
  &ATK_Tackle,
  &ATK_Water_Gun,
  &ATK_Aqua_Jet,
  &ATK_Aqua_Tail,
  &ATK_Muddy_Water,
  NULL
};

static const attack* BUIZEL_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Water_Gun,
  &ATK_Aqua_Jet,
  &ATK_Water_Pulse,
  &ATK_Swift,
  NULL
};

static const attack* FLOATZEL_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Waterfall,
  &ATK_Aqua_Jet,
  &ATK_Hydro_Pump,
  &ATK_Swift,
  &ATK_Liquidation,
  NULL
};

static const attack* GOOMY_ATKS[] = {
  &ATK_Tackle,
  &ATK_Water_Gun,
  &ATK_Dragon_Pulse,
  &ATK_Sludge_Wave,
  &ATK_Muddy_Water,
  NULL
};

static const attack* SLIGGOO_ATKS[] = {
  &ATK_Tackle,
  &ATK_Water_Gun,
  &ATK_Dragon_Pulse,
  &ATK_Sludge_Wave,
  &ATK_Water_Pulse,
  &ATK_Muddy_Water,
  NULL
};

static const attack* GOODRA_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Water_Gun,
  &ATK_Aqua_Tail,
  &ATK_Thunder_Punch,
  &ATK_Sludge_Wave,
  &ATK_Power_Whip,
  &ATK_Draco_Meteor,
  &ATK_Muddy_Water,
  NULL
};

static const attack* SMOOCHUM_ATKS[] = {
  &ATK_Frost_Breath,
  &ATK_Pound,
  &ATK_Powder_Snow,
  &ATK_Ice_Punch,
  &ATK_Ice_Beam,
  &ATK_Psyshock,
  NULL
};

static const attack* JYNX_ATKS[] = {
  &ATK_Frost_Breath,
  &ATK_Pound,
  &ATK_Confusion,
  &ATK_Ice_Punch,
  &ATK_Psyshock,
  &ATK_Draining_Kiss,
  &ATK_Focus_Blast,
  &ATK_Avalanche,
  NULL
};

static const attack* SNEASEL_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Feint_Attack,
  &ATK_Ice_Punch,
  &ATK_Avalanche,
  &ATK_Foul_Play,
  &ATK_Triple_Axel,
  NULL
};

static const attack* WEAVILE_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Feint_Attack,
  &ATK_Snarl,
  &ATK_Focus_Blast,
  &ATK_Avalanche,
  &ATK_Foul_Play,
  &ATK_Triple_Axel,
  NULL
};

static const attack* H_SNEASEL_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Rock_Smash,
  &ATK_Aerial_Ace,
  &ATK_XScissor,
  &ATK_Close_Combat,
  NULL
};

static const attack* SNEASLER_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Poison_Jab,
  &ATK_Rock_Smash,
  &ATK_Aerial_Ace,
  &ATK_XScissor,
  &ATK_Close_Combat,
  NULL
};

static const attack* SPHEAL_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Rock_Smash,
  &ATK_Water_Pulse,
  &ATK_Body_Slam,
  &ATK_Aurora_Beam,
  NULL
};

static const attack* SEALEO_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Powder_Snow,
  &ATK_Water_Pulse,
  &ATK_Body_Slam,
  &ATK_Aurora_Beam,
  &ATK_Surf,
  NULL
};

static const attack* WALREIN_ATKS[] = {
  &ATK_Frost_Breath,
  &ATK_Powder_Snow,
  &ATK_Waterfall,
  &ATK_Earthquake,
  &ATK_Blizzard,
  &ATK_Water_Pulse,
  &ATK_Icicle_Spear,
  NULL
};

static const attack* REGIGIGAS_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Hidden_Power,
  &ATK_Thunder,
  &ATK_Focus_Blast,
  &ATK_Giga_Impact,
  NULL
};

static const attack* YAMASK_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Astonish,
  &ATK_Dark_Pulse,
  &ATK_Ominous_Wind,
  &ATK_Shadow_Ball,
  NULL
};

static const attack* COFAGRIGUS_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Zen_Headbutt,
  &ATK_Astonish,
  &ATK_Dark_Pulse,
  &ATK_Shadow_Ball,
  &ATK_Psychic,
  NULL
};

static const attack* PASSIMIAN_ATKS[] = {
  &ATK_Rock_Smash,
  &ATK_Counter,
  &ATK_Take_Down,
  &ATK_Brick_Break,
  &ATK_Close_Combat,
  &ATK_Superpower,
  &ATK_Brutal_Swing,
  NULL
};

static const attack* GUZZLORD_ATKS[] = {
  &ATK_Dragon_Tail,
  &ATK_Snarl,
  &ATK_Dragon_Claw,
  &ATK_Sludge_Bomb,
  &ATK_Crunch,
  &ATK_Brutal_Swing,
  NULL
};

static const attack* TYROGUE_ATKS[] = {
  &ATK_Tackle,
  &ATK_Rock_Smash,
  &ATK_Low_Sweep,
  &ATK_Rock_Slide,
  &ATK_Brick_Break,
  NULL
};

static const attack* HITMONLEE_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Rock_Smash,
  &ATK_Double_Kick,
  &ATK_Stone_Edge,
  &ATK_Low_Sweep,
  &ATK_Brick_Break,
  &ATK_Stomp,
  &ATK_Close_Combat,
  &ATK_Blaze_Kick,
  NULL
};

static const attack* HITMONCHAN_ATKS[] = {
  &ATK_Bullet_Punch,
  &ATK_Rock_Smash,
  &ATK_Counter,
  &ATK_Ice_Punch,
  &ATK_Thunder_Punch,
  &ATK_Fire_Punch,
  &ATK_Brick_Break,
  &ATK_Close_Combat,
  &ATK_Power_Up_Punch,
  NULL
};

static const attack* HITMONTOP_ATKS[] = {
  &ATK_Rock_Smash,
  &ATK_Counter,
  &ATK_Stone_Edge,
  &ATK_Close_Combat,
  &ATK_Gyro_Ball,
  &ATK_Triple_Axel,
  NULL
};

static const attack* SHROODLE_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Mud_Slap,
  &ATK_Poison_Fang,
  &ATK_Sludge_Bomb,
  &ATK_Acid_Spray,
  NULL
};

static const attack* GRAFAIAI_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Mud_Slap,
  &ATK_Poison_Fang,
  &ATK_Sludge_Bomb,
  &ATK_Acid_Spray,
  NULL
};

static const attack* TOEDSCOOL_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Mud_Shot,
  &ATK_Earth_Power,
  &ATK_Seed_Bomb,
  &ATK_Wrap,
  NULL
};

static const attack* TOEDSCRUEL_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Mud_Shot,
  &ATK_Earth_Power,
  &ATK_Seed_Bomb,
  &ATK_Acid_Spray,
  NULL
};

static const attack* VAROOM_ATKS[] = {
  &ATK_Lick,
  &ATK_Poison_Jab,
  &ATK_Metal_Sound,
  &ATK_Gunk_Shot,
  &ATK_Gyro_Ball,
  &ATK_Acid_Spray,
  NULL
};

static const attack* REVAVROOM_ATKS[] = {
  &ATK_Lick,
  &ATK_Poison_Jab,
  &ATK_Metal_Sound,
  &ATK_Gunk_Shot,
  &ATK_Gyro_Ball,
  &ATK_Overheat,
  &ATK_Acid_Spray,
  NULL
};

static const attack* SEVIPER_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Iron_Tail,
  &ATK_Wrap,
  &ATK_Poison_Fang,
  &ATK_Crunch,
  NULL
};

static const attack* POOCHYENA_ATKS[] = {
  &ATK_Tackle,
  &ATK_Snarl,
  &ATK_Dig,
  &ATK_Poison_Fang,
  &ATK_Crunch,
  NULL
};

static const attack* MIGHTYENA_ATKS[] = {
  &ATK_Bite,
  &ATK_Sucker_Punch,
  &ATK_Fire_Fang,
  &ATK_Thunder_Fang,
  &ATK_Ice_Fang,
  &ATK_Poison_Fang,
  &ATK_Play_Rough,
  &ATK_Crunch,
  NULL
};

static const attack* TIMBURR_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Pound,
  &ATK_Low_Sweep,
  &ATK_Rock_Tomb,
  &ATK_Brick_Break,
  NULL
};

static const attack* GURDURR_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Poison_Jab,
  &ATK_Stone_Edge,
  &ATK_Low_Sweep,
  &ATK_Brick_Break,
  NULL
};

static const attack* CONKELDURR_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Counter,
  &ATK_Stone_Edge,
  &ATK_Dynamic_Punch,
  &ATK_Focus_Blast,
  &ATK_Brutal_Swing,
  NULL
};

static const attack* MARSHADOW_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Poison_Jab,
  &ATK_Counter,
  &ATK_Ice_Punch,
  &ATK_Rock_Slide,
  &ATK_Thunder_Punch,
  &ATK_Fire_Punch,
  &ATK_Close_Combat,
  NULL
};

static const attack* HEATMOR_ATKS[] = {
  &ATK_Lick,
  &ATK_Fire_Spin,
  &ATK_Flamethrower,
  &ATK_Thunder_Punch,
  &ATK_Power_Up_Punch,
  NULL
};

static const attack* KECLEON_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Lick,
  &ATK_Flamethrower,
  &ATK_Ice_Beam,
  &ATK_Aerial_Ace,
  &ATK_Shadow_Sneak,
  &ATK_Thunder,
  &ATK_Foul_Play,
  NULL
};

static const attack* VULLABY_ATKS[] = {
  &ATK_Feint_Attack,
  &ATK_Air_Slash,
  &ATK_Dark_Pulse,
  &ATK_Brave_Bird,
  &ATK_Foul_Play,
  NULL
};

static const attack* MANDIBUZZ_ATKS[] = {
  &ATK_Air_Slash,
  &ATK_Snarl,
  &ATK_Dark_Pulse,
  &ATK_Aerial_Ace,
  &ATK_Shadow_Ball,
  &ATK_Foul_Play,
  NULL
};

static const attack* NATU_ATKS[] = {
  &ATK_Peck,
  &ATK_Quick_Attack,
  &ATK_Drill_Peck,
  &ATK_Psyshock,
  &ATK_Night_Shade,
  NULL
};

static const attack* XATU_ATKS[] = {
  &ATK_Feint_Attack,
  &ATK_Air_Slash,
  &ATK_Aerial_Ace,
  &ATK_Ominous_Wind,
  &ATK_Future_Sight,
  NULL
};

static const attack* MURKROW_ATKS[] = {
  &ATK_Peck,
  &ATK_Feint_Attack,
  &ATK_Dark_Pulse,
  &ATK_Drill_Peck,
  &ATK_Foul_Play,
  NULL
};

static const attack* HONCHKROW_ATKS[] = {
  &ATK_Peck,
  &ATK_Snarl,
  &ATK_Dark_Pulse,
  &ATK_Psychic,
  &ATK_Brave_Bird,
  &ATK_Sky_Attack,
  NULL
};

static const attack* POIPOLE_ATKS[] = {
  &ATK_Peck,
  &ATK_Poison_Jab,
  &ATK_Sludge_Bomb,
  &ATK_Sludge_Wave,
  &ATK_Fell_Stinger,
  NULL
};

static const attack* NAGANADEL_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Air_Slash,
  &ATK_Dragon_Pulse,
  &ATK_Dragon_Claw,
  &ATK_Sludge_Bomb,
  &ATK_Fell_Stinger,
  &ATK_Acrobatics,
  NULL
};

static const attack* DEINO_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Tackle,
  &ATK_Dragon_Pulse,
  &ATK_Body_Slam,
  &ATK_Crunch,
  NULL
};

static const attack* ZWEILOUS_ATKS[] = {
  &ATK_Bite,
  &ATK_Dragon_Breath,
  &ATK_Dark_Pulse,
  &ATK_Dragon_Pulse,
  &ATK_Body_Slam,
  NULL
};

static const attack* HYDREIGON_ATKS[] = {
  &ATK_Bite,
  &ATK_Dragon_Breath,
  &ATK_Dark_Pulse,
  &ATK_Flash_Cannon,
  &ATK_Dragon_Pulse,
  &ATK_Brutal_Swing,
  NULL
};

static const attack* DRILBUR_ATKS[] = {
  &ATK_Scratch,
  &ATK_Mud_Slap,
  &ATK_Dig,
  &ATK_Drill_Run,
  &ATK_Rock_Tomb,
  NULL
};

static const attack* EXCADRILL_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Metal_Claw,
  &ATK_Mud_Slap,
  &ATK_Earthquake,
  &ATK_Drill_Run,
  &ATK_Rock_Slide,
  &ATK_Iron_Head,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* FALINKS_ATKS[] = {
  &ATK_Rock_Smash,
  &ATK_Counter,
  &ATK_Megahorn,
  &ATK_Brick_Break,
  &ATK_Superpower,
  NULL
};

static const attack* ANORITH_ATKS[] = {
  &ATK_Scratch,
  &ATK_Struggle_Bug,
  &ATK_Aqua_Jet,
  &ATK_Ancient_Power,
  &ATK_Cross_Poison,
  NULL
};

static const attack* ARMALDO_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Struggle_Bug,
  &ATK_Cross_Poison,
  &ATK_Water_Pulse,
  &ATK_Rock_Blast,
  &ATK_Liquidation,
  NULL
};

static const attack* FINNEON_ATKS[] = {
  &ATK_Pound,
  &ATK_Water_Gun,
  &ATK_Ice_Beam,
  &ATK_Water_Pulse,
  &ATK_Silver_Wind,
  NULL
};

static const attack* LUMINEON_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Waterfall,
  &ATK_Blizzard,
  &ATK_Water_Pulse,
  &ATK_Silver_Wind,
  NULL
};

static const attack* SEWADDLE_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Struggle_Bug,
  &ATK_Seed_Bomb,
  &ATK_Silver_Wind,
  &ATK_Energy_Ball,
  NULL
};

static const attack* SWADLOON_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Struggle_Bug,
  &ATK_Bug_Buzz,
  &ATK_Silver_Wind,
  &ATK_Energy_Ball,
  NULL
};

static const attack* LEAVANNY_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Shadow_Claw,
  &ATK_Razor_Leaf,
  &ATK_XScissor,
  &ATK_Leaf_Blade,
  &ATK_Silver_Wind,
  &ATK_Leaf_Storm,
  NULL
};

static const attack* COMBEE_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Bug_Buzz,
  NULL
};

static const attack* VESPIQUEN_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Bug_Bite,
  &ATK_Poison_Sting,
  &ATK_Air_Slash,
  &ATK_Bug_Buzz,
  &ATK_Power_Gem,
  &ATK_Signal_Beam,
  &ATK_XScissor,
  &ATK_Fell_Stinger,
  NULL
};

static const attack* QWILFISH_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Poison_Sting,
  &ATK_Ice_Beam,
  &ATK_Aqua_Tail,
  &ATK_Sludge_Wave,
  &ATK_Scald,
  &ATK_Acid_Spray,
  &ATK_Fell_Stinger,
  NULL
};

static const attack* H_QWILFISH_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Poison_Sting,
  &ATK_Dark_Pulse,
  &ATK_Ice_Beam,
  &ATK_Aqua_Tail,
  &ATK_Shadow_Ball,
  &ATK_Sludge_Bomb,
  NULL
};

static const attack* OVERQWIL_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Poison_Sting,
  &ATK_Dark_Pulse,
  &ATK_Ice_Beam,
  &ATK_Aqua_Tail,
  &ATK_Shadow_Ball,
  &ATK_Sludge_Bomb,
  NULL
};

static const attack* VENIPEDE_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Poison_Sting,
  &ATK_Sludge_Bomb,
  &ATK_Signal_Beam,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* WHIRLIPEDE_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Poison_Sting,
  &ATK_Sludge_Bomb,
  &ATK_Signal_Beam,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* SCOLIPEDE_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Poison_Jab,
  &ATK_Poison_Sting,
  &ATK_Megahorn,
  &ATK_Sludge_Bomb,
  &ATK_XScissor,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* MUDBRAY_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Rock_Smash,
  &ATK_Earthquake,
  &ATK_Bulldoze,
  &ATK_Body_Slam,
  NULL
};

static const attack* MUDSDALE_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Rock_Smash,
  &ATK_Earthquake,
  &ATK_Bulldoze,
  &ATK_Body_Slam,
  &ATK_Heavy_Slam,
  NULL
};

static const attack* BRONZOR_ATKS[] = {
  &ATK_Tackle,
  &ATK_Confusion,
  &ATK_Psyshock,
  &ATK_Gyro_Ball,
  &ATK_Heavy_Slam,
  &ATK_Payback,
  NULL
};

static const attack* BRONZONG_ATKS[] = {
  &ATK_Confusion,
  &ATK_Feint_Attack,
  &ATK_Metal_Sound,
  &ATK_Flash_Cannon,
  &ATK_Psyshock,
  &ATK_Bulldoze,
  &ATK_Psychic,
  &ATK_Heavy_Slam,
  &ATK_Payback,
  NULL
};

static const attack* SHUCKLE_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Struggle_Bug,
  &ATK_Stone_Edge,
  &ATK_Rock_Blast,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* A_GEODUDE_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Volt_Switch,
  &ATK_Rock_Tomb,
  &ATK_Rock_Slide,
  &ATK_Thunderbolt,
  NULL
};

static const attack* A_GRAVELER_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Volt_Switch,
  &ATK_Stone_Edge,
  &ATK_Thunderbolt,
  &ATK_Rock_Blast,
  NULL
};

static const attack* A_GOLEM_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Volt_Switch,
  &ATK_Rollout,
  &ATK_Stone_Edge,
  &ATK_Wild_Charge,
  &ATK_Rock_Blast,
  NULL
};

static const attack* GEODUDE_ATKS[] = {
  &ATK_Tackle,
  &ATK_Rock_Throw,
  &ATK_Dig,
  &ATK_Rock_Tomb,
  &ATK_Rock_Slide,
  NULL
};

static const attack* GRAVELER_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Rock_Throw,
  &ATK_Mud_Slap,
  &ATK_Dig,
  &ATK_Stone_Edge,
  &ATK_Rock_Slide,
  &ATK_Rock_Blast,
  NULL
};

static const attack* GOLEM_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Rock_Throw,
  &ATK_Mud_Slap,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Ancient_Power,
  &ATK_Rock_Blast,
  NULL
};

static const attack* DUNSPARCE_ATKS[] = {
  &ATK_Bite,
  &ATK_Astonish,
  &ATK_Rollout,
  &ATK_Dig,
  &ATK_Drill_Run,
  &ATK_Rock_Slide,
  NULL
};

static const attack* BOMBIRDIER_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Wing_Attack,
  &ATK_Rock_Throw,
  &ATK_Aerial_Ace,
  &ATK_Rock_Tomb,
  &ATK_Payback,
  &ATK_Fly,
  NULL
};

static const attack* ONIX_ATKS[] = {
  &ATK_Tackle,
  &ATK_Rock_Throw,
  &ATK_Stone_Edge,
  &ATK_Rock_Slide,
  &ATK_Iron_Head,
  &ATK_Sand_Tomb,
  &ATK_Heavy_Slam,
  &ATK_Breaking_Swipe,
  NULL
};

static const attack* STEELIX_ATKS[] = {
  &ATK_Dragon_Tail,
  &ATK_Iron_Tail,
  &ATK_Thunder_Fang,
  &ATK_Earthquake,
  &ATK_Heavy_Slam,
  &ATK_Crunch,
  &ATK_Psychic_Fangs,
  &ATK_Breaking_Swipe,
  NULL
};

static const attack* SHIELDON_ATKS[] = {
  &ATK_Tackle,
  &ATK_Iron_Tail,
  &ATK_Ancient_Power,
  &ATK_Rock_Tomb,
  &ATK_Heavy_Slam,
  NULL
};

static const attack* BASTIODON_ATKS[] = {
  &ATK_Iron_Tail,
  &ATK_Smack_Down,
  &ATK_Flamethrower,
  &ATK_Stone_Edge,
  &ATK_Flash_Cannon,
  NULL
};

static const attack* LARVITAR_ATKS[] = {
  &ATK_Bite,
  &ATK_Rock_Smash,
  &ATK_Ancient_Power,
  &ATK_Stomp,
  &ATK_Crunch,
  NULL
};

static const attack* PUPITAR_ATKS[] = {
  &ATK_Bite,
  &ATK_Rock_Smash,
  &ATK_Dig,
  &ATK_Ancient_Power,
  &ATK_Crunch,
  NULL
};

static const attack* TYRANITAR_ATKS[] = {
  &ATK_Bite,
  &ATK_Iron_Tail,
  &ATK_Smack_Down,
  &ATK_Stone_Edge,
  &ATK_Fire_Blast,
  &ATK_Crunch,
  &ATK_Brutal_Swing,
  NULL
};

static const attack* HOOH_ATKS[] = {
  &ATK_Steel_Wing,
  &ATK_Extrasensory,
  &ATK_Hidden_Power,
  &ATK_Incinerate,
  &ATK_Earthquake,
  &ATK_Fire_Blast,
  &ATK_Solar_Beam,
  &ATK_Brave_Bird,
  &ATK_Sacred_Fire,
  NULL
};

static const attack* LUGIA_ATKS[] = {
  &ATK_Dragon_Tail,
  &ATK_Extrasensory,
  &ATK_Hydro_Pump,
  &ATK_Sky_Attack,
  &ATK_Future_Sight,
  &ATK_Aeroblast,
  &ATK_Fly,
  NULL
};

static const attack* PIDOVE_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Air_Slash,
  &ATK_Heat_Wave,
  &ATK_Aerial_Ace,
  &ATK_Air_Cutter,
  NULL
};

static const attack* TRANQUILL_ATKS[] = {
  &ATK_Steel_Wing,
  &ATK_Air_Slash,
  &ATK_Heat_Wave,
  &ATK_Aerial_Ace,
  &ATK_Sky_Attack,
  NULL
};

static const attack* UNFEZANT_ATKS[] = {
  &ATK_Steel_Wing,
  &ATK_Air_Slash,
  &ATK_Hyper_Beam,
  &ATK_Heat_Wave,
  &ATK_Sky_Attack,
  NULL
};

static const attack* TIRTOUGA_ATKS[] = {
  &ATK_Bite,
  &ATK_Water_Gun,
  &ATK_Ancient_Power,
  &ATK_Body_Slam,
  &ATK_Surf,
  NULL
};

static const attack* CARRACOSTA_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Water_Gun,
  &ATK_Ancient_Power,
  &ATK_Body_Slam,
  &ATK_Surf,
  &ATK_Liquidation,
  NULL
};

static const attack* CARBINK_ATKS[] = {
  &ATK_Tackle,
  &ATK_Rock_Throw,
  &ATK_Rock_Slide,
  &ATK_Power_Gem,
  &ATK_Moonblast,
  NULL
};

static const attack* WAILMER_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Splash,
  &ATK_Rollout,
  &ATK_Water_Pulse,
  &ATK_Scald,
  &ATK_Body_Slam,
  &ATK_Heavy_Slam,
  NULL
};

static const attack* WAILORD_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Zen_Headbutt,
  &ATK_Rollout,
  &ATK_Hyper_Beam,
  &ATK_Blizzard,
  &ATK_Scald,
  &ATK_Surf,
  NULL
};

static const attack* SLAKOTH_ATKS[] = {
  &ATK_Yawn,
  &ATK_Night_Slash,
  &ATK_Brick_Break,
  &ATK_Body_Slam,
  NULL
};

static const attack* VIGOROTH_ATKS[] = {
  &ATK_Scratch,
  &ATK_Counter,
  &ATK_Rock_Slide,
  &ATK_Bulldoze,
  &ATK_Brick_Break,
  &ATK_Body_Slam,
  NULL
};

static const attack* SLAKING_ATKS[] = {
  &ATK_Yawn,
  &ATK_Hyper_Beam,
  &ATK_Earthquake,
  &ATK_Play_Rough,
  &ATK_Body_Slam,
  NULL
};

static const attack* KOMALA_ATKS[] = {
  &ATK_Yawn,
  &ATK_Rollout,
  &ATK_Play_Rough,
  &ATK_Bulldoze,
  &ATK_Payback,
  NULL
};

static const attack* PAWNIARD_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Scratch,
  &ATK_Night_Slash,
  &ATK_Iron_Head,
  &ATK_XScissor,
  NULL
};

static const attack* BISHARP_ATKS[] = {
  &ATK_Metal_Claw,
  &ATK_Snarl,
  &ATK_Dark_Pulse,
  &ATK_Iron_Head,
  &ATK_XScissor,
  &ATK_Focus_Blast,
  NULL
};

static const attack* KINGAMBIT_ATKS[] = {
  &ATK_Metal_Claw,
  &ATK_Snarl,
  &ATK_Metal_Sound,
  &ATK_Dark_Pulse,
  &ATK_Iron_Head,
  &ATK_XScissor,
  &ATK_Focus_Blast,
  &ATK_Foul_Play,
  NULL
};

static const attack* ORANGURU_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Confusion,
  &ATK_Yawn,
  &ATK_Psychic,
  &ATK_Future_Sight,
  &ATK_Foul_Play,
  &ATK_Brutal_Swing,
  &ATK_Trailblaze,
  NULL
};

static const attack* SHELLOS_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Hidden_Power,
  &ATK_Mud_Bomb,
  &ATK_Water_Pulse,
  &ATK_Body_Slam,
  NULL
};

static const attack* GASTRODON_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Hidden_Power,
  &ATK_Earthquake,
  &ATK_Water_Pulse,
  &ATK_Body_Slam,
  &ATK_Earth_Power,
  NULL
};

static const attack* SHAYMIN_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Hidden_Power,
  &ATK_Solar_Beam,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  &ATK_Seed_Flare,
  NULL
};

static const attack* S_SHAYMIN_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Hidden_Power,
  &ATK_Magical_Leaf,
  &ATK_Solar_Beam,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  &ATK_Seed_Flare,
  NULL
};

static const attack* XERNEAS_ATKS[] = {
  &ATK_Tackle,
  &ATK_Zen_Headbutt,
  &ATK_Geomancy,
  &ATK_Megahorn,
  &ATK_Thunder,
  &ATK_Moonblast,
  &ATK_Close_Combat,
  &ATK_Giga_Impact,
  NULL
};

static const attack* BELDUM_ATKS[] = {
  &ATK_Take_Down,
  &ATK_Iron_Head,
  NULL
};

static const attack* NINCADA_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Scratch,
  &ATK_Aerial_Ace,
  &ATK_Bug_Buzz,
  &ATK_Night_Slash,
  NULL
};

static const attack* NINJASK_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Metal_Claw,
  &ATK_Aerial_Ace,
  &ATK_Bug_Buzz,
  &ATK_Shadow_Ball,
  NULL
};

static const attack* METANG_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Metal_Claw,
  &ATK_Zen_Headbutt,
  &ATK_Psyshock,
  &ATK_Psychic,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* METAGROSS_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Bullet_Punch,
  &ATK_Zen_Headbutt,
  &ATK_Earthquake,
  &ATK_Flash_Cannon,
  &ATK_Psychic,
  &ATK_Meteor_Mash,
  NULL
};

static const attack* DWEBBLE_ATKS[] = {
  &ATK_Cut,
  &ATK_Smack_Down,
  &ATK_Rock_Tomb,
  &ATK_XScissor,
  &ATK_Rock_Blast,
  NULL
};

static const attack* CRUSTLE_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Smack_Down,
  &ATK_Rock_Slide,
  &ATK_XScissor,
  &ATK_Rock_Blast,
  NULL
};

static const attack* SCYTHER_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Steel_Wing,
  &ATK_Air_Slash,
  &ATK_Aerial_Ace,
  &ATK_Bug_Buzz,
  &ATK_Night_Slash,
  &ATK_XScissor,
  &ATK_Trailblaze,
  NULL
};

static const attack* SCIZOR_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Bullet_Punch,
  &ATK_Night_Slash,
  &ATK_Iron_Head,
  &ATK_XScissor,
  &ATK_Trailblaze,
  NULL
};

static const attack* PHANPY_ATKS[] = {
  &ATK_Tackle,
  &ATK_Rock_Smash,
  &ATK_Rock_Slide,
  &ATK_Bulldoze,
  &ATK_Body_Slam,
  &ATK_Trailblaze,
  NULL
};

static const attack* DONPHAN_ATKS[] = {
  &ATK_Tackle,
  &ATK_Mud_Slap,
  &ATK_Counter,
  &ATK_Charm,
  &ATK_Earthquake,
  &ATK_Play_Rough,
  &ATK_Body_Slam,
  &ATK_Heavy_Slam,
  &ATK_Trailblaze,
  NULL
};

static const attack* TRUBBISH_ATKS[] = {
  &ATK_Pound,
  &ATK_Take_Down,
  &ATK_Seed_Bomb,
  &ATK_Gunk_Shot,
  NULL
};

static const attack* GARBODOR_ATKS[] = {
  &ATK_Infestation,
  &ATK_Take_Down,
  &ATK_Seed_Bomb,
  &ATK_Gunk_Shot,
  &ATK_Body_Slam,
  &ATK_Acid_Spray,
  NULL
};

static const attack* WYNAUT_ATKS[] = {
  &ATK_Splash,
  &ATK_Counter,
  &ATK_Mirror_Coat,
  NULL
};

static const attack* WOBBUFFET_ATKS[] = {
  &ATK_Splash,
  &ATK_Counter,
  &ATK_Charm,
  &ATK_Mirror_Coat,
  NULL
};

static const attack* DEWPIDER_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Infestation,
  &ATK_Bubble_Beam,
  &ATK_Water_Pulse,
  &ATK_Mirror_Coat,
  NULL
};

static const attack* ARAQUANID_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Infestation,
  &ATK_Bug_Buzz,
  &ATK_Bubble_Beam,
  &ATK_Water_Pulse,
  &ATK_Mirror_Coat,
  NULL
};

static const attack* KOFFING_ATKS[] = {
  &ATK_Tackle,
  &ATK_Infestation,
  &ATK_Dark_Pulse,
  &ATK_Sludge,
  &ATK_Sludge_Bomb,
  NULL
};

static const attack* WEEZING_ATKS[] = {
  &ATK_Tackle,
  &ATK_Infestation,
  &ATK_Rollout,
  &ATK_Dark_Pulse,
  &ATK_Shadow_Ball,
  &ATK_Thunderbolt,
  &ATK_Sludge_Bomb,
  NULL
};

static const attack* G_WEEZING_ATKS[] = {
  &ATK_Tackle,
  &ATK_Fairy_Wind,
  &ATK_Hyper_Beam,
  &ATK_Sludge,
  &ATK_Play_Rough,
  &ATK_Overheat,
  &ATK_Brutal_Swing,
  NULL
};

static const attack* GRIMER_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Mud_Slap,
  &ATK_Sludge,
  &ATK_Sludge_Bomb,
  &ATK_Mud_Bomb,
  NULL
};

static const attack* A_GRIMER_ATKS[] = {
  &ATK_Bite,
  &ATK_Poison_Jab,
  &ATK_Sludge_Bomb,
  &ATK_Gunk_Shot,
  &ATK_Crunch,
  NULL
};

static const attack* MUK_ATKS[] = {
  &ATK_Lick,
  &ATK_Poison_Jab,
  &ATK_Infestation,
  &ATK_Dark_Pulse,
  &ATK_Thunder_Punch,
  &ATK_Sludge_Wave,
  &ATK_Gunk_Shot,
  &ATK_Acid_Spray,
  NULL
};

static const attack* A_MUK_ATKS[] = {
  &ATK_Bite,
  &ATK_Poison_Jab,
  &ATK_Snarl,
  &ATK_Dark_Pulse,
  &ATK_Sludge_Wave,
  &ATK_Gunk_Shot,
  &ATK_Acid_Spray,
  NULL
};

static const attack* NIHILEGO_ATKS[] = {
  &ATK_Pound,
  &ATK_Poison_Jab,
  &ATK_Acid,
  &ATK_Rock_Slide,
  &ATK_Power_Gem,
  &ATK_Sludge_Bomb,
  &ATK_Gunk_Shot,
  NULL
};

static const attack* MARACTUS_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Bullet_Seed,
  &ATK_Aerial_Ace,
  &ATK_Petal_Blizzard,
  &ATK_Solar_Beam,
  NULL
};

static const attack* BUDEW_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Hidden_Power,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* ROSELIA_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Poison_Jab,
  &ATK_Magical_Leaf,
  &ATK_Petal_Blizzard,
  &ATK_Dazzling_Gleam,
  &ATK_Sludge_Bomb,
  NULL
};

static const attack* ROSERADE_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Poison_Jab,
  &ATK_Poison_Sting,
  &ATK_Bullet_Seed,
  &ATK_Magical_Leaf,
  &ATK_Dazzling_Gleam,
  &ATK_Sludge_Bomb,
  &ATK_Solar_Beam,
  &ATK_Grass_Knot,
  &ATK_Weather_Ball_Fire,
  &ATK_Leaf_Storm,
  NULL
};

static const attack* CHERUBI_ATKS[] = {
  &ATK_Tackle,
  &ATK_Bullet_Seed,
  &ATK_Petal_Blizzard,
  &ATK_Seed_Bomb,
  &ATK_Dazzling_Gleam,
  NULL
};

static const attack* CHERRIM_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Bullet_Seed,
  &ATK_Hyper_Beam,
  &ATK_Dazzling_Gleam,
  &ATK_Solar_Beam,
  &ATK_Weather_Ball_Fire,
  NULL
};

static const attack* O_CHERRIM_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Bullet_Seed,
  &ATK_Hyper_Beam,
  &ATK_Dazzling_Gleam,
  &ATK_Solar_Beam,
  NULL
};

static const attack* COMFEY_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Hidden_Power,
  &ATK_Petal_Blizzard,
  &ATK_Draining_Kiss,
  &ATK_Grass_Knot,
  NULL
};

static const attack* SMOLIV_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Tackle,
  &ATK_Seed_Bomb,
  &ATK_Energy_Ball,
  &ATK_Earth_Power,
  &ATK_Trailblaze,
  NULL
};

static const attack* DOLLIV_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Tackle,
  &ATK_Magical_Leaf,
  &ATK_Seed_Bomb,
  &ATK_Energy_Ball,
  &ATK_Earth_Power,
  &ATK_Trailblaze,
  NULL
};

static const attack* ARBOLIVA_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Tackle,
  &ATK_Magical_Leaf,
  &ATK_Seed_Bomb,
  &ATK_Energy_Ball,
  &ATK_Earth_Power,
  &ATK_Trailblaze,
  NULL
};

static const attack* PICHU_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Thunder_Punch,
  &ATK_Thunderbolt,
  &ATK_Disarming_Voice,
  NULL
};

static const attack* EMOLGA_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Quick_Attack,
  &ATK_Discharge,
  &ATK_Aerial_Ace,
  &ATK_Thunderbolt,
  &ATK_Acrobatics,
  NULL
};

static const attack* MAREEP_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Tackle,
  &ATK_Discharge,
  &ATK_Thunderbolt,
  &ATK_Body_Slam,
  &ATK_Trailblaze,
  NULL
};

static const attack* FLAAFFY_ATKS[] = {
  &ATK_Tackle,
  &ATK_Charge_Beam,
  &ATK_Discharge,
  &ATK_Power_Gem,
  &ATK_Thunderbolt,
  &ATK_Trailblaze,
  NULL
};

static const attack* MUNNA_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Charge_Beam,
  &ATK_Psybeam,
  &ATK_Psyshock,
  &ATK_Dazzling_Gleam,
  NULL
};

static const attack* MUSHARNA_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Charge_Beam,
  &ATK_Psyshock,
  &ATK_Dazzling_Gleam,
  &ATK_Future_Sight,
  NULL
};

static const attack* GOTHITA_ATKS[] = {
  &ATK_Pound,
  &ATK_Confusion,
  &ATK_Psybeam,
  &ATK_Psyshock,
  &ATK_Psychic,
  NULL
};

static const attack* GOTHORITA_ATKS[] = {
  &ATK_Pound,
  &ATK_Confusion,
  &ATK_Psybeam,
  &ATK_Psychic,
  &ATK_Future_Sight,
  NULL
};

static const attack* GOTHITELLE_ATKS[] = {
  &ATK_Confusion,
  &ATK_Charm,
  &ATK_Rock_Slide,
  &ATK_Psychic,
  &ATK_Future_Sight,
  NULL
};

static const attack* ZEKROM_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Charge_Beam,
  &ATK_Flash_Cannon,
  &ATK_Wild_Charge,
  &ATK_Outrage,
  &ATK_Crunch,
  &ATK_Fusion_Bolt,
  NULL
};

static const attack* ELEKID_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Low_Kick,
  &ATK_Discharge,
  &ATK_Thunder_Punch,
  &ATK_Thunderbolt,
  &ATK_Brick_Break,
  NULL
};

static const attack* ELECTABUZZ_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Low_Kick,
  &ATK_Thunder_Punch,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  NULL
};

static const attack* ELECTIVIRE_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Low_Kick,
  &ATK_Flamethrower,
  &ATK_Ice_Punch,
  &ATK_Thunder_Punch,
  &ATK_Thunder,
  &ATK_Wild_Charge,
  NULL
};

static const attack* MORPEKO_ATKS[] = {
  &ATK_Bite,
  &ATK_Thunder_Shock,
  &ATK_Charge_Beam,
  &ATK_Seed_Bomb,
  &ATK_Outrage,
  &ATK_Psychic_Fangs,
  &ATK_Aura_Wheel,
  NULL
};

static const attack* RAIKOU_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Volt_Switch,
  &ATK_Shadow_Ball,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  &ATK_Wild_Charge,
  NULL
};

static const attack* PAWMI_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Spark,
  &ATK_Charge_Beam,
  &ATK_Discharge,
  &ATK_Thunderbolt,
  &ATK_Wild_Charge,
  NULL
};

static const attack* PAWMO_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Spark,
  &ATK_Charge_Beam,
  &ATK_Discharge,
  &ATK_Thunder_Punch,
  &ATK_Thunderbolt,
  &ATK_Wild_Charge,
  NULL
};

static const attack* PAWMOT_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Spark,
  &ATK_Low_Kick,
  &ATK_Charge_Beam,
  &ATK_Discharge,
  &ATK_Thunder_Punch,
  &ATK_Thunderbolt,
  &ATK_Brick_Break,
  &ATK_Close_Combat,
  &ATK_Wild_Charge,
  NULL
};

static const attack* ZAPDOS_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Charge_Beam,
  &ATK_Drill_Peck,
  &ATK_Ancient_Power,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  &ATK_Zap_Cannon,
  NULL
};

static const attack* G_ZAPDOS_ATKS[] = {
  &ATK_Counter,
  &ATK_Ancient_Power,
  &ATK_Close_Combat,
  &ATK_Brave_Bird,
  &ATK_Fly,
  NULL
};

static const attack* DEDENNE_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Tackle,
  &ATK_Discharge,
  &ATK_Parabolic_Charge,
  &ATK_Play_Rough,
  NULL
};

static const attack* HELIOPTILE_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Quick_Attack,
  &ATK_Parabolic_Charge,
  &ATK_Thunderbolt,
  &ATK_Bulldoze,
  NULL
};

static const attack* HELIOLISK_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Mud_Slap,
  &ATK_Volt_Switch,
  &ATK_Parabolic_Charge,
  &ATK_Thunderbolt,
  &ATK_Bulldoze,
  &ATK_Grass_Knot,
  &ATK_Breaking_Swipe,
  NULL
};

static const attack* CORPHISH_ATKS[] = {
  &ATK_Bubble,
  &ATK_Rock_Smash,
  &ATK_Vise_Grip,
  &ATK_Bubble_Beam,
  &ATK_Aqua_Jet,
  &ATK_Scald,
  &ATK_Razor_Shell,
  NULL
};

static const attack* CRAWDAUNT_ATKS[] = {
  &ATK_Snarl,
  &ATK_Waterfall,
  &ATK_Vise_Grip,
  &ATK_Night_Slash,
  &ATK_Bubble_Beam,
  &ATK_Scald,
  &ATK_Crabhammer,
  &ATK_Razor_Shell,
  NULL
};

static const attack* CRABRAWLER_ATKS[] = {
  &ATK_Bubble,
  &ATK_Rock_Smash,
  &ATK_Brick_Break,
  &ATK_Crabhammer,
  &ATK_Power_Up_Punch,
  &ATK_Payback,
  NULL
};

static const attack* CRABOMINABLE_ATKS[] = {
  &ATK_Bubble,
  &ATK_Rock_Smash,
  &ATK_Ice_Punch,
  &ATK_Brick_Break,
  &ATK_Crabhammer,
  &ATK_Power_Up_Punch,
  &ATK_Payback,
  NULL
};

static const attack* CLAUNCHER_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Smack_Down,
  &ATK_Aqua_Jet,
  &ATK_Water_Pulse,
  &ATK_Crabhammer,
  NULL
};

static const attack* CLAWITZER_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Smack_Down,
  &ATK_Dark_Pulse,
  &ATK_Ice_Beam,
  &ATK_Water_Pulse,
  &ATK_Crabhammer,
  NULL
};

static const attack* KLINK_ATKS[] = {
  &ATK_Charge_Beam,
  &ATK_Volt_Switch,
  &ATK_Metal_Sound,
  &ATK_Vise_Grip,
  &ATK_Discharge,
  &ATK_Zap_Cannon,
  NULL
};

static const attack* KLANG_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Charge_Beam,
  &ATK_Metal_Sound,
  &ATK_Vise_Grip,
  &ATK_Thunderbolt,
  &ATK_Zap_Cannon,
  NULL
};

static const attack* KLINKKLANG_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Charge_Beam,
  &ATK_Metal_Sound,
  &ATK_Hyper_Beam,
  &ATK_Flash_Cannon,
  &ATK_Zap_Cannon,
  &ATK_Mirror_Shot,
  NULL
};

static const attack* ZYGARDE10_ATKS[] = {
  &ATK_Bite,
  &ATK_Zen_Headbutt,
  &ATK_Dragon_Tail,
  &ATK_Hyper_Beam,
  &ATK_Earthquake,
  &ATK_Bulldoze,
  &ATK_Outrage,
  &ATK_Crunch,
  NULL
};

static const attack* ZYGARDE50_ATKS[] = {
  &ATK_Bite,
  &ATK_Zen_Headbutt,
  &ATK_Dragon_Tail,
  &ATK_Hyper_Beam,
  &ATK_Earthquake,
  &ATK_Bulldoze,
  &ATK_Outrage,
  &ATK_Crunch,
  NULL
};

static const attack* ZYGARDE_ATKS[] = {
  &ATK_Bite,
  &ATK_Zen_Headbutt,
  &ATK_Dragon_Tail,
  &ATK_Hyper_Beam,
  &ATK_Earthquake,
  &ATK_Bulldoze,
  &ATK_Outrage,
  &ATK_Crunch,
  NULL
};

static const attack* DEOXYS_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Charge_Beam,
  &ATK_Hyper_Beam,
  &ATK_Thunderbolt,
  &ATK_Psycho_Boost,
  NULL
};

static const attack* A_DEOXYS_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Zen_Headbutt,
  &ATK_Dark_Pulse,
  &ATK_Zap_Cannon,
  &ATK_Psycho_Boost,
  NULL
};

static const attack* D_DEOXYS_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Counter,
  &ATK_Rock_Slide,
  &ATK_Thunderbolt,
  &ATK_Psycho_Boost,
  NULL
};

static const attack* S_DEOXYS_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Charge_Beam,
  &ATK_Thunderbolt,
  &ATK_Swift,
  &ATK_Psycho_Boost,
  NULL
};

static const attack* PORYGON_ATKS[] = {
  &ATK_Tackle,
  &ATK_Zen_Headbutt,
  &ATK_Charge_Beam,
  &ATK_Hidden_Power,
  &ATK_Hyper_Beam,
  &ATK_Psybeam,
  &ATK_Discharge,
  &ATK_Signal_Beam,
  &ATK_Solar_Beam,
  &ATK_Zap_Cannon,
  NULL
};

static const attack* PORYGON2_ATKS[] = {
  &ATK_Charge_Beam,
  &ATK_Hidden_Power,
  &ATK_Lock_On,
  &ATK_Hyper_Beam,
  &ATK_Solar_Beam,
  &ATK_Zap_Cannon,
  &ATK_Tri_Attack,
  NULL
};

static const attack* PORYGONZ_ATKS[] = {
  &ATK_Charge_Beam,
  &ATK_Hidden_Power,
  &ATK_Lock_On,
  &ATK_Hyper_Beam,
  &ATK_Blizzard,
  &ATK_Solar_Beam,
  &ATK_Zap_Cannon,
  &ATK_Tri_Attack,
  NULL
};

static const attack* REMORAID_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Water_Gun,
  &ATK_Water_Pulse,
  &ATK_Aurora_Beam,
  &ATK_Rock_Blast,
  NULL
};

static const attack* OCTILLERY_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Water_Gun,
  &ATK_Lock_On,
  &ATK_Gunk_Shot,
  &ATK_Water_Pulse,
  &ATK_Aurora_Beam,
  &ATK_Acid_Spray,
  &ATK_Octazooka,
  NULL
};

static const attack* SKARMORY_ATKS[] = {
  &ATK_Steel_Wing,
  &ATK_Air_Slash,
  &ATK_Flash_Cannon,
  &ATK_Brave_Bird,
  &ATK_Sky_Attack,
  NULL
};

static const attack* AUDINO_ATKS[] = {
  &ATK_Pound,
  &ATK_Zen_Headbutt,
  &ATK_Hyper_Beam,
  &ATK_Disarming_Voice,
  &ATK_Dazzling_Gleam,
  &ATK_Body_Slam,
  NULL
};

static const attack* KLEFKI_ATKS[] = {
  &ATK_Tackle,
  &ATK_Astonish,
  &ATK_Flash_Cannon,
  &ATK_Draining_Kiss,
  &ATK_Play_Rough,
  &ATK_Foul_Play,
  NULL
};

static const attack* RHYHORN_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Rock_Smash,
  &ATK_Bulldoze,
  &ATK_Horn_Attack,
  &ATK_Stomp,
  NULL
};

static const attack* RHYDON_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Rock_Smash,
  &ATK_Megahorn,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Surf,
  &ATK_Breaking_Swipe,
  NULL
};

static const attack* RHYPERIOR_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Smack_Down,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Surf,
  &ATK_Skull_Bash,
  &ATK_Superpower,
  &ATK_Rock_Wrecker,
  &ATK_Breaking_Swipe,
  NULL
};

static const attack* WHISMUR_ATKS[] = {
  &ATK_Pound,
  &ATK_Astonish,
  &ATK_Flamethrower,
  &ATK_Disarming_Voice,
  &ATK_Stomp,
  NULL
};

static const attack* LOUDRED_ATKS[] = {
  &ATK_Bite,
  &ATK_Rock_Smash,
  &ATK_Flamethrower,
  &ATK_Disarming_Voice,
  &ATK_Stomp,
  NULL
};

static const attack* EXPLOUD_ATKS[] = {
  &ATK_Bite,
  &ATK_Astonish,
  &ATK_Disarming_Voice,
  &ATK_Fire_Blast,
  &ATK_Crunch,
  &ATK_Boomburst,
  NULL
};

static const attack* JANGMO_O_ATKS[] = {
  &ATK_Tackle,
  &ATK_Dragon_Tail,
  &ATK_Dragon_Pulse,
  &ATK_Dragon_Claw,
  &ATK_Brick_Break,
  NULL
};

static const attack* HAKAMO_O_ATKS[] = {
  &ATK_Tackle,
  &ATK_Dragon_Tail,
  &ATK_Dragon_Pulse,
  &ATK_Dragon_Claw,
  &ATK_Brick_Break,
  NULL
};

static const attack* KOMMO_O_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Dragon_Tail,
  &ATK_Flamethrower,
  &ATK_Dragon_Claw,
  &ATK_Brick_Break,
  &ATK_Close_Combat,
  &ATK_Boomburst,
  NULL
};

static const attack* RALTS_ATKS[] = {
  &ATK_Confusion,
  &ATK_Charge_Beam,
  &ATK_Psyshock,
  &ATK_Shadow_Sneak,
  &ATK_Disarming_Voice,
  NULL
};

static const attack* KIRLIA_ATKS[] = {
  &ATK_Confusion,
  &ATK_Charge_Beam,
  &ATK_Magical_Leaf,
  &ATK_Shadow_Sneak,
  &ATK_Disarming_Voice,
  &ATK_Draining_Kiss,
  &ATK_Psychic,
  &ATK_Triple_Axel,
  NULL
};

static const attack* GALLADE_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Psycho_Cut,
  &ATK_Confusion,
  &ATK_Charm,
  &ATK_Psychic,
  &ATK_Leaf_Blade,
  &ATK_Close_Combat,
  &ATK_Synchronoise,
  NULL
};

static const attack* GARDEVOIR_ATKS[] = {
  &ATK_Confusion,
  &ATK_Charge_Beam,
  &ATK_Charm,
  &ATK_Magical_Leaf,
  &ATK_Shadow_Ball,
  &ATK_Dazzling_Gleam,
  &ATK_Psychic,
  &ATK_Synchronoise,
  &ATK_Triple_Axel,
  NULL
};

static const attack* HATENNA_ATKS[] = {
  &ATK_Confusion,
  &ATK_Charm,
  &ATK_Psyshock,
  &ATK_Dazzling_Gleam,
  &ATK_Psychic,
  NULL
};

static const attack* HATTREM_ATKS[] = {
  &ATK_Confusion,
  &ATK_Charm,
  &ATK_Psyshock,
  &ATK_Dazzling_Gleam,
  &ATK_Psychic,
  NULL
};

static const attack* HATTERENE_ATKS[] = {
  &ATK_Psycho_Cut,
  &ATK_Confusion,
  &ATK_Charm,
  &ATK_Psyshock,
  &ATK_Dazzling_Gleam,
  &ATK_Psychic,
  &ATK_Power_Whip,
  NULL
};

static const attack* MEWTWO_ATKS[] = {
  &ATK_Psycho_Cut,
  &ATK_Confusion,
  &ATK_Hyper_Beam,
  &ATK_Flamethrower,
  &ATK_Ice_Beam,
  &ATK_Shadow_Ball,
  &ATK_Thunderbolt,
  &ATK_Psychic,
  &ATK_Psystrike,
  &ATK_Focus_Blast,
  NULL
};

static const attack* A_MEWTWO_ATKS[] = {
  &ATK_Confusion,
  &ATK_Iron_Tail,
  &ATK_Earthquake,
  &ATK_Rock_Slide,
  &ATK_Psystrike,
  &ATK_Dynamic_Punch,
  &ATK_Future_Sight,
  NULL
};

static const attack* PHEROMOSA_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Low_Kick,
  &ATK_Bug_Buzz,
  &ATK_Close_Combat,
  &ATK_Focus_Blast,
  &ATK_Lunge,
  NULL
};

static const attack* AMAURA_ATKS[] = {
  &ATK_Frost_Breath,
  &ATK_Powder_Snow,
  &ATK_Ancient_Power,
  &ATK_Thunderbolt,
  &ATK_Aurora_Beam,
  &ATK_Weather_Ball_Ice,
  NULL
};

static const attack* AURORUS_ATKS[] = {
  &ATK_Frost_Breath,
  &ATK_Rock_Throw,
  &ATK_Powder_Snow,
  &ATK_Hyper_Beam,
  &ATK_Blizzard,
  &ATK_Ancient_Power,
  &ATK_Thunderbolt,
  &ATK_Weather_Ball_Ice,
  &ATK_Meteor_Beam,
  NULL
};

static const attack* SHELLDER_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Tackle,
  &ATK_Bubble_Beam,
  &ATK_Water_Pulse,
  &ATK_Icy_Wind,
  &ATK_Razor_Shell,
  NULL
};

static const attack* CLOYSTER_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Frost_Breath,
  &ATK_Blizzard,
  &ATK_Hydro_Pump,
  &ATK_Icy_Wind,
  &ATK_Aurora_Beam,
  &ATK_Avalanche,
  &ATK_Razor_Shell,
  &ATK_Liquidation,
  NULL
};

static const attack* CRYOGONAL_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Frost_Breath,
  &ATK_Night_Slash,
  &ATK_Water_Pulse,
  &ATK_Solar_Beam,
  &ATK_Aurora_Beam,
  &ATK_Triple_Axel,
  NULL
};

static const attack* BOUNSWEET_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Charm,
  &ATK_Draining_Kiss,
  &ATK_Energy_Ball,
  NULL
};

static const attack* STEENEE_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Charm,
  &ATK_Draining_Kiss,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  &ATK_Triple_Axel,
  NULL
};

static const attack* TSAREENA_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Charm,
  &ATK_Magical_Leaf,
  &ATK_Draining_Kiss,
  &ATK_Stomp,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  &ATK_Triple_Axel,
  &ATK_High_Jump_Kick,
  NULL
};

static const attack* MIENFOO_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Pound,
  &ATK_Low_Sweep,
  &ATK_Brick_Break,
  &ATK_Focus_Blast,
  &ATK_High_Jump_Kick,
  NULL
};

static const attack* MIENSHAO_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Poison_Jab,
  &ATK_Force_Palm,
  &ATK_Stone_Edge,
  &ATK_Brick_Break,
  &ATK_Grass_Knot,
  &ATK_Blaze_Kick,
  &ATK_Brutal_Swing,
  NULL
};

static const attack* MUDKIP_ATKS[] = {
  &ATK_Tackle,
  &ATK_Water_Gun,
  &ATK_Sludge,
  &ATK_Dig,
  &ATK_Stomp,
  NULL
};

static const attack* MARSHTOMP_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Water_Gun,
  &ATK_Sludge,
  &ATK_Mud_Bomb,
  &ATK_Surf,
  NULL
};

static const attack* OMANYTE_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Water_Gun,
  &ATK_Bubble_Beam,
  &ATK_Ancient_Power,
  &ATK_Rock_Tomb,
  &ATK_Brine,
  &ATK_Rock_Blast,
  NULL
};

static const attack* OMASTAR_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Rock_Throw,
  &ATK_Water_Gun,
  &ATK_Ancient_Power,
  &ATK_Rock_Slide,
  &ATK_Hydro_Pump,
  &ATK_Rock_Blast,
  NULL
};

static const attack* STONJOURNER_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Stone_Edge,
  &ATK_Rock_Slide,
  &ATK_Stomp,
  NULL
};

static const attack* SWAMPERT_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Water_Gun,
  &ATK_Sludge,
  &ATK_Earthquake,
  &ATK_Sludge_Wave,
  &ATK_Surf,
  &ATK_Hydro_Cannon,
  &ATK_Muddy_Water,
  NULL
};

static const attack* NUMEL_ATKS[] = {
  &ATK_Ember,
  &ATK_Tackle,
  &ATK_Heat_Wave,
  &ATK_Bulldoze,
  &ATK_Stomp,
  NULL
};

static const attack* CAMERUPT_ATKS[] = {
  &ATK_Ember,
  &ATK_Rock_Smash,
  &ATK_Incinerate,
  &ATK_Earthquake,
  &ATK_Solar_Beam,
  &ATK_Overheat,
  &ATK_Earth_Power,
  NULL
};

static const attack* TORKOAL_ATKS[] = {
  &ATK_Ember,
  &ATK_Fire_Spin,
  &ATK_Earthquake,
  &ATK_Solar_Beam,
  &ATK_Overheat,
  NULL
};

static const attack* TEPIG_ATKS[] = {
  &ATK_Ember,
  &ATK_Tackle,
  &ATK_Flamethrower,
  &ATK_Flame_Charge,
  &ATK_Body_Slam,
  NULL
};

static const attack* PIGNITE_ATKS[] = {
  &ATK_Ember,
  &ATK_Tackle,
  &ATK_Flamethrower,
  &ATK_Rock_Tomb,
  &ATK_Flame_Charge,
  NULL
};

static const attack* EMBOAR_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Ember,
  &ATK_Heat_Wave,
  &ATK_Rock_Slide,
  &ATK_Flame_Charge,
  &ATK_Focus_Blast,
  &ATK_Blast_Burn,
  NULL
};

static const attack* TORCHIC_ATKS[] = {
  &ATK_Ember,
  &ATK_Scratch,
  &ATK_Flamethrower,
  &ATK_Rock_Tomb,
  &ATK_Flame_Charge,
  NULL
};

static const attack* COMBUSKEN_ATKS[] = {
  &ATK_Ember,
  &ATK_Peck,
  &ATK_Flamethrower,
  &ATK_Rock_Slide,
  &ATK_Flame_Charge,
  NULL
};

static const attack* BLAZIKEN_ATKS[] = {
  &ATK_Counter,
  &ATK_Fire_Spin,
  &ATK_Stone_Edge,
  &ATK_Focus_Blast,
  &ATK_Brave_Bird,
  &ATK_Overheat,
  &ATK_Blast_Burn,
  &ATK_Blaze_Kick,
  NULL
};

static const attack* HIPPOPOTAS_ATKS[] = {
  &ATK_Bite,
  &ATK_Tackle,
  &ATK_Sand_Attack,
  &ATK_Dig,
  &ATK_Rock_Tomb,
  &ATK_Body_Slam,
  NULL
};

static const attack* HIPPOWDON_ATKS[] = {
  &ATK_Bite,
  &ATK_Fire_Fang,
  &ATK_Thunder_Fang,
  &ATK_Ice_Fang,
  &ATK_Sand_Attack,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Body_Slam,
  &ATK_Weather_Ball_Rock,
  &ATK_Earth_Power,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* CASTFORM_ATKS[] = {
  &ATK_Tackle,
  &ATK_Hex,
  &ATK_Hurricane,
  &ATK_Energy_Ball,
  &ATK_Weather_Ball_Rock,
  &ATK_Weather_Ball_Normal,
  NULL
};

static const attack* ROOKIDEE_ATKS[] = {
  &ATK_Peck,
  &ATK_Air_Slash,
  &ATK_Sand_Attack,
  &ATK_Sky_Attack,
  &ATK_Payback,
  NULL
};

static const attack* CORVISQUIRE_ATKS[] = {
  &ATK_Peck,
  &ATK_Air_Slash,
  &ATK_Sand_Attack,
  &ATK_Sky_Attack,
  &ATK_Payback,
  NULL
};

static const attack* CORVIKNIGHT_ATKS[] = {
  &ATK_Steel_Wing,
  &ATK_Air_Slash,
  &ATK_Sand_Attack,
  &ATK_Iron_Head,
  &ATK_Sky_Attack,
  &ATK_Payback,
  NULL
};

static const attack* PERRSERKER_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Metal_Claw,
  &ATK_Metal_Sound,
  &ATK_Iron_Head,
  &ATK_Play_Rough,
  &ATK_Close_Combat,
  &ATK_Foul_Play,
  &ATK_Trailblaze,
  NULL
};

static const attack* SABLEYE_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Feint_Attack,
  &ATK_Power_Gem,
  &ATK_Shadow_Sneak,
  &ATK_Dazzling_Gleam,
  &ATK_Foul_Play,
  NULL
};

static const attack* XURKITREE_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Spark,
  &ATK_Discharge,
  &ATK_Thunder,
  &ATK_Dazzling_Gleam,
  &ATK_Power_Whip,
  NULL
};

static const attack* TADBULB_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Water_Gun,
  &ATK_Discharge,
  &ATK_Parabolic_Charge,
  &ATK_Zap_Cannon,
  NULL
};

static const attack* BELLIBOLT_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Thunder_Shock,
  &ATK_Water_Gun,
  &ATK_Discharge,
  &ATK_Parabolic_Charge,
  &ATK_Zap_Cannon,
  NULL
};

static const attack* MAGNEMITE_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Spark,
  &ATK_Volt_Switch,
  &ATK_Metal_Sound,
  &ATK_Discharge,
  &ATK_Magnet_Bomb,
  &ATK_Thunderbolt,
  NULL
};

static const attack* MAGNETON_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Spark,
  &ATK_Charge_Beam,
  &ATK_Volt_Switch,
  &ATK_Metal_Sound,
  &ATK_Discharge,
  &ATK_Flash_Cannon,
  &ATK_Magnet_Bomb,
  &ATK_Zap_Cannon,
  NULL
};

static const attack* MAGNEZONE_ATKS[] = {
  &ATK_Spark,
  &ATK_Charge_Beam,
  &ATK_Volt_Switch,
  &ATK_Metal_Sound,
  &ATK_Flash_Cannon,
  &ATK_Wild_Charge,
  &ATK_Zap_Cannon,
  &ATK_Mirror_Shot,
  NULL
};

static const attack* MELTAN_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Flash_Cannon,
  &ATK_Thunderbolt,
  NULL
};

static const attack* MELMETAL_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Hyper_Beam,
  &ATK_Flash_Cannon,
  &ATK_Rock_Slide,
  &ATK_Thunderbolt,
  &ATK_Superpower,
  &ATK_Double_Iron_Bash,
  NULL
};

static const attack* ARON_ATKS[] = {
  &ATK_Tackle,
  &ATK_Metal_Claw,
  &ATK_Metal_Sound,
  &ATK_Rock_Tomb,
  &ATK_Iron_Head,
  &ATK_Body_Slam,
  NULL
};

static const attack* LAIRON_ATKS[] = {
  &ATK_Metal_Claw,
  &ATK_Iron_Tail,
  &ATK_Metal_Sound,
  &ATK_Rock_Tomb,
  &ATK_Rock_Slide,
  &ATK_Body_Slam,
  &ATK_Heavy_Slam,
  NULL
};

static const attack* AGGRON_ATKS[] = {
  &ATK_Dragon_Tail,
  &ATK_Iron_Tail,
  &ATK_Smack_Down,
  &ATK_Stone_Edge,
  &ATK_Rock_Tomb,
  &ATK_Thunder,
  &ATK_Heavy_Slam,
  &ATK_Meteor_Beam,
  NULL
};

static const attack* DREEPY_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Astonish,
  &ATK_Struggle,
  NULL
};

static const attack* DRAKLOAK_ATKS[] = {
  &ATK_Dragon_Tail,
  &ATK_Astonish,
  &ATK_Shadow_Ball,
  &ATK_Dragon_Pulse,
  &ATK_Outrage,
  NULL
};

static const attack* DRAGAPULT_ATKS[] = {
  &ATK_Dragon_Tail,
  &ATK_Astonish,
  &ATK_Shadow_Ball,
  &ATK_Dragon_Pulse,
  &ATK_Outrage,
  &ATK_Breaking_Swipe,
  NULL
};

static const attack* FRIGIBAX_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Ice_Fang,
  &ATK_Dragon_Claw,
  &ATK_Icy_Wind,
  &ATK_Avalanche,
  &ATK_Outrage,
  NULL
};

static const attack* ARCTIBAX_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Ice_Fang,
  &ATK_Dragon_Claw,
  &ATK_Icy_Wind,
  &ATK_Avalanche,
  &ATK_Outrage,
  NULL
};

static const attack* BAXCALIBUR_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Ice_Fang,
  &ATK_Blizzard,
  &ATK_Dragon_Claw,
  &ATK_Icy_Wind,
  &ATK_Avalanche,
  &ATK_Outrage,
  NULL
};

static const attack* DELIBIRD_ATKS[] = {
  &ATK_Present,
  &ATK_Ice_Punch,
  &ATK_Aerial_Ace,
  &ATK_Icy_Wind,
  &ATK_Fly,
  &ATK_Triple_Axel,
  NULL
};

static const attack* MEDITITE_ATKS[] = {
  &ATK_Confusion,
  &ATK_Rock_Smash,
  &ATK_Ice_Punch,
  &ATK_Low_Sweep,
  &ATK_Psyshock,
  NULL
};

static const attack* MEDICHAM_ATKS[] = {
  &ATK_Psycho_Cut,
  &ATK_Counter,
  &ATK_Ice_Punch,
  &ATK_Psychic,
  &ATK_Dynamic_Punch,
  &ATK_Power_Up_Punch,
  NULL
};

static const attack* CRESSELIA_ATKS[] = {
  &ATK_Psycho_Cut,
  &ATK_Confusion,
  &ATK_Moonblast,
  &ATK_Aurora_Beam,
  &ATK_Grass_Knot,
  &ATK_Future_Sight,
  NULL
};

static const attack* RIOLU_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Counter,
  &ATK_Cross_Chop,
  &ATK_Low_Sweep,
  &ATK_Thunder_Punch,
  &ATK_Brick_Break,
  &ATK_Blaze_Kick,
  NULL
};

static const attack* LUCARIO_ATKS[] = {
  &ATK_Bullet_Punch,
  &ATK_Counter,
  &ATK_Force_Palm,
  &ATK_Flash_Cannon,
  &ATK_Shadow_Ball,
  &ATK_Thunder_Punch,
  &ATK_Close_Combat,
  &ATK_Blaze_Kick,
  &ATK_Power_Up_Punch,
  &ATK_Aura_Sphere,
  NULL
};

static const attack* TOGEPI_ATKS[] = {
  &ATK_Peck,
  &ATK_Zen_Headbutt,
  &ATK_Hidden_Power,
  &ATK_Psyshock,
  &ATK_Ancient_Power,
  &ATK_Dazzling_Gleam,
  NULL
};

static const attack* TOGETIC_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Steel_Wing,
  &ATK_Extrasensory,
  &ATK_Hidden_Power,
  &ATK_Fairy_Wind,
  &ATK_Aerial_Ace,
  &ATK_Psyshock,
  &ATK_Ancient_Power,
  &ATK_Draining_Kiss,
  &ATK_Dazzling_Gleam,
  NULL
};

static const attack* TOGEKISS_ATKS[] = {
  &ATK_Air_Slash,
  &ATK_Hidden_Power,
  &ATK_Charm,
  &ATK_Flamethrower,
  &ATK_Aerial_Ace,
  &ATK_Psyshock,
  &ATK_Ancient_Power,
  &ATK_Dazzling_Gleam,
  &ATK_Aura_Sphere,
  NULL
};

static const attack* H_VOLTORB_ATKS[] = {
  &ATK_Tackle,
  &ATK_Charge_Beam,
  &ATK_Thunderbolt,
  &ATK_Swift,
  &ATK_Energy_Ball,
  NULL
};

static const attack* VOLTORB_ATKS[] = {
  &ATK_Spark,
  &ATK_Tackle,
  &ATK_Discharge,
  &ATK_Thunderbolt,
  &ATK_Signal_Beam,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* H_ELECTRODE_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Tackle,
  &ATK_Swift,
  &ATK_Wild_Charge,
  &ATK_Energy_Ball,
  NULL
};

static const attack* ELECTRODE_ATKS[] = {
  &ATK_Spark,
  &ATK_Tackle,
  &ATK_Volt_Switch,
  &ATK_Hyper_Beam,
  &ATK_Discharge,
  &ATK_Thunderbolt,
  &ATK_Foul_Play,
  NULL
};

static const attack* ELECTRIKE_ATKS[] = {
  &ATK_Spark,
  &ATK_Quick_Attack,
  &ATK_Discharge,
  &ATK_Thunderbolt,
  &ATK_Swift,
  NULL
};

static const attack* MANECTRIC_ATKS[] = {
  &ATK_Charge_Beam,
  &ATK_Snarl,
  &ATK_Thunder_Fang,
  &ATK_Thunder,
  &ATK_Flame_Burst,
  &ATK_Wild_Charge,
  &ATK_Overheat,
  &ATK_Psychic_Fangs,
  NULL
};

static const attack* MAWILE_ATKS[] = {
  &ATK_Bite,
  &ATK_Fire_Fang,
  &ATK_Astonish,
  &ATK_Ice_Fang,
  &ATK_Fairy_Wind,
  &ATK_Vise_Grip,
  &ATK_Iron_Head,
  &ATK_Play_Rough,
  &ATK_Power_Up_Punch,
  NULL
};

static const attack* JIRACHI_ATKS[] = {
  &ATK_Confusion,
  &ATK_Charge_Beam,
  &ATK_Dazzling_Gleam,
  &ATK_Psychic,
  &ATK_Doom_Desire,
  NULL
};

static const attack* RAYQUAZA_ATKS[] = {
  &ATK_Dragon_Tail,
  &ATK_Air_Slash,
  &ATK_Aerial_Ace,
  &ATK_Ancient_Power,
  &ATK_Hurricane,
  &ATK_Outrage,
  &ATK_Breaking_Swipe,
  &ATK_Dragon_Ascent,
  NULL
};

static const attack* BAGON_ATKS[] = {
  &ATK_Bite,
  &ATK_Ember,
  &ATK_Flamethrower,
  &ATK_Twister,
  &ATK_Crunch,
  NULL
};

static const attack* SHELGON_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Ember,
  &ATK_Flamethrower,
  &ATK_Twister,
  &ATK_Dragon_Pulse,
  NULL
};

static const attack* SALAMENCE_ATKS[] = {
  &ATK_Bite,
  &ATK_Fire_Fang,
  &ATK_Dragon_Tail,
  &ATK_Fire_Blast,
  &ATK_Hydro_Pump,
  &ATK_Outrage,
  &ATK_Draco_Meteor,
  &ATK_Fly,
  NULL
};

static const attack* BARBOACH_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Water_Gun,
  &ATK_Ice_Beam,
  &ATK_Aqua_Tail,
  &ATK_Mud_Bomb,
  &ATK_Scald,
  NULL
};

static const attack* WHISCASH_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Water_Gun,
  &ATK_Blizzard,
  &ATK_Mud_Bomb,
  &ATK_Water_Pulse,
  &ATK_Scald,
  NULL
};

static const attack* TURTONATOR_ATKS[] = {
  &ATK_Ember,
  &ATK_Fire_Spin,
  &ATK_Incinerate,
  &ATK_Flamethrower,
  &ATK_Flash_Cannon,
  &ATK_Dragon_Pulse,
  &ATK_Overheat,
  NULL
};

static const attack* REGIROCK_ATKS[] = {
  &ATK_Rock_Throw,
  &ATK_Rock_Smash,
  &ATK_Lock_On,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Focus_Blast,
  &ATK_Zap_Cannon,
  NULL
};

static const attack* REGICE_ATKS[] = {
  &ATK_Frost_Breath,
  &ATK_Rock_Smash,
  &ATK_Lock_On,
  &ATK_Earthquake,
  &ATK_Blizzard,
  &ATK_Thunder,
  &ATK_Focus_Blast,
  NULL
};

static const attack* REGISTEEL_ATKS[] = {
  &ATK_Metal_Claw,
  &ATK_Rock_Smash,
  &ATK_Lock_On,
  &ATK_Hyper_Beam,
  &ATK_Flash_Cannon,
  &ATK_Focus_Blast,
  &ATK_Zap_Cannon,
  NULL
};

static const attack* REGIELEKI_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Volt_Switch,
  &ATK_Lock_On,
  &ATK_Hyper_Beam,
  &ATK_Thunder,
  &ATK_Zap_Cannon,
  NULL
};

static const attack* SUICUNE_ATKS[] = {
  &ATK_Extrasensory,
  &ATK_Snarl,
  &ATK_Hidden_Power,
  &ATK_Ice_Fang,
  &ATK_Ice_Beam,
  &ATK_Bubble_Beam,
  &ATK_Water_Pulse,
  &ATK_Scald,
  &ATK_Hydro_Pump,
  NULL
};

static const attack* KYOGRE_ATKS[] = {
  &ATK_Waterfall,
  &ATK_Blizzard,
  &ATK_Thunder,
  &ATK_Hydro_Pump,
  &ATK_Surf,
  &ATK_Origin_Pulse,
  NULL
};

static const attack* GROUDON_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Dragon_Tail,
  &ATK_Earthquake,
  &ATK_Fire_Blast,
  &ATK_Fire_Punch,
  &ATK_Solar_Beam,
  &ATK_Precipice_Blades,
  NULL
};

static const attack* DARUMAKA_ATKS[] = {
  &ATK_Tackle,
  &ATK_Fire_Fang,
  &ATK_Flame_Charge,
  &ATK_Fire_Punch,
  NULL
};

static const attack* G_DARUMAKA_ATKS[] = {
  &ATK_Tackle,
  &ATK_Ice_Fang,
  &ATK_Ice_Punch,
  &ATK_Ice_Beam,
  NULL
};

static const attack* DARMANITAN_ATKS[] = {
  &ATK_Tackle,
  &ATK_Fire_Fang,
  &ATK_Incinerate,
  &ATK_Rock_Slide,
  &ATK_Psychic,
  &ATK_Focus_Blast,
  &ATK_Overheat,
  NULL
};

static const attack* G_DARMANITAN_ATKS[] = {
  &ATK_Tackle,
  &ATK_Ice_Fang,
  &ATK_Ice_Punch,
  &ATK_Avalanche,
  &ATK_Overheat,
  &ATK_Superpower,
  NULL
};

static const attack* ZACIAN_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Metal_Claw,
  &ATK_Fire_Fang,
  &ATK_Snarl,
  &ATK_Iron_Head,
  &ATK_Play_Rough,
  &ATK_Close_Combat,
  &ATK_Wild_Charge,
  NULL
};

static const attack* ZAMAZENTA_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Metal_Claw,
  &ATK_Snarl,
  &ATK_Ice_Fang,
  &ATK_Iron_Head,
  &ATK_Moonblast,
  &ATK_Close_Combat,
  &ATK_Crunch,
  NULL
};

static const attack* C_ZACIAN_ATKS[] = {
  &ATK_Metal_Claw,
  &ATK_Air_Slash,
  &ATK_Play_Rough,
  &ATK_Close_Combat,
  &ATK_Giga_Impact,
  &ATK_Behemoth_Blade,
  NULL
};

static const attack* C_ZAMAZENTA_ATKS[] = {
  &ATK_Metal_Claw,
  &ATK_Ice_Fang,
  &ATK_Moonblast,
  &ATK_Close_Combat,
  &ATK_Giga_Impact,
  &ATK_Behemoth_Bash,
  NULL
};

static const attack* ETERNATUS_ATKS[] = {
  &ATK_Dragon_Tail,
  &ATK_Poison_Jab,
  &ATK_Sludge_Bomb,
  &ATK_Dragon_Pulse,
  &ATK_Flamethrower,
  &ATK_Dynamax_Cannon,
  NULL
};

static const attack* PIPLUP_ATKS[] = {
  &ATK_Pound,
  &ATK_Bubble,
  &ATK_Drill_Peck,
  &ATK_Bubble_Beam,
  &ATK_Icy_Wind,
  NULL
};

static const attack* PRINPLUP_ATKS[] = {
  &ATK_Metal_Claw,
  &ATK_Bubble,
  &ATK_Bubble_Beam,
  &ATK_Hydro_Pump,
  &ATK_Icy_Wind,
  NULL
};

static const attack* EMPOLEON_ATKS[] = {
  &ATK_Metal_Claw,
  &ATK_Steel_Wing,
  &ATK_Waterfall,
  &ATK_Flash_Cannon,
  &ATK_Drill_Peck,
  &ATK_Blizzard,
  &ATK_Hydro_Pump,
  &ATK_Hydro_Cannon,
  NULL
};

static const attack* CARVANHA_ATKS[] = {
  &ATK_Bite,
  &ATK_Snarl,
  &ATK_Poison_Fang,
  &ATK_Aqua_Jet,
  &ATK_Crunch,
  NULL
};

static const attack* SHARPEDO_ATKS[] = {
  &ATK_Bite,
  &ATK_Waterfall,
  &ATK_Poison_Fang,
  &ATK_Hydro_Pump,
  &ATK_Crunch,
  NULL
};

static const attack* HAWLUCHA_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Poison_Jab,
  &ATK_Aerial_Ace,
  &ATK_XScissor,
  &ATK_Power_Up_Punch,
  &ATK_Flying_Press,
  NULL
};

static const attack* KUBFU_ATKS[] = {
  &ATK_Rock_Smash,
  &ATK_Counter,
  &ATK_Brick_Break,
  &ATK_Close_Combat,
  &ATK_Dynamic_Punch,
  NULL
};

static const attack* S_URSHIFU_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Rock_Smash,
  &ATK_Counter,
  &ATK_Brick_Break,
  &ATK_Close_Combat,
  &ATK_Dynamic_Punch,
  &ATK_Payback,
  NULL
};

static const attack* R_URSHIFU_ATKS[] = {
  &ATK_Rock_Smash,
  &ATK_Counter,
  &ATK_Waterfall,
  &ATK_Aqua_Jet,
  &ATK_Brick_Break,
  &ATK_Close_Combat,
  &ATK_Dynamic_Punch,
  NULL
};

static const attack* KABUTO_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Scratch,
  &ATK_Aqua_Jet,
  &ATK_Ancient_Power,
  &ATK_Rock_Tomb,
  NULL
};

static const attack* KABUTOPS_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Mud_Shot,
  &ATK_Rock_Smash,
  &ATK_Waterfall,
  &ATK_Stone_Edge,
  &ATK_Ancient_Power,
  &ATK_Water_Pulse,
  NULL
};

static const attack* LATIOS_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Zen_Headbutt,
  &ATK_Dragon_Claw,
  &ATK_Psychic,
  &ATK_Solar_Beam,
  &ATK_Luster_Purge,
  NULL
};

static const attack* LATIAS_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Zen_Headbutt,
  &ATK_Charm,
  &ATK_Thunder,
  &ATK_Psychic,
  &ATK_Outrage,
  &ATK_Mist_Ball,
  NULL
};

static const attack* DIALGA_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Metal_Claw,
  &ATK_Iron_Head,
  &ATK_Thunder,
  &ATK_Draco_Meteor,
  NULL
};

static const attack* O_DIALGA_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Metal_Claw,
  &ATK_Iron_Head,
  &ATK_Thunder,
  &ATK_Draco_Meteor,
  &ATK_Roar_of_Time,
  NULL
};

static const attack* LARVESTA_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Ember,
  &ATK_Flame_Wheel,
  &ATK_Bug_Buzz,
  &ATK_Flame_Charge,
  NULL
};

static const attack* VOLCARONA_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Fire_Spin,
  &ATK_Bug_Buzz,
  &ATK_Solar_Beam,
  &ATK_Hurricane,
  &ATK_Overheat,
  NULL
};

static const attack* COBALION_ATKS[] = {
  &ATK_Metal_Claw,
  &ATK_Zen_Headbutt,
  &ATK_Double_Kick,
  &ATK_Stone_Edge,
  &ATK_Iron_Head,
  &ATK_Close_Combat,
  &ATK_Sacred_Sword,
  NULL
};

static const attack* HEATRAN_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Fire_Spin,
  &ATK_Flamethrower,
  &ATK_Stone_Edge,
  &ATK_Iron_Head,
  &ATK_Fire_Blast,
  &ATK_Earth_Power,
  &ATK_Magma_Storm,
  NULL
};

static const attack* ROWLET_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Tackle,
  &ATK_Magical_Leaf,
  &ATK_Leafage,
  &ATK_Seed_Bomb,
  &ATK_Energy_Ball,
  NULL
};

static const attack* DARTRIX_ATKS[] = {
  &ATK_Peck,
  &ATK_Razor_Leaf,
  &ATK_Magical_Leaf,
  &ATK_Leafage,
  &ATK_Seed_Bomb,
  &ATK_Brave_Bird,
  &ATK_Energy_Ball,
  NULL
};

static const attack* DECIDUEYE_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Astonish,
  &ATK_Magical_Leaf,
  &ATK_Leafage,
  &ATK_Shadow_Sneak,
  &ATK_Brave_Bird,
  &ATK_Energy_Ball,
  &ATK_Frenzy_Plant,
  &ATK_Spirit_Shackle,
  NULL
};

static const attack* H_DECIDUEYE_ATKS[] = {
  &ATK_Psycho_Cut,
  &ATK_Magical_Leaf,
  &ATK_Aerial_Ace,
  &ATK_Night_Shade,
  &ATK_Energy_Ball,
  &ATK_Aura_Sphere,
  &ATK_Trailblaze,
  NULL
};

static const attack* ZARUDE_ATKS[] = {
  &ATK_Bite,
  &ATK_Vine_Whip,
  &ATK_Dark_Pulse,
  &ATK_Power_Whip,
  &ATK_Energy_Ball,
  NULL
};

static const attack* BELLSPROUT_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Acid,
  &ATK_Wrap,
  &ATK_Sludge_Bomb,
  &ATK_Power_Whip,
  NULL
};

static const attack* WEEPINBELL_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Acid,
  &ATK_Bullet_Seed,
  &ATK_Seed_Bomb,
  &ATK_Sludge_Bomb,
  &ATK_Power_Whip,
  NULL
};

static const attack* VICTREEBEL_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Acid,
  &ATK_Magical_Leaf,
  &ATK_Sludge_Bomb,
  &ATK_Solar_Beam,
  &ATK_Leaf_Blade,
  &ATK_Acid_Spray,
  &ATK_Leaf_Tornado,
  NULL
};

static const attack* TANDEMAUS_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Bullet_Seed,
  &ATK_Take_Down,
  &ATK_Play_Rough,
  &ATK_Swift,
  &ATK_Crunch,
  NULL
};

static const attack* MAUSHOLD_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Bullet_Seed,
  &ATK_Take_Down,
  &ATK_Play_Rough,
  &ATK_Swift,
  &ATK_Crunch,
  NULL
};

static const attack* SNIVY_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Tackle,
  &ATK_Wrap,
  &ATK_Seed_Bomb,
  &ATK_Energy_Ball,
  NULL
};

static const attack* SERVINE_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Iron_Tail,
  &ATK_Wrap,
  &ATK_Grass_Knot,
  &ATK_Leaf_Tornado,
  NULL
};

static const attack* SERPERIOR_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Iron_Tail,
  &ATK_Aerial_Ace,
  &ATK_Grass_Knot,
  &ATK_Frenzy_Plant,
  &ATK_Leaf_Tornado,
  NULL
};

static const attack* VICTINI_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Confusion,
  &ATK_Psychic,
  &ATK_Focus_Blast,
  &ATK_Overheat,
  &ATK_V_Create,
  NULL
};

static const attack* BULBASAUR_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Tackle,
  &ATK_Seed_Bomb,
  &ATK_Sludge_Bomb,
  &ATK_Power_Whip,
  NULL
};

static const attack* IVYSAUR_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Razor_Leaf,
  &ATK_Sludge_Bomb,
  &ATK_Solar_Beam,
  &ATK_Power_Whip,
  NULL
};

static const attack* VENUSAUR_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Razor_Leaf,
  &ATK_Petal_Blizzard,
  &ATK_Sludge_Bomb,
  &ATK_Solar_Beam,
  &ATK_Frenzy_Plant,
  NULL
};

static const attack* FLOETTE_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Tackle,
  &ATK_Petal_Blizzard,
  &ATK_Dazzling_Gleam,
  &ATK_Psychic,
  NULL
};

static const attack* FLABEBE_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Tackle,
  &ATK_Petal_Blizzard,
  &ATK_Dazzling_Gleam,
  &ATK_Psychic,
  NULL
};

static const attack* FLORGES_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Razor_Leaf,
  &ATK_Tackle,
  &ATK_Fairy_Wind,
  &ATK_Petal_Blizzard,
  &ATK_Disarming_Voice,
  &ATK_Moonblast,
  &ATK_Psychic,
  &ATK_Trailblaze,
  NULL
};

static const attack* SWINUB_ATKS[] = {
  &ATK_Tackle,
  &ATK_Powder_Snow,
  &ATK_Rock_Slide,
  &ATK_Icy_Wind,
  &ATK_Body_Slam,
  NULL
};

static const attack* PILOSWINE_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Powder_Snow,
  &ATK_Stone_Edge,
  &ATK_Bulldoze,
  &ATK_Avalanche,
  &ATK_Icicle_Spear,
  &ATK_High_Horsepower,
  NULL
};

static const attack* MAMOSWINE_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Powder_Snow,
  &ATK_Stone_Edge,
  &ATK_Ancient_Power,
  &ATK_Bulldoze,
  &ATK_Avalanche,
  &ATK_Icicle_Spear,
  &ATK_High_Horsepower,
  NULL
};

static const attack* ARTICUNO_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Frost_Breath,
  &ATK_Ice_Beam,
  &ATK_Blizzard,
  &ATK_Ancient_Power,
  &ATK_Icy_Wind,
  &ATK_Hurricane,
  &ATK_Triple_Axel,
  NULL
};

static const attack* G_ARTICUNO_ATKS[] = {
  &ATK_Psycho_Cut,
  &ATK_Confusion,
  &ATK_Ancient_Power,
  &ATK_Brave_Bird,
  &ATK_Future_Sight,
  &ATK_Fly,
  NULL
};

static const attack* TEDDIURSA_ATKS[] = {
  &ATK_Lick,
  &ATK_Scratch,
  &ATK_Cross_Chop,
  &ATK_Play_Rough,
  &ATK_Swift,
  &ATK_Crunch,
  &ATK_Trailblaze,
  NULL
};

static const attack* URSARING_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Metal_Claw,
  &ATK_Counter,
  &ATK_Hyper_Beam,
  &ATK_Play_Rough,
  &ATK_Swift,
  &ATK_Close_Combat,
  &ATK_Trailblaze,
  NULL
};

static const attack* URSALUNA_ATKS[] = {
  &ATK_Tackle,
  &ATK_Rock_Smash,
  &ATK_Ice_Punch,
  &ATK_Aerial_Ace,
  &ATK_Thunder_Punch,
  &ATK_Fire_Punch,
  &ATK_Swift,
  &ATK_High_Horsepower,
  &ATK_Trailblaze,
  NULL
};

static const attack* SNOVER_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Powder_Snow,
  &ATK_Leafage,
  &ATK_Ice_Beam,
  &ATK_Stomp,
  &ATK_Energy_Ball,
  NULL
};

static const attack* ABOMASNOW_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Powder_Snow,
  &ATK_Leafage,
  &ATK_Blizzard,
  &ATK_Icy_Wind,
  &ATK_Energy_Ball,
  &ATK_Outrage,
  &ATK_Weather_Ball_Ice,
  NULL
};

static const attack* CHARMANDER_ATKS[] = {
  &ATK_Ember,
  &ATK_Scratch,
  &ATK_Flamethrower,
  &ATK_Flame_Charge,
  &ATK_Flame_Burst,
  NULL
};

static const attack* CHARMELEON_ATKS[] = {
  &ATK_Ember,
  &ATK_Scratch,
  &ATK_Fire_Fang,
  &ATK_Flamethrower,
  &ATK_Flame_Burst,
  &ATK_Fire_Punch,
  NULL
};

static const attack* CHARIZARD_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Ember,
  &ATK_Wing_Attack,
  &ATK_Air_Slash,
  &ATK_Fire_Spin,
  &ATK_Flamethrower,
  &ATK_Dragon_Claw,
  &ATK_Fire_Blast,
  &ATK_Overheat,
  &ATK_Blast_Burn,
  NULL
};

static const attack* SQUIRTLE_ATKS[] = {
  &ATK_Tackle,
  &ATK_Bubble,
  &ATK_Aqua_Jet,
  &ATK_Aqua_Tail,
  &ATK_Water_Pulse,
  NULL
};

static const attack* WARTORTLE_ATKS[] = {
  &ATK_Bite,
  &ATK_Water_Gun,
  &ATK_Ice_Beam,
  &ATK_Aqua_Jet,
  &ATK_Hydro_Pump,
  NULL
};

static const attack* BLASTOISE_ATKS[] = {
  &ATK_Bite,
  &ATK_Water_Gun,
  &ATK_Rollout,
  &ATK_Flash_Cannon,
  &ATK_Ice_Beam,
  &ATK_Hydro_Pump,
  &ATK_Hydro_Cannon,
  &ATK_Skull_Bash,
  NULL
};

static const attack* SURSKIT_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Bubble,
  &ATK_Bubble_Beam,
  &ATK_Aqua_Jet,
  &ATK_Signal_Beam,
  NULL
};

static const attack* MASQUERAIN_ATKS[] = {
  &ATK_Air_Slash,
  &ATK_Infestation,
  &ATK_Bubble_Beam,
  &ATK_Ominous_Wind,
  &ATK_Air_Cutter,
  &ATK_Silver_Wind,
  &ATK_Lunge,
  NULL
};

static const attack* YANMA_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Quick_Attack,
  &ATK_Aerial_Ace,
  &ATK_Ancient_Power,
  &ATK_Silver_Wind,
  NULL
};

static const attack* YANMEGA_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Wing_Attack,
  &ATK_Aerial_Ace,
  &ATK_Bug_Buzz,
  &ATK_Ancient_Power,
  NULL
};

static const attack* NOIBAT_ATKS[] = {
  &ATK_Bite,
  &ATK_Wing_Attack,
  &ATK_Heat_Wave,
  &ATK_Dragon_Pulse,
  &ATK_Air_Cutter,
  NULL
};

static const attack* NOIVERN_ATKS[] = {
  &ATK_Bite,
  &ATK_Air_Slash,
  &ATK_Heat_Wave,
  &ATK_Psychic,
  &ATK_Hurricane,
  &ATK_Draco_Meteor,
  &ATK_Boomburst,
  NULL
};

static const attack* LEDYBA_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Tackle,
  &ATK_Aerial_Ace,
  &ATK_Swift,
  &ATK_Silver_Wind,
  NULL
};

static const attack* LEDIAN_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Counter,
  &ATK_Struggle_Bug,
  &ATK_Aerial_Ace,
  &ATK_Bug_Buzz,
  &ATK_Dynamic_Punch,
  &ATK_Silver_Wind,
  NULL
};

static const attack* SPINARAK_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Poison_Sting,
  &ATK_Night_Slash,
  &ATK_Cross_Poison,
  &ATK_Signal_Beam,
  NULL
};

static const attack* ARIADOS_ATKS[] = {
  &ATK_Poison_Sting,
  &ATK_Infestation,
  &ATK_Megahorn,
  &ATK_Shadow_Sneak,
  &ATK_Cross_Poison,
  &ATK_Lunge,
  &ATK_Trailblaze,
  NULL
};

static const attack* KRICKETOT_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Struggle_Bug,
  &ATK_Struggle,
  NULL
};

static const attack* KRICKETUNE_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Struggle_Bug,
  &ATK_Aerial_Ace,
  &ATK_Bug_Buzz,
  &ATK_XScissor,
  NULL
};

static const attack* BURMY_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Tackle,
  &ATK_Struggle,
  NULL
};

static const attack* P_WORMADAM_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Confusion,
  &ATK_Psybeam,
  &ATK_Bug_Buzz,
  &ATK_Energy_Ball,
  NULL
};

static const attack* S_WORMADAM_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Confusion,
  &ATK_Psybeam,
  &ATK_Bug_Buzz,
  &ATK_Bulldoze,
  NULL
};

static const attack* T_WORMADAM_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Confusion,
  &ATK_Metal_Sound,
  &ATK_Psybeam,
  &ATK_Bug_Buzz,
  &ATK_Iron_Head,
  NULL
};

static const attack* MOTHIM_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Air_Slash,
  &ATK_Psybeam,
  &ATK_Aerial_Ace,
  &ATK_Bug_Buzz,
  NULL
};

static const attack* STUNFISK_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Mud_Shot,
  &ATK_Discharge,
  &ATK_Mud_Bomb,
  &ATK_Muddy_Water,
  NULL
};

static const attack* G_STUNFISK_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Metal_Claw,
  &ATK_Earthquake,
  &ATK_Flash_Cannon,
  &ATK_Rock_Slide,
  &ATK_Muddy_Water,
  NULL
};

static const attack* SCATTERBUG_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Tackle,
  &ATK_Struggle,
  NULL
};

static const attack* SPEWPA_ATKS[] = {
  &ATK_Struggle_Bug,
  &ATK_Struggle,
  NULL
};

static const attack* VIVILLON_ATKS[] = {
  &ATK_Struggle_Bug,
  &ATK_Gust,
  &ATK_Aerial_Ace,
  &ATK_Bug_Buzz,
  &ATK_Hurricane,
  &ATK_Energy_Ball,
  NULL
};

static const attack* STARLY_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Tackle,
  &ATK_Sand_Attack,
  &ATK_Aerial_Ace,
  &ATK_Brave_Bird,
  &ATK_Fly,
  NULL
};

static const attack* STARAVIA_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Quick_Attack,
  &ATK_Sand_Attack,
  &ATK_Heat_Wave,
  &ATK_Aerial_Ace,
  &ATK_Brave_Bird,
  &ATK_Fly,
  NULL
};

static const attack* STARAPTOR_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Quick_Attack,
  &ATK_Gust,
  &ATK_Sand_Attack,
  &ATK_Heat_Wave,
  &ATK_Close_Combat,
  &ATK_Brave_Bird,
  &ATK_Fly,
  NULL
};

static const attack* YVELTAL_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Snarl,
  &ATK_Gust,
  &ATK_Hyper_Beam,
  &ATK_Dark_Pulse,
  &ATK_Psychic,
  &ATK_Hurricane,
  &ATK_Focus_Blast,
  &ATK_Oblivion_Wing,
  NULL
};

static const attack* CATERPIE_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Tackle,
  &ATK_Struggle,
  NULL
};

static const attack* METAPOD_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Tackle,
  &ATK_Struggle,
  NULL
};

static const attack* BUTTERFREE_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Confusion,
  &ATK_Struggle_Bug,
  &ATK_Bug_Buzz,
  &ATK_Signal_Beam,
  &ATK_Psychic,
  NULL
};

static const attack* WEEDLE_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Poison_Sting,
  &ATK_Struggle,
  NULL
};

static const attack* KAKUNA_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Poison_Sting,
  &ATK_Struggle,
  NULL
};

static const attack* BEEDRILL_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Poison_Jab,
  &ATK_Poison_Sting,
  &ATK_Infestation,
  &ATK_Aerial_Ace,
  &ATK_Drill_Run,
  &ATK_Sludge_Bomb,
  &ATK_XScissor,
  &ATK_Fell_Stinger,
  NULL
};

static const attack* PIDGEY_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Tackle,
  &ATK_Aerial_Ace,
  &ATK_Twister,
  &ATK_Air_Cutter,
  NULL
};

static const attack* PIDGEOTTO_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Steel_Wing,
  &ATK_Aerial_Ace,
  &ATK_Twister,
  &ATK_Air_Cutter,
  NULL
};

static const attack* PIDGEOT_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Steel_Wing,
  &ATK_Air_Slash,
  &ATK_Aerial_Ace,
  &ATK_Air_Cutter,
  &ATK_Hurricane,
  &ATK_Brave_Bird,
  &ATK_Feather_Dance,
  NULL
};

static const attack* MOLTRES_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Fire_Spin,
  &ATK_Heat_Wave,
  &ATK_Ancient_Power,
  &ATK_Fire_Blast,
  &ATK_Sky_Attack,
  &ATK_Overheat,
  NULL
};

static const attack* G_MOLTRES_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Wing_Attack,
  &ATK_Ancient_Power,
  &ATK_Brave_Bird,
  &ATK_Payback,
  &ATK_Fly,
  NULL
};

static const attack* ARCHEN_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Quick_Attack,
  &ATK_Ancient_Power,
  &ATK_Dragon_Claw,
  &ATK_Crunch,
  NULL
};

static const attack* ARCHEOPS_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Steel_Wing,
  &ATK_Ancient_Power,
  &ATK_Dragon_Claw,
  &ATK_Crunch,
  NULL
};

static const attack* YUNGOOS_ATKS[] = {
  &ATK_Bite,
  &ATK_Tackle,
  &ATK_Rock_Tomb,
  &ATK_Hyper_Fang,
  &ATK_Crunch,
  NULL
};

static const attack* GUMSHOOS_ATKS[] = {
  &ATK_Bite,
  &ATK_Take_Down,
  &ATK_Rock_Tomb,
  &ATK_Hyper_Fang,
  &ATK_Crunch,
  NULL
};

static const attack* RATTATA_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Tackle,
  &ATK_Dig,
  &ATK_Hyper_Fang,
  &ATK_Body_Slam,
  NULL
};

static const attack* ALOLAN_RATTATA_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Tackle,
  &ATK_Shadow_Ball,
  &ATK_Hyper_Fang,
  &ATK_Crunch,
  NULL
};

static const attack* RATICATE_ATKS[] = {
  &ATK_Bite,
  &ATK_Quick_Attack,
  &ATK_Hyper_Beam,
  &ATK_Dig,
  &ATK_Hyper_Fang,
  NULL
};

static const attack* ALOLAN_RATICATE_ATKS[] = {
  &ATK_Bite,
  &ATK_Quick_Attack,
  &ATK_Hyper_Beam,
  &ATK_Hyper_Fang,
  &ATK_Crunch,
  NULL
};

static const attack* FOONGUS_ATKS[] = {
  &ATK_Feint_Attack,
  &ATK_Astonish,
  &ATK_Body_Slam,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* AMOONGUSS_ATKS[] = {
  &ATK_Feint_Attack,
  &ATK_Astonish,
  &ATK_Sludge_Bomb,
  &ATK_Grass_Knot,
  &ATK_Foul_Play,
  NULL
};

static const attack* SHUPPET_ATKS[] = {
  &ATK_Feint_Attack,
  &ATK_Astonish,
  &ATK_Shadow_Sneak,
  &ATK_Ominous_Wind,
  &ATK_Night_Shade,
  NULL
};

static const attack* BANETTE_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Hex,
  &ATK_Shadow_Ball,
  &ATK_Thunder,
  &ATK_Dazzling_Gleam,
  NULL
};

static const attack* SHEDINJA_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Shadow_Claw,
  &ATK_Struggle_Bug,
  &ATK_Dig,
  &ATK_Aerial_Ace,
  &ATK_Shadow_Sneak,
  NULL
};

static const attack* LITWICK_ATKS[] = {
  &ATK_Ember,
  &ATK_Astonish,
  &ATK_Heat_Wave,
  &ATK_Flame_Charge,
  &ATK_Flame_Burst,
  &ATK_Mystical_Fire,
  NULL
};

static const attack* LAMPENT_ATKS[] = {
  &ATK_Ember,
  &ATK_Astonish,
  &ATK_Heat_Wave,
  &ATK_Flame_Burst,
  &ATK_Energy_Ball,
  NULL
};

static const attack* CHANDELURE_ATKS[] = {
  &ATK_Hex,
  &ATK_Fire_Spin,
  &ATK_Incinerate,
  &ATK_Shadow_Ball,
  &ATK_Flame_Charge,
  &ATK_Overheat,
  &ATK_Energy_Ball,
  &ATK_Poltergeist,
  NULL
};

static const attack* GOLETT_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Astonish,
  &ATK_Shadow_Punch,
  &ATK_Brick_Break,
  &ATK_Night_Shade,
  NULL
};

static const attack* GOLURK_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Astonish,
  &ATK_Shadow_Punch,
  &ATK_Dynamic_Punch,
  &ATK_Earth_Power,
  &ATK_Poltergeist,
  NULL
};

static const attack* DUSKULL_ATKS[] = {
  &ATK_Astonish,
  &ATK_Hex,
  &ATK_Shadow_Sneak,
  &ATK_Ominous_Wind,
  &ATK_Night_Shade,
  NULL
};

static const attack* DUSCLOPS_ATKS[] = {
  &ATK_Feint_Attack,
  &ATK_Hex,
  &ATK_Ice_Punch,
  &ATK_Shadow_Punch,
  &ATK_Fire_Punch,
  &ATK_Poltergeist,
  NULL
};

static const attack* DUSKNOIR_ATKS[] = {
  &ATK_Astonish,
  &ATK_Hex,
  &ATK_Dark_Pulse,
  &ATK_Shadow_Punch,
  &ATK_Ominous_Wind,
  &ATK_Shadow_Ball,
  &ATK_Psychic,
  &ATK_Dynamic_Punch,
  &ATK_Poltergeist,
  NULL
};

static const attack* GOURGEIST_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Hex,
  &ATK_Seed_Bomb,
  &ATK_Shadow_Punch,
  &ATK_Fire_Blast,
  &ATK_Foul_Play,
  &ATK_Poltergeist,
  NULL
};

static const attack* TROPIUS_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Air_Slash,
  &ATK_Aerial_Ace,
  &ATK_Leaf_Blade,
  &ATK_Stomp,
  &ATK_Brutal_Swing,
  NULL
};

static const attack* ABSOL_ATKS[] = {
  &ATK_Psycho_Cut,
  &ATK_Snarl,
  &ATK_Dark_Pulse,
  &ATK_Megahorn,
  &ATK_Thunder,
  &ATK_Payback,
  &ATK_Brutal_Swing,
  NULL
};

static const attack* SWABLU_ATKS[] = {
  &ATK_Peck,
  &ATK_Astonish,
  &ATK_Ice_Beam,
  &ATK_Aerial_Ace,
  &ATK_Disarming_Voice,
  NULL
};

static const attack* ALTARIA_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Peck,
  &ATK_Dragon_Pulse,
  &ATK_Dazzling_Gleam,
  &ATK_Moonblast,
  &ATK_Sky_Attack,
  NULL
};

static const attack* DIANCIE_ATKS[] = {
  &ATK_Tackle,
  &ATK_Rock_Throw,
  &ATK_Rock_Slide,
  &ATK_Power_Gem,
  &ATK_Moonblast,
  NULL
};

static const attack* SPEAROW_ATKS[] = {
  &ATK_Peck,
  &ATK_Quick_Attack,
  &ATK_Drill_Peck,
  &ATK_Aerial_Ace,
  &ATK_Twister,
  &ATK_Sky_Attack,
  NULL
};

static const attack* FEAROW_ATKS[] = {
  &ATK_Peck,
  &ATK_Steel_Wing,
  &ATK_Aerial_Ace,
  &ATK_Drill_Run,
  &ATK_Twister,
  &ATK_Sky_Attack,
  &ATK_Fly,
  NULL
};

static const attack* PIKPEK_ATKS[] = {
  &ATK_Peck,
  &ATK_Rock_Smash,
  &ATK_Drill_Peck,
  &ATK_Aerial_Ace,
  &ATK_Sky_Attack,
  NULL
};

static const attack* TRUMBEAK_ATKS[] = {
  &ATK_Peck,
  &ATK_Rock_Smash,
  &ATK_Drill_Peck,
  &ATK_Sky_Attack,
  &ATK_Rock_Blast,
  NULL
};

static const attack* TOUCANNON_ATKS[] = {
  &ATK_Peck,
  &ATK_Rock_Smash,
  &ATK_Bullet_Seed,
  &ATK_Flash_Cannon,
  &ATK_Drill_Peck,
  &ATK_Rock_Blast,
  NULL
};

static const attack* EKANS_ATKS[] = {
  &ATK_Acid,
  &ATK_Poison_Sting,
  &ATK_Wrap,
  &ATK_Poison_Fang,
  &ATK_Sludge_Bomb,
  &ATK_Gunk_Shot,
  NULL
};

static const attack* ARBOK_ATKS[] = {
  &ATK_Bite,
  &ATK_Acid,
  &ATK_Poison_Sting,
  &ATK_Dragon_Tail,
  &ATK_Dark_Pulse,
  &ATK_Sludge_Wave,
  &ATK_Gunk_Shot,
  &ATK_Acid_Spray,
  NULL
};

static const attack* PIKACHU_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Quick_Attack,
  &ATK_Present,
  &ATK_Discharge,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  &ATK_Wild_Charge,
  &ATK_Surf,
  &ATK_Volt_Tackle,
  NULL
};

static const attack* RAICHU_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Spark,
  &ATK_Volt_Switch,
  &ATK_Charm,
  &ATK_Thunder_Punch,
  &ATK_Thunder,
  &ATK_Brick_Break,
  &ATK_Wild_Charge,
  &ATK_Surf,
  &ATK_Skull_Bash,
  &ATK_Trailblaze,
  NULL
};

static const attack* ALOLAN_RAICHU_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Spark,
  &ATK_Volt_Switch,
  &ATK_Thunder_Punch,
  &ATK_Psychic,
  &ATK_Wild_Charge,
  &ATK_Grass_Knot,
  &ATK_Surf,
  &ATK_Trailblaze,
  NULL
};

static const attack* SANDSHREW_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Scratch,
  &ATK_Sand_Attack,
  &ATK_Dig,
  &ATK_Rock_Tomb,
  &ATK_Rock_Slide,
  &ATK_Sand_Tomb,
  NULL
};

static const attack* ALOLAN_SANDSHREW_ATKS[] = {
  &ATK_Metal_Claw,
  &ATK_Powder_Snow,
  &ATK_Blizzard,
  &ATK_Night_Slash,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* SANDSLASH_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Metal_Claw,
  &ATK_Sand_Attack,
  &ATK_Earthquake,
  &ATK_Night_Slash,
  &ATK_Rock_Tomb,
  &ATK_Bulldoze,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* ALOLAN_SANDSLASH_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Metal_Claw,
  &ATK_Powder_Snow,
  &ATK_Ice_Punch,
  &ATK_Blizzard,
  &ATK_Aerial_Ace,
  &ATK_Drill_Run,
  &ATK_Bulldoze,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* ENTEI_ATKS[] = {
  &ATK_Fire_Fang,
  &ATK_Fire_Spin,
  &ATK_Flamethrower,
  &ATK_Iron_Head,
  &ATK_Flame_Charge,
  &ATK_Fire_Blast,
  &ATK_Overheat,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* TRAPINCH_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Struggle_Bug,
  &ATK_Sand_Attack,
  &ATK_Dig,
  &ATK_Sand_Tomb,
  &ATK_Crunch,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* VIBRAVA_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Mud_Shot,
  &ATK_Sand_Attack,
  &ATK_Bug_Buzz,
  &ATK_Bulldoze,
  &ATK_Sand_Tomb,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* FLYGON_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Dragon_Tail,
  &ATK_Sand_Attack,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Dragon_Claw,
  &ATK_Earth_Power,
  &ATK_Boomburst,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* DIGLETT_ATKS[] = {
  &ATK_Scratch,
  &ATK_Mud_Shot,
  &ATK_Mud_Slap,
  &ATK_Dig,
  &ATK_Rock_Tomb,
  &ATK_Mud_Bomb,
  NULL
};

static const attack* ALOLAN_DIGLETT_ATKS[] = {
  &ATK_Metal_Claw,
  &ATK_Mud_Slap,
  &ATK_Sand_Attack,
  &ATK_Dig,
  &ATK_Rock_Tomb,
  &ATK_Mud_Bomb,
  NULL
};

static const attack* DUGTRIO_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Mud_Shot,
  &ATK_Mud_Slap,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Mud_Bomb,
  NULL
};

static const attack* ALOLAN_DUGTRIO_ATKS[] = {
  &ATK_Metal_Claw,
  &ATK_Mud_Slap,
  &ATK_Sand_Attack,
  &ATK_Earthquake,
  &ATK_Iron_Head,
  &ATK_Mud_Bomb,
  NULL
};

static const attack* NIDORANF_ATKS[] = {
  &ATK_Bite,
  &ATK_Poison_Sting,
  &ATK_Poison_Fang,
  &ATK_Sludge_Bomb,
  &ATK_Body_Slam,
  NULL
};

static const attack* NIDORANM_ATKS[] = {
  &ATK_Peck,
  &ATK_Poison_Sting,
  &ATK_Sludge_Bomb,
  &ATK_Horn_Attack,
  &ATK_Body_Slam,
  NULL
};

static const attack* NIDORINA_ATKS[] = {
  &ATK_Bite,
  &ATK_Poison_Sting,
  &ATK_Dig,
  &ATK_Poison_Fang,
  &ATK_Thunderbolt,
  &ATK_Sludge_Bomb,
  NULL
};

static const attack* NIDORINO_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Poison_Sting,
  &ATK_Dig,
  &ATK_Ice_Beam,
  &ATK_Sludge_Bomb,
  &ATK_Horn_Attack,
  NULL
};

static const attack* NIDOQUEEN_ATKS[] = {
  &ATK_Bite,
  &ATK_Poison_Jab,
  &ATK_Poison_Sting,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Poison_Fang,
  &ATK_Sludge_Wave,
  &ATK_Earth_Power,
  NULL
};

static const attack* NIDOKING_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Poison_Jab,
  &ATK_Poison_Sting,
  &ATK_Iron_Tail,
  &ATK_Double_Kick,
  &ATK_Megahorn,
  &ATK_Earthquake,
  &ATK_Sludge_Wave,
  &ATK_Sand_Tomb,
  &ATK_Earth_Power,
  NULL
};

static const attack* CLEFFA_ATKS[] = {
  &ATK_Pound,
  &ATK_Zen_Headbutt,
  &ATK_Psyshock,
  &ATK_Signal_Beam,
  &ATK_Psychic,
  &ATK_Swift,
  &ATK_Body_Slam,
  &ATK_Grass_Knot,
  NULL
};

static const attack* CLEFAIRY_ATKS[] = {
  &ATK_Pound,
  &ATK_Zen_Headbutt,
  &ATK_Disarming_Voice,
  &ATK_Moonblast,
  &ATK_Swift,
  &ATK_Body_Slam,
  NULL
};

static const attack* CLEFABLE_ATKS[] = {
  &ATK_Pound,
  &ATK_Zen_Headbutt,
  &ATK_Charge_Beam,
  &ATK_Charm,
  &ATK_Fairy_Wind,
  &ATK_Draining_Kiss,
  &ATK_Dazzling_Gleam,
  &ATK_Moonblast,
  &ATK_Psychic,
  &ATK_Swift,
  &ATK_Meteor_Mash,
  NULL
};

static const attack* VULPIX_ATKS[] = {
  &ATK_Ember,
  &ATK_Quick_Attack,
  &ATK_Flamethrower,
  &ATK_Flame_Charge,
  &ATK_Body_Slam,
  &ATK_Weather_Ball_Fire,
  NULL
};

static const attack* ALOLAN_VULPIX_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Powder_Snow,
  &ATK_Dark_Pulse,
  &ATK_Ice_Beam,
  &ATK_Blizzard,
  &ATK_Weather_Ball_Ice,
  NULL
};

static const attack* NINETALES_ATKS[] = {
  &ATK_Ember,
  &ATK_Feint_Attack,
  &ATK_Fire_Spin,
  &ATK_Flamethrower,
  &ATK_Heat_Wave,
  &ATK_Psyshock,
  &ATK_Fire_Blast,
  &ATK_Solar_Beam,
  &ATK_Overheat,
  &ATK_Weather_Ball_Fire,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* ALOLAN_NINETALES_ATKS[] = {
  &ATK_Feint_Attack,
  &ATK_Powder_Snow,
  &ATK_Charm,
  &ATK_Ice_Beam,
  &ATK_Blizzard,
  &ATK_Psyshock,
  &ATK_Dazzling_Gleam,
  &ATK_Weather_Ball_Ice,
  NULL
};

static const attack* JIGGLYPUFF_ATKS[] = {
  &ATK_Pound,
  &ATK_Feint_Attack,
  &ATK_Disarming_Voice,
  &ATK_Dazzling_Gleam,
  &ATK_Play_Rough,
  &ATK_Swift,
  &ATK_Body_Slam,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* WIGGLYTUFF_ATKS[] = {
  &ATK_Pound,
  &ATK_Feint_Attack,
  &ATK_Charm,
  &ATK_Hyper_Beam,
  &ATK_Ice_Beam,
  &ATK_Disarming_Voice,
  &ATK_Dazzling_Gleam,
  &ATK_Play_Rough,
  &ATK_Icy_Wind,
  &ATK_Swift,
  NULL
};

static const attack* ZUBAT_ATKS[] = {
  &ATK_Bite,
  &ATK_Quick_Attack,
  &ATK_Poison_Fang,
  &ATK_Sludge_Bomb,
  &ATK_Air_Cutter,
  &ATK_Swift,
  NULL
};

static const attack* GOLBAT_ATKS[] = {
  &ATK_Bite,
  &ATK_Wing_Attack,
  &ATK_Poison_Fang,
  &ATK_Ominous_Wind,
  &ATK_Shadow_Ball,
  &ATK_Air_Cutter,
  NULL
};

static const attack* CROBAT_ATKS[] = {
  &ATK_Bite,
  &ATK_Air_Slash,
  &ATK_Poison_Fang,
  &ATK_Shadow_Ball,
  &ATK_Cross_Poison,
  &ATK_Sludge_Bomb,
  &ATK_Air_Cutter,
  NULL
};

static const attack* ODDISH_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Acid,
  &ATK_Seed_Bomb,
  &ATK_Moonblast,
  &ATK_Sludge_Bomb,
  NULL
};

static const attack* GLOOM_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Acid,
  &ATK_Petal_Blizzard,
  &ATK_Moonblast,
  &ATK_Sludge_Bomb,
  NULL
};

static const attack* VILEPLOOM_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Acid,
  &ATK_Petal_Blizzard,
  &ATK_Moonblast,
  &ATK_Sludge_Bomb,
  &ATK_Solar_Beam,
  NULL
};

static const attack* BELLOSSOM_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Acid,
  &ATK_Bullet_Seed,
  &ATK_Magical_Leaf,
  &ATK_Petal_Blizzard,
  &ATK_Dazzling_Gleam,
  &ATK_Leaf_Blade,
  NULL
};

static const attack* VENONAT_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Confusion,
  &ATK_Psybeam,
  &ATK_Poison_Fang,
  &ATK_Signal_Beam,
  NULL
};

static const attack* VENOMOTH_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Confusion,
  &ATK_Infestation,
  &ATK_Psywave,
  &ATK_Bug_Buzz,
  &ATK_Poison_Fang,
  &ATK_Psychic,
  &ATK_Silver_Wind,
  NULL
};

static const attack* MEOWTH_ATKS[] = {
  &ATK_Bite,
  &ATK_Scratch,
  &ATK_Dark_Pulse,
  &ATK_Night_Slash,
  &ATK_Body_Slam,
  &ATK_Foul_Play,
  NULL
};

static const attack* A_MEOWTH_ATKS[] = {
  &ATK_Bite,
  &ATK_Scratch,
  &ATK_Dark_Pulse,
  &ATK_Night_Slash,
  &ATK_Foul_Play,
  &ATK_Trailblaze,
  NULL
};

static const attack* G_MEOWTH_ATKS[] = {
  &ATK_Scratch,
  &ATK_Metal_Claw,
  &ATK_Metal_Sound,
  &ATK_Dig,
  &ATK_Night_Slash,
  &ATK_Gyro_Ball,
  &ATK_Trailblaze,
  NULL
};

static const attack* PERSIAN_ATKS[] = {
  &ATK_Scratch,
  &ATK_Feint_Attack,
  &ATK_Night_Slash,
  &ATK_Power_Gem,
  &ATK_Play_Rough,
  &ATK_Foul_Play,
  &ATK_Payback,
  NULL
};

static const attack* A_PERSIAN_ATKS[] = {
  &ATK_Scratch,
  &ATK_Feint_Attack,
  &ATK_Dark_Pulse,
  &ATK_Play_Rough,
  &ATK_Foul_Play,
  &ATK_Payback,
  &ATK_Trailblaze,
  NULL
};

static const attack* PARAS_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Scratch,
  &ATK_Seed_Bomb,
  &ATK_Cross_Poison,
  &ATK_XScissor,
  NULL
};

static const attack* PARASECT_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Bug_Bite,
  &ATK_Struggle_Bug,
  &ATK_Cross_Poison,
  &ATK_XScissor,
  &ATK_Solar_Beam,
  NULL
};

static const attack* PSYDUCK_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Zen_Headbutt,
  &ATK_Cross_Chop,
  &ATK_Psybeam,
  &ATK_Aqua_Tail,
  NULL
};

static const attack* GOLDUCK_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Confusion,
  &ATK_Cross_Chop,
  &ATK_Ice_Beam,
  &ATK_Bubble_Beam,
  &ATK_Hydro_Pump,
  &ATK_Psychic,
  &ATK_Synchronoise,
  &ATK_Liquidation,
  NULL
};

static const attack* MAGBY_ATKS[] = {
  &ATK_Karate_Chop,
  &ATK_Ember,
  &ATK_Flamethrower,
  &ATK_Flame_Burst,
  &ATK_Fire_Punch,
  &ATK_Brick_Break,
  NULL
};

static const attack* MAGMAR_ATKS[] = {
  &ATK_Karate_Chop,
  &ATK_Ember,
  &ATK_Flamethrower,
  &ATK_Fire_Blast,
  &ATK_Fire_Punch,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* MAGMORTAR_ATKS[] = {
  &ATK_Karate_Chop,
  &ATK_Fire_Spin,
  &ATK_Thunderbolt,
  &ATK_Fire_Blast,
  &ATK_Psychic,
  &ATK_Fire_Punch,
  &ATK_Brick_Break,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* MANKEY_ATKS[] = {
  &ATK_Karate_Chop,
  &ATK_Scratch,
  &ATK_Cross_Chop,
  &ATK_Low_Sweep,
  &ATK_Brick_Break,
  NULL
};

static const attack* PRIMEAPE_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Karate_Chop,
  &ATK_Counter,
  &ATK_Cross_Chop,
  &ATK_Ice_Punch,
  &ATK_Night_Slash,
  &ATK_Low_Sweep,
  &ATK_Close_Combat,
  &ATK_Rage_Fist,
};

static const attack* ANNIHILAPE_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Counter,
  &ATK_Ice_Punch,
  &ATK_Night_Slash,
  &ATK_Low_Sweep,
  &ATK_Shadow_Ball,
  &ATK_Close_Combat,
  &ATK_Rage_Fist,
  NULL
};

static const attack* CLODSIRE_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Poison_Sting,
  &ATK_Megahorn,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Sludge_Bomb,
  &ATK_Water_Pulse,
  &ATK_Acid_Spray,
  NULL
};

static const attack* PINSIR_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Bug_Bite,
  &ATK_Rock_Smash,
  &ATK_Vise_Grip,
  &ATK_Submission,
  &ATK_XScissor,
  &ATK_Close_Combat,
  &ATK_Superpower,
  NULL
};

static const attack* PINECO_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Tackle,
  &ATK_Rock_Tomb,
  &ATK_Sand_Tomb,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* FORRETRESS_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Volt_Switch,
  &ATK_Struggle_Bug,
  &ATK_Earthquake,
  &ATK_Rock_Tomb,
  &ATK_Sand_Tomb,
  &ATK_Heavy_Slam,
  &ATK_Mirror_Shot,
  NULL
};

static const attack* VOLBEAT_ATKS[] = {
  &ATK_Tackle,
  &ATK_Struggle_Bug,
  &ATK_Bug_Buzz,
  &ATK_Thunderbolt,
  &ATK_Signal_Beam,
  NULL
};

static const attack* ILLUMISE_ATKS[] = {
  &ATK_Tackle,
  &ATK_Struggle_Bug,
  &ATK_Bug_Buzz,
  &ATK_Dazzling_Gleam,
  &ATK_Silver_Wind,
  NULL
};

static const attack* WURMPLE_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Tackle,
  &ATK_Struggle,
  NULL
};

static const attack* SILCOON_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Tackle,
  &ATK_Struggle,
  NULL
};

static const attack* BEAUTIFLY_ATKS[] = {
  &ATK_Infestation,
  &ATK_Struggle_Bug,
  &ATK_Bug_Buzz,
  &ATK_Air_Cutter,
  &ATK_Silver_Wind,
  NULL
};

static const attack* CASCOON_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Poison_Sting,
  &ATK_Struggle,
  NULL
};

static const attack* DUSTOX_ATKS[] = {
  &ATK_Confusion,
  &ATK_Struggle_Bug,
  &ATK_Bug_Buzz,
  &ATK_Sludge_Bomb,
  &ATK_Silver_Wind,
  NULL
};

static const attack* SPOINK_ATKS[] = {
  &ATK_Splash,
  &ATK_Zen_Headbutt,
  &ATK_Psybeam,
  &ATK_Shadow_Ball,
  &ATK_Mirror_Coat,
  NULL
};

static const attack* GRUMPIG_ATKS[] = {
  &ATK_Charge_Beam,
  &ATK_Extrasensory,
  &ATK_Psywave,
  &ATK_Psychic,
  &ATK_Shadow_Ball,
  &ATK_Dynamic_Punch,
  &ATK_Mirror_Coat,
  NULL
};

static const attack* BERGMITE_ATKS[] = {
  &ATK_Bite,
  &ATK_Tackle,
  &ATK_Icy_Wind,
  &ATK_Mirror_Coat,
  &ATK_Crunch,
  NULL
};

static const attack* AVALUGG_ATKS[] = {
  &ATK_Bite,
  &ATK_Ice_Fang,
  &ATK_Earthquake,
  &ATK_Icy_Wind,
  &ATK_Body_Slam,
  &ATK_Avalanche,
  &ATK_Mirror_Coat,
  &ATK_Crunch,
  NULL
};

static const attack* H_AVALUGG_ATKS[] = {
  &ATK_Bite,
  &ATK_Tackle,
  &ATK_Powder_Snow,
  &ATK_Blizzard,
  &ATK_Rock_Slide,
  &ATK_Icy_Wind,
  &ATK_Crunch,
  NULL
};

static const attack* FEEBAS_ATKS[] = {
  &ATK_Tackle,
  &ATK_Splash,
  &ATK_Mirror_Coat,
  NULL
};

static const attack* MILOTIC_ATKS[] = {
  &ATK_Dragon_Tail,
  &ATK_Waterfall,
  &ATK_Wrap,
  &ATK_Hyper_Beam,
  &ATK_Blizzard,
  &ATK_Surf,
  NULL
};

static const attack* LILEEP_ATKS[] = {
  &ATK_Acid,
  &ATK_Infestation,
  &ATK_Bullet_Seed,
  &ATK_Ancient_Power,
  &ATK_Grass_Knot,
  &ATK_Mirror_Coat,
  NULL
};

static const attack* CRADILY_ATKS[] = {
  &ATK_Acid,
  &ATK_Infestation,
  &ATK_Bullet_Seed,
  &ATK_Stone_Edge,
  &ATK_Rock_Tomb,
  &ATK_Rock_Slide,
  &ATK_Bulldoze,
  &ATK_Grass_Knot,
  NULL
};

static const attack* JOLTIK_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Charge_Beam,
  &ATK_Discharge,
  &ATK_Bug_Buzz,
  &ATK_Cross_Poison,
  NULL
};

static const attack* GRAVANTULA_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Volt_Switch,
  &ATK_Discharge,
  &ATK_Bug_Buzz,
  &ATK_Cross_Poison,
  &ATK_Energy_Ball,
  &ATK_Lunge,
  NULL
};

static const attack* CUTIEFLY_ATKS[] = {
  &ATK_Struggle_Bug,
  &ATK_Fairy_Wind,
  &ATK_Bug_Buzz,
  &ATK_Dazzling_Gleam,
  NULL
};

static const attack* RIBOMBEE_ATKS[] = {
  &ATK_Struggle_Bug,
  &ATK_Charm,
  &ATK_Fairy_Wind,
  &ATK_Bug_Buzz,
  &ATK_Draining_Kiss,
  &ATK_Dazzling_Gleam,
  NULL
};

static const attack* HERACROSS_ATKS[] = {
  &ATK_Counter,
  &ATK_Struggle_Bug,
  &ATK_Megahorn,
  &ATK_Earthquake,
  &ATK_Close_Combat,
  &ATK_Rock_Blast,
  &ATK_Upper_Hand,
  NULL
};

static const attack* KARRABLAST_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Peck,
  &ATK_Aerial_Ace,
  &ATK_Drill_Run,
  &ATK_Signal_Beam,
  &ATK_XScissor,
  NULL
};

static const attack* ESCAVALIER_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Counter,
  &ATK_Megahorn,
  &ATK_Aerial_Ace,
  &ATK_Drill_Run,
  &ATK_Acid_Spray,
  &ATK_Razor_Shell,
  NULL
};

static const attack* SHELMET_ATKS[] = {
  &ATK_Acid,
  &ATK_Infestation,
  &ATK_Bug_Buzz,
  &ATK_Signal_Beam,
  &ATK_Body_Slam,
  NULL
};

static const attack* ACCELGOR_ATKS[] = {
  &ATK_Acid,
  &ATK_Infestation,
  &ATK_Water_Shuriken,
  &ATK_Bug_Buzz,
  &ATK_Signal_Beam,
  &ATK_Focus_Blast,
  &ATK_Energy_Ball,
  &ATK_Acid_Spray,
  NULL
};

static const attack* ABRA_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Charge_Beam,
  &ATK_Psyshock,
  &ATK_Shadow_Ball,
  &ATK_Signal_Beam,
  NULL
};

static const attack* KADABRA_ATKS[] = {
  &ATK_Psycho_Cut,
  &ATK_Confusion,
  &ATK_Psybeam,
  &ATK_Shadow_Ball,
  &ATK_Dazzling_Gleam,
  NULL
};

static const attack* ALAKAZAM_ATKS[] = {
  &ATK_Psycho_Cut,
  &ATK_Confusion,
  &ATK_Counter,
  &ATK_Shadow_Ball,
  &ATK_Dazzling_Gleam,
  &ATK_Psychic,
  &ATK_Fire_Punch,
  &ATK_Focus_Blast,
  &ATK_Future_Sight,
  NULL
};

static const attack* STUFFUL_ATKS[] = {
  &ATK_Tackle,
  &ATK_Take_Down,
  &ATK_Brick_Break,
  &ATK_Stomp,
  &ATK_Superpower,
  NULL
};

static const attack* BEWEAR_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Shadow_Claw,
  &ATK_Tackle,
  &ATK_Stomp,
  &ATK_Superpower,
  &ATK_Drain_Punch,
  &ATK_Payback,
  NULL
};

static const attack* MACHOP_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Karate_Chop,
  &ATK_Rock_Smash,
  &ATK_Cross_Chop,
  &ATK_Low_Sweep,
  &ATK_Brick_Break,
  NULL
};

static const attack* MACHOKE_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Karate_Chop,
  &ATK_Cross_Chop,
  &ATK_Submission,
  &ATK_Brick_Break,
  &ATK_Dynamic_Punch,
  NULL
};

static const attack* MACHAMP_ATKS[] = {
  &ATK_Karate_Chop,
  &ATK_Bullet_Punch,
  &ATK_Counter,
  &ATK_Cross_Chop,
  &ATK_Stone_Edge,
  &ATK_Submission,
  &ATK_Rock_Slide,
  &ATK_Close_Combat,
  &ATK_Dynamic_Punch,
  &ATK_Heavy_Slam,
  &ATK_Payback,
  NULL
};

static const attack* BUNEARY_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Pound,
  &ATK_Fire_Punch,
  &ATK_Swift,
  NULL
};

static const attack* LOPUNNY_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Pound,
  &ATK_Double_Kick,
  &ATK_Hyper_Beam,
  &ATK_Fire_Punch,
  &ATK_Focus_Blast,
  &ATK_Triple_Axel,
  NULL
};

static const attack* HOUNDOUR_ATKS[] = {
  &ATK_Ember,
  &ATK_Feint_Attack,
  &ATK_Dark_Pulse,
  &ATK_Flamethrower,
  &ATK_Crunch,
  NULL
};

static const attack* HOUNDOOM_ATKS[] = {
  &ATK_Fire_Fang,
  &ATK_Snarl,
  &ATK_Flamethrower,
  &ATK_Fire_Blast,
  &ATK_Crunch,
  &ATK_Foul_Play,
  NULL
};

static const attack* TREECKO_ATKS[] = {
  &ATK_Pound,
  &ATK_Bullet_Seed,
  &ATK_Aerial_Ace,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* GROVYLE_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Bullet_Seed,
  &ATK_Aerial_Ace,
  &ATK_Leaf_Blade,
  &ATK_Grass_Knot,
  NULL
};

static const attack* SCEPTILE_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Bullet_Seed,
  &ATK_Earthquake,
  &ATK_Aerial_Ace,
  &ATK_Dragon_Claw,
  &ATK_Leaf_Blade,
  &ATK_Frenzy_Plant,
  &ATK_Breaking_Swipe,
  NULL
};

static const attack* SLUGMA_ATKS[] = {
  &ATK_Ember,
  &ATK_Rock_Throw,
  &ATK_Rock_Slide,
  &ATK_Flame_Charge,
  &ATK_Flame_Burst,
  NULL
};

static const attack* MAGCARGO_ATKS[] = {
  &ATK_Ember,
  &ATK_Rock_Throw,
  &ATK_Incinerate,
  &ATK_Stone_Edge,
  &ATK_Heat_Wave,
  &ATK_Rock_Tomb,
  &ATK_Overheat,
  NULL
};

static const attack* CHARCADET_ATKS[] = {
  &ATK_Ember,
  &ATK_Incinerate,
  &ATK_Flamethrower,
  &ATK_Heat_Wave,
  &ATK_Flame_Charge,
  NULL
};

static const attack* CERULEDGE_ATKS[] = {
  &ATK_Ember,
  &ATK_Incinerate,
  &ATK_Flamethrower,
  &ATK_Heat_Wave,
  &ATK_Shadow_Ball,
  &ATK_Flame_Charge,
  NULL
};

static const attack* ARMAROUGE_ATKS[] = {
  &ATK_Ember,
  &ATK_Incinerate,
  &ATK_Flamethrower,
  &ATK_Heat_Wave,
  &ATK_Psyshock,
  &ATK_Flame_Charge,
  NULL
};

static const attack* TANGELA_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Infestation,
  &ATK_Sludge_Bomb,
  &ATK_Solar_Beam,
  &ATK_Power_Whip,
  &ATK_Grass_Knot,
  NULL
};

static const attack* TANGROWTH_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Infestation,
  &ATK_Ancient_Power,
  &ATK_Rock_Slide,
  &ATK_Sludge_Bomb,
  &ATK_Solar_Beam,
  &ATK_Power_Whip,
  NULL
};

static const attack* TENTACOOL_ATKS[] = {
  &ATK_Poison_Sting,
  &ATK_Bubble,
  &ATK_Wrap,
  &ATK_Bubble_Beam,
  &ATK_Water_Pulse,
  NULL
};

static const attack* TENTACRUEL_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Acid,
  &ATK_Poison_Sting,
  &ATK_Blizzard,
  &ATK_Sludge_Wave,
  &ATK_Scald,
  &ATK_Hydro_Pump,
  &ATK_Acid_Spray,
  NULL
};

static const attack* GROWLITHE_ATKS[] = {
  &ATK_Bite,
  &ATK_Ember,
  &ATK_Flame_Wheel,
  &ATK_Flamethrower,
  &ATK_Body_Slam,
  NULL
};

static const attack* H_GROWLITHE_ATKS[] = {
  &ATK_Bite,
  &ATK_Ember,
  &ATK_Flamethrower,
  &ATK_Rock_Slide,
  &ATK_Crunch,
  NULL
};

static const attack* ARCANINE_ATKS[] = {
  &ATK_Bite,
  &ATK_Fire_Fang,
  &ATK_Snarl,
  &ATK_Thunder_Fang,
  &ATK_Flamethrower,
  &ATK_Bulldoze,
  &ATK_Fire_Blast,
  &ATK_Wild_Charge,
  &ATK_Crunch,
  &ATK_Psychic_Fangs,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* H_ARCANINE_ATKS[] = {
  &ATK_Fire_Fang,
  &ATK_Rock_Smash,
  &ATK_Snarl,
  &ATK_Flamethrower,
  &ATK_Rock_Slide,
  &ATK_Wild_Charge,
  &ATK_Crunch,
  NULL
};

static const attack* FUECOCO_ATKS[] = {
  &ATK_Bite,
  &ATK_Incinerate,
  &ATK_Flamethrower,
  &ATK_Disarming_Voice,
  &ATK_Crunch,
  NULL
};

static const attack* CROCALOR_ATKS[] = {
  &ATK_Bite,
  &ATK_Incinerate,
  &ATK_Flamethrower,
  &ATK_Disarming_Voice,
  &ATK_Crunch,
  NULL
};

static const attack* SKELEDIRGE_ATKS[] = {
  &ATK_Bite,
  &ATK_Hex,
  &ATK_Incinerate,
  &ATK_Flamethrower,
  &ATK_Shadow_Ball,
  &ATK_Disarming_Voice,
  &ATK_Crunch,
  &ATK_Blast_Burn,
  &ATK_Torch_Song,
  NULL
};

static const attack* MEW_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Frost_Breath,
  &ATK_Pound,
  &ATK_Cut,
  &ATK_Poison_Jab,
  &ATK_Steel_Wing,
  &ATK_Rock_Smash,
  &ATK_Charge_Beam,
  &ATK_Volt_Switch,
  &ATK_Dragon_Tail,
  &ATK_Infestation,
  &ATK_Struggle_Bug,
  &ATK_Snarl,
  &ATK_Hyper_Beam,
  &ATK_Dark_Pulse,
  &ATK_Stone_Edge,
  &ATK_Flash_Cannon,
  &ATK_Ice_Beam,
  &ATK_Blizzard,
  &ATK_Low_Sweep,
  &ATK_Psyshock,
  &ATK_Ancient_Power,
  &ATK_Rock_Slide,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  &ATK_Dragon_Claw,
  &ATK_Dazzling_Gleam,
  &ATK_Bulldoze,
  &ATK_Flame_Charge,
  &ATK_Psychic,
  &ATK_Solar_Beam,
  &ATK_Focus_Blast,
  &ATK_Wild_Charge,
  &ATK_Gyro_Ball,
  &ATK_Overheat,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  &ATK_Surf,
  NULL
};

static const attack* GASTLY_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Lick,
  &ATK_Astonish,
  &ATK_Dark_Pulse,
  &ATK_Ominous_Wind,
  &ATK_Sludge_Bomb,
  &ATK_Night_Shade,
  NULL
};

static const attack* HAUNTER_ATKS[] = {
  &ATK_Lick,
  &ATK_Shadow_Claw,
  &ATK_Astonish,
  &ATK_Dark_Pulse,
  &ATK_Ice_Punch,
  &ATK_Shadow_Punch,
  &ATK_Shadow_Ball,
  &ATK_Sludge_Bomb,
  NULL
};

static const attack* GENGAR_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Lick,
  &ATK_Shadow_Claw,
  &ATK_Hex,
  &ATK_Dark_Pulse,
  &ATK_Shadow_Punch,
  &ATK_Shadow_Ball,
  &ATK_Sludge_Bomb,
  &ATK_Sludge_Wave,
  &ATK_Psychic,
  &ATK_Focus_Blast,
  NULL
};

static const attack* POLIWAG_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Bubble,
  &ATK_Bubble_Beam,
  &ATK_Mud_Bomb,
  &ATK_Body_Slam,
  NULL
};

static const attack* POLIWHIRL_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Bubble,
  &ATK_Bubble_Beam,
  &ATK_Mud_Bomb,
  &ATK_Water_Pulse,
  &ATK_Scald,
  NULL
};

static const attack* POLIWRATH_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Bubble,
  &ATK_Rock_Smash,
  &ATK_Counter,
  &ATK_Ice_Punch,
  &ATK_Submission,
  &ATK_Scald,
  &ATK_Hydro_Pump,
  &ATK_Icy_Wind,
  &ATK_Dynamic_Punch,
  &ATK_Power_Up_Punch,
  NULL
};

static const attack* POLITOED_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Bubble,
  &ATK_Earthquake,
  &ATK_Ice_Beam,
  &ATK_Blizzard,
  &ATK_Scald,
  &ATK_Hydro_Pump,
  &ATK_Surf,
  &ATK_Weather_Ball_Water,
  NULL
};

static const attack* KRABBY_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Bubble,
  &ATK_Vise_Grip,
  &ATK_Bubble_Beam,
  &ATK_Water_Pulse,
  &ATK_Razor_Shell,
  NULL
};

static const attack* KINGLER_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Metal_Claw,
  &ATK_Bubble,
  &ATK_Vise_Grip,
  &ATK_XScissor,
  &ATK_Water_Pulse,
  &ATK_Crabhammer,
  &ATK_Razor_Shell,
  NULL
};

static const attack* CUBONE_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Rock_Smash,
  &ATK_Dig,
  &ATK_Bone_Club,
  &ATK_Bulldoze,
  NULL
};

static const attack* MAROWAK_ATKS[] = {
  &ATK_Mud_Slap,
  &ATK_Rock_Smash,
  &ATK_Dig,
  &ATK_Earthquake,
  &ATK_Rock_Slide,
  &ATK_Bone_Club,
  NULL
};

static const attack* A_MAROWAK_ATKS[] = {
  &ATK_Rock_Smash,
  &ATK_Hex,
  &ATK_Fire_Spin,
  &ATK_Flame_Wheel,
  &ATK_Shadow_Ball,
  &ATK_Bone_Club,
  &ATK_Fire_Blast,
  &ATK_Shadow_Bone,
  NULL
};

static const attack* LICKITUNG_ATKS[] = {
  &ATK_Lick,
  &ATK_Zen_Headbutt,
  &ATK_Wrap,
  &ATK_Hyper_Beam,
  &ATK_Power_Whip,
  &ATK_Stomp,
  &ATK_Body_Slam,
  NULL
};

static const attack* SPRIGATITO_ATKS[] = {
  &ATK_Charm,
  &ATK_Leafage,
  &ATK_Play_Rough,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* FLORAGATO_ATKS[] = {
  &ATK_Charm,
  &ATK_Leafage,
  &ATK_Play_Rough,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* MEOWSCARADA_ATKS[] = {
  &ATK_Charm,
  &ATK_Leafage,
  &ATK_Night_Slash,
  &ATK_Play_Rough,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  &ATK_Frenzy_Plant,
  &ATK_Flower_Trick,
  NULL
};

static const attack* DRATINI_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Iron_Tail,
  &ATK_Wrap,
  &ATK_Aqua_Tail,
  &ATK_Twister,
  NULL
};

static const attack* DRAGONAIR_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Iron_Tail,
  &ATK_Wrap,
  &ATK_Aqua_Tail,
  &ATK_Dragon_Pulse,
  &ATK_Body_Slam,
  NULL
};

static const attack* DRAGONITE_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Steel_Wing,
  &ATK_Dragon_Tail,
  &ATK_Hyper_Beam,
  &ATK_Dragon_Pulse,
  &ATK_Dragon_Claw,
  &ATK_Hurricane,
  &ATK_Outrage,
  &ATK_Draco_Meteor,
  &ATK_Superpower,
  NULL
};

static const attack* RESHIRAM_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Fire_Fang,
  &ATK_Stone_Edge,
  &ATK_Overheat,
  &ATK_Crunch,
  &ATK_Draco_Meteor,
  &ATK_Fusion_Flare,
  NULL
};

static const attack* KYUREM_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Steel_Wing,
  &ATK_Blizzard,
  &ATK_Dragon_Claw,
  &ATK_Draco_Meteor,
  &ATK_Glaciate,
  NULL
};

static const attack* W_KYUREM_ATKS[] = {
  &ATK_Dragon_Breath,
  &ATK_Steel_Wing,
  &ATK_Ice_Fang,
  &ATK_Blizzard,
  &ATK_Ancient_Power,
  &ATK_Dragon_Pulse,
  &ATK_Focus_Blast,
  &ATK_Fusion_Flare,
  &ATK_Ice_Burn,
  NULL
};

static const attack* B_KYUREM_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Dragon_Tail,
  &ATK_Stone_Edge,
  &ATK_Blizzard,
  &ATK_Iron_Head,
  &ATK_Outrage,
  &ATK_Fusion_Bolt,
  &ATK_Freeze_Shock,
  NULL
};

static const attack* TERRAKION_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Smack_Down,
  &ATK_Double_Kick,
  &ATK_Earthquake,
  &ATK_Rock_Slide,
  &ATK_Close_Combat,
  &ATK_Sacred_Sword,
  NULL
};

static const attack* FROAKIE_ATKS[] = {
  &ATK_Pound,
  &ATK_Bubble,
  &ATK_Aerial_Ace,
  &ATK_Water_Pulse,
  &ATK_Surf,
  NULL
};

static const attack* FROGADIER_ATKS[] = {
  &ATK_Pound,
  &ATK_Bubble,
  &ATK_Aerial_Ace,
  &ATK_Water_Pulse,
  &ATK_Surf,
  NULL
};

static const attack* GRENINJA_ATKS[] = {
  &ATK_Bubble,
  &ATK_Feint_Attack,
  &ATK_Water_Shuriken,
  &ATK_Aerial_Ace,
  &ATK_Night_Slash,
  &ATK_Hydro_Pump,
  &ATK_Surf,
  &ATK_Hydro_Cannon,
  NULL
};

static const attack* MAGIKARP_ATKS[] = {
  &ATK_Splash,
  &ATK_Struggle,
  NULL
};

static const attack* AMPHAROS_ATKS[] = {
  &ATK_Charge_Beam,
  &ATK_Volt_Switch,
  &ATK_Power_Gem,
  &ATK_Thunder_Punch,
  &ATK_Thunder,
  &ATK_Dragon_Pulse,
  &ATK_Focus_Blast,
  &ATK_Zap_Cannon,
  &ATK_Brutal_Swing,
  &ATK_Trailblaze,
  NULL
};

static const attack* AERODACTYL_ATKS[] = {
  &ATK_Bite,
  &ATK_Rock_Throw,
  &ATK_Steel_Wing,
  &ATK_Hyper_Beam,
  &ATK_Ancient_Power,
  &ATK_Rock_Slide,
  &ATK_Iron_Head,
  &ATK_Earth_Power,
  NULL
};

static const attack* GIBLE_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Take_Down,
  &ATK_Dig,
  &ATK_Twister,
  &ATK_Body_Slam,
  NULL
};

static const attack* GABITE_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Take_Down,
  &ATK_Flamethrower,
  &ATK_Dig,
  &ATK_Twister,
  NULL
};

static const attack* GARCHOMP_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Dragon_Tail,
  &ATK_Earthquake,
  &ATK_Fire_Blast,
  &ATK_Sand_Tomb,
  &ATK_Outrage,
  &ATK_Earth_Power,
  NULL
};

static const attack* GYARADOS_ATKS[] = {
  &ATK_Bite,
  &ATK_Dragon_Breath,
  &ATK_Dragon_Tail,
  &ATK_Waterfall,
  &ATK_Aqua_Tail,
  &ATK_Twister,
  &ATK_Dragon_Pulse,
  &ATK_Hydro_Pump,
  &ATK_Outrage,
  &ATK_Crunch,
  NULL
};

static const attack* LAPRAS_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Frost_Breath,
  &ATK_Water_Gun,
  &ATK_Psywave,
  &ATK_Ice_Beam,
  &ATK_Blizzard,
  &ATK_Dragon_Pulse,
  &ATK_Hydro_Pump,
  &ATK_Surf,
  &ATK_Skull_Bash,
  &ATK_Sparkling_Aria,
  NULL
};

static const attack* CURSOLA_ATKS[] = {
  &ATK_Astonish,
  &ATK_Hex,
  &ATK_Shadow_Ball,
  &ATK_Brine,
  &ATK_Rock_Blast,
  &ATK_Night_Shade,
  NULL
};

static const attack* G_CORSOLA_ATKS[] = {
  &ATK_Tackle,
  &ATK_Astonish,
  &ATK_Power_Gem,
  &ATK_Rock_Blast,
  &ATK_Night_Shade,
  NULL
};

static const attack* CORSOLA_ATKS[] = {
  &ATK_Tackle,
  &ATK_Bubble,
  &ATK_Bubble_Beam,
  &ATK_Power_Gem,
  &ATK_Rock_Blast,
  NULL
};

static const attack* FRILLISH_ATKS[] = {
  &ATK_Bubble,
  &ATK_Hex,
  &ATK_Ice_Beam,
  &ATK_Ominous_Wind,
  &ATK_Night_Shade,
  NULL
};

static const attack* JELLICENT_ATKS[] = {
  &ATK_Bubble,
  &ATK_Hex,
  &ATK_Ice_Beam,
  &ATK_Bubble_Beam,
  &ATK_Shadow_Ball,
  &ATK_Surf,
  NULL
};

static const attack* HOOTHOOT_ATKS[] = {
  &ATK_Peck,
  &ATK_Feint_Attack,
  &ATK_Aerial_Ace,
  &ATK_Sky_Attack,
  &ATK_Night_Shade,
  NULL
};

static const attack* NOCTOWL_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Extrasensory,
  &ATK_Shadow_Ball,
  &ATK_Psychic,
  &ATK_Sky_Attack,
  &ATK_Night_Shade,
  NULL
};

static const attack* SNORUNT_ATKS[] = {
  &ATK_Powder_Snow,
  &ATK_Hex,
  &ATK_Shadow_Ball,
  &ATK_Icy_Wind,
  &ATK_Avalanche,
  NULL
};

static const attack* FROSLASS_ATKS[] = {
  &ATK_Powder_Snow,
  &ATK_Hex,
  &ATK_Shadow_Ball,
  &ATK_Avalanche,
  &ATK_Crunch,
  &ATK_Triple_Axel,
  NULL
};

static const attack* GLALIE_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Frost_Breath,
  &ATK_Rollout,
  &ATK_Shadow_Ball,
  &ATK_Avalanche,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* CETODDLE_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Tackle,
  &ATK_Powder_Snow,
  &ATK_Body_Slam,
  &ATK_Avalanche,
  &ATK_Heavy_Slam,
  NULL
};

static const attack* CETITAN_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Tackle,
  &ATK_Powder_Snow,
  &ATK_Body_Slam,
  &ATK_Avalanche,
  &ATK_Heavy_Slam,
  NULL
};

static const attack* CHESPIN_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Take_Down,
  &ATK_Seed_Bomb,
  &ATK_Body_Slam,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* QUILLADIN_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Vine_Whip,
  &ATK_Body_Slam,
  &ATK_Gyro_Ball,
  &ATK_Energy_Ball,
  NULL
};

static const attack* CHESNAUGHT_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Vine_Whip,
  &ATK_Smack_Down,
  &ATK_Thunder_Punch,
  &ATK_Solar_Beam,
  &ATK_Gyro_Ball,
  &ATK_Energy_Ball,
  &ATK_Frenzy_Plant,
  &ATK_Superpower,
  NULL
};

static const attack* MILTANK_ATKS[] = {
  &ATK_Tackle,
  &ATK_Zen_Headbutt,
  &ATK_Rollout,
  &ATK_Ice_Beam,
  &ATK_Thunderbolt,
  &ATK_Stomp,
  &ATK_Body_Slam,
  &ATK_Gyro_Ball,
  NULL
};

static const attack* MANTYKE_ATKS[] = {
  &ATK_Tackle,
  &ATK_Bubble,
  &ATK_Ice_Beam,
  &ATK_Aerial_Ace,
  &ATK_Water_Pulse,
  NULL
};

static const attack* MANTINE_ATKS[] = {
  &ATK_Wing_Attack,
  &ATK_Bubble,
  &ATK_Bullet_Seed,
  &ATK_Ice_Beam,
  &ATK_Aerial_Ace,
  &ATK_Bubble_Beam,
  &ATK_Water_Pulse,
  NULL
};

static const attack* CARNIVINE_ATKS[] = {
  &ATK_Bite,
  &ATK_Vine_Whip,
  &ATK_Power_Whip,
  &ATK_Energy_Ball,
  &ATK_Crunch,
  NULL
};

static const attack* LICKILICKY_ATKS[] = {
  &ATK_Lick,
  &ATK_Zen_Headbutt,
  &ATK_Rollout,
  &ATK_Hyper_Beam,
  &ATK_Earthquake,
  &ATK_Shadow_Ball,
  &ATK_Solar_Beam,
  &ATK_Body_Slam,
  NULL
};

static const attack* MUNCHLAX_ATKS[] = {
  &ATK_Lick,
  &ATK_Tackle,
  &ATK_Gunk_Shot,
  &ATK_Bulldoze,
  &ATK_Heavy_Slam,
  NULL
};

static const attack* SNORLAX_ATKS[] = {
  &ATK_Lick,
  &ATK_Zen_Headbutt,
  &ATK_Yawn,
  &ATK_Hyper_Beam,
  &ATK_Earthquake,
  &ATK_Body_Slam,
  &ATK_Heavy_Slam,
  &ATK_Outrage,
  &ATK_Skull_Bash,
  &ATK_Superpower,
  NULL
};

static const attack* HAPPINY_ATKS[] = {
  &ATK_Pound,
  &ATK_Zen_Headbutt,
  &ATK_Psychic,
  NULL
};

static const attack* CHANSEY_ATKS[] = {
  &ATK_Pound,
  &ATK_Zen_Headbutt,
  &ATK_Hyper_Beam,
  &ATK_Dazzling_Gleam,
  &ATK_Psychic,
  NULL
};

static const attack* BLISSEY_ATKS[] = {
  &ATK_Pound,
  &ATK_Zen_Headbutt,
  &ATK_Hyper_Beam,
  &ATK_Dazzling_Gleam,
  &ATK_Psychic,
  &ATK_Wild_Charge,
  NULL
};

static const attack* KANGASKHAN_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Mud_Slap,
  &ATK_Earthquake,
  &ATK_Brick_Break,
  &ATK_Stomp,
  &ATK_Outrage,
  &ATK_Crunch,
  &ATK_Power_Up_Punch,
  NULL
};

static const attack* BUZZWOLE_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Counter,
  &ATK_Lunge,
  &ATK_Superpower,
  &ATK_Fell_Stinger,
  &ATK_Power_Up_Punch,
  NULL
};

static const attack* DRIFLOON_ATKS[] = {
  &ATK_Astonish,
  &ATK_Hex,
  &ATK_Ominous_Wind,
  &ATK_Shadow_Ball,
  &ATK_Icy_Wind,
  NULL
};

static const attack* DRIFBLIM_ATKS[] = {
  &ATK_Astonish,
  &ATK_Hex,
  &ATK_Ominous_Wind,
  &ATK_Shadow_Ball,
  &ATK_Icy_Wind,
  &ATK_Mystical_Fire,
  NULL
};

static const attack* SPIRITOMB_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Feint_Attack,
  &ATK_Rock_Tomb,
  &ATK_Shadow_Sneak,
  &ATK_Ominous_Wind,
  &ATK_Shadow_Ball,
  NULL
};

static const attack* AIPOM_ATKS[] = {
  &ATK_Scratch,
  &ATK_Astonish,
  &ATK_Aerial_Ace,
  &ATK_Low_Sweep,
  &ATK_Swift,
  NULL
};

static const attack* AMBIPOM_ATKS[] = {
  &ATK_Scratch,
  &ATK_Astonish,
  &ATK_Hyper_Beam,
  &ATK_Aerial_Ace,
  &ATK_Low_Sweep,
  NULL
};

static const attack* I_LANDORUS_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Rock_Throw,
  &ATK_Rock_Slide,
  &ATK_Focus_Blast,
  &ATK_Outrage,
  &ATK_Earth_Power,
  NULL
};

static const attack* I_THUNDURUS_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Astonish,
  &ATK_Thunder_Punch,
  &ATK_Thunder,
  &ATK_Brick_Break,
  &ATK_Crunch,
  NULL
};

static const attack* T_LANDORUS_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Extrasensory,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Bulldoze,
  &ATK_Superpower,
  &ATK_Sandsear_Storm,
  NULL
};

static const attack* T_THUNDURUS_ATKS[] = {
  &ATK_Bite,
  &ATK_Volt_Switch,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  &ATK_Sludge_Wave,
  &ATK_Focus_Blast,
  &ATK_Wildbolt_Storm,
  NULL
};

static const attack* I_TORNADUS_ATKS[] = {
  &ATK_Bite,
  &ATK_Air_Slash,
  &ATK_Hyper_Beam,
  &ATK_Dark_Pulse,
  &ATK_Hurricane,
  &ATK_Grass_Knot,
  NULL
};

static const attack* T_TORNADUS_ATKS[] = {
  &ATK_Astonish,
  &ATK_Gust,
  &ATK_Heat_Wave,
  &ATK_Psychic,
  &ATK_Hurricane,
  &ATK_Focus_Blast,
  &ATK_Bleakwind_Storm,
  NULL
};

static const attack* B_ORICORIO_ATKS[] = {
  &ATK_Pound,
  &ATK_Air_Slash,
  &ATK_Aerial_Ace,
  &ATK_Air_Cutter,
  &ATK_Hurricane,
  NULL
};

static const attack* PS_ORICORIO_ATKS[] = {
  &ATK_Pound,
  &ATK_Air_Slash,
  &ATK_Aerial_Ace,
  &ATK_Air_Cutter,
  &ATK_Hurricane,
  NULL
};

static const attack* S_ORICORIO_ATKS[] = {
  &ATK_Pound,
  &ATK_Air_Slash,
  &ATK_Aerial_Ace,
  &ATK_Air_Cutter,
  &ATK_Hurricane,
  NULL
};

static const attack* P_ORICORIO_ATKS[] = {
  &ATK_Pound,
  &ATK_Air_Slash,
  &ATK_Aerial_Ace,
  &ATK_Air_Cutter,
  &ATK_Hurricane,
  NULL
};

static const attack* EEVEE_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Tackle,
  &ATK_Dig,
  &ATK_Swift,
  &ATK_Body_Slam,
  &ATK_Last_Resort,
  NULL
};

static const attack* JOLTEON_ATKS[] = {
  &ATK_Thunder_Shock,
  &ATK_Volt_Switch,
  &ATK_Discharge,
  &ATK_Thunder,
  &ATK_Thunderbolt,
  &ATK_Zap_Cannon,
  &ATK_Last_Resort,
  NULL
};

static const attack* GLACEON_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Frost_Breath,
  &ATK_Ice_Beam,
  &ATK_Water_Pulse,
  &ATK_Icy_Wind,
  &ATK_Avalanche,
  &ATK_Last_Resort,
  NULL
};

static const attack* ESPEON_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Confusion,
  &ATK_Psybeam,
  &ATK_Shadow_Ball,
  &ATK_Psychic,
  &ATK_Future_Sight,
  &ATK_Last_Resort,
  &ATK_Psychic_Fangs,
  NULL
};

static const attack* GREAVARD_ATKS[] = {
  &ATK_Bite,
  &ATK_Lick,
  &ATK_Dig,
  &ATK_Shadow_Ball,
  &ATK_Psychic_Fangs,
  NULL
};

static const attack* HOUNDSTONE_ATKS[] = {
  &ATK_Bite,
  &ATK_Lick,
  &ATK_Dig,
  &ATK_Shadow_Ball,
  &ATK_Psychic_Fangs,
  NULL
};

static const attack* SINISTEA_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Astonish,
  &ATK_Dark_Pulse,
  &ATK_Shadow_Ball,
  &ATK_Psychic,
  NULL
};

static const attack* POLTEAGEIST_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Astonish,
  &ATK_Hex,
  &ATK_Dark_Pulse,
  &ATK_Shadow_Ball,
  &ATK_Psychic,
  NULL
};

static const attack* MIMEJR_ATKS[] = {
  &ATK_Pound,
  &ATK_Confusion,
  &ATK_Psybeam,
  &ATK_Psyshock,
  &ATK_Psychic,
  NULL
};

static const attack* MRMIME_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Confusion,
  &ATK_Magical_Leaf,
  &ATK_Psywave,
  &ATK_Psybeam,
  &ATK_Shadow_Ball,
  &ATK_Psychic,
  NULL
};

static const attack* G_MRMIME_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Confusion,
  &ATK_Psybeam,
  &ATK_Ice_Punch,
  &ATK_Psychic,
  &ATK_Triple_Axel,
  NULL
};

static const attack* MRRIME_ATKS[] = {
  &ATK_Ice_Shard,
  &ATK_Zen_Headbutt,
  &ATK_Confusion,
  &ATK_Psybeam,
  &ATK_Ice_Punch,
  &ATK_Psychic,
  &ATK_Icy_Wind,
  &ATK_Triple_Axel,
  NULL
};

static const attack* BIDOOF_ATKS[] = {
  &ATK_Tackle,
  &ATK_Take_Down,
  &ATK_Ice_Beam,
  &ATK_Shadow_Ball,
  &ATK_Thunderbolt,
  &ATK_Hyper_Fang,
  &ATK_Grass_Knot,
  &ATK_Crunch,
  &ATK_Superpower,
  NULL
};

static const attack* BIBAREL_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Take_Down,
  &ATK_Rollout,
  &ATK_Hyper_Beam,
  &ATK_Hyper_Fang,
  &ATK_Surf,
  NULL
};

static const attack* LEAFEON_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Quick_Attack,
  &ATK_Bullet_Seed,
  &ATK_Solar_Beam,
  &ATK_Leaf_Blade,
  &ATK_Energy_Ball,
  &ATK_Last_Resort,
  NULL
};

static const attack* SYLVEON_ATKS[] = {
  &ATK_Quick_Attack,
  &ATK_Charm,
  &ATK_Psyshock,
  &ATK_Draining_Kiss,
  &ATK_Dazzling_Gleam,
  &ATK_Moonblast,
  &ATK_Last_Resort,
  NULL
};

static const attack* VAPOREON_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Aqua_Tail,
  &ATK_Water_Pulse,
  &ATK_Scald,
  &ATK_Hydro_Pump,
  &ATK_Last_Resort,
  &ATK_Liquidation,
  NULL
};

static const attack* FLAREON_ATKS[] = {
  &ATK_Ember,
  &ATK_Fire_Spin,
  &ATK_Flamethrower,
  &ATK_Heat_Wave,
  &ATK_Fire_Blast,
  &ATK_Overheat,
  &ATK_Last_Resort,
  &ATK_Superpower,
  NULL
};

static const attack* UMBREON_ATKS[] = {
  &ATK_Feint_Attack,
  &ATK_Snarl,
  &ATK_Dark_Pulse,
  &ATK_Psychic,
  &ATK_Foul_Play,
  &ATK_Last_Resort,
  NULL
};

static const attack* PHANTUMP_ATKS[] = {
  &ATK_Tackle,
  &ATK_Astonish,
  &ATK_Seed_Bomb,
  &ATK_Shadow_Ball,
  &ATK_Foul_Play,
  NULL
};

static const attack* TREVENANT_ATKS[] = {
  &ATK_Sucker_Punch,
  &ATK_Shadow_Claw,
  &ATK_Seed_Bomb,
  &ATK_Shadow_Ball,
  &ATK_Foul_Play,
  NULL
};

static const attack* PUMPKABOO_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Astonish,
  &ATK_Shadow_Sneak,
  &ATK_Grass_Knot,
  &ATK_Foul_Play,
  NULL
};

static const attack* WOOPER_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Water_Gun,
  &ATK_Dig,
  &ATK_Mud_Bomb,
  &ATK_Body_Slam,
  NULL
};

static const attack* P_WOOPER_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Poison_Jab,
  &ATK_Dig,
  &ATK_Sludge_Wave,
  &ATK_Body_Slam,
  NULL
};

static const attack* QUAGSIRE_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Water_Gun,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Aqua_Tail,
  &ATK_Sludge_Bomb,
  &ATK_Mud_Bomb,
  &ATK_Acid_Spray,
  NULL
};

static const attack* SKWOVET_ATKS[] = {
  &ATK_Bite,
  &ATK_Tackle,
  &ATK_Bullet_Seed,
  &ATK_Body_Slam,
  &ATK_Crunch,
  &ATK_Trailblaze,
  NULL
};

static const attack* GREEDENT_ATKS[] = {
  &ATK_Bite,
  &ATK_Mud_Shot,
  &ATK_Tackle,
  &ATK_Bullet_Seed,
  &ATK_Body_Slam,
  &ATK_Crunch,
  &ATK_Trailblaze,
  NULL
};

static const attack* WOOLOO_ATKS[] = {
  &ATK_Tackle,
  &ATK_Take_Down,
  &ATK_Stomp,
  &ATK_Wild_Charge,
  &ATK_Payback,
  NULL
};

static const attack* DUBWOOL_ATKS[] = {
  &ATK_Tackle,
  &ATK_Take_Down,
  &ATK_Double_Kick,
  &ATK_Body_Slam,
  &ATK_Wild_Charge,
  &ATK_Payback,
  NULL
};

static const attack* GROOKEY_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Scratch,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* THWACKEY_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Scratch,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* RILLABOOM_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Scratch,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  &ATK_Earth_Power,
  NULL
};

static const attack* TURTWIG_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Tackle,
  &ATK_Seed_Bomb,
  &ATK_Body_Slam,
  &ATK_Energy_Ball,
  NULL
};

static const attack* GROTLE_ATKS[] = {
  &ATK_Bite,
  &ATK_Razor_Leaf,
  &ATK_Solar_Beam,
  &ATK_Body_Slam,
  &ATK_Energy_Ball,
  NULL
};

static const attack* TORTERRA_ATKS[] = {
  &ATK_Bite,
  &ATK_Razor_Leaf,
  &ATK_Earthquake,
  &ATK_Stone_Edge,
  &ATK_Solar_Beam,
  &ATK_Sand_Tomb,
  &ATK_Frenzy_Plant,
  NULL
};

static const attack* SHROOMISH_ATKS[] = {
  &ATK_Tackle,
  &ATK_Bullet_Seed,
  &ATK_Seed_Bomb,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* BRELOOM_ATKS[] = {
  &ATK_Counter,
  &ATK_Bullet_Seed,
  &ATK_Force_Palm,
  &ATK_Seed_Bomb,
  &ATK_Sludge_Bomb,
  &ATK_Dynamic_Punch,
  &ATK_Grass_Knot,
  NULL
};

static const attack* LITLEO_ATKS[] = {
  &ATK_Ember,
  &ATK_Tackle,
  &ATK_Fire_Fang,
  &ATK_Incinerate,
  &ATK_Flamethrower,
  &ATK_Flame_Charge,
  &ATK_Crunch,
  NULL
};

static const attack* PYROAR_ATKS[] = {
  &ATK_Ember,
  &ATK_Fire_Fang,
  &ATK_Take_Down,
  &ATK_Incinerate,
  &ATK_Dark_Pulse,
  &ATK_Flame_Charge,
  &ATK_Solar_Beam,
  &ATK_Overheat,
  NULL
};

static const attack* SCORBUNNY_ATKS[] = {
  &ATK_Tackle,
  &ATK_Fire_Spin,
  &ATK_Flamethrower,
  &ATK_Flame_Charge,
  NULL
};

static const attack* RABOOT_ATKS[] = {
  &ATK_Tackle,
  &ATK_Fire_Spin,
  &ATK_Flamethrower,
  &ATK_Flame_Charge,
  NULL
};

static const attack* CINDERACE_ATKS[] = {
  &ATK_Tackle,
  &ATK_Fire_Spin,
  &ATK_Flamethrower,
  &ATK_Flame_Charge,
  &ATK_Focus_Blast,
  NULL
};

static const attack* ZIGZAGOON_ATKS[] = {
  &ATK_Tackle,
  &ATK_Rock_Smash,
  &ATK_Sand_Attack,
  &ATK_Dig,
  &ATK_Thunderbolt,
  &ATK_Grass_Knot,
  NULL
};

static const attack* G_ZIGZAGOON_ATKS[] = {
  &ATK_Tackle,
  &ATK_Take_Down,
  &ATK_Dig,
  &ATK_Swift,
  &ATK_Body_Slam,
  NULL
};

static const attack* LINOONE_ATKS[] = {
  &ATK_Shadow_Claw,
  &ATK_Tackle,
  &ATK_Sand_Attack,
  &ATK_Dig,
  &ATK_Thunder,
  &ATK_Swift,
  &ATK_Grass_Knot,
  NULL
};

static const attack* G_LINOONE_ATKS[] = {
  &ATK_Lick,
  &ATK_Snarl,
  &ATK_Dig,
  &ATK_Gunk_Shot,
  &ATK_Body_Slam,
  NULL
};

static const attack* OBSTAGOON_ATKS[] = {
  &ATK_Lick,
  &ATK_Counter,
  &ATK_Hyper_Beam,
  &ATK_Cross_Chop,
  &ATK_Night_Slash,
  &ATK_Gunk_Shot,
  &ATK_Obstruct,
  NULL
};

static const attack* LILLIPUP_ATKS[] = {
  &ATK_Tackle,
  &ATK_Take_Down,
  &ATK_Sand_Attack,
  &ATK_Dig,
  &ATK_Rock_Tomb,
  &ATK_Thunderbolt,
  NULL
};

static const attack* HERDIER_ATKS[] = {
  &ATK_Lick,
  &ATK_Take_Down,
  &ATK_Sand_Attack,
  &ATK_Dig,
  &ATK_Thunderbolt,
  &ATK_Play_Rough,
  NULL
};

static const attack* STOUTLAND_ATKS[] = {
  &ATK_Lick,
  &ATK_Take_Down,
  &ATK_Ice_Fang,
  &ATK_Sand_Attack,
  &ATK_Play_Rough,
  &ATK_Wild_Charge,
  &ATK_Crunch,
  NULL
};

static const attack* SKITTY_ATKS[] = {
  &ATK_Tackle,
  &ATK_Feint_Attack,
  &ATK_Dig,
  &ATK_Disarming_Voice,
  &ATK_Wild_Charge,
  NULL
};

static const attack* DELCATTY_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Feint_Attack,
  &ATK_Charm,
  &ATK_Disarming_Voice,
  &ATK_Play_Rough,
  &ATK_Wild_Charge,
  NULL
};

static const attack* HOPPIP_ATKS[] = {
  &ATK_Tackle,
  &ATK_Bullet_Seed,
  &ATK_Seed_Bomb,
  &ATK_Dazzling_Gleam,
  &ATK_Grass_Knot,
  NULL
};

static const attack* SKIPLOOM_ATKS[] = {
  &ATK_Tackle,
  &ATK_Bullet_Seed,
  &ATK_Dazzling_Gleam,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* JUMPLUFF_ATKS[] = {
  &ATK_Infestation,
  &ATK_Bullet_Seed,
  &ATK_Fairy_Wind,
  &ATK_Aerial_Ace,
  &ATK_Dazzling_Gleam,
  &ATK_Solar_Beam,
  &ATK_Energy_Ball,
  &ATK_Acrobatics,
  NULL
};

static const attack* DODUO_ATKS[] = {
  &ATK_Peck,
  &ATK_Quick_Attack,
  &ATK_Drill_Peck,
  &ATK_Aerial_Ace,
  &ATK_Swift,
  &ATK_Brave_Bird,
  NULL
};

static const attack* DODRIO_ATKS[] = {
  &ATK_Feint_Attack,
  &ATK_Steel_Wing,
  &ATK_Drill_Peck,
  &ATK_Aerial_Ace,
  &ATK_Air_Cutter,
  &ATK_Brave_Bird,
  NULL
};

static const attack* WOOBAT_ATKS[] = {
  &ATK_Confusion,
  &ATK_Air_Slash,
  &ATK_Aerial_Ace,
  &ATK_Psyshock,
  &ATK_Air_Cutter,
  NULL
};

static const attack* SWOOBAT_ATKS[] = {
  &ATK_Confusion,
  &ATK_Air_Slash,
  &ATK_Aerial_Ace,
  &ATK_Psychic,
  &ATK_Future_Sight,
  &ATK_Fly,
  &ATK_Psychic_Fangs,
  NULL
};

static const attack* SUNKERN_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Cut,
  &ATK_Seed_Bomb,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* SUNFLORA_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Bullet_Seed,
  &ATK_Petal_Blizzard,
  &ATK_Sludge_Bomb,
  &ATK_Solar_Beam,
  &ATK_Leaf_Storm,
  NULL
};

static const attack* CELEBI_ATKS[] = {
  &ATK_Confusion,
  &ATK_Charge_Beam,
  &ATK_Magical_Leaf,
  &ATK_Hyper_Beam,
  &ATK_Seed_Bomb,
  &ATK_Dazzling_Gleam,
  &ATK_Psychic,
  &ATK_Leaf_Storm,
  NULL
};

static const attack* CHIKORITA_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Tackle,
  &ATK_Magical_Leaf,
  &ATK_Body_Slam,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* BAYLEEF_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Tackle,
  &ATK_Magical_Leaf,
  &ATK_Ancient_Power,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* MEGANIUM_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Razor_Leaf,
  &ATK_Magical_Leaf,
  &ATK_Earthquake,
  &ATK_Petal_Blizzard,
  &ATK_Solar_Beam,
  &ATK_Frenzy_Plant,
  NULL
};

static const attack* GOSSIFLEUR_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Bullet_Seed,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* ELDEGOSS_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Bullet_Seed,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* PATRAT_ATKS[] = {
  &ATK_Bite,
  &ATK_Tackle,
  &ATK_Dig,
  &ATK_Hyper_Fang,
  &ATK_Grass_Knot,
  NULL
};

static const attack* WATCHOG_ATKS[] = {
  &ATK_Bite,
  &ATK_Low_Kick,
  &ATK_Hyper_Fang,
  &ATK_Grass_Knot,
  &ATK_Crunch,
  NULL
};

static const attack* GULPIN_ATKS[] = {
  &ATK_Pound,
  &ATK_Rock_Smash,
  &ATK_Sludge,
  &ATK_Ice_Beam,
  &ATK_Gunk_Shot,
  NULL
};

static const attack* SWALOT_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Rock_Smash,
  &ATK_Infestation,
  &ATK_Sludge,
  &ATK_Ice_Beam,
  &ATK_Sludge_Bomb,
  &ATK_Gunk_Shot,
  &ATK_Acid_Spray,
  NULL
};

static const attack* TOXEL_ATKS[] = {
  &ATK_Acid,
  &ATK_Power_Up_Punch,
  NULL
};

static const attack* TOXTRICITY_ATKS[] = {
  &ATK_Spark,
  &ATK_Poison_Jab,
  &ATK_Acid,
  &ATK_Discharge,
  &ATK_Wild_Charge,
  &ATK_Acid_Spray,
  &ATK_Power_Up_Punch,
  NULL
};

static const attack* CHIMCHAR_ATKS[] = {
  &ATK_Ember,
  &ATK_Scratch,
  &ATK_Flame_Wheel,
  &ATK_Flamethrower,
  &ATK_Flame_Charge,
  NULL
};

static const attack* MONFERNO_ATKS[] = {
  &ATK_Ember,
  &ATK_Rock_Smash,
  &ATK_Flame_Wheel,
  &ATK_Flamethrower,
  &ATK_Low_Sweep,
  NULL
};

static const attack* INFERNAPE_ATKS[] = {
  &ATK_Rock_Smash,
  &ATK_Fire_Spin,
  &ATK_Flamethrower,
  &ATK_Solar_Beam,
  &ATK_Close_Combat,
  &ATK_Blast_Burn,
  NULL
};

static const attack* PETILIL_ATKS[] = {
  &ATK_Hidden_Power,
  &ATK_Charm,
  &ATK_Seed_Bomb,
  &ATK_Energy_Ball,
  NULL
};

static const attack* LILLIGANT_ATKS[] = {
  &ATK_Hidden_Power,
  &ATK_Charm,
  &ATK_Magical_Leaf,
  &ATK_Hyper_Beam,
  &ATK_Petal_Blizzard,
  &ATK_Solar_Beam,
  NULL
};

static const attack* FENNEKIN_ATKS[] = {
  &ATK_Ember,
  &ATK_Scratch,
  &ATK_Flamethrower,
  &ATK_Psyshock,
  &ATK_Flame_Charge,
  NULL
};

static const attack* BRAIXEN_ATKS[] = {
  &ATK_Ember,
  &ATK_Scratch,
  &ATK_Flamethrower,
  &ATK_Psyshock,
  &ATK_Flame_Charge,
  NULL
};

static const attack* DELPHOX_ATKS[] = {
  &ATK_Scratch,
  &ATK_Zen_Headbutt,
  &ATK_Fire_Spin,
  &ATK_Flamethrower,
  &ATK_Flame_Charge,
  &ATK_Fire_Blast,
  &ATK_Psychic,
  &ATK_Blast_Burn,
  &ATK_Mystical_Fire,
  NULL
};

static const attack* LITTEN_ATKS[] = {
  &ATK_Ember,
  &ATK_Scratch,
  &ATK_Flamethrower,
  &ATK_Flame_Charge,
  &ATK_Crunch,
  NULL
};

static const attack* TORRACAT_ATKS[] = {
  &ATK_Bite,
  &ATK_Ember,
  &ATK_Flamethrower,
  &ATK_Flame_Charge,
  &ATK_Crunch,
  NULL
};

static const attack* INCINEROAR_ATKS[] = {
  &ATK_Fire_Fang,
  &ATK_Snarl,
  &ATK_Double_Kick,
  &ATK_Dark_Pulse,
  &ATK_Flame_Charge,
  &ATK_Fire_Blast,
  &ATK_Blast_Burn,
  &ATK_Blaze_Kick,
  &ATK_Darkest_Lariat,
  NULL
};

static const attack* PONYTA_ATKS[] = {
  &ATK_Ember,
  &ATK_Tackle,
  &ATK_Flame_Wheel,
  &ATK_Flame_Charge,
  &ATK_Fire_Blast,
  &ATK_Stomp,
  NULL
};

static const attack* G_PONYTA_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Psycho_Cut,
  &ATK_Psybeam,
  &ATK_Play_Rough,
  &ATK_Swift,
  NULL
};

static const attack* RAPIDASH_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Ember,
  &ATK_Fire_Spin,
  &ATK_Incinerate,
  &ATK_Heat_Wave,
  &ATK_Drill_Run,
  &ATK_Flame_Charge,
  &ATK_Fire_Blast,
  &ATK_Wild_Charge,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* G_RAPIDASH_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Psycho_Cut,
  &ATK_Fairy_Wind,
  &ATK_Megahorn,
  &ATK_Play_Rough,
  &ATK_Psychic,
  &ATK_Body_Slam,
  &ATK_Wild_Charge,
  &ATK_High_Horsepower,
  NULL
};

static const attack* STANTLER_ATKS[] = {
  &ATK_Tackle,
  &ATK_Zen_Headbutt,
  &ATK_Megahorn,
  &ATK_Stomp,
  &ATK_Wild_Charge,
  NULL
};

static const attack* SMEARGLE_ATKS[] = {
  &ATK_Splash,
  &ATK_Struggle,
  NULL
};

static const attack* SANDYGAST_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Astonish,
  &ATK_Sand_Attack,
  &ATK_Shadow_Ball,
  &ATK_Sand_Tomb,
  &ATK_Earth_Power,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* PALOSSAND_ATKS[] = {
  &ATK_Mud_Shot,
  &ATK_Astonish,
  &ATK_Sand_Attack,
  &ATK_Shadow_Ball,
  &ATK_Sand_Tomb,
  &ATK_Earth_Power,
  &ATK_Scorching_Sands,
  NULL
};

static const attack* SLOWPOKE_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Confusion,
  &ATK_Psyshock,
  &ATK_Water_Pulse,
  &ATK_Psychic,
  NULL
};

static const attack* SLOWBRO_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Confusion,
  &ATK_Ice_Beam,
  &ATK_Water_Pulse,
  &ATK_Scald,
  &ATK_Psychic,
  &ATK_Surf,
  NULL
};

static const attack* G_SLOWPOKE_ATKS[] = {
  &ATK_Confusion,
  &ATK_Iron_Tail,
  &ATK_Psyshock,
  &ATK_Psychic,
  &ATK_Surf,
  NULL
};

static const attack* G_SLOWBRO_ATKS[] = {
  &ATK_Poison_Jab,
  &ATK_Confusion,
  &ATK_Sludge_Bomb,
  &ATK_Scald,
  &ATK_Psychic,
  &ATK_Focus_Blast,
  &ATK_Surf,
  &ATK_Brutal_Swing,
  NULL
};

static const attack* SLOWKING_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Confusion,
  &ATK_Blizzard,
  &ATK_Fire_Blast,
  &ATK_Scald,
  &ATK_Psychic,
  &ATK_Surf,
  NULL
};

static const attack* G_SLOWKING_ATKS[] = {
  &ATK_Acid,
  &ATK_Confusion,
  &ATK_Hex,
  &ATK_Shadow_Ball,
  &ATK_Sludge_Wave,
  &ATK_Scald,
  &ATK_Future_Sight,
  &ATK_Surf,
  NULL
};

static const attack* DROWZEE_ATKS[] = {
  &ATK_Pound,
  &ATK_Confusion,
  &ATK_Psybeam,
  &ATK_Psyshock,
  &ATK_Psychic,
  NULL
};

static const attack* HYPNO_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Confusion,
  &ATK_Ice_Punch,
  &ATK_Psyshock,
  &ATK_Shadow_Ball,
  &ATK_Thunder_Punch,
  &ATK_Psychic,
  &ATK_Fire_Punch,
  &ATK_Focus_Blast,
  &ATK_Future_Sight,
  NULL
};

static const attack* C_HOOPA_ATKS[] = {
  &ATK_Confusion,
  &ATK_Astonish,
  &ATK_Psybeam,
  &ATK_Shadow_Ball,
  &ATK_Psychic,
  NULL
};

static const attack* U_HOOPA_ATKS[] = {
  &ATK_Confusion,
  &ATK_Astonish,
  &ATK_Dark_Pulse,
  &ATK_Shadow_Ball,
  &ATK_Psychic,
  NULL
};

static const attack* VOLCANION_ATKS[] = {
  &ATK_Water_Gun,
  &ATK_Take_Down,
  &ATK_Incinerate,
  &ATK_Sludge_Bomb,
  &ATK_Hydro_Pump,
  &ATK_Overheat,
  &ATK_Earth_Power,
  NULL
};

static const attack* THROH_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Zen_Headbutt,
  &ATK_Low_Sweep,
  &ATK_Brick_Break,
  &ATK_Body_Slam,
  &ATK_Focus_Blast,
  NULL
};

static const attack* SAWK_ATKS[] = {
  &ATK_Low_Kick,
  &ATK_Poison_Jab,
  &ATK_Low_Sweep,
  &ATK_Brick_Break,
  &ATK_Body_Slam,
  &ATK_Focus_Blast,
  NULL
};

static const attack* SKIDDO_ATKS[] = {
  &ATK_Zen_Headbutt,
  &ATK_Rock_Smash,
  &ATK_Seed_Bomb,
  &ATK_Brick_Break,
  NULL
};

static const attack* GOGOAT_ATKS[] = {
  &ATK_Vine_Whip,
  &ATK_Zen_Headbutt,
  &ATK_Rock_Smash,
  &ATK_Seed_Bomb,
  &ATK_Leaf_Blade,
  &ATK_Brick_Break,
  NULL
};

static const attack* GRUBBIN_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Spark,
  &ATK_Vise_Grip,
  &ATK_XScissor,
  &ATK_Crunch,
  NULL
};

static const attack* CHARJABUG_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Spark,
  &ATK_Volt_Switch,
  &ATK_Discharge,
  &ATK_XScissor,
  &ATK_Crunch,
  NULL
};

static const attack* VIKAVOLT_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Spark,
  &ATK_Mud_Slap,
  &ATK_Volt_Switch,
  &ATK_Discharge,
  &ATK_XScissor,
  &ATK_Crunch,
  &ATK_Fly,
  NULL
};

static const attack* NYMBLE_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Struggle_Bug,
  &ATK_Bug_Buzz,
  &ATK_XScissor,
  &ATK_Trailblaze,
  NULL
};

static const attack* LECHONK_ATKS[] = {
  &ATK_Tackle,
  &ATK_Take_Down,
  &ATK_Dig,
  &ATK_Body_Slam,
  &ATK_Trailblaze,
  NULL
};

static const attack* F_OINKOLOGNE_ATKS[] = {
  &ATK_Tackle,
  &ATK_Take_Down,
  &ATK_Dig,
  &ATK_Body_Slam,
  &ATK_Trailblaze,
  NULL
};

static const attack* M_OINKOLOGNE_ATKS[] = {
  &ATK_Tackle,
  &ATK_Take_Down,
  &ATK_Dig,
  &ATK_Body_Slam,
  &ATK_Trailblaze,
  NULL
};

static const attack* SIZZLIPEDE_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Ember,
  &ATK_Heat_Wave,
  &ATK_Bug_Buzz,
  &ATK_Crunch,
  NULL
};

static const attack* CENTISKORCH_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Ember,
  &ATK_Heat_Wave,
  &ATK_Bug_Buzz,
  &ATK_Crunch,
  &ATK_Lunge,
  NULL
};

static const attack* LOKIX_ATKS[] = {
  &ATK_Bug_Bite,
  &ATK_Sucker_Punch,
  &ATK_Counter,
  &ATK_Dark_Pulse,
  &ATK_Bug_Buzz,
  &ATK_Trailblaze,
  NULL
};

static const attack* FOMANTIS_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Razor_Leaf,
  &ATK_Leafage,
  &ATK_Leaf_Blade,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  &ATK_Trailblaze,
  NULL
};

static const attack* LURANTIS_ATKS[] = {
  &ATK_Fury_Cutter,
  &ATK_Razor_Leaf,
  &ATK_Leafage,
  &ATK_XScissor,
  &ATK_Leaf_Blade,
  &ATK_Superpower,
  &ATK_Leaf_Storm,
  &ATK_Trailblaze,
  NULL
};

static const attack* COTTONEE_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Charm,
  &ATK_Seed_Bomb,
  &ATK_Grass_Knot,
  &ATK_Energy_Ball,
  NULL
};

static const attack* WHIMSICOTT_ATKS[] = {
  &ATK_Razor_Leaf,
  &ATK_Charm,
  &ATK_Fairy_Wind,
  &ATK_Seed_Bomb,
  &ATK_Moonblast,
  &ATK_Hurricane,
  &ATK_Grass_Knot,
  NULL
};

static inline float
type_effectiveness_mult(int te){
  if(te < -3 || te > 2){
    throw std::invalid_argument("bad type effectiveness");
  }
  static const float pow16[6] = { 0.244, 0.390625, 0.625, 1, 1.6, 2.56 };
  return pow16[te + 3];
}

// calculate type relation of at on dt0 + dt1
static inline float
type_effectiveness(pgo_types_e at, pgo_types_e dt0, pgo_types_e dt1){
  return type_effectiveness_mult(typing_relation(at, dt0, dt1));
}

typedef struct species {
  unsigned idx; // pokedex index, not unique
  std::string name;
  pgo_types_e t1, t2;
  unsigned atk;
  unsigned def;
  unsigned sta;
  const char *from;    // from what does it evolve? NULL for nothing
  const attack** attacks;  // array of attack indices this form can learn
  bool shiny;         // is there a shiny form?
  bool shadow;        // is there a shadow form?
  std::vector<const attack*> elite; // exclusive attacks requiring an elite tm
  enum species_cat {
    CAT_NORMAL,
    CAT_MYTHICAL,
    CAT_LEGENDARY,
    CAT_ULTRABEAST,
    CAT_BABY,
  } category;
  int a2cost;         // cost in kStardust to teach second attack {-1, 10, 50, 75, 100}
  const char *gmax;   // gmax attack name or NULL

  species() {
  }

  species(const std::string& s)
      : name(s) {
  }

  species(unsigned i, const char *n, pgo_types_e T1, pgo_types_e T2,
          unsigned A, unsigned D, unsigned S, const char *From,
          const attack **Attacks, bool Shiny, bool Shadow,
          const std::vector<const attack*>& Elite,
          species_cat Category, int A2Cost, const char *GMax)
    : idx(i),
    name(n),
    t1(T1),
    t2(T2),
    atk(A),
    def(D),
    sta(S),
    from(From),
    attacks(Attacks),
    shiny(Shiny),
    shadow(Shadow),
    elite(Elite),
    category(Category),
    a2cost(A2Cost),
    gmax(GMax)
  { }

  // effectiveness of attack a on our typing
  float type_effectiveness(const attack *a) const {
    return ::type_effectiveness(a->type, t1, t2);
  }

  const char *categorystr() const {
    switch(category){
      case CAT_NORMAL: return "";
      case CAT_MYTHICAL: return " Mythical";
      case CAT_LEGENDARY: return " Legendary";
      case CAT_ULTRABEAST: return " Ultra Beast";
      case CAT_BABY: return " Baby";
      default: throw std::exception();
    }
  }

} species;

static const species sdex[] = {
  {    1, "Bulbasaur", TYPE_GRASS, TYPE_POISON, 118, 111, 128, NULL, BULBASAUR_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {    2, "Ivysaur", TYPE_GRASS, TYPE_POISON, 151, 143, 155, "Bulbasaur", IVYSAUR_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {    3, "Venusaur", TYPE_GRASS, TYPE_POISON, 198, 189, 190, "Ivysaur", VENUSAUR_ATKS, true, true,
    { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, },
  {    4, "Charmander", TYPE_FIRE, TYPECOUNT, 116, 93, 118, NULL, CHARMANDER_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {    5, "Charmeleon", TYPE_FIRE, TYPECOUNT, 158, 126, 151, "Charmander", CHARMELEON_ATKS, true, false,
    { &ATK_Scratch, }, species::CAT_NORMAL, 10, nullptr, },
  {    6, "Charizard", TYPE_FIRE, TYPE_FLYING, 223, 173, 186, "Charmeleon", CHARIZARD_ATKS, true, false,
    { &ATK_Dragon_Breath, &ATK_Ember, &ATK_Wing_Attack, &ATK_Blast_Burn, &ATK_Flamethrower, }, species::CAT_NORMAL, 10, nullptr, },
  {    7, "Squirtle", TYPE_WATER, TYPECOUNT, 94, 121, 127, NULL, SQUIRTLE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {    8, "Wartortle", TYPE_WATER, TYPECOUNT, 126, 155, 153, "Squirtle", WARTORTLE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {    9, "Blastoise", TYPE_WATER, TYPECOUNT, 171, 207, 188, "Wartortle", BLASTOISE_ATKS, true, true,
    { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, },
  {   10, "Caterpie", TYPE_BUG, TYPECOUNT, 55, 55, 128, NULL, CATERPIE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   11, "Metapod", TYPE_BUG, TYPECOUNT, 45, 80, 137, "Caterpie", METAPOD_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   12, "Butterfree", TYPE_BUG, TYPE_FLYING, 167, 137, 155, "Metapod", BUTTERFREE_ATKS, true, true,
    { &ATK_Bug_Bite, }, species::CAT_NORMAL, 10, nullptr, },
  {   13, "Weedle", TYPE_BUG, TYPE_POISON, 63, 50, 120, NULL, WEEDLE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   14, "Kakuna", TYPE_BUG, TYPE_POISON, 46, 75, 128, "Weedle", KAKUNA_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   15, "Beedrill", TYPE_BUG, TYPE_POISON, 169, 130, 163, "Kakuna", BEEDRILL_ATKS, true, true,
    { &ATK_Bug_Bite, &ATK_Drill_Run, }, species::CAT_NORMAL, 10, nullptr, },
  {   16, "Pidgey", TYPE_NORMAL, TYPE_FLYING, 85, 73, 120, NULL, PIDGEY_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   17, "Pidgeotto", TYPE_NORMAL, TYPE_FLYING, 117, 105, 160, "Pidgey", PIDGEOTTO_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   18, "Pidgeot", TYPE_NORMAL, TYPE_FLYING, 166, 154, 195, "Pidgeotto", PIDGEOT_ATKS, true, true,
    { &ATK_Gust, &ATK_Wing_Attack, &ATK_Air_Cutter, }, species::CAT_NORMAL, 10, nullptr, },
  {   19, "Rattata", TYPE_NORMAL, TYPECOUNT, 103, 70, 102, NULL, RATTATA_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   19, "Alolan Rattata", TYPE_DARK, TYPE_NORMAL, 103, 70, 102, NULL, ALOLAN_RATTATA_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   20, "Raticate", TYPE_NORMAL, TYPECOUNT, 161, 139, 146, "Rattata", RATICATE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   20, "Alolan Raticate", TYPE_DARK, TYPE_NORMAL, 135, 154, 181, "Alolan Rattata", ALOLAN_RATICATE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   21, "Spearow", TYPE_NORMAL, TYPE_FLYING, 112, 60, 120, NULL, SPEAROW_ATKS, true, false,
    { &ATK_Twister, }, species::CAT_NORMAL, 10, nullptr, },
  {   22, "Fearow", TYPE_NORMAL, TYPE_FLYING, 182, 133, 163, "Spearow", FEAROW_ATKS, true, false,
    { &ATK_Twister, }, species::CAT_NORMAL, 10, nullptr, },
  {   23, "Ekans", TYPE_POISON, TYPECOUNT, 110, 97, 111, NULL, EKANS_ATKS, true, true,
    { &ATK_Gunk_Shot, }, species::CAT_NORMAL, 50, nullptr, },
  {   24, "Arbok", TYPE_POISON, TYPECOUNT, 167, 153, 155, "Ekans", ARBOK_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   25, "Pikachu", TYPE_ELECTRIC, TYPECOUNT, 112, 96, 111, "Pichu", PIKACHU_ATKS, true, false,
    { &ATK_Present, &ATK_Surf, &ATK_Thunder, }, species::CAT_NORMAL, 10, nullptr, },
  {   26, "Raichu", TYPE_ELECTRIC, TYPECOUNT, 193, 151, 155, "Pikachu", RAICHU_ATKS, true, false,
    { &ATK_Thunder, }, species::CAT_NORMAL, 10, nullptr, },
  {   26, "Alolan Raichu", TYPE_ELECTRIC, TYPE_PSYCHIC, 201, 154, 155, NULL, ALOLAN_RAICHU_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {   27, "Sandshrew", TYPE_GROUND, TYPECOUNT, 126, 120, 137, NULL, SANDSHREW_ATKS, true, true,
    { &ATK_Rock_Tomb, }, species::CAT_NORMAL, 50, nullptr, },
  {   27, "Alolan Sandshrew", TYPE_ICE, TYPE_STEEL, 125, 129, 137, NULL, ALOLAN_SANDSHREW_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   28, "Sandslash", TYPE_GROUND, TYPECOUNT, 182, 175, 181, "Sandshrew", SANDSLASH_ATKS, true, true,
    { &ATK_Night_Slash, }, species::CAT_NORMAL, 50, nullptr, },
  {   28, "Alolan Sandslash", TYPE_ICE, TYPE_STEEL, 177, 195, 181, "Alolan Sandshrew", ALOLAN_SANDSLASH_ATKS, true, true,
    { &ATK_Shadow_Claw, }, species::CAT_NORMAL, 50, nullptr, },
  {   29, "Nidoran♀", TYPE_POISON, TYPECOUNT, 86, 89, 146, NULL, NIDORANF_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   30, "Nidorina", TYPE_POISON, TYPECOUNT, 117, 120, 172, "Nidoran♀", NIDORINA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   31, "Nidoqueen", TYPE_POISON, TYPE_GROUND, 180, 173, 207, "Nidorina", NIDOQUEEN_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   32, "Nidoran♂", TYPE_POISON, TYPECOUNT, 105, 76, 130, NULL, NIDORANM_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   33, "Nidorino", TYPE_POISON, TYPECOUNT, 137, 111, 156, "Nidoran♂", NIDORINO_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   34, "Nidoking", TYPE_POISON, TYPE_GROUND, 204, 156, 191, "Nidorino", NIDOKING_ATKS, true, true,
    { &ATK_Fury_Cutter, }, species::CAT_NORMAL, 50, nullptr, },
  {   35, "Clefairy", TYPE_FAIRY, TYPECOUNT, 107, 108, 172, "Cleffa", CLEFAIRY_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {   36, "Clefable", TYPE_FAIRY, TYPECOUNT, 178, 162, 216, "Clefairy", CLEFABLE_ATKS, true, false,
    { &ATK_Pound, }, species::CAT_NORMAL, 10, nullptr, },
  {   37, "Vulpix", TYPE_FIRE, TYPECOUNT, 96, 109, 116, NULL, VULPIX_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   37, "Alolan Vulpix", TYPE_ICE, TYPECOUNT, 96, 109, 116, NULL, ALOLAN_VULPIX_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   38, "Ninetales", TYPE_FIRE, TYPECOUNT, 169, 190, 177, "Vulpix", NINETALES_ATKS, true, true,
    { &ATK_Ember, &ATK_Fire_Blast, &ATK_Flamethrower, }, species::CAT_NORMAL, 50, nullptr, },
  {   38, "Alolan Ninetales", TYPE_ICE, TYPE_FAIRY, 170, 193, 177, "Alolan Vulpix", ALOLAN_NINETALES_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   39, "Jigglypuff", TYPE_NORMAL, TYPE_FAIRY, 80, 41, 251, NULL, JIGGLYPUFF_ATKS, true, false,
    { &ATK_Body_Slam, &ATK_Play_Rough, }, species::CAT_NORMAL, 10, nullptr, },
  {   40, "Wigglytuff", TYPE_NORMAL, TYPE_FAIRY, 156, 90, 295, "Jigglypuff", WIGGLYTUFF_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {   41, "Zubat", TYPE_POISON, TYPE_FLYING, 83, 73, 120, NULL, ZUBAT_ATKS, true, true,
    { &ATK_Sludge_Bomb, }, species::CAT_NORMAL, 10, nullptr, },
  {   42, "Golbat", TYPE_POISON, TYPE_FLYING, 161, 150, 181, "Zubat", GOLBAT_ATKS, true, true,
    { &ATK_Ominous_Wind, }, species::CAT_NORMAL, 10, nullptr, },
  {   43, "Oddish", TYPE_GRASS, TYPE_POISON, 131, 112, 128, NULL, ODDISH_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   44, "Gloom", TYPE_GRASS, TYPE_POISON, 153, 136, 155, "Oddish", GLOOM_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   45, "Vileplume", TYPE_GRASS, TYPE_POISON, 202, 167, 181, "Gloom", VILEPLOOM_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   46, "Paras", TYPE_BUG, TYPE_GRASS, 121, 99, 111, NULL, PARAS_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {   47, "Parasect", TYPE_BUG, TYPE_GRASS, 165, 146, 155, "Paras", PARASECT_ATKS, true, false,
    { &ATK_Bug_Bite, }, species::CAT_NORMAL, 50, nullptr, },
  {   48, "Venonat", TYPE_BUG, TYPE_POISON, 100, 100, 155, NULL, VENONAT_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   49, "Venomoth", TYPE_BUG, TYPE_POISON, 179, 143, 172, "Venonat", VENOMOTH_ATKS, true, true,
    { &ATK_Bug_Bite, }, species::CAT_NORMAL, 50, nullptr, },
  {   50, "Diglett", TYPE_GROUND, TYPECOUNT, 109, 78, 67, NULL, DIGLETT_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   50, "Alolan Diglett", TYPE_GROUND, TYPE_STEEL, 109, 82, 67, NULL, ALOLAN_DIGLETT_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   51, "Dugtrio", TYPE_GROUND, TYPECOUNT, 167, 134, 111, "Diglett", DUGTRIO_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   51, "Alolan Dugtrio", TYPE_GROUND, TYPE_STEEL, 201, 145, 111, "Alolan Diglett", ALOLAN_DUGTRIO_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   52, "Meowth", TYPE_NORMAL, TYPECOUNT, 92, 78, 120, NULL, MEOWTH_ATKS, true, true,
    { &ATK_Body_Slam, }, species::CAT_NORMAL, 50, nullptr, },
  {   52, "Alolan Meowth", TYPE_DARK, TYPECOUNT, 99, 78, 120, NULL, A_MEOWTH_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   52, "Galarian Meowth", TYPE_STEEL, TYPECOUNT, 115, 92, 137, NULL, G_MEOWTH_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   53, "Persian", TYPE_NORMAL, TYPECOUNT, 150, 136, 163, "Meowth", PERSIAN_ATKS, true, true,
    { &ATK_Night_Slash, }, species::CAT_NORMAL, 50, nullptr, },
  {   53, "Alolan Persian", TYPE_DARK, TYPECOUNT, 158, 136, 163, "Alolan Meowth", A_PERSIAN_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   54, "Psyduck", TYPE_WATER, TYPECOUNT, 122, 95, 137, NULL, PSYDUCK_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   55, "Golduck", TYPE_WATER, TYPECOUNT, 191, 162, 190, "Psyduck", GOLDUCK_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   56, "Mankey", TYPE_FIGHTING, TYPECOUNT, 148, 82, 120, NULL, MANKEY_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   57, "Primeape", TYPE_FIGHTING, TYPECOUNT, 207, 138, 163, "Mankey", PRIMEAPE_ATKS, true, true,
    { &ATK_Karate_Chop, &ATK_Cross_Chop, &ATK_Rage_Fist, }, species::CAT_NORMAL, 50, nullptr, },
  {   58, "Growlithe", TYPE_FIRE, TYPECOUNT, 136, 93, 146, NULL, GROWLITHE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   58, "Hisuian Growlithe", TYPE_FIRE, TYPE_ROCK, 142, 92, 155, NULL, H_GROWLITHE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   59, "Arcanine", TYPE_FIRE, TYPECOUNT, 227, 166, 207, "Growlithe", ARCANINE_ATKS, true, true,
    { &ATK_Bite, &ATK_Bulldoze, }, species::CAT_NORMAL, 50, nullptr, },
  {   59, "Hisuian Arcanine", TYPE_FIRE, TYPE_ROCK, 232, 165, 216, "Hisuian Growlithe", H_ARCANINE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   60, "Poliwag", TYPE_WATER, TYPECOUNT, 101, 82, 120, NULL, POLIWAG_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   61, "Poliwhirl", TYPE_WATER, TYPECOUNT, 130, 123, 163, "Poliwag", POLIWHIRL_ATKS, true, true,
    { &ATK_Scald, }, species::CAT_NORMAL, 50, nullptr, },
  {   62, "Poliwrath", TYPE_WATER, TYPE_FIGHTING, 182, 184, 207, "Poliwhirl", POLIWRATH_ATKS, true, true,
    { &ATK_Counter, &ATK_Submission, }, species::CAT_NORMAL, 50, nullptr, },
  {   63, "Abra", TYPE_PSYCHIC, TYPECOUNT, 195, 82, 93, NULL, ABRA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   64, "Kadabra", TYPE_PSYCHIC, TYPECOUNT, 232, 117, 120, "Abra", KADABRA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   65, "Alakazam", TYPE_PSYCHIC, TYPECOUNT, 271, 167, 146, "Kadabra", ALAKAZAM_ATKS, true, true,
    { &ATK_Counter, &ATK_Dazzling_Gleam, &ATK_Psychic, }, species::CAT_NORMAL, 50, nullptr, },
  {   66, "Machop", TYPE_FIGHTING, TYPECOUNT, 137, 82, 172, NULL, MACHOP_ATKS, true, true,
    { &ATK_Low_Kick, }, species::CAT_NORMAL, 50, nullptr, },
  {   67, "Machoke", TYPE_FIGHTING, TYPECOUNT, 177, 125, 190, "Machop", MACHOKE_ATKS, true, true,
    { &ATK_Cross_Chop, }, species::CAT_NORMAL, 50, nullptr, },
  {   68, "Machamp", TYPE_FIGHTING, TYPECOUNT, 234, 159, 207, "Machoke", MACHAMP_ATKS, true, true,
    { &ATK_Karate_Chop, &ATK_Payback, &ATK_Stone_Edge, &ATK_Submission, }, species::CAT_NORMAL, 50, nullptr, },
  {   69, "Bellsprout", TYPE_GRASS, TYPE_POISON, 139, 61, 137, NULL, BELLSPROUT_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   70, "Weepinbell", TYPE_GRASS, TYPE_POISON, 172, 92, 163, "Bellsprout", WEEPINBELL_ATKS, true, true,
    { &ATK_Razor_Leaf, }, species::CAT_NORMAL, 50, nullptr, },
  {   71, "Victreebel", TYPE_GRASS, TYPE_POISON, 207, 135, 190, "Weepinbell", VICTREEBEL_ATKS, true, true,
    { &ATK_Magical_Leaf, }, species::CAT_NORMAL, 50, nullptr, },
  {   72, "Tentacool", TYPE_WATER, TYPE_POISON, 97, 149, 120, NULL, TENTACOOL_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   73, "Tentacruel", TYPE_WATER, TYPE_POISON, 166, 209, 190, "Tentacool", TENTACRUEL_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   74, "Geodude", TYPE_ROCK, TYPE_GROUND, 132, 132, 120, NULL, GEODUDE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   74, "Alolan Geodude", TYPE_ROCK, TYPE_ELECTRIC, 132, 132, 120, NULL, A_GEODUDE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   75, "Graveler", TYPE_ROCK, TYPE_GROUND, 164, 164, 146, "Geodude", GRAVELER_ATKS, true, true,
    { &ATK_Rock_Slide, }, species::CAT_NORMAL, 10, nullptr, },
  {   75, "Alolan Graveler", TYPE_ROCK, TYPE_ELECTRIC, 164, 164, 146, "Alolan Geodude", A_GRAVELER_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   76, "Golem", TYPE_ROCK, TYPE_GROUND, 211, 198, 190, "Graveler", GOLEM_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {   76, "Alolan Golem", TYPE_ROCK, TYPE_ELECTRIC, 211, 198, 190, "Alolan Graveler", A_GOLEM_ATKS, true, true,
    { &ATK_Rollout, }, species::CAT_NORMAL, 10, nullptr, },
  {   77, "Ponyta", TYPE_FIRE, TYPECOUNT, 170, 127, 137, NULL, PONYTA_ATKS, true, true,
    { &ATK_Fire_Blast, }, species::CAT_NORMAL, 50, nullptr, },
  {   77, "Galarian Ponyta", TYPE_PSYCHIC, TYPECOUNT, 170, 127, 137, NULL, G_PONYTA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   78, "Rapidash", TYPE_FIRE, TYPECOUNT, 207, 162, 163, "Ponyta", RAPIDASH_ATKS, true, true,
    { &ATK_Ember, &ATK_Wild_Charge, }, species::CAT_NORMAL, 50, nullptr, },
  {   78, "Galarian Rapidash", TYPE_PSYCHIC, TYPE_FAIRY, 207, 162, 163, "Galarian Ponyta", G_RAPIDASH_ATKS, true, true,
    { &ATK_Wild_Charge, }, species::CAT_NORMAL, 50, nullptr, },
  {   79, "Slowpoke", TYPE_WATER, TYPE_PSYCHIC, 109, 98, 207, NULL, SLOWPOKE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   79, "Galarian Slowpoke", TYPE_PSYCHIC, TYPECOUNT, 109, 98, 207, NULL, G_SLOWPOKE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   80, "Slowbro", TYPE_WATER, TYPE_PSYCHIC, 177, 180, 216, "Slowpoke", SLOWBRO_ATKS, true, true,
    { &ATK_Surf, }, species::CAT_NORMAL, 50, nullptr, },
  {   80, "Galarian Slowbro", TYPE_POISON, TYPE_PSYCHIC, 182, 156, 216, "Galarian Slowpoke", G_SLOWBRO_ATKS, true, true,
    { &ATK_Surf, }, species::CAT_NORMAL, 50, nullptr, },
  {   81, "Magnemite", TYPE_ELECTRIC, TYPE_STEEL, 165, 121, 93, NULL, MAGNEMITE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   82, "Magneton", TYPE_ELECTRIC, TYPE_STEEL, 223, 169, 137, "Magnemite", MAGNETON_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   83, "Farfetch'd", TYPE_NORMAL, TYPE_FLYING, 124, 115, 141, NULL, FARFETCHD_ATKS, true, true,
    { &ATK_Cut, }, species::CAT_NORMAL, 50, nullptr, },
  {   83, "Galarian Farfetch'd", TYPE_FIGHTING, TYPECOUNT, 174, 114, 141, NULL, G_FARFETCHD_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   84, "Doduo", TYPE_NORMAL, TYPE_FLYING, 158, 83, 111, NULL, DODUO_ATKS, true, false,
    { &ATK_Swift, }, species::CAT_NORMAL, 50, nullptr, },
  {   85, "Dodrio", TYPE_NORMAL, TYPE_FLYING, 218, 140, 155, "Doduo", DODRIO_ATKS, true, false,
    { &ATK_Air_Cutter, }, species::CAT_NORMAL, 50, nullptr, },
  {   86, "Seel", TYPE_WATER, TYPECOUNT, 85, 121, 163, NULL, SEEL_ATKS, true, false,
    { &ATK_Water_Gun, &ATK_Aqua_Jet, }, species::CAT_NORMAL, 50, nullptr, },
  {   87, "Dewgong", TYPE_WATER, TYPE_ICE, 139, 177, 207, "Seel", DEWGONG_ATKS, true, false,
    { &ATK_Ice_Shard, &ATK_Aqua_Jet, &ATK_Icy_Wind, }, species::CAT_NORMAL, 50, nullptr, },
  {   88, "Grimer", TYPE_POISON, TYPECOUNT, 135, 90, 190, NULL, GRIMER_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   88, "Alolan Grimer", TYPE_POISON, TYPE_DARK, 135, 90, 190, NULL, A_GRIMER_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   89, "Muk", TYPE_POISON, TYPECOUNT, 190, 172, 233, "Grimer", MUK_ATKS, true, true,
    { &ATK_Lick, }, species::CAT_NORMAL, 50, nullptr, },
  {   89, "Alolan Muk", TYPE_POISON, TYPE_DARK, 190, 172, 233, "Alolan Grimer", A_MUK_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   90, "Shellder", TYPE_WATER, TYPECOUNT, 116, 134, 102, NULL, SHELLDER_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   91, "Cloyster", TYPE_WATER, TYPE_ICE, 186, 256, 137, "Shellder", CLOYSTER_ATKS, true, true,
    { &ATK_Blizzard, }, species::CAT_NORMAL, 50, nullptr, },
  {   92, "Gastly", TYPE_GHOST, TYPE_POISON, 186, 67, 102, NULL, GASTLY_ATKS, true, true,
    { &ATK_Sucker_Punch, &ATK_Ominous_Wind, }, species::CAT_NORMAL, 50, nullptr, },
  {   93, "Haunter", TYPE_GHOST, TYPE_POISON, 223, 107, 128, "Gastly", HAUNTER_ATKS, true, true,
    { &ATK_Lick, }, species::CAT_NORMAL, 50, nullptr, },
  {   94, "Gengar", TYPE_GHOST, TYPE_POISON, 261, 149, 155, "Haunter", GENGAR_ATKS, true, true,
    { &ATK_Lick, &ATK_Dark_Pulse, &ATK_Psychic, &ATK_Shadow_Punch, &ATK_Sludge_Wave, }, species::CAT_NORMAL, 50, nullptr, },
  {   95, "Onix", TYPE_ROCK, TYPE_GROUND, 85, 232, 111, NULL, ONIX_ATKS, true, true,
    { &ATK_Iron_Head, &ATK_Rock_Slide, }, species::CAT_NORMAL, 75, nullptr, },
  {   96, "Drowzee", TYPE_PSYCHIC, TYPECOUNT, 89, 136, 155, NULL, DROWZEE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   97, "Hypno", TYPE_PSYCHIC, TYPECOUNT, 144, 193, 198, "Drowzee", HYPNO_ATKS, true, true,
    { &ATK_Psyshock, }, species::CAT_NORMAL, 50, nullptr, },
  {   98, "Krabby", TYPE_WATER, TYPECOUNT, 181, 124, 102, NULL, KRABBY_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {   99, "Kingler", TYPE_WATER, TYPECOUNT, 240, 181, 146, "Krabby", KINGLER_ATKS, true, true,
    { &ATK_Mud_Shot, }, species::CAT_NORMAL, 50, nullptr, },
  {  100, "Voltorb", TYPE_ELECTRIC, TYPECOUNT, 109, 111, 120, NULL, VOLTORB_ATKS, true, true,
    { &ATK_Signal_Beam, }, species::CAT_NORMAL, 50, nullptr, },
  {  100, "Hisuian Voltorb", TYPE_ELECTRIC, TYPE_GRASS, 109, 111, 120, NULL, H_VOLTORB_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  101, "Electrode", TYPE_ELECTRIC, TYPECOUNT, 173, 173, 155, "Voltorb", ELECTRODE_ATKS, true, true,
    { &ATK_Tackle, }, species::CAT_NORMAL, 50, nullptr, },
  {  101, "Hisuian Electrode", TYPE_ELECTRIC, TYPE_GRASS, 176, 176, 155, "Hisuian Voltorb", H_ELECTRODE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  102, "Exeggcute", TYPE_GRASS, TYPE_PSYCHIC, 107, 125, 155, NULL, EXEGGCUTE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  103, "Exeggutor", TYPE_GRASS, TYPE_PSYCHIC, 233, 149, 216, "Exeggcute", EXEGGUTOR_ATKS, true, true,
    { &ATK_Zen_Headbutt, }, species::CAT_NORMAL, 50, nullptr, },
  {  103, "Alolan Exeggutor", TYPE_GRASS, TYPE_DRAGON, 230, 153, 216, "Exeggcute", A_EXEGGUTOR_ATKS, true, true,
    { &ATK_Draco_Meteor, }, species::CAT_NORMAL, 50, nullptr, },
  {  104, "Cubone", TYPE_GROUND, TYPECOUNT, 90, 144, 137, NULL, CUBONE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  105, "Marowak", TYPE_GROUND, TYPECOUNT, 144, 186, 155, "Cubone", MAROWAK_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  105, "Alolan Marowak", TYPE_FIRE, TYPE_GHOST, 144, 186, 155, "Cubone", A_MAROWAK_ATKS, true, true,
    { &ATK_Shadow_Bone, }, species::CAT_NORMAL, 50, nullptr, },
  {  106, "Hitmonlee", TYPE_FIGHTING, TYPECOUNT, 224, 181, 137, "Tyrogue", HITMONLEE_ATKS, true, true,
    { &ATK_Brick_Break, &ATK_Stomp, }, species::CAT_NORMAL, 75, nullptr, },
  {  107, "Hitmonchan", TYPE_FIGHTING, TYPECOUNT, 193, 197, 137, "Tyrogue", HITMONCHAN_ATKS, true, true,
    { &ATK_Rock_Smash, &ATK_Brick_Break, }, species::CAT_NORMAL, 75, nullptr, },
  {  108, "Lickitung", TYPE_NORMAL, TYPECOUNT, 108, 137, 207, NULL, LICKITUNG_ATKS, true, false,
    { &ATK_Body_Slam, }, species::CAT_NORMAL, 50, nullptr, },
  {  109, "Koffing", TYPE_POISON, TYPECOUNT, 119, 141, 120, NULL, KOFFING_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  110, "Weezing", TYPE_POISON, TYPECOUNT, 174, 197, 163, "Koffing", WEEZING_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  110, "Galarian Weezing", TYPE_POISON, TYPE_FAIRY, 174, 197, 163, NULL, G_WEEZING_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  111, "Rhyhorn", TYPE_GROUND, TYPE_ROCK, 140, 127, 190, NULL, RHYHORN_ATKS, true, true,{}, species::CAT_NORMAL, 50, nullptr, },
  {  112, "Rhydon", TYPE_GROUND, TYPE_ROCK, 222, 171, 233, "Rhyhorn", RHYDON_ATKS, true, true,
    { &ATK_Megahorn, }, species::CAT_NORMAL, 50, nullptr, },
  {  113, "Chansey", TYPE_NORMAL, TYPECOUNT, 60, 128, 487, "Happiny", CHANSEY_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  114, "Tangela", TYPE_GRASS, TYPECOUNT, 183, 169, 163, NULL, TANGELA_ATKS, true, true,
    { &ATK_Power_Whip, }, species::CAT_NORMAL, 50, nullptr, },
  {  115, "Kangaskhan", TYPE_NORMAL, TYPECOUNT, 181, 165, 233, NULL, KANGASKHAN_ATKS, true, true,
    { &ATK_Brick_Break, &ATK_Stomp, }, species::CAT_NORMAL, 50, nullptr, },
  {  116, "Horsea", TYPE_WATER, TYPECOUNT, 129, 103, 102, NULL, HORSEA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  117, "Seadra", TYPE_WATER, TYPECOUNT, 187, 156, 146, "Horsea", SEADRA_ATKS, true, true,
    { &ATK_Blizzard, }, species::CAT_NORMAL, 50, nullptr, },
  {  118, "Goldeen", TYPE_WATER, TYPECOUNT, 123, 110, 128, NULL, GOLDEEN_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  119, "Seaking", TYPE_WATER, TYPECOUNT, 175, 147, 190, "Goldeen", SEAKING_ATKS, true, false,
    { &ATK_Poison_Jab, &ATK_Drill_Run, &ATK_Icy_Wind, }, species::CAT_NORMAL, 50, nullptr, },
  {  120, "Staryu", TYPE_WATER, TYPECOUNT, 137, 112, 102, NULL, STARYU_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  121, "Starmie", TYPE_WATER, TYPE_PSYCHIC, 210, 184, 155, "Staryu", STARMIE_ATKS, true, false,
    { &ATK_Tackle, }, species::CAT_NORMAL, 50, nullptr, },
  {  122, "Mr. Mime", TYPE_PSYCHIC, TYPE_FAIRY, 192, 205, 120, "Mime Jr.", MRMIME_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  122, "Galarian Mr. Mime", TYPE_ICE, TYPE_PSYCHIC, 183, 169, 137, NULL, G_MRMIME_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  123, "Scyther", TYPE_BUG, TYPE_FLYING, 218, 170, 172, NULL, SCYTHER_ATKS, true, true,
    { &ATK_Steel_Wing, &ATK_Bug_Buzz, }, species::CAT_NORMAL, 75, nullptr, },
  {  124, "Jynx", TYPE_ICE, TYPE_PSYCHIC, 223, 151, 163, "Smoochum", JYNX_ATKS, true, false,
    { &ATK_Pound, &ATK_Ice_Punch, }, species::CAT_NORMAL, 75, nullptr, },
  {  125, "Electabuzz", TYPE_ELECTRIC, TYPECOUNT, 198, 158, 163, "Elekid", ELECTABUZZ_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  126, "Magmar", TYPE_FIRE, TYPECOUNT, 206, 154, 163, NULL, MAGMAR_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  127, "Pinsir", TYPE_BUG, TYPECOUNT, 238, 182, 163, NULL, PINSIR_ATKS, true, true,
    { &ATK_Submission, }, species::CAT_NORMAL, 75, nullptr, },
  {  128, "Tauros", TYPE_NORMAL, TYPECOUNT, 198, 183, 181, NULL, TAUROS_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  128, "Paldean Combat Tauros", TYPE_FIGHTING, TYPECOUNT, 210, 193, 181, NULL, C_TAUROS_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  128, "Paldean Blaze Tauros", TYPE_FIGHTING, TYPE_FIRE, 210, 193, 181, NULL, B_TAUROS_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  128, "Paldean Aqua Tauros", TYPE_FIGHTING, TYPE_WATER, 210, 193, 181, NULL, A_TAUROS_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  129, "Magikarp", TYPE_WATER, TYPECOUNT, 29, 85, 85, NULL, MAGIKARP_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  130, "Gyarados", TYPE_WATER, TYPE_FLYING, 237, 186, 216, "Magikarp", GYARADOS_ATKS, true, true,
    { &ATK_Dragon_Tail, &ATK_Aqua_Tail, &ATK_Dragon_Pulse, }, species::CAT_NORMAL, 10, nullptr, },
  {  131, "Lapras", TYPE_WATER, TYPE_ICE, 165, 174, 277, NULL, LAPRAS_ATKS, true, true,
    { &ATK_Ice_Shard, &ATK_Dragon_Pulse, &ATK_Ice_Beam, }, species::CAT_NORMAL, 75, nullptr, },
  {  132, "Ditto", TYPE_NORMAL, TYPECOUNT, 91, 91, 134, NULL, DITTO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  133, "Eevee", TYPE_NORMAL, TYPECOUNT, 104, 114, 146, NULL, EEVEE_ATKS, true, false,
    { &ATK_Body_Slam, &ATK_Last_Resort, }, species::CAT_NORMAL, 75, nullptr, },
  {  134, "Vaporeon", TYPE_WATER, TYPECOUNT, 205, 161, 277, "Eevee", VAPOREON_ATKS, true, false,
    { &ATK_Scald, &ATK_Last_Resort, }, species::CAT_NORMAL, 75, nullptr, },
  {  135, "Jolteon", TYPE_ELECTRIC, TYPECOUNT, 232, 182, 163, "Eevee", JOLTEON_ATKS, true, false,
    { &ATK_Zap_Cannon, &ATK_Last_Resort, }, species::CAT_NORMAL, 75, nullptr, },
  {  136, "Flareon", TYPE_FIRE, TYPECOUNT, 246, 179, 163, "Eevee", FLAREON_ATKS, true, false,
    { &ATK_Heat_Wave, &ATK_Superpower, &ATK_Last_Resort, }, species::CAT_NORMAL, 75, nullptr, },
  {  137, "Porygon", TYPE_NORMAL, TYPECOUNT, 153, 136, 163, NULL, PORYGON_ATKS, true, true,
    { &ATK_Discharge, &ATK_Psybeam, &ATK_Signal_Beam, }, species::CAT_NORMAL, 50, nullptr, },
  {  138, "Omanyte", TYPE_ROCK, TYPE_WATER, 155, 153, 111, NULL, OMANYTE_ATKS, true, true,
    { &ATK_Brine, &ATK_Rock_Tomb, }, species::CAT_NORMAL, 75, nullptr, },
  {  139, "Omastar", TYPE_ROCK, TYPE_WATER, 207, 201, 172, "Omanyte", OMASTAR_ATKS, true, true,
    { &ATK_Rock_Throw, &ATK_Rock_Slide, }, species::CAT_NORMAL, 75, nullptr, },
  {  140, "Kabuto", TYPE_ROCK, TYPE_WATER, 148, 140, 102, NULL, KABUTO_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  141, "Kabutops", TYPE_ROCK, TYPE_WATER, 220, 186, 155, "Kabuto", KABUTOPS_ATKS, true, true,
    { &ATK_Fury_Cutter, }, species::CAT_NORMAL, 75, nullptr, },
  {  142, "Aerodactyl", TYPE_ROCK, TYPE_FLYING, 221, 159, 190, NULL, AERODACTYL_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  143, "Snorlax", TYPE_NORMAL, TYPECOUNT, 190, 169, 330, "Munchlax", SNORLAX_ATKS, true, true,
    { &ATK_Yawn, }, species::CAT_NORMAL, 75, nullptr, },
  {  144, "Articuno", TYPE_ICE, TYPE_FLYING, 192, 236, 207, NULL, ARTICUNO_ATKS, true, true,
    { &ATK_Hurricane, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  144, "Galarian Articuno", TYPE_PSYCHIC, TYPE_FLYING, 250, 197, 207, NULL, G_ARTICUNO_ATKS, true, true, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  145, "Zapdos", TYPE_ELECTRIC, TYPE_FLYING, 253, 185, 207, NULL, ZAPDOS_ATKS, true, true,
    { &ATK_Thunder_Shock, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  145, "Galarian Zapdos", TYPE_FIGHTING, TYPE_FLYING, 252, 189, 207, NULL, G_ZAPDOS_ATKS, true, true, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  146, "Moltres", TYPE_FIRE, TYPE_FLYING, 251, 181, 207, NULL, MOLTRES_ATKS, true, true,
    { &ATK_Sky_Attack, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  146, "Galarian Moltres", TYPE_DARK, TYPE_FLYING, 202, 231, 207, NULL, G_MOLTRES_ATKS, true, true, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  147, "Dratini", TYPE_DRAGON, TYPECOUNT, 119, 91, 121, NULL, DRATINI_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  148, "Dragonair", TYPE_DRAGON, TYPECOUNT, 163, 135, 156, "Dratini", DRAGONAIR_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  149, "Dragonite", TYPE_DRAGON, TYPE_FLYING, 263, 198, 209, "Dragonair", DRAGONITE_ATKS, true, true,
    { &ATK_Draco_Meteor, &ATK_Dragon_Pulse, }, species::CAT_NORMAL, 75, nullptr, },
  {  150, "Mewtwo", TYPE_PSYCHIC, TYPECOUNT, 300, 182, 214, NULL, MEWTWO_ATKS, true, true,
    { &ATK_Hyper_Beam, &ATK_Psystrike, &ATK_Shadow_Ball, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  150, "Armored Mewtwo", TYPE_PSYCHIC, TYPECOUNT, 182, 278, 214, NULL, A_MEWTWO_ATKS, false, false,
    { &ATK_Psystrike, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  151, "Mew", TYPE_PSYCHIC, TYPECOUNT, 210, 210, 225, NULL, MEW_ATKS, true, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  152, "Chikorita", TYPE_GRASS, TYPECOUNT, 92, 122, 128, NULL, CHIKORITA_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  153, "Bayleef", TYPE_GRASS, TYPECOUNT, 122, 155, 155, "Chikorita", BAYLEEF_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  154, "Meganium", TYPE_GRASS, TYPECOUNT, 168, 202, 190, "Bayleef", MEGANIUM_ATKS, true, true,
    { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, },
  {  155, "Cyndaquil", TYPE_FIRE, TYPECOUNT, 116, 93, 118, NULL, CYNDAQUIL_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  156, "Quilava", TYPE_FIRE, TYPECOUNT, 158, 126, 151, "Cyndaquil", QUILAVA_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  157, "Typhlosion", TYPE_FIRE, TYPECOUNT, 223, 173, 186, "Quilava", TYPHLOSION_ATKS, true, true,
    { &ATK_Blast_Burn, }, species::CAT_NORMAL, 10, nullptr, },
  {  157, "Hisuian Typhlosion", TYPE_FIRE, TYPE_GHOST, 238, 172, 177, NULL, H_TYPHLOSION_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  158, "Totodile", TYPE_WATER, TYPECOUNT, 117, 109, 137, NULL, TOTODILE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  159, "Croconaw", TYPE_WATER, TYPECOUNT, 150, 142, 163, "Totodile", CROCONAW_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  160, "Feraligatr", TYPE_WATER, TYPECOUNT, 205, 188, 198, "Croconaw", FERALIGATR_ATKS, true, true,
    { &ATK_Water_Gun, &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, },
  {  161, "Sentret", TYPE_NORMAL, TYPECOUNT, 79, 73, 111, NULL, SENTRET_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  162, "Furret", TYPE_NORMAL, TYPECOUNT, 148, 125, 198, "Sentret", FURRET_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  163, "Hoothoot", TYPE_NORMAL, TYPE_FLYING, 67, 88, 155, NULL, HOOTHOOT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  164, "Noctowl", TYPE_NORMAL, TYPE_FLYING, 145, 156, 225, "Hoothoot", NOCTOWL_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  165, "Ledyba", TYPE_BUG, TYPE_FLYING, 72, 118, 120, NULL, LEDYBA_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  166, "Ledian", TYPE_BUG, TYPE_FLYING, 107, 179, 146, "Ledyba", LEDIAN_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  167, "Spinarak", TYPE_BUG, TYPE_POISON, 105, 73, 120, NULL, SPINARAK_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  168, "Ariados", TYPE_BUG, TYPE_POISON, 161, 124, 172, "Spinarak", ARIADOS_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  169, "Crobat", TYPE_POISON, TYPE_FLYING, 194, 178, 198, "Golbat", CROBAT_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  170, "Chinchou", TYPE_WATER, TYPE_ELECTRIC, 106, 97, 181, NULL, CHINCHOU_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  171, "Lanturn", TYPE_WATER, TYPE_ELECTRIC, 146, 137, 268, "Chinchou", LANTURN_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  172, "Pichu", TYPE_ELECTRIC, TYPECOUNT, 77, 53, 85, NULL, PICHU_ATKS, true, false, {}, species::CAT_BABY, 10, nullptr, },
  {  173, "Cleffa", TYPE_FAIRY, TYPECOUNT, 75, 79, 137, NULL, CLEFFA_ATKS, true, false,
    { &ATK_Body_Slam, &ATK_Psychic, }, species::CAT_BABY, 10, nullptr, },
  {  174, "Igglybuff", TYPE_NORMAL, TYPE_FAIRY, 69, 32, 207, NULL, IGGLYBUFF_ATKS, true, false,
    { &ATK_Body_Slam, }, species::CAT_BABY, 10, nullptr, },
  {  175, "Togepi", TYPE_FAIRY, TYPECOUNT, 67, 116, 111, NULL, TOGEPI_ATKS, true, false,
    { &ATK_Zen_Headbutt, }, species::CAT_BABY, 10, nullptr, },
  {  176, "Togetic", TYPE_FAIRY, TYPE_FLYING, 139, 181, 146, "Togepi", TOGETIC_ATKS, true, false,
    { &ATK_Steel_Wing, &ATK_Zen_Headbutt, }, species::CAT_NORMAL, 50, nullptr, },
  {  177, "Natu", TYPE_PSYCHIC, TYPE_FLYING, 134, 89, 120, NULL, NATU_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  178, "Xatu", TYPE_PSYCHIC, TYPE_FLYING, 192, 146, 163, "Natu", XATU_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  179, "Mareep", TYPE_ELECTRIC, TYPECOUNT, 114, 79, 146, NULL, MAREEP_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  180, "Flaaffy", TYPE_ELECTRIC, TYPECOUNT, 145, 109, 172, "Mareep", FLAAFFY_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  181, "Ampharos", TYPE_ELECTRIC, TYPECOUNT, 211, 169, 207, "Flaaffy", AMPHAROS_ATKS, true, true,
    { &ATK_Dragon_Pulse, }, species::CAT_NORMAL, 75, nullptr, },
  {  182, "Bellossom", TYPE_GRASS, TYPECOUNT, 169, 186, 181, "Gloom", BELLOSSOM_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  183, "Marill", TYPE_WATER, TYPE_FAIRY, 37, 93, 172, "Azurill", MARILL_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  184, "Azumarill", TYPE_WATER, TYPE_FAIRY, 112, 152, 225, "Marill", AZUMARILL_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  185, "Sudowoodo", TYPE_ROCK, TYPECOUNT, 167, 176, 172, "Bonsly", SUDOWOODO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  186, "Politoed", TYPE_WATER, TYPECOUNT, 174, 179, 207, "Poliwhirl", POLITOED_ATKS, true, true,
    { &ATK_Earthquake, &ATK_Ice_Beam, }, species::CAT_NORMAL, 50, nullptr, },
  {  187, "Hoppip", TYPE_GRASS, TYPE_FLYING, 67, 94, 111, NULL, HOPPIP_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  188, "Skiploom", TYPE_GRASS, TYPE_FLYING, 91, 120, 146, "Hoppip", SKIPLOOM_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  189, "Jumpluff", TYPE_GRASS, TYPE_FLYING, 118, 183, 181, "Skiploom", JUMPLUFF_ATKS, true, true,
    { &ATK_Acrobatics, }, species::CAT_NORMAL, 50, nullptr, },
  {  190, "Aipom", TYPE_NORMAL, TYPECOUNT, 136, 112, 146, NULL, AIPOM_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  191, "Sunkern", TYPE_GRASS, TYPECOUNT, 55, 55, 102, NULL, SUNKERN_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  192, "Sunflora", TYPE_GRASS, TYPECOUNT, 185, 135, 181, "Sunkern", SUNFLORA_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  193, "Yanma", TYPE_BUG, TYPE_FLYING, 154, 94, 163, NULL, YANMA_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  194, "Wooper", TYPE_WATER, TYPE_GROUND, 75, 66, 146, NULL, WOOPER_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  194, "Paldean Wooper", TYPE_POISON, TYPE_GROUND, 75, 66, 146, NULL, P_WOOPER_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  195, "Quagsire", TYPE_WATER, TYPE_GROUND, 152, 143, 216, "Wooper", QUAGSIRE_ATKS, true, true,
    { &ATK_Aqua_Tail, }, species::CAT_NORMAL, 50, nullptr, },
  {  196, "Espeon", TYPE_PSYCHIC, TYPECOUNT, 261, 175, 163, "Eevee", ESPEON_ATKS, true, false,
    { &ATK_Last_Resort, &ATK_Shadow_Ball, }, species::CAT_NORMAL, 75, nullptr, },
  {  197, "Umbreon", TYPE_DARK, TYPECOUNT, 126, 240, 216, "Eevee", UMBREON_ATKS, true, false,
    { &ATK_Last_Resort, &ATK_Psychic, }, species::CAT_NORMAL, 75, nullptr, },
  {  198, "Murkrow", TYPE_DARK, TYPE_FLYING, 175, 87, 155, NULL, MURKROW_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  199, "Slowking", TYPE_WATER, TYPE_PSYCHIC, 177, 180, 216, "Slowpoke", SLOWKING_ATKS, true, true,
    { &ATK_Surf, }, species::CAT_NORMAL, 50, nullptr, },
  {  199, "Galarian Slowking", TYPE_POISON, TYPE_PSYCHIC, 190, 180, 216, "Galarian Slowpoke", G_SLOWKING_ATKS, true, true,
    { &ATK_Surf, }, species::CAT_NORMAL, 50, nullptr, },
  {  200, "Misdreavus", TYPE_GHOST, TYPECOUNT, 167, 154, 155, NULL, MISDREAVUS_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  201, "Unown", TYPE_PSYCHIC, TYPECOUNT, 136, 91, 134, NULL, UNOWN_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  202, "Wobbuffet", TYPE_PSYCHIC, TYPECOUNT, 60, 106, 382, "Wynaut", WOBBUFFET_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  203, "Girafarig", TYPE_NORMAL, TYPE_PSYCHIC, 182, 133, 172, NULL, GIRAFARIG_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  204, "Pineco", TYPE_BUG, TYPECOUNT, 108, 122, 137, NULL, PINECO_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  205, "Forretress", TYPE_BUG, TYPE_STEEL, 161, 205, 181, "Pineco", FORRETRESS_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  206, "Dunsparce", TYPE_NORMAL, TYPECOUNT, 131, 128, 225, NULL, DUNSPARCE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  207, "Gligar", TYPE_GROUND, TYPE_FLYING, 143, 184, 163, NULL, GLIGAR_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  208, "Steelix", TYPE_STEEL, TYPE_GROUND, 148, 272, 181, "Onix", STEELIX_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  209, "Snubbull", TYPE_FAIRY, TYPECOUNT, 137, 85, 155, NULL, SNUBBULL_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  210, "Granbull", TYPE_FAIRY, TYPECOUNT, 212, 131, 207, "Snubbull", GRANBULL_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  211, "Qwilfish", TYPE_WATER, TYPE_POISON, 184, 138, 163, NULL, QWILFISH_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  211, "Hisuian Qwilfish", TYPE_DARK, TYPE_POISON, 184, 151, 163, NULL, H_QWILFISH_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  212, "Scizor", TYPE_BUG, TYPE_STEEL, 236, 181, 172, "Scyther", SCIZOR_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  213, "Shuckle", TYPE_BUG, TYPE_ROCK, 17, 396, 85, NULL, SHUCKLE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  214, "Heracross", TYPE_BUG, TYPE_FIGHTING, 234, 179, 190, NULL, HERACROSS_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  215, "Sneasel", TYPE_DARK, TYPE_ICE, 189, 146, 146, NULL, SNEASEL_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  215, "Hisuian Sneasel", TYPE_FIGHTING, TYPE_POISON, 189, 146, 146, NULL, H_SNEASEL_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  216, "Teddiursa", TYPE_NORMAL, TYPECOUNT, 142, 93, 155, NULL, TEDDIURSA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  217, "Ursaring", TYPE_NORMAL, TYPECOUNT, 236, 144, 207, "Teddiursa", URSARING_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  218, "Slugma", TYPE_FIRE, TYPECOUNT, 118, 71, 120, NULL, SLUGMA_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  219, "Magcargo", TYPE_FIRE, TYPE_ROCK, 139, 191, 137, "Slugma", MAGCARGO_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  220, "Swinub", TYPE_ICE, TYPE_GROUND, 90, 69, 137, NULL, SWINUB_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  221, "Piloswine", TYPE_ICE, TYPE_GROUND, 181, 138, 225, "Swinub", PILOSWINE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  222, "Corsola", TYPE_WATER, TYPE_ROCK, 118, 156, 146, NULL, CORSOLA_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  222, "Galarian Corsola", TYPE_GHOST, TYPECOUNT, 116, 182, 155, NULL, G_CORSOLA_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  223, "Remoraid", TYPE_WATER, TYPECOUNT, 127, 69, 111, NULL, REMORAID_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  224, "Octillery", TYPE_WATER, TYPECOUNT, 197, 141, 181, "Remoraid", OCTILLERY_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  225, "Delibird", TYPE_ICE, TYPE_FLYING, 128, 90, 128, NULL, DELIBIRD_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  226, "Mantine", TYPE_WATER, TYPE_FLYING, 148, 226, 163, "Mantyke", MANTINE_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  227, "Skarmory", TYPE_STEEL, TYPE_FLYING, 148, 226, 163, NULL, SKARMORY_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  228, "Houndour", TYPE_DARK, TYPE_FIRE, 152, 83, 128, NULL, HOUNDOUR_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  229, "Houndoom", TYPE_DARK, TYPE_FIRE, 224, 144, 181, "Houndour", HOUNDOOM_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  230, "Kingdra", TYPE_WATER, TYPE_DRAGON, 194, 194, 181, "Seadra", KINGDRA_ATKS, true, true,
    { &ATK_Water_Gun, }, species::CAT_NORMAL, 50, nullptr, },
  {  231, "Phanpy", TYPE_GROUND, TYPECOUNT, 107, 98, 207, NULL, PHANPY_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  232, "Donphan", TYPE_GROUND, TYPECOUNT, 214, 185, 207, "Phanpy", DONPHAN_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  233, "Porygon2", TYPE_NORMAL, TYPECOUNT, 198, 180, 198, "Porygon", PORYGON2_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  234, "Stantler", TYPE_NORMAL, TYPECOUNT, 192, 131, 177, NULL, STANTLER_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  235, "Smeargle", TYPE_NORMAL, TYPECOUNT, 40, 83, 146, NULL, SMEARGLE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  236, "Tyrogue", TYPE_FIGHTING, TYPECOUNT, 64, 64, 111, NULL, TYROGUE_ATKS, true, false, {}, species::CAT_BABY, 10, nullptr, },
  {  237, "Hitmontop", TYPE_FIGHTING, TYPECOUNT, 173, 207, 137, "Tyrogue", HITMONTOP_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  238, "Smoochum", TYPE_ICE, TYPE_PSYCHIC, 153, 91, 128, NULL, SMOOCHUM_ATKS, true, false,
    { &ATK_Frost_Breath, }, species::CAT_BABY, 10, nullptr, },
  {  239, "Elekid", TYPE_ELECTRIC, TYPECOUNT, 135, 101, 128, NULL, ELEKID_ATKS, true, true,
    { &ATK_Thunderbolt, }, species::CAT_BABY, 10, nullptr, },
  {  240, "Magby", TYPE_FIRE, TYPECOUNT, 151, 99, 128, NULL, MAGBY_ATKS, true, true,
    { &ATK_Flamethrower, }, species::CAT_BABY, 10, nullptr, },
  {  241, "Miltank", TYPE_NORMAL, TYPECOUNT, 157, 193, 216, NULL, MILTANK_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  242, "Blissey", TYPE_NORMAL, TYPECOUNT, 129, 169, 496, "Chansey", BLISSEY_ATKS, true, false,
    { &ATK_Wild_Charge, }, species::CAT_NORMAL, 75, nullptr, },
  {  243, "Raikou", TYPE_ELECTRIC, TYPECOUNT, 241, 195, 207, NULL, RAIKOU_ATKS, true, true, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  244, "Entei", TYPE_FIRE, TYPECOUNT, 235, 171, 251, NULL, ENTEI_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  245, "Suicune", TYPE_WATER, TYPECOUNT, 180, 235, 225, NULL, SUICUNE_ATKS, true, true,
    { &ATK_Hidden_Power, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  246, "Larvitar", TYPE_ROCK, TYPE_GROUND, 115, 93, 137, NULL, LARVITAR_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  247, "Pupitar", TYPE_ROCK, TYPE_GROUND, 155, 133, 172, "Larvitar", PUPITAR_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  248, "Tyranitar", TYPE_ROCK, TYPE_DARK, 251, 207, 225, "Pupitar", TYRANITAR_ATKS, true, true,
    { &ATK_Smack_Down, }, species::CAT_NORMAL, 75, nullptr, },
  {  249, "Lugia", TYPE_PSYCHIC, TYPE_FLYING, 193, 310, 235, NULL, LUGIA_ATKS, true, true,
    { &ATK_Aeroblast, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  250, "Ho-Oh", TYPE_FIRE, TYPE_FLYING, 239, 244, 214, NULL, HOOH_ATKS, true, true,
    { &ATK_Earthquake, &ATK_Sacred_Fire, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  251, "Celebi", TYPE_PSYCHIC, TYPE_GRASS, 210, 210, 225, NULL, CELEBI_ATKS, true, false,
    { &ATK_Magical_Leaf, }, species::CAT_MYTHICAL, 100, nullptr, },
  {  252, "Treecko", TYPE_GRASS, TYPECOUNT, 124, 94, 120, NULL, TREECKO_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  253, "Grovyle", TYPE_GRASS, TYPECOUNT, 172, 120, 137, "Treecko", GROVYLE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  254, "Sceptile", TYPE_GRASS, TYPECOUNT, 223, 169, 172, "Grovyle", SCEPTILE_ATKS, true, true,
    { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, },
  {  255, "Torchic", TYPE_FIRE, TYPECOUNT, 130, 87, 128, NULL, TORCHIC_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  256, "Combusken", TYPE_FIRE, TYPE_FIGHTING, 163, 115, 155, "Torchic", COMBUSKEN_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  257, "Blaziken", TYPE_FIRE, TYPE_FIGHTING, 240, 141, 190, "Combusken", BLAZIKEN_ATKS, true, true,
    { &ATK_Blast_Burn, &ATK_Stone_Edge, }, species::CAT_NORMAL, 10, nullptr, },
  {  258, "Mudkip", TYPE_WATER, TYPECOUNT, 126, 93, 137, NULL, MUDKIP_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  259, "Marshtomp", TYPE_WATER, TYPE_GROUND, 156, 133, 172, "Mudkip", MARSHTOMP_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  260, "Swampert", TYPE_WATER, TYPE_GROUND, 208, 175, 225, "Marshtomp", SWAMPERT_ATKS, true, true,
    { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, },
  {  261, "Poochyena", TYPE_DARK, TYPECOUNT, 96, 61, 111, NULL, POOCHYENA_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  262, "Mightyena", TYPE_DARK, TYPECOUNT, 171, 132, 172, "Poochyena", MIGHTYENA_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  263, "Zigzagoon", TYPE_NORMAL, TYPECOUNT, 58, 80, 116, NULL, ZIGZAGOON_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  263, "Galarian Zigzagoon", TYPE_DARK, TYPE_NORMAL, 58, 80, 116, NULL, G_ZIGZAGOON_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  264, "Linoone", TYPE_NORMAL, TYPECOUNT, 142, 128, 186, "Zigzagoon", LINOONE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  264, "Galarian Linoone", TYPE_DARK, TYPE_NORMAL, 142, 128, 186, "Galarian Zigzagoon", G_LINOONE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  265, "Wurmple", TYPE_BUG, TYPECOUNT, 75, 59, 128, NULL, WURMPLE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  266, "Silcoon", TYPE_BUG, TYPECOUNT, 60, 77, 137, "Wurmple", SILCOON_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  267, "Beautifly", TYPE_BUG, TYPE_FLYING, 189, 98, 155, "Silcoon", BEAUTIFLY_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  268, "Cascoon", TYPE_BUG, TYPECOUNT, 60, 77, 137, "Wurmple", CASCOON_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  269, "Dustox", TYPE_BUG, TYPE_POISON, 98, 162, 155, "Cascoon", DUSTOX_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  270, "Lotad", TYPE_WATER, TYPE_GRASS, 71, 77, 120, NULL, LOTAD_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  271, "Lombre", TYPE_WATER, TYPE_GRASS, 112, 119, 155, "Lotad", LOMBRE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  272, "Ludicolo", TYPE_WATER, TYPE_GRASS, 173, 176, 190, "Lombre", LUDICOLO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  273, "Seedot", TYPE_GRASS, TYPECOUNT, 71, 77, 120, NULL, SEEDOT_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  274, "Nuzleaf", TYPE_GRASS, TYPE_DARK, 134, 78, 172, "Seedot", NUZLEAF_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  275, "Shiftry", TYPE_GRASS, TYPE_DARK, 200, 121, 207, "Nuzleaf", SHIFTRY_ATKS, true, true,
    { &ATK_Bullet_Seed, }, species::CAT_NORMAL, 50, nullptr, },
  {  276, "Taillow", TYPE_NORMAL, TYPE_FLYING, 106, 61, 120, NULL, TAILLOW_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  277, "Swellow", TYPE_NORMAL, TYPE_FLYING, 185, 124, 155, "Taillow", SWELLOW_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  278, "Wingull", TYPE_WATER, TYPE_FLYING, 106, 61, 120, NULL, WINGULL_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  279, "Pelipper", TYPE_WATER, TYPE_FLYING, 175, 174, 155, "Wingull", PELIPPER_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  280, "Ralts", TYPE_PSYCHIC, TYPE_FAIRY, 79, 59, 99, NULL, RALTS_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  281, "Kirlia", TYPE_PSYCHIC, TYPE_FAIRY, 117, 90, 116, "Ralts", KIRLIA_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  282, "Gardevoir", TYPE_PSYCHIC, TYPE_FAIRY, 237, 195, 169, "Kirlia", GARDEVOIR_ATKS, true, true,
    { &ATK_Synchronoise, }, species::CAT_NORMAL, 75, nullptr, },
  {  283, "Surskit", TYPE_BUG, TYPE_WATER, 93, 87, 120, NULL, SURSKIT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  284, "Masquerain", TYPE_BUG, TYPE_FLYING, 192, 150, 172, "Surskit", MASQUERAIN_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  285, "Shroomish", TYPE_GRASS, TYPECOUNT, 74, 110, 155, NULL, SHROOMISH_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  286, "Breloom", TYPE_GRASS, TYPE_FIGHTING, 241, 144, 155, "Shroomish", BRELOOM_ATKS, true, false,
    { &ATK_Grass_Knot, }, species::CAT_NORMAL, 50, nullptr, },
  {  287, "Slakoth", TYPE_NORMAL, TYPECOUNT, 104, 92, 155, NULL, SLAKOTH_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  288, "Vigoroth", TYPE_NORMAL, TYPECOUNT, 159, 145, 190, "Slakoth", VIGOROTH_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  289, "Slaking", TYPE_NORMAL, TYPECOUNT, 290, 166, 284, "Vigoroth", SLAKING_ATKS, true, true,
    { &ATK_Body_Slam, }, species::CAT_NORMAL, 75, nullptr, },
  {  290, "Nincada", TYPE_BUG, TYPE_GROUND, 80, 126, 104, NULL, NINCADA_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  291, "Ninjask", TYPE_BUG, TYPE_FLYING, 199, 112, 156, "Nincada", NINJASK_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  292, "Shedinja", TYPE_BUG, TYPE_GHOST, 153, 73, 1, NULL, SHEDINJA_ATKS, true, false,
    { &ATK_Struggle_Bug, }, species::CAT_NORMAL, 75, nullptr, },
  {  293, "Whismur", TYPE_NORMAL, TYPECOUNT, 92, 42, 162, NULL, WHISMUR_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  294, "Loudred", TYPE_NORMAL, TYPECOUNT, 134, 81, 197, "Whismur", LOUDRED_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  295, "Exploud", TYPE_NORMAL, TYPECOUNT, 179, 137, 232, "Loudred", EXPLOUD_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  296, "Makuhita", TYPE_FIGHTING, TYPECOUNT, 99, 54, 176, NULL, MAKUHITA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  297, "Hariyama", TYPE_FIGHTING, TYPECOUNT, 209, 114, 302, "Makuhita", HARIYAMA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  298, "Azurill", TYPE_NORMAL, TYPE_FAIRY, 36, 71, 137, NULL, AZURILL_ATKS, true, false, {}, species::CAT_BABY, 10, nullptr, },
  {  299, "Nosepass", TYPE_ROCK, TYPECOUNT, 82, 215, 102, NULL, NOSEPASS_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  300, "Skitty", TYPE_NORMAL, TYPECOUNT, 84, 79, 137, NULL, SKITTY_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  301, "Delcatty", TYPE_NORMAL, TYPECOUNT, 132, 127, 172, "Skitty", DELCATTY_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  302, "Sableye", TYPE_DARK, TYPE_GHOST, 141, 136, 137, NULL, SABLEYE_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  303, "Mawile", TYPE_STEEL, TYPE_FAIRY, 155, 141, 137, NULL, MAWILE_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  304, "Aron", TYPE_STEEL, TYPE_ROCK, 121, 141, 137, NULL, ARON_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  305, "Lairon", TYPE_STEEL, TYPE_ROCK, 158, 198, 155, "Aron", LAIRON_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  306, "Aggron", TYPE_STEEL, TYPE_ROCK, 198, 257, 172, "Lairon", AGGRON_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  307, "Meditite", TYPE_FIGHTING, TYPE_PSYCHIC, 78, 107, 102, NULL, MEDITITE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  308, "Medicham", TYPE_FIGHTING, TYPE_PSYCHIC, 121, 152, 155, "Meditite", MEDICHAM_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  309, "Electrike", TYPE_ELECTRIC, TYPECOUNT, 123, 78, 120, NULL, ELECTRIKE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  310, "Manectric", TYPE_ELECTRIC, TYPECOUNT, 215, 127, 172, "Electrike", MANECTRIC_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  311, "Plusle", TYPE_ELECTRIC, TYPECOUNT, 167, 129, 155, NULL, PLUSLE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  312, "Minun", TYPE_ELECTRIC, TYPECOUNT, 147, 150, 155, NULL, MINUN_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  313, "Volbeat", TYPE_BUG, TYPECOUNT, 143, 166, 163, NULL, VOLBEAT_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  314, "Illumise", TYPE_BUG, TYPECOUNT, 143, 166, 163, NULL, ILLUMISE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  315, "Roselia", TYPE_GRASS, TYPE_POISON, 186, 131, 137, "Budew", ROSELIA_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  316, "Gulpin", TYPE_POISON, TYPECOUNT, 80, 99, 172, NULL, GULPIN_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  317, "Swalot", TYPE_POISON, TYPECOUNT, 140, 159, 225, "Gulpin", SWALOT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  318, "Carvanha", TYPE_WATER, TYPE_DARK, 171, 39, 128, NULL, CARVANHA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  319, "Sharpedo", TYPE_WATER, TYPE_DARK, 243, 83, 172, "Carvanha", SHARPEDO_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  320, "Wailmer", TYPE_WATER, TYPECOUNT, 136, 68, 277, NULL, WAILMER_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  321, "Wailord", TYPE_WATER, TYPECOUNT, 175, 87, 347, "Wailmer", WAILORD_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  322, "Numel", TYPE_FIRE, TYPE_GROUND, 119, 79, 155, NULL, NUMEL_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  323, "Camerupt", TYPE_FIRE, TYPE_GROUND, 194, 136, 172, "Numel", CAMERUPT_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  324, "Torkoal", TYPE_FIRE, TYPECOUNT, 151, 203, 172, NULL, TORKOAL_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  325, "Spoink", TYPE_PSYCHIC, TYPECOUNT, 125, 122, 155, NULL, SPOINK_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  326, "Grumpig", TYPE_PSYCHIC, TYPECOUNT, 171, 188, 190, "Spoink", GRUMPIG_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  327, "Spinda", TYPE_NORMAL, TYPECOUNT, 116, 116, 155, NULL, SPINDA_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  328, "Trapinch", TYPE_GROUND, TYPECOUNT, 162, 78, 128, NULL, TRAPINCH_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  329, "Vibrava", TYPE_GROUND, TYPE_DRAGON, 134, 99, 137, "Trapinch", VIBRAVA_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  330, "Flygon", TYPE_GROUND, TYPE_DRAGON, 205, 168, 190, "Vibrava", FLYGON_ATKS, true, true,
    { &ATK_Earth_Power, }, species::CAT_NORMAL, 75, nullptr, },
  {  331, "Cacnea", TYPE_GRASS, TYPECOUNT, 156, 74, 137, NULL, CACNEA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  332, "Cacturne", TYPE_GRASS, TYPE_DARK, 221, 115, 172, "Cacnea", CACTURNE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  333, "Swablu", TYPE_NORMAL, TYPE_FLYING, 76, 132, 128, NULL, SWABLU_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  334, "Altaria", TYPE_DRAGON, TYPE_FLYING, 141, 201, 181, "Swablu", ALTARIA_ATKS, true, false,
    { &ATK_Moonblast, }, species::CAT_NORMAL, 10, nullptr, },
  {  335, "Zangoose", TYPE_NORMAL, TYPECOUNT, 222, 124, 177, NULL, ZANGOOSE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  336, "Seviper", TYPE_POISON, TYPECOUNT, 196, 118, 177, NULL, SEVIPER_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  337, "Lunatone", TYPE_ROCK, TYPE_PSYCHIC, 178, 153, 207, NULL, LUNATONE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  338, "Solrock", TYPE_ROCK, TYPE_PSYCHIC, 178, 153, 207, NULL, SOLROCK_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  339, "Barboach", TYPE_WATER, TYPE_GROUND, 93, 82, 137, NULL, BARBOACH_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  340, "Whiscash", TYPE_WATER, TYPE_GROUND, 151, 141, 242, "Barboach", WHISCASH_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  341, "Corphish", TYPE_WATER, TYPECOUNT, 141, 99, 125, NULL, CORPHISH_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  342, "Crawdaunt", TYPE_WATER, TYPE_DARK, 224, 142, 160, "Corphish", CRAWDAUNT_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  343, "Baltoy", TYPE_GROUND, TYPE_PSYCHIC, 77, 124, 120, NULL, BALTOY_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  344, "Claydol", TYPE_GROUND, TYPE_PSYCHIC, 140, 229, 155, "Baltoy", CLAYDOL_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  345, "Lileep", TYPE_ROCK, TYPE_GRASS, 105, 150, 165, NULL, LILEEP_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  346, "Cradily", TYPE_ROCK, TYPE_GRASS, 152, 194, 200, "Lileep", CRADILY_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  347, "Anorith", TYPE_ROCK, TYPE_BUG, 176, 100, 128, NULL, ANORITH_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  348, "Armaldo", TYPE_ROCK, TYPE_BUG, 222, 174, 181, "Anorith", ARMALDO_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  349, "Feebas", TYPE_WATER, TYPECOUNT, 29, 85, 85, NULL, FEEBAS_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  350, "Milotic", TYPE_WATER, TYPECOUNT, 192, 219, 216, "Feebas", MILOTIC_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  351, "Castform", TYPE_NORMAL, TYPECOUNT, 139, 139, 172, NULL, CASTFORM_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  352, "Kecleon", TYPE_NORMAL, TYPECOUNT, 161, 189, 155, NULL, KECLEON_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  353, "Shuppet", TYPE_GHOST, TYPECOUNT, 138, 65, 127, NULL, SHUPPET_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  354, "Banette", TYPE_GHOST, TYPECOUNT, 218, 126, 162, "Shuppet", BANETTE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  355, "Duskull", TYPE_GHOST, TYPECOUNT, 70, 162, 85, NULL, DUSKULL_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  356, "Dusclops", TYPE_GHOST, TYPECOUNT, 124, 234, 120, "Duskull", DUSCLOPS_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  357, "Tropius", TYPE_GRASS, TYPE_FLYING, 136, 163, 223, NULL, TROPIUS_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  358, "Chimecho", TYPE_PSYCHIC, TYPECOUNT, 175, 170, 181, "Chingling", CHIMECHO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  359, "Absol", TYPE_DARK, TYPECOUNT, 246, 120, 163, NULL, ABSOL_ATKS, true, true,
    { &ATK_Brutal_Swing, }, species::CAT_NORMAL, 75, nullptr, },
  {  360, "Wynaut", TYPE_PSYCHIC, TYPECOUNT, 41, 86, 216, NULL, WYNAUT_ATKS, true, false, {}, species::CAT_BABY, 10, nullptr, },
  {  361, "Snorunt", TYPE_ICE, TYPECOUNT, 95, 95, 137, NULL, SNORUNT_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  362, "Glalie", TYPE_ICE, TYPECOUNT, 162, 162, 190, "Snorunt", GLALIE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  363, "Spheal", TYPE_ICE, TYPE_WATER, 95, 90, 172, NULL, SPHEAL_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  364, "Sealeo", TYPE_ICE, TYPE_WATER, 137, 132, 207, "Spheal", SEALEO_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  365, "Walrein", TYPE_ICE, TYPE_WATER, 182, 176, 242, "Sealeo", WALREIN_ATKS, true, true,
    { &ATK_Powder_Snow, &ATK_Icicle_Spear, }, species::CAT_NORMAL, 50, nullptr, },
  {  366, "Clamperl", TYPE_WATER, TYPECOUNT, 133, 135, 111, NULL, CLAMPERL_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  367, "Huntail", TYPE_WATER, TYPECOUNT, 197, 179, 146, "Clamperl", HUNTAIL_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  368, "Gorebyss", TYPE_WATER, TYPECOUNT, 211, 179, 146, "Clamperl", GOREBYSS_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  369, "Relicanth", TYPE_WATER, TYPE_ROCK, 162, 203, 225, NULL, RELICANTH_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  370, "Luvdisc", TYPE_WATER, TYPECOUNT, 81, 128, 125, NULL, LUVDISC_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  371, "Bagon", TYPE_DRAGON, TYPECOUNT, 134, 93, 128, NULL, BAGON_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  372, "Shelgon", TYPE_DRAGON, TYPECOUNT, 172, 155, 163, "Bagon", SHELGON_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  373, "Salamence", TYPE_DRAGON, TYPE_FLYING, 277, 168, 216, "Shelgon", SALAMENCE_ATKS, true, true,
    { &ATK_Outrage, }, species::CAT_NORMAL, 75, nullptr, },
  {  374, "Beldum", TYPE_STEEL, TYPE_PSYCHIC, 96, 132, 120, NULL, BELDUM_ATKS, true, true,{}, species::CAT_NORMAL, 75, nullptr, },
  {  375, "Metang", TYPE_STEEL, TYPE_PSYCHIC, 138, 176, 155, "Beldum", METANG_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  376, "Metagross", TYPE_STEEL, TYPE_PSYCHIC, 257, 228, 190, "Metang", METAGROSS_ATKS, true, true,
    { &ATK_Meteor_Mash, }, species::CAT_NORMAL, 75, nullptr, },
  {  377, "Regirock", TYPE_ROCK, TYPECOUNT, 179, 309, 190, NULL, REGIROCK_ATKS, true, true,
    { &ATK_Earthquake, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  378, "Regice", TYPE_ICE, TYPECOUNT, 179, 309, 190, NULL, REGICE_ATKS, true, true,
    { &ATK_Thunder, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  379, "Registeel", TYPE_STEEL, TYPECOUNT, 143, 285, 190, NULL, REGISTEEL_ATKS, true, true,
    { &ATK_Zap_Cannon, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  380, "Latias", TYPE_DRAGON, TYPE_PSYCHIC, 228, 246, 190, NULL, LATIAS_ATKS, true, true,
    { &ATK_Mist_Ball, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  381, "Latios", TYPE_DRAGON, TYPE_PSYCHIC, 268, 212, 190, NULL, LATIOS_ATKS, true, true,
    { &ATK_Luster_Purge, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  382, "Kyogre", TYPE_WATER, TYPECOUNT, 270, 228, 205, NULL, KYOGRE_ATKS, true, true,
    { &ATK_Origin_Pulse, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  383, "Groudon", TYPE_GROUND, TYPECOUNT, 270, 228, 205, NULL, GROUDON_ATKS, true, true,
    { &ATK_Fire_Punch, &ATK_Precipice_Blades, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  384, "Rayquaza", TYPE_DRAGON, TYPE_FLYING, 284, 170, 213, NULL, RAYQUAZA_ATKS, true, false,
    { &ATK_Breaking_Swipe, &ATK_Dragon_Ascent, &ATK_Hurricane, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  385, "Jirachi", TYPE_STEEL, TYPE_PSYCHIC, 210, 210, 225, NULL, JIRACHI_ATKS, true, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  386, "Deoxys", TYPE_PSYCHIC, TYPECOUNT, 345, 115, 137, NULL, DEOXYS_ATKS, true, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  386, "Deoxys Attack", TYPE_PSYCHIC, TYPECOUNT, 414, 46, 137, NULL, A_DEOXYS_ATKS, true, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  386, "Deoxys Defense", TYPE_PSYCHIC, TYPECOUNT, 144, 330, 137, NULL, D_DEOXYS_ATKS, true, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  386, "Deoxys Speed", TYPE_PSYCHIC, TYPECOUNT, 230, 218, 137, NULL, S_DEOXYS_ATKS, true, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  387, "Turtwig", TYPE_GRASS, TYPECOUNT, 119, 110, 146, NULL, TURTWIG_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  388, "Grotle", TYPE_GRASS, TYPECOUNT, 157, 143, 181, "Turtwig", GROTLE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  389, "Torterra", TYPE_GRASS, TYPE_GROUND, 202, 188, 216, "Grotle", TORTERRA_ATKS, true, true,
    { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, },
  {  390, "Chimchar", TYPE_FIRE, TYPECOUNT, 113, 86, 127, NULL, CHIMCHAR_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  391, "Monferno", TYPE_FIRE, TYPE_FIGHTING, 158, 105, 162, "Chimchar", MONFERNO_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  392, "Infernape", TYPE_FIRE, TYPE_FIGHTING, 222, 151, 183, "Monferno", INFERNAPE_ATKS, true, true,
    { &ATK_Blast_Burn, }, species::CAT_NORMAL, 10, nullptr, },
  {  393, "Piplup", TYPE_WATER, TYPECOUNT, 112, 102, 142, NULL, PIPLUP_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  394, "Prinplup", TYPE_WATER, TYPECOUNT, 150, 139, 162, "Piplup", PRINPLUP_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  395, "Empoleon", TYPE_WATER, TYPE_STEEL, 210, 186, 197, "Prinplup", EMPOLEON_ATKS, true, true,
    { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, },
  {  396, "Starly", TYPE_NORMAL, TYPE_FLYING, 101, 58, 120, NULL, STARLY_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  397, "Staravia", TYPE_NORMAL, TYPE_FLYING, 142, 94, 146, "Starly", STARAVIA_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  398, "Staraptor", TYPE_NORMAL, TYPE_FLYING, 234, 140, 198, "Staravia", STARAPTOR_ATKS, true, true,
    { &ATK_Gust, }, species::CAT_NORMAL, 10, nullptr, },
  {  399, "Bidoof", TYPE_NORMAL, TYPECOUNT, 80, 73, 153, NULL, BIDOOF_ATKS, true, true,
    { &ATK_Ice_Beam, &ATK_Shadow_Ball, &ATK_Superpower, &ATK_Thunderbolt, }, species::CAT_NORMAL, 10, nullptr, },
  {  400, "Bibarel", TYPE_NORMAL, TYPE_WATER, 162, 119, 188, "Bidoof", BIBAREL_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  401, "Kricketot", TYPE_BUG, TYPECOUNT, 45, 74, 114, NULL, KRICKETOT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  402, "Kricketune", TYPE_BUG, TYPECOUNT, 160, 100, 184, "Kricketot", KRICKETUNE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  403, "Shinx", TYPE_ELECTRIC, TYPECOUNT, 117, 64, 128, NULL, SHINX_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  404, "Luxio", TYPE_ELECTRIC, TYPECOUNT, 159, 95, 155, "Shinx", LUXIO_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  405, "Luxray", TYPE_ELECTRIC, TYPECOUNT, 232, 156, 190, "Luxio", LUXRAY_ATKS, true, true,
    { &ATK_Psychic_Fangs, }, species::CAT_NORMAL, 75, nullptr, },
  {  406, "Budew", TYPE_GRASS, TYPE_POISON, 91, 109, 120, NULL, BUDEW_ATKS, true, false, {}, species::CAT_BABY, 10, nullptr, },
  {  407, "Roserade", TYPE_GRASS, TYPE_POISON, 243, 185, 155, "Roselia", ROSERADE_ATKS, true, false,
    { &ATK_Bullet_Seed, &ATK_Weather_Ball_Fire, }, species::CAT_NORMAL, 50, nullptr, },
  {  408, "Cranidos", TYPE_ROCK, TYPECOUNT, 218, 71, 167, NULL, CRANIDOS_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  409, "Rampardos", TYPE_ROCK, TYPECOUNT, 295, 109, 219, "Cranidos", RAMPARDOS_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  410, "Shieldon", TYPE_ROCK, TYPE_STEEL, 76, 195, 102, NULL, SHIELDON_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  411, "Bastiodon", TYPE_ROCK, TYPE_STEEL, 94, 286, 155, "Shieldon", BASTIODON_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  // plant, sandy, and trash burmy all have the same stats and attacks, but
  // different evolution targets which *do* have different stats.
  {  412, "Burmy", TYPE_BUG, TYPECOUNT, 53, 83, 120, NULL, BURMY_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  413, "Plant Wormadam", TYPE_BUG, TYPE_GRASS, 141, 180, 155, "Burmy", P_WORMADAM_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  413, "Sandy Wormadam", TYPE_BUG, TYPE_GROUND, 141, 180, 155, "Burmy", S_WORMADAM_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  413, "Trash Wormadam", TYPE_BUG, TYPE_STEEL, 127, 175, 155, "Burmy", T_WORMADAM_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  414, "Mothim", TYPE_BUG, TYPE_FLYING, 185, 98, 172, "Burmy", MOTHIM_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  415, "Combee", TYPE_BUG, TYPE_FLYING, 59, 83, 102, NULL, COMBEE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  416, "Vespiquen", TYPE_BUG, TYPE_FLYING, 149, 190, 172, "Combee", VESPIQUEN_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  417, "Pachirisu", TYPE_ELECTRIC, TYPECOUNT, 94, 172, 155, NULL, PACHIRISU_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  418, "Buizel", TYPE_WATER, TYPECOUNT, 132, 67, 146, NULL, BUIZEL_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  419, "Floatzel", TYPE_WATER, TYPECOUNT, 221, 114, 198, "Buizel", FLOATZEL_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  420, "Cherubi", TYPE_GRASS, TYPECOUNT, 108, 92, 128, NULL, CHERUBI_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  421, "Cherrim", TYPE_GRASS, TYPECOUNT, 170, 153, 172, "Cherubi", CHERRIM_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  421, "Cherrim (Overcast Form)", TYPE_GRASS, TYPECOUNT, 170, 153, 172, "Cherubi", O_CHERRIM_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  422, "Shellos", TYPE_WATER, TYPECOUNT, 103, 105, 183, NULL, SHELLOS_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  423, "Gastrodon", TYPE_WATER, TYPE_GROUND, 169, 143, 244, "Shellos", GASTRODON_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  424, "Ambipom", TYPE_NORMAL, TYPECOUNT, 205, 143, 181, "Aipom", AMBIPOM_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  425, "Drifloon", TYPE_GHOST, TYPE_FLYING, 117, 80, 207, NULL, DRIFLOON_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  426, "Drifblim", TYPE_GHOST, TYPE_FLYING, 180, 102, 312, "Drifloon", DRIFBLIM_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  427, "Buneary", TYPE_NORMAL, TYPECOUNT, 130, 105, 146, NULL, BUNEARY_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  428, "Lopunny", TYPE_NORMAL, TYPECOUNT, 156, 194, 163, "Buneary", LOPUNNY_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  429, "Mismagius", TYPE_GHOST, TYPECOUNT, 211, 187, 155, "Misdreavus", MISMAGIUS_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  430, "Honchkrow", TYPE_DARK, TYPE_FLYING, 243, 103, 225, "Murkrow", HONCHKROW_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  431, "Glameow", TYPE_NORMAL, TYPECOUNT, 109, 82, 135, NULL, GLAMEOW_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  432, "Purugly", TYPE_NORMAL, TYPECOUNT, 172, 133, 174, "Glameow", PURUGLY_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  433, "Chingling", TYPE_PSYCHIC, TYPECOUNT, 114, 94, 128, NULL, CHINGLING_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  434, "Stunky", TYPE_POISON, TYPE_DARK, 121, 90, 160, NULL, STUNKY_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  435, "Skuntank", TYPE_POISON, TYPE_DARK, 184, 132, 230, "Stunky", SKUNTANK_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  436, "Bronzor", TYPE_STEEL, TYPE_PSYCHIC, 43, 154, 149, NULL, BRONZOR_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  437, "Bronzong", TYPE_STEEL, TYPE_PSYCHIC, 161, 213, 167, "Bronzor", BRONZONG_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  438, "Bonsly", TYPE_ROCK, TYPECOUNT, 124, 133, 137, NULL, BONSLY_ATKS, true, false, {}, species::CAT_BABY, 10, nullptr, },
  {  439, "Mime Jr.", TYPE_PSYCHIC, TYPE_FAIRY, 125, 142, 85, NULL, MIMEJR_ATKS, true, false, {}, species::CAT_BABY, 10, nullptr, },
  {  440, "Happiny", TYPE_NORMAL, TYPECOUNT, 25, 77, 225, NULL, HAPPINY_ATKS, true, false, {}, species::CAT_BABY, 10, nullptr, },
  {  441, "Chatot", TYPE_NORMAL, TYPE_FLYING, 183, 91, 183, NULL, CHATOT_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  442, "Spiritomb", TYPE_GHOST, TYPE_DARK, 169, 199, 137, NULL, SPIRITOMB_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  443, "Gible", TYPE_DRAGON, TYPE_GROUND, 124, 84, 151, NULL, GIBLE_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  444, "Gabite", TYPE_DRAGON, TYPE_GROUND, 172, 125, 169, "Gible", GABITE_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  445, "Garchomp", TYPE_DRAGON, TYPE_GROUND, 261, 193, 239, "Gabite", GARCHOMP_ATKS, true, true,
    { &ATK_Earth_Power, }, species::CAT_NORMAL, 75, nullptr, },
  {  446, "Munchlax", TYPE_NORMAL, TYPECOUNT, 137, 117, 286, NULL, MUNCHLAX_ATKS, true, false, {}, species::CAT_BABY, 10, nullptr, },
  {  447, "Riolu", TYPE_FIGHTING, TYPECOUNT, 127, 78, 120, NULL, RIOLU_ATKS, true, false, {}, species::CAT_BABY, 10, nullptr, },
  {  448, "Lucario", TYPE_FIGHTING, TYPE_STEEL, 236, 144, 172, "Riolu", LUCARIO_ATKS, true, false,
    { &ATK_Force_Palm, }, species::CAT_NORMAL, 75, nullptr, },
  {  449, "Hippopotas", TYPE_GROUND, TYPECOUNT, 124, 118, 169, NULL, HIPPOPOTAS_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  450, "Hippowdon", TYPE_GROUND, TYPECOUNT, 201, 191, 239, "Hippopotas", HIPPOWDON_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  451, "Skorupi", TYPE_POISON, TYPE_BUG, 93, 151, 120, NULL, SKORUPI_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  452, "Drapion", TYPE_POISON, TYPE_DARK, 180, 202, 172, "Skorupi", DRAPION_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  453, "Croagunk", TYPE_POISON, TYPE_FIGHTING, 116, 76, 134, NULL, CROAGUNK_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  454, "Toxicroak", TYPE_POISON, TYPE_FIGHTING, 211, 133, 195, "Croagunk", TOXICROAK_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  455, "Carnivine", TYPE_GRASS, TYPECOUNT, 187, 136, 179, NULL, CARNIVINE_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  456, "Finneon", TYPE_WATER, TYPECOUNT, 96, 116, 135, NULL, FINNEON_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  457, "Lumineon", TYPE_WATER, TYPECOUNT, 142, 170, 170, "Finneon", LUMINEON_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  458, "Mantyke", TYPE_WATER, TYPE_FLYING, 105, 179, 128, NULL, MANTYKE_ATKS, true, false, {}, species::CAT_BABY, 10, nullptr, },
  {  459, "Snover", TYPE_GRASS, TYPE_ICE, 115, 105, 155, NULL, SNOVER_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  460, "Abomasnow", TYPE_GRASS, TYPE_ICE, 178, 158, 207, "Snover", ABOMASNOW_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  461, "Weavile", TYPE_DARK, TYPE_ICE, 243, 171, 172, "Sneasel", WEAVILE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  462, "Magnezone", TYPE_ELECTRIC, TYPE_STEEL, 238, 205, 172, "Magneton", MAGNEZONE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  463, "Lickilicky", TYPE_NORMAL, TYPECOUNT, 161, 181, 242, "Lickitung", LICKILICKY_ATKS, true, false,
    { &ATK_Body_Slam, }, species::CAT_NORMAL, 50, nullptr, },
  {  464, "Rhyperior", TYPE_GROUND, TYPE_ROCK, 241, 190, 251, "Rhydon", RHYPERIOR_ATKS, true, true,
    { &ATK_Rock_Wrecker, }, species::CAT_NORMAL, 50, nullptr, },
  {  465, "Tangrowth", TYPE_GRASS, TYPECOUNT, 207, 184, 225, "Tangela", TANGROWTH_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  466, "Electivire", TYPE_ELECTRIC, TYPECOUNT, 249, 163, 181, "Electabuzz", ELECTIVIRE_ATKS, true, true,
    { &ATK_Flamethrower, }, species::CAT_NORMAL, 75, nullptr, },
  {  467, "Magmortar", TYPE_FIRE, TYPECOUNT, 247, 172, 181, "Magmar", MAGMORTAR_ATKS, true, true,
    { &ATK_Thunderbolt, }, species::CAT_NORMAL, 75, nullptr, },
  {  468, "Togekiss", TYPE_FAIRY, TYPE_FLYING, 225, 217, 198, "Togetic", TOGEKISS_ATKS, true, false,
    { &ATK_Aura_Sphere, }, species::CAT_NORMAL, 50, nullptr, },
  {  469, "Yanmega", TYPE_BUG, TYPE_FLYING, 231, 156, 200, "Yanma", YANMEGA_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  470, "Leafeon", TYPE_GRASS, TYPECOUNT, 216, 219, 163, "Eevee", LEAFEON_ATKS, true, false,
    { &ATK_Bullet_Seed, &ATK_Last_Resort, }, species::CAT_NORMAL, 75, nullptr, },
  {  471, "Glaceon", TYPE_ICE, TYPECOUNT, 238, 205, 163, "Eevee", GLACEON_ATKS, true, false,
    { &ATK_Water_Pulse, &ATK_Last_Resort, }, species::CAT_NORMAL, 75, nullptr, },
  {  472, "Gliscor", TYPE_GROUND, TYPE_FLYING, 185, 222, 181, "Gligar", GLISCOR_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  473, "Mamoswine", TYPE_ICE, TYPE_GROUND, 247, 146, 242, "Piloswine", MAMOSWINE_ATKS, true, true,
    { &ATK_Ancient_Power, }, species::CAT_NORMAL, 50, nullptr, },
  {  474, "Porygon-Z", TYPE_NORMAL, TYPECOUNT, 264, 150, 198, "Porygon2", PORYGONZ_ATKS, true, true,
    { &ATK_Tri_Attack, }, species::CAT_NORMAL, 50, nullptr, },
  {  475, "Gallade", TYPE_PSYCHIC, TYPE_FIGHTING, 237, 195, 169, "Kirlia", GALLADE_ATKS, true, true,
    { &ATK_Synchronoise, }, species::CAT_NORMAL, 75, nullptr, },
  {  476, "Probopass", TYPE_ROCK, TYPE_STEEL, 135, 275, 155, "Nosepass", PROBOPASS_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  477, "Dusknoir", TYPE_GHOST, TYPECOUNT, 180, 254, 128, "Dusclops", DUSKNOIR_ATKS, true, true,
    { &ATK_Shadow_Ball, }, species::CAT_NORMAL, 50, nullptr, },
  {  478, "Froslass", TYPE_ICE, TYPE_GHOST, 171, 150, 172, "Snorunt", FROSLASS_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  479, "Rotom", TYPE_ELECTRIC, TYPE_GHOST, 185, 159, 137, NULL, ROTOM_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  479, "Heat Rotom", TYPE_ELECTRIC, TYPE_FIRE, 204, 219, 137, NULL, HEAT_ROTOM_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  479, "Wash Rotom", TYPE_ELECTRIC, TYPE_WATER, 204, 219, 137, NULL, WASH_ROTOM_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  479, "Frost Rotom", TYPE_ELECTRIC, TYPE_ICE, 204, 219, 137, NULL, FROST_ROTOM_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  479, "Fan Rotom", TYPE_ELECTRIC, TYPE_FLYING, 204, 219, 137, NULL, FAN_ROTOM_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  479, "Mow Rotom", TYPE_ELECTRIC, TYPE_GRASS, 204, 219, 137, NULL, MOW_ROTOM_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  480, "Uxie", TYPE_PSYCHIC, TYPECOUNT, 156, 270, 181, NULL, UXIE_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  481, "Mesprit", TYPE_PSYCHIC, TYPECOUNT, 212, 212, 190, NULL, MESPRIT_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  482, "Azelf", TYPE_PSYCHIC, TYPECOUNT, 270, 151, 181, NULL, AZELF_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  483, "Dialga", TYPE_STEEL, TYPE_DRAGON, 275, 211, 205, NULL, DIALGA_ATKS, true, true, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  483, "Origin Forme Dialga", TYPE_STEEL, TYPE_DRAGON, 270, 225, 205, NULL, O_DIALGA_ATKS, true, true, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  484, "Palkia", TYPE_WATER, TYPE_DRAGON, 280, 215, 189, NULL, PALKIA_ATKS, true, true, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  484, "Origin Forme Palkia", TYPE_WATER, TYPE_DRAGON, 286, 223, 189, NULL, O_PALKIA_ATKS, true, true, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  485, "Heatran", TYPE_FIRE, TYPE_STEEL, 251, 213, 209, NULL, HEATRAN_ATKS, true, true,
    { &ATK_Magma_Storm, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  486, "Regigigas", TYPE_NORMAL, TYPECOUNT, 287, 210, 221, NULL, REGIGIGAS_ATKS, true, true, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  487, "Altered Forme Giratina", TYPE_GHOST, TYPE_DRAGON, 187, 225, 284, NULL, A_GIRATINA_ATKS, true, false,
    { &ATK_Shadow_Force, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  487, "Origin Forme Giratina", TYPE_GHOST, TYPE_DRAGON, 225, 187, 284, NULL, O_GIRATINA_ATKS, true, false,
    { &ATK_Shadow_Force, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  488, "Cresselia", TYPE_PSYCHIC, TYPECOUNT, 152, 258, 260, NULL, CRESSELIA_ATKS, true, false,
    { &ATK_Grass_Knot, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  491, "Darkrai", TYPE_DARK, TYPECOUNT, 285, 198, 172, NULL, DARKRAI_ATKS, true, false,
    { &ATK_Sludge_Bomb, }, species::CAT_MYTHICAL, 100, nullptr, },
  {  492, "Shaymin", TYPE_GRASS, TYPECOUNT, 210, 210, 225, NULL, SHAYMIN_ATKS, true, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  492, "Sky Shaymin", TYPE_GRASS, TYPE_FLYING, 261, 166, 225, NULL, S_SHAYMIN_ATKS, true, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  494, "Victini", TYPE_PSYCHIC, TYPE_FIRE, 210, 210, 225, NULL, VICTINI_ATKS, false, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  495, "Snivy", TYPE_GRASS, TYPECOUNT, 88, 107, 128, NULL, SNIVY_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  496, "Servine", TYPE_GRASS, TYPECOUNT, 122, 152, 155, "Snivy", SERVINE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  497, "Serperior", TYPE_GRASS, TYPECOUNT, 161, 204, 181, "Servine", SERPERIOR_ATKS, true, false,
    { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, },
  {  498, "Tepig", TYPE_FIRE, TYPECOUNT, 115, 85, 163, NULL, TEPIG_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  499, "Pignite", TYPE_FIRE, TYPE_FIGHTING, 173, 106, 207, "Tepig", PIGNITE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  500, "Emboar", TYPE_FIRE, TYPE_FIGHTING, 235, 127, 242, "Pignite", EMBOAR_ATKS, true, true,
    { &ATK_Blast_Burn, }, species::CAT_NORMAL, 10, nullptr, },
  {  501, "Oshawott", TYPE_WATER, TYPECOUNT, 117, 85, 146, NULL, OSHAWOTT_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  502, "Dewott", TYPE_WATER, TYPECOUNT, 159, 116, 181, "Oshawott", DEWOTT_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  503, "Samurott", TYPE_WATER, TYPECOUNT, 212, 157, 216, "Dewott", SAMUROTT_ATKS, true, true,
    { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, },
  {  503, "Hisuian Samurott", TYPE_WATER, TYPE_DARK, 218, 152, 207, NULL, H_SAMUROTT_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  504, "Patrat", TYPE_NORMAL, TYPECOUNT, 98, 73, 128, NULL, PATRAT_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  505, "Watchog", TYPE_NORMAL, TYPECOUNT, 165, 139, 155, "Patrat", WATCHOG_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  506, "Lillipup", TYPE_NORMAL, TYPECOUNT, 107, 86, 128, NULL, LILLIPUP_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  507, "Herdier", TYPE_NORMAL, TYPECOUNT, 145, 126, 163, "Lillipup", HERDIER_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  508, "Stoutland", TYPE_NORMAL, TYPECOUNT, 206, 182, 198, "Herdier", STOUTLAND_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  509, "Purrloin", TYPE_DARK, TYPECOUNT, 98, 73, 121, NULL, PURRLOIN_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  510, "Liepard", TYPE_DARK, TYPECOUNT, 187, 106, 162, "Purrloin", LIEPARD_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  511, "Pansage", TYPE_GRASS, TYPECOUNT, 104, 94, 137, NULL, PANSAGE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  512, "Simisage", TYPE_GRASS, TYPECOUNT, 206, 133, 181, "Pansage", SIMISAGE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  513, "Pansear", TYPE_FIRE, TYPECOUNT, 104, 94, 137, NULL, PANSEAR_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  514, "Simisear", TYPE_FIRE, TYPECOUNT, 206, 133, 181, "Pansear", SIMISEAR_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  515, "Panpour", TYPE_WATER, TYPECOUNT, 104, 94, 137, NULL, PANPOUR_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  516, "Simipour", TYPE_WATER, TYPECOUNT, 206, 133, 181, "Panpour", SIMIPOUR_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  517, "Munna", TYPE_PSYCHIC, TYPECOUNT, 111, 92, 183, NULL, MUNNA_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  518, "Musharna", TYPE_PSYCHIC, TYPECOUNT, 183, 166, 253, "Munna", MUSHARNA_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  519, "Pidove", TYPE_NORMAL, TYPE_FLYING, 98, 80, 137, NULL, PIDOVE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  520, "Tranquill", TYPE_NORMAL, TYPE_FLYING, 144, 107, 158, "Pidove", TRANQUILL_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  521, "Unfezant", TYPE_NORMAL, TYPE_FLYING, 226, 146, 190, "Tranquill", UNFEZANT_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  522, "Blitzle", TYPE_ELECTRIC, TYPECOUNT, 118, 64, 128, NULL, BLITZLE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  523, "Zebstrika", TYPE_ELECTRIC, TYPECOUNT, 211, 136, 181, "Blitzle", ZEBSTRIKA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  524, "Roggenrola", TYPE_ROCK, TYPECOUNT, 121, 110, 146, NULL, ROGGENROLA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  525, "Boldore", TYPE_ROCK, TYPECOUNT, 174, 143, 172, "Roggenrola", BOLDORE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  526, "Gigalith", TYPE_ROCK, TYPECOUNT, 226, 201, 198, "Boldore", GIGALITH_ATKS, true, true,
    { &ATK_Meteor_Beam, }, species::CAT_NORMAL, 50, nullptr, },
  {  527, "Woobat", TYPE_PSYCHIC, TYPE_FLYING, 107, 85, 163, NULL, WOOBAT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  528, "Swoobat", TYPE_PSYCHIC, TYPE_FLYING, 161, 119, 167, "Woobat", SWOOBAT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  529, "Drilbur", TYPE_GROUND, TYPECOUNT, 154, 85, 155, NULL, DRILBUR_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  530, "Excadrill", TYPE_GROUND, TYPE_STEEL, 255, 129, 242, "Drilbur", EXCADRILL_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  531, "Audino", TYPE_NORMAL, TYPECOUNT, 114, 163, 230, NULL, AUDINO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  532, "Timburr", TYPE_FIGHTING, TYPECOUNT, 134, 87, 181, NULL, TIMBURR_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  533, "Gurdurr", TYPE_FIGHTING, TYPECOUNT, 180, 134, 198, "Timburr", GURDURR_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  534, "Conkeldurr", TYPE_FIGHTING, TYPECOUNT, 243, 158, 233, "Gurdurr", CONKELDURR_ATKS, true, true,
    { &ATK_Brutal_Swing, }, species::CAT_NORMAL, 75, nullptr, },
  {  535, "Tympole", TYPE_WATER, TYPECOUNT, 98, 78, 137, NULL, TYMPOLE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  536, "Palpitoad", TYPE_WATER, TYPE_GROUND, 128, 109, 181, "Tympole", PALPITOAD_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  537, "Seismitoad", TYPE_WATER, TYPE_GROUND, 188, 150, 233, "Palpitoad", SEISMITOAD_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  538, "Throh", TYPE_FIGHTING, TYPECOUNT, 172, 160, 260, NULL, THROH_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  539, "Sawk", TYPE_FIGHTING, TYPECOUNT, 231, 153, 181, NULL, SAWK_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  540, "Sewaddle", TYPE_BUG, TYPE_GRASS, 96, 124, 128, NULL, SEWADDLE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  541, "Swadloon", TYPE_BUG, TYPE_GRASS, 115, 162, 146, "Sewaddle", SWADLOON_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  542, "Leavanny", TYPE_BUG, TYPE_GRASS, 205, 165, 181, "Swadloon", LEAVANNY_ATKS, true, false,
    { &ATK_Shadow_Claw, }, species::CAT_NORMAL, 10, nullptr, },
  {  543, "Venipede", TYPE_BUG, TYPE_POISON, 83, 99, 102, NULL, VENIPEDE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  544, "Whirlipede", TYPE_BUG, TYPE_POISON, 100, 173, 120, "Venipede", WHIRLIPEDE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  545, "Scolipede", TYPE_BUG, TYPE_POISON, 203, 175, 155, "Whirlipede", SCOLIPEDE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  546, "Cottonee", TYPE_GRASS, TYPE_FAIRY, 71, 111, 120, NULL, COTTONEE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  547, "Whimsicott", TYPE_GRASS, TYPE_FAIRY, 164, 176, 155, "Cottonee", WHIMSICOTT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  548, "Petilil", TYPE_GRASS, TYPECOUNT, 119, 91, 128, NULL, PETILIL_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  549, "Lilligant", TYPE_GRASS, TYPECOUNT, 214, 155, 172, "Petilil", LILLIGANT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  549, "Hisuian Lilligant", TYPE_GRASS, TYPECOUNT, 214, 155, 172, NULL, LILLIGANT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  550, "Basculin", TYPE_WATER, TYPECOUNT, 189, 129, 172, NULL, BASCULIN_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  551, "Sandile", TYPE_GROUND, TYPE_DARK, 132, 69, 137, NULL, SANDILE_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  552, "Krokorok", TYPE_GROUND, TYPE_DARK, 155, 90, 155, "Sandile", KROKOROK_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  553, "Krookodile", TYPE_GROUND, TYPE_DARK, 229, 158, 216, "Krokorok", KROOKODILE_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  554, "Darumaka", TYPE_FIRE, TYPECOUNT, 153, 86, 172, NULL, DARUMAKA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  554, "Galarian Darumaka", TYPE_ICE, TYPECOUNT, 153, 86, 172, NULL, G_DARUMAKA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  555, "Darmanitan", TYPE_FIRE, TYPECOUNT, 263, 114, 233, "Darumaka", DARMANITAN_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  555, "Galarian Darmanitan", TYPE_ICE, TYPECOUNT, 263, 114, 233, "Galarian Darumaka", G_DARMANITAN_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  556, "Maractus", TYPE_GRASS, TYPECOUNT, 201, 130, 181, NULL, MARACTUS_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  557, "Dwebble", TYPE_BUG, TYPE_ROCK, 118, 128, 137, NULL, DWEBBLE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  558, "Crustle", TYPE_BUG, TYPE_ROCK, 188, 200, 172, "Dwebble", CRUSTLE_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  559, "Scraggy", TYPE_DARK, TYPE_FIGHTING, 132, 132, 137, NULL, SCRAGGY_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  560, "Scrafty", TYPE_DARK, TYPE_FIGHTING, 163, 222, 163, "Scraggy", SCRAFTY_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  561, "Sigilyph", TYPE_PSYCHIC, TYPE_FLYING, 204, 167, 176, NULL, SIGILYPH_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  562, "Yamask", TYPE_GHOST, TYPECOUNT, 95, 141, 116, NULL, YAMASK_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  562, "Galarian Yamask", TYPE_GROUND, TYPE_GHOST, 95, 141, 116, NULL, G_YAMASK_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  563, "Cofagrigus", TYPE_GHOST, TYPECOUNT, 163, 237, 151, "Yamask", COFAGRIGUS_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  564, "Tirtouga", TYPE_WATER, TYPE_ROCK, 134, 146, 144, NULL, TIRTOUGA_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  565, "Carracosta", TYPE_WATER, TYPE_ROCK, 192, 197, 179, "Tirtouga", CARRACOSTA_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  566, "Archen", TYPE_ROCK, TYPE_FLYING, 213, 89, 146, NULL, ARCHEN_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  567, "Archeops", TYPE_ROCK, TYPE_FLYING, 292, 139, 181, "Archen", ARCHEOPS_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  568, "Trubbish", TYPE_POISON, TYPECOUNT, 96, 122, 137, NULL, TRUBBISH_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  569, "Garbodor", TYPE_POISON, TYPECOUNT, 181, 164, 190, "Trubbish", GARBODOR_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  570, "Zorua", TYPE_DARK, TYPECOUNT, 153, 78, 120, NULL, ZORUA_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  570, "Hisuian Zorua", TYPE_NORMAL, TYPE_GHOST, 162, 79, 111, NULL, HZORUA_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  571, "Zoroark", TYPE_DARK, TYPECOUNT, 250, 127, 155, "Zorua", ZOROARK_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  571, "Hisuian Zoroark", TYPE_NORMAL, TYPE_GHOST, 261, 128, 146, "Hisuian Zorua", HZOROARK_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  572, "Minccino", TYPE_NORMAL, TYPECOUNT, 98, 80, 146, NULL, MINCCINO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  573, "Cinccino", TYPE_NORMAL, TYPECOUNT, 198, 130, 181, "Minccino", CINCCINO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  574, "Gothita", TYPE_PSYCHIC, TYPECOUNT, 98, 112, 128, NULL, GOTHITA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  575, "Gothorita", TYPE_PSYCHIC, TYPECOUNT, 137, 153, 155, "Gothita", GOTHORITA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  576, "Gothitelle", TYPE_PSYCHIC, TYPECOUNT, 176, 205, 172, "Gothorita", GOTHITELLE_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  577, "Solosis", TYPE_PSYCHIC, TYPECOUNT, 170, 83, 128, NULL, SOLOSIS_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  578, "Duosion", TYPE_PSYCHIC, TYPECOUNT, 208, 103, 163, "Solosis", DUOSION_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  579, "Reuniclus", TYPE_PSYCHIC, TYPECOUNT, 214, 148, 242, "Duosion", REUNICLUS_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  580, "Ducklett", TYPE_WATER, TYPE_FLYING, 84, 96, 158, NULL, DUCKLETT_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  581, "Swanna", TYPE_WATER, TYPE_FLYING, 182, 132, 181, "Ducklett", SWANNA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  582, "Vanillite", TYPE_ICE, TYPECOUNT, 118, 106, 113, NULL, VANILLITE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  583, "Vanillish", TYPE_ICE, TYPECOUNT, 151, 138, 139, "Vanillite", VANILLISH_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  584, "Vanilluxe", TYPE_ICE, TYPECOUNT, 218, 184, 174, "Vanillish", VANILLUXE_ATKS, true, false,
    { &ATK_Avalanche, }, species::CAT_NORMAL, 50, nullptr, },
  {  585, "Deerling", TYPE_NORMAL, TYPE_GRASS, 115, 100, 155, NULL, DEERLING_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  586, "Sawsbuck", TYPE_NORMAL, TYPE_GRASS, 198, 146, 190, "Deerling", SAWSBUCK_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  587, "Emolga", TYPE_ELECTRIC, TYPE_FLYING, 158, 127, 146, NULL, EMOLGA_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  588, "Karrablast", TYPE_BUG, TYPECOUNT, 137, 87, 137, NULL, KARRABLAST_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  589, "Escavalier", TYPE_BUG, TYPE_STEEL, 223, 187, 172, "Karrablast", ESCAVALIER_ATKS, true, true,
    { &ATK_Razor_Shell, }, species::CAT_NORMAL, 50, nullptr, },
  {  590, "Foongus", TYPE_GRASS, TYPE_POISON, 97, 91, 170, NULL, FOONGUS_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  591, "Amoonguss", TYPE_GRASS, TYPE_POISON, 155, 139, 249, "Foongus", AMOONGUSS_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  592, "Frillish", TYPE_WATER, TYPE_GHOST, 115, 134, 146, NULL, FRILLISH_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  593, "Jellicent", TYPE_WATER, TYPE_GHOST, 159, 178, 225, "Frillish", JELLICENT_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  594, "Alomomola", TYPE_WATER, TYPECOUNT, 138, 131, 338, NULL, ALOMOMOLA_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  595, "Joltik", TYPE_BUG, TYPE_ELECTRIC, 110, 98, 137, NULL, JOLTIK_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  596, "Galvantula", TYPE_BUG, TYPE_ELECTRIC, 201, 128, 172, "Joltik", GRAVANTULA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  597, "Ferroseed", TYPE_GRASS, TYPE_STEEL, 82, 155, 127, NULL, FERROSEED_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  598, "Ferrothorn", TYPE_GRASS, TYPE_STEEL, 158, 223, 179, "Ferroseed", FERROTHORN_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  599, "Klink", TYPE_STEEL, TYPECOUNT, 98, 121, 120, NULL, KLINK_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  600, "Klang", TYPE_STEEL, TYPECOUNT, 150, 174, 155, "Klink", KLANG_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  601, "Klinklang", TYPE_STEEL, TYPECOUNT, 199, 214, 155, "Klang", KLINKKLANG_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  602, "Tynamo", TYPE_ELECTRIC, TYPECOUNT, 105, 78, 111, NULL, TYNAMO_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  603, "Eelektrik", TYPE_ELECTRIC, TYPECOUNT, 156, 130, 163, "Tynamo", EELEKTRIK_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  604, "Eelektross", TYPE_ELECTRIC, TYPECOUNT, 217, 152, 198, "Eelektrik", EELEKTROSS_ATKS, true, false,
    { &ATK_Volt_Switch, }, species::CAT_NORMAL, 10, nullptr, },
  {  605, "Elgyem", TYPE_PSYCHIC, TYPECOUNT, 148, 100, 146, NULL, ELGYEM_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  606, "Beheeyem", TYPE_PSYCHIC, TYPECOUNT, 221, 163, 181, "Elgyem", BEHEEYEM_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  607, "Litwick", TYPE_GHOST, TYPE_FIRE, 108, 98, 137, NULL, LITWICK_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  608, "Lampent", TYPE_GHOST, TYPE_FIRE, 169, 115, 155, "Litwick", LAMPENT_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  609, "Chandelure", TYPE_GHOST, TYPE_FIRE, 271, 182, 155, "Lampent", CHANDELURE_ATKS, true, true,
    { &ATK_Poltergeist, }, species::CAT_NORMAL, 75, nullptr, },
  {  610, "Axew", TYPE_DRAGON, TYPECOUNT, 154, 101, 130, NULL, AXEW_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  611, "Fraxure", TYPE_DRAGON, TYPECOUNT, 212, 123, 165, "Axew", FRAXURE_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  612, "Haxorus", TYPE_DRAGON, TYPECOUNT, 284, 172, 183, "Fraxure", HAXORUS_ATKS, true, false,
    { &ATK_Breaking_Swipe, }, species::CAT_NORMAL, 75, nullptr, },
  {  613, "Cubchoo", TYPE_ICE, TYPECOUNT, 128, 74, 146, NULL, CUBCHOO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  614, "Beartic", TYPE_ICE, TYPECOUNT, 233, 152, 216, "Cubchoo", BEARTIC_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  615, "Cryogonal", TYPE_ICE, TYPECOUNT, 190, 218, 190, NULL, CRYOGONAL_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  616, "Shelmet", TYPE_BUG, TYPECOUNT, 72, 140, 137, NULL, SHELMET_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  617, "Accelgor", TYPE_BUG, TYPECOUNT, 220, 120, 190, "Shelmet", ACCELGOR_ATKS, true, true,
    { &ATK_Energy_Ball, }, species::CAT_NORMAL, 50, nullptr, },
  {  618, "Stunfisk", TYPE_GROUND, TYPE_ELECTRIC, 144, 171, 240, NULL, STUNFISK_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  618, "Galarian Stunfisk", TYPE_GROUND, TYPE_STEEL, 144, 171, 240, NULL, G_STUNFISK_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  619, "Mienfoo", TYPE_FIGHTING, TYPECOUNT, 160, 98, 128, NULL, MIENFOO_ATKS, true, false,
    { &ATK_High_Jump_Kick, }, species::CAT_NORMAL, 50, nullptr, },
  {  620, "Mienshao", TYPE_FIGHTING, TYPECOUNT, 258, 127, 163, "Mienfoo", MIENSHAO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  621, "Druddigon", TYPE_DRAGON, TYPECOUNT, 213, 170, 184, NULL, DRUDDIGON_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  622, "Golett", TYPE_GROUND, TYPE_GHOST, 127, 92, 153, NULL, GOLETT_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  623, "Golurk", TYPE_GROUND, TYPE_GHOST, 222, 154, 205, "Golett", GOLURK_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  624, "Pawniard", TYPE_DARK, TYPE_STEEL, 154, 114, 128, NULL, PAWNIARD_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  625, "Bisharp", TYPE_DARK, TYPE_STEEL, 232, 176, 163, "Pawniard", BISHARP_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  626, "Bouffalant", TYPE_NORMAL, TYPECOUNT, 195, 182, 216, NULL, BOUFFALANT_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  627, "Rufflet", TYPE_NORMAL, TYPE_FLYING, 150, 97, 172, NULL, RUFFLET_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  628, "Braviary", TYPE_NORMAL, TYPE_FLYING, 232, 152, 225, "Rufflet", BRAVIARY_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  628, "Hisuian Braviary", TYPE_PSYCHIC, TYPE_FLYING, 213, 137, 242, NULL, H_BRAVIARY_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  629, "Vullaby", TYPE_DARK, TYPE_FLYING, 105, 139, 172, NULL, VULLABY_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  630, "Mandibuzz", TYPE_DARK, TYPE_FLYING, 129, 205, 242, "Vullaby", MANDIBUZZ_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  631, "Heatmor", TYPE_FIRE, TYPECOUNT, 204, 129, 198, NULL, HEATMOR_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  632, "Durant", TYPE_BUG, TYPE_STEEL, 217, 188, 151, NULL, DURANT_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  633, "Deino", TYPE_DARK, TYPE_DRAGON, 116, 93, 141, NULL, DEINO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  634, "Zweilous", TYPE_DARK, TYPE_DRAGON, 159, 135, 176, "Deino", ZWEILOUS_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  635, "Hydreigon", TYPE_DARK, TYPE_DRAGON, 256, 188, 211, "Zweilous", HYDREIGON_ATKS, true, false,
    { &ATK_Brutal_Swing, }, species::CAT_NORMAL, 75, nullptr, },
  {  636, "Larvesta", TYPE_BUG, TYPE_FIRE, 156, 107, 146, NULL, LARVESTA_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  637, "Volcarona", TYPE_BUG, TYPE_FIRE, 264, 189, 198, "Larvesta", VOLCARONA_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  638, "Cobalion", TYPE_STEEL, TYPE_FIGHTING, 192, 229, 209, NULL, COBALION_ATKS, true, false,
    { &ATK_Sacred_Sword, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  639, "Terrakion", TYPE_ROCK, TYPE_FIGHTING, 260, 192, 209, NULL, TERRAKION_ATKS, true, true,
    { &ATK_Sacred_Sword, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  640, "Virizion", TYPE_GRASS, TYPE_FIGHTING, 192, 229, 209, NULL, VIRIZION_ATKS, true, false,
    { &ATK_Sacred_Sword, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  641, "Incarnate Forme Tornadus", TYPE_FLYING, TYPECOUNT, 266, 164, 188, NULL, I_TORNADUS_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  641, "Therian Forme Tornadus", TYPE_FLYING, TYPECOUNT, 238, 189, 188, NULL, T_TORNADUS_ATKS, true, false,
    { &ATK_Bleakwind_Storm, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  642, "Incarnate Forme Thundurus", TYPE_ELECTRIC, TYPE_FLYING, 266, 164, 188, NULL, I_THUNDURUS_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  642, "Therian Forme Thundurus", TYPE_ELECTRIC, TYPE_FLYING, 295, 161, 188, NULL, T_THUNDURUS_ATKS, true, false,
    { &ATK_Wildbolt_Storm, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  643, "Reshiram", TYPE_DRAGON, TYPE_FIRE, 275, 211, 205, NULL, RESHIRAM_ATKS, true, false,
    { &ATK_Fusion_Flare, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  644, "Zekrom", TYPE_DRAGON, TYPE_ELECTRIC, 275, 211, 205, NULL, ZEKROM_ATKS, true, false,
    { &ATK_Fusion_Bolt, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  645, "Incarnate Forme Landorus", TYPE_GROUND, TYPE_FLYING, 261, 182, 205, NULL, I_LANDORUS_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  645, "Therian Forme Landorus", TYPE_GROUND, TYPE_FLYING, 289, 179, 205, NULL, T_LANDORUS_ATKS, true, false,
    { &ATK_Sandsear_Storm, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  646, "Kyurem", TYPE_DRAGON, TYPE_ICE, 246, 170, 245, NULL, KYUREM_ATKS, true, false,
    { &ATK_Glaciate, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  646, "White Kyurem", TYPE_DRAGON, TYPE_ICE, 310, 183, 245, "Kyurem", W_KYUREM_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  646, "Black Kyurem", TYPE_DRAGON, TYPE_ICE, 310, 183, 245, "Kyurem", B_KYUREM_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  647, "Keldeo", TYPE_WATER, TYPE_FIGHTING, 260, 192, 209, NULL, KELDEO_ATKS, false, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  648, "Meloetta", TYPE_NORMAL, TYPE_PSYCHIC, 250, 225, 225, NULL, MELOETTA_ATKS, true, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  649, "Genesect", TYPE_BUG, TYPE_STEEL, 252, 199, 174, NULL, GENESECT_ATKS, true, false,
    { &ATK_Techno_Blast, }, species::CAT_MYTHICAL, 100, nullptr, },
  {  650, "Chespin", TYPE_GRASS, TYPECOUNT, 110, 106, 148, NULL, CHESPIN_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  651, "Quilladin", TYPE_GRASS, TYPECOUNT, 146, 156, 156, "Chespin", QUILLADIN_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  652, "Chesnaught", TYPE_GRASS, TYPE_FIGHTING, 201, 204, 204, "Quilladin", CHESNAUGHT_ATKS, true, false,
    { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, },
  {  653, "Fennekin", TYPE_FIRE, TYPECOUNT, 116, 102, 120, NULL, FENNEKIN_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  654, "Braixen", TYPE_FIRE, TYPECOUNT, 171, 130, 153, "Fennekin", BRAIXEN_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  655, "Delphox", TYPE_FIRE, TYPE_PSYCHIC, 230, 189, 181, "Braixen", DELPHOX_ATKS, true, false,
    { &ATK_Blast_Burn, }, species::CAT_NORMAL, 10, nullptr, },
  {  656, "Froakie", TYPE_WATER, TYPECOUNT, 122, 84, 121, NULL, FROAKIE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  657, "Frogadier", TYPE_WATER, TYPECOUNT, 168, 114, 144, "Froakie", FROGADIER_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  658, "Greninja", TYPE_WATER, TYPE_DARK, 223, 152, 176, "Frogadier", GRENINJA_ATKS, true, false,
    { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, },
  {  659, "Bunnelby", TYPE_NORMAL, TYPECOUNT, 68, 72, 116, NULL, BUNNELBY_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  660, "Diggersby", TYPE_NORMAL, TYPE_GROUND, 112, 155, 198, "Bunnelby", DIGGERSBY_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  661, "Fletchling", TYPE_NORMAL, TYPE_FLYING, 95, 80, 128, NULL, FLETCHLING_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  662, "Fletchinder", TYPE_FIRE, TYPE_FLYING, 145, 110, 158, "Fletchling", FLETCHINDER_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  663, "Talonflame", TYPE_FIRE, TYPE_FLYING, 176, 155, 186, "Fletchinder", TALONFLAME_ATKS, true, false,
    { &ATK_Incinerate, }, species::CAT_NORMAL, 10, nullptr, },
  {  664, "Scatterbug", TYPE_BUG, TYPECOUNT, 63, 63, 116, NULL, SCATTERBUG_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  665, "Spewpa", TYPE_BUG, TYPECOUNT, 48, 89, 128, "Scatterbug", SPEWPA_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  666, "Vivillon", TYPE_BUG, TYPE_FLYING, 176, 103, 190, "Spewpa", VIVILLON_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  667, "Litleo", TYPE_FIRE, TYPE_NORMAL, 139, 112, 158, NULL, LITLEO_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  668, "Pyroar", TYPE_FIRE, TYPE_NORMAL, 221, 149, 200, "Litleo", PYROAR_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  669, "Flabébé", TYPE_FAIRY, TYPECOUNT, 108, 120, 127, NULL, FLABEBE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  670, "Floette", TYPE_FAIRY, TYPECOUNT, 136, 151, 144, "Flabébé", FLOETTE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  671, "Florges", TYPE_FAIRY, TYPECOUNT, 212, 244, 186, "Floette", FLORGES_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  672, "Skiddo", TYPE_GRASS, TYPECOUNT, 123, 102, 165, NULL, SKIDDO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  673, "Gogoat", TYPE_GRASS, TYPECOUNT, 196, 146, 265, "Skiddo", GOGOAT_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  674, "Pancham", TYPE_FIGHTING, TYPECOUNT, 145, 107, 167, NULL, PANCHAM_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  675, "Pangoro", TYPE_FIGHTING, TYPE_DARK, 226, 146, 216, "Pancham", PANGORO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  676, "Furfrou", TYPE_NORMAL, TYPECOUNT, 164, 167, 181, NULL, FURFROU_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  677, "Espurr", TYPE_PSYCHIC, TYPECOUNT, 120, 114, 158, NULL, ESPURR_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  678, "Meowstic", TYPE_PSYCHIC, TYPECOUNT, 166, 167, 179, "Espurr", MEOWSTIC_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  679, "Honedge", TYPE_STEEL, TYPE_GHOST, 135, 139, 128, NULL, HONEDGE_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  680, "Doublade", TYPE_STEEL, TYPE_GHOST, 188, 206, 153, NULL, DOUBLADE_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  681, "Aegislash (Blade)", TYPE_STEEL, TYPE_GHOST, 291, 97, 155, NULL, AEGISLASH_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  681, "Aegislash (Shield)", TYPE_STEEL, TYPE_GHOST, 97, 291, 155, NULL, AEGISLASH_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  682, "Spritzee", TYPE_FAIRY, TYPECOUNT, 110, 113, 186, NULL, SPRITZEE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  683, "Aromatisse", TYPE_FAIRY, TYPECOUNT, 173, 150, 226, "Spritzee", AROMATISSE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  684, "Swirlix", TYPE_FAIRY, TYPECOUNT, 109, 119, 158, NULL, SWIRLIX_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  685, "Slurpuff", TYPE_FAIRY, TYPECOUNT, 168, 163, 193, "Swirlix", SLURPUFF_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  686, "Inkay", TYPE_DARK, TYPE_PSYCHIC, 98, 95, 142, NULL, INKAY_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  687, "Malamar", TYPE_DARK, TYPE_PSYCHIC, 177, 165, 200, "Inkay", MALAMAR_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  688, "Binacle", TYPE_ROCK, TYPE_WATER, 96, 120, 123, NULL, BINACLE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  689, "Barbaracle", TYPE_ROCK, TYPE_WATER, 194, 205, 176, "Binacle", BARBARACLE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  690, "Skrelp", TYPE_POISON, TYPE_WATER, 109, 109, 137, NULL, SKRELP_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  691, "Dragalge", TYPE_POISON, TYPE_DRAGON, 177, 207, 163, "Skrelp", DRAGALGE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  692, "Clauncher", TYPE_WATER, TYPECOUNT, 108, 117, 137, NULL, CLAUNCHER_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  693, "Clawitzer", TYPE_WATER, TYPECOUNT, 221, 171, 174, "Clauncher", CLAWITZER_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  694, "Helioptile", TYPE_ELECTRIC, TYPE_NORMAL, 115, 78, 127, NULL, HELIOPTILE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  695, "Heliolisk", TYPE_ELECTRIC, TYPE_NORMAL, 219, 168, 158, "Helioptile", HELIOLISK_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  696, "Tyrunt", TYPE_ROCK, TYPE_DRAGON, 158, 123, 151, NULL, TYRUNT_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  697, "Tyrantrum", TYPE_ROCK, TYPE_DRAGON, 227, 191, 193, "Tyrunt", TYRANTRUM_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  698, "Amaura", TYPE_ROCK, TYPE_ICE, 124, 109, 184, NULL, AMAURA_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  699, "Aurorus", TYPE_ROCK, TYPE_ICE, 186, 163, 265, "Amaura", AURORUS_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  700, "Sylveon", TYPE_FAIRY, TYPECOUNT, 203, 205, 216, "Eevee", SYLVEON_ATKS, true, false,
    { &ATK_Last_Resort, &ATK_Psyshock, }, species::CAT_NORMAL, 75, nullptr, },
  {  701, "Hawlucha", TYPE_FIGHTING, TYPE_FLYING, 195, 153, 186, NULL, HAWLUCHA_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  702, "Dedenne", TYPE_ELECTRIC, TYPE_FAIRY, 164, 134, 167, NULL, DEDENNE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  703, "Carbink", TYPE_ROCK, TYPE_FAIRY, 95, 285, 137, NULL, CARBINK_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  704, "Goomy", TYPE_DRAGON, TYPECOUNT, 101, 112, 128, NULL, GOOMY_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  705, "Sliggoo", TYPE_DRAGON, TYPECOUNT, 159, 176, 169, "Goomy", SLIGGOO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  706, "Goodra", TYPE_DRAGON, TYPECOUNT, 220, 242, 207, "Sliggoo", GOODRA_ATKS, true, false,
    { &ATK_Thunder_Punch, }, species::CAT_NORMAL, 75, nullptr, },
  {  707, "Klefki", TYPE_STEEL, TYPE_FAIRY, 160, 179, 149, NULL, KLEFKI_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  708, "Phantump", TYPE_GHOST, TYPE_GRASS, 125, 103, 125, NULL, PHANTUMP_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  709, "Trevenant", TYPE_GHOST, TYPE_GRASS, 201, 154, 198, "Phantump", TREVENANT_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  710, "Pumpkaboo", TYPE_GHOST, TYPE_GRASS, 121, 123, 135, NULL, PUMPKABOO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  710, "Pumpkaboo Small", TYPE_GHOST, TYPE_GRASS, 122, 124, 127, NULL, PUMPKABOO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  710, "Pumpkaboo Large", TYPE_GHOST, TYPE_GRASS, 120, 122, 144, NULL, PUMPKABOO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  710, "Pumpkaboo Super", TYPE_GHOST, TYPE_GRASS, 118, 120, 153, NULL, PUMPKABOO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  711, "Gourgeist", TYPE_GHOST, TYPE_GRASS, 175, 213, 163, "Pumpkaboo", GOURGEIST_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  711, "Gourgeist Small", TYPE_GHOST, TYPE_GRASS, 171, 219, 146, "Pumpkaboo Small", GOURGEIST_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  711, "Gourgeist Large", TYPE_GHOST, TYPE_GRASS, 179, 206, 181, "Pumpkaboo Large", GOURGEIST_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  711, "Gourgeist Super", TYPE_GHOST, TYPE_GRASS, 182, 200, 198, "Pumpkaboo Super", GOURGEIST_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  712, "Bergmite", TYPE_ICE, TYPECOUNT, 117, 120, 146, NULL, BERGMITE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  713, "Avalugg", TYPE_ICE, TYPECOUNT, 196, 240, 216, "Bergmite", AVALUGG_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  713, "Hisuian Avalugg", TYPE_ICE, TYPE_ROCK, 214, 238, 216, NULL, H_AVALUGG_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  714, "Noibat", TYPE_FLYING, TYPE_DRAGON, 83, 73, 120, NULL, NOIBAT_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  715, "Noivern", TYPE_FLYING, TYPE_DRAGON, 205, 175, 198, "Noibat", NOIVERN_ATKS, true, false,
    { &ATK_Boomburst, }, species::CAT_NORMAL, 75, nullptr, },
  {  716, "Xerneas", TYPE_FAIRY, TYPECOUNT, 250, 185, 246, NULL, XERNEAS_ATKS, true, false,
    { &ATK_Geomancy, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  717, "Yveltal", TYPE_DARK, TYPE_FLYING, 250, 185, 246, NULL, YVELTAL_ATKS, true, false,
    { &ATK_Oblivion_Wing, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  718, "Zygarde 10%", TYPE_DRAGON, TYPE_GROUND, 205, 173, 144, NULL, ZYGARDE10_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  718, "Zygarde 50%", TYPE_DRAGON, TYPE_GROUND, 203, 232, 239, "Zygarde 10%", ZYGARDE50_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  718, "Zygarde Complete", TYPE_DRAGON, TYPE_GROUND, 184, 207, 389, "Zygarde 50%", ZYGARDE_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  719, "Diancie", TYPE_ROCK, TYPE_FAIRY, 190, 285, 137, NULL, DIANCIE_ATKS, true, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  720, "Hoopa Confined", TYPE_PSYCHIC, TYPE_GHOST, 261, 187, 173, NULL, C_HOOPA_ATKS, false, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  720, "Hoopa Unbound", TYPE_PSYCHIC, TYPE_DARK, 311, 191, 173, NULL, U_HOOPA_ATKS, false, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  721, "Volcanion", TYPE_FIRE, TYPE_WATER, 252, 216, 190, NULL, VOLCANION_ATKS, false, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  722, "Rowlet", TYPE_GRASS, TYPE_FLYING, 102, 99, 169, NULL, ROWLET_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  723, "Dartrix", TYPE_GRASS, TYPE_FLYING, 142, 139, 186, "Rowlet", DARTRIX_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  724, "Decidueye", TYPE_GRASS, TYPE_GHOST, 210, 179, 186, "Dartrix", DECIDUEYE_ATKS, true, false,
    { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, },
  {  724, "Hisuian Decidueye", TYPE_GRASS, TYPE_FIGHTING, 213, 174, 204, NULL, H_DECIDUEYE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  725, "Litten", TYPE_FIRE, TYPECOUNT, 128, 79, 128, NULL, LITTEN_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  726, "Torracat", TYPE_FIRE, TYPECOUNT, 174, 103, 163, NULL, TORRACAT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  727, "Incineroar", TYPE_FIRE, TYPE_DARK, 214, 175, 216, NULL, INCINEROAR_ATKS, true, false,
    { &ATK_Blast_Burn, }, species::CAT_NORMAL, 10, nullptr, },
  {  728, "Popplio", TYPE_WATER, TYPECOUNT, 120, 103, 137, NULL, POPPLIO_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  729, "Brionne", TYPE_WATER, TYPECOUNT, 168, 145, 155, "Popplio", BRIONNE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  730, "Primarina", TYPE_WATER, TYPE_FAIRY, 232, 195, 190, "Brionne", PRIMARINA_ATKS, true, false,
    { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, },
  {  731, "Pikipek", TYPE_NORMAL, TYPE_FLYING, 136, 59, 111, NULL, PIKPEK_ATKS, false, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  732, "Trumbeak", TYPE_NORMAL, TYPE_FLYING, 159, 100, 146, "Pikipek", TRUMBEAK_ATKS, false, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  733, "Toucannon", TYPE_NORMAL, TYPE_FLYING, 222, 146, 190, "Trumbeak", TOUCANNON_ATKS, false, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  734, "Yungoos", TYPE_NORMAL, TYPECOUNT, 122, 56, 134, NULL, YUNGOOS_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  735, "Gumshoos", TYPE_NORMAL, TYPECOUNT, 194, 113, 204, "Yungoos", GUMSHOOS_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  736, "Grubbin", TYPE_BUG, TYPECOUNT, 115, 85, 132, NULL, GRUBBIN_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  737, "Charjabug", TYPE_BUG, TYPE_ELECTRIC, 145, 161, 149, "Grubbin", CHARJABUG_ATKS, true, false,
    { &ATK_Volt_Switch, }, species::CAT_NORMAL, 10, nullptr, },
  {  738, "Vikavolt", TYPE_BUG, TYPE_ELECTRIC, 254, 158, 184, "Charjabug", VIKAVOLT_ATKS, true, false,
    { &ATK_Volt_Switch, }, species::CAT_NORMAL, 10, nullptr, },
  {  739, "Crabrawler", TYPE_FIGHTING, TYPECOUNT, 150, 104, 132, NULL, CRABRAWLER_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  740, "Crabominable", TYPE_FIGHTING, TYPE_ICE, 231, 138, 219, "Crabrawler", CRABOMINABLE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  741, "Oricorio (Baile)", TYPE_FIRE, TYPE_FLYING, 196, 145, 181, NULL, B_ORICORIO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  741, "Oricorio (Pom-Pom)", TYPE_ELECTRIC, TYPE_FLYING, 196, 145, 181, NULL, P_ORICORIO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  741, "Oricorio (Pa'u)", TYPE_PSYCHIC, TYPE_FLYING, 196, 145, 181, NULL, PS_ORICORIO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  741, "Oricorio (Sensu)", TYPE_GHOST, TYPE_FLYING, 196, 145, 181, NULL, S_ORICORIO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  742, "Cutiefly", TYPE_BUG, TYPE_FAIRY, 110, 81, 120, NULL, CUTIEFLY_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  743, "Ribombee", TYPE_BUG, TYPE_FAIRY, 198, 146, 155, "Cutiefly", RIBOMBEE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  744, "Rockruff", TYPE_ROCK, TYPECOUNT, 117, 78, 128, NULL, ROCKRUFF_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  745, "Lycanroc Midday", TYPE_ROCK, TYPECOUNT, 231, 140, 181, "Rockruff", LYCANROC_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  745, "Lycanroc Midnight", TYPE_ROCK, TYPECOUNT, 218, 152, 198, "Rockruff", N_LYCANROC_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  745, "Lycanroc Dusk", TYPE_ROCK, TYPECOUNT, 234, 139, 181, "Rockruff", D_LYCANROC_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  // not released as of 2025-06-25
  {  746, "Wishiwashi", TYPE_WATER, TYPECOUNT, 46, 43, 128, NULL, WISHIWASHI_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  747, "Mareanie", TYPE_POISON, TYPE_WATER, 98, 110, 137, NULL, MAREANIE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  748, "Toxapex", TYPE_POISON, TYPE_WATER, 114, 273, 137, "Mareanie", TOXAPEX_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  749, "Mudbray", TYPE_GROUND, TYPECOUNT, 175, 121, 172, NULL, MUDBRAY_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  750, "Mudsdale", TYPE_GROUND, TYPECOUNT, 214, 174, 225, "Mudbray", MUDSDALE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  751, "Dewpider", TYPE_WATER, TYPE_BUG, 72, 117, 116, NULL, DEWPIDER_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  752, "Araquanid", TYPE_WATER, TYPE_BUG, 126, 219, 169, "Dewpider", ARAQUANID_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  753, "Fomantis", TYPE_GRASS, TYPECOUNT, 100, 64, 120, NULL, FOMANTIS_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  754, "Lurantis", TYPE_GRASS, TYPECOUNT, 192, 169, 172, "Fomantis", LURANTIS_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  755, "Morelull", TYPE_GRASS, TYPE_FAIRY, 108, 119, 120, NULL, MORELULL_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  756, "Shiinotic", TYPE_GRASS, TYPE_FAIRY, 154, 168, 155, "Morelull", SHIINOTIC_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  757, "Salandit", TYPE_POISON, TYPE_FIRE, 136, 80, 134, NULL, SALANDIT_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  758, "Salazzle", TYPE_POISON, TYPE_FIRE, 228, 130, 169, "Salandit", SALAZZLE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  759, "Stufful", TYPE_NORMAL, TYPE_FIGHTING, 136, 95, 172, NULL, STUFFUL_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  760, "Bewear", TYPE_NORMAL, TYPE_FIGHTING, 226, 141, 260, "Stufful", BEWEAR_ATKS, true, false,
    { &ATK_Drain_Punch, }, species::CAT_NORMAL, 10, nullptr, },
  {  761, "Bounsweet", TYPE_GRASS, TYPECOUNT, 55, 69, 123, NULL, BOUNSWEET_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  762, "Steenee", TYPE_GRASS, TYPECOUNT, 78, 94, 141, "Bounsweet", STEENEE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  763, "Tsareena", TYPE_GRASS, TYPECOUNT, 222, 195, 176, "Steenee", TSAREENA_ATKS, true, false,
    { &ATK_High_Jump_Kick, }, species::CAT_NORMAL, 10, nullptr, },
  {  764, "Comfey", TYPE_FAIRY, TYPECOUNT, 165, 215, 139, NULL, COMFEY_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  765, "Oranguru", TYPE_NORMAL, TYPE_PSYCHIC, 168, 192, 207, NULL, ORANGURU_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  766, "Passimian", TYPE_FIGHTING, TYPECOUNT, 222, 160, 225, NULL, PASSIMIAN_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  767, "Wimpod", TYPE_BUG, TYPE_WATER, 67, 74, 93, NULL, WIMPOD_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  768, "Golisopod", TYPE_BUG, TYPE_WATER, 218, 226, 181, "Wimpod", GOLISOPOD_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  769, "Sandygast", TYPE_GHOST, TYPE_GROUND, 120, 118, 146, NULL, SANDYGAST_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  770, "Palossand", TYPE_GHOST, TYPE_GROUND, 178, 178, 198, "Sandygast", PALOSSAND_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  775, "Komala", TYPE_NORMAL, TYPECOUNT, 216, 165, 163, NULL, KOMALA_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  776, "Turtonator", TYPE_FIRE, TYPE_DRAGON, 165, 215, 155, NULL, TURTONATOR_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  777, "Togedemaru", TYPE_ELECTRIC, TYPE_STEEL, 190, 145, 163, NULL, TOGEDEMARU_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  // not released as of 2025-06-25
  {  778, "Mimikyu", TYPE_GHOST, TYPE_FAIRY, 177, 199, 146, NULL, MIMIKYU_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  779, "Bruxish", TYPE_WATER, TYPE_PSYCHIC, 208, 145, 169, NULL, BRUXISH_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  780, "Drampa", TYPE_NORMAL, TYPE_DRAGON, 231, 164, 186, NULL, DRAMPA_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  781, "Dhelmise", TYPE_GHOST, TYPE_GRASS, 233, 179, 172, NULL, DHELMISE_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  782, "Jangmo-o", TYPE_DRAGON, TYPECOUNT, 102, 108, 128, NULL, JANGMO_O_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  783, "Hakamo-o", TYPE_DRAGON, TYPE_FIGHTING, 145, 162, 146, "Jangmo-o", HAKAMO_O_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  784, "Kommo-o", TYPE_DRAGON, TYPE_FIGHTING, 222, 240, 181, "Hakamo-o", KOMMO_O_ATKS, true, false,
    { &ATK_Clanging_Scales, }, species::CAT_NORMAL, 75, nullptr, },
  {  785, "Tapu Koko", TYPE_ELECTRIC, TYPE_FAIRY, 250, 181, 172, NULL, TAPUKOKO_ATKS, true, false,
    { &ATK_Natures_Madness, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  786, "Tapu Lele", TYPE_PSYCHIC, TYPE_FAIRY, 259, 208, 172, NULL, TAPULELE_ATKS, true, false,
    { &ATK_Natures_Madness, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  787, "Tapu Bulu", TYPE_GRASS, TYPE_FAIRY, 249, 215, 172, NULL, TAPUBULU_ATKS, true, false,
    { &ATK_Natures_Madness, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  788, "Tapu Fini", TYPE_WATER, TYPE_FAIRY, 189, 254, 172, NULL, TAPUFINI_ATKS, true, false,
    { &ATK_Natures_Madness, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  789, "Cosmog", TYPE_PSYCHIC, TYPECOUNT, 54, 57, 125, NULL, COSMOG_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  790, "Cosmoem", TYPE_PSYCHIC, TYPECOUNT, 54, 242, 125, "Cosmog", COSMOEM_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  791, "Solgaleo", TYPE_PSYCHIC, TYPE_STEEL, 255, 191, 264, "Cosmoem", SOLGALEO_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  792, "Lunala", TYPE_PSYCHIC, TYPE_GHOST, 255, 191, 264, "Cosmoem", LUNALA_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  793, "Nihilego", TYPE_ROCK, TYPE_POISON, 249, 210, 240, NULL, NIHILEGO_ATKS, true, false, {}, species::CAT_ULTRABEAST, 100, nullptr, },
  {  794, "Buzzwole", TYPE_BUG, TYPE_FIGHTING, 236, 196, 216, NULL, BUZZWOLE_ATKS, true, false, {}, species::CAT_ULTRABEAST, 100, nullptr, },
  {  795, "Pheromosa", TYPE_BUG, TYPE_FIGHTING, 316, 85, 174, NULL, PHEROMOSA_ATKS, true, false, {}, species::CAT_ULTRABEAST, 100, nullptr, },
  {  796, "Xurkitree", TYPE_ELECTRIC, TYPECOUNT, 330, 144, 195, NULL, XURKITREE_ATKS, true, false, {}, species::CAT_ULTRABEAST, 100, nullptr, },
  {  797, "Celesteela", TYPE_STEEL, TYPE_FLYING, 207, 199, 219, NULL, CELESTEELA_ATKS, true, false, {}, species::CAT_ULTRABEAST, 100, nullptr, },
  {  798, "Kartana", TYPE_GRASS, TYPE_STEEL, 323, 182, 139, NULL, KARTANA_ATKS, true, false, {}, species::CAT_ULTRABEAST, 100, nullptr, },
  {  799, "Guzzlord", TYPE_DARK, TYPE_DRAGON, 188, 99, 440, NULL, GUZZLORD_ATKS, true, false, {}, species::CAT_ULTRABEAST, 100, nullptr, },
  {  800, "Necrozma", TYPE_PSYCHIC, TYPECOUNT, 251, 195, 219, NULL, NECROZMA_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  800, "Necrozma Dusk Mane", TYPE_PSYCHIC, TYPE_STEEL, 277, 220, 200, "Necrozma", DUSK_NECROZMA_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  800, "Necrozma Dawn Wings", TYPE_PSYCHIC, TYPE_GHOST, 277, 220, 200, "Necrozma", DAWN_NECROZMA_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  802, "Marshadow", TYPE_FIGHTING, TYPE_GHOST, 265, 190, 207, NULL, MARSHADOW_ATKS, true, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  803, "Poipole", TYPE_POISON, TYPECOUNT, 145, 133, 167, NULL, POIPOLE_ATKS, true, false, {}, species::CAT_ULTRABEAST, 100, nullptr, },
  {  804, "Naganadel", TYPE_POISON, TYPE_DRAGON, 263, 159, 177, "Poipole", NAGANADEL_ATKS, true, false, {}, species::CAT_ULTRABEAST, 100, nullptr, },
  {  805, "Stakataka", TYPE_ROCK, TYPE_STEEL, 213, 298, 156, NULL, STAKATAKA_ATKS, true, false, {}, species::CAT_ULTRABEAST, 100, nullptr, },
  {  806, "Blacephalon", TYPE_FIRE, TYPE_GHOST, 315, 148, 142, NULL, BLACEPHALON_ATKS, true, false, {}, species::CAT_ULTRABEAST, 100, nullptr, },
  {  808, "Meltan", TYPE_STEEL, TYPECOUNT, 118, 99, 130, NULL, MELTAN_ATKS, true, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  809, "Melmetal", TYPE_STEEL, TYPECOUNT, 226, 190, 264, "Meltan", MELMETAL_ATKS, true, false,
    { &ATK_Double_Iron_Bash, }, species::CAT_MYTHICAL, 100, nullptr, },
  {  810, "Grookey", TYPE_GRASS, TYPECOUNT, 122, 91, 137, NULL, GROOKEY_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  811, "Thwackey", TYPE_GRASS, TYPECOUNT, 165, 134, 172, "Grookey", THWACKEY_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  812, "Rillaboom", TYPE_GRASS, TYPECOUNT, 239, 168, 225, "Thwackey", RILLABOOM_ATKS, true, true, {}, species::CAT_NORMAL, 10, nullptr, },
  {  813, "Scorbunny", TYPE_FIRE, TYPECOUNT, 132, 79, 137, NULL, SCORBUNNY_ATKS, false, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  814, "Raboot", TYPE_FIRE, TYPECOUNT, 170, 125, 163, "Scorbunny", RABOOT_ATKS, false, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  815, "Cinderace", TYPE_FIRE, TYPECOUNT, 238, 163, 190, "Raboot", CINDERACE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  816, "Sobble", TYPE_WATER, TYPECOUNT, 132, 79, 137, NULL, SOBBLE_ATKS, false, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  817, "Drizzile", TYPE_WATER, TYPECOUNT, 186, 113, 163, "Sobble", DRIZZILE_ATKS, false, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  818, "Inteleon", TYPE_WATER, TYPECOUNT, 262, 142, 172, "Drizzile", INTELEON_ATKS, false, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  819, "Skwovet", TYPE_NORMAL, TYPECOUNT, 95, 86, 172, NULL, SKWOVET_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  820, "Greedent", TYPE_NORMAL, TYPECOUNT, 160, 156, 260, "Skwovet", GREEDENT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  821, "Rookidee", TYPE_FLYING, TYPECOUNT, 88, 67, 116, NULL, ROOKIDEE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  822, "Corvisquire", TYPE_FLYING, TYPECOUNT, 129, 110, 169, "Rookidee", CORVISQUIRE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  823, "Corviknight", TYPE_FLYING, TYPE_STEEL, 163, 192, 221, "Corvisquire", CORVIKNIGHT_ATKS, true, false,
    { &ATK_Iron_Head, }, species::CAT_NORMAL, 10, nullptr, },
  {  827, "Nickit", TYPE_DARK, TYPECOUNT, 85, 82, 120, NULL, NICKIT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  828, "Thievul", TYPE_DARK, TYPECOUNT, 172, 164, 172, "Nickit", THIEVUL_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  829, "Gossifleur", TYPE_GRASS, TYPECOUNT, 70, 104, 120, NULL, GOSSIFLEUR_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  830, "Eldegoss", TYPE_GRASS, TYPECOUNT, 148, 211, 155, "Gossifleur", ELDEGOSS_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  831, "Wooloo", TYPE_NORMAL, TYPECOUNT, 76, 97, 123, NULL, WOOLOO_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  832, "Dubwool", TYPE_NORMAL, TYPECOUNT, 159, 198, 176, "Wooloo", DUBWOOL_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  835, "Yamper", TYPE_ELECTRIC, TYPECOUNT, 80, 90, 153, NULL, YAMPER_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  836, "Boltund",TYPE_ELECTRIC, TYPECOUNT, 197, 131, 170, NULL, BOLTUND_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  840, "Applin", TYPE_GRASS, TYPE_DRAGON, 71, 116, 120, NULL, APPLIN_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  841, "Flapple", TYPE_GRASS, TYPE_DRAGON, 214, 144, 172, "Applin", FLAPPLE_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  842, "Appletun", TYPE_GRASS, TYPE_DRAGON, 178, 146, 242, "Applin", APPLETUN_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  848, "Toxel", TYPE_ELECTRIC, TYPE_POISON, 97, 65, 120, NULL, TOXEL_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  849, "Toxtricity", TYPE_ELECTRIC, TYPE_POISON, 224, 140, 181, "Toxel", TOXTRICITY_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  850, "Sizzlipede", TYPE_FIRE, TYPE_BUG, 118, 90, 137, NULL, SIZZLIPEDE_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  851, "Centiskorch", TYPE_FIRE, TYPE_BUG, 220, 158, 225, "Sizzlipede", CENTISKORCH_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  854, "Sinistea", TYPE_GHOST, TYPECOUNT, 134, 96, 120, NULL, SINISTEA_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  855, "Polteageist", TYPE_GHOST, TYPECOUNT, 248, 189, 155, "Sinistea", POLTEAGEIST_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  856, "Hatenna", TYPE_PSYCHIC, TYPECOUNT, 98, 93, 123, NULL, HATENNA_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  857, "Hattrem", TYPE_PSYCHIC, TYPECOUNT, 153, 133, 149, "Hatenna", HATTREM_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  858, "Hatterene", TYPE_PSYCHIC, TYPE_FAIRY, 237, 182, 149, "Hattrem", HATTERENE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  862, "Obstagoon", TYPE_DARK, TYPE_NORMAL, 180, 194, 212, "Galarian Linoone", OBSTAGOON_ATKS, true, true,
    { &ATK_Obstruct, }, species::CAT_NORMAL, 10, nullptr, },
  {  863, "Perrserker", TYPE_STEEL, TYPECOUNT, 195, 162, 172, "Galarian Meowth", PERRSERKER_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  864, "Cursola", TYPE_GHOST, TYPECOUNT, 253, 182, 155, "Galarian Corsola", CURSOLA_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  865, "Sirfetch'd", TYPE_FIGHTING, TYPECOUNT, 248, 176, 158, "Galarian Farfetch'd", SIRFETCHD_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  866, "Mr. Rime", TYPE_ICE, TYPE_PSYCHIC, 212, 179, 190, "Galarian Mr. Mime", MRRIME_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  867, "Runerigus", TYPE_GROUND, TYPE_GHOST, 163, 237, 151, "Galarian Yamask", RUNERIGUS_ATKS, true, true, {}, species::CAT_NORMAL, 50, nullptr, },
  {  870, "Falinks", TYPE_FIGHTING, TYPECOUNT, 193, 170, 163, NULL, FALINKS_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  874, "Stonjourner", TYPE_ROCK, TYPECOUNT, 222, 182, 225, NULL, STONJOURNER_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  877, "Morpeko", TYPE_ELECTRIC, TYPE_DARK, 192, 121, 151, NULL, MORPEKO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  885, "Dreepy", TYPE_DRAGON, TYPE_GHOST, 117, 61, 99, NULL, DREEPY_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  886, "Drakloak", TYPE_DRAGON, TYPE_GHOST, 163, 105, 169, "Dreepy", DRAKLOAK_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  887, "Dragapult", TYPE_DRAGON, TYPE_GHOST, 266, 170, 204, "Drakloak", DRAGAPULT_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  888, "Zacian", TYPE_FAIRY, TYPECOUNT, 254, 236, 192, NULL, ZACIAN_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  888, "Crowned Sword Zacian", TYPE_FAIRY, TYPE_STEEL, 332, 240, 192, "Zacian", C_ZACIAN_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  889, "Zamazenta", TYPE_FIGHTING, TYPECOUNT, 254, 236, 192, NULL, ZAMAZENTA_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  889, "Crowned Shield Zamazenta", TYPE_FIGHTING, TYPE_STEEL, 250, 292, 192, "Zamazenta", C_ZAMAZENTA_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  890, "Eternatus", TYPE_DRAGON, TYPE_POISON, 278, 192, 268, nullptr, ETERNATUS_ATKS, false, false,
    { &ATK_Dynamax_Cannon, }, species::CAT_LEGENDARY, 100, nullptr, },
  {  891, "Kubfu", TYPE_FIGHTING, TYPECOUNT, 170, 112, 155, NULL, KUBFU_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  892, "Single Strike Style Urshifu", TYPE_FIGHTING, TYPE_DARK, 254, 177, 225, "Kubfu", S_URSHIFU_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  892, "Rapid Strike Style Urshifu", TYPE_FIGHTING, TYPE_WATER, 254, 177, 225, "Kubfu", R_URSHIFU_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  893, "Zarude", TYPE_DARK, TYPE_GRASS, 242, 215, 233, NULL, ZARUDE_ATKS, true, false, {}, species::CAT_MYTHICAL, 100, nullptr, },
  {  894, "Regieleki", TYPE_ELECTRIC, TYPECOUNT, 250, 125, 190, NULL, REGIELEKI_ATKS, true, true, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  895, "Regidrago", TYPE_DRAGON, TYPECOUNT, 202, 101, 400, NULL, REGIDRAGO_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  899, "Wyrdeer", TYPE_NORMAL, TYPE_PSYCHIC, 206, 145, 230, NULL, WYRDEER_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  900, "Kleavor", TYPE_BUG, TYPE_ROCK, 253, 174, 172, NULL, KLEAVOR_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  901, "Ursaluna", TYPE_GROUND, TYPE_NORMAL, 243, 181, 277, "Ursaring", URSALUNA_ATKS, true, true,
    { &ATK_High_Horsepower, }, species::CAT_NORMAL, 50, nullptr, },
  //{  902, "Basculegion", TYPE_WATER, TYPE_GHOST, nullptr, },
  {  903, "Sneasler", TYPE_FIGHTING, TYPE_POISON, 259, 158, 190, "Hisuian Sneasel", SNEASLER_ATKS, true, true, {}, species::CAT_NORMAL, 75, nullptr, },
  {  904, "Overqwil", TYPE_DARK, TYPE_POISON, 222, 171, 198, "Hisuian Qwilfish", OVERQWIL_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  905, "Incarnate Forme Enamorus", TYPE_FAIRY, TYPE_FLYING, 281, 162, 179, NULL, I_ENAMORUS_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  906, "Sprigatito", TYPE_GRASS, TYPECOUNT, 116, 99, 120, NULL, SPRIGATITO_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  907, "Floragato", TYPE_GRASS, TYPECOUNT, 157, 128, 156, "Sprigatito", FLORAGATO_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  908, "Meowscarada", TYPE_GRASS, TYPE_DARK, 233, 153, 183, "Floragato", MEOWSCARADA_ATKS, true, false,
    { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, },
  {  909, "Fuecoco", TYPE_FIRE, TYPECOUNT, 112, 96, 167, NULL, FUECOCO_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  910, "Crocalor", TYPE_FIRE, TYPECOUNT, 162, 134, 191, "Fuecoco", CROCALOR_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  911, "Skeledirge", TYPE_FIRE, TYPE_GHOST, 207, 178, 232, "Crocalor", SKELEDIRGE_ATKS, true, false,
    { &ATK_Blast_Burn, }, species::CAT_NORMAL, 10, nullptr, },
  {  912, "Quaxly", TYPE_WATER, TYPECOUNT, 120, 86, 146, NULL, QUAXLY_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  913, "Quaxwell", TYPE_WATER, TYPECOUNT, 162, 123, 172, "Quaxly", QUAXWELL_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  914, "Quaquaval", TYPE_WATER, TYPE_FIGHTING, 236, 159, 198, "Quaxwell", QUAQUAVAL_ATKS, true, false,
    { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, },
  {  915, "Lechonk", TYPE_NORMAL, TYPECOUNT, 81, 79, 144, NULL, LECHONK_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  916, "Male Oinkologne", TYPE_NORMAL, TYPECOUNT, 186, 153, 242, "Lechonk", M_OINKOLOGNE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  916, "Female Oinkologne", TYPE_NORMAL, TYPECOUNT, 169, 162, 251, "Lechonk", F_OINKOLOGNE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  // not released as of 2025-06-25!
  {  917, "Tarountula", TYPE_BUG, TYPECOUNT, 70, 77, 111, NULL, TAROUNTULA_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  918, "Spidops", TYPE_BUG, TYPECOUNT, 139, 166, 155, "Tarountula", SPIDOPS_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  919, "Nymble", TYPE_BUG, TYPECOUNT, 81, 65, 107, NULL, NYMBLE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  920, "Lokix", TYPE_BUG, TYPE_DARK, 199, 144, 174, "Nymble", LOKIX_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  921, "Pawmi", TYPE_ELECTRIC, TYPECOUNT, 95, 45, 128, NULL, PAWMI_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  922, "Pawmo", TYPE_ELECTRIC, TYPE_FIGHTING, 147, 82, 155, "Pawmi", PAWMO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  923, "Pawmot", TYPE_ELECTRIC, TYPE_FIGHTING, 232, 141, 172, "Pawmo", PAWMOT_ATKS, true, false,
    { &ATK_Brick_Break, }, species::CAT_NORMAL, 75, nullptr, },
  {  924, "Tandemaus", TYPE_NORMAL, TYPECOUNT, 98, 90, 137, NULL, TANDEMAUS_ATKS, false, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  925, "Maushold", TYPE_NORMAL, TYPECOUNT, 159, 157, 179, "Tandemaus", MAUSHOLD_ATKS, false, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  926, "Fidough", TYPE_FAIRY, TYPECOUNT, 102, 126, 114, NULL, FIDOUGH_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  927, "Dachsbun", TYPE_FAIRY, TYPECOUNT, 159, 212, 149, "Fidough", DACHSBUN_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  928, "Smoliv", TYPE_GRASS, TYPE_NORMAL, 100, 89, 121, NULL, SMOLIV_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  929, "Dolliv", TYPE_GRASS, TYPE_NORMAL, 137, 131, 141, "Smoliv", DOLLIV_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  930, "Arboliva", TYPE_GRASS, TYPE_NORMAL, 219, 189, 186, "Dolliv", ARBOLIVA_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  935, "Charcadet", TYPE_FIRE, TYPECOUNT, 92, 74, 120, NULL, CHARCADET_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  936, "Armarouge", TYPE_FIRE, TYPE_PSYCHIC, 234, 185, 198, "Charcadet", ARMAROUGE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  937, "Ceruledge", TYPE_FIRE, TYPE_GHOST, 239, 189, 181, "Charcadet", CERULEDGE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  938, "Tadbulb", TYPE_ELECTRIC, TYPECOUNT, 104, 73, 156, NULL, TADBULB_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  939, "Bellibolt", TYPE_ELECTRIC, TYPECOUNT, 184, 165, 240, "Tadbulb", BELLIBOLT_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  944, "Shroodle", TYPE_POISON, TYPE_NORMAL, 124, 70, 120, NULL, SHROODLE_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  945, "Grafaiai", TYPE_POISON, TYPE_NORMAL, 199, 149, 160, "Shroodle", GRAFAIAI_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  // released 2025-07-29
  {  948, "Toedscool", TYPE_GRASS, TYPE_GROUND, 97, 149, 120, nullptr, TOEDSCOOL_ATKS, false, false, {}, species::CAT_NORMAL, 50,  nullptr, },
  {  949, "Toedscruel", TYPE_GRASS, TYPE_GROUND, 166, 209, 190, nullptr, TOEDSCRUEL_ATKS, false, false, {}, species::CAT_NORMAL, 50,  nullptr, },
  {  957, "Tinkatink", TYPE_FAIRY, TYPE_STEEL, 85, 110, 137, NULL, TINKATINK_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  958, "Tinkatuff", TYPE_FAIRY, TYPE_STEEL, 109, 145, 163, "Tinkatink", TINKATUFF_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  959, "Tinkaton", TYPE_FAIRY, TYPE_STEEL, 155, 196, 198, "Tinkatuff", TINKATON_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  960, "Wiglett", TYPE_WATER, TYPECOUNT, 109, 52, 67, NULL, WIGLETT_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  961, "Wugtrio", TYPE_WATER, TYPECOUNT, 205, 136, 111, "Wiglett", WUGTRIO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  962, "Bombirdier", TYPE_FLYING, TYPE_DARK, 198, 172, 172, NULL, BOMBIRDIER_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  965, "Varoom", TYPE_STEEL, TYPE_POISON, 123, 107, 128, NULL, VAROOM_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  966, "Revavroom", TYPE_STEEL, TYPE_POISON, 229, 168, 190, "Varoom", REVAVROOM_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  971, "Greavard", TYPE_GHOST, TYPECOUNT, 105, 106, 137, NULL, GREAVARD_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  972, "Houndstone", TYPE_GHOST, TYPECOUNT, 186, 195, 176, "Greavard", HOUNDSTONE_ATKS, false, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  974, "Cetoddle", TYPE_ICE, TYPECOUNT, 119, 80, 239, NULL, CETODDLE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  975, "Cetitan", TYPE_ICE, TYPECOUNT, 208, 123, 347, "Cetoddle", CETITAN_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  // released 2025-07-03
  {  978, "Tatsugiri", TYPE_DRAGON, TYPE_WATER, 226, 166, 169, NULL, TATSUGIRI_ATKS, false, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  979, "Annihilape", TYPE_FIGHTING, TYPE_GHOST, 220, 178, 242, "Primeape", ANNIHILAPE_ATKS, true, true,
    { &ATK_Rage_Fist, }, species::CAT_NORMAL, 50, nullptr, },
  {  980, "Clodsire", TYPE_POISON, TYPE_GROUND, 127, 151, 277, "Paldean Wooper", CLODSIRE_ATKS, true, false,
    { &ATK_Megahorn, }, species::CAT_NORMAL, 50, nullptr, },
  {  983, "Kingambit", TYPE_DARK, TYPE_STEEL, 238, 203, 225, "Bisharp", KINGAMBIT_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  996, "Frigibax", TYPE_DRAGON, TYPE_ICE, 134, 86, 163, NULL, FRIGIBAX_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  997, "Arctibax", TYPE_DRAGON, TYPE_ICE, 173, 128, 207, "Frigibax", ARCTIBAX_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  998, "Baxcalibur", TYPE_DRAGON, TYPE_ICE, 254, 168, 229, "Arctibax", BAXCALIBUR_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  999, "Gimmighoul", TYPE_GHOST, TYPECOUNT, 140, 76, 128, NULL, GIMMIGHOUL_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  { 1000, "Gholdengo", TYPE_STEEL, TYPE_GHOST, 252, 190, 202, "Gimmighoul", GHOLDENGO_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  //{ 1001, "Wo-Chien", TYPE_DARK, TYPE_GRASS, 186, 242, 198, nullptr, },
  //{ 1002, "Chien-Pao", TYPE_DARK, TYPE_ICE, 261, 167, 190, nullptr, },
  //{ 1003, "Ting-Lu", TYPE_DARK, TYPE_GROUND, 194, 203, 321, nullptr, },
  //{ 1004, "Chi-Yu", TYPE_DARK, TYPE_FIRE, 269, 221, 146, nullptr, },
  //{ 1005, "Roaring Moon", TYPE_DRAGON, TYPE_DARK, 280, 196, 233, nullptr, },
  //{ 1006, "Iron Valiant", TYPE_FAIRY, TYPE_FIGHTING, 279, 171, 179, nullptr, },
  //{ 1007, "Koraidon", TYPE_FIGHTING, TYPE_DRAGON, 263, 223, 205, nullptr, },
  //{ 1008, "Miraidon", TYPE_ELECTRIC, TYPE_DRAGON, 263, 223, 205, nullptr, },
  //{ 1009, "Walking Wake", TYPE_WATER, TYPE_DRAGON, nullptr, },
  //{ 1010, "Iron Leaves", TYPE_GRASS, TYPE_PSYCHIC, nullptr, },
  //{ 1011, "Dipplin", TYPE_GRASS, TYPE_DRAGON, nullptr, },
  //{ 1012, "Poltchageist", TYPE_GRASS, TYPE_GHOST, nullptr, },
  //{ 1013, "Sinistcha", TYPE_GRASS, TYPE_GHOST, nullptr, },
  //{ 1014, "Okidogi", TYPE_POISON, TYPE_FIGHTING, nullptr, },
  //{ 1015, "Munkidori", TYPE_POISON, TYPE_PSYCHIC, nullptr, },
  //{ 1016, "Fezandipti", TYPE_POISON, TYPE_FAIRY, nullptr, },
  //{ 1018, "Archaludon", TYPE_STEEL, TYPE_DRAGON, nullptr, },
  //{ 1019, "Hydrapple", TYPE_GRASS, TYPE_DRAGON, nullptr, },
  //{ 1020, "Gouging Fire", TYPE_FIRE, TYPE_DRAGON, nullptr, },
  //{ 1021, "Raging Bolt", TYPE_ELECTRIC, TYPE_DRAGON, nullptr, },
  //{ 1022, "Iron Boulder", TYPE_ROCK, TYPE_PSYCHIC, nullptr, },
  //{ 1023, "Iron Crown", TYPE_STEEL, TYPE_PSYCHIC, nullptr, },
  //{ 1025, "Pecharunt", TYPE_POISON, TYPE_GHOST, nullptr, },
};

#define SPECIESCOUNT (sizeof(sdex) / sizeof(*sdex))

// mega and primal forms are never shadows
static const species megasdex[] = {
  {    3, "Mega Venusaur", TYPE_GRASS, TYPE_POISON, 241, 246, 190, "Venusaur", VENUSAUR_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {    6, "Mega Charizard X", TYPE_FIRE, TYPE_DRAGON, 273, 213, 186, "Charizard", CHARIZARD_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {    6, "Mega Charizard Y", TYPE_FIRE, TYPE_FLYING, 319, 212, 186, "Charizard", CHARIZARD_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {    9, "Mega Blastoise", TYPE_WATER, TYPECOUNT, 264, 237, 188, "Blastoise", BLASTOISE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {   15, "Mega Beedrill", TYPE_BUG, TYPE_POISON, 303, 148, 163, "Beedrill", BEEDRILL_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {   18, "Mega Pidgeot", TYPE_NORMAL, TYPE_FLYING, 280, 175, 195, "Pidgeot", PIDGEOT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {   65, "Mega Alakazam", TYPE_PSYCHIC, TYPECOUNT, 367, 207, 146, "Alakazam", ALAKAZAM_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {   80, "Mega Slowbro", TYPE_WATER, TYPE_PSYCHIC, 224, 259, 216, "Slowbro", SLOWBRO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {   94, "Mega Gengar", TYPE_GHOST, TYPE_POISON, 349, 199, 155, "Gengar", GENGAR_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  115, "Mega Kangaskhan", TYPE_NORMAL, TYPECOUNT, 246, 210, 233, "Kangaskhan", KANGASKHAN_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  127, "Mega Pinsir", TYPE_BUG, TYPE_FLYING, 305, 231, 163, "Pinsir", PINSIR_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  130, "Mega Gyarados", TYPE_WATER, TYPE_DARK, 292, 247, 216, "Gyarados", GYARADOS_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  142, "Mega Aerodactyl", TYPE_ROCK, TYPE_FLYING, 292, 210, 190, "Aerodactyl", AERODACTYL_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  // not yet released
  //{  150, "Mega Mewtwo X", TYPE_PSYCHIC, TYPE_FIGHTING, 412, 222, 235, "Mewtwo", MEWTWO_ATKS, false, false, {}, species::CAT_LEGENDARY, nullptr, },
  //{  150, "Mega Mewtwo Y", TYPE_PSYCHIC, TYPECOUNT, 426, 229, 235, "Mewtwo", MEWTWO_ATKS, false, false, {}, species::CAT_LEGENDARY, nullptr, },
  {  181, "Mega Ampharos", TYPE_ELECTRIC, TYPE_DRAGON, 294, 203, 207, "Ampharos", AMPHAROS_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  208, "Mega Steelix", TYPE_STEEL, TYPE_GROUND, 212, 327, 181, "Steelix", STEELIX_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  212, "Mega Scizor", TYPE_BUG, TYPE_STEEL, 279, 250, 172, "Scizor", SCIZOR_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  214, "Mega Heracross", TYPE_BUG, TYPE_FIGHTING, 334, 223, 190, "Heracross", HERACROSS_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  229, "Mega Houndoom", TYPE_DARK, TYPE_FIRE, 289, 194, 181, "Houndoom", HOUNDOOM_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  248, "Mega Tyranitar", TYPE_ROCK, TYPE_DARK, 309, 276, 225, "Tyranitar", TYRANITAR_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  254, "Mega Sceptile", TYPE_GRASS, TYPE_DRAGON, 320, 186, 172, "Sceptile", SCEPTILE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  257, "Mega Blaziken", TYPE_FIRE, TYPE_FIGHTING, 329, 168, 190, "Blaziken", BLAZIKEN_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  260, "Mega Swampert", TYPE_WATER, TYPE_GROUND, 283, 218, 225, "Swampert", SWAMPERT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  282, "Mega Gardevoir", TYPE_PSYCHIC, TYPE_FAIRY, 326, 229, 169, "Gardevoir", GARDEVOIR_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  302, "Mega Sableye", TYPE_DARK, TYPE_GHOST, 151, 216, 137, "Sableye", SABLEYE_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  303, "Mega Mawile", TYPE_STEEL, TYPE_FAIRY, 188, 217, 137, "Mawile", MAWILE_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  306, "Mega Aggron", TYPE_STEEL, TYPECOUNT, 247, 331, 172, "Aggron", AGGRON_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  308, "Mega Medicham", TYPE_FIGHTING, TYPE_PSYCHIC, 205, 179, 155, "Medicham", MEDICHAM_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  310, "Mega Manectric", TYPE_ELECTRIC, TYPECOUNT, 286, 179, 172, "Manectric", MANECTRIC_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  // not yet released
  // {  319, "Mega Sharpedo", TYPE_DARK, TYPE_WATER, 289, 144, 172, "Sharpedo", SHARPEDO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  334, "Mega Altaria", TYPE_DRAGON, TYPE_FAIRY, 222, 218, 181, "Altaria", ALTARIA_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  354, "Mega Banette", TYPE_GHOST, TYPECOUNT, 312, 160, 162, "Banette", BANETTE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  359, "Mega Absol", TYPE_DARK, TYPECOUNT, 314, 130, 163, "Absol", ABSOL_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  362, "Mega Glalie", TYPE_ICE, TYPECOUNT, 252, 168, 190, "Glalie", GLALIE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  373, "Mega Salamence", TYPE_DRAGON, TYPE_FLYING, 310, 251, 216, "Salamence", SALAMENCE_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  380, "Mega Latias", TYPE_DRAGON, TYPE_PSYCHIC, 289, 297, 190, "Latias", LATIAS_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  381, "Mega Latios", TYPE_DRAGON, TYPE_PSYCHIC, 335, 241, 190, "Latios", LATIOS_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  382, "Primal Kyogre", TYPE_WATER, TYPECOUNT, 353, 268, 218, "Kyogre", KYOGRE_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  383, "Primal Groudon", TYPE_GROUND, TYPE_FIRE, 353, 268, 218, "Groudon", GROUDON_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  384, "Mega Rayquaza", TYPE_DRAGON, TYPE_FLYING, 377, 210, 227, "Rayquaza", RAYQUAZA_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  428, "Mega Lopunny", TYPE_NORMAL, TYPE_FIGHTING, 282, 214, 163, "Lopunny", LOPUNNY_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  445, "Mega Garchomp", TYPE_DRAGON, TYPE_GROUND, 339, 222, 239, "Garchomp", GARCHOMP_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  448, "Mega Lucario", TYPE_FIGHTING, TYPE_STEEL, 310, 175, 172, "Lucario", LUCARIO_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  460, "Mega Abomasnow", TYPE_GRASS, TYPE_ICE, 240, 191, 207, "Abomasnow", ABOMASNOW_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  475, "Mega Gallade", TYPE_PSYCHIC, TYPE_FIGHTING, 326, 230, 169, "Gallade", GALLADE_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  531, "Mega Audino", TYPE_NORMAL, TYPE_FAIRY, 147, 239, 230, "Audino", AUDINO_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  719, "Mega Diancie", TYPE_ROCK, TYPE_FAIRY, 342, 235, 137, "Diancie", DIANCIE_ATKS, true, false, {}, species::CAT_NORMAL, 100, nullptr, },
};

#define MEGACOUNT (sizeof(megasdex) / sizeof(*megasdex))

// dynamax forms are never shadows
static const species dynadex[] = {
  {    3, "Dmax Venusaur", TYPE_GRASS, TYPE_POISON, 198, 189, 190, "Venusaur", VENUSAUR_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {    6, "Dmax Charizard", TYPE_FIRE, TYPE_FLYING, 223, 173, 186, "Charizard", CHARIZARD_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {    9, "Dmax Blastoise", TYPE_WATER, TYPECOUNT, 171, 207, 188, "Blastoise", BLASTOISE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {   12, "Dmax Butterfree", TYPE_BUG, TYPE_FLYING, 167, 137, 155, "Butterfree", BUTTERFREE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {   68, "Dmax Machamp", TYPE_FIGHTING, TYPECOUNT, 234, 159, 207, "Machamp", MACHAMP_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {   94, "Dmax Gengar", TYPE_GHOST, TYPE_POISON, 261, 149, 155, "Gengar", GENGAR_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {   99, "Dmax Kingler", TYPE_WATER, TYPECOUNT, 240, 181, 146, "Kingler", KINGLER_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  144, "Dmax Articuno", TYPE_ICE, TYPE_FLYING, 192, 236, 207, "Articuno", ARTICUNO_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  145, "Dmax Zapdos", TYPE_ELECTRIC, TYPE_FLYING, 253, 185, 207, "Zapdos", ZAPDOS_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  146, "Dmax Moltres", TYPE_FIRE, TYPE_FLYING, 251, 181, 207, "Moltres", MOLTRES_ATKS, true, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  242, "Dmax Blissey", TYPE_NORMAL, TYPECOUNT, 129, 169, 496, "Blissey", BLISSEY_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  243, "Dmax Raikou", TYPE_ELECTRIC, TYPECOUNT, 241, 195, 207, "Raikou", RAIKOU_ATKS, true, false, {}, species::CAT_NORMAL, 100, nullptr, },
  {  244, "Dmax Entei", TYPE_FIRE, TYPECOUNT, 235, 171, 251, "Entei", ENTEI_ATKS, true, false, {}, species::CAT_NORMAL, 100, nullptr, },
  {  245, "Dmax Suicune", TYPE_WATER, TYPECOUNT, 180, 235, 225, "Suicune", SUICUNE_ATKS, true, false, {}, species::CAT_NORMAL, 100, nullptr, },
  {  302, "Dmax Sableye", TYPE_DARK, TYPE_GHOST, 151, 216, 137, "Sableye", SABLEYE_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  321, "Dmax Wailord", TYPE_WATER, TYPECOUNT, 175, 87, 347, "Wailord", WAILORD_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  376, "Dmax Metagross", TYPE_STEEL, TYPE_PSYCHIC, 257, 228, 190, "Metagross", METAGROSS_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  521, "Dmax Unfezant", TYPE_NORMAL, TYPE_FLYING, 226, 146, 190, "Unfezant", UNFEZANT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  530, "Dmax Excadrill", TYPE_GROUND, TYPE_STEEL, 255, 129, 242, "Excadrill", EXCADRILL_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  555, "Dmax Darmanitan", TYPE_FIRE, TYPECOUNT, 263, 114, 233, "Darmanitan", DARMANITAN_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  615, "Dmax Cryogonal", TYPE_ICE, TYPECOUNT, 190, 218, 190, "Cryogonal", CRYOGONAL_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  766, "Dmax Passimian", TYPE_FIGHTING, TYPECOUNT, 222, 160, 225, "Passimian", PASSIMIAN_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  812, "Dmax Rillaboom", TYPE_GRASS, TYPECOUNT, 239, 168, 225, "Rillaboom", RILLABOOM_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  815, "Dmax Cinderace", TYPE_FIRE, TYPECOUNT, 238, 163, 190, "Cinderace", CINDERACE_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  818, "Dmax Inteleon", TYPE_WATER, TYPECOUNT, 262, 142, 172, "Inteleon", INTELEON_ATKS, false, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  820, "Dmax Greedent", TYPE_NORMAL, TYPECOUNT, 160, 156, 260, "Greedent", GREEDENT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  823, "Dmax Corviknight", TYPE_FLYING, TYPE_STEEL, 163, 192, 221, "Corviknight", CORVIKNIGHT_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  832, "Dmax Dubwool", TYPE_NORMAL, TYPECOUNT, 159, 198, 176, "Dubwool", DUBWOOL_ATKS, true, false, {}, species::CAT_NORMAL, 10, nullptr, },
  {  849, "Dmax Toxtricity", TYPE_ELECTRIC, TYPE_POISON, 224, 140, 181, "Toxtricity", TOXTRICITY_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  858, "Dmax Hatterene", TYPE_PSYCHIC, TYPE_FAIRY, 237, 182, 149, "Hatterene", HATTERENE_ATKS, true, false, {}, species::CAT_NORMAL, 50, nullptr, },
  {  870, "Dmax Falinks", TYPE_FIGHTING, TYPECOUNT, 193, 170, 163, "Falinks", FALINKS_ATKS, true, false, {}, species::CAT_NORMAL, 75, nullptr, },
  {  892, "Single Strike Style Dmax Urshifu", TYPE_FIGHTING, TYPE_DARK, 254, 177, 225, "Single Strike Style Urshifu", S_URSHIFU_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
  {  892, "Rapid Strike Style Dmax Urshifu", TYPE_FIGHTING, TYPE_WATER, 254, 177, 225, "Rapid Strike Style Urshifu", R_URSHIFU_ATKS, false, false, {}, species::CAT_LEGENDARY, 100, nullptr, },
};

#define DYNACOUNT (sizeof(dynadex) / sizeof(*dynadex))

// gigantimax forms are never shadows
static const species gigantasdex[] = {
  {    3, "Gmax Venusaur", TYPE_GRASS, TYPE_POISON, 198, 189, 190, "Venusaur", VENUSAUR_ATKS, true, false, {}, species::CAT_NORMAL, 10, "G-Max Vine Lash", },
  {    6, "Gmax Charizard", TYPE_FIRE, TYPE_FLYING, 223, 173, 186, "Charizard", CHARIZARD_ATKS, true, false, {}, species::CAT_NORMAL, 10, "G-Max Wildfire", },
  {    9, "Gmax Blastoise", TYPE_WATER, TYPECOUNT, 171, 207, 188, "Blastoise", BLASTOISE_ATKS, true, false, {}, species::CAT_NORMAL, 10, "G-Max Cannonade", },
  {   12, "Gmax Butterfree", TYPE_BUG, TYPE_FLYING, 167, 137, 155, "Butterfree", BUTTERFREE_ATKS, true, false, {}, species::CAT_NORMAL, 10, "G-Max Befuddle", },
  {   68, "Gmax Machamp", TYPE_FIGHTING, TYPECOUNT, 234, 159, 207, "Machamp", MACHAMP_ATKS, true, false, {}, species::CAT_NORMAL, 50, "G-Max Chi Strike", },
  {   94, "Gmax Gengar", TYPE_GHOST, TYPE_POISON, 261, 149, 155, "Gengar", GENGAR_ATKS, true, false, {}, species::CAT_NORMAL, 50, "G-Max Terror", },
  {   99, "Gmax Kingler", TYPE_WATER, TYPECOUNT, 240, 181, 146, "Kingler", KINGLER_ATKS, true, false, {}, species::CAT_NORMAL, 50, "G-Max Foam Burst", },
  {  131, "Gmax Lapras", TYPE_WATER, TYPE_ICE, 165, 174, 277, "Lapras", LAPRAS_ATKS, true, false, {}, species::CAT_NORMAL, 75, "G-Max Resonance", },
  {  143, "Gmax Snorlax", TYPE_NORMAL, TYPECOUNT, 190, 169, 330, "Snorlax", SNORLAX_ATKS, true, false, {}, species::CAT_NORMAL, 75, "G-Max Replenish", },
  {  812, "Gmax Rillaboom", TYPE_GRASS, TYPECOUNT, 239, 168, 225, "Rillaboom", RILLABOOM_ATKS, true, false, {}, species::CAT_NORMAL, 10, "G-Max Drum Solo", },
  {  815, "Gmax Cinderace", TYPE_FIRE, TYPECOUNT, 238, 163, 190, "Cinderace", CINDERACE_ATKS, true, false, {}, species::CAT_NORMAL, 10, "G-Max Fireball", },
  {  818, "Gmax Inteleon", TYPE_WATER, TYPECOUNT, 262, 142, 172, "Inteleon", INTELEON_ATKS, false, false, {}, species::CAT_NORMAL, 10, "G-Max Hydrosnipe", },
  {  858, "Gmax Hatterene", TYPE_PSYCHIC, TYPE_FAIRY, 237, 182, 149, "Hatterene", HATTERENE_ATKS, true, false, {}, species::CAT_NORMAL, 50, "G-Max Smite", },
  {  849, "Gmax Toxtricity", TYPE_ELECTRIC, TYPE_POISON, 224, 140, 181, "Toxtricity", TOXTRICITY_ATKS, true, false, {}, species::CAT_NORMAL, 75, "G-Max Stun Shock", },
};

#define GIGANTACOUNT (sizeof(gigantasdex) / sizeof(*gigantasdex))

static const struct spokedex {
  const species *dex;
  unsigned dcount;
} sdexen[] = {
  { sdex, SPECIESCOUNT, },
  { megasdex, MEGACOUNT, },
  { dynadex, DYNACOUNT, },
  { gigantasdex, GIGANTACOUNT, },
  { NULL, 0, }
};

typedef struct stats {
  const species* s;
  unsigned atk, def, sta;   // base stats for the Form
  unsigned hlevel;          // halflevel 1..99
  unsigned ia, id, is;      // individual vector components
  float effa, effd;         // effective attack and defense
  unsigned mhp;             // max hit points
  unsigned cp;              // combat power
  float average;            // arithemetic mean of effa, effd, mhp
  float geommean;           // geometric mean of effa, effd, mhp
  float apercent;           // geommean advantage over pessimal level-maxed iv
  struct stats* next;
} stats;

// atk, def, and sta all ought be mod forms (i.e. sum of base and IV)
static int
calccp(unsigned atk, unsigned def, unsigned sta, unsigned halflevel){
  float cand = (atk * sqrt(def) * sqrt(sta) * pow(cpm(halflevel), 2)) / 10;
  return cand < 10 ? 10 : floor(cand);
}

static inline float
calc_eff_a_raw(unsigned atk, unsigned halflevel){
  return cpm(halflevel) * atk;
}

// calculate eff_a and apply the shadow bonus, if appropriate.
// provide mod_a as atk.
static inline float
calc_eff_a(unsigned atk, unsigned halflevel, bool isshadow){
  float effa = calc_eff_a_raw(atk, halflevel);
  if(isshadow){
    effa = effa * 6 / 5;
  }
  return effa;
}

// calculate eff_d and apply the shadow penalty, if appropriate.
// provide mod_d as def.
static inline float
calc_eff_d(unsigned def, unsigned halflevel, bool isshadow){
  float s = cpm(halflevel) * def;
  if(isshadow){
    s = s * 5 / 6;
  }
  return s;
}

// provide mod_s
static inline unsigned
calc_mhp(unsigned sta, unsigned halflevel){
  return floor(cpm(halflevel) * sta);
}

static inline float
calc_avg(float effa, float effd, unsigned mhp){
  return (effa + effd + mhp) / 3;
}

static inline float
calc_fit(float effa, float effd, unsigned mhp){
  return cbrt(effa * effd * mhp);
}

static inline float
calc_ppe(const attack *a){
  return a->powertrain / (float)-a->energytrain;
}

// returns integer part, sets *half to 1 if it's a +0.5
// (aren't guaranteed exact representation with floats)
static inline unsigned
halflevel_to_level(unsigned hl, unsigned* half){
  *half = !(hl % 2);
  return (hl + 1) / 2;
}

// FIXME binary search on it
static unsigned
maxlevel_cp_bounded(unsigned atk, unsigned def, unsigned sta, int cpceil, int *cp){
  unsigned lastgood = 0;
  *cp = 0;
  for(unsigned hl = 1 ; hl <= MAX_HALFLEVEL ; ++hl){
    int tmpc = calccp(atk, def, sta, hl);
    if(tmpc <= cpceil){
      lastgood = hl;
      *cp = tmpc;
    }else{
      break;
    }
  }
  return lastgood;
}

// optimize on arithmetic or geometric mean subject to floor
static int
update_optset(stats** osets, const species* s, unsigned ia, unsigned id,
              unsigned is, unsigned hl, float floor, float* minmean,
              bool isshadow, bool amean){
  stats **prev = osets;
  stats *cur;
  unsigned moda = s->atk + ia;
  unsigned modd = s->def + id;
  float effa = calc_eff_a(moda, hl, isshadow);
  float effd = calc_eff_d(modd, hl, isshadow);
  unsigned mods = s->sta + is;
  unsigned mhp = calc_mhp(s->sta + is, hl);
  float am = calc_avg(effa, effd, mhp);
  float gm = calc_fit(effa, effd, mhp);
  float m = amean ? am : gm;
  if(m < *minmean || *minmean <= 0){
    *minmean = m;
  }
  if(m < floor){
    return 0;
  }
  while( (cur = *prev) ){
    if(hl < cur->hlevel){
      break; // we're a lower level than any on the list; insert
    }
    if(hl == cur->hlevel){ // need compare
      if(effa == cur->effa && effd == cur->effd && mhp == cur->mhp){
        // we're equal to something on the list; insert here
        break;
      }else if(effa <= cur->effa && effd <= cur->effd && mhp <= cur->mhp){
        // we're strictly less than something on the list; exit
        return 0;
      }else if(effa >= cur->effa && effd >= cur->effd && mhp >= cur->mhp){
        // we're strictly better than something on the list; remove it and continue
        *prev = cur->next;
        delete cur;
      }else{
        // we're not comparable; continue
        prev = &cur->next;
      }
    }else{
      prev = &cur->next;
    }
  }
  cur = new(stats);
  cur->atk = s->atk;
  cur->def = s->def;
  cur->sta = s->sta;
  cur->hlevel = hl;
  cur->ia = ia;
  cur->id = id;
  cur->is = is;
  cur->effa = effa;
  cur->effd = effd;
  cur->mhp = mhp;
  cur->cp = calccp(moda, modd, mods, cur->hlevel);
  cur->average = am;
  cur->geommean = gm;
  cur->next = *prev;
  *prev = cur;
  return 0;
}

// returns the optimal levels+ivs (using arithmetic or geometric mean of effA,
// effD, and MHP) with a CP less than or equal to cpceil and arithmetic or
// geometric mean of EffA, EffD and MHP greater than or equal to floor.
stats *find_optimal_set(const species* s, int cpceil, float floor, bool isshadow, bool amean){
  stats* optsets = NULL;
  float minmean = -1;
  for(int iva = 0 ; iva < 16 ; ++iva){
    for(int ivd = 0 ; ivd < 16 ; ++ivd){
      for(int ivs = 0 ; ivs < 16 ; ++ivs){
        int cp;
        unsigned hl = maxlevel_cp_bounded(s->atk + iva, s->def + ivd, s->sta + ivs, cpceil, &cp);
        if(update_optset(&optsets, s, iva, ivd, ivs, hl, floor, &minmean, isshadow, amean) < 0){
          return NULL;
        }
      }
    }
  }
  stats* collectopt = NULL;
  stats** qopt = &collectopt;
  float maxmean = -1;
  // print the optimal frontier (large), and extract the truly optimal sets (small)
  while(optsets){
    stats* cur;
    cur = optsets;
    optsets = cur->next;
    cur->s = s;
    float m = amean ? cur->average : cur->geommean;
    //printf(" %u/%u/%u: %2u %4u %.3f %.3f %u %.3f\n", cur->ia, cur->id, cur->is,
    //    cur->hlevel, cur->cp, cur->effa, cur->effd, cur->mhp, cur->geommean);
    if(m > maxmean){ // new optimal
      stats* c;
      // clean out existing true optimals
      while( (c = collectopt) ){
        collectopt = c->next;
        delete c;
      }
      collectopt = cur;
      qopt = &cur->next;
      cur->next = NULL;
      maxmean = m;
    }else if(m == maxmean){ // FIXME unsafe FP comparison
      *qopt = cur;
      qopt = &cur->next;
      cur->next = NULL;
    }else{
      delete cur;
    }
  }
  for(stats *ss = collectopt ; ss ; ss = ss->next){
    ss->apercent = ((amean ? ss->average : ss->geommean) / minmean - 1.0) * 100;
  }
  return collectopt;
}

const species* lookup_species(const char* name){
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    if(strcasecmp(sdex[i].name.c_str(), name) == 0){
      return &sdex[i];
    }
  }
  return NULL;
}

const species* lookup_mega(const char* name){
  for(unsigned i = 0 ; i < MEGACOUNT ; ++i){
    if(strcasecmp(megasdex[i].name.c_str(), name) == 0){
      return &megasdex[i];
    }
  }
  return NULL;
}

void print_type(pgo_types_e t){
  if(t != TYPECOUNT){
    printf("\\calign{\\includegraphics[height=1em,keepaspectratio]{images/%s.png}}", tnames[t]);
  }
}

// emit the symbols for some typing. nothing is shown for TYPECOUNT, and
// monotypes are only displayed once.
void print_types(pgo_types_e t1, pgo_types_e t2){
  print_type(t1);
  if(t1 != t2){
    putc(' ', stdout);
    print_type(t2);
  }
}

void print_type_big(pgo_types_e t){
  if(t != TYPECOUNT){
    printf("\\calign{\\includegraphics[height=2em,keepaspectratio]{images/%s.png}}", tnames[t]);
  }
}

int print_types_big(pgo_types_e t1, pgo_types_e t2){
  print_type_big(t1);
  if(t1 != t2){
    putc(' ', stdout);
    print_type_big(t2);
    return 2;
  }
  return 1;
}

int print_weather_big(pgo_weather_t w, bool doprint){
  int count = 1;
  const char *ws = WNames[w];
  if(doprint){
    printf("\\calign{\\includegraphics[height=2em,keepaspectratio]{images/%s.png}} ", ws);
  }
  ws = WSNames[w];
  if(ws){
    ++count;
    if(doprint){
      printf("\\calign{\\includegraphics[height=2em,keepaspectratio]{images/%s.png}} ", ws);
    }
  }
  return count;
}

int print_weathers_big(pgo_types_e t1, pgo_types_e t2, bool doprint){
  int count = 0;
  pgo_weather_t w1 = wboosts[t1];
  if(t2 != TYPECOUNT){
    pgo_weather_t w2 = wboosts[t2];
    if(w2 != w1){
      count += print_weather_big(w2, doprint);
    }
  }
  count += print_weather_big(w1, doprint);
  return count;
}

static unsigned
learner_count_sdex(const attack *as, const species *dex, unsigned dcount){
  unsigned count = 0;
  for(unsigned u = 0 ; u < dcount ; ++u){
    const species* s = &dex[u];
    if(s->attacks){
      for(const attack** a = s->attacks ; *a ; ++a){
        if(strcmp((*a)->name, as->name) == 0){
          ++count;
          break;
        }
      }
    }
  }
  return count;
}

// how many species can learn this attack?
unsigned learner_count(const attack* as){
  unsigned count = 0;
  for(auto &sd : sdexen){
    count += learner_count_sdex(as, sd.dex, sd.dcount);
  }
  return count;
}

void print_optimal_latex(const species* sp){
  stats* s = find_optimal_set(sp, 2500, 0, false, false);
  printf("\\hfill{}");
  unsigned cp = 0;
  unsigned printed = 0;
  while(s){
    stats* tmp = s->next;
    cp = s->cp;
    if(++printed < 3){
      unsigned half;
      unsigned l = halflevel_to_level(s->hlevel, &half);
      printf("\\ivlev{%u}{%u}{%u}{%2u%s}", s->ia, s->id, s->is, l, half ? ".5" : "");
      printf(" (%u) ", s->cp);
    }
    delete s;
    s = tmp;
  }
  if(printed >= 3){
    printf("(%u more)", printed - 2);
  }
  printed = 0;
  if(cp >= 1500){
    s = find_optimal_set(sp, 1500, 0, false, false);
    printf("\\newline{}\\hfill{}");
    while(s){
      stats* tmp = s->next;
      if(++printed < 3){
        unsigned half;
        unsigned l = halflevel_to_level(s->hlevel, &half);
        printf("\\ivlev{%u}{%u}{%u}{%2u%s}", s->ia, s->id, s->is, l, half ? ".5" : "");
        printf(" (%u) ", s->cp);
      }
      delete s;
      s = tmp;
    }
  }
  if(printed >= 3){
    printf("(%u more)", printed - 2);
  }
}

static inline unsigned
has_stab_p(const species *s, const attack *a){
  return a->type == s->t1 || a->type == s->t2;
}

// apply the 1.2x stab multiplier to a damage calculation
static inline float
calc_stab(float d){
  return d * 6 / 5;
}

static int
escape_filename(const char *s){
  for(const char* curs = s ; *curs ; ++curs){
    if(*curs != '\'' && *curs != '%'){
      if(printf("%c", *curs) < 0){
        return -1;
      }
    } // simply drop apostrophes and percents?
  }
  return 0;
}

static int
escape_string(const char *s){
  for(const char* curs = s ; *curs ; ++curs){
    if(*curs != '%'){
      if(printf("%c", *curs) < 0){
        return -1;
      }
    }else{
      if(printf("\\%%") < 0){
        return -1;
      }
    }
  }
  return 0;
}

// make a string fit for a label.
static int
label_string(const char *s){
  for(const char* curs = s ; *curs ; ++curs){
    if(*curs == '%'){
      continue;
    /*}else if(isspace(*curs)){
      continue;*/
    }else{
      if(printf("%c", *curs) < 0){
        return -1;
      }
    }
  }
  return 0;
}

static bool
exclusive_attack_p(const species *s, const attack *a){
  for(const auto &atk : s->elite){
    if(strcmp(a->name, atk->name) == 0){
      return true;
    }
  }
  return false;
}

// does this species have a mega form?
static bool
has_mega(const species *s){
  for(unsigned i = 0 ; i < MEGACOUNT ; ++i){
    if(strcmp(megasdex[i].from, s->name.c_str()) == 0){
      return true;
    }
  }
  return false;
}

// does this species have a Dynamax form?
static bool
has_dmax(const species *s){
  for(unsigned i = 0 ; i < DYNACOUNT ; ++i){
    if(strcmp(dynadex[i].from, s->name.c_str()) == 0){
      return true;
    }
  }
  return false;
}

// does this species have a Gigantamax form?
static bool
has_gmax(const species *s){
  for(unsigned i = 0 ; i < GIGANTACOUNT ; ++i){
    if(strcmp(gigantasdex[i].from, s->name.c_str()) == 0){
      return true;
    }
  }
  return false;
}

// return its evolution target, if there is one
// FIXME what if there are multiple?
const species *get_persistent_evolution(const species *s){
  const char *cstr = s->name.c_str();
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const species *t = &sdex[i];
    if(t->from && !strcmp(t->from, cstr)){
      return t;
    }
  }
  return NULL;
}

const species *get_previous_evolution(const species *s){
  if(s->from == NULL){
    return NULL;
  }
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const species *t = &sdex[i];
    if(!strcmp(s->from, t->name.c_str())){
      return t;
    }
  }
  return NULL;
}

static int
a2cost_to_cgroup(int a2cost){
  if(a2cost == 100){
    return 4;
  }else if(a2cost == 75){
    return 3;
  }else if(a2cost == 50){
    return 2;
  }else if(a2cost == 10){
    return 1;
  }
  std::cerr << "invalid a2cost: " << a2cost << std::endl;
  return -1;
}

#define REGION_COUNT 11

static inline int
region_idx_first(unsigned region){
  static const int regfirst[] = {
    1, 152, 252, 387, 494, 650, 722, 808, 810, 899, 906
  };
  if(region > sizeof(regfirst) / sizeof(*regfirst)){
    std::cerr << "don't know region " << region << std::endl;
    throw std::invalid_argument("bad region");
  }
  return regfirst[region];
}

static inline int
region_idx_last(unsigned region){
  if(region == REGION_COUNT - 1){
    return 1025;
  }
  return region_idx_first(region + 1) - 1;
}

// determine generation from pokédex entry. return -1 on failure.
// returns 0..GENERATION_COUNT - 1, which map to 1..GENERATION_COUNT.
static inline int
idx_to_region_int(int idx){
  for(int i = 0 ; i < REGION_COUNT ; ++i){
    if(idx <= region_idx_last(i)){
      return i;
    }
  }
  throw std::invalid_argument("bad idx");
}

static inline const char *
idx_to_region(int idx){
  static const char *regions[] = {
    "Kanto", "Johto", "Hoenn", "Sinnoh", "Unova", "Kalos",
    "Alola", "Unknown", "Galar", "Hisui", "Paldea",
  };
  int r = idx_to_region_int(idx);
  return regions[r];
}

#define GENERATION_COUNT 9

static inline int
generation_idx_last(unsigned gen){
  static const int genlast[GENERATION_COUNT] = {
    151, 251, 386, 493, 649, 721, 809, 905, 1025
  };
  if(gen > sizeof(genlast) / sizeof(*genlast)){
    std::cerr << "don't know generation " << gen << std::endl;
    throw std::invalid_argument("bad generation");
  }
  return genlast[gen];
}

static inline int
generation_idx_first(int gen){
  if(gen == 0){
    return 1;
  }
  return generation_idx_last(gen - 1) + 1;
}

// determine generation from pokédex entry. return -1 on failure.
// returns 0..GENERATION_COUNT - 1, which map to 1..GENERATION_COUNT.
static inline int
idx_to_generation_int(int idx){
  for(int i = 0 ; i < GENERATION_COUNT ; ++i){
    if(idx <= generation_idx_last(i)){
      return i;
    }
  }
  throw std::invalid_argument("bad idx");
}

static inline const char *
idx_to_generation(int idx){
  int g = idx_to_generation_int(idx);
  if(g >= 0 && g < GENERATION_COUNT){
    static const char *genstrs[GENERATION_COUNT] = {
      "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"
    };
    return genstrs[g];
  }
  return nullptr;
}

static void
print_previous_species(const species *s){
  const species *devol = get_previous_evolution(s);
  if(devol){
    print_previous_species(devol);
  }
  escape_string(s->name.c_str());
  printf(" (\\pageref{species:");
  label_string(s->name.c_str());
  printf("}) → ");
  // ugh, special case -- this line occupies too much space
  if(!s->name.compare("Galarian Zigzagoon")){
    printf("\\\\\\hfill\n");
  }
}

int print_icons(const species *s, bool doprint){
  int count = 0;
  if(s->category == species::CAT_ULTRABEAST){
    ++count;
    if(doprint){
      printf(" \\calign{\\includegraphics[height=2em,keepaspectratio]{images/ultrahole.png}}");
    }
  }
  if(has_mega(s)){
    ++count;
    if(doprint){
      printf(" \\calign{\\includegraphics[height=2em,keepaspectratio]{images/mega.png}}");
    }
  }
  count += print_weathers_big(s->t1, s->t2, doprint);
  return count;
}

static void
print_buff(unsigned chance, int buff, const char *sig){
  if(!chance){
    return;
  }
  if(chance != 1000){ // don't print chance if it's 100%
    printf("%g\\%%", chance / 10.0);
  }
  printf("%s", sig);
  if(buff > 0){ // FIXME need indicate magnitude of buff
    printf("↑");
    if(buff > 1){
      printf("%d", buff);
    }
  }else{
    printf("↓");
    if(buff < -1){
      printf("%d", -buff);
    }
  }
}

void print_species_latex(const species* s, bool overzoom, bool bg){
  printf("\\vfill\n");
  printf("\\begin{speciesbox}[title=\\#%04u ", s->idx);
  escape_string(s->name.c_str());
  printf(",title style={left color=%s,right color=%s},after title={",
          TNames[s->t1], s->t2 == TYPECOUNT ? TNames[s->t1] : TNames[s->t2]);
  if(s->shiny){
    printf("\\calign{\\includegraphics[height=1em,keepaspectratio]{images/shiny.png}}");
  }
  printf("\\hfill%u %u %u %.2f %.2f}", s->atk, s->def, s->sta,
      calc_avg(s->atk, s->def, s->sta), calc_fit(s->atk, s->def, s->sta));
  if(overzoom){
    printf(",interior style={fill overzoom image=images/pokédex/");
    escape_filename(s->name.c_str());
    printf(",fill image opacity=0.1}");
  }
  printf("]\\phantomsection\\label{species:");
  label_string(s->name.c_str());
  printf("}{");
  printf("\\footnotesize");

  // the table containing image and attack data
  printf("\\begin{tabularx}{\\linewidth}{@{}c X @{}}");
  printf("\\includegraphics[width=0.3\\linewidth,valign=c,keepaspectratio]{images/pokédex/");
  for(const char* curs = s->name.c_str() ; *curs ; ++curs){
    if(*curs != '%' && *curs != '\''){
      printf("%c", *curs);
    }
  }
  printf(".png} &\\begingroup\\setlength{\\tabcolsep}{4pt}\\begin{tabular}{lrrrrr}\n");
  for(const attack** a = s->attacks ; *a ; ++a){
    unsigned stab = has_stab_p(s, *a);
    float power = (*a)->powertrain;
    if(stab){
      power = power * 6 / 5;
    }
    print_type((*a)->type);
    if((*a)->type == TYPECOUNT){
      printf("\\hspace{1em}");
    }
    if(charged_attack_p(*a)){
      const float dpe = power / -(*a)->energytrain;
      if(exclusive_attack_p(s, *a)){
        printf(" \\textit{%s} & & \\textit{%g} & \\textit{%d} & \\textit{%.2f} &",
            (*a)->name, power, (*a)->energytrain, dpe);
      }else{
        printf(" %s & & %g & %d & %.2f &",
            (*a)->name, power, (*a)->energytrain, dpe);
      }
      if((*a)->chance_user_attack || (*a)->chance_user_defense ||
          (*a)->chance_opp_attack || (*a)->chance_opp_defense){
        printf("{\\scriptsize{}");
        if((*a)->chance_user_attack){
          print_buff((*a)->chance_user_attack, (*a)->user_attack, "A");
        }
        if((*a)->chance_user_defense){
          print_buff((*a)->chance_user_defense, (*a)->user_defense, "D");
        }
        // need special case this as it takes too much space otherwise
        if((*a)->chance_opp_attack && (*a)->chance_opp_attack == (*a)->chance_opp_defense
            && (*a)->opp_attack == (*a)->opp_defense){
          print_buff((*a)->chance_opp_attack, (*a)->opp_attack, "OA+D");
        }else{
          if((*a)->chance_opp_attack){
            print_buff((*a)->chance_opp_attack, (*a)->opp_attack, "OA");
          }
          if((*a)->chance_opp_defense){
            print_buff((*a)->chance_opp_defense, (*a)->opp_defense, "OD");
          }
        }
        printf("}");
      }
      printf("\\\\\n");
    }else{ // fast attacks
      const float dpt = power / (*a)->turns;
      const float ept = static_cast<float>((*a)->energytrain) / (*a)->turns;
      if(exclusive_attack_p(s, *a)){
        printf(" \\textit{%s} & \\textit{%u} & \\textit{%g} & \\textit{%d} & \\textit{%.2f} & \\textit{%.2f}\\\\\n",
            (*a)->name, (*a)->turns, power, (*a)->energytrain, dpt, ept);
      }else{
        printf(" %s & %u & %g & %d & %.2f & %.2f \\\\\n",
            (*a)->name, (*a)->turns, power, (*a)->energytrain, dpt, ept);
      }
    }
  }
  printf("\\end{tabular}\\endgroup\\end{tabularx}\n");

  // the minipages with icons and cp data
  printf("\\noindent\\begin{minipage}{0.3\\linewidth}");
  int largeicons = print_types_big(s->t1, s->t2);
  if(has_gmax(s)){
    ++largeicons;
    printf(" \\calign{\\includegraphics[height=2em,keepaspectratio]{images/gigantamax.png}}");
  }
  if(has_dmax(s)){
    ++largeicons;
    printf(" \\calign{\\includegraphics[height=2em,keepaspectratio]{images/dynamax.png}}");
  }
  // we never want some small icons on both lines if we have to have two lines,
  // but we want only one line if we can get away with it. three small icons
  // are too many to put with two large icons.
  if(largeicons + print_icons(s, false) > 4){
    printf("\\\\");
  }
  print_icons(s, true);
  printf("\\end{minipage}\n");
  if(bg){ // evolutionary lineage (only for main forms)
    printf("\\begin{minipage}{0.7\\linewidth}\\raggedleft{}");
    printf("\\vfill{}\\scriptsize{}");
    print_optimal_latex(s);
    printf("\\end{minipage}\\\\");

    printf("\\scriptsize{}CG %d Gen %s %s%s\\hfill{}", a2cost_to_cgroup(s->a2cost), idx_to_generation(s->idx),
            idx_to_region(s->idx), s->categorystr());
    const species *devol = get_previous_evolution(s);
    const species *evol = get_persistent_evolution(s);
    if(devol || evol){
      if(devol){
        print_previous_species(devol);
      }
      printf("\\textbf{");
      escape_string(s->name.c_str());
      printf("}");
      if(!s->name.compare("Galarian Zigzagoon")){
        printf("\\\\\\hfill\n");
      }
      while(evol){
        printf(" → ");
        escape_string(evol->name.c_str());
        printf(" (\\pageref{species:");
        label_string(evol->name.c_str());
        printf("})");
        evol = get_persistent_evolution(evol);
      }
    }else{
      printf("No evolution");
    }
  }else{
    if(s->gmax){
      printf("\\hfill");
      print_type(s->t1);
      printf(" %s", s->gmax);
    }
  }

  // shadow is implemented as subtitle
  if(s->shadow){
    printf("\\tcbsubtitle[before skip=1pt,bottomrule=0pt]{Shadow ");
    escape_string(s->name.c_str());
    printf("\\hfill{}");
    const float atk = s->atk * 6 / 5.0;
    const float def = s->def * 5 / 6.0;
    const float avg = calc_avg(atk, def, s->sta);
    const float gm = calc_fit(atk, def, s->sta);
    printf("%g %g %u %.2f %.2f}\n", atk, def, s->sta, avg, gm);
  }

  printf("}");
  printf("\\end{speciesbox}\n");
  if(bg){
    printf("\\pagecolor{%s!25!white}", TNames[s->t1]);
  }
}

// print those entries containing type(s). pass TYPECOUNT for a wildcard on t2.
// pass the same type twice for only that base type. LaTeX output.
void filter_by_types(int t1, int t2, const species* dex, unsigned count, bool overzoom){
  for(unsigned i = 0 ; i < count ; ++i){
    bool printit = false;
    if(dex[i].t1 == t1){
      if(dex[i].t2 == t2 || t2 == TYPECOUNT || (t1 == t2 && dex[i].t2 == TYPECOUNT)){
        printit = true;
      }
    }
    if(printit){
      print_species_latex(&dex[i], overzoom, true);
    }
  }
}

// return the named charged attack iff s can learn it
const attack *species_charged_attack(const species *s, const char *aname){
  for(const attack **a = s->attacks ; *a ; ++a){
    if((*a)->energytrain >= 0){
      continue;
    }
    if(strcasecmp(aname, (*a)->name)){
      continue;
    }
    return *a;
  }
  return NULL;
}

// return the named fast attack iff s can learn it
const attack *species_fast_attack(const species *s, const char *aname){
  for(const attack **a = s->attacks ; *a ; ++a){
    if((*a)->energytrain <= 0){
      continue;
    }
    if(strcasecmp(aname, (*a)->name)){
      continue;
    }
    return *a;
  }
  return NULL;
}

// lex out iv and level in the form iva-ivd-ivs@l or optCP
int lex_ivlevel(const char* ivl, stats* s, bool shadow){
  int r;
  unsigned cp;
  // allow leading whitespace
  if((r = sscanf(ivl, " opt%u", &cp)) == 1){
    stats *st = find_optimal_set(s->s, cp, 0, shadow, false);
    if(st == NULL){
      fprintf(stderr, "couldn't find optimal config for cp %u\n", cp);
      return -1;
    }
    s->ia = st->ia;
    s->id = st->id;
    s->is = st->is;
    s->hlevel = st->hlevel;
  }else if((r = sscanf(ivl, " max ")) == 0){
    s->ia = s->id = s->is = MAXIVELEM;
    s->hlevel = MAX_HALFLEVEL;
  }else if((r = sscanf(ivl, " %u-%u-%u@", &s->ia, &s->id, &s->is)) == 3){
    ivl = strchr(ivl, '@') + 1;
    if(!isdigit(*ivl)){
      fprintf(stderr, "error lexing L from %s\n", ivl);
      return -1;
    }
    char *endptr;
    s->hlevel = strtoul(ivl, &endptr, 10);
    while(*endptr){
      if(!isspace(*endptr)){
        fprintf(stderr, "invalid characters after level %s\n", endptr);
        return -1;
      }
      ++endptr;
    }
  }else{
    fprintf(stderr, "error lexing A-D-S from %s (got %d)\n", ivl, r);
    return -1;
  }
  if(s->ia > MAXIVELEM || s->id > MAXIVELEM || s->is > MAXIVELEM){
    fprintf(stderr, "invalid iv %u-%u-%u\n", s->ia, s->id, s->is);
    return -1;
  }
  if(s->hlevel < 1 || s->hlevel > MAX_HALFLEVEL){
    fprintf(stderr, "invalid hlevel %u\n", s->hlevel);
    return -1;
  }
  return 0;
}
