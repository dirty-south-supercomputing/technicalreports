#ifndef PGOTYPES_H
#define PGOTYPES_H

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

constexpr unsigned TEAMSIZE = 3;
constexpr int MAXIVELEM = 15;
constexpr unsigned MINCP = 10; // minimum combat power
constexpr int MAXCHARGEDBUFF = 4;
constexpr unsigned ENERGY_MAX = 100;
constexpr unsigned MAX_HALFLEVEL = 99;
static constexpr auto GLCPCAP = 1500;
static constexpr auto ULCPCAP = 2500;

// returns integer part, sets *half to 1 if it's a +0.5
// (aren't guaranteed exact representation with floats)
static inline unsigned
halflevel_to_level(unsigned hl, unsigned* half){
  *half = !(hl % 2);
  return (hl + 1) / 2;
}

enum pgo_types_e {
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
};

// there are 171 distinct species types (18 + C(18, 2))
#define TYPINGCOUNT 171
// but there are 324 if one considers ordering, which one generally oughtn't
#define TYPECOUNTSQUARED 324

enum pgo_weather_t {
  WEATHER_CLEAR,
  WEATHER_RAIN,
  WEATHER_PARTLY_CLOUDY,
  WEATHER_CLOUDY,
  WEATHER_WINDY,
  WEATHER_SNOW,
  WEATHER_FOG,
  WEATHER_EXTREME,
  WEATHERCOUNT
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

// maybe use values from https://github.com/mathiasbynens/pogocpm2level/blob/master/pogocpm2level/cpm2level.py ?
float cpm(int halflevel){
  static const float cpms[] = {
    0.094, 0.1351374318, 0.166398, 0.192650919, 0.215732, 0.2365726613,
    0.25572, 0.2735303812, 0.29025, 0.3060573775, 0.321088,
    0.3354450362, 0.349213, 0.3624577511, 0.375236, 0.387592416, 0.399567,
    0.4111935514, 0.4225, 0.4329264091, 0.443108, 0.4530599591, 0.462798,
    0.472336093, 0.481685, 0.4908558003, 0.499858, 0.508701765, 0.517394,
    0.5259425113, 0.534354, 0.5426357375, 0.550793, 0.5588305862, 0.566755,
    0.5745691333, 0.582279, 0.5898879072, 0.5974, 0.6048236651, 0.612157,
    0.6194041216, 0.626567, 0.6336491432, 0.640653, 0.6475809666,
    0.654436, 0.6612192524, 0.667934, 0.6745818959, 0.681165, 0.6876849038,
    0.694144, 0.70054287, 0.706884, 0.7131691091, 0.719399, 0.7255756136,
    0.7317, 0.7347410093, 0.737769, 0.7407855938, 0.743789, 0.7467812109,
    0.749761, 0.7527290867, 0.755686, 0.7586303683, 0.761564,
    0.7644860647, 0.767397, 0.7702972656, 0.773187, 0.7760649616,
    0.778933, 0.7817900548, 0.784637, 0.7874736075, 0.7903, 0.792803968,
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
    throw std::invalid_argument("invalid halflevel");
  }
  return cpms[halflevel - 1];
}

static inline float mapbuff(int bufflevel){
  static const float buffmap[9] = { 4/8, 4/7, 4/6, 4/5, 1, 5/4, 6/4, 7/4, 8/4 };
  return buffmap[bufflevel + 4];
}

// currently only have trainer battle stats; need to add raid
struct attack {
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
};

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

static inline int
print_fast_attack_rowcolor(const attack *a){
  float ppt = a->powertrain / (float)a->turns;
  float ept = a->energytrain / (float)a->turns;
  if(ppt * ept >= 12){
    return printf("\\rowcolor{Green!10}");
  }else if(ppt * ept >= 10.5){
    return printf("\\rowcolor{Green!25}");
  }else if(ppt * ept >= 9){
    return printf("\\rowcolor{Green!50}");
  }
  return 0;
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
static const attack ATK_Bug_Bite = { "Bug Bite", TYPE_BUG, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 6, 1, false, };
static const attack ATK_Bullet_Punch = { "Bullet Punch", TYPE_STEEL, 8, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 11, 2, false, };
static const attack ATK_Bullet_Seed = { "Bullet Seed", TYPE_GRASS, 5, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	7, 13, 2, false, };
static const attack ATK_Charge_Beam = { "Charge Beam", TYPE_ELECTRIC, 5, 11, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	7, 14, 2, false, };
static const attack ATK_Charm = { "Charm", TYPE_FAIRY, 13, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	20, 11, 3, false, };
static const attack ATK_Confusion = { "Confusion", TYPE_PSYCHIC, 16, 14, 4, 0, 0, 0, 0, 0, 0, 0, 0,
	19, 14, 3, false, };
static const attack ATK_Counter = { "Counter", TYPE_FIGHTING, 8, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 9, 2, false, };
static const attack ATK_Cut = { "Cut", TYPE_NORMAL, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 1, false, };
static const attack ATK_Double_Kick = { "Double Kick", TYPE_FIGHTING, 8, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 13, 2, false, };
static const attack ATK_Dragon_Breath = { "Dragon Breath", TYPE_DRAGON, 3, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 4, 1, false, };
static const attack ATK_Dragon_Tail = { "Dragon Tail", TYPE_DRAGON, 9, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	14, 8, 2, false, };
static const attack ATK_Ember = { "Ember", TYPE_FIRE, 4, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Fury_Cutter = { "Fury Cutter", TYPE_BUG, 3, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Leafage = { "Leafage", TYPE_GRASS, 6, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Metal_Sound = { "Metal Sound", TYPE_STEEL, 5, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Mud_Shot = { "Mud Shot", TYPE_GROUND, 4, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Mud_Slap = { "Mud-Slap", TYPE_GROUND, 12, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	19, 13, 3, false, };
static const attack ATK_Peck = { "Peck", TYPE_FLYING, 6, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Smack_Down = { "Smack Down", TYPE_ROCK, 12, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 7, 2, false, };
static const attack ATK_Snarl = { "Snarl", TYPE_DARK, 5, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 13, 2, false, };
static const attack ATK_Spark = { "Spark", TYPE_ELECTRIC, 5, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Splash = { "Splash", TYPE_WATER, 0, 12, 4, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 17, 3, false, };
static const attack ATK_Steel_Wing = { "Steel Wing", TYPE_STEEL, 7, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Volt_Switch = { "Volt Switch", TYPE_ELECTRIC, 14, 16, 4, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Aerial_Ace = { "Aerial Ace", TYPE_FLYING, 60, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Aura_Sphere = { "Aura Sphere", TYPE_FIGHTING, 80, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Body_Slam = { "Body Slam", TYPE_NORMAL, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Boomburst = { "Boomburst", TYPE_NORMAL, 150, -70, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	150, 100, 5, false, };
static const attack ATK_Bone_Club = { "Bone Club", TYPE_GROUND, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	40, 33, 3, false, };
static const attack ATK_Brave_Bird = { "Brave Bird", TYPE_FLYING, 130, -55, 0, 0, 1000, 0, 0, 0, -3, 0, 0,
	130, 100, 4, false, };
static const attack ATK_Breaking_Swipe = { "Breaking Swipe", TYPE_DRAGON, 50, -50, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
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
static const attack ATK_Chilling_Water = { "Chilling Water", TYPE_WATER, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	65, 33, 7, false, };
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
static const attack ATK_Doom_Desire = { "Doom Desire", TYPE_STEEL, 80, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 33, 3, false, };
static const attack ATK_Double_Iron_Bash = { "Double Iron Bash", TYPE_STEEL, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 4, false, };
static const attack ATK_Draco_Meteor = { "Draco Meteor", TYPE_DRAGON, 150, -65, 0, 1000, 0, 0, 0, -2, 0, 0, 0,
	150, 100, 7, false, };
static const attack ATK_Dragon_Ascent = { "Dragon Ascent", TYPE_FLYING, 150, -70, 0, 0, 1000, 0, 0, 0, -1, 0, 0,
	140, 50, 7, false, };
static const attack ATK_Dragon_Claw = { "Dragon Claw", TYPE_DRAGON, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 3, false, };
static const attack ATK_Dragon_Energy = { "Dragon Energy", TYPE_DRAGON, 100, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	155, 50, 7, false, };
static const attack ATK_Dragon_Pulse = { "Dragon Pulse", TYPE_DRAGON, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 7, false, };
static const attack ATK_Drain_Punch = { "Drain Punch", TYPE_FIGHTING, 20, -40, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	50, 33, 5, false, };
static const attack ATK_Draining_Kiss = { "Draining Kiss", TYPE_FAIRY, 60, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 5, false, };
static const attack ATK_Drill_Peck = { "Drill Peck", TYPE_FLYING, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Flame_Wheel = { "Flame Wheel", TYPE_FIRE, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Gyro_Ball = { "Gyro Ball", TYPE_STEEL, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Night_Shade = { "Night Shade", TYPE_GHOST, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 5, false, };
static const attack ATK_Night_Slash = { "Night Slash", TYPE_DARK, 50, -35, 0, 125, 0, 0, 0, 2, 0, 0, 0,
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
static const attack ATK_Parabolic_Charge = { "Parabolic Charge", TYPE_ELECTRIC, 70, -50, 0, 0, 660, 0, 0, 0, 1, 0, 0,
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
static const attack ATK_Rock_Slide = { "Rock Slide", TYPE_ROCK, 75, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Scorching_Sands = { "Scorching Sands", TYPE_GROUND, 80, -50, 0, 0, 0, 100, 0, 0, 0, -1, 0,
	90, 50, 6, false, };
static const attack ATK_Seed_Bomb = { "Seed Bomb", TYPE_GRASS, 55, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Sky_Attack = { "Sky Attack", TYPE_FLYING, 75, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Water_Pulse = { "Water Pulse", TYPE_WATER, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 6, false, };
static const attack ATK_Weather_Ball_Fire = { "Weather Ball (Fire)", TYPE_FIRE, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 3, false, };
static const attack ATK_Weather_Ball_Ice = { "Weather Ball (Ice)", TYPE_ICE, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 3, false, };
static const attack ATK_Weather_Ball_Rock = { "Weather Ball (Rock)", TYPE_ROCK, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 4, false, };
static const attack ATK_Weather_Ball_Normal = { "Weather Ball (Normal)", TYPE_NORMAL, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 3, false, };
static const attack ATK_Weather_Ball_Water = { "Weather Ball (Water)", TYPE_WATER, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 3, false, };
static const attack ATK_Wild_Charge = { "Wild Charge", TYPE_ELECTRIC, 100, -45, 0, 0, 1000, 0, 0, 0, -2, 0, 0,
	90, 50, 5, false, };
static const attack ATK_Wildbolt_Storm = { "Wildbolt Storm", TYPE_ELECTRIC, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	150, 100, 5, false, };
static const attack ATK_Wrap = { "Wrap", TYPE_NORMAL, 60, -45, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	25, 33, 6, false, };
static const attack ATK_X_Scissor = { "X-Scissor", TYPE_BUG, 65, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 3, false, };
static const attack ATK_Zap_Cannon = { "Zap Cannon", TYPE_ELECTRIC, 150, -80, 0, 0, 0, 330, 0, 0, 0, -1, 0,
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
  &ATK_Chilling_Water,
  &ATK_Clanging_Scales,
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
  &ATK_X_Scissor,
  &ATK_Zap_Cannon,
};

const size_t ATTACKCOUNT = sizeof(attacks) / sizeof(*attacks);

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

struct species {
  unsigned idx; // pokedex index, not unique
  std::string name;
  pgo_types_e t1, t2;
  unsigned atk;
  unsigned def;
  unsigned sta;
  const char *from;    // from what does it evolve? NULL for nothing
  std::vector<const attack*> attacks; // attacks we can learn
  bool shiny;         // is there a shiny form?
  bool shadow;        // is there a shadow form?
  bool dmax;          // is there a dynamax form?
  std::vector<const attack*> elite; // exclusive attacks requiring an elite tm
  enum species_cat {
    CAT_NORMAL,
    CAT_MYTHICAL,
    CAT_LEGENDARY,
    CAT_ULTRABEAST,
    CAT_BABY,
    CAT_PARADOX,
    CAT_FPARTNER, // first partners (previously "starters")
  } category;
  int a2cost;         // cost in kStardust to teach second attack {-1, 10, 50, 75, 100}
  const char *gmax;   // gmax attack name or NULL
  enum evol_item {
    EVOL_NOITEM,
    EVOL_SUNSTONE,
    EVOL_KINGSROCK,
    EVOL_METALCOAT,
    EVOL_DRAGONSCALE,
    EVOL_UPGRADE,
    EVOL_SINNOHSTONE,
    EVOL_UNOVASTONE,
    EVOL_TARTAPPLE,
    EVOL_SWEETAPPLE,
    EVOL_SYRUPYAPPLE,
    EVOL_GIMMICOINS,
    EVOL_ZYGARDECELL,
  } evolitem;

  species() {
  }

  species(const std::string& s)
      : name(s) {
  }

  species(unsigned i, const char *n, pgo_types_e T1, pgo_types_e T2,
          unsigned A, unsigned D, unsigned S, const char *From,
          const std::vector<const attack*> &Attacks,
          bool Shiny, bool Shadow, bool Dmax,
          const std::vector<const attack*>& Elite,
          species_cat Category, int A2Cost, const char *GMax,
          evol_item Evolitem = EVOL_NOITEM)
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
    dmax(Dmax),
    elite(Elite),
    category(Category),
    a2cost(A2Cost),
    gmax(GMax),
    evolitem(Evolitem)
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
      case CAT_PARADOX: return " Paradox";
      case CAT_FPARTNER: return " First Partner";
      default: throw std::exception();
    }
  }

};

static const species sdex[] = {
  // the Seed Pokemon
  {  1, "Bulbasaur", TYPE_GRASS, TYPE_POISON, 118, 111, 128, nullptr,
		{ &ATK_Vine_Whip, &ATK_Tackle, &ATK_Seed_Bomb, &ATK_Sludge_Bomb, &ATK_Power_Whip, },
		true, true, true, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  2, "Ivysaur", TYPE_GRASS, TYPE_POISON, 151, 143, 155, "Bulbasaur",
		{ &ATK_Vine_Whip, &ATK_Razor_Leaf, &ATK_Sludge_Bomb, &ATK_Solar_Beam, &ATK_Power_Whip, },
		true, true, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  3, "Venusaur", TYPE_GRASS, TYPE_POISON, 198, 189, 190, "Ivysaur",
		{ &ATK_Vine_Whip, &ATK_Razor_Leaf, &ATK_Petal_Blizzard, &ATK_Sludge, &ATK_Sludge_Bomb, &ATK_Solar_Beam, &ATK_Frenzy_Plant, },
		true, true, true, { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, "G-Max Vine Lash", },
  // the Lizard
  {  4, "Charmander", TYPE_FIRE, TYPECOUNT, 116, 93, 118, nullptr,
		{ &ATK_Ember, &ATK_Scratch, &ATK_Flamethrower, &ATK_Flame_Charge, &ATK_Flame_Burst, },
		true, true, true, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  // the Flame
  {  5, "Charmeleon", TYPE_FIRE, TYPECOUNT, 158, 126, 151, "Charmander",
		{ &ATK_Ember, &ATK_Scratch, &ATK_Fire_Fang, &ATK_Flamethrower, &ATK_Flame_Burst, &ATK_Fire_Punch, },
		true, true, true, { &ATK_Scratch, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  6, "Charizard", TYPE_FIRE, TYPE_FLYING, 223, 173, 186, "Charmeleon",
		{ &ATK_Air_Slash, &ATK_Fire_Spin, &ATK_Wing_Attack, &ATK_Ember, &ATK_Dragon_Breath, &ATK_Air_Cutter, &ATK_Fire_Blast, &ATK_Overheat, &ATK_Dragon_Claw, &ATK_Blast_Burn, &ATK_Flamethrower, },
		true, true, true, { &ATK_Dragon_Breath, &ATK_Ember, &ATK_Wing_Attack, &ATK_Blast_Burn, &ATK_Flamethrower, }, species::CAT_NORMAL, 10, "G-Max Wildfire", },
  // the Tiny Turtle
  {  7, "Squirtle", TYPE_WATER, TYPECOUNT, 94, 121, 127, nullptr,
		{ &ATK_Tackle, &ATK_Bubble, &ATK_Aqua_Jet, &ATK_Aqua_Tail, &ATK_Water_Pulse, },
		true, true, true, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  // the Turtle
  {  8, "Wartortle", TYPE_WATER, TYPECOUNT, 126, 155, 153, "Squirtle",
		{ &ATK_Bite, &ATK_Water_Gun, &ATK_Ice_Beam, &ATK_Aqua_Jet, &ATK_Hydro_Pump, },
		true, true, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Shellfish
  {  9, "Blastoise", TYPE_WATER, TYPECOUNT, 171, 207, 188, "Wartortle",
		{ &ATK_Bite, &ATK_Water_Gun, &ATK_Rollout, &ATK_Flash_Cannon, &ATK_Ice_Beam, &ATK_Hydro_Pump, &ATK_Hydro_Cannon, &ATK_Skull_Bash, },
		true, true, true, { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, "G-Max Cannonade", },
  // the Worm
  {  10, "Caterpie", TYPE_BUG, TYPECOUNT, 55, 55, 128, nullptr,
		{ &ATK_Bug_Bite, &ATK_Tackle, &ATK_Struggle, },
		true, true, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Cocoon
  {  11, "Metapod", TYPE_BUG, TYPECOUNT, 45, 80, 137, "Caterpie",
		{ &ATK_Bug_Bite, &ATK_Tackle, &ATK_Struggle, },
		true, true, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Butterfly
  {  12, "Butterfree", TYPE_BUG, TYPE_FLYING, 167, 137, 155, "Metapod",
		{ &ATK_Bug_Bite, &ATK_Confusion, &ATK_Struggle_Bug, &ATK_Bug_Buzz, &ATK_Signal_Beam, &ATK_Psychic, },
		true, true, true, { &ATK_Bug_Bite, }, species::CAT_NORMAL, 10, "G-Max Befuddle", },
  // the Hairy Bug
  {  13, "Weedle", TYPE_BUG, TYPE_POISON, 63, 50, 120, nullptr,
		{ &ATK_Bug_Bite, &ATK_Poison_Sting, &ATK_Struggle, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Cocoon
  {  14, "Kakuna", TYPE_BUG, TYPE_POISON, 46, 75, 128, "Weedle",
		{ &ATK_Bug_Bite, &ATK_Poison_Sting, &ATK_Struggle, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Poison Bee
  {  15, "Beedrill", TYPE_BUG, TYPE_POISON, 169, 130, 163, "Kakuna",
		{ &ATK_Bug_Bite, &ATK_Poison_Jab, &ATK_Poison_Sting, &ATK_Infestation, &ATK_Aerial_Ace, &ATK_Drill_Run, &ATK_Sludge_Bomb, &ATK_X_Scissor, &ATK_Fell_Stinger, },
		true, true, false, { &ATK_Bug_Bite, &ATK_Drill_Run, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Tiny Bird
  {  16, "Pidgey", TYPE_NORMAL, TYPE_FLYING, 85, 73, 120, nullptr,
		{ &ATK_Quick_Attack, &ATK_Tackle, &ATK_Aerial_Ace, &ATK_Twister, &ATK_Air_Cutter, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Bird
  {  17, "Pidgeotto", TYPE_NORMAL, TYPE_FLYING, 117, 105, 160, "Pidgey",
		{ &ATK_Wing_Attack, &ATK_Steel_Wing, &ATK_Aerial_Ace, &ATK_Twister, &ATK_Air_Cutter, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Bird
  {  18, "Pidgeot", TYPE_NORMAL, TYPE_FLYING, 166, 154, 195, "Pidgeotto",
		{ &ATK_Air_Slash, &ATK_Steel_Wing, &ATK_Wing_Attack, &ATK_Gust, &ATK_Hurricane, &ATK_Aerial_Ace, &ATK_Brave_Bird, &ATK_Feather_Dance, &ATK_Air_Cutter, },
		true, true, false, { &ATK_Gust, &ATK_Wing_Attack, &ATK_Air_Cutter, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Mouse
  {  19, "Rattata", TYPE_NORMAL, TYPECOUNT, 103, 70, 102, nullptr,
		{ &ATK_Quick_Attack, &ATK_Tackle, &ATK_Dig, &ATK_Hyper_Fang, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  19, "Alolan Rattata", TYPE_DARK, TYPE_NORMAL, 103, 70, 102, nullptr,
		{ &ATK_Quick_Attack, &ATK_Tackle, &ATK_Shadow_Ball, &ATK_Hyper_Fang, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  20, "Raticate", TYPE_NORMAL, TYPECOUNT, 161, 139, 146, "Rattata",
		{ &ATK_Bite, &ATK_Quick_Attack, &ATK_Hyper_Beam, &ATK_Dig, &ATK_Hyper_Fang, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  20, "Alolan Raticate", TYPE_DARK, TYPE_NORMAL, 135, 154, 181, "Alolan Rattata",
		{ &ATK_Bite, &ATK_Quick_Attack, &ATK_Hyper_Beam, &ATK_Hyper_Fang, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Tiny Bird
  {  21, "Spearow", TYPE_NORMAL, TYPE_FLYING, 112, 60, 120, nullptr,
		{ &ATK_Peck, &ATK_Quick_Attack, &ATK_Drill_Peck, &ATK_Aerial_Ace, &ATK_Twister, &ATK_Sky_Attack, },
		true, false, false, { &ATK_Twister, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Beak
  {  22, "Fearow", TYPE_NORMAL, TYPE_FLYING, 182, 133, 163, "Spearow",
		{ &ATK_Peck, &ATK_Steel_Wing, &ATK_Aerial_Ace, &ATK_Drill_Peck, &ATK_Drill_Run, &ATK_Twister, &ATK_Sky_Attack, &ATK_Fly, },
		true, false, false, { &ATK_Twister, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Snake
  {  23, "Ekans", TYPE_POISON, TYPECOUNT, 110, 97, 111, nullptr,
		{ &ATK_Acid, &ATK_Poison_Sting, &ATK_Wrap, &ATK_Poison_Fang, &ATK_Sludge_Bomb, &ATK_Gunk_Shot, },
		true, true, false, { &ATK_Gunk_Shot, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Cobra
  {  24, "Arbok", TYPE_POISON, TYPECOUNT, 167, 153, 155, "Ekans",
		{ &ATK_Bite, &ATK_Acid, &ATK_Poison_Sting, &ATK_Dragon_Tail, &ATK_Dark_Pulse, &ATK_Sludge_Wave, &ATK_Gunk_Shot, &ATK_Acid_Spray, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Mouse
  {  25, "Pikachu", TYPE_ELECTRIC, TYPECOUNT, 112, 96, 111, "Pichu",
		{ &ATK_Thunder_Shock, &ATK_Quick_Attack, &ATK_Present, &ATK_Discharge, &ATK_Thunder, &ATK_Thunderbolt, &ATK_Wild_Charge, &ATK_Surf, &ATK_Volt_Tackle, },
		true, false, false, { &ATK_Present, &ATK_Surf, &ATK_Thunder, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  26, "Raichu", TYPE_ELECTRIC, TYPECOUNT, 193, 151, 155, "Pikachu",
		{ &ATK_Thunder_Shock, &ATK_Spark, &ATK_Volt_Switch, &ATK_Charm, &ATK_Thunder_Punch, &ATK_Thunder, &ATK_Brick_Break, &ATK_Wild_Charge, &ATK_Surf, &ATK_Skull_Bash, &ATK_Trailblaze, },
		true, false, false, { &ATK_Thunder, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  26, "Alolan Raichu", TYPE_ELECTRIC, TYPE_PSYCHIC, 201, 154, 155, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Spark, &ATK_Volt_Switch, &ATK_Thunder_Punch, &ATK_Psyshock, &ATK_Psychic, &ATK_Wild_Charge, &ATK_Grass_Knot, &ATK_Surf, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  27, "Sandshrew", TYPE_GROUND, TYPECOUNT, 126, 120, 137, nullptr,
		{ &ATK_Mud_Shot, &ATK_Scratch, &ATK_Sand_Attack, &ATK_Dig, &ATK_Rock_Tomb, &ATK_Rock_Slide, &ATK_Sand_Tomb, },
		true, true, false, { &ATK_Rock_Tomb, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  27, "Alolan Sandshrew", TYPE_ICE, TYPE_STEEL, 125, 129, 137, nullptr,
		{ &ATK_Metal_Claw, &ATK_Powder_Snow, &ATK_Blizzard, &ATK_Night_Slash, &ATK_Gyro_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  28, "Sandslash", TYPE_GROUND, TYPECOUNT, 182, 175, 181, "Sandshrew",
		{ &ATK_Mud_Shot, &ATK_Metal_Claw, &ATK_Sand_Attack, &ATK_Earthquake, &ATK_Night_Slash, &ATK_Rock_Tomb, &ATK_Bulldoze, &ATK_Scorching_Sands, },
		true, true, false, { &ATK_Night_Slash, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  28, "Alolan Sandslash", TYPE_ICE, TYPE_STEEL, 177, 195, 181, "Alolan Sandshrew",
		{ &ATK_Shadow_Claw, &ATK_Metal_Claw, &ATK_Powder_Snow, &ATK_Ice_Punch, &ATK_Blizzard, &ATK_Aerial_Ace, &ATK_Drill_Run, &ATK_Bulldoze, &ATK_Gyro_Ball, },
		true, true, false, { &ATK_Shadow_Claw, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Poison Pin
  {  29, "Nidoran♀", TYPE_POISON, TYPECOUNT, 86, 89, 146, nullptr,
		{ &ATK_Bite, &ATK_Poison_Sting, &ATK_Poison_Fang, &ATK_Sludge_Bomb, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  30, "Nidorina", TYPE_POISON, TYPECOUNT, 117, 120, 172, "Nidoran♀",
		{ &ATK_Bite, &ATK_Poison_Sting, &ATK_Dig, &ATK_Poison_Fang, &ATK_Thunderbolt, &ATK_Sludge_Bomb, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Drill
  {  31, "Nidoqueen", TYPE_POISON, TYPE_GROUND, 180, 173, 207, "Nidorina",
		{ &ATK_Bite, &ATK_Poison_Jab, &ATK_Poison_Sting, &ATK_Mud_Slap, &ATK_Earthquake, &ATK_Stone_Edge, &ATK_Poison_Fang, &ATK_Sludge_Wave, &ATK_Earth_Power, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  32, "Nidoran♂", TYPE_POISON, TYPECOUNT, 105, 76, 130, nullptr,
		{ &ATK_Peck, &ATK_Poison_Sting, &ATK_Sludge_Bomb, &ATK_Horn_Attack, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  33, "Nidorino", TYPE_POISON, TYPECOUNT, 137, 111, 156, "Nidoran♂",
		{ &ATK_Poison_Jab, &ATK_Poison_Sting, &ATK_Dig, &ATK_Ice_Beam, &ATK_Sludge_Bomb, &ATK_Horn_Attack, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  34, "Nidoking", TYPE_POISON, TYPE_GROUND, 204, 156, 191, "Nidorino",
		{ &ATK_Fury_Cutter, &ATK_Poison_Jab, &ATK_Poison_Sting, &ATK_Iron_Tail, &ATK_Double_Kick, &ATK_Megahorn, &ATK_Earthquake, &ATK_Sludge_Wave, &ATK_Sand_Tomb, &ATK_Earth_Power, },
		true, true, false, { &ATK_Fury_Cutter, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Fairy
  {  35, "Clefairy", TYPE_FAIRY, TYPECOUNT, 107, 108, 172, "Cleffa",
		{ &ATK_Pound, &ATK_Zen_Headbutt, &ATK_Disarming_Voice, &ATK_Moonblast, &ATK_Swift, &ATK_Body_Slam, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  36, "Clefable", TYPE_FAIRY, TYPECOUNT, 178, 162, 216, "Clefairy",
		{ &ATK_Pound, &ATK_Zen_Headbutt, &ATK_Charge_Beam, &ATK_Charm, &ATK_Fairy_Wind, &ATK_Draining_Kiss, &ATK_Dazzling_Gleam, &ATK_Moonblast, &ATK_Psychic, &ATK_Swift, &ATK_Meteor_Mash, },
		true, false, false, { &ATK_Pound, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Fox
  {  37, "Vulpix", TYPE_FIRE, TYPECOUNT, 96, 109, 116, nullptr,
		{ &ATK_Ember, &ATK_Quick_Attack, &ATK_Flamethrower, &ATK_Flame_Charge, &ATK_Body_Slam, &ATK_Weather_Ball_Fire, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  37, "Alolan Vulpix", TYPE_ICE, TYPECOUNT, 96, 109, 116, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Powder_Snow, &ATK_Dark_Pulse, &ATK_Ice_Beam, &ATK_Blizzard, &ATK_Weather_Ball_Ice, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  38, "Ninetales", TYPE_FIRE, TYPECOUNT, 169, 190, 177, "Vulpix",
		{ &ATK_Ember, &ATK_Feint_Attack, &ATK_Fire_Spin, &ATK_Flamethrower, &ATK_Heat_Wave, &ATK_Psyshock, &ATK_Fire_Blast, &ATK_Solar_Beam, &ATK_Overheat, &ATK_Weather_Ball_Fire, &ATK_Scorching_Sands, },
		true, true, false, { &ATK_Ember, &ATK_Fire_Blast, &ATK_Flamethrower, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  38, "Alolan Ninetales", TYPE_ICE, TYPE_FAIRY, 170, 193, 177, "Alolan Vulpix",
		{ &ATK_Feint_Attack, &ATK_Powder_Snow, &ATK_Charm, &ATK_Ice_Beam, &ATK_Blizzard, &ATK_Psyshock, &ATK_Dazzling_Gleam, &ATK_Weather_Ball_Ice, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Balloon
  {  39, "Jigglypuff", TYPE_NORMAL, TYPE_FAIRY, 80, 41, 251, nullptr,
		{ &ATK_Pound, &ATK_Feint_Attack, &ATK_Disarming_Voice, &ATK_Dazzling_Gleam, &ATK_Play_Rough, &ATK_Swift, &ATK_Body_Slam, &ATK_Gyro_Ball, },
		true, false, false, { &ATK_Body_Slam, &ATK_Play_Rough, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  40, "Wigglytuff", TYPE_NORMAL, TYPE_FAIRY, 156, 90, 295, "Jigglypuff",
		{ &ATK_Pound, &ATK_Feint_Attack, &ATK_Charm, &ATK_Hyper_Beam, &ATK_Ice_Beam, &ATK_Disarming_Voice, &ATK_Dazzling_Gleam, &ATK_Play_Rough, &ATK_Icy_Wind, &ATK_Swift, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Bat
  {  41, "Zubat", TYPE_POISON, TYPE_FLYING, 83, 73, 120, nullptr,
		{ &ATK_Bite, &ATK_Quick_Attack, &ATK_Poison_Fang, &ATK_Sludge_Bomb, &ATK_Air_Cutter, &ATK_Swift, },
		true, true, false, { &ATK_Sludge_Bomb, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  42, "Golbat", TYPE_POISON, TYPE_FLYING, 161, 150, 181, "Zubat",
		{ &ATK_Bite, &ATK_Wing_Attack, &ATK_Poison_Fang, &ATK_Ominous_Wind, &ATK_Shadow_Ball, &ATK_Air_Cutter, },
		true, true, false, { &ATK_Ominous_Wind, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Weed
  {  43, "Oddish", TYPE_GRASS, TYPE_POISON, 131, 112, 128, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Acid, &ATK_Seed_Bomb, &ATK_Moonblast, &ATK_Sludge_Bomb, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  44, "Gloom", TYPE_GRASS, TYPE_POISON, 153, 136, 155, "Oddish",
		{ &ATK_Razor_Leaf, &ATK_Acid, &ATK_Petal_Blizzard, &ATK_Moonblast, &ATK_Sludge_Bomb, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Flower
  {  45, "Vileplume", TYPE_GRASS, TYPE_POISON, 202, 167, 181, "Gloom",
		{ &ATK_Razor_Leaf, &ATK_Acid, &ATK_Petal_Blizzard, &ATK_Moonblast, &ATK_Sludge_Bomb, &ATK_Solar_Beam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Mushroom
  {  46, "Paras", TYPE_BUG, TYPE_GRASS, 121, 99, 111, nullptr,
		{ &ATK_Bug_Bite, &ATK_Scratch, &ATK_Seed_Bomb, &ATK_Cross_Poison, &ATK_X_Scissor, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  47, "Parasect", TYPE_BUG, TYPE_GRASS, 165, 146, 155, "Paras",
		{ &ATK_Fury_Cutter, &ATK_Bug_Bite, &ATK_Struggle_Bug, &ATK_Cross_Poison, &ATK_X_Scissor, &ATK_Solar_Beam, },
		true, false, false, { &ATK_Bug_Bite, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Insect
  {  48, "Venonat", TYPE_BUG, TYPE_POISON, 100, 100, 155, nullptr,
		{ &ATK_Bug_Bite, &ATK_Confusion, &ATK_Psybeam, &ATK_Poison_Fang, &ATK_Signal_Beam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  49, "Venomoth", TYPE_BUG, TYPE_POISON, 179, 143, 172, "Venonat",
		{ &ATK_Bug_Bite, &ATK_Confusion, &ATK_Infestation, &ATK_Psywave, &ATK_Bug_Buzz, &ATK_Poison_Fang, &ATK_Psychic, &ATK_Silver_Wind, },
		true, true, false, { &ATK_Bug_Bite, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Mole
  {  50, "Diglett", TYPE_GROUND, TYPECOUNT, 109, 78, 67, nullptr,
		{ &ATK_Scratch, &ATK_Mud_Shot, &ATK_Mud_Slap, &ATK_Dig, &ATK_Rock_Tomb, &ATK_Mud_Bomb, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  50, "Alolan Diglett", TYPE_GROUND, TYPE_STEEL, 108, 81, 67, nullptr,
		{ &ATK_Metal_Claw, &ATK_Mud_Slap, &ATK_Sand_Attack, &ATK_Dig, &ATK_Rock_Tomb, &ATK_Mud_Bomb, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  51, "Dugtrio", TYPE_GROUND, TYPECOUNT, 167, 134, 111, "Diglett",
		{ &ATK_Sucker_Punch, &ATK_Mud_Shot, &ATK_Mud_Slap, &ATK_Earthquake, &ATK_Stone_Edge, &ATK_Mud_Bomb, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  51, "Alolan Dugtrio", TYPE_GROUND, TYPE_STEEL, 201, 142, 111, "Alolan Diglett",
		{ &ATK_Metal_Claw, &ATK_Mud_Slap, &ATK_Sand_Attack, &ATK_Earthquake, &ATK_Iron_Head, &ATK_Mud_Bomb, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Scratch Cat
  {  52, "Meowth", TYPE_NORMAL, TYPECOUNT, 92, 78, 120, nullptr,
		{ &ATK_Bite, &ATK_Scratch, &ATK_Dark_Pulse, &ATK_Night_Slash, &ATK_Body_Slam, &ATK_Foul_Play, },
		true, true, false, { &ATK_Body_Slam, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  52, "Alolan Meowth", TYPE_DARK, TYPECOUNT, 99, 78, 120, nullptr,
		{ &ATK_Bite, &ATK_Scratch, &ATK_Dark_Pulse, &ATK_Night_Slash, &ATK_Foul_Play, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  52, "Galarian Meowth", TYPE_STEEL, TYPECOUNT, 115, 92, 137, nullptr,
		{ &ATK_Scratch, &ATK_Metal_Claw, &ATK_Metal_Sound, &ATK_Dig, &ATK_Night_Slash, &ATK_Gyro_Ball, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Classy Cat
  {  53, "Persian", TYPE_NORMAL, TYPECOUNT, 150, 136, 163, "Meowth",
		{ &ATK_Scratch, &ATK_Feint_Attack, &ATK_Night_Slash, &ATK_Power_Gem, &ATK_Play_Rough, &ATK_Foul_Play, &ATK_Payback, },
		true, true, false, { &ATK_Night_Slash, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  53, "Alolan Persian", TYPE_DARK, TYPECOUNT, 158, 136, 163, "Alolan Meowth",
		{ &ATK_Scratch, &ATK_Feint_Attack, &ATK_Dark_Pulse, &ATK_Play_Rough, &ATK_Foul_Play, &ATK_Payback, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Duck
  {  54, "Psyduck", TYPE_WATER, TYPECOUNT, 122, 95, 137, nullptr,
		{ &ATK_Water_Gun, &ATK_Zen_Headbutt, &ATK_Cross_Chop, &ATK_Psybeam, &ATK_Aqua_Tail, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  55, "Golduck", TYPE_WATER, TYPECOUNT, 191, 162, 190, "Psyduck",
		{ &ATK_Water_Gun, &ATK_Confusion, &ATK_Cross_Chop, &ATK_Ice_Beam, &ATK_Bubble_Beam, &ATK_Hydro_Pump, &ATK_Psychic, &ATK_Synchronoise, &ATK_Liquidation, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Pig Monkey
  {  56, "Mankey", TYPE_FIGHTING, TYPECOUNT, 148, 82, 120, nullptr,
		{ &ATK_Karate_Chop, &ATK_Scratch, &ATK_Cross_Chop, &ATK_Low_Sweep, &ATK_Brick_Break, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  57, "Primeape", TYPE_FIGHTING, TYPECOUNT, 207, 138, 163, "Mankey",
		{ &ATK_Low_Kick, &ATK_Karate_Chop, &ATK_Counter, &ATK_Cross_Chop, &ATK_Ice_Punch, &ATK_Night_Slash, &ATK_Low_Sweep, &ATK_Close_Combat, &ATK_Rage_Fist, },
		true, true, false, { &ATK_Karate_Chop, &ATK_Cross_Chop, &ATK_Rage_Fist, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Puppy
  {  58, "Growlithe", TYPE_FIRE, TYPECOUNT, 136, 93, 146, nullptr,
		{ &ATK_Bite, &ATK_Ember, &ATK_Flame_Wheel, &ATK_Flamethrower, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  58, "Hisuian Growlithe", TYPE_FIRE, TYPE_ROCK, 142, 92, 155, nullptr,
		{ &ATK_Bite, &ATK_Ember, &ATK_Flamethrower, &ATK_Rock_Slide, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Legendary
  {  59, "Arcanine", TYPE_FIRE, TYPECOUNT, 227, 166, 207, "Growlithe",
		{ &ATK_Bite, &ATK_Fire_Fang, &ATK_Snarl, &ATK_Thunder_Fang, &ATK_Flamethrower, &ATK_Bulldoze, &ATK_Fire_Blast, &ATK_Wild_Charge, &ATK_Crunch, &ATK_Psychic_Fangs, &ATK_Scorching_Sands, },
		true, true, false, { &ATK_Bite, &ATK_Bulldoze, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  59, "Hisuian Arcanine", TYPE_FIRE, TYPE_ROCK, 232, 165, 216, "Hisuian Growlithe",
		{ &ATK_Fire_Fang, &ATK_Rock_Smash, &ATK_Snarl, &ATK_Flamethrower, &ATK_Rock_Slide, &ATK_Wild_Charge, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Tadpole
  {  60, "Poliwag", TYPE_WATER, TYPECOUNT, 101, 82, 120, nullptr,
		{ &ATK_Mud_Shot, &ATK_Bubble, &ATK_Bubble_Beam, &ATK_Mud_Bomb, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  61, "Poliwhirl", TYPE_WATER, TYPECOUNT, 130, 123, 163, "Poliwag",
		{ &ATK_Mud_Shot, &ATK_Bubble, &ATK_Bubble_Beam, &ATK_Mud_Bomb, &ATK_Water_Pulse, &ATK_Scald, },
		true, true, false, { &ATK_Scald, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  62, "Poliwrath", TYPE_WATER, TYPE_FIGHTING, 182, 184, 207, "Poliwhirl",
		{ &ATK_Mud_Shot, &ATK_Bubble, &ATK_Rock_Smash, &ATK_Counter, &ATK_Ice_Punch, &ATK_Submission, &ATK_Scald, &ATK_Hydro_Pump, &ATK_Icy_Wind, &ATK_Dynamic_Punch, &ATK_Power_Up_Punch, },
		true, true, false, { &ATK_Counter, &ATK_Submission, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Psi
  {  63, "Abra", TYPE_PSYCHIC, TYPECOUNT, 195, 82, 93, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Charge_Beam, &ATK_Psyshock, &ATK_Shadow_Ball, &ATK_Signal_Beam, },
		true, true, true, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  64, "Kadabra", TYPE_PSYCHIC, TYPECOUNT, 232, 117, 120, "Abra",
		{ &ATK_Psycho_Cut, &ATK_Confusion, &ATK_Psybeam, &ATK_Shadow_Ball, &ATK_Dazzling_Gleam, },
		true, true, true, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  65, "Alakazam", TYPE_PSYCHIC, TYPECOUNT, 271, 167, 146, "Kadabra",
		{ &ATK_Psycho_Cut, &ATK_Confusion, &ATK_Counter, &ATK_Shadow_Ball, &ATK_Dazzling_Gleam, &ATK_Psychic, &ATK_Fire_Punch, &ATK_Focus_Blast, &ATK_Future_Sight, },
		true, true, true, { &ATK_Counter, &ATK_Dazzling_Gleam, &ATK_Psychic, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Superpower
  {  66, "Machop", TYPE_FIGHTING, TYPECOUNT, 137, 82, 172, nullptr,
		{ &ATK_Low_Kick, &ATK_Karate_Chop, &ATK_Rock_Smash, &ATK_Cross_Chop, &ATK_Low_Sweep, &ATK_Brick_Break, },
		true, true, true, { &ATK_Low_Kick, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  67, "Machoke", TYPE_FIGHTING, TYPECOUNT, 177, 125, 190, "Machop",
		{ &ATK_Low_Kick, &ATK_Karate_Chop, &ATK_Cross_Chop, &ATK_Submission, &ATK_Brick_Break, &ATK_Dynamic_Punch, },
		true, true, true, { &ATK_Cross_Chop, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  68, "Machamp", TYPE_FIGHTING, TYPECOUNT, 234, 159, 207, "Machoke",
		{ &ATK_Karate_Chop, &ATK_Bullet_Punch, &ATK_Counter, &ATK_Cross_Chop, &ATK_Stone_Edge, &ATK_Submission, &ATK_Rock_Slide, &ATK_Close_Combat, &ATK_Dynamic_Punch, &ATK_Heavy_Slam, &ATK_Payback, },
		true, true, true, { &ATK_Karate_Chop, &ATK_Payback, &ATK_Stone_Edge, &ATK_Submission, }, species::CAT_NORMAL, 50, "G-Max Chi Strike", },
  // the Flower
  {  69, "Bellsprout", TYPE_GRASS, TYPE_POISON, 139, 61, 137, nullptr,
		{ &ATK_Vine_Whip, &ATK_Acid, &ATK_Wrap, &ATK_Sludge_Bomb, &ATK_Power_Whip, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Flycatcher
  {  70, "Weepinbell", TYPE_GRASS, TYPE_POISON, 172, 92, 163, "Bellsprout",
		{ &ATK_Razor_Leaf, &ATK_Acid, &ATK_Bullet_Seed, &ATK_Seed_Bomb, &ATK_Sludge_Bomb, &ATK_Power_Whip, },
		true, true, false, { &ATK_Razor_Leaf, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  71, "Victreebel", TYPE_GRASS, TYPE_POISON, 207, 135, 190, "Weepinbell",
		{ &ATK_Razor_Leaf, &ATK_Acid, &ATK_Magical_Leaf, &ATK_Sludge_Bomb, &ATK_Solar_Beam, &ATK_Leaf_Blade, &ATK_Acid_Spray, &ATK_Leaf_Tornado, },
		true, true, false, { &ATK_Magical_Leaf, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Jellyfish
  {  72, "Tentacool", TYPE_WATER, TYPE_POISON, 97, 149, 120, nullptr,
		{ &ATK_Poison_Sting, &ATK_Bubble, &ATK_Wrap, &ATK_Bubble_Beam, &ATK_Water_Pulse, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  73, "Tentacruel", TYPE_WATER, TYPE_POISON, 166, 209, 190, "Tentacool",
		{ &ATK_Poison_Jab, &ATK_Acid, &ATK_Poison_Sting, &ATK_Blizzard, &ATK_Sludge_Wave, &ATK_Scald, &ATK_Hydro_Pump, &ATK_Acid_Spray, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Rock
  {  74, "Geodude", TYPE_ROCK, TYPE_GROUND, 132, 132, 120, nullptr,
		{ &ATK_Tackle, &ATK_Rock_Throw, &ATK_Dig, &ATK_Rock_Tomb, &ATK_Rock_Slide, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  74, "Alolan Geodude", TYPE_ROCK, TYPE_ELECTRIC, 132, 132, 120, nullptr,
		{ &ATK_Rock_Throw, &ATK_Volt_Switch, &ATK_Rock_Tomb, &ATK_Rock_Slide, &ATK_Thunderbolt, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  75, "Graveler", TYPE_ROCK, TYPE_GROUND, 164, 164, 146, "Geodude",
		{ &ATK_Mud_Shot, &ATK_Rock_Throw, &ATK_Mud_Slap, &ATK_Dig, &ATK_Stone_Edge, &ATK_Rock_Slide, &ATK_Rock_Blast, },
		true, true, false, { &ATK_Rock_Slide, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  75, "Alolan Graveler", TYPE_ROCK, TYPE_ELECTRIC, 164, 164, 146, "Alolan Geodude",
		{ &ATK_Rock_Throw, &ATK_Volt_Switch, &ATK_Stone_Edge, &ATK_Thunderbolt, &ATK_Rock_Blast, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Megaton
  {  76, "Golem", TYPE_ROCK, TYPE_GROUND, 211, 198, 190, "Graveler",
		{ &ATK_Mud_Shot, &ATK_Rock_Throw, &ATK_Mud_Slap, &ATK_Earthquake, &ATK_Stone_Edge, &ATK_Ancient_Power, &ATK_Rock_Blast, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  76, "Alolan Golem", TYPE_ROCK, TYPE_ELECTRIC, 211, 198, 190, "Alolan Graveler",
		{ &ATK_Rock_Throw, &ATK_Volt_Switch, &ATK_Rollout, &ATK_Stone_Edge, &ATK_Wild_Charge, &ATK_Rock_Blast, },
		true, true, false, { &ATK_Rollout, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Fire Horse
  {  77, "Ponyta", TYPE_FIRE, TYPECOUNT, 170, 127, 137, nullptr,
		{ &ATK_Ember, &ATK_Tackle, &ATK_Flame_Wheel, &ATK_Flame_Charge, &ATK_Fire_Blast, &ATK_Stomp, },
		true, true, false, { &ATK_Fire_Blast, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  77, "Galarian Ponyta", TYPE_PSYCHIC, TYPECOUNT, 170, 127, 137, nullptr,
		{ &ATK_Low_Kick, &ATK_Psycho_Cut, &ATK_Psybeam, &ATK_Play_Rough, &ATK_Swift, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  78, "Rapidash", TYPE_FIRE, TYPECOUNT, 207, 162, 163, "Ponyta",
		{ &ATK_Low_Kick, &ATK_Ember, &ATK_Fire_Spin, &ATK_Incinerate, &ATK_Heat_Wave, &ATK_Drill_Run, &ATK_Flame_Charge, &ATK_Fire_Blast, &ATK_Wild_Charge, &ATK_Scorching_Sands, },
		true, true, false, { &ATK_Ember, &ATK_Wild_Charge, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  78, "Galarian Rapidash", TYPE_PSYCHIC, TYPE_FAIRY, 207, 162, 163, "Galarian Ponyta",
		{ &ATK_Low_Kick, &ATK_Psycho_Cut, &ATK_Fairy_Wind, &ATK_Megahorn, &ATK_Play_Rough, &ATK_Psychic, &ATK_Body_Slam, &ATK_Wild_Charge, &ATK_High_Horsepower, },
		true, true, false, { &ATK_Wild_Charge, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Dopey
  {  79, "Slowpoke", TYPE_WATER, TYPE_PSYCHIC, 109, 98, 207, nullptr,
		{ &ATK_Water_Gun, &ATK_Confusion, &ATK_Psyshock, &ATK_Water_Pulse, &ATK_Psychic, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  79, "Galarian Slowpoke", TYPE_PSYCHIC, TYPECOUNT, 109, 98, 207, nullptr,
		{ &ATK_Confusion, &ATK_Iron_Tail, &ATK_Psyshock, &ATK_Psychic, &ATK_Surf, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Hermit Crab
  {  80, "Slowbro", TYPE_WATER, TYPE_PSYCHIC, 177, 180, 216, "Slowpoke",
		{ &ATK_Water_Gun, &ATK_Confusion, &ATK_Drain_Punch, &ATK_Ice_Beam, &ATK_Water_Pulse, &ATK_Scald, &ATK_Psychic, &ATK_Surf, },
		true, true, false, { &ATK_Surf, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  80, "Galarian Slowbro", TYPE_POISON, TYPE_PSYCHIC, 182, 156, 216, "Galarian Slowpoke",
		{ &ATK_Poison_Jab, &ATK_Confusion, &ATK_Drain_Punch, &ATK_Sludge_Bomb, &ATK_Scald, &ATK_Psychic, &ATK_Focus_Blast, &ATK_Surf, &ATK_Brutal_Swing, },
		true, true, false, { &ATK_Surf, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Magnet
  {  81, "Magnemite", TYPE_ELECTRIC, TYPE_STEEL, 165, 121, 93, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Spark, &ATK_Volt_Switch, &ATK_Metal_Sound, &ATK_Discharge, &ATK_Magnet_Bomb, &ATK_Thunderbolt, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  82, "Magneton", TYPE_ELECTRIC, TYPE_STEEL, 223, 169, 137, "Magnemite",
		{ &ATK_Thunder_Shock, &ATK_Spark, &ATK_Charge_Beam, &ATK_Volt_Switch, &ATK_Metal_Sound, &ATK_Discharge, &ATK_Flash_Cannon, &ATK_Magnet_Bomb, &ATK_Zap_Cannon, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Wild Duck
  {  83, "Farfetch'd", TYPE_NORMAL, TYPE_FLYING, 124, 115, 141, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Cut, &ATK_Air_Slash, &ATK_Aerial_Ace, &ATK_Leaf_Blade, &ATK_Air_Cutter, },
		true, true, false, { &ATK_Cut, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  83, "Galarian Farfetch'd", TYPE_FIGHTING, TYPECOUNT, 174, 114, 141, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Rock_Smash, &ATK_Leaf_Blade, &ATK_Brick_Break, &ATK_Brave_Bird, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Twin Bird
  {  84, "Doduo", TYPE_NORMAL, TYPE_FLYING, 158, 83, 111, nullptr,
		{ &ATK_Peck, &ATK_Quick_Attack, &ATK_Drill_Peck, &ATK_Aerial_Ace, &ATK_Swift, &ATK_Brave_Bird, },
		true, false, false, { &ATK_Swift, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Triple Bird
  {  85, "Dodrio", TYPE_NORMAL, TYPE_FLYING, 218, 140, 155, "Doduo",
		{ &ATK_Feint_Attack, &ATK_Steel_Wing, &ATK_Peck, &ATK_Trailblaze, &ATK_Drill_Peck, &ATK_Aerial_Ace, &ATK_Air_Cutter, &ATK_Brave_Bird, },
		true, false, false, { &ATK_Air_Cutter, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Sea Lion
  {  86, "Seel", TYPE_WATER, TYPECOUNT, 85, 121, 163, nullptr,
		{ &ATK_Lick, &ATK_Ice_Shard, &ATK_Water_Gun, &ATK_Aqua_Jet, &ATK_Aqua_Tail, &ATK_Icy_Wind, &ATK_Aurora_Beam, },
		true, false, false, { &ATK_Water_Gun, &ATK_Aqua_Jet, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  87, "Dewgong", TYPE_WATER, TYPE_ICE, 139, 177, 207, "Seel",
		{ &ATK_Ice_Shard, &ATK_Frost_Breath, &ATK_Iron_Tail, &ATK_Blizzard, &ATK_Drill_Run, &ATK_Aqua_Jet, &ATK_Water_Pulse, &ATK_Icy_Wind, &ATK_Aurora_Beam, &ATK_Liquidation, },
		true, false, false, { &ATK_Ice_Shard, &ATK_Aqua_Jet, &ATK_Icy_Wind, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Sludge
  {  88, "Grimer", TYPE_POISON, TYPECOUNT, 135, 90, 190, nullptr,
		{ &ATK_Poison_Jab, &ATK_Mud_Slap, &ATK_Sludge, &ATK_Sludge_Bomb, &ATK_Mud_Bomb, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  88, "Alolan Grimer", TYPE_POISON, TYPE_DARK, 135, 90, 190, nullptr,
		{ &ATK_Bite, &ATK_Poison_Jab, &ATK_Sludge_Bomb, &ATK_Gunk_Shot, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  89, "Muk", TYPE_POISON, TYPECOUNT, 190, 172, 233, "Grimer",
		{ &ATK_Lick, &ATK_Poison_Jab, &ATK_Infestation, &ATK_Dark_Pulse, &ATK_Thunder_Punch, &ATK_Sludge_Wave, &ATK_Gunk_Shot, &ATK_Acid_Spray, },
		true, true, false, { &ATK_Lick, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  89, "Alolan Muk", TYPE_POISON, TYPE_DARK, 190, 172, 233, "Alolan Grimer",
		{ &ATK_Bite, &ATK_Poison_Jab, &ATK_Snarl, &ATK_Dark_Pulse, &ATK_Sludge_Wave, &ATK_Gunk_Shot, &ATK_Acid_Spray, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Bivalve
  {  90, "Shellder", TYPE_WATER, TYPECOUNT, 116, 134, 102, nullptr,
		{ &ATK_Ice_Shard, &ATK_Tackle, &ATK_Bubble_Beam, &ATK_Water_Pulse, &ATK_Icy_Wind, &ATK_Razor_Shell, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  91, "Cloyster", TYPE_WATER, TYPE_ICE, 186, 256, 137, "Shellder",
		{ &ATK_Ice_Shard, &ATK_Frost_Breath, &ATK_Blizzard, &ATK_Hydro_Pump, &ATK_Icy_Wind, &ATK_Aurora_Beam, &ATK_Avalanche, &ATK_Razor_Shell, &ATK_Liquidation, },
		true, true, false, { &ATK_Blizzard, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Gas
  {  92, "Gastly", TYPE_GHOST, TYPE_POISON, 186, 67, 102, nullptr,
		{ &ATK_Sucker_Punch, &ATK_Lick, &ATK_Astonish, &ATK_Dark_Pulse, &ATK_Ominous_Wind, &ATK_Sludge_Bomb, &ATK_Night_Shade, },
		true, true, true, { &ATK_Sucker_Punch, &ATK_Ominous_Wind, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  93, "Haunter", TYPE_GHOST, TYPE_POISON, 223, 107, 128, "Gastly",
		{ &ATK_Lick, &ATK_Shadow_Claw, &ATK_Astonish, &ATK_Dark_Pulse, &ATK_Ice_Punch, &ATK_Shadow_Punch, &ATK_Shadow_Ball, &ATK_Sludge_Bomb, },
		true, true, true, { &ATK_Lick, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Shadow
  {  94, "Gengar", TYPE_GHOST, TYPE_POISON, 261, 149, 155, "Haunter",
		{ &ATK_Sucker_Punch, &ATK_Lick, &ATK_Shadow_Claw, &ATK_Hex, &ATK_Drain_Punch, &ATK_Dark_Pulse, &ATK_Shadow_Punch, &ATK_Shadow_Ball, &ATK_Sludge_Bomb, &ATK_Sludge_Wave, &ATK_Psychic, &ATK_Focus_Blast, },
		true, true, true, { &ATK_Lick, &ATK_Dark_Pulse, &ATK_Psychic, &ATK_Shadow_Punch, &ATK_Sludge_Wave, }, species::CAT_NORMAL, 50, "G-Max Terror", },
  // the Rock Snake
  {  95, "Onix", TYPE_ROCK, TYPE_GROUND, 85, 232, 111, nullptr,
		{ &ATK_Tackle, &ATK_Rock_Throw, &ATK_Stone_Edge, &ATK_Rock_Slide, &ATK_Iron_Head, &ATK_Sand_Tomb, &ATK_Heavy_Slam, &ATK_Breaking_Swipe, },
		true, true, false, { &ATK_Iron_Head, &ATK_Rock_Slide, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Hypnosis
  {  96, "Drowzee", TYPE_PSYCHIC, TYPECOUNT, 89, 136, 155, nullptr,
		{ &ATK_Pound, &ATK_Confusion, &ATK_Psybeam, &ATK_Psyshock, &ATK_Psychic, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  97, "Hypno", TYPE_PSYCHIC, TYPECOUNT, 144, 193, 198, "Drowzee",
		{ &ATK_Zen_Headbutt, &ATK_Confusion, &ATK_Ice_Punch, &ATK_Psyshock, &ATK_Shadow_Ball, &ATK_Thunder_Punch, &ATK_Psychic, &ATK_Fire_Punch, &ATK_Focus_Blast, &ATK_Future_Sight, },
		true, true, false, { &ATK_Psyshock, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the River Crab
  {  98, "Krabby", TYPE_WATER, TYPECOUNT, 181, 124, 102, nullptr,
		{ &ATK_Mud_Shot, &ATK_Bubble, &ATK_Vise_Grip, &ATK_Bubble_Beam, &ATK_Water_Pulse, &ATK_Razor_Shell, },
		true, true, true, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Pincer
  {  99, "Kingler", TYPE_WATER, TYPECOUNT, 240, 181, 146, "Krabby",
		{ &ATK_Mud_Shot, &ATK_Metal_Claw, &ATK_Bubble, &ATK_Vise_Grip, &ATK_X_Scissor, &ATK_Water_Pulse, &ATK_Crabhammer, &ATK_Razor_Shell, },
		true, true, true, { &ATK_Mud_Shot, }, species::CAT_NORMAL, 50, "G-Max Foam Burst", },
  // the Ball
  {  100, "Voltorb", TYPE_ELECTRIC, TYPECOUNT, 109, 111, 120, nullptr,
		{ &ATK_Spark, &ATK_Tackle, &ATK_Discharge, &ATK_Thunderbolt, &ATK_Signal_Beam, &ATK_Gyro_Ball, },
		true, true, false, { &ATK_Signal_Beam, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Sphere
  {  100, "Hisuian Voltorb", TYPE_ELECTRIC, TYPE_GRASS, 109, 111, 120, nullptr,
		{ &ATK_Tackle, &ATK_Charge_Beam, &ATK_Thunderbolt, &ATK_Swift, &ATK_Energy_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  101, "Electrode", TYPE_ELECTRIC, TYPECOUNT, 173, 173, 155, "Voltorb",
		{ &ATK_Spark, &ATK_Tackle, &ATK_Volt_Switch, &ATK_Hyper_Beam, &ATK_Discharge, &ATK_Thunderbolt, &ATK_Foul_Play, },
		true, true, false, { &ATK_Tackle, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  101, "Hisuian Electrode", TYPE_ELECTRIC, TYPE_GRASS, 176, 176, 155, "Hisuian Voltorb",
		{ &ATK_Thunder_Shock, &ATK_Tackle, &ATK_Swift, &ATK_Wild_Charge, &ATK_Energy_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Egg
  {  102, "Exeggcute", TYPE_GRASS, TYPE_PSYCHIC, 107, 125, 155, nullptr,
		{ &ATK_Confusion, &ATK_Bullet_Seed, &ATK_Seed_Bomb, &ATK_Ancient_Power, &ATK_Psychic, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Coconut
  {  103, "Exeggutor", TYPE_GRASS, TYPE_PSYCHIC, 233, 149, 216, "Exeggcute",
		{ &ATK_Zen_Headbutt, &ATK_Confusion, &ATK_Bullet_Seed, &ATK_Extrasensory, &ATK_Seed_Bomb, &ATK_Psychic, &ATK_Solar_Beam, },
		true, true, false, { &ATK_Zen_Headbutt, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  103, "Alolan Exeggutor", TYPE_GRASS, TYPE_DRAGON, 230, 153, 216, "Exeggcute",
		{ &ATK_Dragon_Tail, &ATK_Bullet_Seed, &ATK_Seed_Bomb, &ATK_Dragon_Pulse, &ATK_Solar_Beam, &ATK_Draco_Meteor, },
		true, true, false, { &ATK_Draco_Meteor, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Lonely
  {  104, "Cubone", TYPE_GROUND, TYPECOUNT, 90, 144, 137, nullptr,
		{ &ATK_Mud_Slap, &ATK_Rock_Smash, &ATK_Dig, &ATK_Bone_Club, &ATK_Bulldoze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Bone Keeper
  {  105, "Marowak", TYPE_GROUND, TYPECOUNT, 144, 186, 155, "Cubone",
		{ &ATK_Mud_Slap, &ATK_Rock_Smash, &ATK_Dig, &ATK_Earthquake, &ATK_Rock_Slide, &ATK_Bone_Club, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  105, "Alolan Marowak", TYPE_FIRE, TYPE_GHOST, 144, 186, 155, "Cubone",
		{ &ATK_Rock_Smash, &ATK_Hex, &ATK_Fire_Spin, &ATK_Flame_Wheel, &ATK_Shadow_Ball, &ATK_Bone_Club, &ATK_Fire_Blast, &ATK_Shadow_Bone, },
		true, true, false, { &ATK_Shadow_Bone, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Kicking
  {  106, "Hitmonlee", TYPE_FIGHTING, TYPECOUNT, 224, 181, 137, "Tyrogue",
		{ &ATK_Low_Kick, &ATK_Rock_Smash, &ATK_Double_Kick, &ATK_Stone_Edge, &ATK_Low_Sweep, &ATK_Brick_Break, &ATK_Stomp, &ATK_Close_Combat, &ATK_Blaze_Kick, },
		true, true, false, { &ATK_Brick_Break, &ATK_Stomp, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Punching
  {  107, "Hitmonchan", TYPE_FIGHTING, TYPECOUNT, 193, 197, 137, "Tyrogue",
		{ &ATK_Bullet_Punch, &ATK_Rock_Smash, &ATK_Counter, &ATK_Ice_Punch, &ATK_Thunder_Punch, &ATK_Fire_Punch, &ATK_Brick_Break, &ATK_Close_Combat, &ATK_Power_Up_Punch, },
		true, true, false, { &ATK_Rock_Smash, &ATK_Brick_Break, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Locking
  {  108, "Lickitung", TYPE_NORMAL, TYPECOUNT, 108, 137, 207, nullptr,
		{ &ATK_Lick, &ATK_Zen_Headbutt, &ATK_Wrap, &ATK_Hyper_Beam, &ATK_Power_Whip, &ATK_Stomp, &ATK_Body_Slam, },
		true, false, false, { &ATK_Body_Slam, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Poison Gas
  {  109, "Koffing", TYPE_POISON, TYPECOUNT, 119, 141, 120, nullptr,
		{ &ATK_Tackle, &ATK_Infestation, &ATK_Dark_Pulse, &ATK_Sludge, &ATK_Sludge_Bomb, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  110, "Weezing", TYPE_POISON, TYPECOUNT, 174, 197, 163, "Koffing",
		{ &ATK_Tackle, &ATK_Infestation, &ATK_Rollout, &ATK_Dark_Pulse, &ATK_Shadow_Ball, &ATK_Thunderbolt, &ATK_Sludge_Bomb, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  110, "Galarian Weezing", TYPE_POISON, TYPE_FAIRY, 174, 197, 163, nullptr,
		{ &ATK_Tackle, &ATK_Fairy_Wind, &ATK_Hyper_Beam, &ATK_Sludge, &ATK_Play_Rough, &ATK_Overheat, &ATK_Brutal_Swing, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Spikes
  {  111, "Rhyhorn", TYPE_GROUND, TYPE_ROCK, 140, 127, 190, nullptr,
		{ &ATK_Mud_Slap, &ATK_Rock_Smash, &ATK_Bulldoze, &ATK_Horn_Attack, &ATK_Stomp, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Drill
  {  112, "Rhydon", TYPE_GROUND, TYPE_ROCK, 222, 171, 233, "Rhyhorn",
		{ &ATK_Mud_Slap, &ATK_Rock_Smash, &ATK_Megahorn, &ATK_Earthquake, &ATK_Stone_Edge, &ATK_Surf, &ATK_Breaking_Swipe, },
		true, true, false, { &ATK_Megahorn, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Egg
  {  113, "Chansey", TYPE_NORMAL, TYPECOUNT, 60, 128, 487, "Happiny",
		{ &ATK_Pound, &ATK_Zen_Headbutt, &ATK_Hyper_Beam, &ATK_Dazzling_Gleam, &ATK_Psychic, },
		true, false, true, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Vine
  {  114, "Tangela", TYPE_GRASS, TYPECOUNT, 183, 169, 163, nullptr,
		{ &ATK_Vine_Whip, &ATK_Infestation, &ATK_Sludge_Bomb, &ATK_Solar_Beam, &ATK_Power_Whip, &ATK_Grass_Knot, },
		true, true, false, { &ATK_Power_Whip, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Parent
  {  115, "Kangaskhan", TYPE_NORMAL, TYPECOUNT, 181, 165, 233, nullptr,
		{ &ATK_Low_Kick, &ATK_Mud_Slap, &ATK_Earthquake, &ATK_Brick_Break, &ATK_Stomp, &ATK_Outrage, &ATK_Crunch, &ATK_Power_Up_Punch, },
		true, true, false, { &ATK_Brick_Break, &ATK_Stomp, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Dragon
  {  116, "Horsea", TYPE_WATER, TYPECOUNT, 129, 103, 102, nullptr,
		{ &ATK_Water_Gun, &ATK_Bubble, &ATK_Flash_Cannon, &ATK_Bubble_Beam, &ATK_Dragon_Pulse, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  117, "Seadra", TYPE_WATER, TYPECOUNT, 187, 156, 146, "Horsea",
		{ &ATK_Dragon_Breath, &ATK_Water_Gun, &ATK_Blizzard, &ATK_Dragon_Pulse, &ATK_Hydro_Pump, &ATK_Aurora_Beam, },
		true, true, false, { &ATK_Blizzard, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Goldfish
  {  118, "Goldeen", TYPE_WATER, TYPECOUNT, 123, 110, 128, nullptr,
		{ &ATK_Peck, &ATK_Mud_Shot, &ATK_Aqua_Tail, &ATK_Water_Pulse, &ATK_Horn_Attack, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  119, "Seaking", TYPE_WATER, TYPECOUNT, 175, 147, 190, "Goldeen",
		{ &ATK_Peck, &ATK_Poison_Jab, &ATK_Waterfall, &ATK_Megahorn, &ATK_Ice_Beam, &ATK_Drill_Run, &ATK_Water_Pulse, &ATK_Icy_Wind, },
		true, false, false, { &ATK_Poison_Jab, &ATK_Drill_Run, &ATK_Icy_Wind, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Star Shape
  {  120, "Staryu", TYPE_WATER, TYPECOUNT, 137, 112, 102, nullptr,
		{ &ATK_Tackle, &ATK_Water_Gun, &ATK_Bubble_Beam, &ATK_Power_Gem, &ATK_Swift, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Mysterious
  {  121, "Starmie", TYPE_WATER, TYPE_PSYCHIC, 210, 184, 155, "Staryu",
		{ &ATK_Tackle, &ATK_Water_Gun, &ATK_Hidden_Power, &ATK_Psywave, &ATK_Psybeam, &ATK_Ice_Beam, &ATK_Power_Gem, &ATK_Thunder, &ATK_Hydro_Pump, &ATK_Psychic, &ATK_Surf, },
		true, true, false, { &ATK_Tackle, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Mime
  {  122, "Mr. Mime", TYPE_PSYCHIC, TYPE_FAIRY, 192, 205, 120, "Mime Jr.",
		{ &ATK_Zen_Headbutt, &ATK_Confusion, &ATK_Magical_Leaf, &ATK_Psywave, &ATK_Psybeam, &ATK_Shadow_Ball, &ATK_Psychic, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  122, "Galarian Mr. Mime", TYPE_ICE, TYPE_PSYCHIC, 183, 169, 137, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Confusion, &ATK_Psybeam, &ATK_Ice_Punch, &ATK_Psychic, &ATK_Triple_Axel, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Mantis
  {  123, "Scyther", TYPE_BUG, TYPE_FLYING, 218, 170, 172, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Steel_Wing, &ATK_Air_Slash, &ATK_Aerial_Ace, &ATK_Bug_Buzz, &ATK_Night_Slash, &ATK_X_Scissor, &ATK_Trailblaze, },
		true, true, false, { &ATK_Steel_Wing, &ATK_Bug_Buzz, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Human Shape
  {  124, "Jynx", TYPE_ICE, TYPE_PSYCHIC, 223, 151, 163, "Smoochum",
		{ &ATK_Frost_Breath, &ATK_Pound, &ATK_Confusion, &ATK_Ice_Punch, &ATK_Psyshock, &ATK_Draining_Kiss, &ATK_Focus_Blast, &ATK_Avalanche, },
		true, false, false, { &ATK_Pound, &ATK_Ice_Punch, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Electric
  {  125, "Electabuzz", TYPE_ELECTRIC, TYPECOUNT, 198, 158, 163, "Elekid",
		{ &ATK_Thunder_Shock, &ATK_Low_Kick, &ATK_Thunder_Punch, &ATK_Thunder, &ATK_Thunderbolt, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Spitfire
  {  126, "Magmar", TYPE_FIRE, TYPECOUNT, 206, 154, 163, nullptr,
		{ &ATK_Karate_Chop, &ATK_Ember, &ATK_Flamethrower, &ATK_Fire_Blast, &ATK_Fire_Punch, &ATK_Scorching_Sands, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  127, "Pinsir", TYPE_BUG, TYPECOUNT, 238, 182, 163, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Bug_Bite, &ATK_Rock_Smash, &ATK_Vise_Grip, &ATK_Submission, &ATK_X_Scissor, &ATK_Close_Combat, &ATK_Superpower, },
		true, true, false, { &ATK_Submission, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  128, "Tauros", TYPE_NORMAL, TYPECOUNT, 198, 183, 181, nullptr,
		{ &ATK_Tackle, &ATK_Zen_Headbutt, &ATK_Earthquake, &ATK_Iron_Head, &ATK_Horn_Attack, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  128, "Paldean Combat Tauros", TYPE_FIGHTING, TYPECOUNT, 210, 193, 181, nullptr,
		{ &ATK_Tackle, &ATK_Zen_Headbutt, &ATK_Double_Kick, &ATK_Earthquake, &ATK_Iron_Head, &ATK_Close_Combat, &ATK_Trailblaze, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  128, "Paldean Blaze Tauros", TYPE_FIGHTING, TYPE_FIRE, 210, 193, 181, nullptr,
		{ &ATK_Tackle, &ATK_Zen_Headbutt, &ATK_Double_Kick, &ATK_Earthquake, &ATK_Iron_Head, &ATK_Flame_Charge, &ATK_Trailblaze, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  128, "Paldean Aqua Tauros", TYPE_FIGHTING, TYPE_WATER, 210, 193, 181, nullptr,
		{ &ATK_Tackle, &ATK_Zen_Headbutt, &ATK_Double_Kick, &ATK_Earthquake, &ATK_Aqua_Jet, &ATK_Iron_Head, &ATK_Horn_Attack, &ATK_Trailblaze, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  129, "Magikarp", TYPE_WATER, TYPECOUNT, 29, 85, 85, nullptr,
		{ &ATK_Splash, &ATK_Struggle, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  130, "Gyarados", TYPE_WATER, TYPE_FLYING, 237, 186, 216, "Magikarp",
		{ &ATK_Bite, &ATK_Dragon_Breath, &ATK_Dragon_Tail, &ATK_Waterfall, &ATK_Aqua_Tail, &ATK_Twister, &ATK_Dragon_Pulse, &ATK_Hydro_Pump, &ATK_Outrage, &ATK_Crunch, },
		true, true, false, { &ATK_Dragon_Tail, &ATK_Aqua_Tail, &ATK_Dragon_Pulse, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  131, "Lapras", TYPE_WATER, TYPE_ICE, 165, 174, 277, nullptr,
		{ &ATK_Ice_Shard, &ATK_Frost_Breath, &ATK_Water_Gun, &ATK_Psywave, &ATK_Ice_Beam, &ATK_Blizzard, &ATK_Dragon_Pulse, &ATK_Hydro_Pump, &ATK_Surf, &ATK_Skull_Bash, &ATK_Sparkling_Aria, },
		true, true, false, { &ATK_Ice_Shard, &ATK_Dragon_Pulse, &ATK_Ice_Beam, }, species::CAT_NORMAL, 75, "G-Max Resonance", species::EVOL_NOITEM, },
  {  132, "Ditto", TYPE_NORMAL, TYPECOUNT, 91, 91, 134, nullptr,
		{ &ATK_Transform, &ATK_Struggle, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  133, "Eevee", TYPE_NORMAL, TYPECOUNT, 104, 114, 146, nullptr,
		{ &ATK_Quick_Attack, &ATK_Tackle, &ATK_Dig, &ATK_Swift, &ATK_Body_Slam, &ATK_Last_Resort, },
		true, false, false, { &ATK_Body_Slam, &ATK_Last_Resort, }, species::CAT_NORMAL, 75, "G-Max Cuddle", species::EVOL_NOITEM, },
  {  134, "Vaporeon", TYPE_WATER, TYPECOUNT, 205, 161, 277, "Eevee",
		{ &ATK_Water_Gun, &ATK_Aqua_Tail, &ATK_Water_Pulse, &ATK_Scald, &ATK_Hydro_Pump, &ATK_Last_Resort, &ATK_Liquidation, },
		true, false, false, { &ATK_Scald, &ATK_Last_Resort, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  135, "Jolteon", TYPE_ELECTRIC, TYPECOUNT, 232, 182, 163, "Eevee",
		{ &ATK_Thunder_Shock, &ATK_Volt_Switch, &ATK_Discharge, &ATK_Thunder, &ATK_Thunderbolt, &ATK_Zap_Cannon, &ATK_Last_Resort, },
		true, false, false, { &ATK_Zap_Cannon, &ATK_Last_Resort, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  136, "Flareon", TYPE_FIRE, TYPECOUNT, 246, 179, 163, "Eevee",
		{ &ATK_Ember, &ATK_Fire_Spin, &ATK_Flamethrower, &ATK_Heat_Wave, &ATK_Fire_Blast, &ATK_Overheat, &ATK_Last_Resort, &ATK_Superpower, },
		true, false, false, { &ATK_Heat_Wave, &ATK_Superpower, &ATK_Last_Resort, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  137, "Porygon", TYPE_NORMAL, TYPECOUNT, 153, 136, 163, nullptr,
		{ &ATK_Tackle, &ATK_Zen_Headbutt, &ATK_Charge_Beam, &ATK_Hidden_Power, &ATK_Hyper_Beam, &ATK_Psybeam, &ATK_Discharge, &ATK_Signal_Beam, &ATK_Solar_Beam, &ATK_Zap_Cannon, },
		true, true, false, { &ATK_Discharge, &ATK_Psybeam, &ATK_Signal_Beam, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  138, "Omanyte", TYPE_ROCK, TYPE_WATER, 155, 153, 111, nullptr,
		{ &ATK_Mud_Shot, &ATK_Water_Gun, &ATK_Bubble_Beam, &ATK_Ancient_Power, &ATK_Rock_Tomb, &ATK_Brine, &ATK_Rock_Blast, },
		true, true, false, { &ATK_Brine, &ATK_Rock_Tomb, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  139, "Omastar", TYPE_ROCK, TYPE_WATER, 207, 201, 172, "Omanyte",
		{ &ATK_Mud_Shot, &ATK_Rock_Throw, &ATK_Water_Gun, &ATK_Ancient_Power, &ATK_Rock_Slide, &ATK_Hydro_Pump, &ATK_Rock_Blast, },
		true, true, false, { &ATK_Rock_Throw, &ATK_Rock_Slide, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  140, "Kabuto", TYPE_ROCK, TYPE_WATER, 148, 140, 102, nullptr,
		{ &ATK_Mud_Shot, &ATK_Scratch, &ATK_Aqua_Jet, &ATK_Ancient_Power, &ATK_Rock_Tomb, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  141, "Kabutops", TYPE_ROCK, TYPE_WATER, 220, 186, 155, "Kabuto",
		{ &ATK_Fury_Cutter, &ATK_Mud_Shot, &ATK_Rock_Smash, &ATK_Waterfall, &ATK_Stone_Edge, &ATK_Ancient_Power, &ATK_Water_Pulse, },
		true, true, false, { &ATK_Fury_Cutter, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  142, "Aerodactyl", TYPE_ROCK, TYPE_FLYING, 221, 159, 190, nullptr,
		{ &ATK_Bite, &ATK_Rock_Throw, &ATK_Steel_Wing, &ATK_Hyper_Beam, &ATK_Ancient_Power, &ATK_Rock_Slide, &ATK_Iron_Head, &ATK_Earth_Power, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  143, "Snorlax", TYPE_NORMAL, TYPECOUNT, 190, 169, 330, "Munchlax",
		{ &ATK_Lick, &ATK_Zen_Headbutt, &ATK_Yawn, &ATK_Hyper_Beam, &ATK_Earthquake, &ATK_Body_Slam, &ATK_Heavy_Slam, &ATK_Outrage, &ATK_Skull_Bash, &ATK_Superpower, },
		true, true, false, { &ATK_Yawn, }, species::CAT_NORMAL, 75, "G-Max Replenish", },
  {  144, "Articuno", TYPE_ICE, TYPE_FLYING, 192, 236, 207, nullptr,
		{ &ATK_Ice_Shard, &ATK_Frost_Breath, &ATK_Ice_Beam, &ATK_Blizzard, &ATK_Ancient_Power, &ATK_Icy_Wind, &ATK_Hurricane, &ATK_Triple_Axel, },
		true, true, true, { &ATK_Hurricane, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  144, "Galarian Articuno", TYPE_PSYCHIC, TYPE_FLYING, 250, 197, 207, nullptr,
		{ &ATK_Psycho_Cut, &ATK_Confusion, &ATK_Ancient_Power, &ATK_Brave_Bird, &ATK_Future_Sight, &ATK_Fly, },
		true, true, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  145, "Zapdos", TYPE_ELECTRIC, TYPE_FLYING, 253, 185, 207, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Charge_Beam, &ATK_Drill_Peck, &ATK_Ancient_Power, &ATK_Thunder, &ATK_Thunderbolt, &ATK_Zap_Cannon, },
		true, true, true, { &ATK_Thunder_Shock, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  145, "Galarian Zapdos", TYPE_FIGHTING, TYPE_FLYING, 252, 189, 207, nullptr,
		{ &ATK_Counter, &ATK_Ancient_Power, &ATK_Close_Combat, &ATK_Brave_Bird, &ATK_Fly, },
		true, true, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  146, "Moltres", TYPE_FIRE, TYPE_FLYING, 251, 181, 207, nullptr,
		{ &ATK_Wing_Attack, &ATK_Fire_Spin, &ATK_Heat_Wave, &ATK_Ancient_Power, &ATK_Fire_Blast, &ATK_Sky_Attack, &ATK_Overheat, },
		true, true, true, { &ATK_Sky_Attack, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  146, "Galarian Moltres", TYPE_DARK, TYPE_FLYING, 202, 231, 207, nullptr,
		{ &ATK_Sucker_Punch, &ATK_Wing_Attack, &ATK_Ancient_Power, &ATK_Brave_Bird, &ATK_Payback, &ATK_Fly, },
		true, true, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  147, "Dratini", TYPE_DRAGON, TYPECOUNT, 119, 91, 121, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Iron_Tail, &ATK_Wrap, &ATK_Aqua_Tail, &ATK_Twister, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  148, "Dragonair", TYPE_DRAGON, TYPECOUNT, 163, 135, 156, "Dratini",
		{ &ATK_Dragon_Breath, &ATK_Iron_Tail, &ATK_Wrap, &ATK_Aqua_Tail, &ATK_Dragon_Pulse, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  149, "Dragonite", TYPE_DRAGON, TYPE_FLYING, 263, 198, 209, "Dragonair",
		{ &ATK_Steel_Wing, &ATK_Dragon_Tail, &ATK_Dragon_Breath, &ATK_Hyper_Beam, &ATK_Superpower, &ATK_Hurricane, &ATK_Thunder_Punch, &ATK_Outrage, &ATK_Dragon_Claw, &ATK_Draco_Meteor, &ATK_Dragon_Pulse, },
		true, true, false, { &ATK_Draco_Meteor, &ATK_Dragon_Pulse, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  150, "Mewtwo", TYPE_PSYCHIC, TYPECOUNT, 300, 182, 214, nullptr,
		{ &ATK_Psycho_Cut, &ATK_Confusion, &ATK_Hyper_Beam, &ATK_Flamethrower, &ATK_Ice_Beam, &ATK_Shadow_Ball, &ATK_Thunderbolt, &ATK_Psychic, &ATK_Psystrike, &ATK_Focus_Blast, },
		true, true, false, { &ATK_Hyper_Beam, &ATK_Psystrike, &ATK_Shadow_Ball, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  150, "Armored Mewtwo", TYPE_PSYCHIC, TYPECOUNT, 182, 278, 214, nullptr,
		{ &ATK_Confusion, &ATK_Iron_Tail, &ATK_Earthquake, &ATK_Rock_Slide, &ATK_Psystrike, &ATK_Dynamic_Punch, &ATK_Future_Sight, },
		false, false, false, { &ATK_Psystrike, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  151, "Mew", TYPE_PSYCHIC, TYPECOUNT, 210, 210, 225, nullptr,
		{ &ATK_Shadow_Claw, &ATK_Frost_Breath, &ATK_Pound, &ATK_Cut, &ATK_Poison_Jab, &ATK_Steel_Wing, &ATK_Rock_Smash, &ATK_Charge_Beam, &ATK_Volt_Switch, &ATK_Dragon_Tail, &ATK_Infestation, &ATK_Struggle_Bug, &ATK_Snarl, &ATK_Hyper_Beam, &ATK_Dark_Pulse, &ATK_Stone_Edge, &ATK_Flash_Cannon, &ATK_Ice_Beam, &ATK_Blizzard, &ATK_Low_Sweep, &ATK_Psyshock, &ATK_Ancient_Power, &ATK_Rock_Slide, &ATK_Thunder, &ATK_Thunderbolt, &ATK_Dragon_Claw, &ATK_Dazzling_Gleam, &ATK_Bulldoze, &ATK_Flame_Charge, &ATK_Psychic, &ATK_Solar_Beam, &ATK_Focus_Blast, &ATK_Wild_Charge, &ATK_Gyro_Ball, &ATK_Overheat, &ATK_Grass_Knot, &ATK_Energy_Ball, &ATK_Surf, },
		true, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  152, "Chikorita", TYPE_GRASS, TYPECOUNT, 92, 122, 128, nullptr,
		{ &ATK_Vine_Whip, &ATK_Tackle, &ATK_Magical_Leaf, &ATK_Body_Slam, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		true, true, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  153, "Bayleef", TYPE_GRASS, TYPECOUNT, 122, 155, 155, "Chikorita",
		{ &ATK_Razor_Leaf, &ATK_Tackle, &ATK_Magical_Leaf, &ATK_Ancient_Power, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  154, "Meganium", TYPE_GRASS, TYPECOUNT, 168, 202, 190, "Bayleef",
		{ &ATK_Vine_Whip, &ATK_Razor_Leaf, &ATK_Magical_Leaf, &ATK_Earthquake, &ATK_Petal_Blizzard, &ATK_Solar_Beam, &ATK_Frenzy_Plant, },
		true, true, false, { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  155, "Cyndaquil", TYPE_FIRE, TYPECOUNT, 116, 93, 118, nullptr,
		{ &ATK_Ember, &ATK_Tackle, &ATK_Flamethrower, &ATK_Flame_Charge, &ATK_Swift, },
		true, true, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  156, "Quilava", TYPE_FIRE, TYPECOUNT, 158, 126, 151, "Cyndaquil",
		{ &ATK_Ember, &ATK_Tackle, &ATK_Flamethrower, &ATK_Dig, &ATK_Flame_Charge, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  157, "Typhlosion", TYPE_FIRE, TYPECOUNT, 223, 173, 186, "Quilava",
		{ &ATK_Ember, &ATK_Shadow_Claw, &ATK_Incinerate, &ATK_Thunder_Punch, &ATK_Fire_Blast, &ATK_Solar_Beam, &ATK_Overheat, &ATK_Blast_Burn, },
		true, true, false, { &ATK_Blast_Burn, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  157, "Hisuian Typhlosion", TYPE_FIRE, TYPE_GHOST, 238, 172, 177, nullptr,
		{ &ATK_Ember, &ATK_Hex, &ATK_Shadow_Ball, &ATK_Fire_Punch, &ATK_Wild_Charge, &ATK_Night_Shade, &ATK_Overheat, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  158, "Totodile", TYPE_WATER, TYPECOUNT, 117, 109, 137, nullptr,
		{ &ATK_Scratch, &ATK_Water_Gun, &ATK_Aqua_Jet, &ATK_Water_Pulse, &ATK_Crunch, },
		true, true, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  159, "Croconaw", TYPE_WATER, TYPECOUNT, 150, 142, 163, "Totodile",
		{ &ATK_Scratch, &ATK_Water_Gun, &ATK_Ice_Punch, &ATK_Water_Pulse, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  160, "Feraligatr", TYPE_WATER, TYPECOUNT, 205, 188, 198, "Croconaw",
		{ &ATK_Bite, &ATK_Shadow_Claw, &ATK_Water_Gun, &ATK_Waterfall, &ATK_Ice_Fang, &ATK_Ice_Beam, &ATK_Hydro_Pump, &ATK_Crunch, &ATK_Hydro_Cannon, },
		true, true, false, { &ATK_Water_Gun, &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  161, "Sentret", TYPE_NORMAL, TYPECOUNT, 79, 73, 111, nullptr,
		{ &ATK_Quick_Attack, &ATK_Scratch, &ATK_Dig, &ATK_Brick_Break, &ATK_Grass_Knot, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  162, "Furret", TYPE_NORMAL, TYPECOUNT, 148, 125, 198, "Sentret",
		{ &ATK_Sucker_Punch, &ATK_Quick_Attack, &ATK_Hyper_Beam, &ATK_Dig, &ATK_Brick_Break, &ATK_Swift, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  163, "Hoothoot", TYPE_NORMAL, TYPE_FLYING, 67, 88, 155, nullptr,
		{ &ATK_Peck, &ATK_Feint_Attack, &ATK_Aerial_Ace, &ATK_Sky_Attack, &ATK_Night_Shade, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  164, "Noctowl", TYPE_NORMAL, TYPE_FLYING, 145, 156, 225, "Hoothoot",
		{ &ATK_Wing_Attack, &ATK_Extrasensory, &ATK_Shadow_Ball, &ATK_Psychic, &ATK_Sky_Attack, &ATK_Night_Shade, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  165, "Ledyba", TYPE_BUG, TYPE_FLYING, 72, 118, 120, nullptr,
		{ &ATK_Bug_Bite, &ATK_Tackle, &ATK_Aerial_Ace, &ATK_Swift, &ATK_Silver_Wind, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  166, "Ledian", TYPE_BUG, TYPE_FLYING, 107, 179, 146, "Ledyba",
		{ &ATK_Bug_Bite, &ATK_Counter, &ATK_Struggle_Bug, &ATK_Aerial_Ace, &ATK_Bug_Buzz, &ATK_Dynamic_Punch, &ATK_Silver_Wind, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  167, "Spinarak", TYPE_BUG, TYPE_POISON, 105, 73, 120, nullptr,
		{ &ATK_Bug_Bite, &ATK_Poison_Sting, &ATK_Night_Slash, &ATK_Cross_Poison, &ATK_Signal_Beam, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  168, "Ariados", TYPE_BUG, TYPE_POISON, 161, 124, 172, "Spinarak",
		{ &ATK_Poison_Sting, &ATK_Infestation, &ATK_Megahorn, &ATK_Shadow_Sneak, &ATK_Cross_Poison, &ATK_Lunge, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  169, "Crobat", TYPE_POISON, TYPE_FLYING, 194, 178, 198, "Golbat",
		{ &ATK_Bite, &ATK_Air_Slash, &ATK_Poison_Fang, &ATK_Shadow_Ball, &ATK_Cross_Poison, &ATK_Sludge_Bomb, &ATK_Air_Cutter, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  170, "Chinchou", TYPE_WATER, TYPE_ELECTRIC, 106, 97, 181, nullptr,
		{ &ATK_Spark, &ATK_Bubble, &ATK_Bubble_Beam, &ATK_Thunderbolt, &ATK_Water_Pulse, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  171, "Lanturn", TYPE_WATER, TYPE_ELECTRIC, 146, 137, 268, "Chinchou",
		{ &ATK_Spark, &ATK_Water_Gun, &ATK_Charge_Beam, &ATK_Thunder, &ATK_Thunderbolt, &ATK_Hydro_Pump, &ATK_Surf, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  172, "Pichu", TYPE_ELECTRIC, TYPECOUNT, 77, 53, 85, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Thunder_Punch, &ATK_Thunderbolt, &ATK_Disarming_Voice, },
		true, false, false, { }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  {  173, "Cleffa", TYPE_FAIRY, TYPECOUNT, 75, 79, 137, nullptr,
		{ &ATK_Pound, &ATK_Zen_Headbutt, &ATK_Psyshock, &ATK_Signal_Beam, &ATK_Psychic, &ATK_Swift, &ATK_Body_Slam, &ATK_Grass_Knot, },
		true, false, false, { &ATK_Body_Slam, &ATK_Psychic, }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  {  174, "Igglybuff", TYPE_NORMAL, TYPE_FAIRY, 69, 32, 207, nullptr,
		{ &ATK_Pound, &ATK_Feint_Attack, &ATK_Shadow_Ball, &ATK_Psychic, &ATK_Swift, &ATK_Body_Slam, &ATK_Wild_Charge, },
		true, false, false, { &ATK_Body_Slam, }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  {  175, "Togepi", TYPE_FAIRY, TYPECOUNT, 67, 116, 111, nullptr,
		{ &ATK_Peck, &ATK_Zen_Headbutt, &ATK_Hidden_Power, &ATK_Psyshock, &ATK_Ancient_Power, &ATK_Dazzling_Gleam, },
		true, false, false, { &ATK_Zen_Headbutt, }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  {  176, "Togetic", TYPE_FAIRY, TYPE_FLYING, 139, 181, 146, "Togepi",
		{ &ATK_Zen_Headbutt, &ATK_Steel_Wing, &ATK_Extrasensory, &ATK_Hidden_Power, &ATK_Fairy_Wind, &ATK_Aerial_Ace, &ATK_Psyshock, &ATK_Ancient_Power, &ATK_Draining_Kiss, &ATK_Dazzling_Gleam, },
		true, false, false, { &ATK_Steel_Wing, &ATK_Zen_Headbutt, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  177, "Natu", TYPE_PSYCHIC, TYPE_FLYING, 134, 89, 120, nullptr,
		{ &ATK_Peck, &ATK_Quick_Attack, &ATK_Drill_Peck, &ATK_Psyshock, &ATK_Night_Shade, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  178, "Xatu", TYPE_PSYCHIC, TYPE_FLYING, 192, 146, 163, "Natu",
		{ &ATK_Feint_Attack, &ATK_Air_Slash, &ATK_Peck, &ATK_Psyshock, &ATK_Aerial_Ace, &ATK_Ominous_Wind, &ATK_Future_Sight, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  179, "Mareep", TYPE_ELECTRIC, TYPECOUNT, 114, 79, 146, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Tackle, &ATK_Discharge, &ATK_Thunderbolt, &ATK_Body_Slam, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  180, "Flaaffy", TYPE_ELECTRIC, TYPECOUNT, 145, 109, 172, "Mareep",
		{ &ATK_Tackle, &ATK_Charge_Beam, &ATK_Discharge, &ATK_Power_Gem, &ATK_Thunderbolt, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  181, "Ampharos", TYPE_ELECTRIC, TYPECOUNT, 211, 169, 207, "Flaaffy",
		{ &ATK_Charge_Beam, &ATK_Volt_Switch, &ATK_Power_Gem, &ATK_Thunder_Punch, &ATK_Thunder, &ATK_Dragon_Pulse, &ATK_Focus_Blast, &ATK_Zap_Cannon, &ATK_Brutal_Swing, &ATK_Trailblaze, },
		true, true, false, { &ATK_Dragon_Pulse, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  182, "Bellossom", TYPE_GRASS, TYPECOUNT, 169, 186, 181, "Gloom",
		{ &ATK_Razor_Leaf, &ATK_Acid, &ATK_Bullet_Seed, &ATK_Magical_Leaf, &ATK_Petal_Blizzard, &ATK_Dazzling_Gleam, &ATK_Leaf_Blade, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SUNSTONE, },
  {  183, "Marill", TYPE_WATER, TYPE_FAIRY, 37, 93, 172, "Azurill",
		{ &ATK_Tackle, &ATK_Bubble, &ATK_Bubble_Beam, &ATK_Aqua_Tail, &ATK_Body_Slam, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  184, "Azumarill", TYPE_WATER, TYPE_FAIRY, 112, 152, 225, "Marill",
		{ &ATK_Bubble, &ATK_Rock_Smash, &ATK_Ice_Beam, &ATK_Play_Rough, &ATK_Hydro_Pump, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Imitation
  {  185, "Sudowoodo", TYPE_ROCK, TYPECOUNT, 167, 176, 172, "Bonsly",
		{ &ATK_Rock_Throw, &ATK_Counter, &ATK_Earthquake, &ATK_Stone_Edge, &ATK_Rock_Tomb, &ATK_Rock_Slide, &ATK_Meteor_Beam, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Frog
  {  186, "Politoed", TYPE_WATER, TYPECOUNT, 174, 179, 207, "Poliwhirl",
		{ &ATK_Mud_Shot, &ATK_Bubble, &ATK_Earthquake, &ATK_Ice_Beam, &ATK_Blizzard, &ATK_Scald, &ATK_Hydro_Pump, &ATK_Surf, &ATK_Weather_Ball_Water, },
		true, true, false, { &ATK_Earthquake, &ATK_Ice_Beam, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_KINGSROCK, },
  {  187, "Hoppip", TYPE_GRASS, TYPE_FLYING, 67, 94, 111, nullptr,
		{ &ATK_Tackle, &ATK_Bullet_Seed, &ATK_Seed_Bomb, &ATK_Dazzling_Gleam, &ATK_Grass_Knot, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  188, "Skiploom", TYPE_GRASS, TYPE_FLYING, 91, 120, 146, "Hoppip",
		{ &ATK_Tackle, &ATK_Bullet_Seed, &ATK_Dazzling_Gleam, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  189, "Jumpluff", TYPE_GRASS, TYPE_FLYING, 118, 183, 181, "Skiploom",
		{ &ATK_Infestation, &ATK_Bullet_Seed, &ATK_Fairy_Wind, &ATK_Aerial_Ace, &ATK_Dazzling_Gleam, &ATK_Solar_Beam, &ATK_Energy_Ball, &ATK_Acrobatics, },
		true, true, false, { &ATK_Acrobatics, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  190, "Aipom", TYPE_NORMAL, TYPECOUNT, 136, 112, 146, nullptr,
		{ &ATK_Scratch, &ATK_Astonish, &ATK_Aerial_Ace, &ATK_Low_Sweep, &ATK_Swift, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  191, "Sunkern", TYPE_GRASS, TYPECOUNT, 55, 55, 102, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Cut, &ATK_Seed_Bomb, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  192, "Sunflora", TYPE_GRASS, TYPECOUNT, 185, 135, 181, "Sunkern",
		{ &ATK_Razor_Leaf, &ATK_Bullet_Seed, &ATK_Petal_Blizzard, &ATK_Sludge_Bomb, &ATK_Solar_Beam, &ATK_Leaf_Storm, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SUNSTONE, },
  {  193, "Yanma", TYPE_BUG, TYPE_FLYING, 154, 94, 163, nullptr,
		{ &ATK_Wing_Attack, &ATK_Quick_Attack, &ATK_Aerial_Ace, &ATK_Ancient_Power, &ATK_Silver_Wind, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  194, "Wooper", TYPE_WATER, TYPE_GROUND, 75, 66, 146, nullptr,
		{ &ATK_Mud_Shot, &ATK_Water_Gun, &ATK_Dig, &ATK_Mud_Bomb, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  194, "Paldean Wooper", TYPE_POISON, TYPE_GROUND, 75, 66, 146, nullptr,
		{ &ATK_Mud_Shot, &ATK_Poison_Jab, &ATK_Dig, &ATK_Sludge_Wave, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  195, "Quagsire", TYPE_WATER, TYPE_GROUND, 152, 143, 216, "Wooper",
		{ &ATK_Mud_Shot, &ATK_Water_Gun, &ATK_Drain_Punch, &ATK_Earthquake, &ATK_Stone_Edge, &ATK_Aqua_Tail, &ATK_Sludge_Bomb, &ATK_Mud_Bomb, &ATK_Acid_Spray, },
		true, true, false, { &ATK_Aqua_Tail, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  196, "Espeon", TYPE_PSYCHIC, TYPECOUNT, 261, 175, 163, "Eevee",
		{ &ATK_Zen_Headbutt, &ATK_Confusion, &ATK_Psybeam, &ATK_Shadow_Ball, &ATK_Psychic, &ATK_Future_Sight, &ATK_Last_Resort, &ATK_Psychic_Fangs, },
		true, false, false, { &ATK_Last_Resort, &ATK_Shadow_Ball, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  197, "Umbreon", TYPE_DARK, TYPECOUNT, 126, 240, 216, "Eevee",
		{ &ATK_Feint_Attack, &ATK_Snarl, &ATK_Dark_Pulse, &ATK_Psychic, &ATK_Foul_Play, &ATK_Last_Resort, },
		true, false, false, { &ATK_Last_Resort, &ATK_Psychic, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  198, "Murkrow", TYPE_DARK, TYPE_FLYING, 175, 87, 155, nullptr,
		{ &ATK_Peck, &ATK_Feint_Attack, &ATK_Dark_Pulse, &ATK_Drill_Peck, &ATK_Foul_Play, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Royal
  {  199, "Slowking", TYPE_WATER, TYPE_PSYCHIC, 177, 180, 216, "Slowpoke",
		{ &ATK_Water_Gun, &ATK_Confusion, &ATK_Blizzard, &ATK_Fire_Blast, &ATK_Scald, &ATK_Psychic, &ATK_Surf, },
		true, true, false, { &ATK_Surf, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_KINGSROCK, },
  {  199, "Galarian Slowking", TYPE_POISON, TYPE_PSYCHIC, 190, 180, 216, "Galarian Slowpoke",
		{ &ATK_Acid, &ATK_Confusion, &ATK_Hex, &ATK_Psyshock, &ATK_Shadow_Ball, &ATK_Sludge_Wave, &ATK_Scald, &ATK_Future_Sight, &ATK_Surf, },
		true, true, false, { &ATK_Surf, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  200, "Misdreavus", TYPE_GHOST, TYPECOUNT, 167, 154, 155, nullptr,
		{ &ATK_Astonish, &ATK_Hex, &ATK_Psywave, &ATK_Dark_Pulse, &ATK_Shadow_Sneak, &ATK_Ominous_Wind, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  201, "Unown", TYPE_PSYCHIC, TYPECOUNT, 136, 91, 134, nullptr,
		{ &ATK_Hidden_Power, &ATK_Struggle, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  202, "Wobbuffet", TYPE_PSYCHIC, TYPECOUNT, 60, 106, 382, "Wynaut",
		{ &ATK_Splash, &ATK_Counter, &ATK_Charm, &ATK_Mirror_Coat, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  203, "Girafarig", TYPE_NORMAL, TYPE_PSYCHIC, 182, 133, 172, nullptr,
		{ &ATK_Tackle, &ATK_Confusion, &ATK_Double_Kick, &ATK_Thunderbolt, &ATK_Psychic, &ATK_Mirror_Coat, &ATK_Psychic_Fangs, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  204, "Pineco", TYPE_BUG, TYPECOUNT, 108, 122, 137, nullptr,
		{ &ATK_Bug_Bite, &ATK_Tackle, &ATK_Rock_Tomb, &ATK_Sand_Tomb, &ATK_Gyro_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  205, "Forretress", TYPE_BUG, TYPE_STEEL, 161, 205, 181, "Pineco",
		{ &ATK_Bug_Bite, &ATK_Volt_Switch, &ATK_Struggle_Bug, &ATK_Earthquake, &ATK_Rock_Tomb, &ATK_Sand_Tomb, &ATK_Heavy_Slam, &ATK_Mirror_Shot, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  206, "Dunsparce", TYPE_NORMAL, TYPECOUNT, 131, 128, 225, nullptr,
		{ &ATK_Bite, &ATK_Astonish, &ATK_Rollout, &ATK_Dig, &ATK_Drill_Run, &ATK_Rock_Slide, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  207, "Gligar", TYPE_GROUND, TYPE_FLYING, 143, 184, 163, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Wing_Attack, &ATK_Dig, &ATK_Aerial_Ace, &ATK_Night_Slash, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Iron Snake
  {  208, "Steelix", TYPE_STEEL, TYPE_GROUND, 148, 272, 181, "Onix",
		{ &ATK_Dragon_Tail, &ATK_Iron_Tail, &ATK_Thunder_Fang, &ATK_Earthquake, &ATK_Heavy_Slam, &ATK_Crunch, &ATK_Psychic_Fangs, &ATK_Breaking_Swipe, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_METALCOAT, },
  {  209, "Snubbull", TYPE_FAIRY, TYPECOUNT, 137, 85, 155, nullptr,
		{ &ATK_Bite, &ATK_Tackle, &ATK_Dazzling_Gleam, &ATK_Brick_Break, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  210, "Granbull", TYPE_FAIRY, TYPECOUNT, 212, 131, 207, "Snubbull",
		{ &ATK_Bite, &ATK_Snarl, &ATK_Charm, &ATK_Play_Rough, &ATK_Close_Combat, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the balloon
  {  211, "Qwilfish", TYPE_WATER, TYPE_POISON, 184, 138, 163, nullptr,
		{ &ATK_Water_Gun, &ATK_Poison_Sting, &ATK_Ice_Beam, &ATK_Aqua_Tail, &ATK_Sludge_Wave, &ATK_Scald, &ATK_Acid_Spray, &ATK_Fell_Stinger, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  211, "Hisuian Qwilfish", TYPE_DARK, TYPE_POISON, 184, 151, 163, nullptr,
		{ &ATK_Poison_Jab, &ATK_Poison_Sting, &ATK_Dark_Pulse, &ATK_Ice_Beam, &ATK_Aqua_Tail, &ATK_Shadow_Ball, &ATK_Sludge_Bomb, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Pincer
  {  212, "Scizor", TYPE_BUG, TYPE_STEEL, 236, 181, 172, "Scyther",
		{ &ATK_Fury_Cutter, &ATK_Bullet_Punch, &ATK_Night_Slash, &ATK_Iron_Head, &ATK_X_Scissor, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_METALCOAT, },
  {  213, "Shuckle", TYPE_BUG, TYPE_ROCK, 17, 396, 85, nullptr,
		{ &ATK_Rock_Throw, &ATK_Struggle_Bug, &ATK_Stone_Edge, &ATK_Rock_Blast, &ATK_Gyro_Ball, },
		true, true, true, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  214, "Heracross", TYPE_BUG, TYPE_FIGHTING, 234, 179, 190, nullptr,
		{ &ATK_Counter, &ATK_Struggle_Bug, &ATK_Megahorn, &ATK_Earthquake, &ATK_Close_Combat, &ATK_Rock_Blast, &ATK_Upper_Hand, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  215, "Sneasel", TYPE_DARK, TYPE_ICE, 189, 146, 146, nullptr,
		{ &ATK_Ice_Shard, &ATK_Feint_Attack, &ATK_Ice_Punch, &ATK_Avalanche, &ATK_Foul_Play, &ATK_Triple_Axel, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  215, "Hisuian Sneasel", TYPE_FIGHTING, TYPE_POISON, 189, 146, 146, nullptr,
		{ &ATK_Poison_Jab, &ATK_Rock_Smash, &ATK_Aerial_Ace, &ATK_X_Scissor, &ATK_Close_Combat, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  216, "Teddiursa", TYPE_NORMAL, TYPECOUNT, 142, 93, 155, nullptr,
		{ &ATK_Lick, &ATK_Scratch, &ATK_Cross_Chop, &ATK_Play_Rough, &ATK_Swift, &ATK_Crunch, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  217, "Ursaring", TYPE_NORMAL, TYPECOUNT, 236, 144, 207, "Teddiursa",
		{ &ATK_Shadow_Claw, &ATK_Metal_Claw, &ATK_Counter, &ATK_Hyper_Beam, &ATK_Play_Rough, &ATK_Swift, &ATK_Close_Combat, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  218, "Slugma", TYPE_FIRE, TYPECOUNT, 118, 71, 120, nullptr,
		{ &ATK_Ember, &ATK_Rock_Throw, &ATK_Rock_Slide, &ATK_Flame_Charge, &ATK_Flame_Burst, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  219, "Magcargo", TYPE_FIRE, TYPE_ROCK, 139, 191, 137, "Slugma",
		{ &ATK_Ember, &ATK_Rock_Throw, &ATK_Incinerate, &ATK_Stone_Edge, &ATK_Heat_Wave, &ATK_Rock_Tomb, &ATK_Overheat, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  220, "Swinub", TYPE_ICE, TYPE_GROUND, 90, 69, 137, nullptr,
		{ &ATK_Tackle, &ATK_Powder_Snow, &ATK_Rock_Slide, &ATK_Icy_Wind, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  221, "Piloswine", TYPE_ICE, TYPE_GROUND, 181, 138, 225, "Swinub",
		{ &ATK_Ice_Shard, &ATK_Powder_Snow, &ATK_Stone_Edge, &ATK_Bulldoze, &ATK_Avalanche, &ATK_Icicle_Spear, &ATK_High_Horsepower, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  222, "Corsola", TYPE_WATER, TYPE_ROCK, 118, 156, 146, nullptr,
		{ &ATK_Tackle, &ATK_Bubble, &ATK_Bubble_Beam, &ATK_Power_Gem, &ATK_Rock_Blast, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  222, "Galarian Corsola", TYPE_GHOST, TYPECOUNT, 116, 182, 155, nullptr,
		{ &ATK_Tackle, &ATK_Astonish, &ATK_Power_Gem, &ATK_Rock_Blast, &ATK_Night_Shade, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  223, "Remoraid", TYPE_WATER, TYPECOUNT, 127, 69, 111, nullptr,
		{ &ATK_Mud_Shot, &ATK_Water_Gun, &ATK_Water_Pulse, &ATK_Aurora_Beam, &ATK_Rock_Blast, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  224, "Octillery", TYPE_WATER, TYPECOUNT, 197, 141, 181, "Remoraid",
		{ &ATK_Mud_Shot, &ATK_Water_Gun, &ATK_Lock_On, &ATK_Gunk_Shot, &ATK_Water_Pulse, &ATK_Aurora_Beam, &ATK_Acid_Spray, &ATK_Octazooka, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  225, "Delibird", TYPE_ICE, TYPE_FLYING, 128, 90, 128, nullptr,
		{ &ATK_Present, &ATK_Ice_Punch, &ATK_Aerial_Ace, &ATK_Icy_Wind, &ATK_Fly, &ATK_Triple_Axel, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  226, "Mantine", TYPE_WATER, TYPE_FLYING, 148, 226, 163, "Mantyke",
		{ &ATK_Wing_Attack, &ATK_Bubble, &ATK_Bullet_Seed, &ATK_Ice_Beam, &ATK_Aerial_Ace, &ATK_Bubble_Beam, &ATK_Water_Pulse, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  227, "Skarmory", TYPE_STEEL, TYPE_FLYING, 148, 226, 163, nullptr,
		{ &ATK_Steel_Wing, &ATK_Air_Slash, &ATK_Flash_Cannon, &ATK_Brave_Bird, &ATK_Sky_Attack, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  228, "Houndour", TYPE_DARK, TYPE_FIRE, 152, 83, 128, nullptr,
		{ &ATK_Ember, &ATK_Feint_Attack, &ATK_Dark_Pulse, &ATK_Flamethrower, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  229, "Houndoom", TYPE_DARK, TYPE_FIRE, 224, 144, 181, "Houndour",
		{ &ATK_Fire_Fang, &ATK_Snarl, &ATK_Flamethrower, &ATK_Fire_Blast, &ATK_Crunch, &ATK_Foul_Play, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  230, "Kingdra", TYPE_WATER, TYPE_DRAGON, 194, 194, 181, "Seadra",
		{ &ATK_Waterfall, &ATK_Dragon_Breath, &ATK_Water_Gun, &ATK_Swift, &ATK_Hydro_Pump, &ATK_Octazooka, &ATK_Blizzard, &ATK_Outrage, },
		true, true, false, { &ATK_Water_Gun, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_DRAGONSCALE, },
  {  231, "Phanpy", TYPE_GROUND, TYPECOUNT, 107, 98, 207, nullptr,
		{ &ATK_Tackle, &ATK_Rock_Smash, &ATK_Rock_Slide, &ATK_Bulldoze, &ATK_Body_Slam, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  232, "Donphan", TYPE_GROUND, TYPECOUNT, 214, 185, 207, "Phanpy",
		{ &ATK_Tackle, &ATK_Mud_Slap, &ATK_Counter, &ATK_Charm, &ATK_Earthquake, &ATK_Play_Rough, &ATK_Body_Slam, &ATK_Heavy_Slam, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  233, "Porygon2", TYPE_NORMAL, TYPECOUNT, 198, 180, 198, "Porygon",
		{ &ATK_Charge_Beam, &ATK_Hidden_Power, &ATK_Lock_On, &ATK_Hyper_Beam, &ATK_Solar_Beam, &ATK_Zap_Cannon, &ATK_Tri_Attack, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_UPGRADE, },
  {  234, "Stantler", TYPE_NORMAL, TYPECOUNT, 192, 131, 177, nullptr,
		{ &ATK_Tackle, &ATK_Zen_Headbutt, &ATK_Megahorn, &ATK_Stomp, &ATK_Wild_Charge, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  235, "Smeargle", TYPE_NORMAL, TYPECOUNT, 40, 83, 146, nullptr,
		{ &ATK_Splash, &ATK_Struggle, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Scuffle
  {  236, "Tyrogue", TYPE_FIGHTING, TYPECOUNT, 64, 64, 111, nullptr,
		{ &ATK_Tackle, &ATK_Rock_Smash, &ATK_Low_Sweep, &ATK_Rock_Slide, &ATK_Brick_Break, },
		true, false, false, { }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  // the Handstand
  {  237, "Hitmontop", TYPE_FIGHTING, TYPECOUNT, 173, 207, 137, "Tyrogue",
		{ &ATK_Rock_Smash, &ATK_Counter, &ATK_Stone_Edge, &ATK_Close_Combat, &ATK_Gyro_Ball, &ATK_Triple_Axel, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Kiss
  {  238, "Smoochum", TYPE_ICE, TYPE_PSYCHIC, 153, 91, 128, nullptr,
		{ &ATK_Frost_Breath, &ATK_Pound, &ATK_Powder_Snow, &ATK_Ice_Punch, &ATK_Ice_Beam, &ATK_Psyshock, },
		true, false, false, { &ATK_Frost_Breath, }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  // the Electric
  {  239, "Elekid", TYPE_ELECTRIC, TYPECOUNT, 135, 101, 128, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Low_Kick, &ATK_Discharge, &ATK_Thunder_Punch, &ATK_Thunderbolt, &ATK_Brick_Break, },
		true, true, false, { &ATK_Thunderbolt, }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  // the Live Coal
  {  240, "Magby", TYPE_FIRE, TYPECOUNT, 151, 99, 128, nullptr,
		{ &ATK_Karate_Chop, &ATK_Ember, &ATK_Flamethrower, &ATK_Flame_Burst, &ATK_Fire_Punch, &ATK_Brick_Break, },
		true, true, false, { &ATK_Flamethrower, }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  // the Milk Cow
  {  241, "Miltank", TYPE_NORMAL, TYPECOUNT, 157, 193, 216, nullptr,
		{ &ATK_Tackle, &ATK_Zen_Headbutt, &ATK_Rollout, &ATK_Ice_Beam, &ATK_Thunderbolt, &ATK_Stomp, &ATK_Body_Slam, &ATK_Gyro_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Happiness
  {  242, "Blissey", TYPE_NORMAL, TYPECOUNT, 129, 169, 496, "Chansey",
		{ &ATK_Pound, &ATK_Zen_Headbutt, &ATK_Hyper_Beam, &ATK_Dazzling_Gleam, &ATK_Psychic, &ATK_Wild_Charge, },
		true, false, true, { &ATK_Wild_Charge, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Thunder
  {  243, "Raikou", TYPE_ELECTRIC, TYPECOUNT, 241, 195, 207, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Volt_Switch, &ATK_Aura_Sphere, &ATK_Shadow_Ball, &ATK_Thunder, &ATK_Thunderbolt, &ATK_Wild_Charge, },
		true, true, true, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // the Volcano
  {  244, "Entei", TYPE_FIRE, TYPECOUNT, 235, 171, 251, nullptr,
		{ &ATK_Fire_Fang, &ATK_Fire_Spin, &ATK_Flamethrower, &ATK_Iron_Head, &ATK_Flame_Charge, &ATK_Fire_Blast, &ATK_Overheat, &ATK_Scorching_Sands, },
		true, true, true, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // the Aurora
  {  245, "Suicune", TYPE_WATER, TYPECOUNT, 180, 235, 225, nullptr,
		{ &ATK_Extrasensory, &ATK_Snarl, &ATK_Hidden_Power, &ATK_Ice_Fang, &ATK_Ice_Beam, &ATK_Bubble_Beam, &ATK_Water_Pulse, &ATK_Scald, &ATK_Hydro_Pump, },
		true, true, true, { &ATK_Hidden_Power, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // the Rock Skin
  {  246, "Larvitar", TYPE_ROCK, TYPE_GROUND, 115, 93, 137, nullptr,
		{ &ATK_Bite, &ATK_Rock_Smash, &ATK_Ancient_Power, &ATK_Stomp, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Hard Shell
  {  247, "Pupitar", TYPE_ROCK, TYPE_GROUND, 155, 133, 172, "Larvitar",
		{ &ATK_Bite, &ATK_Rock_Smash, &ATK_Dig, &ATK_Ancient_Power, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Armor
  {  248, "Tyranitar", TYPE_ROCK, TYPE_DARK, 251, 207, 225, "Pupitar",
		{ &ATK_Bite, &ATK_Iron_Tail, &ATK_Smack_Down, &ATK_Stone_Edge, &ATK_Fire_Blast, &ATK_Crunch, &ATK_Brutal_Swing, },
		true, true, false, { &ATK_Smack_Down, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Diving
  {  249, "Lugia", TYPE_PSYCHIC, TYPE_FLYING, 193, 310, 235, nullptr,
		{ &ATK_Dragon_Tail, &ATK_Extrasensory, &ATK_Hydro_Pump, &ATK_Sky_Attack, &ATK_Future_Sight, &ATK_Aeroblast, &ATK_Fly, },
		true, true, false, { &ATK_Aeroblast, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // the Rainbow
  {  250, "Ho-Oh", TYPE_FIRE, TYPE_FLYING, 239, 244, 214, nullptr,
		{ &ATK_Steel_Wing, &ATK_Extrasensory, &ATK_Hidden_Power, &ATK_Incinerate, &ATK_Earthquake, &ATK_Fire_Blast, &ATK_Solar_Beam, &ATK_Brave_Bird, &ATK_Sacred_Fire, },
		true, true, false, { &ATK_Earthquake, &ATK_Sacred_Fire, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // the Time Travel
  {  251, "Celebi", TYPE_PSYCHIC, TYPE_GRASS, 210, 210, 225, nullptr,
		{ &ATK_Charge_Beam, &ATK_Confusion, &ATK_Magical_Leaf, &ATK_Hyper_Beam, &ATK_Aura_Sphere, &ATK_Seed_Bomb, &ATK_Leaf_Storm, &ATK_Psychic, &ATK_Dazzling_Gleam, },
		true, false, false, { &ATK_Magical_Leaf, }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  // the Wood Gecko
  {  252, "Treecko", TYPE_GRASS, TYPECOUNT, 124, 94, 120, nullptr,
		{ &ATK_Pound, &ATK_Bullet_Seed, &ATK_Aerial_Ace, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		true, true, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  253, "Grovyle", TYPE_GRASS, TYPECOUNT, 172, 120, 137, "Treecko",
		{ &ATK_Quick_Attack, &ATK_Bullet_Seed, &ATK_Aerial_Ace, &ATK_Leaf_Blade, &ATK_Grass_Knot, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Forest
  {  254, "Sceptile", TYPE_GRASS, TYPECOUNT, 223, 169, 172, "Grovyle",
		{ &ATK_Fury_Cutter, &ATK_Bullet_Seed, &ATK_Earthquake, &ATK_Aerial_Ace, &ATK_Dragon_Claw, &ATK_Leaf_Blade, &ATK_Frenzy_Plant, &ATK_Breaking_Swipe, },
		true, true, false, { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Chick
  {  255, "Torchic", TYPE_FIRE, TYPECOUNT, 130, 87, 128, nullptr,
		{ &ATK_Ember, &ATK_Scratch, &ATK_Flamethrower, &ATK_Rock_Tomb, &ATK_Flame_Charge, },
		true, true, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  // the Young Fowl
  {  256, "Combusken", TYPE_FIRE, TYPE_FIGHTING, 163, 115, 155, "Torchic",
		{ &ATK_Ember, &ATK_Peck, &ATK_Flamethrower, &ATK_Rock_Slide, &ATK_Flame_Charge, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Blaze
  {  257, "Blaziken", TYPE_FIRE, TYPE_FIGHTING, 240, 141, 190, "Combusken",
		{ &ATK_Counter, &ATK_Fire_Spin, &ATK_Stone_Edge, &ATK_Focus_Blast, &ATK_Brave_Bird, &ATK_Overheat, &ATK_Blast_Burn, &ATK_Blaze_Kick, },
		true, true, false, { &ATK_Blast_Burn, &ATK_Stone_Edge, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Mud Fish
  {  258, "Mudkip", TYPE_WATER, TYPECOUNT, 126, 93, 137, nullptr,
		{ &ATK_Tackle, &ATK_Water_Gun, &ATK_Sludge, &ATK_Dig, &ATK_Stomp, },
		true, true, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  259, "Marshtomp", TYPE_WATER, TYPE_GROUND, 156, 133, 172, "Mudkip",
		{ &ATK_Mud_Shot, &ATK_Water_Gun, &ATK_Sludge, &ATK_Mud_Bomb, &ATK_Surf, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  260, "Swampert", TYPE_WATER, TYPE_GROUND, 208, 175, 225, "Marshtomp",
		{ &ATK_Mud_Shot, &ATK_Water_Gun, &ATK_Sludge, &ATK_Earthquake, &ATK_Sludge_Wave, &ATK_Surf, &ATK_Hydro_Cannon, &ATK_Muddy_Water, },
		true, true, false, { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the Bite
  {  261, "Poochyena", TYPE_DARK, TYPECOUNT, 96, 61, 111, nullptr,
		{ &ATK_Tackle, &ATK_Snarl, &ATK_Dig, &ATK_Poison_Fang, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  262, "Mightyena", TYPE_DARK, TYPECOUNT, 171, 132, 172, "Poochyena",
		{ &ATK_Bite, &ATK_Sucker_Punch, &ATK_Fire_Fang, &ATK_Thunder_Fang, &ATK_Ice_Fang, &ATK_Poison_Fang, &ATK_Play_Rough, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  263, "Zigzagoon", TYPE_NORMAL, TYPECOUNT, 58, 80, 116, nullptr,
		{ &ATK_Tackle, &ATK_Rock_Smash, &ATK_Sand_Attack, &ATK_Dig, &ATK_Thunderbolt, &ATK_Grass_Knot, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  263, "Galarian Zigzagoon", TYPE_DARK, TYPE_NORMAL, 58, 80, 116, nullptr,
		{ &ATK_Tackle, &ATK_Take_Down, &ATK_Dig, &ATK_Swift, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  264, "Linoone", TYPE_NORMAL, TYPECOUNT, 142, 128, 186, "Zigzagoon",
		{ &ATK_Shadow_Claw, &ATK_Tackle, &ATK_Sand_Attack, &ATK_Dig, &ATK_Thunder, &ATK_Swift, &ATK_Grass_Knot, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  264, "Galarian Linoone", TYPE_DARK, TYPE_NORMAL, 142, 128, 186, "Galarian Zigzagoon",
		{ &ATK_Lick, &ATK_Snarl, &ATK_Dig, &ATK_Gunk_Shot, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  265, "Wurmple", TYPE_BUG, TYPECOUNT, 75, 59, 128, nullptr,
		{ &ATK_Bug_Bite, &ATK_Tackle, &ATK_Struggle, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  266, "Silcoon", TYPE_BUG, TYPECOUNT, 60, 77, 137, "Wurmple",
		{ &ATK_Bug_Bite, &ATK_Tackle, &ATK_Struggle, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  267, "Beautifly", TYPE_BUG, TYPE_FLYING, 189, 98, 155, "Silcoon",
		{ &ATK_Infestation, &ATK_Struggle_Bug, &ATK_Bug_Buzz, &ATK_Air_Cutter, &ATK_Silver_Wind, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  268, "Cascoon", TYPE_BUG, TYPECOUNT, 60, 77, 137, "Wurmple",
		{ &ATK_Bug_Bite, &ATK_Poison_Sting, &ATK_Struggle, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  269, "Dustox", TYPE_BUG, TYPE_POISON, 98, 162, 155, "Cascoon",
		{ &ATK_Confusion, &ATK_Struggle_Bug, &ATK_Bug_Buzz, &ATK_Sludge_Bomb, &ATK_Silver_Wind, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  270, "Lotad", TYPE_WATER, TYPE_GRASS, 71, 77, 120, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Water_Gun, &ATK_Bubble_Beam, &ATK_Scald, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  271, "Lombre", TYPE_WATER, TYPE_GRASS, 112, 119, 155, "Lotad",
		{ &ATK_Razor_Leaf, &ATK_Bubble, &ATK_Ice_Beam, &ATK_Bubble_Beam, &ATK_Scald, &ATK_Grass_Knot, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  272, "Ludicolo", TYPE_WATER, TYPE_GRASS, 173, 176, 190, "Lombre",
		{ &ATK_Razor_Leaf, &ATK_Bubble, &ATK_Ice_Beam, &ATK_Blizzard, &ATK_Scald, &ATK_Hydro_Pump, &ATK_Solar_Beam, &ATK_Energy_Ball, &ATK_Leaf_Storm, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  273, "Seedot", TYPE_GRASS, TYPECOUNT, 71, 77, 120, nullptr,
		{ &ATK_Quick_Attack, &ATK_Bullet_Seed, &ATK_Grass_Knot, &ATK_Energy_Ball, &ATK_Foul_Play, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  274, "Nuzleaf", TYPE_GRASS, TYPE_DARK, 134, 78, 172, "Seedot",
		{ &ATK_Razor_Leaf, &ATK_Feint_Attack, &ATK_Leaf_Blade, &ATK_Grass_Knot, &ATK_Foul_Play, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  275, "Shiftry", TYPE_GRASS, TYPE_DARK, 200, 121, 207, "Nuzleaf",
		{ &ATK_Razor_Leaf, &ATK_Feint_Attack, &ATK_Bullet_Seed, &ATK_Snarl, &ATK_Leaf_Blade, &ATK_Hurricane, &ATK_Foul_Play, &ATK_Leaf_Tornado, },
		true, true, false, { &ATK_Bullet_Seed, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  276, "Taillow", TYPE_NORMAL, TYPE_FLYING, 106, 61, 120, nullptr,
		{ &ATK_Peck, &ATK_Quick_Attack, &ATK_Aerial_Ace, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  277, "Swellow", TYPE_NORMAL, TYPE_FLYING, 185, 124, 155, "Taillow",
		{ &ATK_Wing_Attack, &ATK_Steel_Wing, &ATK_Aerial_Ace, &ATK_Brave_Bird, &ATK_Sky_Attack, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  278, "Wingull", TYPE_WATER, TYPE_FLYING, 106, 61, 120, nullptr,
		{ &ATK_Quick_Attack, &ATK_Water_Gun, &ATK_Ice_Beam, &ATK_Water_Pulse, &ATK_Air_Cutter, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  279, "Pelipper", TYPE_WATER, TYPE_FLYING, 175, 174, 155, "Wingull",
		{ &ATK_Wing_Attack, &ATK_Water_Gun, &ATK_Blizzard, &ATK_Hydro_Pump, &ATK_Hurricane, &ATK_Weather_Ball_Water, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the feeling
  {  280, "Ralts", TYPE_PSYCHIC, TYPE_FAIRY, 79, 59, 99, nullptr,
		{ &ATK_Confusion, &ATK_Charge_Beam, &ATK_Psyshock, &ATK_Shadow_Sneak, &ATK_Disarming_Voice, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  281, "Kirlia", TYPE_PSYCHIC, TYPE_FAIRY, 117, 90, 116, "Ralts",
		{ &ATK_Confusion, &ATK_Charge_Beam, &ATK_Magical_Leaf, &ATK_Shadow_Sneak, &ATK_Disarming_Voice, &ATK_Draining_Kiss, &ATK_Psychic, &ATK_Triple_Axel, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  282, "Gardevoir", TYPE_PSYCHIC, TYPE_FAIRY, 237, 195, 169, "Kirlia",
		{ &ATK_Confusion, &ATK_Charge_Beam, &ATK_Charm, &ATK_Magical_Leaf, &ATK_Shadow_Ball, &ATK_Dazzling_Gleam, &ATK_Psychic, &ATK_Synchronoise, &ATK_Triple_Axel, },
		true, true, false, { &ATK_Synchronoise, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  283, "Surskit", TYPE_BUG, TYPE_WATER, 93, 87, 120, nullptr,
		{ &ATK_Bug_Bite, &ATK_Bubble, &ATK_Bubble_Beam, &ATK_Aqua_Jet, &ATK_Signal_Beam, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  284, "Masquerain", TYPE_BUG, TYPE_FLYING, 192, 150, 172, "Surskit",
		{ &ATK_Air_Slash, &ATK_Infestation, &ATK_Bubble_Beam, &ATK_Ominous_Wind, &ATK_Air_Cutter, &ATK_Silver_Wind, &ATK_Lunge, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  285, "Shroomish", TYPE_GRASS, TYPECOUNT, 74, 110, 155, nullptr,
		{ &ATK_Tackle, &ATK_Bullet_Seed, &ATK_Seed_Bomb, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  286, "Breloom", TYPE_GRASS, TYPE_FIGHTING, 241, 144, 155, "Shroomish",
		{ &ATK_Counter, &ATK_Bullet_Seed, &ATK_Force_Palm, &ATK_Seed_Bomb, &ATK_Sludge_Bomb, &ATK_Dynamic_Punch, &ATK_Grass_Knot, },
		true, false, false, { &ATK_Grass_Knot, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  287, "Slakoth", TYPE_NORMAL, TYPECOUNT, 104, 92, 155, nullptr,
		{ &ATK_Yawn, &ATK_Night_Slash, &ATK_Brick_Break, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  288, "Vigoroth", TYPE_NORMAL, TYPECOUNT, 159, 145, 190, "Slakoth",
		{ &ATK_Scratch, &ATK_Counter, &ATK_Rock_Slide, &ATK_Bulldoze, &ATK_Brick_Break, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  289, "Slaking", TYPE_NORMAL, TYPECOUNT, 290, 166, 284, "Vigoroth",
		{ &ATK_Yawn, &ATK_Hyper_Beam, &ATK_Earthquake, &ATK_Play_Rough, &ATK_Body_Slam, },
		true, true, false, { &ATK_Body_Slam, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  290, "Nincada", TYPE_BUG, TYPE_GROUND, 80, 126, 104, nullptr,
		{ &ATK_Bug_Bite, &ATK_Scratch, &ATK_Aerial_Ace, &ATK_Bug_Buzz, &ATK_Night_Slash, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  291, "Ninjask", TYPE_BUG, TYPE_FLYING, 199, 112, 156, "Nincada",
		{ &ATK_Fury_Cutter, &ATK_Metal_Claw, &ATK_Aerial_Ace, &ATK_Bug_Buzz, &ATK_Shadow_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  292, "Shedinja", TYPE_BUG, TYPE_GHOST, 153, 73, 1, nullptr,
		{ &ATK_Bug_Bite, &ATK_Shadow_Claw, &ATK_Struggle_Bug, &ATK_Dig, &ATK_Aerial_Ace, &ATK_Shadow_Sneak, },
		true, false, false, { &ATK_Struggle_Bug, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  293, "Whismur", TYPE_NORMAL, TYPECOUNT, 92, 42, 162, nullptr,
		{ &ATK_Pound, &ATK_Astonish, &ATK_Flamethrower, &ATK_Disarming_Voice, &ATK_Stomp, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  294, "Loudred", TYPE_NORMAL, TYPECOUNT, 134, 81, 197, "Whismur",
		{ &ATK_Bite, &ATK_Rock_Smash, &ATK_Flamethrower, &ATK_Disarming_Voice, &ATK_Stomp, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  295, "Exploud", TYPE_NORMAL, TYPECOUNT, 179, 137, 232, "Loudred",
		{ &ATK_Bite, &ATK_Astonish, &ATK_Disarming_Voice, &ATK_Fire_Blast, &ATK_Crunch, &ATK_Boomburst, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  296, "Makuhita", TYPE_FIGHTING, TYPECOUNT, 99, 54, 176, nullptr,
		{ &ATK_Tackle, &ATK_Rock_Smash, &ATK_Cross_Chop, &ATK_Low_Sweep, &ATK_Heavy_Slam, &ATK_Upper_Hand, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  297, "Hariyama", TYPE_FIGHTING, TYPECOUNT, 209, 114, 302, "Makuhita",
		{ &ATK_Bullet_Punch, &ATK_Counter, &ATK_Force_Palm, &ATK_Close_Combat, &ATK_Dynamic_Punch, &ATK_Heavy_Slam, &ATK_Superpower, &ATK_Upper_Hand, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  298, "Azurill", TYPE_NORMAL, TYPE_FAIRY, 36, 71, 137, nullptr,
		{ &ATK_Splash, &ATK_Bubble, &ATK_Ice_Beam, &ATK_Bubble_Beam, &ATK_Body_Slam, },
		true, false, false, { }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  {  299, "Nosepass", TYPE_ROCK, TYPECOUNT, 82, 215, 102, nullptr,
		{ &ATK_Spark, &ATK_Rock_Throw, &ATK_Rock_Slide, &ATK_Thunderbolt, &ATK_Rock_Blast, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  300, "Skitty", TYPE_NORMAL, TYPECOUNT, 84, 79, 137, nullptr,
		{ &ATK_Tackle, &ATK_Feint_Attack, &ATK_Dig, &ATK_Disarming_Voice, &ATK_Wild_Charge, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  301, "Delcatty", TYPE_NORMAL, TYPECOUNT, 132, 127, 172, "Skitty",
		{ &ATK_Zen_Headbutt, &ATK_Feint_Attack, &ATK_Charm, &ATK_Disarming_Voice, &ATK_Play_Rough, &ATK_Wild_Charge, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  302, "Sableye", TYPE_DARK, TYPE_GHOST, 141, 136, 137, nullptr,
		{ &ATK_Shadow_Claw, &ATK_Feint_Attack, &ATK_Drain_Punch, &ATK_Power_Gem, &ATK_Shadow_Sneak, &ATK_Dazzling_Gleam, &ATK_Foul_Play, },
		true, true, true, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  303, "Mawile", TYPE_STEEL, TYPE_FAIRY, 155, 141, 137, nullptr,
		{ &ATK_Bite, &ATK_Fire_Fang, &ATK_Astonish, &ATK_Ice_Fang, &ATK_Fairy_Wind, &ATK_Vise_Grip, &ATK_Iron_Head, &ATK_Play_Rough, &ATK_Power_Up_Punch, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  304, "Aron", TYPE_STEEL, TYPE_ROCK, 121, 141, 137, nullptr,
		{ &ATK_Tackle, &ATK_Metal_Claw, &ATK_Metal_Sound, &ATK_Rock_Tomb, &ATK_Iron_Head, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  305, "Lairon", TYPE_STEEL, TYPE_ROCK, 158, 198, 155, "Aron",
		{ &ATK_Metal_Claw, &ATK_Iron_Tail, &ATK_Metal_Sound, &ATK_Rock_Tomb, &ATK_Rock_Slide, &ATK_Body_Slam, &ATK_Heavy_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  306, "Aggron", TYPE_STEEL, TYPE_ROCK, 198, 257, 172, "Lairon",
		{ &ATK_Dragon_Tail, &ATK_Iron_Tail, &ATK_Smack_Down, &ATK_Stone_Edge, &ATK_Rock_Tomb, &ATK_Thunder, &ATK_Heavy_Slam, &ATK_Meteor_Beam, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  307, "Meditite", TYPE_FIGHTING, TYPE_PSYCHIC, 78, 107, 102, nullptr,
		{ &ATK_Confusion, &ATK_Rock_Smash, &ATK_Ice_Punch, &ATK_Low_Sweep, &ATK_Psyshock, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  308, "Medicham", TYPE_FIGHTING, TYPE_PSYCHIC, 121, 152, 155, "Meditite",
		{ &ATK_Psycho_Cut, &ATK_Counter, &ATK_Ice_Punch, &ATK_Psychic, &ATK_Dynamic_Punch, &ATK_Power_Up_Punch, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  309, "Electrike", TYPE_ELECTRIC, TYPECOUNT, 123, 78, 120, nullptr,
		{ &ATK_Spark, &ATK_Quick_Attack, &ATK_Discharge, &ATK_Thunderbolt, &ATK_Swift, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  310, "Manectric", TYPE_ELECTRIC, TYPECOUNT, 215, 127, 172, "Electrike",
		{ &ATK_Charge_Beam, &ATK_Snarl, &ATK_Thunder_Fang, &ATK_Thunder, &ATK_Flame_Burst, &ATK_Wild_Charge, &ATK_Overheat, &ATK_Psychic_Fangs, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  311, "Plusle", TYPE_ELECTRIC, TYPECOUNT, 167, 129, 155, nullptr,
		{ &ATK_Spark, &ATK_Quick_Attack, &ATK_Discharge, &ATK_Thunderbolt, &ATK_Swift, &ATK_Grass_Knot, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  312, "Minun", TYPE_ELECTRIC, TYPECOUNT, 147, 150, 155, nullptr,
		{ &ATK_Spark, &ATK_Quick_Attack, &ATK_Discharge, &ATK_Thunderbolt, &ATK_Swift, &ATK_Grass_Knot, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  313, "Volbeat", TYPE_BUG, TYPECOUNT, 143, 166, 163, nullptr,
		{ &ATK_Tackle, &ATK_Struggle_Bug, &ATK_Bug_Buzz, &ATK_Thunderbolt, &ATK_Signal_Beam, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  314, "Illumise", TYPE_BUG, TYPECOUNT, 143, 166, 163, nullptr,
		{ &ATK_Tackle, &ATK_Struggle_Bug, &ATK_Bug_Buzz, &ATK_Dazzling_Gleam, &ATK_Silver_Wind, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  315, "Roselia", TYPE_GRASS, TYPE_POISON, 186, 131, 137, "Budew",
		{ &ATK_Razor_Leaf, &ATK_Poison_Jab, &ATK_Magical_Leaf, &ATK_Petal_Blizzard, &ATK_Dazzling_Gleam, &ATK_Sludge_Bomb, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  316, "Gulpin", TYPE_POISON, TYPECOUNT, 80, 99, 172, nullptr,
		{ &ATK_Pound, &ATK_Rock_Smash, &ATK_Sludge, &ATK_Ice_Beam, &ATK_Gunk_Shot, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  317, "Swalot", TYPE_POISON, TYPECOUNT, 140, 159, 225, "Gulpin",
		{ &ATK_Mud_Shot, &ATK_Rock_Smash, &ATK_Infestation, &ATK_Sludge, &ATK_Ice_Beam, &ATK_Sludge_Bomb, &ATK_Gunk_Shot, &ATK_Acid_Spray, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  318, "Carvanha", TYPE_WATER, TYPE_DARK, 171, 39, 128, nullptr,
		{ &ATK_Bite, &ATK_Snarl, &ATK_Poison_Fang, &ATK_Aqua_Jet, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  319, "Sharpedo", TYPE_WATER, TYPE_DARK, 243, 83, 172, "Carvanha",
		{ &ATK_Bite, &ATK_Waterfall, &ATK_Poison_Fang, &ATK_Hydro_Pump, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  320, "Wailmer", TYPE_WATER, TYPECOUNT, 136, 68, 277, nullptr,
		{ &ATK_Water_Gun, &ATK_Splash, &ATK_Rollout, &ATK_Water_Pulse, &ATK_Scald, &ATK_Body_Slam, &ATK_Heavy_Slam, },
		true, true, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  321, "Wailord", TYPE_WATER, TYPECOUNT, 175, 87, 347, "Wailmer",
		{ &ATK_Water_Gun, &ATK_Zen_Headbutt, &ATK_Rollout, &ATK_Hyper_Beam, &ATK_Blizzard, &ATK_Scald, &ATK_Surf, },
		true, true, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  322, "Numel", TYPE_FIRE, TYPE_GROUND, 119, 79, 155, nullptr,
		{ &ATK_Ember, &ATK_Tackle, &ATK_Heat_Wave, &ATK_Bulldoze, &ATK_Stomp, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  323, "Camerupt", TYPE_FIRE, TYPE_GROUND, 194, 136, 172, "Numel",
		{ &ATK_Ember, &ATK_Rock_Smash, &ATK_Incinerate, &ATK_Earthquake, &ATK_Solar_Beam, &ATK_Overheat, &ATK_Earth_Power, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  324, "Torkoal", TYPE_FIRE, TYPECOUNT, 151, 203, 172, nullptr,
		{ &ATK_Ember, &ATK_Fire_Spin, &ATK_Flame_Wheel, &ATK_Earthquake, &ATK_Solar_Beam, &ATK_Overheat, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  325, "Spoink", TYPE_PSYCHIC, TYPECOUNT, 125, 122, 155, nullptr,
		{ &ATK_Splash, &ATK_Zen_Headbutt, &ATK_Psybeam, &ATK_Shadow_Ball, &ATK_Mirror_Coat, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  326, "Grumpig", TYPE_PSYCHIC, TYPECOUNT, 171, 188, 190, "Spoink",
		{ &ATK_Charge_Beam, &ATK_Extrasensory, &ATK_Psywave, &ATK_Psychic, &ATK_Shadow_Ball, &ATK_Dynamic_Punch, &ATK_Mirror_Coat, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  327, "Spinda", TYPE_NORMAL, TYPECOUNT, 116, 116, 155, nullptr,
		{ &ATK_Sucker_Punch, &ATK_Psycho_Cut, &ATK_Dig, &ATK_Rock_Tomb, &ATK_Icy_Wind, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  328, "Trapinch", TYPE_GROUND, TYPECOUNT, 162, 78, 128, nullptr,
		{ &ATK_Mud_Shot, &ATK_Struggle_Bug, &ATK_Sand_Attack, &ATK_Dig, &ATK_Sand_Tomb, &ATK_Crunch, &ATK_Scorching_Sands, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  329, "Vibrava", TYPE_GROUND, TYPE_DRAGON, 134, 99, 137, "Trapinch",
		{ &ATK_Dragon_Breath, &ATK_Mud_Shot, &ATK_Sand_Attack, &ATK_Bug_Buzz, &ATK_Bulldoze, &ATK_Sand_Tomb, &ATK_Scorching_Sands, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  330, "Flygon", TYPE_GROUND, TYPE_DRAGON, 205, 168, 190, "Vibrava",
		{ &ATK_Mud_Shot, &ATK_Sand_Attack, &ATK_Dragon_Tail, &ATK_Boomburst, &ATK_Earthquake, &ATK_Scorching_Sands, &ATK_Sand_Tomb, &ATK_Stone_Edge, &ATK_Dragon_Claw, &ATK_Earth_Power, },
		true, true, false, { &ATK_Earth_Power, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  331, "Cacnea", TYPE_GRASS, TYPECOUNT, 156, 74, 137, nullptr,
		{ &ATK_Sucker_Punch, &ATK_Poison_Sting, &ATK_Sand_Attack, &ATK_Seed_Bomb, &ATK_Brick_Break, &ATK_Grass_Knot, &ATK_Payback, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  332, "Cacturne", TYPE_GRASS, TYPE_DARK, 221, 115, 172, "Cacnea",
		{ &ATK_Sucker_Punch, &ATK_Poison_Jab, &ATK_Sand_Attack, &ATK_Dark_Pulse, &ATK_Dynamic_Punch, &ATK_Grass_Knot, &ATK_Payback, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  333, "Swablu", TYPE_NORMAL, TYPE_FLYING, 76, 132, 128, nullptr,
		{ &ATK_Peck, &ATK_Astonish, &ATK_Ice_Beam, &ATK_Aerial_Ace, &ATK_Disarming_Voice, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  334, "Altaria", TYPE_DRAGON, TYPE_FLYING, 141, 201, 181, "Swablu",
		{ &ATK_Dragon_Breath, &ATK_Peck, &ATK_Flamethrower, &ATK_Dragon_Pulse, &ATK_Dazzling_Gleam, &ATK_Moonblast, &ATK_Sky_Attack, },
		true, false, false, { &ATK_Moonblast, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  335, "Zangoose", TYPE_NORMAL, TYPECOUNT, 222, 124, 177, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Shadow_Claw, &ATK_Dig, &ATK_Night_Slash, &ATK_Close_Combat, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  336, "Seviper", TYPE_POISON, TYPECOUNT, 196, 118, 177, nullptr,
		{ &ATK_Poison_Jab, &ATK_Iron_Tail, &ATK_Wrap, &ATK_Poison_Fang, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  337, "Lunatone", TYPE_ROCK, TYPE_PSYCHIC, 178, 153, 207, nullptr,
		{ &ATK_Rock_Throw, &ATK_Confusion, &ATK_Psywave, &ATK_Rock_Slide, &ATK_Moonblast, &ATK_Psychic, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  338, "Solrock", TYPE_ROCK, TYPE_PSYCHIC, 178, 153, 207, nullptr,
		{ &ATK_Rock_Throw, &ATK_Confusion, &ATK_Psywave, &ATK_Rock_Slide, &ATK_Psychic, &ATK_Solar_Beam, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  339, "Barboach", TYPE_WATER, TYPE_GROUND, 93, 82, 137, nullptr,
		{ &ATK_Mud_Shot, &ATK_Water_Gun, &ATK_Ice_Beam, &ATK_Aqua_Tail, &ATK_Mud_Bomb, &ATK_Scald, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  340, "Whiscash", TYPE_WATER, TYPE_GROUND, 151, 141, 242, "Barboach",
		{ &ATK_Mud_Shot, &ATK_Water_Gun, &ATK_Blizzard, &ATK_Mud_Bomb, &ATK_Water_Pulse, &ATK_Scald, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  341, "Corphish", TYPE_WATER, TYPECOUNT, 141, 99, 125, nullptr,
		{ &ATK_Bubble, &ATK_Rock_Smash, &ATK_Vise_Grip, &ATK_Bubble_Beam, &ATK_Aqua_Jet, &ATK_Scald, &ATK_Razor_Shell, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  342, "Crawdaunt", TYPE_WATER, TYPE_DARK, 224, 142, 160, "Corphish",
		{ &ATK_Snarl, &ATK_Waterfall, &ATK_Vise_Grip, &ATK_Night_Slash, &ATK_Bubble_Beam, &ATK_Scald, &ATK_Crabhammer, &ATK_Razor_Shell, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  343, "Baltoy", TYPE_GROUND, TYPE_PSYCHIC, 77, 124, 120, nullptr,
		{ &ATK_Confusion, &ATK_Extrasensory, &ATK_Dig, &ATK_Psybeam, &ATK_Gyro_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  344, "Claydol", TYPE_GROUND, TYPE_PSYCHIC, 140, 229, 155, "Baltoy",
		{ &ATK_Mud_Slap, &ATK_Confusion, &ATK_Extrasensory, &ATK_Earthquake, &ATK_Ice_Beam, &ATK_Rock_Tomb, &ATK_Shadow_Ball, &ATK_Psychic, &ATK_Gyro_Ball, &ATK_Earth_Power, &ATK_Scorching_Sands, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  345, "Lileep", TYPE_ROCK, TYPE_GRASS, 105, 150, 165, nullptr,
		{ &ATK_Acid, &ATK_Infestation, &ATK_Bullet_Seed, &ATK_Ancient_Power, &ATK_Grass_Knot, &ATK_Mirror_Coat, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  346, "Cradily", TYPE_ROCK, TYPE_GRASS, 152, 194, 200, "Lileep",
		{ &ATK_Acid, &ATK_Infestation, &ATK_Bullet_Seed, &ATK_Stone_Edge, &ATK_Rock_Tomb, &ATK_Rock_Slide, &ATK_Bulldoze, &ATK_Grass_Knot, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  347, "Anorith", TYPE_ROCK, TYPE_BUG, 176, 100, 128, nullptr,
		{ &ATK_Scratch, &ATK_Struggle_Bug, &ATK_Aqua_Jet, &ATK_Ancient_Power, &ATK_Cross_Poison, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  348, "Armaldo", TYPE_ROCK, TYPE_BUG, 222, 174, 181, "Anorith",
		{ &ATK_Fury_Cutter, &ATK_Struggle_Bug, &ATK_Cross_Poison, &ATK_Water_Pulse, &ATK_Rock_Blast, &ATK_Liquidation, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  349, "Feebas", TYPE_WATER, TYPECOUNT, 29, 85, 85, nullptr,
		{ &ATK_Tackle, &ATK_Splash, &ATK_Mirror_Coat, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  350, "Milotic", TYPE_WATER, TYPECOUNT, 192, 219, 216, "Feebas",
		{ &ATK_Dragon_Tail, &ATK_Waterfall, &ATK_Wrap, &ATK_Hyper_Beam, &ATK_Blizzard, &ATK_Surf, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  351, "Castform", TYPE_NORMAL, TYPECOUNT, 139, 139, 172, nullptr,
		{ &ATK_Tackle, &ATK_Hex, &ATK_Hurricane, &ATK_Energy_Ball, &ATK_Weather_Ball_Rock, &ATK_Weather_Ball_Normal, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  351, "Castform (Sunny)", TYPE_FIRE, TYPECOUNT, 139, 139, 172, nullptr,
		{ &ATK_Tackle, &ATK_Ember, &ATK_Fire_Blast, &ATK_Solar_Beam, &ATK_Weather_Ball_Fire, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  351, "Castform (Rainy)", TYPE_WATER, TYPECOUNT, 139, 139, 172, nullptr,
		{ &ATK_Tackle, &ATK_Water_Gun, &ATK_Thunder, &ATK_Hydro_Pump, &ATK_Weather_Ball_Water, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  351, "Castform (Snowy)", TYPE_ICE, TYPECOUNT, 139, 139, 172, nullptr,
		{ &ATK_Tackle, &ATK_Powder_Snow, &ATK_Ice_Beam, &ATK_Blizzard, &ATK_Weather_Ball_Ice, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  352, "Kecleon", TYPE_NORMAL, TYPECOUNT, 161, 189, 155, nullptr,
		{ &ATK_Sucker_Punch, &ATK_Lick, &ATK_Flamethrower, &ATK_Ice_Beam, &ATK_Aerial_Ace, &ATK_Shadow_Sneak, &ATK_Thunder, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  353, "Shuppet", TYPE_GHOST, TYPECOUNT, 138, 65, 127, nullptr,
		{ &ATK_Feint_Attack, &ATK_Astonish, &ATK_Shadow_Sneak, &ATK_Ominous_Wind, &ATK_Night_Shade, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  354, "Banette", TYPE_GHOST, TYPECOUNT, 218, 126, 162, "Shuppet",
		{ &ATK_Shadow_Claw, &ATK_Hex, &ATK_Shadow_Ball, &ATK_Thunder, &ATK_Dazzling_Gleam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  355, "Duskull", TYPE_GHOST, TYPECOUNT, 70, 162, 85, nullptr,
		{ &ATK_Astonish, &ATK_Hex, &ATK_Shadow_Sneak, &ATK_Ominous_Wind, &ATK_Night_Shade, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  356, "Dusclops", TYPE_GHOST, TYPECOUNT, 124, 234, 120, "Duskull",
		{ &ATK_Feint_Attack, &ATK_Hex, &ATK_Ice_Punch, &ATK_Shadow_Punch, &ATK_Fire_Punch, &ATK_Poltergeist, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  357, "Tropius", TYPE_GRASS, TYPE_FLYING, 136, 163, 223, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Air_Slash, &ATK_Aerial_Ace, &ATK_Leaf_Blade, &ATK_Stomp, &ATK_Brutal_Swing, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  358, "Chimecho", TYPE_PSYCHIC, TYPECOUNT, 175, 170, 181, "Chingling",
		{ &ATK_Astonish, &ATK_Extrasensory, &ATK_Psyshock, &ATK_Shadow_Ball, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  359, "Absol", TYPE_DARK, TYPECOUNT, 246, 120, 163, nullptr,
		{ &ATK_Psycho_Cut, &ATK_Snarl, &ATK_Dark_Pulse, &ATK_Megahorn, &ATK_Thunder, &ATK_Payback, &ATK_Brutal_Swing, },
		true, true, false, { &ATK_Brutal_Swing, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  360, "Wynaut", TYPE_PSYCHIC, TYPECOUNT, 41, 86, 216, nullptr,
		{ &ATK_Splash, &ATK_Counter, &ATK_Mirror_Coat, },
		true, false, false, { }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  {  361, "Snorunt", TYPE_ICE, TYPECOUNT, 95, 95, 137, nullptr,
		{ &ATK_Powder_Snow, &ATK_Hex, &ATK_Shadow_Ball, &ATK_Icy_Wind, &ATK_Avalanche, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  362, "Glalie", TYPE_ICE, TYPECOUNT, 162, 162, 190, "Snorunt",
		{ &ATK_Ice_Shard, &ATK_Frost_Breath, &ATK_Rollout, &ATK_Shadow_Ball, &ATK_Avalanche, &ATK_Gyro_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the clap
  {  363, "Spheal", TYPE_ICE, TYPE_WATER, 95, 90, 172, nullptr,
		{ &ATK_Water_Gun, &ATK_Rock_Smash, &ATK_Water_Pulse, &ATK_Body_Slam, &ATK_Aurora_Beam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the ball roll
  {  364, "Sealeo", TYPE_ICE, TYPE_WATER, 137, 132, 207, "Spheal",
		{ &ATK_Water_Gun, &ATK_Powder_Snow, &ATK_Water_Pulse, &ATK_Body_Slam, &ATK_Aurora_Beam, &ATK_Surf, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the ice break
  {  365, "Walrein", TYPE_ICE, TYPE_WATER, 182, 176, 242, "Sealeo",
		{ &ATK_Frost_Breath, &ATK_Powder_Snow, &ATK_Waterfall, &ATK_Earthquake, &ATK_Blizzard, &ATK_Water_Pulse, &ATK_Icicle_Spear, },
		true, true, false, { &ATK_Powder_Snow, &ATK_Icicle_Spear, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the bivalve
  {  366, "Clamperl", TYPE_WATER, TYPECOUNT, 133, 135, 111, nullptr,
		{ &ATK_Water_Gun, &ATK_Ice_Beam, &ATK_Water_Pulse, &ATK_Body_Slam, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the deep sea
  {  367, "Huntail", TYPE_WATER, TYPECOUNT, 197, 179, 146, "Clamperl",
		{ &ATK_Bite, &ATK_Water_Gun, &ATK_Ice_Beam, &ATK_Aqua_Tail, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the south sea
  {  368, "Gorebyss", TYPE_WATER, TYPECOUNT, 211, 179, 146, "Clamperl",
		{ &ATK_Water_Gun, &ATK_Confusion, &ATK_Draining_Kiss, &ATK_Water_Pulse, &ATK_Psychic, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the longevity
  {  369, "Relicanth", TYPE_WATER, TYPE_ROCK, 162, 203, 225, nullptr,
		{ &ATK_Water_Gun, &ATK_Zen_Headbutt, &ATK_Aqua_Tail, &ATK_Ancient_Power, &ATK_Hydro_Pump, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the rendezvous
  {  370, "Luvdisc", TYPE_WATER, TYPECOUNT, 81, 128, 125, nullptr,
		{ &ATK_Water_Gun, &ATK_Splash, &ATK_Aqua_Jet, &ATK_Draining_Kiss, &ATK_Water_Pulse, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the rock head
  {  371, "Bagon", TYPE_DRAGON, TYPECOUNT, 134, 93, 128, nullptr,
		{ &ATK_Bite, &ATK_Ember, &ATK_Flamethrower, &ATK_Twister, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the endurance
  {  372, "Shelgon", TYPE_DRAGON, TYPECOUNT, 172, 155, 163, "Bagon",
		{ &ATK_Dragon_Breath, &ATK_Ember, &ATK_Crunch, &ATK_Flamethrower, &ATK_Twister, &ATK_Dragon_Pulse, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the dragon
  {  373, "Salamence", TYPE_DRAGON, TYPE_FLYING, 277, 168, 216, "Shelgon",
		{ &ATK_Bite, &ATK_Fire_Fang, &ATK_Dragon_Tail, &ATK_Crunch, &ATK_Fire_Blast, &ATK_Brutal_Swing, &ATK_Hydro_Pump, &ATK_Outrage, &ATK_Draco_Meteor, &ATK_Fly, },
		true, true, false, { &ATK_Outrage, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the iron ball
  {  374, "Beldum", TYPE_STEEL, TYPE_PSYCHIC, 96, 132, 120, nullptr,
		{ &ATK_Take_Down, &ATK_Iron_Head, },
		true, true, true, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the iron claw
  {  375, "Metang", TYPE_STEEL, TYPE_PSYCHIC, 138, 176, 155, "Beldum",
		{ &ATK_Fury_Cutter, &ATK_Metal_Claw, &ATK_Zen_Headbutt, &ATK_Psyshock, &ATK_Psychic, &ATK_Gyro_Ball, },
		true, true, true, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the iron leg
  {  376, "Metagross", TYPE_STEEL, TYPE_PSYCHIC, 257, 228, 190, "Metang",
		{ &ATK_Fury_Cutter, &ATK_Bullet_Punch, &ATK_Zen_Headbutt, &ATK_Shadow_Claw, &ATK_Earthquake, &ATK_Flash_Cannon, &ATK_Psychic, &ATK_Meteor_Mash, },
		true, true, true, { &ATK_Shadow_Claw, &ATK_Meteor_Mash, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the rock peak
  {  377, "Regirock", TYPE_ROCK, TYPECOUNT, 179, 309, 190, nullptr,
		{ &ATK_Rock_Throw, &ATK_Rock_Smash, &ATK_Lock_On, &ATK_Earthquake, &ATK_Stone_Edge, &ATK_Focus_Blast, &ATK_Zap_Cannon, },
		true, true, false, { &ATK_Earthquake, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // the iceberg
  {  378, "Regice", TYPE_ICE, TYPECOUNT, 179, 309, 190, nullptr,
		{ &ATK_Frost_Breath, &ATK_Rock_Smash, &ATK_Lock_On, &ATK_Earthquake, &ATK_Blizzard, &ATK_Thunder, &ATK_Focus_Blast, },
		true, true, false, { &ATK_Thunder, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // the iron
  {  379, "Registeel", TYPE_STEEL, TYPECOUNT, 143, 285, 190, nullptr,
		{ &ATK_Metal_Claw, &ATK_Rock_Smash, &ATK_Lock_On, &ATK_Hyper_Beam, &ATK_Flash_Cannon, &ATK_Focus_Blast, &ATK_Zap_Cannon, },
		true, true, false, { &ATK_Zap_Cannon, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // the eon
  {  380, "Latias", TYPE_DRAGON, TYPE_PSYCHIC, 228, 246, 190, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Zen_Headbutt, &ATK_Charm, &ATK_Aura_Sphere, &ATK_Thunder, &ATK_Psychic, &ATK_Outrage, &ATK_Mist_Ball, },
		true, true, false, { &ATK_Mist_Ball, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // the eon
  {  381, "Latios", TYPE_DRAGON, TYPE_PSYCHIC, 268, 212, 190, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Zen_Headbutt, &ATK_Dragon_Claw, &ATK_Aura_Sphere, &ATK_Psychic, &ATK_Solar_Beam, &ATK_Luster_Purge, },
		true, true, false, { &ATK_Luster_Purge, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // the sea basin
  {  382, "Kyogre", TYPE_WATER, TYPECOUNT, 270, 228, 205, nullptr,
		{ &ATK_Waterfall, &ATK_Blizzard, &ATK_Thunder, &ATK_Hydro_Pump, &ATK_Surf, &ATK_Origin_Pulse, },
		true, true, false, { &ATK_Origin_Pulse, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // the continent
  {  383, "Groudon", TYPE_GROUND, TYPECOUNT, 270, 228, 205, nullptr,
		{ &ATK_Mud_Shot, &ATK_Dragon_Tail, &ATK_Earthquake, &ATK_Fire_Blast, &ATK_Fire_Punch, &ATK_Solar_Beam, &ATK_Precipice_Blades, },
		true, true, false, { &ATK_Fire_Punch, &ATK_Precipice_Blades, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  384, "Rayquaza", TYPE_DRAGON, TYPE_FLYING, 284, 170, 213, nullptr,
		{ &ATK_Dragon_Tail, &ATK_Air_Slash, &ATK_Aerial_Ace, &ATK_Ancient_Power, &ATK_Hurricane, &ATK_Outrage, &ATK_Breaking_Swipe, &ATK_Dragon_Ascent, },
		true, false, false, { &ATK_Breaking_Swipe, &ATK_Dragon_Ascent, &ATK_Hurricane, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  385, "Jirachi", TYPE_STEEL, TYPE_PSYCHIC, 210, 210, 225, nullptr,
		{ &ATK_Confusion, &ATK_Charge_Beam, &ATK_Aura_Sphere, &ATK_Dazzling_Gleam, &ATK_Psychic, &ATK_Doom_Desire, },
		true, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  386, "Deoxys", TYPE_PSYCHIC, TYPECOUNT, 345, 115, 137, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Charge_Beam, &ATK_Hyper_Beam, &ATK_Thunderbolt, &ATK_Psycho_Boost, },
		true, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  386, "Deoxys Attack", TYPE_PSYCHIC, TYPECOUNT, 414, 46, 137, nullptr,
		{ &ATK_Poison_Jab, &ATK_Zen_Headbutt, &ATK_Dark_Pulse, &ATK_Zap_Cannon, &ATK_Psycho_Boost, },
		true, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  386, "Deoxys Defense", TYPE_PSYCHIC, TYPECOUNT, 144, 330, 137, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Counter, &ATK_Rock_Slide, &ATK_Thunderbolt, &ATK_Psycho_Boost, },
		true, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  386, "Deoxys Speed", TYPE_PSYCHIC, TYPECOUNT, 230, 218, 137, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Charge_Beam, &ATK_Thunderbolt, &ATK_Swift, &ATK_Psycho_Boost, },
		true, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  387, "Turtwig", TYPE_GRASS, TYPECOUNT, 119, 110, 146, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Tackle, &ATK_Seed_Bomb, &ATK_Body_Slam, &ATK_Energy_Ball, },
		true, true, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  388, "Grotle", TYPE_GRASS, TYPECOUNT, 157, 143, 181, "Turtwig",
		{ &ATK_Bite, &ATK_Razor_Leaf, &ATK_Solar_Beam, &ATK_Body_Slam, &ATK_Energy_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  389, "Torterra", TYPE_GRASS, TYPE_GROUND, 202, 188, 216, "Grotle",
		{ &ATK_Mud_Slap, &ATK_Razor_Leaf, &ATK_Bite, &ATK_Earthquake, &ATK_Sand_Tomb, &ATK_Stone_Edge, &ATK_Solar_Beam, &ATK_Frenzy_Plant, },
		true, true, false, { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  390, "Chimchar", TYPE_FIRE, TYPECOUNT, 113, 86, 127, nullptr,
		{ &ATK_Ember, &ATK_Scratch, &ATK_Flame_Wheel, &ATK_Flamethrower, &ATK_Flame_Charge, },
		true, true, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  391, "Monferno", TYPE_FIRE, TYPE_FIGHTING, 158, 105, 162, "Chimchar",
		{ &ATK_Ember, &ATK_Rock_Smash, &ATK_Flame_Wheel, &ATK_Flamethrower, &ATK_Low_Sweep, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  392, "Infernape", TYPE_FIRE, TYPE_FIGHTING, 222, 151, 183, "Monferno",
		{ &ATK_Rock_Smash, &ATK_Fire_Spin, &ATK_Flamethrower, &ATK_Solar_Beam, &ATK_Close_Combat, &ATK_Blast_Burn, },
		true, true, false, { &ATK_Blast_Burn, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  393, "Piplup", TYPE_WATER, TYPECOUNT, 112, 102, 142, nullptr,
		{ &ATK_Pound, &ATK_Bubble, &ATK_Drill_Peck, &ATK_Bubble_Beam, &ATK_Icy_Wind, },
		true, true, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  394, "Prinplup", TYPE_WATER, TYPECOUNT, 150, 139, 162, "Piplup",
		{ &ATK_Metal_Claw, &ATK_Bubble, &ATK_Bubble_Beam, &ATK_Hydro_Pump, &ATK_Icy_Wind, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  395, "Empoleon", TYPE_WATER, TYPE_STEEL, 210, 186, 197, "Prinplup",
		{ &ATK_Metal_Claw, &ATK_Steel_Wing, &ATK_Waterfall, &ATK_Metal_Sound, &ATK_Flash_Cannon, &ATK_Drill_Peck, &ATK_Blizzard, &ATK_Hydro_Pump, &ATK_Hydro_Cannon, },
		true, true, false, { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  396, "Starly", TYPE_NORMAL, TYPE_FLYING, 101, 58, 120, nullptr,
		{ &ATK_Quick_Attack, &ATK_Tackle, &ATK_Sand_Attack, &ATK_Aerial_Ace, &ATK_Brave_Bird, &ATK_Fly, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  397, "Staravia", TYPE_NORMAL, TYPE_FLYING, 142, 94, 146, "Starly",
		{ &ATK_Wing_Attack, &ATK_Quick_Attack, &ATK_Sand_Attack, &ATK_Heat_Wave, &ATK_Aerial_Ace, &ATK_Brave_Bird, &ATK_Fly, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  398, "Staraptor", TYPE_NORMAL, TYPE_FLYING, 234, 140, 198, "Staravia",
		{ &ATK_Wing_Attack, &ATK_Quick_Attack, &ATK_Gust, &ATK_Sand_Attack, &ATK_Heat_Wave, &ATK_Close_Combat, &ATK_Brave_Bird, &ATK_Fly, },
		true, true, false, { &ATK_Gust, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  399, "Bidoof", TYPE_NORMAL, TYPECOUNT, 80, 73, 153, nullptr,
		{ &ATK_Tackle, &ATK_Take_Down, &ATK_Ice_Beam, &ATK_Shadow_Ball, &ATK_Thunderbolt, &ATK_Hyper_Fang, &ATK_Grass_Knot, &ATK_Crunch, &ATK_Superpower, },
		true, true, false, { &ATK_Ice_Beam, &ATK_Shadow_Ball, &ATK_Superpower, &ATK_Thunderbolt, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  400, "Bibarel", TYPE_NORMAL, TYPE_WATER, 162, 119, 188, "Bidoof",
		{ &ATK_Water_Gun, &ATK_Take_Down, &ATK_Rollout, &ATK_Hyper_Beam, &ATK_Hyper_Fang, &ATK_Surf, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  401, "Kricketot", TYPE_BUG, TYPECOUNT, 45, 74, 114, nullptr,
		{ &ATK_Bug_Bite, &ATK_Struggle_Bug, &ATK_Struggle, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  402, "Kricketune", TYPE_BUG, TYPECOUNT, 160, 100, 184, "Kricketot",
		{ &ATK_Fury_Cutter, &ATK_Struggle_Bug, &ATK_Aerial_Ace, &ATK_Bug_Buzz, &ATK_X_Scissor, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  403, "Shinx", TYPE_ELECTRIC, TYPECOUNT, 117, 64, 128, nullptr,
		{ &ATK_Spark, &ATK_Tackle, &ATK_Discharge, &ATK_Thunderbolt, &ATK_Swift, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  404, "Luxio", TYPE_ELECTRIC, TYPECOUNT, 159, 95, 155, "Shinx",
		{ &ATK_Bite, &ATK_Spark, &ATK_Thunderbolt, &ATK_Wild_Charge, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  405, "Luxray", TYPE_ELECTRIC, TYPECOUNT, 232, 156, 190, "Luxio",
		{ &ATK_Spark, &ATK_Snarl, &ATK_Hidden_Power, &ATK_Hyper_Beam, &ATK_Wild_Charge, &ATK_Crunch, &ATK_Psychic_Fangs, },
		true, true, false, { &ATK_Psychic_Fangs, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  406, "Budew", TYPE_GRASS, TYPE_POISON, 91, 109, 120, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Hidden_Power, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  {  407, "Roserade", TYPE_GRASS, TYPE_POISON, 243, 185, 155, "Roselia",
		{ &ATK_Razor_Leaf, &ATK_Poison_Jab, &ATK_Poison_Sting, &ATK_Bullet_Seed, &ATK_Magical_Leaf, &ATK_Dazzling_Gleam, &ATK_Sludge_Bomb, &ATK_Solar_Beam, &ATK_Grass_Knot, &ATK_Weather_Ball_Fire, &ATK_Leaf_Storm, },
		true, false, false, { &ATK_Bullet_Seed, &ATK_Weather_Ball_Fire, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  {  408, "Cranidos", TYPE_ROCK, TYPECOUNT, 218, 71, 167, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Take_Down, &ATK_Ancient_Power, &ATK_Rock_Tomb, &ATK_Bulldoze, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  409, "Rampardos", TYPE_ROCK, TYPECOUNT, 295, 109, 219, "Cranidos",
		{ &ATK_Zen_Headbutt, &ATK_Smack_Down, &ATK_Flamethrower, &ATK_Rock_Slide, &ATK_Outrage, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  410, "Shieldon", TYPE_ROCK, TYPE_STEEL, 76, 195, 102, nullptr,
		{ &ATK_Tackle, &ATK_Iron_Tail, &ATK_Ancient_Power, &ATK_Rock_Tomb, &ATK_Heavy_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  411, "Bastiodon", TYPE_ROCK, TYPE_STEEL, 94, 286, 155, "Shieldon",
		{ &ATK_Iron_Tail, &ATK_Smack_Down, &ATK_Flamethrower, &ATK_Stone_Edge, &ATK_Flash_Cannon, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  412, "Burmy", TYPE_BUG, TYPECOUNT, 53, 83, 120, nullptr,
		{ &ATK_Bug_Bite, &ATK_Tackle, &ATK_Struggle, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  413, "Plant Wormadam", TYPE_BUG, TYPE_GRASS, 141, 180, 155, "Burmy",
		{ &ATK_Bug_Bite, &ATK_Confusion, &ATK_Psybeam, &ATK_Bug_Buzz, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  413, "Sandy Wormadam", TYPE_BUG, TYPE_GROUND, 141, 180, 155, "Burmy",
		{ &ATK_Bug_Bite, &ATK_Confusion, &ATK_Psybeam, &ATK_Bug_Buzz, &ATK_Bulldoze, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  413, "Trash Wormadam", TYPE_BUG, TYPE_STEEL, 127, 175, 155, "Burmy",
		{ &ATK_Bug_Bite, &ATK_Confusion, &ATK_Metal_Sound, &ATK_Psybeam, &ATK_Bug_Buzz, &ATK_Iron_Head, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  414, "Mothim", TYPE_BUG, TYPE_FLYING, 185, 98, 172, "Burmy",
		{ &ATK_Bug_Bite, &ATK_Air_Slash, &ATK_Psybeam, &ATK_Aerial_Ace, &ATK_Bug_Buzz, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  415, "Combee", TYPE_BUG, TYPE_FLYING, 59, 83, 102, nullptr,
		{ &ATK_Bug_Bite, &ATK_Bug_Buzz, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  416, "Vespiquen", TYPE_BUG, TYPE_FLYING, 149, 190, 172, "Combee",
		{ &ATK_Fury_Cutter, &ATK_Bug_Bite, &ATK_Poison_Sting, &ATK_Air_Slash, &ATK_Bug_Buzz, &ATK_Power_Gem, &ATK_Signal_Beam, &ATK_X_Scissor, &ATK_Fell_Stinger, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  417, "Pachirisu", TYPE_ELECTRIC, TYPECOUNT, 94, 172, 155, nullptr,
		{ &ATK_Spark, &ATK_Volt_Switch, &ATK_Thunder_Punch, &ATK_Thunder, &ATK_Thunderbolt, &ATK_Hyper_Fang, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  418, "Buizel", TYPE_WATER, TYPECOUNT, 132, 67, 146, nullptr,
		{ &ATK_Quick_Attack, &ATK_Water_Gun, &ATK_Aqua_Jet, &ATK_Water_Pulse, &ATK_Swift, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  419, "Floatzel", TYPE_WATER, TYPECOUNT, 221, 114, 198, "Buizel",
		{ &ATK_Water_Gun, &ATK_Waterfall, &ATK_Aqua_Jet, &ATK_Hydro_Pump, &ATK_Swift, &ATK_Liquidation, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  420, "Cherubi", TYPE_GRASS, TYPECOUNT, 108, 92, 128, nullptr,
		{ &ATK_Tackle, &ATK_Bullet_Seed, &ATK_Petal_Blizzard, &ATK_Seed_Bomb, &ATK_Dazzling_Gleam, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  421, "Cherrim (Sunny Form)", TYPE_GRASS, TYPECOUNT, 170, 153, 172, "Cherubi",
		{ &ATK_Razor_Leaf, &ATK_Bullet_Seed, &ATK_Hyper_Beam, &ATK_Dazzling_Gleam, &ATK_Solar_Beam, &ATK_Weather_Ball_Fire, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  421, "Cherrim (Overcast Form)", TYPE_GRASS, TYPECOUNT, 170, 153, 172, "Cherubi",
		{ &ATK_Razor_Leaf, &ATK_Bullet_Seed, &ATK_Hyper_Beam, &ATK_Dazzling_Gleam, &ATK_Solar_Beam, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  422, "Shellos", TYPE_WATER, TYPECOUNT, 103, 105, 183, nullptr,
		{ &ATK_Mud_Slap, &ATK_Hidden_Power, &ATK_Mud_Bomb, &ATK_Water_Pulse, &ATK_Body_Slam, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  423, "Gastrodon", TYPE_WATER, TYPE_GROUND, 169, 143, 244, "Shellos",
		{ &ATK_Mud_Slap, &ATK_Hidden_Power, &ATK_Earthquake, &ATK_Water_Pulse, &ATK_Body_Slam, &ATK_Earth_Power, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  424, "Ambipom", TYPE_NORMAL, TYPECOUNT, 205, 143, 181, "Aipom",
		{ &ATK_Scratch, &ATK_Astonish, &ATK_Hyper_Beam, &ATK_Aerial_Ace, &ATK_Low_Sweep, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  {  425, "Drifloon", TYPE_GHOST, TYPE_FLYING, 117, 80, 207, nullptr,
		{ &ATK_Astonish, &ATK_Hex, &ATK_Ominous_Wind, &ATK_Shadow_Ball, &ATK_Icy_Wind, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  426, "Drifblim", TYPE_GHOST, TYPE_FLYING, 180, 102, 312, "Drifloon",
		{ &ATK_Astonish, &ATK_Hex, &ATK_Ominous_Wind, &ATK_Shadow_Ball, &ATK_Icy_Wind, &ATK_Mystical_Fire, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  427, "Buneary", TYPE_NORMAL, TYPECOUNT, 130, 105, 146, nullptr,
		{ &ATK_Quick_Attack, &ATK_Pound, &ATK_Fire_Punch, &ATK_Swift, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  428, "Lopunny", TYPE_NORMAL, TYPECOUNT, 156, 194, 163, "Buneary",
		{ &ATK_Low_Kick, &ATK_Pound, &ATK_Double_Kick, &ATK_Hyper_Beam, &ATK_Fire_Punch, &ATK_Focus_Blast, &ATK_Triple_Axel, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  429, "Mismagius", TYPE_GHOST, TYPECOUNT, 211, 187, 155, "Misdreavus",
		{ &ATK_Sucker_Punch, &ATK_Hex, &ATK_Magical_Leaf, &ATK_Psywave, &ATK_Dark_Pulse, &ATK_Shadow_Ball, &ATK_Dazzling_Gleam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  {  430, "Honchkrow", TYPE_DARK, TYPE_FLYING, 243, 103, 225, "Murkrow",
		{ &ATK_Peck, &ATK_Snarl, &ATK_Dark_Pulse, &ATK_Psychic, &ATK_Brave_Bird, &ATK_Sky_Attack, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  {  431, "Glameow", TYPE_NORMAL, TYPECOUNT, 109, 82, 135, nullptr,
		{ &ATK_Quick_Attack, &ATK_Scratch, &ATK_Aerial_Ace, &ATK_Thunderbolt, &ATK_Play_Rough, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  432, "Purugly", TYPE_NORMAL, TYPECOUNT, 172, 133, 174, "Glameow",
		{ &ATK_Shadow_Claw, &ATK_Scratch, &ATK_Aerial_Ace, &ATK_Thunder, &ATK_Play_Rough, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  433, "Chingling", TYPE_PSYCHIC, TYPECOUNT, 114, 94, 128, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Astonish, &ATK_Wrap, &ATK_Psyshock, &ATK_Shadow_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  434, "Stunky", TYPE_POISON, TYPE_DARK, 121, 90, 160, nullptr,
		{ &ATK_Bite, &ATK_Scratch, &ATK_Flamethrower, &ATK_Sludge_Bomb, &ATK_Crunch, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  435, "Skuntank", TYPE_POISON, TYPE_DARK, 184, 132, 230, "Stunky",
		{ &ATK_Bite, &ATK_Poison_Jab, &ATK_Flamethrower, &ATK_Sludge_Bomb, &ATK_Crunch, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  436, "Bronzor", TYPE_STEEL, TYPE_PSYCHIC, 43, 154, 149, nullptr,
		{ &ATK_Tackle, &ATK_Confusion, &ATK_Psyshock, &ATK_Gyro_Ball, &ATK_Heavy_Slam, &ATK_Payback, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  437, "Bronzong", TYPE_STEEL, TYPE_PSYCHIC, 161, 213, 167, "Bronzor",
		{ &ATK_Confusion, &ATK_Feint_Attack, &ATK_Metal_Sound, &ATK_Flash_Cannon, &ATK_Psyshock, &ATK_Bulldoze, &ATK_Psychic, &ATK_Heavy_Slam, &ATK_Payback, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  438, "Bonsly", TYPE_ROCK, TYPECOUNT, 124, 133, 137, nullptr,
		{ &ATK_Rock_Throw, &ATK_Counter, &ATK_Earthquake, &ATK_Rock_Tomb, &ATK_Rock_Slide, },
		true, false, false, { }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  {  439, "Mime Jr.", TYPE_PSYCHIC, TYPE_FAIRY, 125, 142, 85, nullptr,
		{ &ATK_Pound, &ATK_Confusion, &ATK_Psybeam, &ATK_Psyshock, &ATK_Psychic, },
		true, false, false, { }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  // the Playhouse
  {  440, "Happiny", TYPE_NORMAL, TYPECOUNT, 25, 77, 225, nullptr,
		{ &ATK_Pound, &ATK_Zen_Headbutt, &ATK_Psychic, },
		true, false, false, { }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  {  441, "Chatot", TYPE_NORMAL, TYPE_FLYING, 183, 91, 183, nullptr,
		{ &ATK_Peck, &ATK_Steel_Wing, &ATK_Heat_Wave, &ATK_Sky_Attack, &ATK_Night_Shade, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  442, "Spiritomb", TYPE_GHOST, TYPE_DARK, 169, 199, 137, nullptr,
		{ &ATK_Sucker_Punch, &ATK_Feint_Attack, &ATK_Rock_Tomb, &ATK_Shadow_Sneak, &ATK_Ominous_Wind, &ATK_Shadow_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  443, "Gible", TYPE_DRAGON, TYPE_GROUND, 124, 84, 151, nullptr,
		{ &ATK_Mud_Shot, &ATK_Take_Down, &ATK_Dig, &ATK_Twister, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  444, "Gabite", TYPE_DRAGON, TYPE_GROUND, 172, 125, 169, "Gible",
		{ &ATK_Mud_Shot, &ATK_Take_Down, &ATK_Flamethrower, &ATK_Dig, &ATK_Twister, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  445, "Garchomp", TYPE_DRAGON, TYPE_GROUND, 261, 193, 239, "Gabite",
		{ &ATK_Mud_Shot, &ATK_Dragon_Tail, &ATK_Earthquake, &ATK_Breaking_Swipe, &ATK_Fire_Blast, &ATK_Sand_Tomb, &ATK_Outrage, &ATK_Earth_Power, },
		true, true, false, { &ATK_Earth_Power, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  446, "Munchlax", TYPE_NORMAL, TYPECOUNT, 137, 117, 286, nullptr,
		{ &ATK_Lick, &ATK_Tackle, &ATK_Gunk_Shot, &ATK_Bulldoze, &ATK_Heavy_Slam, },
		true, false, false, { }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  // the emanation
  {  447, "Riolu", TYPE_FIGHTING, TYPECOUNT, 127, 78, 120, nullptr,
		{ &ATK_Quick_Attack, &ATK_Counter, &ATK_Cross_Chop, &ATK_Low_Sweep, &ATK_Thunder_Punch, &ATK_Brick_Break, &ATK_Blaze_Kick, },
		true, false, false, { }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  // the aura
  {  448, "Lucario", TYPE_FIGHTING, TYPE_STEEL, 236, 144, 172, "Riolu",
		{ &ATK_Bullet_Punch, &ATK_Counter, &ATK_Force_Palm, &ATK_Flash_Cannon, &ATK_Shadow_Ball, &ATK_Thunder_Punch, &ATK_Close_Combat, &ATK_Blaze_Kick, &ATK_Power_Up_Punch, &ATK_Aura_Sphere, },
		true, false, false, { &ATK_Force_Palm, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  449, "Hippopotas", TYPE_GROUND, TYPECOUNT, 124, 118, 169, nullptr,
		{ &ATK_Bite, &ATK_Tackle, &ATK_Sand_Attack, &ATK_Dig, &ATK_Rock_Tomb, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  450, "Hippowdon", TYPE_GROUND, TYPECOUNT, 201, 191, 239, "Hippopotas",
		{ &ATK_Bite, &ATK_Fire_Fang, &ATK_Thunder_Fang, &ATK_Ice_Fang, &ATK_Sand_Attack, &ATK_Earthquake, &ATK_Stone_Edge, &ATK_Body_Slam, &ATK_Weather_Ball_Rock, &ATK_Earth_Power, &ATK_Scorching_Sands, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  451, "Skorupi", TYPE_POISON, TYPE_BUG, 93, 151, 120, nullptr,
		{ &ATK_Poison_Sting, &ATK_Infestation, &ATK_Aqua_Tail, &ATK_Cross_Poison, &ATK_Sludge_Bomb, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  452, "Drapion", TYPE_POISON, TYPE_DARK, 180, 202, 172, "Skorupi",
		{ &ATK_Bite, &ATK_Poison_Sting, &ATK_Infestation, &ATK_Ice_Fang, &ATK_Aqua_Tail, &ATK_Sludge_Bomb, &ATK_Crunch, &ATK_Fell_Stinger, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  453, "Croagunk", TYPE_POISON, TYPE_FIGHTING, 116, 76, 134, nullptr,
		{ &ATK_Mud_Shot, &ATK_Poison_Jab, &ATK_Poison_Sting, &ATK_Low_Sweep, &ATK_Sludge_Bomb, &ATK_Brick_Break, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  454, "Toxicroak", TYPE_POISON, TYPE_FIGHTING, 211, 133, 195, "Croagunk",
		{ &ATK_Mud_Shot, &ATK_Poison_Jab, &ATK_Poison_Sting, &ATK_Counter, &ATK_Drain_Punch, &ATK_Shadow_Ball, &ATK_Sludge_Bomb, &ATK_Mud_Bomb, &ATK_Dynamic_Punch, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  455, "Carnivine", TYPE_GRASS, TYPECOUNT, 187, 136, 179, nullptr,
		{ &ATK_Bite, &ATK_Vine_Whip, &ATK_Power_Whip, &ATK_Energy_Ball, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  456, "Finneon", TYPE_WATER, TYPECOUNT, 96, 116, 135, nullptr,
		{ &ATK_Pound, &ATK_Water_Gun, &ATK_Ice_Beam, &ATK_Water_Pulse, &ATK_Silver_Wind, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  457, "Lumineon", TYPE_WATER, TYPECOUNT, 142, 170, 170, "Finneon",
		{ &ATK_Water_Gun, &ATK_Waterfall, &ATK_Blizzard, &ATK_Water_Pulse, &ATK_Silver_Wind, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  458, "Mantyke", TYPE_WATER, TYPE_FLYING, 105, 179, 128, nullptr,
		{ &ATK_Tackle, &ATK_Bubble, &ATK_Ice_Beam, &ATK_Aerial_Ace, &ATK_Water_Pulse, },
		true, false, false, { }, species::CAT_BABY, 10, nullptr, species::EVOL_NOITEM, },
  {  459, "Snover", TYPE_GRASS, TYPE_ICE, 115, 105, 155, nullptr,
		{ &ATK_Ice_Shard, &ATK_Powder_Snow, &ATK_Leafage, &ATK_Ice_Beam, &ATK_Stomp, &ATK_Energy_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  460, "Abomasnow", TYPE_GRASS, TYPE_ICE, 178, 158, 207, "Snover",
		{ &ATK_Razor_Leaf, &ATK_Powder_Snow, &ATK_Leafage, &ATK_Blizzard, &ATK_Icy_Wind, &ATK_Energy_Ball, &ATK_Outrage, &ATK_Weather_Ball_Ice, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  461, "Weavile", TYPE_DARK, TYPE_ICE, 243, 171, 172, "Sneasel",
		{ &ATK_Ice_Shard, &ATK_Feint_Attack, &ATK_Snarl, &ATK_Focus_Blast, &ATK_Avalanche, &ATK_Foul_Play, &ATK_Triple_Axel, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  // the Magnet Area
  {  462, "Magnezone", TYPE_ELECTRIC, TYPE_STEEL, 238, 205, 172, "Magneton",
		{ &ATK_Spark, &ATK_Charge_Beam, &ATK_Volt_Switch, &ATK_Metal_Sound, &ATK_Flash_Cannon, &ATK_Wild_Charge, &ATK_Zap_Cannon, &ATK_Mirror_Shot, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  463, "Lickilicky", TYPE_NORMAL, TYPECOUNT, 161, 181, 242, "Lickitung",
		{ &ATK_Lick, &ATK_Zen_Headbutt, &ATK_Rollout, &ATK_Hyper_Beam, &ATK_Earthquake, &ATK_Shadow_Ball, &ATK_Solar_Beam, &ATK_Body_Slam, },
		true, false, false, { &ATK_Body_Slam, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  {  464, "Rhyperior", TYPE_GROUND, TYPE_ROCK, 241, 190, 251, "Rhydon",
		{ &ATK_Mud_Slap, &ATK_Smack_Down, &ATK_Earthquake, &ATK_Stone_Edge, &ATK_Surf, &ATK_Skull_Bash, &ATK_Superpower, &ATK_Rock_Wrecker, &ATK_Breaking_Swipe, },
		true, true, false, { &ATK_Rock_Wrecker, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  {  465, "Tangrowth", TYPE_GRASS, TYPECOUNT, 207, 184, 225, "Tangela",
		{ &ATK_Vine_Whip, &ATK_Infestation, &ATK_Ancient_Power, &ATK_Rock_Slide, &ATK_Sludge_Bomb, &ATK_Solar_Beam, &ATK_Power_Whip, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  {  466, "Electivire", TYPE_ELECTRIC, TYPECOUNT, 249, 163, 181, "Electabuzz",
		{ &ATK_Thunder_Shock, &ATK_Low_Kick, &ATK_Flamethrower, &ATK_Ice_Punch, &ATK_Thunder_Punch, &ATK_Thunder, &ATK_Wild_Charge, },
		true, true, false, { &ATK_Flamethrower, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_SINNOHSTONE, },
  {  467, "Magmortar", TYPE_FIRE, TYPECOUNT, 247, 172, 181, "Magmar",
		{ &ATK_Karate_Chop, &ATK_Fire_Spin, &ATK_Thunderbolt, &ATK_Fire_Blast, &ATK_Psychic, &ATK_Fire_Punch, &ATK_Brick_Break, &ATK_Scorching_Sands, },
		true, true, false, { &ATK_Thunderbolt, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_SINNOHSTONE, },
  {  468, "Togekiss", TYPE_FAIRY, TYPE_FLYING, 225, 217, 198, "Togetic",
		{ &ATK_Air_Slash, &ATK_Hidden_Power, &ATK_Charm, &ATK_Peck, &ATK_Flamethrower, &ATK_Aerial_Ace, &ATK_Psyshock, &ATK_Ancient_Power, &ATK_Dazzling_Gleam, &ATK_Aura_Sphere, },
		true, false, false, { &ATK_Aura_Sphere, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  {  469, "Yanmega", TYPE_BUG, TYPE_FLYING, 231, 156, 200, "Yanma",
		{ &ATK_Bug_Bite, &ATK_Wing_Attack, &ATK_Aerial_Ace, &ATK_Bug_Buzz, &ATK_Ancient_Power, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  {  470, "Leafeon", TYPE_GRASS, TYPECOUNT, 216, 219, 163, "Eevee",
		{ &ATK_Razor_Leaf, &ATK_Quick_Attack, &ATK_Bullet_Seed, &ATK_Solar_Beam, &ATK_Leaf_Blade, &ATK_Energy_Ball, &ATK_Last_Resort, },
		true, false, false, { &ATK_Bullet_Seed, &ATK_Last_Resort, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  471, "Glaceon", TYPE_ICE, TYPECOUNT, 238, 205, 163, "Eevee",
		{ &ATK_Ice_Shard, &ATK_Frost_Breath, &ATK_Ice_Beam, &ATK_Water_Pulse, &ATK_Icy_Wind, &ATK_Avalanche, &ATK_Last_Resort, },
		true, false, false, { &ATK_Water_Pulse, &ATK_Last_Resort, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  472, "Gliscor", TYPE_GROUND, TYPE_FLYING, 185, 222, 181, "Gligar",
		{ &ATK_Fury_Cutter, &ATK_Wing_Attack, &ATK_Sand_Attack, &ATK_Earthquake, &ATK_Aerial_Ace, &ATK_Night_Slash, &ATK_Sand_Tomb, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_SINNOHSTONE, },
  {  473, "Mamoswine", TYPE_ICE, TYPE_GROUND, 247, 146, 242, "Piloswine",
		{ &ATK_Mud_Slap, &ATK_Powder_Snow, &ATK_Stone_Edge, &ATK_Ancient_Power, &ATK_Bulldoze, &ATK_Avalanche, &ATK_Icicle_Spear, &ATK_High_Horsepower, },
		true, true, false, { &ATK_Ancient_Power, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  {  474, "Porygon-Z", TYPE_NORMAL, TYPECOUNT, 264, 150, 198, "Porygon2",
		{ &ATK_Charge_Beam, &ATK_Hidden_Power, &ATK_Lock_On, &ATK_Hyper_Beam, &ATK_Blizzard, &ATK_Solar_Beam, &ATK_Zap_Cannon, &ATK_Tri_Attack, },
		true, true, false, { &ATK_Tri_Attack, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  {  475, "Gallade", TYPE_PSYCHIC, TYPE_FIGHTING, 237, 195, 169, "Kirlia",
		{ &ATK_Low_Kick, &ATK_Psycho_Cut, &ATK_Confusion, &ATK_Charm, &ATK_Psychic, &ATK_Leaf_Blade, &ATK_Close_Combat, &ATK_Synchronoise, },
		true, true, false, { &ATK_Synchronoise, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_SINNOHSTONE, },
  {  476, "Probopass", TYPE_ROCK, TYPE_STEEL, 135, 275, 155, "Nosepass",
		{ &ATK_Spark, &ATK_Rock_Throw, &ATK_Rock_Slide, &ATK_Magnet_Bomb, &ATK_Thunderbolt, &ATK_Zap_Cannon, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  477, "Dusknoir", TYPE_GHOST, TYPECOUNT, 180, 254, 128, "Dusclops",
		{ &ATK_Astonish, &ATK_Hex, &ATK_Dark_Pulse, &ATK_Shadow_Punch, &ATK_Ominous_Wind, &ATK_Shadow_Ball, &ATK_Psychic, &ATK_Dynamic_Punch, &ATK_Poltergeist, },
		true, true, false, { &ATK_Shadow_Ball, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  {  478, "Froslass", TYPE_ICE, TYPE_GHOST, 171, 150, 172, "Snorunt",
		{ &ATK_Powder_Snow, &ATK_Hex, &ATK_Shadow_Ball, &ATK_Avalanche, &ATK_Crunch, &ATK_Triple_Axel, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SINNOHSTONE, },
  {  479, "Rotom", TYPE_ELECTRIC, TYPE_GHOST, 185, 159, 137, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Astonish, &ATK_Ominous_Wind, &ATK_Thunder, &ATK_Thunderbolt, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  479, "Heat Rotom", TYPE_ELECTRIC, TYPE_FIRE, 204, 219, 137, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Astonish, &ATK_Thunder, &ATK_Thunderbolt, &ATK_Overheat, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  479, "Wash Rotom", TYPE_ELECTRIC, TYPE_WATER, 204, 219, 137, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Astonish, &ATK_Thunder, &ATK_Thunderbolt, &ATK_Hydro_Pump, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  479, "Frost Rotom", TYPE_ELECTRIC, TYPE_ICE, 204, 219, 137, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Astonish, &ATK_Blizzard, &ATK_Thunder, &ATK_Thunderbolt, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // fan rotom has not been released as of 2025
  /*{  479, "Fan Rotom", TYPE_ELECTRIC, TYPE_FLYING, 204, 219, 137, nullptr,
		{ &ATK_Air_Slash, &ATK_Astonish, &ATK_Ominous_Wind, &ATK_Thunder, &ATK_Thunderbolt, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },*/
  {  479, "Mow Rotom", TYPE_ELECTRIC, TYPE_GRASS, 204, 219, 137, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Astonish, &ATK_Ominous_Wind, &ATK_Thunder, &ATK_Thunderbolt, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the knowledge
  {  480, "Uxie", TYPE_PSYCHIC, TYPECOUNT, 156, 270, 181, nullptr,
		{ &ATK_Confusion, &ATK_Extrasensory, &ATK_Thunder, &ATK_Swift, &ATK_Future_Sight, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // the being of emotion
  {  481, "Mesprit", TYPE_PSYCHIC, TYPECOUNT, 212, 212, 190, nullptr,
		{ &ATK_Confusion, &ATK_Extrasensory, &ATK_Blizzard, &ATK_Swift, &ATK_Future_Sight, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  482, "Azelf", TYPE_PSYCHIC, TYPECOUNT, 270, 151, 181, nullptr,
		{ &ATK_Confusion, &ATK_Extrasensory, &ATK_Fire_Blast, &ATK_Swift, &ATK_Future_Sight, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  483, "Dialga", TYPE_STEEL, TYPE_DRAGON, 275, 211, 205, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Metal_Claw, &ATK_Iron_Head, &ATK_Thunder, &ATK_Draco_Meteor, },
		true, true, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  483, "Origin Forme Dialga", TYPE_STEEL, TYPE_DRAGON, 270, 225, 205, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Metal_Claw, &ATK_Iron_Head, &ATK_Thunder, &ATK_Draco_Meteor, &ATK_Roar_of_Time, },
		true, true, false, { &ATK_Roar_of_Time, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  484, "Palkia", TYPE_WATER, TYPE_DRAGON, 280, 215, 189, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Dragon_Tail, &ATK_Aqua_Tail, &ATK_Fire_Blast, &ATK_Hydro_Pump, &ATK_Draco_Meteor, },
		true, true, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  484, "Origin Forme Palkia", TYPE_WATER, TYPE_DRAGON, 286, 223, 189, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Dragon_Tail, &ATK_Aqua_Tail, &ATK_Fire_Blast, &ATK_Hydro_Pump, &ATK_Draco_Meteor, &ATK_Spacial_Rend, },
		true, true, false, { &ATK_Spacial_Rend, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  485, "Heatran", TYPE_FIRE, TYPE_STEEL, 251, 213, 209, nullptr,
		{ &ATK_Bug_Bite, &ATK_Fire_Spin, &ATK_Flamethrower, &ATK_Stone_Edge, &ATK_Iron_Head, &ATK_Fire_Blast, &ATK_Earth_Power, &ATK_Magma_Storm, },
		true, true, false, { &ATK_Magma_Storm, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  486, "Regigigas", TYPE_NORMAL, TYPECOUNT, 287, 210, 221, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Hidden_Power, &ATK_Thunder, &ATK_Focus_Blast, &ATK_Giga_Impact, },
		true, true, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  487, "Altered Forme Giratina", TYPE_GHOST, TYPE_DRAGON, 187, 225, 284, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Shadow_Claw, &ATK_Ancient_Power, &ATK_Shadow_Sneak, &ATK_Dragon_Claw, &ATK_Shadow_Force, },
		true, true, false, { &ATK_Shadow_Force, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  487, "Origin Forme Giratina", TYPE_GHOST, TYPE_DRAGON, 225, 187, 284, nullptr,
		{ &ATK_Shadow_Claw, &ATK_Dragon_Tail, &ATK_Ominous_Wind, &ATK_Shadow_Ball, &ATK_Dragon_Pulse, &ATK_Shadow_Force, },
		true, true, false, { &ATK_Shadow_Force, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  488, "Cresselia", TYPE_PSYCHIC, TYPECOUNT, 152, 258, 260, nullptr,
		{ &ATK_Psycho_Cut, &ATK_Confusion, &ATK_Moonblast, &ATK_Aurora_Beam, &ATK_Grass_Knot, &ATK_Future_Sight, },
		true, true, false, { &ATK_Grass_Knot, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // phione and manaphy have not been released as of 2025
  // FIXME when released, uncomment the "Sea Guardians" entry in species.tex
  /* {  489, "Phione", TYPE_WATER, TYPECOUNT, 162, 162, 190, nullptr,
		{ &ATK_Bubble, &ATK_Waterfall, &ATK_Bubble_Beam, &ATK_Water_Pulse, &ATK_Surf, },
		false, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  490, "Manaphy", TYPE_WATER, TYPECOUNT, 210, 210, 225, nullptr,
		{ &ATK_Bubble, &ATK_Waterfall, &ATK_Bubble_Beam, &ATK_Psychic, &ATK_Surf, },
		false, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },*/
  {  491, "Darkrai", TYPE_DARK, TYPECOUNT, 285, 198, 172, nullptr,
		{ &ATK_Feint_Attack, &ATK_Snarl, &ATK_Dark_Pulse, &ATK_Shadow_Ball, &ATK_Sludge_Bomb, &ATK_Focus_Blast, },
		true, false, false, { &ATK_Sludge_Bomb, }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  492, "Shaymin", TYPE_GRASS, TYPECOUNT, 210, 210, 225, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Hidden_Power, &ATK_Solar_Beam, &ATK_Grass_Knot, &ATK_Energy_Ball, &ATK_Seed_Flare, },
		true, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  492, "Sky Shaymin", TYPE_GRASS, TYPE_FLYING, 261, 166, 225, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Hidden_Power, &ATK_Magical_Leaf, &ATK_Solar_Beam, &ATK_Grass_Knot, &ATK_Energy_Ball, &ATK_Seed_Flare, },
		true, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  // 493 is arceus
  {  494, "Victini", TYPE_PSYCHIC, TYPE_FIRE, 210, 210, 225, nullptr,
		{ &ATK_Quick_Attack, &ATK_Confusion, &ATK_Psyshock, &ATK_Psychic, &ATK_Focus_Blast, &ATK_Overheat, &ATK_V_Create, },
		false, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  495, "Snivy", TYPE_GRASS, TYPECOUNT, 88, 107, 128, nullptr,
		{ &ATK_Vine_Whip, &ATK_Tackle, &ATK_Wrap, &ATK_Seed_Bomb, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  496, "Servine", TYPE_GRASS, TYPECOUNT, 122, 152, 155, "Snivy",
		{ &ATK_Vine_Whip, &ATK_Iron_Tail, &ATK_Wrap, &ATK_Grass_Knot, &ATK_Leaf_Tornado, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  497, "Serperior", TYPE_GRASS, TYPECOUNT, 161, 204, 181, "Servine",
		{ &ATK_Vine_Whip, &ATK_Iron_Tail, &ATK_Aerial_Ace, &ATK_Grass_Knot, &ATK_Frenzy_Plant, &ATK_Leaf_Tornado, },
		true, false, false, { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  498, "Tepig", TYPE_FIRE, TYPECOUNT, 115, 85, 163, nullptr,
		{ &ATK_Ember, &ATK_Tackle, &ATK_Flamethrower, &ATK_Flame_Charge, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  499, "Pignite", TYPE_FIRE, TYPE_FIGHTING, 173, 106, 207, "Tepig",
		{ &ATK_Ember, &ATK_Tackle, &ATK_Flamethrower, &ATK_Rock_Tomb, &ATK_Flame_Charge, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  500, "Emboar", TYPE_FIRE, TYPE_FIGHTING, 235, 127, 242, "Pignite",
		{ &ATK_Low_Kick, &ATK_Ember, &ATK_Heat_Wave, &ATK_Rock_Slide, &ATK_Flame_Charge, &ATK_Focus_Blast, &ATK_Blast_Burn, },
		true, true, false, { &ATK_Blast_Burn, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  501, "Oshawott", TYPE_WATER, TYPECOUNT, 117, 85, 146, nullptr,
		{ &ATK_Tackle, &ATK_Water_Gun, &ATK_Night_Slash, &ATK_Aqua_Tail, &ATK_Water_Pulse, },
		true, true, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  502, "Dewott", TYPE_WATER, TYPECOUNT, 159, 116, 181, "Oshawott",
		{ &ATK_Fury_Cutter, &ATK_Water_Gun, &ATK_Aqua_Tail, &ATK_X_Scissor, &ATK_Water_Pulse, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  503, "Samurott", TYPE_WATER, TYPECOUNT, 212, 157, 216, "Dewott",
		{ &ATK_Fury_Cutter, &ATK_Waterfall, &ATK_Megahorn, &ATK_Blizzard, &ATK_Hydro_Pump, &ATK_Hydro_Cannon, &ATK_Razor_Shell, &ATK_Liquidation, },
		true, true, false, { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  503, "Hisuian Samurott", TYPE_WATER, TYPE_DARK, 218, 152, 207, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Snarl, &ATK_Waterfall, &ATK_Dark_Pulse, &ATK_X_Scissor, &ATK_Icy_Wind, &ATK_Razor_Shell, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  504, "Patrat", TYPE_NORMAL, TYPECOUNT, 98, 73, 128, nullptr,
		{ &ATK_Bite, &ATK_Tackle, &ATK_Dig, &ATK_Hyper_Fang, &ATK_Grass_Knot, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  505, "Watchog", TYPE_NORMAL, TYPECOUNT, 165, 139, 155, "Patrat",
		{ &ATK_Bite, &ATK_Low_Kick, &ATK_Hyper_Fang, &ATK_Grass_Knot, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  506, "Lillipup", TYPE_NORMAL, TYPECOUNT, 107, 86, 128, nullptr,
		{ &ATK_Tackle, &ATK_Take_Down, &ATK_Sand_Attack, &ATK_Dig, &ATK_Rock_Tomb, &ATK_Thunderbolt, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  507, "Herdier", TYPE_NORMAL, TYPECOUNT, 145, 126, 163, "Lillipup",
		{ &ATK_Lick, &ATK_Take_Down, &ATK_Sand_Attack, &ATK_Dig, &ATK_Thunderbolt, &ATK_Play_Rough, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  508, "Stoutland", TYPE_NORMAL, TYPECOUNT, 206, 182, 198, "Herdier",
		{ &ATK_Lick, &ATK_Take_Down, &ATK_Ice_Fang, &ATK_Sand_Attack, &ATK_Play_Rough, &ATK_Wild_Charge, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  509, "Purrloin", TYPE_DARK, TYPECOUNT, 98, 73, 121, nullptr,
		{ &ATK_Sucker_Punch, &ATK_Scratch, &ATK_Dark_Pulse, &ATK_Night_Slash, &ATK_Play_Rough, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  510, "Liepard", TYPE_DARK, TYPECOUNT, 187, 106, 162, "Purrloin",
		{ &ATK_Snarl, &ATK_Charm, &ATK_Dark_Pulse, &ATK_Play_Rough, &ATK_Gunk_Shot, &ATK_Payback, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  511, "Pansage", TYPE_GRASS, TYPECOUNT, 104, 94, 137, nullptr,
		{ &ATK_Vine_Whip, &ATK_Scratch, &ATK_Seed_Bomb, &ATK_Energy_Ball, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  512, "Simisage", TYPE_GRASS, TYPECOUNT, 206, 133, 181, "Pansage",
		{ &ATK_Bite, &ATK_Vine_Whip, &ATK_Solar_Beam, &ATK_Grass_Knot, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_UNOVASTONE, },
  {  513, "Pansear", TYPE_FIRE, TYPECOUNT, 104, 94, 137, nullptr,
		{ &ATK_Scratch, &ATK_Fire_Spin, &ATK_Flame_Charge, &ATK_Flame_Burst, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  514, "Simisear", TYPE_FIRE, TYPECOUNT, 206, 133, 181, "Pansear",
		{ &ATK_Bite, &ATK_Fire_Spin, &ATK_Flamethrower, &ATK_Fire_Blast, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_UNOVASTONE, },
  {  515, "Panpour", TYPE_WATER, TYPECOUNT, 104, 94, 137, nullptr,
		{ &ATK_Scratch, &ATK_Water_Gun, &ATK_Water_Pulse, &ATK_Crunch, &ATK_Surf, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  516, "Simipour", TYPE_WATER, TYPECOUNT, 206, 133, 181, "Panpour",
		{ &ATK_Bite, &ATK_Water_Gun, &ATK_Hydro_Pump, &ATK_Crunch, &ATK_Surf, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_UNOVASTONE, },
  {  517, "Munna", TYPE_PSYCHIC, TYPECOUNT, 111, 92, 183, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Charge_Beam, &ATK_Psybeam, &ATK_Psyshock, &ATK_Dazzling_Gleam, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  518, "Musharna", TYPE_PSYCHIC, TYPECOUNT, 183, 166, 253, "Munna",
		{ &ATK_Zen_Headbutt, &ATK_Charge_Beam, &ATK_Psyshock, &ATK_Dazzling_Gleam, &ATK_Future_Sight, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_UNOVASTONE, },
  {  519, "Pidove", TYPE_NORMAL, TYPE_FLYING, 98, 80, 137, nullptr,
		{ &ATK_Quick_Attack, &ATK_Air_Slash, &ATK_Heat_Wave, &ATK_Aerial_Ace, &ATK_Air_Cutter, },
		true, true, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  520, "Tranquill", TYPE_NORMAL, TYPE_FLYING, 144, 107, 158, "Pidove",
		{ &ATK_Steel_Wing, &ATK_Air_Slash, &ATK_Heat_Wave, &ATK_Aerial_Ace, &ATK_Sky_Attack, },
		true, true, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  521, "Unfezant", TYPE_NORMAL, TYPE_FLYING, 226, 146, 190, "Tranquill",
		{ &ATK_Steel_Wing, &ATK_Air_Slash, &ATK_Hyper_Beam, &ATK_Heat_Wave, &ATK_Sky_Attack, },
		true, true, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  522, "Blitzle", TYPE_ELECTRIC, TYPECOUNT, 118, 64, 128, nullptr,
		{ &ATK_Spark, &ATK_Low_Kick, &ATK_Discharge, &ATK_Flame_Charge, &ATK_Wild_Charge, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  523, "Zebstrika", TYPE_ELECTRIC, TYPECOUNT, 211, 136, 181, "Blitzle",
		{ &ATK_Spark, &ATK_Quick_Attack, &ATK_Discharge, &ATK_Thunderbolt, &ATK_Flame_Charge, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  524, "Roggenrola", TYPE_ROCK, TYPECOUNT, 121, 110, 146, nullptr,
		{ &ATK_Tackle, &ATK_Smack_Down, &ATK_Stone_Edge, &ATK_Bulldoze, &ATK_Rock_Blast, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  525, "Boldore", TYPE_ROCK, TYPECOUNT, 174, 143, 172, "Roggenrola",
		{ &ATK_Mud_Slap, &ATK_Smack_Down, &ATK_Stone_Edge, &ATK_Rock_Slide, &ATK_Bulldoze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  526, "Gigalith", TYPE_ROCK, TYPECOUNT, 226, 201, 198, "Boldore",
		{ &ATK_Mud_Slap, &ATK_Smack_Down, &ATK_Rock_Slide, &ATK_Solar_Beam, &ATK_Heavy_Slam, &ATK_Superpower, &ATK_Meteor_Beam, },
		true, true, false, { &ATK_Meteor_Beam, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  527, "Woobat", TYPE_PSYCHIC, TYPE_FLYING, 107, 85, 163, nullptr,
		{ &ATK_Confusion, &ATK_Air_Slash, &ATK_Aerial_Ace, &ATK_Psyshock, &ATK_Air_Cutter, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  528, "Swoobat", TYPE_PSYCHIC, TYPE_FLYING, 161, 119, 167, "Woobat",
		{ &ATK_Confusion, &ATK_Air_Slash, &ATK_Aerial_Ace, &ATK_Psychic, &ATK_Future_Sight, &ATK_Fly, &ATK_Psychic_Fangs, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  529, "Drilbur", TYPE_GROUND, TYPECOUNT, 154, 85, 155, nullptr,
		{ &ATK_Scratch, &ATK_Mud_Slap, &ATK_Dig, &ATK_Drill_Run, &ATK_Rock_Tomb, },
		true, true, true, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  530, "Excadrill", TYPE_GROUND, TYPE_STEEL, 255, 129, 242, "Drilbur",
		{ &ATK_Mud_Shot, &ATK_Metal_Claw, &ATK_Mud_Slap, &ATK_Earthquake, &ATK_Drill_Run, &ATK_Rock_Slide, &ATK_Iron_Head, &ATK_Scorching_Sands, },
		true, true, true, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  531, "Audino", TYPE_NORMAL, TYPECOUNT, 114, 163, 230, nullptr,
		{ &ATK_Pound, &ATK_Zen_Headbutt, &ATK_Hyper_Beam, &ATK_Disarming_Voice, &ATK_Dazzling_Gleam, &ATK_Body_Slam, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  532, "Timburr", TYPE_FIGHTING, TYPECOUNT, 134, 87, 181, nullptr,
		{ &ATK_Low_Kick, &ATK_Pound, &ATK_Low_Sweep, &ATK_Rock_Tomb, &ATK_Brick_Break, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  533, "Gurdurr", TYPE_FIGHTING, TYPECOUNT, 180, 134, 198, "Timburr",
		{ &ATK_Low_Kick, &ATK_Poison_Jab, &ATK_Stone_Edge, &ATK_Low_Sweep, &ATK_Brick_Break, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  534, "Conkeldurr", TYPE_FIGHTING, TYPECOUNT, 243, 158, 233, "Gurdurr",
		{ &ATK_Poison_Jab, &ATK_Counter, &ATK_Stone_Edge, &ATK_Dynamic_Punch, &ATK_Focus_Blast, &ATK_Brutal_Swing, },
		true, true, false, { &ATK_Brutal_Swing, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  535, "Tympole", TYPE_WATER, TYPECOUNT, 98, 78, 137, nullptr,
		{ &ATK_Mud_Shot, &ATK_Bubble, &ATK_Sludge_Wave, &ATK_Mud_Bomb, &ATK_Water_Pulse, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  536, "Palpitoad", TYPE_WATER, TYPE_GROUND, 128, 109, 181, "Tympole",
		{ &ATK_Mud_Shot, &ATK_Bubble, &ATK_Sludge_Wave, &ATK_Water_Pulse, &ATK_Earth_Power, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  537, "Seismitoad", TYPE_WATER, TYPE_GROUND, 188, 150, 233, "Palpitoad",
		{ &ATK_Mud_Shot, &ATK_Bubble, &ATK_Sludge_Bomb, &ATK_Earth_Power, &ATK_Muddy_Water, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  538, "Throh", TYPE_FIGHTING, TYPECOUNT, 172, 160, 260, nullptr,
		{ &ATK_Low_Kick, &ATK_Zen_Headbutt, &ATK_Low_Sweep, &ATK_Brick_Break, &ATK_Body_Slam, &ATK_Focus_Blast, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  539, "Sawk", TYPE_FIGHTING, TYPECOUNT, 231, 153, 181, nullptr,
		{ &ATK_Low_Kick, &ATK_Poison_Jab, &ATK_Low_Sweep, &ATK_Brick_Break, &ATK_Body_Slam, &ATK_Focus_Blast, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  540, "Sewaddle", TYPE_BUG, TYPE_GRASS, 96, 124, 128, nullptr,
		{ &ATK_Bug_Bite, &ATK_Struggle_Bug, &ATK_Seed_Bomb, &ATK_Silver_Wind, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  541, "Swadloon", TYPE_BUG, TYPE_GRASS, 115, 162, 146, "Sewaddle",
		{ &ATK_Bug_Bite, &ATK_Struggle_Bug, &ATK_Bug_Buzz, &ATK_Silver_Wind, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  542, "Leavanny", TYPE_BUG, TYPE_GRASS, 205, 165, 181, "Swadloon",
		{ &ATK_Bug_Bite, &ATK_Shadow_Claw, &ATK_Razor_Leaf, &ATK_X_Scissor, &ATK_Leaf_Blade, &ATK_Silver_Wind, &ATK_Leaf_Storm, },
		true, false, false, { &ATK_Shadow_Claw, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  543, "Venipede", TYPE_BUG, TYPE_POISON, 83, 99, 102, nullptr,
		{ &ATK_Bug_Bite, &ATK_Poison_Sting, &ATK_Sludge_Bomb, &ATK_Signal_Beam, &ATK_Gyro_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  544, "Whirlipede", TYPE_BUG, TYPE_POISON, 100, 173, 120, "Venipede",
		{ &ATK_Bug_Bite, &ATK_Poison_Sting, &ATK_Sludge_Bomb, &ATK_Signal_Beam, &ATK_Gyro_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  545, "Scolipede", TYPE_BUG, TYPE_POISON, 203, 175, 155, "Whirlipede",
		{ &ATK_Bug_Bite, &ATK_Poison_Jab, &ATK_Poison_Sting, &ATK_Megahorn, &ATK_Sludge_Bomb, &ATK_X_Scissor, &ATK_Gyro_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  546, "Cottonee", TYPE_GRASS, TYPE_FAIRY, 71, 111, 120, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Charm, &ATK_Seed_Bomb, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  547, "Whimsicott", TYPE_GRASS, TYPE_FAIRY, 164, 176, 155, "Cottonee",
		{ &ATK_Razor_Leaf, &ATK_Charm, &ATK_Fairy_Wind, &ATK_Seed_Bomb, &ATK_Moonblast, &ATK_Hurricane, &ATK_Grass_Knot, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_SUNSTONE, },
  {  548, "Petilil", TYPE_GRASS, TYPECOUNT, 119, 91, 128, nullptr,
		{ &ATK_Hidden_Power, &ATK_Charm, &ATK_Seed_Bomb, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  549, "Lilligant", TYPE_GRASS, TYPECOUNT, 214, 155, 172, "Petilil",
		{ &ATK_Hidden_Power, &ATK_Charm, &ATK_Magical_Leaf, &ATK_Hyper_Beam, &ATK_Petal_Blizzard, &ATK_Solar_Beam, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_SUNSTONE, },
  {  549, "Hisuian Lilligant", TYPE_GRASS, TYPECOUNT, 214, 155, 172, nullptr,
		{ &ATK_Hidden_Power, &ATK_Charm, &ATK_Magical_Leaf, &ATK_Hyper_Beam, &ATK_Petal_Blizzard, &ATK_Solar_Beam, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  550, "Basculin", TYPE_WATER, TYPECOUNT, 189, 129, 172, nullptr,
		{ &ATK_Tackle, &ATK_Water_Gun, &ATK_Aqua_Jet, &ATK_Aqua_Tail, &ATK_Muddy_Water, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  551, "Sandile", TYPE_GROUND, TYPE_DARK, 132, 69, 137, nullptr,
		{ &ATK_Bite, &ATK_Mud_Slap, &ATK_Dig, &ATK_Bulldoze, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  552, "Krokorok", TYPE_GROUND, TYPE_DARK, 155, 90, 155, "Sandile",
		{ &ATK_Bite, &ATK_Mud_Slap, &ATK_Earthquake, &ATK_Bulldoze, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  553, "Krookodile", TYPE_GROUND, TYPE_DARK, 229, 158, 216, "Krokorok",
		{ &ATK_Mud_Slap, &ATK_Snarl, &ATK_Sand_Tomb, &ATK_Earthquake, &ATK_Brick_Break, &ATK_Outrage, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  554, "Darumaka", TYPE_FIRE, TYPECOUNT, 153, 86, 172, nullptr,
		{ &ATK_Tackle, &ATK_Fire_Fang, &ATK_Flame_Charge, &ATK_Fire_Punch, },
		true, true, true, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  554, "Galarian Darumaka", TYPE_ICE, TYPECOUNT, 153, 86, 172, nullptr,
		{ &ATK_Tackle, &ATK_Ice_Fang, &ATK_Ice_Punch, &ATK_Ice_Beam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  555, "Darmanitan", TYPE_FIRE, TYPECOUNT, 263, 114, 233, "Darumaka",
		{ &ATK_Tackle, &ATK_Fire_Fang, &ATK_Incinerate, &ATK_Rock_Slide, &ATK_Psychic, &ATK_Focus_Blast, &ATK_Overheat, },
		true, true, true, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  555, "Galarian Darmanitan", TYPE_ICE, TYPECOUNT, 263, 114, 233, "Galarian Darumaka",
		{ &ATK_Tackle, &ATK_Ice_Fang, &ATK_Ice_Punch, &ATK_Avalanche, &ATK_Overheat, &ATK_Superpower, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  556, "Maractus", TYPE_GRASS, TYPECOUNT, 201, 130, 181, nullptr,
		{ &ATK_Poison_Jab, &ATK_Bullet_Seed, &ATK_Aerial_Ace, &ATK_Petal_Blizzard, &ATK_Solar_Beam, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  557, "Dwebble", TYPE_BUG, TYPE_ROCK, 118, 128, 137, nullptr,
		{ &ATK_Cut, &ATK_Smack_Down, &ATK_Rock_Tomb, &ATK_X_Scissor, &ATK_Rock_Blast, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  558, "Crustle", TYPE_BUG, TYPE_ROCK, 188, 200, 172, "Dwebble",
		{ &ATK_Fury_Cutter, &ATK_Smack_Down, &ATK_Rock_Wrecker, &ATK_Rock_Slide, &ATK_X_Scissor, &ATK_Rock_Blast, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the shedding
  {  559, "Scraggy", TYPE_DARK, TYPE_FIGHTING, 132, 132, 137, nullptr,
		{ &ATK_Feint_Attack, &ATK_Rock_Smash, &ATK_Thunder_Punch, &ATK_Brick_Break, &ATK_Foul_Play, &ATK_Acid_Spray, &ATK_Upper_Hand, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the hoodlum
  {  560, "Scrafty", TYPE_DARK, TYPE_FIGHTING, 163, 222, 163, "Scraggy",
		{ &ATK_Counter, &ATK_Snarl, &ATK_Thunder_Punch, &ATK_Foul_Play, &ATK_Acid_Spray, &ATK_Power_Up_Punch, &ATK_Upper_Hand, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  561, "Sigilyph", TYPE_PSYCHIC, TYPE_FLYING, 204, 167, 176, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Air_Slash, &ATK_Psybeam, &ATK_Ancient_Power, &ATK_Signal_Beam, &ATK_Air_Cutter, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  562, "Yamask", TYPE_GHOST, TYPECOUNT, 95, 141, 116, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Astonish, &ATK_Dark_Pulse, &ATK_Ominous_Wind, &ATK_Shadow_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  562, "Galarian Yamask", TYPE_GROUND, TYPE_GHOST, 95, 141, 116, nullptr,
		{ &ATK_Astonish, &ATK_Rock_Tomb, &ATK_Night_Shade, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  563, "Cofagrigus", TYPE_GHOST, TYPECOUNT, 163, 237, 151, "Yamask",
		{ &ATK_Shadow_Claw, &ATK_Zen_Headbutt, &ATK_Astonish, &ATK_Dark_Pulse, &ATK_Shadow_Ball, &ATK_Psychic, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  564, "Tirtouga", TYPE_WATER, TYPE_ROCK, 134, 146, 144, nullptr,
		{ &ATK_Bite, &ATK_Water_Gun, &ATK_Ancient_Power, &ATK_Body_Slam, &ATK_Surf, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  565, "Carracosta", TYPE_WATER, TYPE_ROCK, 192, 197, 179, "Tirtouga",
		{ &ATK_Rock_Throw, &ATK_Water_Gun, &ATK_Ancient_Power, &ATK_Body_Slam, &ATK_Surf, &ATK_Liquidation, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  566, "Archen", TYPE_ROCK, TYPE_FLYING, 213, 89, 146, nullptr,
		{ &ATK_Wing_Attack, &ATK_Quick_Attack, &ATK_Ancient_Power, &ATK_Dragon_Claw, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  567, "Archeops", TYPE_ROCK, TYPE_FLYING, 292, 139, 181, "Archen",
		{ &ATK_Wing_Attack, &ATK_Steel_Wing, &ATK_Ancient_Power, &ATK_Dragon_Claw, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  568, "Trubbish", TYPE_POISON, TYPECOUNT, 96, 122, 137, nullptr,
		{ &ATK_Pound, &ATK_Take_Down, &ATK_Seed_Bomb, &ATK_Gunk_Shot, },
		true, true, true, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  569, "Garbodor", TYPE_POISON, TYPECOUNT, 181, 164, 190, "Trubbish",
		{ &ATK_Infestation, &ATK_Take_Down, &ATK_Seed_Bomb, &ATK_Gunk_Shot, &ATK_Body_Slam, &ATK_Acid_Spray, },
		true, true, true, { }, species::CAT_NORMAL, 50, "G-Max Malodor", species::EVOL_NOITEM, },
  {  570, "Zorua", TYPE_DARK, TYPECOUNT, 153, 78, 120, nullptr,
		{ &ATK_Scratch, &ATK_Feint_Attack, &ATK_Dark_Pulse, &ATK_Aerial_Ace, &ATK_Night_Shade, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  570, "Hisuian Zorua", TYPE_NORMAL, TYPE_GHOST, 162, 79, 111, nullptr,
		{ &ATK_Scratch, &ATK_Snarl, &ATK_Dark_Pulse, &ATK_Shadow_Ball, &ATK_Night_Shade, &ATK_Foul_Play, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  571, "Zoroark", TYPE_DARK, TYPECOUNT, 250, 127, 155, "Zorua",
		{ &ATK_Shadow_Claw, &ATK_Snarl, &ATK_Flamethrower, &ATK_Sludge_Bomb, &ATK_Night_Shade, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  571, "Hisuian Zoroark", TYPE_NORMAL, TYPE_GHOST, 261, 128, 146, "Hisuian Zorua",
		{ &ATK_Scratch, &ATK_Snarl, &ATK_Dark_Pulse, &ATK_Shadow_Ball, &ATK_Night_Shade, &ATK_Foul_Play, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  572, "Minccino", TYPE_NORMAL, TYPECOUNT, 98, 80, 146, nullptr,
		{ &ATK_Pound, &ATK_Charm, &ATK_Aqua_Tail, &ATK_Thunderbolt, &ATK_Swift, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  573, "Cinccino", TYPE_NORMAL, TYPECOUNT, 198, 130, 181, "Minccino",
		{ &ATK_Pound, &ATK_Charm, &ATK_Hyper_Beam, &ATK_Aqua_Tail, &ATK_Thunderbolt, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_UNOVASTONE, },
  {  574, "Gothita", TYPE_PSYCHIC, TYPECOUNT, 98, 112, 128, nullptr,
		{ &ATK_Pound, &ATK_Confusion, &ATK_Psybeam, &ATK_Psyshock, &ATK_Psychic, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  575, "Gothorita", TYPE_PSYCHIC, TYPECOUNT, 137, 153, 155, "Gothita",
		{ &ATK_Pound, &ATK_Confusion, &ATK_Psybeam, &ATK_Psychic, &ATK_Future_Sight, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  576, "Gothitelle", TYPE_PSYCHIC, TYPECOUNT, 176, 205, 172, "Gothorita",
		{ &ATK_Confusion, &ATK_Charm, &ATK_Rock_Slide, &ATK_Psychic, &ATK_Future_Sight, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  577, "Solosis", TYPE_PSYCHIC, TYPECOUNT, 170, 83, 128, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Hidden_Power, &ATK_Psyshock, &ATK_Thunder, &ATK_Night_Shade, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  578, "Duosion", TYPE_PSYCHIC, TYPECOUNT, 208, 103, 163, "Solosis",
		{ &ATK_Zen_Headbutt, &ATK_Hidden_Power, &ATK_Psyshock, &ATK_Thunder, &ATK_Night_Shade, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  579, "Reuniclus", TYPE_PSYCHIC, TYPECOUNT, 214, 148, 242, "Duosion",
		{ &ATK_Zen_Headbutt, &ATK_Charm, &ATK_Hidden_Power, &ATK_Psyshock, &ATK_Shadow_Ball, &ATK_Thunder, &ATK_Future_Sight, },
		true, true, false, { &ATK_Charm, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  580, "Ducklett", TYPE_WATER, TYPE_FLYING, 84, 96, 158, nullptr,
		{ &ATK_Wing_Attack, &ATK_Water_Gun, &ATK_Aerial_Ace, &ATK_Bubble_Beam, &ATK_Brave_Bird, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  581, "Swanna", TYPE_WATER, TYPE_FLYING, 182, 132, 181, "Ducklett",
		{ &ATK_Water_Gun, &ATK_Air_Slash, &ATK_Ice_Beam, &ATK_Bubble_Beam, &ATK_Hurricane, &ATK_Fly, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  582, "Vanillite", TYPE_ICE, TYPECOUNT, 118, 106, 113, nullptr,
		{ &ATK_Powder_Snow, &ATK_Astonish, &ATK_Ice_Beam, &ATK_Signal_Beam, &ATK_Icy_Wind, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  583, "Vanillish", TYPE_ICE, TYPECOUNT, 151, 138, 139, "Vanillite",
		{ &ATK_Frost_Breath, &ATK_Astonish, &ATK_Ice_Beam, &ATK_Signal_Beam, &ATK_Icy_Wind, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  584, "Vanilluxe", TYPE_ICE, TYPECOUNT, 218, 184, 174, "Vanillish",
		{ &ATK_Frost_Breath, &ATK_Astonish, &ATK_Flash_Cannon, &ATK_Blizzard, &ATK_Signal_Beam, &ATK_Avalanche, },
		true, false, false, { &ATK_Avalanche, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  585, "Deerling", TYPE_NORMAL, TYPE_GRASS, 115, 100, 155, nullptr,
		{ &ATK_Tackle, &ATK_Take_Down, &ATK_Seed_Bomb, &ATK_Wild_Charge, &ATK_Energy_Ball, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  586, "Sawsbuck", TYPE_NORMAL, TYPE_GRASS, 198, 146, 190, "Deerling",
		{ &ATK_Feint_Attack, &ATK_Take_Down, &ATK_Hyper_Beam, &ATK_Megahorn, &ATK_Solar_Beam, &ATK_Wild_Charge, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  587, "Emolga", TYPE_ELECTRIC, TYPE_FLYING, 158, 127, 146, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Quick_Attack, &ATK_Discharge, &ATK_Aerial_Ace, &ATK_Thunderbolt, &ATK_Acrobatics, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  588, "Karrablast", TYPE_BUG, TYPECOUNT, 137, 87, 137, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Peck, &ATK_Aerial_Ace, &ATK_Drill_Run, &ATK_Signal_Beam, &ATK_X_Scissor, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  589, "Escavalier", TYPE_BUG, TYPE_STEEL, 223, 187, 172, "Karrablast",
		{ &ATK_Bug_Bite, &ATK_Counter, &ATK_Megahorn, &ATK_Aerial_Ace, &ATK_Drill_Run, &ATK_Acid_Spray, &ATK_Razor_Shell, },
		true, true, false, { &ATK_Razor_Shell, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  590, "Foongus", TYPE_GRASS, TYPE_POISON, 97, 91, 170, nullptr,
		{ &ATK_Feint_Attack, &ATK_Astonish, &ATK_Body_Slam, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  591, "Amoonguss", TYPE_GRASS, TYPE_POISON, 155, 139, 249, "Foongus",
		{ &ATK_Feint_Attack, &ATK_Astonish, &ATK_Sludge_Bomb, &ATK_Grass_Knot, &ATK_Foul_Play, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  592, "Frillish", TYPE_WATER, TYPE_GHOST, 115, 134, 146, nullptr,
		{ &ATK_Bubble, &ATK_Hex, &ATK_Ice_Beam, &ATK_Ominous_Wind, &ATK_Night_Shade, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  593, "Jellicent", TYPE_WATER, TYPE_GHOST, 159, 178, 225, "Frillish",
		{ &ATK_Bubble, &ATK_Hex, &ATK_Ice_Beam, &ATK_Bubble_Beam, &ATK_Shadow_Ball, &ATK_Surf, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  594, "Alomomola", TYPE_WATER, TYPECOUNT, 138, 131, 338, nullptr,
		{ &ATK_Hidden_Power, &ATK_Waterfall, &ATK_Blizzard, &ATK_Hydro_Pump, &ATK_Psychic, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  595, "Joltik", TYPE_BUG, TYPE_ELECTRIC, 110, 98, 137, nullptr,
		{ &ATK_Sucker_Punch, &ATK_Charge_Beam, &ATK_Discharge, &ATK_Bug_Buzz, &ATK_Cross_Poison, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  596, "Galvantula", TYPE_BUG, TYPE_ELECTRIC, 201, 128, 172, "Joltik",
		{ &ATK_Fury_Cutter, &ATK_Volt_Switch, &ATK_Discharge, &ATK_Bug_Buzz, &ATK_Cross_Poison, &ATK_Energy_Ball, &ATK_Lunge, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  597, "Ferroseed", TYPE_GRASS, TYPE_STEEL, 82, 155, 127, nullptr,
		{ &ATK_Tackle, &ATK_Metal_Claw, &ATK_Flash_Cannon, &ATK_Iron_Head, &ATK_Gyro_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  598, "Ferrothorn", TYPE_GRASS, TYPE_STEEL, 158, 223, 179, "Ferroseed",
		{ &ATK_Metal_Claw, &ATK_Bullet_Seed, &ATK_Flash_Cannon, &ATK_Thunder, &ATK_Power_Whip, &ATK_Acid_Spray, &ATK_Mirror_Shot, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  599, "Klink", TYPE_STEEL, TYPECOUNT, 98, 121, 120, nullptr,
		{ &ATK_Charge_Beam, &ATK_Volt_Switch, &ATK_Metal_Sound, &ATK_Vise_Grip, &ATK_Discharge, &ATK_Zap_Cannon, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  600, "Klang", TYPE_STEEL, TYPECOUNT, 150, 174, 155, "Klink",
		{ &ATK_Thunder_Shock, &ATK_Charge_Beam, &ATK_Metal_Sound, &ATK_Vise_Grip, &ATK_Thunderbolt, &ATK_Zap_Cannon, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  601, "Klinklang", TYPE_STEEL, TYPECOUNT, 199, 214, 155, "Klang",
		{ &ATK_Thunder_Shock, &ATK_Charge_Beam, &ATK_Metal_Sound, &ATK_Hyper_Beam, &ATK_Flash_Cannon, &ATK_Zap_Cannon, &ATK_Mirror_Shot, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  602, "Tynamo", TYPE_ELECTRIC, TYPECOUNT, 105, 78, 111, nullptr,
		{ &ATK_Spark, &ATK_Tackle, &ATK_Struggle, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  603, "Eelektrik", TYPE_ELECTRIC, TYPECOUNT, 156, 130, 163, "Tynamo",
		{ &ATK_Spark, &ATK_Acid, &ATK_Discharge, &ATK_Thunderbolt, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  604, "Eelektross", TYPE_ELECTRIC, TYPECOUNT, 217, 152, 198, "Eelektrik",
		{ &ATK_Spark, &ATK_Acid, &ATK_Volt_Switch, &ATK_Thunderbolt, &ATK_Dragon_Claw, &ATK_Crunch, &ATK_Acid_Spray, &ATK_Liquidation, },
		true, false, false, { &ATK_Volt_Switch, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_UNOVASTONE, },
  {  605, "Elgyem", TYPE_PSYCHIC, TYPECOUNT, 148, 100, 146, nullptr,
		{ &ATK_Confusion, &ATK_Astonish, &ATK_Dark_Pulse, &ATK_Psybeam, &ATK_Psychic, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  606, "Beheeyem", TYPE_PSYCHIC, TYPECOUNT, 221, 163, 181, "Elgyem",
		{ &ATK_Confusion, &ATK_Astonish, &ATK_Dark_Pulse, &ATK_Rock_Slide, &ATK_Psychic, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  607, "Litwick", TYPE_GHOST, TYPE_FIRE, 108, 98, 137, nullptr,
		{ &ATK_Ember, &ATK_Astonish, &ATK_Heat_Wave, &ATK_Flame_Charge, &ATK_Flame_Burst, &ATK_Mystical_Fire, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  608, "Lampent", TYPE_GHOST, TYPE_FIRE, 169, 115, 155, "Litwick",
		{ &ATK_Ember, &ATK_Astonish, &ATK_Heat_Wave, &ATK_Flame_Burst, &ATK_Energy_Ball, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  609, "Chandelure", TYPE_GHOST, TYPE_FIRE, 271, 182, 155, "Lampent",
		{ &ATK_Hex, &ATK_Fire_Spin, &ATK_Incinerate, &ATK_Shadow_Ball, &ATK_Flame_Charge, &ATK_Overheat, &ATK_Energy_Ball, &ATK_Poltergeist, },
		true, true, false, { &ATK_Poltergeist, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_UNOVASTONE, },
  {  610, "Axew", TYPE_DRAGON, TYPECOUNT, 154, 101, 130, nullptr,
		{ &ATK_Scratch, &ATK_Iron_Tail, &ATK_Aqua_Tail, &ATK_Dragon_Pulse, &ATK_Dragon_Claw, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  611, "Fraxure", TYPE_DRAGON, TYPECOUNT, 212, 123, 165, "Axew",
		{ &ATK_Dragon_Tail, &ATK_Iron_Tail, &ATK_Night_Slash, &ATK_Aqua_Tail, &ATK_Dragon_Claw, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  612, "Haxorus", TYPE_DRAGON, TYPECOUNT, 284, 172, 183, "Fraxure",
		{ &ATK_Counter, &ATK_Dragon_Tail, &ATK_Earthquake, &ATK_Night_Slash, &ATK_Dragon_Claw, &ATK_Surf, &ATK_Breaking_Swipe, },
		true, false, false, { &ATK_Breaking_Swipe, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  613, "Cubchoo", TYPE_ICE, TYPECOUNT, 128, 74, 146, nullptr,
		{ &ATK_Powder_Snow, &ATK_Charm, &ATK_Ice_Punch, &ATK_Play_Rough, &ATK_Icy_Wind, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  614, "Beartic", TYPE_ICE, TYPECOUNT, 233, 152, 216, "Cubchoo",
		{ &ATK_Powder_Snow, &ATK_Charm, &ATK_Ice_Punch, &ATK_Play_Rough, &ATK_Surf, &ATK_Liquidation, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  615, "Cryogonal", TYPE_ICE, TYPECOUNT, 190, 218, 190, nullptr,
		{ &ATK_Ice_Shard, &ATK_Frost_Breath, &ATK_Night_Slash, &ATK_Water_Pulse, &ATK_Solar_Beam, &ATK_Aurora_Beam, &ATK_Triple_Axel, },
		true, false, true, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  616, "Shelmet", TYPE_BUG, TYPECOUNT, 72, 140, 137, nullptr,
		{ &ATK_Acid, &ATK_Infestation, &ATK_Bug_Buzz, &ATK_Signal_Beam, &ATK_Body_Slam, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  617, "Accelgor", TYPE_BUG, TYPECOUNT, 220, 120, 190, "Shelmet",
		{ &ATK_Acid, &ATK_Infestation, &ATK_Water_Shuriken, &ATK_Bug_Buzz, &ATK_Signal_Beam, &ATK_Focus_Blast, &ATK_Energy_Ball, &ATK_Acid_Spray, },
		true, true, false, { &ATK_Energy_Ball, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  618, "Stunfisk", TYPE_GROUND, TYPE_ELECTRIC, 144, 171, 240, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Mud_Shot, &ATK_Discharge, &ATK_Mud_Bomb, &ATK_Muddy_Water, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  618, "Galarian Stunfisk", TYPE_GROUND, TYPE_STEEL, 144, 171, 240, nullptr,
		{ &ATK_Mud_Shot, &ATK_Metal_Claw, &ATK_Earthquake, &ATK_Flash_Cannon, &ATK_Rock_Slide, &ATK_Muddy_Water, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  619, "Mienfoo", TYPE_FIGHTING, TYPECOUNT, 160, 98, 128, nullptr,
		{ &ATK_Low_Kick, &ATK_Pound, &ATK_Low_Sweep, &ATK_Drain_Punch, &ATK_Brick_Break, &ATK_Focus_Blast, &ATK_High_Jump_Kick, },
		true, false, false, { &ATK_High_Jump_Kick, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  620, "Mienshao", TYPE_FIGHTING, TYPECOUNT, 258, 127, 163, "Mienfoo",
		{ &ATK_Low_Kick, &ATK_Poison_Jab, &ATK_Force_Palm, &ATK_Drain_Punch, &ATK_Stone_Edge, &ATK_Brick_Break, &ATK_Grass_Knot, &ATK_Blaze_Kick, &ATK_Brutal_Swing, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  621, "Druddigon", TYPE_DRAGON, TYPECOUNT, 213, 170, 184, nullptr,
		{ &ATK_Bite, &ATK_Dragon_Tail, &ATK_Rock_Tomb, &ATK_Hyper_Beam, &ATK_Night_Slash, &ATK_Dragon_Claw, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  622, "Golett", TYPE_GROUND, TYPE_GHOST, 127, 92, 153, nullptr,
		{ &ATK_Mud_Slap, &ATK_Astonish, &ATK_Shadow_Punch, &ATK_Brick_Break, &ATK_Night_Shade, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  623, "Golurk", TYPE_GROUND, TYPE_GHOST, 222, 154, 205, "Golett",
		{ &ATK_Mud_Slap, &ATK_Astonish, &ATK_Shadow_Punch, &ATK_Dynamic_Punch, &ATK_Earth_Power, &ATK_Poltergeist, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  624, "Pawniard", TYPE_DARK, TYPE_STEEL, 154, 114, 128, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Scratch, &ATK_Night_Slash, &ATK_Iron_Head, &ATK_X_Scissor, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  625, "Bisharp", TYPE_DARK, TYPE_STEEL, 232, 176, 163, "Pawniard",
		{ &ATK_Metal_Claw, &ATK_Snarl, &ATK_Dark_Pulse, &ATK_Iron_Head, &ATK_X_Scissor, &ATK_Focus_Blast, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  626, "Bouffalant", TYPE_NORMAL, TYPECOUNT, 195, 182, 216, nullptr,
		{ &ATK_Mud_Shot, &ATK_Zen_Headbutt, &ATK_Megahorn, &ATK_Earthquake, &ATK_Stomp, &ATK_Skull_Bash, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  627, "Rufflet", TYPE_NORMAL, TYPE_FLYING, 150, 97, 172, nullptr,
		{ &ATK_Wing_Attack, &ATK_Peck, &ATK_Aerial_Ace, &ATK_Rock_Tomb, &ATK_Brave_Bird, &ATK_Fly, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  628, "Braviary", TYPE_NORMAL, TYPE_FLYING, 232, 152, 225, "Rufflet",
		{ &ATK_Steel_Wing, &ATK_Air_Slash, &ATK_Heat_Wave, &ATK_Rock_Slide, &ATK_Close_Combat, &ATK_Brave_Bird, &ATK_Fly, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  628, "Hisuian Braviary", TYPE_PSYCHIC, TYPE_FLYING, 213, 137, 242, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Air_Slash, &ATK_Ominous_Wind, &ATK_Dazzling_Gleam, &ATK_Psychic, &ATK_Brave_Bird, &ATK_Fly, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  629, "Vullaby", TYPE_DARK, TYPE_FLYING, 105, 139, 172, nullptr,
		{ &ATK_Feint_Attack, &ATK_Air_Slash, &ATK_Dark_Pulse, &ATK_Brave_Bird, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  630, "Mandibuzz", TYPE_DARK, TYPE_FLYING, 129, 205, 242, "Vullaby",
		{ &ATK_Air_Slash, &ATK_Snarl, &ATK_Dark_Pulse, &ATK_Aerial_Ace, &ATK_Shadow_Ball, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  631, "Heatmor", TYPE_FIRE, TYPECOUNT, 204, 129, 198, nullptr,
		{ &ATK_Lick, &ATK_Fire_Spin, &ATK_Flamethrower, &ATK_Thunder_Punch, &ATK_Power_Up_Punch, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  632, "Durant", TYPE_BUG, TYPE_STEEL, 217, 188, 151, nullptr,
		{ &ATK_Bug_Bite, &ATK_Metal_Claw, &ATK_Stone_Edge, &ATK_Iron_Head, &ATK_X_Scissor, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  633, "Deino", TYPE_DARK, TYPE_DRAGON, 116, 93, 141, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Tackle, &ATK_Dragon_Pulse, &ATK_Body_Slam, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  634, "Zweilous", TYPE_DARK, TYPE_DRAGON, 159, 135, 176, "Deino",
		{ &ATK_Bite, &ATK_Dragon_Breath, &ATK_Dark_Pulse, &ATK_Dragon_Pulse, &ATK_Body_Slam, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  635, "Hydreigon", TYPE_DARK, TYPE_DRAGON, 256, 188, 211, "Zweilous",
		{ &ATK_Bite, &ATK_Dragon_Breath, &ATK_Dark_Pulse, &ATK_Flash_Cannon, &ATK_Dragon_Pulse, &ATK_Brutal_Swing, },
		true, false, false, { &ATK_Brutal_Swing, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  636, "Larvesta", TYPE_BUG, TYPE_FIRE, 156, 107, 146, nullptr,
		{ &ATK_Bug_Bite, &ATK_Ember, &ATK_Flame_Wheel, &ATK_Bug_Buzz, &ATK_Flame_Charge, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  637, "Volcarona", TYPE_BUG, TYPE_FIRE, 264, 189, 198, "Larvesta",
		{ &ATK_Bug_Bite, &ATK_Fire_Spin, &ATK_Bug_Buzz, &ATK_Solar_Beam, &ATK_Hurricane, &ATK_Overheat, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  638, "Cobalion", TYPE_STEEL, TYPE_FIGHTING, 192, 229, 209, nullptr,
		{ &ATK_Metal_Claw, &ATK_Zen_Headbutt, &ATK_Double_Kick, &ATK_Stone_Edge, &ATK_Iron_Head, &ATK_Close_Combat, &ATK_Sacred_Sword, },
		true, false, false, { &ATK_Sacred_Sword, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  639, "Terrakion", TYPE_ROCK, TYPE_FIGHTING, 260, 192, 209, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Smack_Down, &ATK_Double_Kick, &ATK_Earthquake, &ATK_Rock_Slide, &ATK_Close_Combat, &ATK_Sacred_Sword, },
		true, true, false, { &ATK_Sacred_Sword, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  640, "Virizion", TYPE_GRASS, TYPE_FIGHTING, 192, 229, 209, nullptr,
		{ &ATK_Quick_Attack, &ATK_Zen_Headbutt, &ATK_Double_Kick, &ATK_Stone_Edge, &ATK_Leaf_Blade, &ATK_Close_Combat, &ATK_Sacred_Sword, },
		true, false, false, { &ATK_Sacred_Sword, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  641, "Incarnate Form Tornadus", TYPE_FLYING, TYPECOUNT, 266, 164, 188, nullptr,
		{ &ATK_Bite, &ATK_Air_Slash, &ATK_Hyper_Beam, &ATK_Dark_Pulse, &ATK_Hurricane, &ATK_Grass_Knot, },
		true, true, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  641, "Therian Form Tornadus", TYPE_FLYING, TYPECOUNT, 238, 189, 188, nullptr,
		{ &ATK_Astonish, &ATK_Gust, &ATK_Heat_Wave, &ATK_Psychic, &ATK_Hurricane, &ATK_Focus_Blast, &ATK_Bleakwind_Storm, },
		true, false, false, { &ATK_Bleakwind_Storm, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  642, "Incarnate Form Thundurus", TYPE_ELECTRIC, TYPE_FLYING, 266, 164, 188, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Astonish, &ATK_Thunder_Punch, &ATK_Thunder, &ATK_Brick_Break, &ATK_Crunch, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  642, "Therian Form Thundurus", TYPE_ELECTRIC, TYPE_FLYING, 295, 161, 188, nullptr,
		{ &ATK_Bite, &ATK_Volt_Switch, &ATK_Thunder, &ATK_Thunderbolt, &ATK_Sludge_Wave, &ATK_Focus_Blast, &ATK_Wildbolt_Storm, },
		true, false, false, { &ATK_Wildbolt_Storm, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  643, "Reshiram", TYPE_DRAGON, TYPE_FIRE, 275, 211, 205, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Fire_Fang, &ATK_Stone_Edge, &ATK_Overheat, &ATK_Crunch, &ATK_Draco_Meteor, &ATK_Fusion_Flare, },
		true, false, false, { &ATK_Fusion_Flare, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  644, "Zekrom", TYPE_DRAGON, TYPE_ELECTRIC, 275, 211, 205, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Charge_Beam, &ATK_Flash_Cannon, &ATK_Wild_Charge, &ATK_Outrage, &ATK_Crunch, &ATK_Fusion_Bolt, },
		true, false, false, { &ATK_Fusion_Bolt, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  645, "Incarnate Form Landorus", TYPE_GROUND, TYPE_FLYING, 261, 182, 205, nullptr,
		{ &ATK_Mud_Shot, &ATK_Rock_Throw, &ATK_Rock_Slide, &ATK_Focus_Blast, &ATK_Outrage, &ATK_Earth_Power, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  645, "Therian Form Landorus", TYPE_GROUND, TYPE_FLYING, 289, 179, 205, nullptr,
		{ &ATK_Mud_Shot, &ATK_Extrasensory, &ATK_Earthquake, &ATK_Stone_Edge, &ATK_Bulldoze, &ATK_Superpower, &ATK_Sandsear_Storm, },
		true, false, false, { &ATK_Sandsear_Storm, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  646, "Kyurem", TYPE_DRAGON, TYPE_ICE, 246, 170, 245, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Steel_Wing, &ATK_Blizzard, &ATK_Dragon_Claw, &ATK_Draco_Meteor, &ATK_Glaciate, },
		true, false, false, { &ATK_Glaciate, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  646, "White Kyurem", TYPE_DRAGON, TYPE_ICE, 310, 183, 245, "Kyurem",
		{ &ATK_Dragon_Breath, &ATK_Steel_Wing, &ATK_Ice_Fang, &ATK_Blizzard, &ATK_Ancient_Power, &ATK_Dragon_Pulse, &ATK_Focus_Blast, &ATK_Fusion_Flare, &ATK_Ice_Burn, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  646, "Black Kyurem", TYPE_DRAGON, TYPE_ICE, 310, 183, 245, "Kyurem",
		{ &ATK_Shadow_Claw, &ATK_Dragon_Tail, &ATK_Stone_Edge, &ATK_Blizzard, &ATK_Iron_Head, &ATK_Outrage, &ATK_Fusion_Bolt, &ATK_Freeze_Shock, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // the colt
  {  647, "Keldeo", TYPE_WATER, TYPE_FIGHTING, 260, 192, 209, nullptr,
		{ &ATK_Low_Kick, &ATK_Poison_Jab, &ATK_Aqua_Jet, &ATK_X_Scissor, &ATK_Hydro_Pump, &ATK_Close_Combat, &ATK_Sacred_Sword, },
		false, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  // no difference in PGO
  /*{  647, "Keldeo (Resolute Forme)", TYPE_WATER, TYPE_FIGHTING, 260, 192, 209, nullptr,
		{ &ATK_Low_Kick, &ATK_Poison_Jab, &ATK_Aqua_Jet, &ATK_X_Scissor, &ATK_Hydro_Pump, &ATK_Close_Combat, &ATK_Sacred_Sword, },
		false, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },*/
  {  648, "Aria Forme Meloetta", TYPE_NORMAL, TYPE_PSYCHIC, 250, 225, 225, nullptr,
		{ &ATK_Low_Kick, &ATK_Quick_Attack, &ATK_Hyper_Beam, &ATK_Ice_Punch, &ATK_Fire_Punch, &ATK_Close_Combat, },
		false, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  // not yet released
  /*{  648, "Pirouette Forme Meloetta", TYPE_NORMAL, TYPE_FIGHTING, 269, 188, 225, nullptr,
		{ &ATK_Low_Kick, &ATK_Quick_Attack, &ATK_Hyper_Beam, &ATK_Ice_Punch, &ATK_Fire_Punch, &ATK_Close_Combat, },
		false, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },*/
  {  649, "Genesect", TYPE_BUG, TYPE_STEEL, 252, 199, 174, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Metal_Claw, &ATK_Hyper_Beam, &ATK_Magnet_Bomb, &ATK_X_Scissor, &ATK_Techno_Blast, },
		true, false, false, { &ATK_Techno_Blast, }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  650, "Chespin", TYPE_GRASS, TYPECOUNT, 110, 106, 148, nullptr,
		{ &ATK_Vine_Whip, &ATK_Take_Down, &ATK_Seed_Bomb, &ATK_Body_Slam, &ATK_Gyro_Ball, },
		true, false, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  651, "Quilladin", TYPE_GRASS, TYPECOUNT, 146, 156, 156, "Chespin",
		{ &ATK_Low_Kick, &ATK_Vine_Whip, &ATK_Body_Slam, &ATK_Gyro_Ball, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  652, "Chesnaught", TYPE_GRASS, TYPE_FIGHTING, 201, 204, 204, "Quilladin",
		{ &ATK_Low_Kick, &ATK_Vine_Whip, &ATK_Smack_Down, &ATK_Thunder_Punch, &ATK_Solar_Beam, &ATK_Gyro_Ball, &ATK_Energy_Ball, &ATK_Frenzy_Plant, &ATK_Superpower, },
		true, false, false, { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  653, "Fennekin", TYPE_FIRE, TYPECOUNT, 116, 102, 120, nullptr,
		{ &ATK_Ember, &ATK_Scratch, &ATK_Flamethrower, &ATK_Psyshock, &ATK_Flame_Charge, },
		true, false, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  654, "Braixen", TYPE_FIRE, TYPECOUNT, 171, 130, 153, "Fennekin",
		{ &ATK_Ember, &ATK_Scratch, &ATK_Flamethrower, &ATK_Psyshock, &ATK_Flame_Charge, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  655, "Delphox", TYPE_FIRE, TYPE_PSYCHIC, 230, 189, 181, "Braixen",
		{ &ATK_Scratch, &ATK_Zen_Headbutt, &ATK_Fire_Spin, &ATK_Psyshock, &ATK_Flamethrower, &ATK_Flame_Charge, &ATK_Fire_Blast, &ATK_Psychic, &ATK_Blast_Burn, &ATK_Mystical_Fire, },
		true, false, false, { &ATK_Blast_Burn, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  656, "Froakie", TYPE_WATER, TYPECOUNT, 122, 84, 121, nullptr,
		{ &ATK_Pound, &ATK_Bubble, &ATK_Aerial_Ace, &ATK_Water_Pulse, &ATK_Surf, },
		true, false, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  657, "Frogadier", TYPE_WATER, TYPECOUNT, 168, 114, 144, "Froakie",
		{ &ATK_Pound, &ATK_Bubble, &ATK_Aerial_Ace, &ATK_Water_Pulse, &ATK_Surf, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  658, "Greninja", TYPE_WATER, TYPE_DARK, 223, 152, 176, "Frogadier",
		{ &ATK_Bubble, &ATK_Feint_Attack, &ATK_Water_Shuriken, &ATK_Aerial_Ace, &ATK_Night_Slash, &ATK_Hydro_Pump, &ATK_Surf, &ATK_Hydro_Cannon, },
		true, false, false, { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  659, "Bunnelby", TYPE_NORMAL, TYPECOUNT, 68, 72, 116, nullptr,
		{ &ATK_Quick_Attack, &ATK_Mud_Slap, &ATK_Dig, &ATK_Earthquake, &ATK_Bulldoze, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  660, "Diggersby", TYPE_NORMAL, TYPE_GROUND, 112, 155, 198, "Bunnelby",
		{ &ATK_Mud_Shot, &ATK_Quick_Attack, &ATK_Hyper_Beam, &ATK_Dig, &ATK_Earthquake, &ATK_Fire_Punch, &ATK_Scorching_Sands, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  661, "Fletchling", TYPE_NORMAL, TYPE_FLYING, 95, 80, 128, nullptr,
		{ &ATK_Peck, &ATK_Quick_Attack, &ATK_Heat_Wave, &ATK_Aerial_Ace, &ATK_Swift, &ATK_Fly, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  662, "Fletchinder", TYPE_FIRE, TYPE_FLYING, 145, 110, 158, "Fletchling",
		{ &ATK_Ember, &ATK_Peck, &ATK_Steel_Wing, &ATK_Heat_Wave, &ATK_Aerial_Ace, &ATK_Flame_Charge, &ATK_Fly, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  663, "Talonflame", TYPE_FIRE, TYPE_FLYING, 176, 155, 186, "Fletchinder",
		{ &ATK_Peck, &ATK_Steel_Wing, &ATK_Fire_Spin, &ATK_Incinerate, &ATK_Flame_Charge, &ATK_Fire_Blast, &ATK_Hurricane, &ATK_Brave_Bird, &ATK_Fly, },
		true, true, false, { &ATK_Incinerate, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  664, "Scatterbug", TYPE_BUG, TYPECOUNT, 63, 63, 116, nullptr,
		{ &ATK_Bug_Bite, &ATK_Tackle, &ATK_Struggle, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  665, "Spewpa", TYPE_BUG, TYPECOUNT, 48, 89, 128, "Scatterbug",
		{ &ATK_Struggle_Bug, &ATK_Struggle, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  666, "Vivillon", TYPE_BUG, TYPE_FLYING, 176, 103, 190, "Spewpa",
		{ &ATK_Struggle_Bug, &ATK_Gust, &ATK_Aerial_Ace, &ATK_Bug_Buzz, &ATK_Hurricane, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  667, "Litleo", TYPE_FIRE, TYPE_NORMAL, 139, 112, 158, nullptr,
		{ &ATK_Ember, &ATK_Tackle, &ATK_Fire_Fang, &ATK_Incinerate, &ATK_Flamethrower, &ATK_Flame_Charge, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  668, "Pyroar", TYPE_FIRE, TYPE_NORMAL, 221, 149, 200, "Litleo",
		{ &ATK_Ember, &ATK_Fire_Fang, &ATK_Take_Down, &ATK_Incinerate, &ATK_Dark_Pulse, &ATK_Flame_Charge, &ATK_Solar_Beam, &ATK_Overheat, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  // the single bloom
  {  669, "Flabébé", TYPE_FAIRY, TYPECOUNT, 108, 120, 127, nullptr,
		{ &ATK_Vine_Whip, &ATK_Tackle, &ATK_Petal_Blizzard, &ATK_Dazzling_Gleam, &ATK_Psychic, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the single bloom
  {  670, "Floette", TYPE_FAIRY, TYPECOUNT, 136, 151, 144, "Flabébé",
		{ &ATK_Vine_Whip, &ATK_Tackle, &ATK_Petal_Blizzard, &ATK_Dazzling_Gleam, &ATK_Psychic, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the garden
  {  671, "Florges", TYPE_FAIRY, TYPECOUNT, 212, 244, 186, "Floette",
		{ &ATK_Vine_Whip, &ATK_Razor_Leaf, &ATK_Tackle, &ATK_Fairy_Wind, &ATK_Petal_Blizzard, &ATK_Disarming_Voice, &ATK_Moonblast, &ATK_Psychic, &ATK_Trailblaze, &ATK_Chilling_Water, },
		true, false, false, { &ATK_Chilling_Water, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the mount
  {  672, "Skiddo", TYPE_GRASS, TYPECOUNT, 123, 102, 165, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Rock_Smash, &ATK_Seed_Bomb, &ATK_Brick_Break, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the mount
  {  673, "Gogoat", TYPE_GRASS, TYPECOUNT, 196, 146, 265, "Skiddo",
		{ &ATK_Vine_Whip, &ATK_Zen_Headbutt, &ATK_Rock_Smash, &ATK_Seed_Bomb, &ATK_Leaf_Blade, &ATK_Brick_Break, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the playful
  {  674, "Pancham", TYPE_FIGHTING, TYPECOUNT, 145, 107, 167, nullptr,
		{ &ATK_Low_Kick, &ATK_Tackle, &ATK_Low_Sweep, &ATK_Body_Slam, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the daunting
  {  675, "Pangoro", TYPE_FIGHTING, TYPE_DARK, 226, 146, 216, "Pancham",
		{ &ATK_Low_Kick, &ATK_Karate_Chop, &ATK_Bullet_Punch, &ATK_Snarl, &ATK_Night_Slash, &ATK_Rock_Slide, &ATK_Iron_Head, &ATK_Close_Combat, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the poodle
  {  676, "Furfrou", TYPE_NORMAL, TYPECOUNT, 164, 167, 181, nullptr,
		{ &ATK_Bite, &ATK_Sucker_Punch, &ATK_Take_Down, &ATK_Sand_Attack, &ATK_Dark_Pulse, &ATK_Grass_Knot, &ATK_Surf, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the restraint
  {  677, "Espurr", TYPE_PSYCHIC, TYPECOUNT, 120, 114, 158, nullptr,
		{ &ATK_Scratch, &ATK_Confusion, &ATK_Psyshock, &ATK_Psychic, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the constraint
  {  678, "Meowstic", TYPE_PSYCHIC, TYPECOUNT, 166, 167, 179, "Espurr",
		{ &ATK_Sucker_Punch, &ATK_Confusion, &ATK_Thunderbolt, &ATK_Psychic, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the sword
  {  679, "Honedge", TYPE_STEEL, TYPE_GHOST, 135, 139, 128, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Tackle, &ATK_Iron_Head, &ATK_Gyro_Ball, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the sword
  {  680, "Doublade", TYPE_STEEL, TYPE_GHOST, 188, 206, 153, "Honedge",
		{ &ATK_Fury_Cutter, &ATK_Psycho_Cut, &ATK_Iron_Head, &ATK_Gyro_Ball, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  /*{  681, "Aegislash (Blade)", TYPE_STEEL, TYPE_GHOST, 291, 97, 155, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Psycho_Cut, &ATK_Flash_Cannon, &ATK_Shadow_Ball, &ATK_Gyro_Ball, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },*/
  // the royal sword
  {  681, "Aegislash", TYPE_STEEL, TYPE_GHOST, 97, 291, 155, "Doublade",
		{ &ATK_Fury_Cutter, &ATK_Psycho_Cut, &ATK_Flash_Cannon, &ATK_Shadow_Ball, &ATK_Gyro_Ball, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the perfume
  {  682, "Spritzee", TYPE_FAIRY, TYPECOUNT, 110, 113, 186, nullptr,
		{ &ATK_Charge_Beam, &ATK_Charm, &ATK_Thunderbolt, &ATK_Draining_Kiss, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the fragrance
  {  683, "Aromatisse", TYPE_FAIRY, TYPECOUNT, 173, 150, 226, "Spritzee",
		{ &ATK_Charge_Beam, &ATK_Charm, &ATK_Thunderbolt, &ATK_Psyshock, &ATK_Draining_Kiss, &ATK_Moonblast, &ATK_Psychic, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the cotton candy
  {  684, "Swirlix", TYPE_FAIRY, TYPECOUNT, 109, 119, 158, nullptr,
		{ &ATK_Tackle, &ATK_Draining_Kiss, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the meringue
  {  685, "Slurpuff", TYPE_FAIRY, TYPECOUNT, 168, 163, 193, "Swirlix",
		{ &ATK_Tackle, &ATK_Charm, &ATK_Fairy_Wind, &ATK_Flamethrower, &ATK_Draining_Kiss, &ATK_Play_Rough, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the revolving
  {  686, "Inkay", TYPE_DARK, TYPE_PSYCHIC, 98, 95, 142, nullptr,
		{ &ATK_Peck, &ATK_Tackle, &ATK_Psywave, &ATK_Psybeam, &ATK_Night_Slash, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the overturning
  {  687, "Malamar", TYPE_DARK, TYPE_PSYCHIC, 177, 165, 200, "Inkay",
		{ &ATK_Peck, &ATK_Psycho_Cut, &ATK_Psywave, &ATK_Hyper_Beam, &ATK_Psybeam, &ATK_Foul_Play, &ATK_Superpower, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the two-handed
  {  688, "Binacle", TYPE_ROCK, TYPE_WATER, 96, 120, 123, nullptr,
		{ &ATK_Scratch, &ATK_Mud_Slap, &ATK_Dig, &ATK_Cross_Chop, &ATK_Ancient_Power, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the collective
  {  689, "Barbaracle", TYPE_ROCK, TYPE_WATER, 194, 205, 176, "Binacle",
		{ &ATK_Fury_Cutter, &ATK_Water_Gun, &ATK_Mud_Slap, &ATK_Cross_Chop, &ATK_Stone_Edge, &ATK_Grass_Knot, &ATK_Skull_Bash, &ATK_Razor_Shell, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the mock kelp
  {  690, "Skrelp", TYPE_POISON, TYPE_WATER, 109, 109, 137, nullptr,
		{ &ATK_Acid, &ATK_Water_Gun, &ATK_Aqua_Tail, &ATK_Twister, &ATK_Sludge_Bomb, &ATK_Water_Pulse, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the mock kelp
  {  691, "Dragalge", TYPE_POISON, TYPE_DRAGON, 177, 207, 163, "Skrelp",
		{ &ATK_Acid, &ATK_Water_Gun, &ATK_Dragon_Tail, &ATK_Aqua_Tail, &ATK_Sludge_Bomb, &ATK_Gunk_Shot, &ATK_Hydro_Pump, &ATK_Focus_Blast, &ATK_Outrage, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  692, "Clauncher", TYPE_WATER, TYPECOUNT, 108, 117, 137, nullptr,
		{ &ATK_Water_Gun, &ATK_Smack_Down, &ATK_Aqua_Jet, &ATK_Water_Pulse, &ATK_Crabhammer, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  693, "Clawitzer", TYPE_WATER, TYPECOUNT, 221, 171, 174, "Clauncher",
		{ &ATK_Water_Gun, &ATK_Smack_Down, &ATK_Dark_Pulse, &ATK_Ice_Beam, &ATK_Water_Pulse, &ATK_Crabhammer, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  694, "Helioptile", TYPE_ELECTRIC, TYPE_NORMAL, 115, 78, 127, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Quick_Attack, &ATK_Parabolic_Charge, &ATK_Thunderbolt, &ATK_Bulldoze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  695, "Heliolisk", TYPE_ELECTRIC, TYPE_NORMAL, 219, 168, 158, "Helioptile",
		{ &ATK_Quick_Attack, &ATK_Mud_Slap, &ATK_Volt_Switch, &ATK_Parabolic_Charge, &ATK_Thunderbolt, &ATK_Bulldoze, &ATK_Grass_Knot, &ATK_Breaking_Swipe, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_SUNSTONE, },
  {  696, "Tyrunt", TYPE_ROCK, TYPE_DRAGON, 158, 123, 151, nullptr,
		{ &ATK_Tackle, &ATK_Dragon_Tail, &ATK_Rock_Tomb, &ATK_Ancient_Power, &ATK_Dragon_Claw, &ATK_Stomp, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  697, "Tyrantrum", TYPE_ROCK, TYPE_DRAGON, 227, 191, 193, "Tyrunt",
		{ &ATK_Rock_Throw, &ATK_Dragon_Tail, &ATK_Charm, &ATK_Rock_Tomb, &ATK_Earthquake, &ATK_Stone_Edge, &ATK_Outrage, &ATK_Crunch, &ATK_Meteor_Beam, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  698, "Amaura", TYPE_ROCK, TYPE_ICE, 124, 109, 184, nullptr,
		{ &ATK_Frost_Breath, &ATK_Powder_Snow, &ATK_Ancient_Power, &ATK_Thunderbolt, &ATK_Aurora_Beam, &ATK_Weather_Ball_Ice, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  699, "Aurorus", TYPE_ROCK, TYPE_ICE, 186, 163, 265, "Amaura",
		{ &ATK_Frost_Breath, &ATK_Rock_Throw, &ATK_Powder_Snow, &ATK_Hyper_Beam, &ATK_Blizzard, &ATK_Ancient_Power, &ATK_Thunderbolt, &ATK_Weather_Ball_Ice, &ATK_Meteor_Beam, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  700, "Sylveon", TYPE_FAIRY, TYPECOUNT, 203, 205, 216, "Eevee",
		{ &ATK_Quick_Attack, &ATK_Charm, &ATK_Psyshock, &ATK_Draining_Kiss, &ATK_Dazzling_Gleam, &ATK_Moonblast, &ATK_Last_Resort, },
		true, false, false, { &ATK_Last_Resort, &ATK_Psyshock, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  701, "Hawlucha", TYPE_FIGHTING, TYPE_FLYING, 195, 153, 186, nullptr,
		{ &ATK_Low_Kick, &ATK_Poison_Jab, &ATK_Aerial_Ace, &ATK_X_Scissor, &ATK_Power_Up_Punch, &ATK_Flying_Press, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  702, "Dedenne", TYPE_ELECTRIC, TYPE_FAIRY, 164, 134, 167, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Tackle, &ATK_Discharge, &ATK_Parabolic_Charge, &ATK_Play_Rough, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  703, "Carbink", TYPE_ROCK, TYPE_FAIRY, 95, 285, 137, nullptr,
		{ &ATK_Tackle, &ATK_Rock_Throw, &ATK_Rock_Slide, &ATK_Power_Gem, &ATK_Moonblast, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  704, "Goomy", TYPE_DRAGON, TYPECOUNT, 101, 112, 128, nullptr,
		{ &ATK_Tackle, &ATK_Water_Gun, &ATK_Dragon_Pulse, &ATK_Sludge_Wave, &ATK_Muddy_Water, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  705, "Sliggoo", TYPE_DRAGON, TYPECOUNT, 159, 176, 169, "Goomy",
		{ &ATK_Tackle, &ATK_Water_Gun, &ATK_Dragon_Pulse, &ATK_Sludge_Wave, &ATK_Water_Pulse, &ATK_Muddy_Water, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  706, "Goodra", TYPE_DRAGON, TYPECOUNT, 220, 242, 207, "Sliggoo",
		{ &ATK_Dragon_Breath, &ATK_Water_Gun, &ATK_Aqua_Tail, &ATK_Thunder_Punch, &ATK_Sludge_Wave, &ATK_Power_Whip, &ATK_Draco_Meteor, &ATK_Muddy_Water, },
		true, false, false, { &ATK_Thunder_Punch, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  707, "Klefki", TYPE_STEEL, TYPE_FAIRY, 160, 179, 149, nullptr,
		{ &ATK_Tackle, &ATK_Astonish, &ATK_Flash_Cannon, &ATK_Draining_Kiss, &ATK_Play_Rough, &ATK_Foul_Play, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  708, "Phantump", TYPE_GHOST, TYPE_GRASS, 125, 103, 125, nullptr,
		{ &ATK_Tackle, &ATK_Astonish, &ATK_Seed_Bomb, &ATK_Shadow_Ball, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  709, "Trevenant", TYPE_GHOST, TYPE_GRASS, 201, 154, 198, "Phantump",
		{ &ATK_Sucker_Punch, &ATK_Shadow_Claw, &ATK_Seed_Bomb, &ATK_Shadow_Ball, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  710, "Pumpkaboo", TYPE_GHOST, TYPE_GRASS, 121, 123, 135, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Astonish, &ATK_Shadow_Sneak, &ATK_Grass_Knot, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  710, "Pumpkaboo Small", TYPE_GHOST, TYPE_GRASS, 122, 124, 127, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Astonish, &ATK_Shadow_Sneak, &ATK_Grass_Knot, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  710, "Pumpkaboo Large", TYPE_GHOST, TYPE_GRASS, 120, 122, 144, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Astonish, &ATK_Shadow_Sneak, &ATK_Grass_Knot, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  710, "Pumpkaboo Super", TYPE_GHOST, TYPE_GRASS, 118, 120, 153, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Astonish, &ATK_Shadow_Sneak, &ATK_Grass_Knot, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  711, "Gourgeist", TYPE_GHOST, TYPE_GRASS, 175, 213, 163, "Pumpkaboo",
		{ &ATK_Razor_Leaf, &ATK_Hex, &ATK_Seed_Bomb, &ATK_Shadow_Punch, &ATK_Fire_Blast, &ATK_Foul_Play, &ATK_Poltergeist, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  711, "Gourgeist Small", TYPE_GHOST, TYPE_GRASS, 171, 219, 146, "Pumpkaboo Small",
		{ &ATK_Razor_Leaf, &ATK_Hex, &ATK_Seed_Bomb, &ATK_Shadow_Punch, &ATK_Fire_Blast, &ATK_Foul_Play, &ATK_Poltergeist, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  711, "Gourgeist Large", TYPE_GHOST, TYPE_GRASS, 179, 206, 181, "Pumpkaboo Large",
		{ &ATK_Razor_Leaf, &ATK_Hex, &ATK_Seed_Bomb, &ATK_Shadow_Punch, &ATK_Fire_Blast, &ATK_Foul_Play, &ATK_Poltergeist, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  711, "Gourgeist Super", TYPE_GHOST, TYPE_GRASS, 182, 200, 198, "Pumpkaboo Super",
		{ &ATK_Razor_Leaf, &ATK_Hex, &ATK_Seed_Bomb, &ATK_Shadow_Punch, &ATK_Fire_Blast, &ATK_Foul_Play, &ATK_Poltergeist, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  712, "Bergmite", TYPE_ICE, TYPECOUNT, 117, 120, 146, nullptr,
		{ &ATK_Bite, &ATK_Tackle, &ATK_Icy_Wind, &ATK_Mirror_Coat, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  713, "Avalugg", TYPE_ICE, TYPECOUNT, 196, 240, 216, "Bergmite",
		{ &ATK_Bite, &ATK_Ice_Fang, &ATK_Earthquake, &ATK_Icy_Wind, &ATK_Body_Slam, &ATK_Avalanche, &ATK_Mirror_Coat, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  713, "Hisuian Avalugg", TYPE_ICE, TYPE_ROCK, 214, 238, 216, nullptr,
		{ &ATK_Bite, &ATK_Tackle, &ATK_Powder_Snow, &ATK_Blizzard, &ATK_Rock_Slide, &ATK_Icy_Wind, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  714, "Noibat", TYPE_FLYING, TYPE_DRAGON, 83, 73, 120, nullptr,
		{ &ATK_Bite, &ATK_Wing_Attack, &ATK_Heat_Wave, &ATK_Dragon_Pulse, &ATK_Air_Cutter, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  715, "Noivern", TYPE_FLYING, TYPE_DRAGON, 205, 175, 198, "Noibat",
		{ &ATK_Bite, &ATK_Air_Slash, &ATK_Heat_Wave, &ATK_Psychic, &ATK_Hurricane, &ATK_Draco_Meteor, &ATK_Boomburst, },
		true, false, false, { &ATK_Boomburst, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  716, "Xerneas", TYPE_FAIRY, TYPECOUNT, 250, 185, 246, nullptr,
		{ &ATK_Tackle, &ATK_Zen_Headbutt, &ATK_Geomancy, &ATK_Megahorn, &ATK_Thunder, &ATK_Moonblast, &ATK_Close_Combat, &ATK_Giga_Impact, },
		true, false, false, { &ATK_Geomancy, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  717, "Yveltal", TYPE_DARK, TYPE_FLYING, 250, 185, 246, nullptr,
		{ &ATK_Sucker_Punch, &ATK_Snarl, &ATK_Gust, &ATK_Hyper_Beam, &ATK_Dark_Pulse, &ATK_Psychic, &ATK_Hurricane, &ATK_Focus_Blast, &ATK_Oblivion_Wing, },
		true, false, false, { &ATK_Oblivion_Wing, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  718, "Zygarde 10%", TYPE_DRAGON, TYPE_GROUND, 205, 173, 144, nullptr,
		{ &ATK_Bite, &ATK_Zen_Headbutt, &ATK_Dragon_Tail, &ATK_Hyper_Beam, &ATK_Earthquake, &ATK_Bulldoze, &ATK_Outrage, &ATK_Crunch, },
		false, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  718, "Zygarde 50%", TYPE_DRAGON, TYPE_GROUND, 203, 232, 239, "Zygarde 10%",
		{ &ATK_Bite, &ATK_Zen_Headbutt, &ATK_Dragon_Tail, &ATK_Hyper_Beam, &ATK_Earthquake, &ATK_Bulldoze, &ATK_Outrage, &ATK_Crunch, },
		false, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_ZYGARDECELL, },
  {  718, "Zygarde Complete", TYPE_DRAGON, TYPE_GROUND, 184, 207, 389, "Zygarde 50%",
		{ &ATK_Bite, &ATK_Zen_Headbutt, &ATK_Dragon_Tail, &ATK_Hyper_Beam, &ATK_Earthquake, &ATK_Bulldoze, &ATK_Outrage, &ATK_Crunch, },
		false, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_ZYGARDECELL, },
  {  719, "Diancie", TYPE_ROCK, TYPE_FAIRY, 190, 285, 137, nullptr,
		{ &ATK_Tackle, &ATK_Rock_Throw, &ATK_Rock_Slide, &ATK_Power_Gem, &ATK_Moonblast, },
		true, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  720, "Hoopa Confined", TYPE_PSYCHIC, TYPE_GHOST, 261, 187, 173, nullptr,
		{ &ATK_Confusion, &ATK_Astonish, &ATK_Psybeam, &ATK_Shadow_Ball, &ATK_Psychic, },
		false, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  720, "Hoopa Unbound", TYPE_PSYCHIC, TYPE_DARK, 311, 191, 173, nullptr,
		{ &ATK_Confusion, &ATK_Astonish, &ATK_Dark_Pulse, &ATK_Shadow_Ball, &ATK_Psychic, },
		false, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  721, "Volcanion", TYPE_FIRE, TYPE_WATER, 252, 216, 190, nullptr,
		{ &ATK_Water_Gun, &ATK_Take_Down, &ATK_Incinerate, &ATK_Sludge_Bomb, &ATK_Hydro_Pump, &ATK_Overheat, &ATK_Earth_Power, },
		false, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  722, "Rowlet", TYPE_GRASS, TYPE_FLYING, 102, 99, 169, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Tackle, &ATK_Magical_Leaf, &ATK_Leafage, &ATK_Seed_Bomb, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  723, "Dartrix", TYPE_GRASS, TYPE_FLYING, 142, 139, 186, "Rowlet",
		{ &ATK_Peck, &ATK_Razor_Leaf, &ATK_Magical_Leaf, &ATK_Leafage, &ATK_Seed_Bomb, &ATK_Brave_Bird, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  724, "Decidueye", TYPE_GRASS, TYPE_GHOST, 210, 179, 186, "Dartrix",
		{ &ATK_Razor_Leaf, &ATK_Astonish, &ATK_Magical_Leaf, &ATK_Leafage, &ATK_Shadow_Sneak, &ATK_Brave_Bird, &ATK_Energy_Ball, &ATK_Frenzy_Plant, &ATK_Spirit_Shackle, },
		true, false, false, { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  724, "Hisuian Decidueye", TYPE_GRASS, TYPE_FIGHTING, 213, 174, 204, nullptr,
		{ &ATK_Psycho_Cut, &ATK_Magical_Leaf, &ATK_Aerial_Ace, &ATK_Night_Shade, &ATK_Energy_Ball, &ATK_Aura_Sphere, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  725, "Litten", TYPE_FIRE, TYPECOUNT, 128, 79, 128, nullptr,
		{ &ATK_Ember, &ATK_Scratch, &ATK_Flamethrower, &ATK_Flame_Charge, &ATK_Crunch, },
		true, false, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  726, "Torracat", TYPE_FIRE, TYPECOUNT, 174, 103, 163, "Litten",
		{ &ATK_Bite, &ATK_Ember, &ATK_Flamethrower, &ATK_Flame_Charge, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  727, "Incineroar", TYPE_FIRE, TYPE_DARK, 214, 175, 216, "Torracat",
		{ &ATK_Fire_Fang, &ATK_Snarl, &ATK_Double_Kick, &ATK_Dark_Pulse, &ATK_Drain_Punch, &ATK_Brutal_Swing, &ATK_Flame_Charge, &ATK_Fire_Blast, &ATK_Blast_Burn, &ATK_Blaze_Kick, &ATK_Darkest_Lariat, },
		true, false, false, { &ATK_Blast_Burn, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  728, "Popplio", TYPE_WATER, TYPECOUNT, 120, 103, 137, nullptr,
		{ &ATK_Pound, &ATK_Water_Gun, &ATK_Aqua_Jet, &ATK_Aqua_Tail, &ATK_Water_Pulse, },
		true, false, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  729, "Brionne", TYPE_WATER, TYPECOUNT, 168, 145, 155, "Popplio",
		{ &ATK_Water_Gun, &ATK_Charm, &ATK_Aqua_Jet, &ATK_Disarming_Voice, &ATK_Water_Pulse, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  730, "Primarina", TYPE_WATER, TYPE_FAIRY, 232, 195, 190, "Brionne",
		{ &ATK_Waterfall, &ATK_Charm, &ATK_Disarming_Voice, &ATK_Moonblast, &ATK_Hydro_Pump, &ATK_Psychic, &ATK_Hydro_Cannon, &ATK_Sparkling_Aria, },
		true, false, false, { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  731, "Pikipek", TYPE_NORMAL, TYPE_FLYING, 136, 59, 111, nullptr,
		{ &ATK_Peck, &ATK_Rock_Smash, &ATK_Drill_Peck, &ATK_Aerial_Ace, &ATK_Sky_Attack, },
		false, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  732, "Trumbeak", TYPE_NORMAL, TYPE_FLYING, 159, 100, 146, "Pikipek",
		{ &ATK_Peck, &ATK_Rock_Smash, &ATK_Drill_Peck, &ATK_Sky_Attack, &ATK_Rock_Blast, },
		false, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  733, "Toucannon", TYPE_NORMAL, TYPE_FLYING, 222, 146, 190, "Trumbeak",
		{ &ATK_Peck, &ATK_Rock_Smash, &ATK_Bullet_Seed, &ATK_Flash_Cannon, &ATK_Drill_Peck, &ATK_Rock_Blast, },
		false, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  734, "Yungoos", TYPE_NORMAL, TYPECOUNT, 122, 56, 134, nullptr,
		{ &ATK_Bite, &ATK_Tackle, &ATK_Rock_Tomb, &ATK_Hyper_Fang, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  735, "Gumshoos", TYPE_NORMAL, TYPECOUNT, 194, 113, 204, "Yungoos",
		{ &ATK_Bite, &ATK_Take_Down, &ATK_Rock_Tomb, &ATK_Hyper_Fang, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  736, "Grubbin", TYPE_BUG, TYPECOUNT, 115, 85, 132, nullptr,
		{ &ATK_Bug_Bite, &ATK_Spark, &ATK_Vise_Grip, &ATK_X_Scissor, &ATK_Crunch, },
		true, true, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  737, "Charjabug", TYPE_BUG, TYPE_ELECTRIC, 145, 161, 149, "Grubbin",
		{ &ATK_Bug_Bite, &ATK_Spark, &ATK_Volt_Switch, &ATK_Discharge, &ATK_X_Scissor, &ATK_Crunch, },
		true, true, false, { &ATK_Volt_Switch, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  738, "Vikavolt", TYPE_BUG, TYPE_ELECTRIC, 254, 158, 184, "Charjabug",
		{ &ATK_Bug_Bite, &ATK_Spark, &ATK_Mud_Slap, &ATK_Volt_Switch, &ATK_Discharge, &ATK_X_Scissor, &ATK_Crunch, &ATK_Fly, },
		true, true, false, { &ATK_Volt_Switch, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  739, "Crabrawler", TYPE_FIGHTING, TYPECOUNT, 150, 104, 132, nullptr,
		{ &ATK_Bubble, &ATK_Rock_Smash, &ATK_Brick_Break, &ATK_Crabhammer, &ATK_Power_Up_Punch, &ATK_Payback, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  740, "Crabominable", TYPE_FIGHTING, TYPE_ICE, 231, 138, 219, "Crabrawler",
		{ &ATK_Bubble, &ATK_Rock_Smash, &ATK_Ice_Punch, &ATK_Brick_Break, &ATK_Crabhammer, &ATK_Power_Up_Punch, &ATK_Payback, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  741, "Oricorio (Baile)", TYPE_FIRE, TYPE_FLYING, 196, 145, 181, nullptr,
		{ &ATK_Pound, &ATK_Air_Slash, &ATK_Aerial_Ace, &ATK_Air_Cutter, &ATK_Hurricane, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  741, "Oricorio (Pom-Pom)", TYPE_ELECTRIC, TYPE_FLYING, 196, 145, 181, nullptr,
		{ &ATK_Pound, &ATK_Air_Slash, &ATK_Aerial_Ace, &ATK_Air_Cutter, &ATK_Hurricane, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  741, "Oricorio (Pa'u)", TYPE_PSYCHIC, TYPE_FLYING, 196, 145, 181, nullptr,
		{ &ATK_Pound, &ATK_Air_Slash, &ATK_Aerial_Ace, &ATK_Air_Cutter, &ATK_Hurricane, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  741, "Oricorio (Sensu)", TYPE_GHOST, TYPE_FLYING, 196, 145, 181, nullptr,
		{ &ATK_Pound, &ATK_Air_Slash, &ATK_Aerial_Ace, &ATK_Air_Cutter, &ATK_Hurricane, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  742, "Cutiefly", TYPE_BUG, TYPE_FAIRY, 110, 81, 120, nullptr,
		{ &ATK_Struggle_Bug, &ATK_Fairy_Wind, &ATK_Bug_Buzz, &ATK_Dazzling_Gleam, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  743, "Ribombee", TYPE_BUG, TYPE_FAIRY, 198, 146, 155, "Cutiefly",
		{ &ATK_Struggle_Bug, &ATK_Charm, &ATK_Fairy_Wind, &ATK_Bug_Buzz, &ATK_Draining_Kiss, &ATK_Dazzling_Gleam, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  744, "Rockruff", TYPE_ROCK, TYPECOUNT, 117, 78, 128, nullptr,
		{ &ATK_Tackle, &ATK_Rock_Throw, &ATK_Stone_Edge, &ATK_Rock_Tomb, &ATK_Crunch, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  745, "Lycanroc Midday", TYPE_ROCK, TYPECOUNT, 231, 140, 181, "Rockruff",
		{ &ATK_Sucker_Punch, &ATK_Rock_Throw, &ATK_Stone_Edge, &ATK_Drill_Run, &ATK_Crunch, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  745, "Lycanroc Midnight", TYPE_ROCK, TYPECOUNT, 218, 152, 198, "Rockruff",
		{ &ATK_Rock_Throw, &ATK_Counter, &ATK_Stone_Edge, &ATK_Crunch, &ATK_Psychic_Fangs, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  745, "Lycanroc Dusk", TYPE_ROCK, TYPECOUNT, 234, 139, 181, "Rockruff",
		{ &ATK_Sucker_Punch, &ATK_Rock_Throw, &ATK_Counter, &ATK_Stone_Edge, &ATK_Iron_Head, &ATK_Crunch, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // not yet released
  /* {  746, "Wishiwashi", TYPE_WATER, TYPECOUNT, 46, 43, 128, nullptr,
		{ &ATK_Water_Gun, &ATK_Iron_Tail, &ATK_Waterfall, &ATK_Aqua_Tail, &ATK_Brine, &ATK_Surf, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },*/
  {  747, "Mareanie", TYPE_POISON, TYPE_WATER, 98, 110, 137, nullptr,
		{ &ATK_Bite, &ATK_Poison_Sting, &ATK_Sludge_Wave, &ATK_Gunk_Shot, &ATK_Brine, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  748, "Toxapex", TYPE_POISON, TYPE_WATER, 114, 273, 137, "Mareanie",
		{ &ATK_Bite, &ATK_Poison_Jab, &ATK_Sludge_Wave, &ATK_Gunk_Shot, &ATK_Brine, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  749, "Mudbray", TYPE_GROUND, TYPECOUNT, 175, 121, 172, nullptr,
		{ &ATK_Mud_Slap, &ATK_Rock_Smash, &ATK_Earthquake, &ATK_Bulldoze, &ATK_Body_Slam, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  750, "Mudsdale", TYPE_GROUND, TYPECOUNT, 214, 174, 225, "Mudbray",
		{ &ATK_Mud_Slap, &ATK_Rock_Smash, &ATK_Earthquake, &ATK_Bulldoze, &ATK_Body_Slam, &ATK_Heavy_Slam, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  751, "Dewpider", TYPE_WATER, TYPE_BUG, 72, 117, 116, nullptr,
		{ &ATK_Bug_Bite, &ATK_Infestation, &ATK_Bubble_Beam, &ATK_Water_Pulse, &ATK_Mirror_Coat, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  752, "Araquanid", TYPE_WATER, TYPE_BUG, 126, 219, 169, "Dewpider",
		{ &ATK_Bug_Bite, &ATK_Infestation, &ATK_Bug_Buzz, &ATK_Bubble_Beam, &ATK_Water_Pulse, &ATK_Mirror_Coat, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  753, "Fomantis", TYPE_GRASS, TYPECOUNT, 100, 64, 120, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Razor_Leaf, &ATK_Leafage, &ATK_Leaf_Blade, &ATK_Grass_Knot, &ATK_Energy_Ball, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  754, "Lurantis", TYPE_GRASS, TYPECOUNT, 192, 169, 172, "Fomantis",
		{ &ATK_Fury_Cutter, &ATK_Razor_Leaf, &ATK_Leafage, &ATK_X_Scissor, &ATK_Leaf_Blade, &ATK_Superpower, &ATK_Leaf_Storm, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  755, "Morelull", TYPE_GRASS, TYPE_FAIRY, 108, 119, 120, nullptr,
		{ &ATK_Astonish, &ATK_Seed_Bomb, &ATK_Dazzling_Gleam, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  756, "Shiinotic", TYPE_GRASS, TYPE_FAIRY, 154, 168, 155, "Morelull",
		{ &ATK_Astonish, &ATK_Seed_Bomb, &ATK_Moonblast, &ATK_Sludge_Bomb, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  757, "Salandit", TYPE_POISON, TYPE_FIRE, 136, 80, 134, nullptr,
		{ &ATK_Ember, &ATK_Poison_Jab, &ATK_Flamethrower, &ATK_Poison_Fang, &ATK_Dragon_Pulse, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  758, "Salazzle", TYPE_POISON, TYPE_FIRE, 228, 130, 169, "Salandit",
		{ &ATK_Poison_Jab, &ATK_Incinerate, &ATK_Poison_Fang, &ATK_Dragon_Pulse, &ATK_Sludge_Wave, &ATK_Fire_Blast, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  759, "Stufful", TYPE_NORMAL, TYPE_FIGHTING, 136, 95, 172, nullptr,
		{ &ATK_Tackle, &ATK_Take_Down, &ATK_Brick_Break, &ATK_Stomp, &ATK_Superpower, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  760, "Bewear", TYPE_NORMAL, TYPE_FIGHTING, 226, 141, 260, "Stufful",
		{ &ATK_Low_Kick, &ATK_Shadow_Claw, &ATK_Tackle, &ATK_Stomp, &ATK_Superpower, &ATK_Drain_Punch, &ATK_Payback, },
		true, false, false, { &ATK_Drain_Punch, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  761, "Bounsweet", TYPE_GRASS, TYPECOUNT, 55, 69, 123, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Charm, &ATK_Draining_Kiss, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  762, "Steenee", TYPE_GRASS, TYPECOUNT, 78, 94, 141, "Bounsweet",
		{ &ATK_Razor_Leaf, &ATK_Charm, &ATK_Draining_Kiss, &ATK_Grass_Knot, &ATK_Energy_Ball, &ATK_Triple_Axel, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  763, "Tsareena", TYPE_GRASS, TYPECOUNT, 222, 195, 176, "Steenee",
		{ &ATK_Razor_Leaf, &ATK_Charm, &ATK_Magical_Leaf, &ATK_Draining_Kiss, &ATK_Stomp, &ATK_Grass_Knot, &ATK_Energy_Ball, &ATK_Triple_Axel, &ATK_High_Jump_Kick, },
		true, false, false, { &ATK_High_Jump_Kick, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  764, "Comfey", TYPE_FAIRY, TYPECOUNT, 165, 215, 139, nullptr,
		{ &ATK_Vine_Whip, &ATK_Hidden_Power, &ATK_Petal_Blizzard, &ATK_Draining_Kiss, &ATK_Grass_Knot, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  765, "Oranguru", TYPE_NORMAL, TYPE_PSYCHIC, 168, 192, 207, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Confusion, &ATK_Yawn, &ATK_Psychic, &ATK_Future_Sight, &ATK_Foul_Play, &ATK_Brutal_Swing, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  766, "Passimian", TYPE_FIGHTING, TYPECOUNT, 222, 160, 225, nullptr,
		{ &ATK_Rock_Smash, &ATK_Counter, &ATK_Take_Down, &ATK_Drain_Punch, &ATK_Brick_Break, &ATK_Close_Combat, &ATK_Superpower, &ATK_Brutal_Swing, },
		true, false, true, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  767, "Wimpod", TYPE_BUG, TYPE_WATER, 67, 74, 93, nullptr,
		{ &ATK_Struggle_Bug, &ATK_Bug_Buzz, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  768, "Golisopod", TYPE_BUG, TYPE_WATER, 218, 226, 181, "Wimpod",
		{ &ATK_Fury_Cutter, &ATK_Shadow_Claw, &ATK_Metal_Claw, &ATK_Waterfall, &ATK_Aerial_Ace, &ATK_Aqua_Jet, &ATK_X_Scissor, &ATK_Razor_Shell, &ATK_Liquidation, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  769, "Sandygast", TYPE_GHOST, TYPE_GROUND, 120, 118, 146, nullptr,
		{ &ATK_Mud_Shot, &ATK_Astonish, &ATK_Sand_Attack, &ATK_Shadow_Ball, &ATK_Sand_Tomb, &ATK_Earth_Power, &ATK_Scorching_Sands, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  770, "Palossand", TYPE_GHOST, TYPE_GROUND, 178, 178, 198, "Sandygast",
		{ &ATK_Mud_Shot, &ATK_Astonish, &ATK_Sand_Attack, &ATK_Shadow_Ball, &ATK_Sand_Tomb, &ATK_Earth_Power, &ATK_Scorching_Sands, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // 771 is pyukumuku
  // 772 is type: null
  // 773 is silvally
  // 774 is minior
  {  775, "Komala", TYPE_NORMAL, TYPECOUNT, 216, 165, 163, nullptr,
		{ &ATK_Yawn, &ATK_Rollout, &ATK_Play_Rough, &ATK_Bulldoze, &ATK_Payback, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  776, "Turtonator", TYPE_FIRE, TYPE_DRAGON, 165, 215, 155, nullptr,
		{ &ATK_Ember, &ATK_Fire_Spin, &ATK_Incinerate, &ATK_Dragon_Claw, &ATK_Brutal_Swing, &ATK_Flamethrower, &ATK_Flash_Cannon, &ATK_Dragon_Pulse, &ATK_Overheat, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  777, "Togedemaru", TYPE_ELECTRIC, TYPE_STEEL, 190, 145, 163, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Spark, &ATK_Wild_Charge, &ATK_Gyro_Ball, &ATK_Fell_Stinger, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // not yet released
  /* {  778, "Mimikyu", TYPE_GHOST, TYPE_FAIRY, 177, 199, 146, nullptr,
		{ &ATK_Shadow_Claw, &ATK_Charm, &ATK_Shadow_Sneak, &ATK_Shadow_Ball, &ATK_Play_Rough, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },*/
  {  779, "Bruxish", TYPE_WATER, TYPE_PSYCHIC, 208, 145, 169, nullptr,
		{ &ATK_Bite, &ATK_Water_Gun, &ATK_Confusion, &ATK_Aqua_Tail, &ATK_Psyshock, &ATK_Crunch, &ATK_Psychic_Fangs, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  780, "Drampa", TYPE_NORMAL, TYPE_DRAGON, 231, 164, 186, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Extrasensory, &ATK_Dragon_Pulse, &ATK_Swift, &ATK_Outrage, &ATK_Fly, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  781, "Dhelmise", TYPE_GHOST, TYPE_GRASS, 233, 179, 172, nullptr,
		{ &ATK_Shadow_Claw, &ATK_Astonish, &ATK_Metal_Sound, &ATK_Wrap, &ATK_Shadow_Ball, &ATK_Power_Whip, &ATK_Heavy_Slam, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  782, "Jangmo-o", TYPE_DRAGON, TYPECOUNT, 102, 108, 128, nullptr,
		{ &ATK_Tackle, &ATK_Dragon_Tail, &ATK_Dragon_Pulse, &ATK_Dragon_Claw, &ATK_Brick_Break, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  783, "Hakamo-o", TYPE_DRAGON, TYPE_FIGHTING, 145, 162, 146, "Jangmo-o",
		{ &ATK_Tackle, &ATK_Dragon_Tail, &ATK_Rock_Tomb, &ATK_Dragon_Pulse, &ATK_Dragon_Claw, &ATK_Brick_Break, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  784, "Kommo-o", TYPE_DRAGON, TYPE_FIGHTING, 222, 240, 181, "Hakamo-o",
		{ &ATK_Poison_Jab, &ATK_Metal_Sound, &ATK_Dragon_Tail, &ATK_Boomburst, &ATK_Close_Combat, &ATK_Brick_Break, &ATK_Upper_Hand, &ATK_Flamethrower, &ATK_Dragon_Claw, &ATK_Clanging_Scales, },
		true, false, false, { &ATK_Clanging_Scales, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  785, "Tapu Koko", TYPE_ELECTRIC, TYPE_FAIRY, 250, 181, 172, nullptr,
		{ &ATK_Quick_Attack, &ATK_Volt_Switch, &ATK_Thunder, &ATK_Thunderbolt, &ATK_Dazzling_Gleam, &ATK_Brave_Bird, &ATK_Natures_Madness, },
		true, false, false, { &ATK_Natures_Madness, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  786, "Tapu Lele", TYPE_PSYCHIC, TYPE_FAIRY, 259, 208, 172, nullptr,
		{ &ATK_Confusion, &ATK_Astonish, &ATK_Psyshock, &ATK_Draining_Kiss, &ATK_Moonblast, &ATK_Focus_Blast, &ATK_Future_Sight, &ATK_Natures_Madness, },
		true, false, false, { &ATK_Natures_Madness, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  787, "Tapu Bulu", TYPE_GRASS, TYPE_FAIRY, 249, 215, 172, nullptr,
		{ &ATK_Rock_Smash, &ATK_Bullet_Seed, &ATK_Megahorn, &ATK_Dazzling_Gleam, &ATK_Solar_Beam, &ATK_Grass_Knot, &ATK_Natures_Madness, },
		true, false, false, { &ATK_Natures_Madness, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  788, "Tapu Fini", TYPE_WATER, TYPE_FAIRY, 189, 254, 172, nullptr,
		{ &ATK_Water_Gun, &ATK_Hidden_Power, &ATK_Ice_Beam, &ATK_Moonblast, &ATK_Hydro_Pump, &ATK_Surf, &ATK_Natures_Madness, },
		true, false, false, { &ATK_Natures_Madness, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  789, "Cosmog", TYPE_PSYCHIC, TYPECOUNT, 54, 57, 125, nullptr,
		{ &ATK_Splash, &ATK_Struggle, },
		false, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  790, "Cosmoem", TYPE_PSYCHIC, TYPECOUNT, 54, 242, 125, "Cosmog",
		{ &ATK_Splash, &ATK_Struggle, },
		false, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  791, "Solgaleo", TYPE_PSYCHIC, TYPE_STEEL, 255, 191, 264, "Cosmoem",
		{ &ATK_Zen_Headbutt, &ATK_Fire_Spin, &ATK_Flamethrower, &ATK_Iron_Head, &ATK_Solar_Beam, &ATK_Psychic_Fangs, },
		false, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  792, "Lunala", TYPE_PSYCHIC, TYPE_GHOST, 255, 191, 264, "Cosmoem",
		{ &ATK_Shadow_Claw, &ATK_Confusion, &ATK_Air_Slash, &ATK_Shadow_Ball, &ATK_Moonblast, &ATK_Psychic, &ATK_Future_Sight, },
		false, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  793, "Nihilego", TYPE_ROCK, TYPE_POISON, 249, 210, 240, nullptr,
		{ &ATK_Pound, &ATK_Poison_Jab, &ATK_Acid, &ATK_Rock_Slide, &ATK_Power_Gem, &ATK_Sludge_Bomb, &ATK_Gunk_Shot, },
		true, false, false, { }, species::CAT_ULTRABEAST, 100, nullptr, species::EVOL_NOITEM, },
  {  794, "Buzzwole", TYPE_BUG, TYPE_FIGHTING, 236, 196, 216, nullptr,
		{ &ATK_Poison_Jab, &ATK_Counter, &ATK_Lunge, &ATK_Superpower, &ATK_Fell_Stinger, &ATK_Power_Up_Punch, },
		true, false, false, { }, species::CAT_ULTRABEAST, 100, nullptr, species::EVOL_NOITEM, },
  {  795, "Pheromosa", TYPE_BUG, TYPE_FIGHTING, 316, 85, 174, nullptr,
		{ &ATK_Bug_Bite, &ATK_Low_Kick, &ATK_Bug_Buzz, &ATK_Close_Combat, &ATK_Focus_Blast, &ATK_Lunge, },
		true, false, false, { }, species::CAT_ULTRABEAST, 100, nullptr, species::EVOL_NOITEM, },
  {  796, "Xurkitree", TYPE_ELECTRIC, TYPECOUNT, 330, 144, 195, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Spark, &ATK_Discharge, &ATK_Thunder, &ATK_Dazzling_Gleam, &ATK_Power_Whip, },
		true, false, false, { }, species::CAT_ULTRABEAST, 100, nullptr, species::EVOL_NOITEM, },
  {  797, "Celesteela", TYPE_STEEL, TYPE_FLYING, 207, 199, 219, nullptr,
		{ &ATK_Air_Slash, &ATK_Smack_Down, &ATK_Iron_Head, &ATK_Bulldoze, &ATK_Body_Slam, &ATK_Heavy_Slam, },
		true, false, false, { }, species::CAT_ULTRABEAST, 100, nullptr, species::EVOL_NOITEM, },
  {  798, "Kartana", TYPE_GRASS, TYPE_STEEL, 323, 182, 139, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Air_Slash, &ATK_Aerial_Ace, &ATK_Night_Slash, &ATK_X_Scissor, &ATK_Leaf_Blade, },
		true, false, false, { }, species::CAT_ULTRABEAST, 100, nullptr, species::EVOL_NOITEM, },
  {  799, "Guzzlord", TYPE_DARK, TYPE_DRAGON, 188, 99, 440, nullptr,
		{ &ATK_Dragon_Tail, &ATK_Snarl, &ATK_Dragon_Claw, &ATK_Sludge_Bomb, &ATK_Crunch, &ATK_Brutal_Swing, },
		true, false, false, { }, species::CAT_ULTRABEAST, 100, nullptr, species::EVOL_NOITEM, },
  {  800, "Necrozma", TYPE_PSYCHIC, TYPECOUNT, 251, 195, 219, nullptr,
		{ &ATK_Shadow_Claw, &ATK_Psycho_Cut, &ATK_Metal_Claw, &ATK_Dark_Pulse, &ATK_Iron_Head, &ATK_Future_Sight, &ATK_Outrage, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  800, "Necrozma Dusk Mane", TYPE_PSYCHIC, TYPE_STEEL, 277, 220, 200, "Necrozma",
		{ &ATK_Shadow_Claw, &ATK_Psycho_Cut, &ATK_Metal_Claw, &ATK_Dark_Pulse, &ATK_Iron_Head, &ATK_Future_Sight, &ATK_Outrage, &ATK_Sunsteel_Strike, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  800, "Necrozma Dawn Wings", TYPE_PSYCHIC, TYPE_GHOST, 277, 220, 200, "Necrozma",
		{ &ATK_Shadow_Claw, &ATK_Psycho_Cut, &ATK_Metal_Claw, &ATK_Dark_Pulse, &ATK_Iron_Head, &ATK_Future_Sight, &ATK_Outrage, &ATK_Moongeist_Beam, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // 801 is magearna
  {  802, "Marshadow", TYPE_FIGHTING, TYPE_GHOST, 265, 190, 207, nullptr,
		{ &ATK_Sucker_Punch, &ATK_Poison_Jab, &ATK_Counter, &ATK_Ice_Punch, &ATK_Rock_Slide, &ATK_Thunder_Punch, &ATK_Fire_Punch, &ATK_Close_Combat, },
		true, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  803, "Poipole", TYPE_POISON, TYPECOUNT, 145, 133, 167, nullptr,
		{ &ATK_Peck, &ATK_Poison_Jab, &ATK_Sludge_Bomb, &ATK_Sludge_Wave, &ATK_Fell_Stinger, },
		true, false, false, { }, species::CAT_ULTRABEAST, 100, nullptr, species::EVOL_NOITEM, },
  {  804, "Naganadel", TYPE_POISON, TYPE_DRAGON, 263, 159, 177, "Poipole",
		{ &ATK_Poison_Jab, &ATK_Air_Slash, &ATK_Dragon_Pulse, &ATK_Dragon_Claw, &ATK_Sludge_Bomb, &ATK_Fell_Stinger, &ATK_Acrobatics, },
		true, false, false, { }, species::CAT_ULTRABEAST, 100, nullptr, species::EVOL_NOITEM, },
  {  805, "Stakataka", TYPE_ROCK, TYPE_STEEL, 213, 298, 156, nullptr,
		{ &ATK_Rock_Throw, &ATK_Take_Down, &ATK_Stone_Edge, &ATK_Flash_Cannon, &ATK_Bulldoze, },
		true, false, false, { }, species::CAT_ULTRABEAST, 100, nullptr, species::EVOL_NOITEM, },
  {  806, "Blacephalon", TYPE_FIRE, TYPE_GHOST, 315, 148, 142, nullptr,
		{ &ATK_Astonish, &ATK_Incinerate, &ATK_Shadow_Ball, &ATK_Overheat, &ATK_Mystical_Fire, },
		true, false, false, { }, species::CAT_ULTRABEAST, 100, nullptr, species::EVOL_NOITEM, },
  // 807 zeraora, the thunderclap
  {  808, "Meltan", TYPE_STEEL, TYPECOUNT, 118, 99, 130, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Flash_Cannon, &ATK_Thunderbolt, },
		true, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  809, "Melmetal", TYPE_STEEL, TYPECOUNT, 226, 190, 264, "Meltan",
		{ &ATK_Thunder_Shock, &ATK_Hyper_Beam, &ATK_Flash_Cannon, &ATK_Rock_Slide, &ATK_Thunderbolt, &ATK_Superpower, &ATK_Double_Iron_Bash, },
		true, false, false, { &ATK_Double_Iron_Bash, }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  810, "Grookey", TYPE_GRASS, TYPECOUNT, 122, 91, 137, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Scratch, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		true, true, true, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  811, "Thwackey", TYPE_GRASS, TYPECOUNT, 165, 134, 172, "Grookey",
		{ &ATK_Razor_Leaf, &ATK_Scratch, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		true, true, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  812, "Rillaboom", TYPE_GRASS, TYPECOUNT, 239, 168, 225, "Thwackey",
		{ &ATK_Razor_Leaf, &ATK_Scratch, &ATK_Grass_Knot, &ATK_Energy_Ball, &ATK_Earth_Power, },
		true, true, true, { }, species::CAT_NORMAL, 10, "G-Max Drum Solo", },
  {  813, "Scorbunny", TYPE_FIRE, TYPECOUNT, 132, 79, 137, nullptr,
		{ &ATK_Tackle, &ATK_Fire_Spin, &ATK_Flamethrower, &ATK_Flame_Charge, },
		false, false, true, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  814, "Raboot", TYPE_FIRE, TYPECOUNT, 170, 125, 163, "Scorbunny",
		{ &ATK_Tackle, &ATK_Fire_Spin, &ATK_Flamethrower, &ATK_Flame_Charge, },
		false, false, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  815, "Cinderace", TYPE_FIRE, TYPECOUNT, 238, 163, 190, "Raboot",
		{ &ATK_Tackle, &ATK_Fire_Spin, &ATK_Flamethrower, &ATK_Flame_Charge, &ATK_Focus_Blast, },
		true, false, true, { }, species::CAT_NORMAL, 10, "G-Max Fireball", },
  {  816, "Sobble", TYPE_WATER, TYPECOUNT, 132, 79, 137, nullptr,
		{ &ATK_Pound, &ATK_Water_Gun, &ATK_Water_Pulse, &ATK_Surf, },
		false, false, true, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  817, "Drizzile", TYPE_WATER, TYPECOUNT, 186, 113, 163, "Sobble",
		{ &ATK_Pound, &ATK_Water_Gun, &ATK_Water_Pulse, &ATK_Surf, },
		false, false, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  818, "Inteleon", TYPE_WATER, TYPECOUNT, 262, 142, 172, "Drizzile",
		{ &ATK_Pound, &ATK_Water_Gun, &ATK_Shadow_Ball, &ATK_Water_Pulse, &ATK_Surf, },
		false, false, true, { }, species::CAT_NORMAL, 10, "G-Max Hydrosnipe", },
  {  819, "Skwovet", TYPE_NORMAL, TYPECOUNT, 95, 86, 172, nullptr,
		{ &ATK_Bite, &ATK_Tackle, &ATK_Bullet_Seed, &ATK_Body_Slam, &ATK_Crunch, &ATK_Trailblaze, },
		true, false, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  820, "Greedent", TYPE_NORMAL, TYPECOUNT, 160, 156, 260, "Skwovet",
		{ &ATK_Bite, &ATK_Mud_Shot, &ATK_Tackle, &ATK_Bullet_Seed, &ATK_Body_Slam, &ATK_Crunch, &ATK_Trailblaze, },
		true, false, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  821, "Rookidee", TYPE_FLYING, TYPECOUNT, 88, 67, 116, nullptr,
		{ &ATK_Peck, &ATK_Air_Slash, &ATK_Sand_Attack, &ATK_Sky_Attack, &ATK_Payback, },
		true, false, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  822, "Corvisquire", TYPE_FLYING, TYPECOUNT, 129, 110, 169, "Rookidee",
		{ &ATK_Peck, &ATK_Air_Slash, &ATK_Sand_Attack, &ATK_Sky_Attack, &ATK_Payback, },
		true, false, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  823, "Corviknight", TYPE_FLYING, TYPE_STEEL, 163, 192, 221, "Corvisquire",
		{ &ATK_Steel_Wing, &ATK_Air_Slash, &ATK_Sand_Attack, &ATK_Iron_Head, &ATK_Sky_Attack, &ATK_Payback, },
		true, false, true, { &ATK_Iron_Head, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  /*{  824, "Blipbug", TYPE_BUG, TYPECOUNT, 46, 67, 93, nullptr,
    { &ATK_Struggle_Bug, &ATK_Struggle, },
    false, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  825, "Dottler", TYPE_BUG, TYPE_PSYCHIC, 87, 157, 137, "Blipbug",
    { &ATK_Confusion, &ATK_Struggle_Bug, &ATK_Bug_Buzz, &ATK_Psychic, },
    false, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  826, "Orbeetle", TYPE_BUG, TYPE_PSYCHIC, 156, 240, 155, "Dottler",
    { &ATK_Confusion, &ATK_Struggle_Bug, &ATK_Bug_Buzz, &ATK_Psychic, },
    false, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },*/
  {  827, "Nickit", TYPE_DARK, TYPECOUNT, 85, 82, 120, nullptr,
		{ &ATK_Sucker_Punch, &ATK_Quick_Attack, &ATK_Snarl, &ATK_Night_Slash, &ATK_Play_Rough, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  828, "Thievul", TYPE_DARK, TYPECOUNT, 172, 164, 172, "Nickit",
		{ &ATK_Sucker_Punch, &ATK_Quick_Attack, &ATK_Snarl, &ATK_Night_Slash, &ATK_Play_Rough, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  829, "Gossifleur", TYPE_GRASS, TYPECOUNT, 70, 104, 120, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Bullet_Seed, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  830, "Eldegoss", TYPE_GRASS, TYPECOUNT, 148, 211, 155, "Gossifleur",
		{ &ATK_Razor_Leaf, &ATK_Bullet_Seed, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  831, "Wooloo", TYPE_NORMAL, TYPECOUNT, 76, 97, 123, nullptr,
		{ &ATK_Tackle, &ATK_Take_Down, &ATK_Stomp, &ATK_Wild_Charge, &ATK_Payback, },
		true, false, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  832, "Dubwool", TYPE_NORMAL, TYPECOUNT, 159, 198, 176, "Wooloo",
		{ &ATK_Tackle, &ATK_Take_Down, &ATK_Double_Kick, &ATK_Body_Slam, &ATK_Wild_Charge, &ATK_Payback, },
		true, false, true, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  /*{  833, "Chewtle", TYPE_WATER, TYPECOUNT, 114, 85, 137, nullptr,
		{ &ATK_Bite, &ATK_Water_Gun, &ATK_Body_Slam, &ATK_Surf, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  834, "Drednaw", TYPE_WATER, TYPE_ROCK, 213, 164, 207, nullptr,
		{ &ATK_Bite, &ATK_Waterfall, &ATK_Body_Slam, &Rock_Blast, &ATK_Crunch, &ATK_Surf, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },*/
  {  835, "Yamper", TYPE_ELECTRIC, TYPECOUNT, 80, 90, 153, nullptr,
		{ &ATK_Bite, &ATK_Spark, &ATK_Thunder, &ATK_Crunch, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  836, "Boltund", TYPE_ELECTRIC, TYPECOUNT, 197, 131, 170, nullptr,
		{ &ATK_Bite, &ATK_Spark, &ATK_Thunder, &ATK_Crunch, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // 837 rolycoly
  // 838 carkol
  // 839 coalossal
  {  840, "Applin", TYPE_GRASS, TYPE_DRAGON, 71, 116, 120, nullptr,
		{ &ATK_Astonish, &ATK_Struggle, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  841, "Flapple", TYPE_GRASS, TYPE_DRAGON, 214, 144, 172, "Applin",
		{ &ATK_Dragon_Breath, &ATK_Bullet_Seed, &ATK_Seed_Bomb, &ATK_Dragon_Pulse, &ATK_Outrage, &ATK_Fly, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_TARTAPPLE, },
  {  842, "Appletun", TYPE_GRASS, TYPE_DRAGON, 178, 146, 242, "Applin",
		{ &ATK_Astonish, &ATK_Bullet_Seed, &ATK_Seed_Bomb, &ATK_Dragon_Pulse, &ATK_Energy_Ball, &ATK_Outrage, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_SWEETAPPLE, },
  // 843 silcobra
  // 844 sandaconda
  // 845 cramorant
  // 846 arrokuda
  // 847 barraskewda
  {  848, "Toxel", TYPE_ELECTRIC, TYPE_POISON, 97, 65, 120, nullptr,
		{ &ATK_Acid, &ATK_Power_Up_Punch, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  849, "Toxtricity", TYPE_ELECTRIC, TYPE_POISON, 224, 140, 181, "Toxel",
		{ &ATK_Spark, &ATK_Poison_Jab, &ATK_Acid, &ATK_Discharge, &ATK_Wild_Charge, &ATK_Acid_Spray, &ATK_Power_Up_Punch, },
		true, true, true, { }, species::CAT_NORMAL, 75, "G-Max Stun Shock", },
  {  850, "Sizzlipede", TYPE_FIRE, TYPE_BUG, 118, 90, 137, nullptr,
		{ &ATK_Bug_Bite, &ATK_Ember, &ATK_Heat_Wave, &ATK_Bug_Buzz, &ATK_Crunch, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  851, "Centiskorch", TYPE_FIRE, TYPE_BUG, 220, 158, 225, "Sizzlipede",
		{ &ATK_Bug_Bite, &ATK_Ember, &ATK_Heat_Wave, &ATK_Bug_Buzz, &ATK_Crunch, &ATK_Lunge, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // 852 clobbopus
  // 853 grapploct
  {  854, "Sinistea", TYPE_GHOST, TYPECOUNT, 134, 96, 120, nullptr,
		{ &ATK_Sucker_Punch, &ATK_Astonish, &ATK_Dark_Pulse, &ATK_Shadow_Ball, &ATK_Psychic, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  855, "Polteageist", TYPE_GHOST, TYPECOUNT, 248, 189, 155, "Sinistea",
		{ &ATK_Sucker_Punch, &ATK_Astonish, &ATK_Hex, &ATK_Dark_Pulse, &ATK_Shadow_Ball, &ATK_Psychic, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  856, "Hatenna", TYPE_PSYCHIC, TYPECOUNT, 98, 93, 123, nullptr,
		{ &ATK_Confusion, &ATK_Charm, &ATK_Psyshock, &ATK_Dazzling_Gleam, &ATK_Psychic, },
		true, false, true, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  857, "Hattrem", TYPE_PSYCHIC, TYPECOUNT, 153, 133, 149, "Hatenna",
		{ &ATK_Confusion, &ATK_Charm, &ATK_Psyshock, &ATK_Dazzling_Gleam, &ATK_Psychic, },
		true, false, true, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  858, "Hatterene", TYPE_PSYCHIC, TYPE_FAIRY, 237, 182, 149, "Hattrem",
		{ &ATK_Psycho_Cut, &ATK_Confusion, &ATK_Charm, &ATK_Psyshock, &ATK_Dazzling_Gleam, &ATK_Psychic, &ATK_Power_Whip, },
		true, false, true, { }, species::CAT_NORMAL, 50, "G-Max Smite", },
  {  859, "Impidimp", TYPE_DARK, TYPE_FAIRY, 103, 69, 128, nullptr,
		{ &ATK_Bite, &ATK_Sucker_Punch, &ATK_Dark_Pulse, &ATK_Play_Rough, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  860, "Morgrem", TYPE_DARK, TYPE_FAIRY, 145, 102, 163, "Impidimp",
		{ &ATK_Bite, &ATK_Sucker_Punch, &ATK_Dark_Pulse, &ATK_Play_Rough, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Bulk Up
  {  861, "Grimmsnarl", TYPE_DARK, TYPE_FAIRY, 237, 139, 216, "Morgrem",
		{ &ATK_Bite, &ATK_Sucker_Punch, &ATK_Low_Kick, &ATK_Dark_Pulse, &ATK_Play_Rough, &ATK_Foul_Play, &ATK_Power_Up_Punch, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  862, "Obstagoon", TYPE_DARK, TYPE_NORMAL, 180, 194, 212, "Galarian Linoone",
		{ &ATK_Lick, &ATK_Counter, &ATK_Hyper_Beam, &ATK_Cross_Chop, &ATK_Night_Slash, &ATK_Gunk_Shot, &ATK_Obstruct, },
		true, true, false, { &ATK_Obstruct, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  863, "Perrserker", TYPE_STEEL, TYPECOUNT, 195, 162, 172, "Galarian Meowth",
		{ &ATK_Shadow_Claw, &ATK_Metal_Claw, &ATK_Metal_Sound, &ATK_Iron_Head, &ATK_Play_Rough, &ATK_Close_Combat, &ATK_Foul_Play, &ATK_Trailblaze, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  864, "Cursola", TYPE_GHOST, TYPECOUNT, 253, 182, 155, "Galarian Corsola",
		{ &ATK_Astonish, &ATK_Hex, &ATK_Shadow_Ball, &ATK_Brine, &ATK_Rock_Blast, &ATK_Night_Shade, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  865, "Sirfetch'd", TYPE_FIGHTING, TYPECOUNT, 248, 176, 158, "Galarian Farfetch'd",
		{ &ATK_Fury_Cutter, &ATK_Counter, &ATK_Night_Slash, &ATK_Leaf_Blade, &ATK_Close_Combat, &ATK_Brave_Bird, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Comedian
  {  866, "Mr. Rime", TYPE_ICE, TYPE_PSYCHIC, 212, 179, 190, "Galarian Mr. Mime",
		{ &ATK_Ice_Shard, &ATK_Zen_Headbutt, &ATK_Confusion, &ATK_Psybeam, &ATK_Ice_Punch, &ATK_Psychic, &ATK_Icy_Wind, &ATK_Triple_Axel, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  867, "Runerigus", TYPE_GROUND, TYPE_GHOST, 163, 237, 151, "Galarian Yamask",
		{ &ATK_Shadow_Claw, &ATK_Astonish, &ATK_Rock_Smash, &ATK_Shadow_Ball, &ATK_Sand_Tomb, &ATK_Brutal_Swing, },
		true, true, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // 868: milcery
  // 869: alcremie
  {  870, "Falinks", TYPE_FIGHTING, TYPECOUNT, 193, 170, 163, nullptr,
		{ &ATK_Rock_Smash, &ATK_Counter, &ATK_Megahorn, &ATK_Brick_Break, &ATK_Superpower, },
		true, false, true, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // 871 pincurchin
  {  872, "Snom", TYPE_BUG, TYPE_ICE, 76, 59, 102, nullptr,
    { &ATK_Powder_Snow, &ATK_Struggle_Bug, &ATK_Icy_Wind, &ATK_Bug_Buzz, },
    false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  873, "Frosmoth", TYPE_BUG, TYPE_ICE, 230, 155, 172, "Snom",
    { &ATK_Powder_Snow, &ATK_Bug_Bite, &ATK_Icy_Wind, &ATK_Bug_Buzz, &ATK_Ice_Beam, &ATK_Hurricane, },
    false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  874, "Stonjourner", TYPE_ROCK, TYPECOUNT, 222, 182, 225, nullptr,
		{ &ATK_Rock_Throw, &ATK_Stone_Edge, &ATK_Rock_Slide, &ATK_Stomp, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // 875 eiscue
  // the emotion
  {  876, "Indeedee", TYPE_PSYCHIC, TYPE_NORMAL, 208, 166, 155, nullptr,
		{ &ATK_Extrasensory, &ATK_Psybeam, &ATK_Shadow_Ball, &ATK_Psychic, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  877, "Morpeko", TYPE_ELECTRIC, TYPE_DARK, 192, 121, 151, nullptr,
		{ &ATK_Bite, &ATK_Thunder_Shock, &ATK_Charge_Beam, &ATK_Seed_Bomb, &ATK_Outrage, &ATK_Psychic_Fangs, &ATK_Aura_Wheel, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // 878 cufant
  // 879 copperajah
  // 880 is dracozolt
  // 881 is arctozolt
  // 882 is dracovish
  // 883 is arctovish
  {  884, "Duraludon", TYPE_DRAGON, TYPE_STEEL, 239, 185, 172, nullptr,
		{ &ATK_Dragon_Tail, &ATK_Metal_Claw, &ATK_Flash_Cannon, &ATK_Dragon_Claw, &ATK_Hyper_Beam, },
		false, false, true, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  885, "Dreepy", TYPE_DRAGON, TYPE_GHOST, 117, 61, 99, nullptr,
		{ &ATK_Quick_Attack, &ATK_Astonish, &ATK_Struggle, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  886, "Drakloak", TYPE_DRAGON, TYPE_GHOST, 163, 105, 169, "Dreepy",
		{ &ATK_Dragon_Tail, &ATK_Astonish, &ATK_Shadow_Ball, &ATK_Dragon_Pulse, &ATK_Outrage, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  887, "Dragapult", TYPE_DRAGON, TYPE_GHOST, 266, 170, 204, "Drakloak",
		{ &ATK_Dragon_Tail, &ATK_Astonish, &ATK_Shadow_Ball, &ATK_Dragon_Pulse, &ATK_Outrage, &ATK_Breaking_Swipe, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  888, "Zacian", TYPE_FAIRY, TYPECOUNT, 254, 236, 192, nullptr,
		{ &ATK_Quick_Attack, &ATK_Metal_Claw, &ATK_Fire_Fang, &ATK_Snarl, &ATK_Iron_Head, &ATK_Play_Rough, &ATK_Close_Combat, &ATK_Wild_Charge, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  888, "Crowned Sword Zacian", TYPE_FAIRY, TYPE_STEEL, 332, 240, 192, "Zacian",
		{ &ATK_Metal_Claw, &ATK_Air_Slash, &ATK_Play_Rough, &ATK_Close_Combat, &ATK_Giga_Impact, &ATK_Behemoth_Blade, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  889, "Zamazenta", TYPE_FIGHTING, TYPECOUNT, 254, 236, 192, nullptr,
		{ &ATK_Quick_Attack, &ATK_Metal_Claw, &ATK_Snarl, &ATK_Ice_Fang, &ATK_Iron_Head, &ATK_Moonblast, &ATK_Close_Combat, &ATK_Crunch, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  889, "Crowned Shield Zamazenta", TYPE_FIGHTING, TYPE_STEEL, 250, 292, 192, "Zamazenta",
		{ &ATK_Metal_Claw, &ATK_Ice_Fang, &ATK_Moonblast, &ATK_Close_Combat, &ATK_Giga_Impact, &ATK_Behemoth_Bash, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  890, "Eternatus", TYPE_DRAGON, TYPE_POISON, 278, 192, 268, nullptr,
		{ &ATK_Dragon_Tail, &ATK_Poison_Jab, &ATK_Sludge_Bomb, &ATK_Dragon_Pulse, &ATK_Flamethrower, &ATK_Dynamax_Cannon, },
		false, false, false, { &ATK_Dynamax_Cannon, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  /* {  890, "Eternamax Eternatus", TYPE_DRAGON, TYPE_POISON, 251, 505, 452, nullptr,
		{ &ATK_Dragon_Tail, &ATK_Poison_Jab, &ATK_Sludge_Bomb, &ATK_Dragon_Pulse, &ATK_Flamethrower, &ATK_Hyper_Beam, &ATK_Dynamax_Cannon, },
		false, false, false, { &ATK_Dynamax_Cannon, }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },*/
  {  891, "Kubfu", TYPE_FIGHTING, TYPECOUNT, 170, 112, 155, nullptr,
		{ &ATK_Rock_Smash, &ATK_Counter, &ATK_Brick_Break, &ATK_Close_Combat, &ATK_Dynamic_Punch, },
		false, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  892, "Single Strike Style Urshifu", TYPE_FIGHTING, TYPE_DARK, 254, 177, 225, "Kubfu",
		{ &ATK_Sucker_Punch, &ATK_Rock_Smash, &ATK_Counter, &ATK_Brick_Break, &ATK_Close_Combat, &ATK_Dynamic_Punch, &ATK_Payback, },
		false, false, true, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  892, "Rapid Strike Style Urshifu", TYPE_FIGHTING, TYPE_WATER, 254, 177, 225, "Kubfu",
		{ &ATK_Rock_Smash, &ATK_Counter, &ATK_Waterfall, &ATK_Aqua_Jet, &ATK_Brick_Break, &ATK_Close_Combat, &ATK_Dynamic_Punch, },
		false, false, true, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  893, "Zarude", TYPE_DARK, TYPE_GRASS, 242, 215, 233, nullptr,
		{ &ATK_Bite, &ATK_Vine_Whip, &ATK_Dark_Pulse, &ATK_Power_Whip, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_MYTHICAL, 100, nullptr, species::EVOL_NOITEM, },
  {  894, "Regieleki", TYPE_ELECTRIC, TYPECOUNT, 250, 125, 190, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Volt_Switch, &ATK_Lock_On, &ATK_Hyper_Beam, &ATK_Thunder, &ATK_Zap_Cannon, },
		true, true, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  895, "Regidrago", TYPE_DRAGON, TYPECOUNT, 202, 101, 400, nullptr,
		{ &ATK_Bite, &ATK_Dragon_Breath, &ATK_Hyper_Beam, &ATK_Dragon_Pulse, &ATK_Dragon_Energy, &ATK_Outrage, &ATK_Breaking_Swipe, },
		true, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  // 896 -- glastrier
  // 897 -- spectrier
  // 898 -- calyrex
  {  899, "Wyrdeer", TYPE_NORMAL, TYPE_PSYCHIC, 206, 145, 230, nullptr,
		{ &ATK_Tackle, &ATK_Zen_Headbutt, &ATK_Confusion, &ATK_Megahorn, &ATK_Psychic, &ATK_Stomp, &ATK_Wild_Charge, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Axe
  {  900, "Kleavor", TYPE_BUG, TYPE_ROCK, 253, 174, 172, nullptr,
		{ &ATK_Fury_Cutter, &ATK_Quick_Attack, &ATK_Air_Slash, &ATK_Stone_Edge, &ATK_Rock_Slide, &ATK_X_Scissor, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  901, "Ursaluna", TYPE_GROUND, TYPE_NORMAL, 243, 181, 277, "Ursaring",
		{ &ATK_Tackle, &ATK_Rock_Smash, &ATK_Ice_Punch, &ATK_Aerial_Ace, &ATK_Thunder_Punch, &ATK_Fire_Punch, &ATK_Swift, &ATK_High_Horsepower, &ATK_Trailblaze, },
		true, true, false, { &ATK_High_Horsepower, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  /*{  902, "Basculegion", TYPE_WATER, TYPE_GHOST, , , , nullptr, species::EVOL_NOITEM, }, */
  {  903, "Sneasler", TYPE_FIGHTING, TYPE_POISON, 259, 158, 190, "Hisuian Sneasel",
		{ &ATK_Shadow_Claw, &ATK_Poison_Jab, &ATK_Rock_Smash, &ATK_Aerial_Ace, &ATK_X_Scissor, &ATK_Close_Combat, },
		true, true, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  904, "Overqwil", TYPE_DARK, TYPE_POISON, 222, 171, 198, "Hisuian Qwilfish",
		{ &ATK_Poison_Jab, &ATK_Poison_Sting, &ATK_Dark_Pulse, &ATK_Ice_Beam, &ATK_Aqua_Tail, &ATK_Shadow_Ball, &ATK_Sludge_Bomb, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // therian form enamorus has not been released
  {  905, "Incarnate Form Enamorus", TYPE_FAIRY, TYPE_FLYING, 281, 162, 179, nullptr,
		{ &ATK_Zen_Headbutt, &ATK_Astonish, &ATK_Fairy_Wind, &ATK_Dazzling_Gleam, &ATK_Grass_Knot, &ATK_Fly, },
		false, false, false, { }, species::CAT_LEGENDARY, 100, nullptr, species::EVOL_NOITEM, },
  {  906, "Sprigatito", TYPE_GRASS, TYPECOUNT, 116, 99, 120, nullptr,
		{ &ATK_Charm, &ATK_Leafage, &ATK_Play_Rough, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  907, "Floragato", TYPE_GRASS, TYPECOUNT, 157, 128, 156, "Sprigatito",
		{ &ATK_Charm, &ATK_Leafage, &ATK_Play_Rough, &ATK_Grass_Knot, &ATK_Energy_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  908, "Meowscarada", TYPE_GRASS, TYPE_DARK, 233, 153, 183, "Floragato",
		{ &ATK_Charm, &ATK_Leafage, &ATK_Night_Slash, &ATK_Play_Rough, &ATK_Grass_Knot, &ATK_Energy_Ball, &ATK_Frenzy_Plant, &ATK_Flower_Trick, },
		true, false, false, { &ATK_Frenzy_Plant, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  909, "Fuecoco", TYPE_FIRE, TYPECOUNT, 112, 96, 167, nullptr,
		{ &ATK_Bite, &ATK_Incinerate, &ATK_Flamethrower, &ATK_Disarming_Voice, &ATK_Crunch, },
		true, false, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  910, "Crocalor", TYPE_FIRE, TYPECOUNT, 162, 134, 191, "Fuecoco",
		{ &ATK_Bite, &ATK_Incinerate, &ATK_Flamethrower, &ATK_Disarming_Voice, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  911, "Skeledirge", TYPE_FIRE, TYPE_GHOST, 207, 178, 232, "Crocalor",
		{ &ATK_Bite, &ATK_Hex, &ATK_Incinerate, &ATK_Flamethrower, &ATK_Shadow_Ball, &ATK_Disarming_Voice, &ATK_Crunch, &ATK_Blast_Burn, &ATK_Torch_Song, },
		true, false, false, { &ATK_Blast_Burn, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  912, "Quaxly", TYPE_WATER, TYPECOUNT, 120, 86, 146, nullptr,
		{ &ATK_Wing_Attack, &ATK_Water_Gun, &ATK_Aerial_Ace, &ATK_Aqua_Jet, &ATK_Liquidation, },
		true, false, false, { }, species::CAT_FPARTNER, 10, nullptr, species::EVOL_NOITEM, },
  {  913, "Quaxwell", TYPE_WATER, TYPECOUNT, 162, 123, 172, "Quaxly",
		{ &ATK_Wing_Attack, &ATK_Water_Gun, &ATK_Aerial_Ace, &ATK_Aqua_Jet, &ATK_Liquidation, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  914, "Quaquaval", TYPE_WATER, TYPE_FIGHTING, 236, 159, 198, "Quaxwell",
		{ &ATK_Wing_Attack, &ATK_Water_Gun, &ATK_Aerial_Ace, &ATK_Aqua_Jet, &ATK_Aqua_Step, &ATK_Close_Combat, &ATK_Liquidation, &ATK_Hydro_Cannon, },
		true, false, false, { &ATK_Hydro_Cannon, }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  915, "Lechonk", TYPE_NORMAL, TYPECOUNT, 81, 79, 144, nullptr,
		{ &ATK_Tackle, &ATK_Take_Down, &ATK_Dig, &ATK_Body_Slam, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  916, "Male Oinkologne", TYPE_NORMAL, TYPECOUNT, 186, 153, 242, "Lechonk",
		{ &ATK_Tackle, &ATK_Take_Down, &ATK_Dig, &ATK_Body_Slam, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  916, "Female Oinkologne", TYPE_NORMAL, TYPECOUNT, 169, 162, 251, "Lechonk",
		{ &ATK_Tackle, &ATK_Take_Down, &ATK_Dig, &ATK_Body_Slam, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  917, "Tarountula", TYPE_BUG, TYPECOUNT, 70, 77, 111, nullptr,
		{ &ATK_Bug_Bite, &ATK_Struggle_Bug, &ATK_X_Scissor, &ATK_Grass_Knot, &ATK_Lunge, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  918, "Spidops", TYPE_BUG, TYPECOUNT, 139, 166, 155, "Tarountula",
		{ &ATK_Bug_Bite, &ATK_Counter, &ATK_Rock_Tomb, &ATK_X_Scissor, &ATK_Grass_Knot, &ATK_Lunge, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  919, "Nymble", TYPE_BUG, TYPECOUNT, 81, 65, 107, nullptr,
		{ &ATK_Bug_Bite, &ATK_Struggle_Bug, &ATK_Bug_Buzz, &ATK_X_Scissor, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  920, "Lokix", TYPE_BUG, TYPE_DARK, 199, 144, 174, "Nymble",
		{ &ATK_Bug_Bite, &ATK_Sucker_Punch, &ATK_Counter, &ATK_Dark_Pulse, &ATK_Bug_Buzz, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  921, "Pawmi", TYPE_ELECTRIC, TYPECOUNT, 95, 45, 128, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Spark, &ATK_Charge_Beam, &ATK_Discharge, &ATK_Thunderbolt, &ATK_Wild_Charge, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  922, "Pawmo", TYPE_ELECTRIC, TYPE_FIGHTING, 147, 82, 155, "Pawmi",
		{ &ATK_Thunder_Shock, &ATK_Spark, &ATK_Charge_Beam, &ATK_Discharge, &ATK_Thunder_Punch, &ATK_Thunderbolt, &ATK_Wild_Charge, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  923, "Pawmot", TYPE_ELECTRIC, TYPE_FIGHTING, 232, 141, 172, "Pawmo",
		{ &ATK_Thunder_Shock, &ATK_Spark, &ATK_Low_Kick, &ATK_Charge_Beam, &ATK_Discharge, &ATK_Thunder_Punch, &ATK_Thunderbolt, &ATK_Brick_Break, &ATK_Close_Combat, &ATK_Wild_Charge, },
		true, false, false, { &ATK_Brick_Break, }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  924, "Tandemaus", TYPE_NORMAL, TYPECOUNT, 98, 90, 137, nullptr,
		{ &ATK_Mud_Shot, &ATK_Bullet_Seed, &ATK_Take_Down, &ATK_Play_Rough, &ATK_Swift, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  925, "Maushold", TYPE_NORMAL, TYPECOUNT, 159, 157, 179, "Tandemaus",
		{ &ATK_Mud_Shot, &ATK_Bullet_Seed, &ATK_Take_Down, &ATK_Play_Rough, &ATK_Swift, &ATK_Crunch, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  {  926, "Fidough", TYPE_FAIRY, TYPECOUNT, 102, 126, 114, nullptr,
		{ &ATK_Bite, &ATK_Charm, &ATK_Play_Rough, &ATK_Body_Slam, &ATK_Psychic_Fangs, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  927, "Dachsbun", TYPE_FAIRY, TYPECOUNT, 159, 212, 149, "Fidough",
		{ &ATK_Bite, &ATK_Charm, &ATK_Play_Rough, &ATK_Body_Slam, &ATK_Psychic_Fangs, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  928, "Smoliv", TYPE_GRASS, TYPE_NORMAL, 100, 89, 121, nullptr,
		{ &ATK_Razor_Leaf, &ATK_Tackle, &ATK_Seed_Bomb, &ATK_Energy_Ball, &ATK_Earth_Power, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  929, "Dolliv", TYPE_GRASS, TYPE_NORMAL, 137, 131, 141, "Smoliv",
		{ &ATK_Razor_Leaf, &ATK_Tackle, &ATK_Magical_Leaf, &ATK_Seed_Bomb, &ATK_Energy_Ball, &ATK_Earth_Power, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  930, "Arboliva", TYPE_GRASS, TYPE_NORMAL, 219, 189, 186, "Dolliv",
		{ &ATK_Razor_Leaf, &ATK_Tackle, &ATK_Magical_Leaf, &ATK_Seed_Bomb, &ATK_Energy_Ball, &ATK_Earth_Power, &ATK_Trailblaze, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  /*{  931, "Squakabilly", TYPE_NORMALY, TYPE_FLYING, 185, 105, 193, nullptr,
		{ &ATK_Peck, &ATK_Air_Slash, &ATK_Aerial_Ace, &ATK_Brave_Bird, &ATK_Fly, },
		false, false, false, { }, species::CAT_NORMAL, 25, nullptr, species::EVOL_NOITEM, },
  {  932, "Nacli", TYPE_ROCK, TYPECOUNT, 95, 108, 146, nullptr,
		{ &ATK_Mud_Shot, &ATK_Rock_Throw, &ATK_Ancient_Power, &ATK_Rock_Tomb, &ATK_Rock_Blast, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  933, "Naclstack", TYPE_ROCK, TYPECOUNT, 105, 160, 155, "Nacli",
		{ &ATK_Mud_Shot, &ATK_Smack_Down, &ATK_Ancient_Power, &ATK_Rock_Tomb, &ATK_Rock_Blast, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  934, "Garganacl", TYPE_ROCK, TYPECOUNT, 171, 212, 225, "Naclstack",
		{ &ATK_Mud_Shot, &ATK_Smack_Down, &ATK_Ancient_Power, &ATK_Rock_Tomb, &ATK_Rock_Blast, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },*/
  {  935, "Charcadet", TYPE_FIRE, TYPECOUNT, 92, 74, 120, nullptr,
		{ &ATK_Ember, &ATK_Incinerate, &ATK_Flamethrower, &ATK_Heat_Wave, &ATK_Flame_Charge, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  936, "Armarouge", TYPE_FIRE, TYPE_PSYCHIC, 234, 185, 198, "Charcadet",
		{ &ATK_Ember, &ATK_Incinerate, &ATK_Flamethrower, &ATK_Heat_Wave, &ATK_Psyshock, &ATK_Flame_Charge, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  937, "Ceruledge", TYPE_FIRE, TYPE_GHOST, 239, 189, 181, "Charcadet",
		{ &ATK_Ember, &ATK_Incinerate, &ATK_Flamethrower, &ATK_Heat_Wave, &ATK_Shadow_Ball, &ATK_Flame_Charge, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  938, "Tadbulb", TYPE_ELECTRIC, TYPECOUNT, 104, 73, 156, nullptr,
		{ &ATK_Thunder_Shock, &ATK_Water_Gun, &ATK_Discharge, &ATK_Parabolic_Charge, &ATK_Zap_Cannon, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  939, "Bellibolt", TYPE_ELECTRIC, TYPECOUNT, 184, 165, 240, "Tadbulb",
		{ &ATK_Sucker_Punch, &ATK_Thunder_Shock, &ATK_Water_Gun, &ATK_Discharge, &ATK_Parabolic_Charge, &ATK_Zap_Cannon, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // 940 wattrel
  // 941 kilowattrel
  // 942 maschiff
  // 943 mabosstiff
  // the Toxic Mouse
  {  944, "Shroodle", TYPE_POISON, TYPE_NORMAL, 124, 70, 120, nullptr,
		{ &ATK_Poison_Jab, &ATK_Mud_Slap, &ATK_Poison_Fang, &ATK_Sludge_Bomb, &ATK_Acid_Spray, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // the Toxic Monkey
  {  945, "Grafaiai", TYPE_POISON, TYPE_NORMAL, 199, 149, 160, "Shroodle",
		{ &ATK_Poison_Jab, &ATK_Mud_Slap, &ATK_Poison_Fang, &ATK_Sludge_Bomb, &ATK_Acid_Spray, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // 946 bramblin
  // 947 brambleghast
  // the Woodear
  {  948, "Toedscool", TYPE_GRASS, TYPE_GROUND, 97, 149, 120, nullptr,
		{ &ATK_Mud_Slap, &ATK_Mud_Shot, &ATK_Earth_Power, &ATK_Seed_Bomb, &ATK_Wrap, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  949, "Toedscruel", TYPE_GRASS, TYPE_GROUND, 166, 209, 190, "Toedscool",
		{ &ATK_Mud_Slap, &ATK_Mud_Shot, &ATK_Earth_Power, &ATK_Seed_Bomb, &ATK_Acid_Spray, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // 950 klawf
  // 951 capsakid
  // 952 scovillain
  // 953 rellor
  // 954 rabsca
  // 955 flittle
  // 956 espathra
  // the Metalsmith
  {  957, "Tinkatink", TYPE_FAIRY, TYPE_STEEL, 85, 110, 137, nullptr,
		{ &ATK_Rock_Smash, &ATK_Fairy_Wind, &ATK_Flash_Cannon, &ATK_Play_Rough, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Hammer
  {  958, "Tinkatuff", TYPE_FAIRY, TYPE_STEEL, 109, 145, 163, "Tinkatink",
		{ &ATK_Rock_Smash, &ATK_Fairy_Wind, &ATK_Flash_Cannon, &ATK_Play_Rough, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  959, "Tinkaton", TYPE_FAIRY, TYPE_STEEL, 155, 196, 198, "Tinkatuff",
		{ &ATK_Rock_Smash, &ATK_Fairy_Wind, &ATK_Flash_Cannon, &ATK_Play_Rough, &ATK_Bulldoze, &ATK_Heavy_Slam, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Garden Eel
  {  960, "Wiglett", TYPE_WATER, TYPECOUNT, 109, 52, 67, nullptr,
		{ &ATK_Water_Gun, &ATK_Mud_Slap, &ATK_Dig, &ATK_Surf, &ATK_Liquidation, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  961, "Wugtrio", TYPE_WATER, TYPECOUNT, 205, 136, 111, "Wiglett",
		{ &ATK_Water_Gun, &ATK_Mud_Slap, &ATK_Dig, &ATK_Surf, &ATK_Liquidation, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  962, "Bombirdier", TYPE_FLYING, TYPE_DARK, 198, 172, 172, nullptr,
		{ &ATK_Sucker_Punch, &ATK_Wing_Attack, &ATK_Rock_Throw, &ATK_Aerial_Ace, &ATK_Rock_Tomb, &ATK_Payback, &ATK_Fly, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // 963 finizen
  // 964 palafin
  {  965, "Varoom", TYPE_STEEL, TYPE_POISON, 123, 107, 128, nullptr,
		{ &ATK_Lick, &ATK_Poison_Jab, &ATK_Metal_Sound, &ATK_Gunk_Shot, &ATK_Gyro_Ball, &ATK_Acid_Spray, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  966, "Revavroom", TYPE_STEEL, TYPE_POISON, 229, 168, 190, "Varoom",
		{ &ATK_Lick, &ATK_Poison_Jab, &ATK_Metal_Sound, &ATK_Gunk_Shot, &ATK_Gyro_Ball, &ATK_Overheat, &ATK_Acid_Spray, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // 967 cyclizar
  // 968 orthworm
  // 969 glimmet
  // 970 glimmora
  {  971, "Greavard", TYPE_GHOST, TYPECOUNT, 105, 106, 137, nullptr,
		{ &ATK_Bite, &ATK_Lick, &ATK_Dig, &ATK_Shadow_Ball, &ATK_Psychic_Fangs, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  972, "Houndstone", TYPE_GHOST, TYPECOUNT, 186, 195, 176, "Greavard",
		{ &ATK_Bite, &ATK_Lick, &ATK_Dig, &ATK_Shadow_Ball, &ATK_Psychic_Fangs, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // 973 flamigo
  {  974, "Cetoddle", TYPE_ICE, TYPECOUNT, 119, 80, 239, nullptr,
		{ &ATK_Ice_Shard, &ATK_Tackle, &ATK_Powder_Snow, &ATK_Body_Slam, &ATK_Avalanche, &ATK_Heavy_Slam, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  975, "Cetitan", TYPE_ICE, TYPECOUNT, 208, 123, 347, "Cetoddle",
		{ &ATK_Ice_Shard, &ATK_Tackle, &ATK_Powder_Snow, &ATK_Body_Slam, &ATK_Avalanche, &ATK_Heavy_Slam, },
		true, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // 976 is veluza
  // the Big Catfish
  {  977, "Dondozo", TYPE_WATER, TYPECOUNT, 176, 178, 312, nullptr,
		{ &ATK_Water_Gun, &ATK_Waterfall, &ATK_Hydro_Pump, &ATK_Outrage, &ATK_Surf, &ATK_Liquidation, },
    false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  978, "Tatsugiri", TYPE_DRAGON, TYPE_WATER, 226, 166, 169, nullptr,
		{ &ATK_Take_Down, &ATK_Water_Gun, &ATK_Outrage, &ATK_Surf, &ATK_Hydro_Pump, &ATK_Muddy_Water, },
		false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // the Rage Monkey
  {  979, "Annihilape", TYPE_FIGHTING, TYPE_GHOST, 220, 178, 242, "Primeape",
		{ &ATK_Low_Kick, &ATK_Counter, &ATK_Ice_Punch, &ATK_Night_Slash, &ATK_Low_Sweep, &ATK_Shadow_Ball, &ATK_Close_Combat, &ATK_Rage_Fist, },
		true, true, false, { &ATK_Rage_Fist, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  980, "Clodsire", TYPE_POISON, TYPE_GROUND, 127, 151, 277, "Paldean Wooper",
		{ &ATK_Mud_Shot, &ATK_Poison_Sting, &ATK_Megahorn, &ATK_Earthquake, &ATK_Stone_Edge, &ATK_Sludge_Bomb, &ATK_Water_Pulse, &ATK_Acid_Spray, },
		true, false, false, { &ATK_Megahorn, }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  // 981 farigiraf
  // the land snake
  {  982, "Dudunsparce", TYPE_NORMAL, TYPECOUNT, 188, 150, 268, "Dunsparce",
		{ &ATK_Bite, &ATK_Astonish, &ATK_Rollout, &ATK_Dig, &ATK_Drill_Run, &ATK_Rock_Slide, },
		false, false, false, { }, species::CAT_NORMAL, 50, nullptr, species::EVOL_NOITEM, },
  {  983, "Kingambit", TYPE_DARK, TYPE_STEEL, 238, 203, 225, "Bisharp",
		{ &ATK_Metal_Claw, &ATK_Snarl, &ATK_Metal_Sound, &ATK_Dark_Pulse, &ATK_Iron_Head, &ATK_X_Scissor, &ATK_Focus_Blast, &ATK_Foul_Play, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  // 984 is great tusk
  // 985 is scream tail
  // 986 is brute bonnet
  // 987 is flutter mane
  // 988 is slither wing
  // 989 is sandy shocks
  // 990 is iron treads
  // 991 is iron bundle
  // 992 is iron hands
  // 993 is iron jugulis
  // 994 is iron moth
  // 995 is iron thorns
  {  996, "Frigibax", TYPE_DRAGON, TYPE_ICE, 134, 86, 163, nullptr,
		{ &ATK_Dragon_Breath, &ATK_Ice_Fang, &ATK_Dragon_Claw, &ATK_Icy_Wind, &ATK_Avalanche, &ATK_Outrage, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  997, "Arctibax", TYPE_DRAGON, TYPE_ICE, 173, 128, 207, "Frigibax",
		{ &ATK_Dragon_Breath, &ATK_Ice_Fang, &ATK_Dragon_Claw, &ATK_Icy_Wind, &ATK_Avalanche, &ATK_Outrage, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  998, "Baxcalibur", TYPE_DRAGON, TYPE_ICE, 254, 168, 229, "Arctibax",
		{ &ATK_Dragon_Breath, &ATK_Ice_Fang, &ATK_Blizzard, &ATK_Dragon_Claw, &ATK_Icy_Wind, &ATK_Avalanche, &ATK_Outrage, },
		true, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_NOITEM, },
  {  999, "Gimmighoul", TYPE_GHOST, TYPECOUNT, 140, 76, 128, nullptr,
		{ &ATK_Astonish, &ATK_Shadow_Ball, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_GIMMICOINS, },
  {  1000, "Gholdengo", TYPE_STEEL, TYPE_GHOST, 252, 190, 202, "Gimmighoul",
		{ &ATK_Astonish, &ATK_Hex, &ATK_Power_Gem, &ATK_Shadow_Ball, &ATK_Dazzling_Gleam, &ATK_Focus_Blast, },
		true, false, false, { }, species::CAT_NORMAL, 10, nullptr, species::EVOL_NOITEM, },
  //{ 1001, "Wo-Chien", TYPE_DARK, TYPE_GRASS, 186, 242, 198, nullptr, },
  //{ 1002, "Chien-Pao", TYPE_DARK, TYPE_ICE, 261, 167, 190, nullptr, },
  //{ 1003, "Ting-Lu", TYPE_DARK, TYPE_GROUND, 194, 203, 321, nullptr, },
  //{ 1004, "Chi-Yu", TYPE_DARK, TYPE_FIRE, 269, 221, 146, nullptr, },
  //{ 1005, "Roaring Moon", TYPE_DRAGON, TYPE_DARK, 280, 196, 233, nullptr, },
  //{ 1006, "Iron Valiant", TYPE_FAIRY, TYPE_FIGHTING, 279, 171, 179, nullptr, },
  //{ 1007, "Koraidon", TYPE_FIGHTING, TYPE_DRAGON, 263, 223, 205, nullptr, },
  //{ 1008, "Miraidon", TYPE_ELECTRIC, TYPE_DRAGON, 263, 223, 205, nullptr, },
  /*
  { 1009, "Walking Wake", TYPE_WATER, TYPE_DRAGON, 256, 188, 223, nullptr,
    { },
    false, false, false, { }, species::CAT_PARADOX, 100, nullptr, species::EVOL_NOITEM, },
  { 1010, "Iron Leaves", TYPE_GRASS, TYPE_PSYCHIC, 259, 213, 207, nullptr,
    { },
    false, false, false, { }, species::CAT_PARADOX, 100, nullptr, species::EVOL_NOITEM, },
    */
  { 1011, "Dipplin", TYPE_GRASS, TYPE_DRAGON, 173, 184, 190, "Applin",
    { &ATK_Dragon_Tail, &ATK_Bullet_Seed, &ATK_Rollout, &ATK_Seed_Bomb, &ATK_Dragon_Pulse, &ATK_Outrage, },
    false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_SWEETAPPLE, },
  //{ 1012, "Poltchageist", TYPE_GRASS, TYPE_GHOST, nullptr, },
  //{ 1013, "Sinistcha", TYPE_GRASS, TYPE_GHOST, nullptr, },
  //{ 1014, "Okidogi", TYPE_POISON, TYPE_FIGHTING, nullptr, },
  //{ 1015, "Munkidori", TYPE_POISON, TYPE_PSYCHIC, nullptr, },
  //{ 1016, "Fezandipti", TYPE_POISON, TYPE_FAIRY, nullptr, },
  //{ 1018, "Archaludon", TYPE_STEEL, TYPE_DRAGON, 250, 215, 207, "Duraludon", },
  { 1019, "Hydrapple", TYPE_GRASS, TYPE_DRAGON, 216, 186, 235, "Dipplin",
    { &ATK_Dragon_Tail, &ATK_Bullet_Seed, &ATK_Rollout, &ATK_Seed_Bomb, &ATK_Dragon_Pulse, &ATK_Outrage, },
    false, false, false, { }, species::CAT_NORMAL, 75, nullptr, species::EVOL_SYRUPYAPPLE, },
  //{ 1020, "Gouging Fire", TYPE_FIRE, TYPE_DRAGON, nullptr, },
  //{ 1021, "Raging Bolt", TYPE_ELECTRIC, TYPE_DRAGON, nullptr, },
  //{ 1022, "Iron Boulder", TYPE_ROCK, TYPE_PSYCHIC, nullptr, },
  //{ 1023, "Iron Crown", TYPE_STEEL, TYPE_PSYCHIC, nullptr, },
  //{ 1025, "Pecharunt", TYPE_POISON, TYPE_GHOST, nullptr, },
};

#define SPECIESCOUNT (sizeof(sdex) / sizeof(*sdex))

struct mega {
  unsigned idx; // pokedex index, not unique
  std::string name;
  pgo_types_e t1, t2;
  unsigned atk;
  unsigned def;
  unsigned sta;

  mega() {
  }

  mega(const std::string& s)
      : name(s) {
  }

  mega(unsigned i, const char *n, pgo_types_e T1, pgo_types_e T2,
          unsigned A, unsigned D, unsigned S)
    : idx(i),
    name(n),
    t1(T1),
    t2(T2),
    atk(A),
    def(D),
    sta(S)
  { }
};

// mega and primal forms are never shadows
static const mega megasdex[] = {
  {  3, "Mega Venusaur", TYPE_GRASS, TYPE_POISON, 241, 246, 190, },
  {  6, "Mega Charizard X", TYPE_FIRE, TYPE_DRAGON, 273, 213, 186, },
  {  6, "Mega Charizard Y", TYPE_FIRE, TYPE_FLYING, 319, 212, 186, },
  {  9, "Mega Blastoise", TYPE_WATER, TYPECOUNT, 264, 237, 188, },
  {  15, "Mega Beedrill", TYPE_BUG, TYPE_POISON, 303, 148, 163, },
  {  18, "Mega Pidgeot", TYPE_NORMAL, TYPE_FLYING, 280, 175, 195, },
  {  65, "Mega Alakazam", TYPE_PSYCHIC, TYPECOUNT, 367, 207, 146, },
  {  80, "Mega Slowbro", TYPE_WATER, TYPE_PSYCHIC, 224, 259, 216, },
  {  94, "Mega Gengar", TYPE_GHOST, TYPE_POISON, 349, 199, 155, },
  {  115, "Mega Kangaskhan", TYPE_NORMAL, TYPECOUNT, 246, 210, 233, },
  {  127, "Mega Pinsir", TYPE_BUG, TYPE_FLYING, 305, 231, 163, },
  {  130, "Mega Gyarados", TYPE_WATER, TYPE_DARK, 292, 247, 216, },
  {  142, "Mega Aerodactyl", TYPE_ROCK, TYPE_FLYING, 292, 210, 190, },
  {  181, "Mega Ampharos", TYPE_ELECTRIC, TYPE_DRAGON, 294, 203, 207, },
  {  208, "Mega Steelix", TYPE_STEEL, TYPE_GROUND, 212, 327, 181, },
  {  212, "Mega Scizor", TYPE_BUG, TYPE_STEEL, 279, 250, 172, },
  {  214, "Mega Heracross", TYPE_BUG, TYPE_FIGHTING, 334, 223, 190, },
  {  229, "Mega Houndoom", TYPE_DARK, TYPE_FIRE, 289, 194, 181, },
  {  248, "Mega Tyranitar", TYPE_ROCK, TYPE_DARK, 309, 276, 225, },
  {  254, "Mega Sceptile", TYPE_GRASS, TYPE_DRAGON, 320, 186, 172, },
  {  257, "Mega Blaziken", TYPE_FIRE, TYPE_FIGHTING, 329, 168, 190, },
  {  260, "Mega Swampert", TYPE_WATER, TYPE_GROUND, 283, 218, 225, },
  {  282, "Mega Gardevoir", TYPE_PSYCHIC, TYPE_FAIRY, 326, 229, 169, },
  {  302, "Mega Sableye", TYPE_DARK, TYPE_GHOST, 151, 216, 137, },
  {  303, "Mega Mawile", TYPE_STEEL, TYPE_FAIRY, 188, 217, 137, },
  {  306, "Mega Aggron", TYPE_STEEL, TYPECOUNT, 247, 331, 172, },
  {  308, "Mega Medicham", TYPE_FIGHTING, TYPE_PSYCHIC, 205, 179, 155, },
  {  310, "Mega Manectric", TYPE_ELECTRIC, TYPECOUNT, 286, 179, 172, },
  {  319, "Mega Sharpedo", TYPE_WATER, TYPE_DARK, 289, 144, 172, },
  {  323, "Mega Camerupt", TYPE_FIRE, TYPE_GROUND, 253, 183, 172, },
  {  334, "Mega Altaria", TYPE_DRAGON, TYPE_FAIRY, 222, 218, 181, },
  {  354, "Mega Banette", TYPE_GHOST, TYPECOUNT, 312, 160, 162, },
  {  359, "Mega Absol", TYPE_DARK, TYPECOUNT, 314, 130, 163, },
  {  362, "Mega Glalie", TYPE_ICE, TYPECOUNT, 252, 168, 190, },
  {  373, "Mega Salamence", TYPE_DRAGON, TYPE_FLYING, 310, 251, 216, },
  {  376, "Mega Metagross", TYPE_STEEL, TYPE_PSYCHIC, 300, 289, 190, },
  {  380, "Mega Latias", TYPE_DRAGON, TYPE_PSYCHIC, 289, 297, 190, },
  {  381, "Mega Latios", TYPE_DRAGON, TYPE_PSYCHIC, 335, 241, 190, },
  {  382, "Primal Kyogre", TYPE_WATER, TYPECOUNT, 353, 268, 218, },
  {  383, "Primal Groudon", TYPE_GROUND, TYPE_FIRE, 353, 268, 218, },
  {  384, "Mega Rayquaza", TYPE_DRAGON, TYPE_FLYING, 377, 210, 227, },
  {  428, "Mega Lopunny", TYPE_NORMAL, TYPE_FIGHTING, 282, 214, 163, },
  {  445, "Mega Garchomp", TYPE_DRAGON, TYPE_GROUND, 339, 222, 239, },
  {  448, "Mega Lucario", TYPE_FIGHTING, TYPE_STEEL, 310, 175, 172, },
  {  460, "Mega Abomasnow", TYPE_GRASS, TYPE_ICE, 240, 191, 207, },
  {  475, "Mega Gallade", TYPE_PSYCHIC, TYPE_FIGHTING, 326, 230, 169, },
  {  531, "Mega Audino", TYPE_NORMAL, TYPE_FAIRY, 147, 239, 230, },
  {  719, "Mega Diancie", TYPE_ROCK, TYPE_FAIRY, 342, 235, 137, },
};

#define MEGACOUNT (sizeof(megasdex) / sizeof(*megasdex))

// FIXME get rid of this
static const struct spokedex {
  const species *dex;
  unsigned dcount;
} sdexen[] = {
  { sdex, SPECIESCOUNT, },
  { NULL, 0, }
};

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
calc_amean(float effa, float effd, unsigned mhp){
  return (effa + effd + mhp) / 3;
}

static inline float
calc_gmean(float effa, float effd, unsigned mhp){
  return cbrt(effa * effd * mhp);
}

static inline float
calc_ppe(const attack *a){
  return a->powertrain / (float)-a->energytrain;
}

static inline unsigned
calc_max_cp(const species *s){
  return calccp(s->atk, s->def, s->sta, MAX_HALFLEVEL);
}

struct stats {
  const species* s;
  unsigned hlevel;          // halflevel 1..99
  unsigned ia, id, is;      // individual vector components
  float effa, effd;         // effective attack and defense
  unsigned mhp;             // max hit points
  int cp;                   // combat power
  float average;            // arithemetic mean of effa, effd, mhp
  float geommean;           // geometric mean of effa, effd, mhp
  float apercent;           // geommean advantage over pessimal level-maxed iv
  bool shadow;              // is this the shadow variant?
  struct stats* next;

  stats(){
    s = nullptr;
  }

  stats(const species *S, unsigned Hlevel, unsigned IA, unsigned ID, unsigned IS, bool Shadow) :
    s(S),
    hlevel(Hlevel),
    ia(IA),
    id(ID),
    is(IS),
    shadow(Shadow)
  {
    unsigned moda = s->atk + ia;
    unsigned modd = s->def + id;
    unsigned mods = s->sta + is;
    effa = calc_eff_a(moda, hlevel, Shadow);
    effd = calc_eff_d(modd, hlevel, Shadow);
    mhp = calc_mhp(mods, hlevel);
    cp = calccp(moda, modd, mods, hlevel);
    average = calc_amean(effa, effd, mhp);
    geommean = calc_gmean(effa, effd, mhp);
  }

  float bulk() const {
    return sqrt(mhp * effd);
  }

};

static inline void
summarize_stat(const stats &st){
  std::cout << st.geommean << " " << st.average << " ";
  std::cout << st.cp;
  std::cout << " " << st.effa << " " << st.effd << " " << st.mhp << " ";
  unsigned half;
  unsigned l = halflevel_to_level(st.hlevel, &half);
  std::cout << "\\ivlev{" << st.ia << "}{" << st.id << "}{" << st.is << "}{" << l;
  if(half){
    std::cout << ".5";
  }
  std::cout << "}";
  if(st.shadow){
    std::cout << " \\shadow";
  }
  std::cout << std::endl;
}

static inline int
statscmp_gmean(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  return st1->geommean < st2->geommean ? -1 :
          st1->geommean > st2->geommean ? 1 : 0;
}

static inline int
statscmp_amean(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  return st1->average < st2->average ? -1 :
          st1->average > st2->average ? 1 : 0;
}

static inline int
statscmp_atk(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  return st1->effa < st2->effa ? -1 :
          st1->effa > st2->effa ? 1 : 0;
}

static inline int
statscmp_def(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  return st1->effd < st2->effd ? -1 :
          st1->effd > st2->effd ? 1 : 0;
}

static inline int
statscmp_mhp(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  return st1->mhp < st2->mhp ? -1 :
          st1->mhp > st2->mhp ? 1 : 0;
}

static inline int
statscmp_bulk(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  const float b1 = st1->bulk();
  const float b2 = st2->bulk();
  return b1 < b2 ? -1 : b1 > b2 ? 1 : 0;
}

// FIXME binary search on it
static unsigned
maxlevel_cp_bounded(unsigned atk, unsigned def, unsigned sta, int cpceil, int *cp){
  unsigned lastgood = 0;
  *cp = 0;
  for(unsigned hl = 1 ; hl <= MAX_HALFLEVEL ; ++hl){
    int tmpc = calccp(atk, def, sta, hl);
    if(tmpc <= cpceil || cpceil <= 0){
      lastgood = hl;
      *cp = tmpc;
    }else{
      break;
    }
  }
  return lastgood;
}

static inline float
calc_pok_amean(const stats *s){
  return calc_amean(s->effa, s->effd, s->mhp);
}

static inline float
calc_pok_gmean(const stats *s){
  return calc_gmean(s->effa, s->effd, s->mhp);
}

static inline float
calc_pok_bulk(const stats *s){
  return s->bulk();
}

static void
order_ivs_internal(const species *s, int cpceil, stats *svec, bool shadow){
  unsigned idx = 0;
  for(int iva = 0 ; iva < 16 ; ++iva){
    for(int ivd = 0 ; ivd < 16 ; ++ivd){
      for(int ivs = 0 ; ivs < 16 ; ++ivs){
        auto &st = svec[idx];
        const unsigned moda = s->atk + iva;
        const unsigned modd = s->def + ivd;
        const unsigned mods = s->sta + ivs;
        st.hlevel = maxlevel_cp_bounded(moda, modd, mods, cpceil, &st.cp);
        st.s = s;
        st.effa = calc_eff_a(moda, st.hlevel, shadow);
        st.effd = calc_eff_d(modd, st.hlevel, shadow);
        st.mhp = calc_mhp(mods, st.hlevel);
        st.ia = iva;
        st.id = ivd;
        st.is = ivs;
        st.shadow = shadow;
        st.geommean = calc_pok_gmean(&st);
        st.average = calc_pok_amean(&st);
        ++idx;
      }
    }
  }
}

static constexpr unsigned IVLEVVEC =
  (MAXIVELEM + 1) * (MAXIVELEM + 1) * (MAXIVELEM + 1);

// generate the stats for each of 4,096 possible IVs at their maximum level
// subject to the cpceiling (-1 for no ceiling), ordered according to fitfxn.
static inline stats *
order_ivs(const species *s, int cpceil, int(*cmpfxn)(const void*, const void*),
          unsigned *vcount){
  *vcount = IVLEVVEC;
  if(s->shadow){
    *vcount *= 2;
  }
  stats *svec = new stats[*vcount];
  order_ivs_internal(s, cpceil, svec, false);
  if(s->shadow){
    order_ivs_internal(s, cpceil, svec + IVLEVVEC, true);
  }
  qsort(svec, *vcount, sizeof(*svec), cmpfxn);
  return svec;
}

// instantiation of a pokémon -- species, IVs, and known attacks
struct pmon { // static elements
  struct stats s;
  const attack *fa, *ca1, *ca2;
};

static inline float
calc_fitfxn(float (*fitfxn)(const stats *), const species *s,
            unsigned ia, unsigned id, unsigned is, unsigned hl,
            bool shadow){
  stats st{s, hl, ia, id, is, shadow};
//std::cerr << s->name << " " << ia << " " << id << " " << is << " " << hl << std::endl;
  return fitfxn(&st);
}

// optimize on comparable fitness function subject to floor
static int
update_optset(stats** osets, const species* s, unsigned ia, unsigned id,
              unsigned is, unsigned hl, float floor, float* minfit,
              bool isshadow, float (*fitfxn)(const stats *)){
  stats **prev = osets;
  stats *cur;
  float m = calc_fitfxn(fitfxn, s, ia, id, is, hl, isshadow);
  unsigned moda = s->atk + ia;
  unsigned modd = s->def + id;
  float effa = calc_eff_a(moda, hl, isshadow);
  float effd = calc_eff_d(modd, hl, isshadow);
  unsigned mods = s->sta + is;
  unsigned mhp = calc_mhp(mods, hl);
  if(m < *minfit || *minfit <= 0){
    *minfit = m;
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
  cur = new stats(s, hl, ia, id, is, isshadow);
  cur->next = *prev;
  *prev = cur;
  return 0;
}

// returns the optimal levels+ivs (using provided comparable fitness function)
// with a CP less than or equal to cpceil and fitness function greater than or
// equal to floor.
// FIXME if last argument is true, we want calc_amean(), otherwise calc_gmean()
stats *find_optimal_set(const species* s, int cpceil, float floor, bool isshadow, float(*fitfxn)(const stats *)){
  stats* optsets = NULL;
  float minfit = -1;
  for(int iva = 0 ; iva < 16 ; ++iva){
    for(int ivd = 0 ; ivd < 16 ; ++ivd){
      for(int ivs = 0 ; ivs < 16 ; ++ivs){
        int cp;
        unsigned hl = maxlevel_cp_bounded(s->atk + iva, s->def + ivd, s->sta + ivs, cpceil, &cp);
        if(update_optset(&optsets, s, iva, ivd, ivs, hl, floor, &minfit, isshadow, fitfxn) < 0){
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
    float m = fitfxn(cur);
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
    ss->apercent = (fitfxn(ss) / minfit - 1.0) * 100;
  }
  return collectopt;
}

static inline pgo_types_e
lookup_type(const char *t){
  for(unsigned i = 0 ; i < TYPECOUNT ; ++i){
    if(strcasecmp(t, tnames[i]) == 0){
      return static_cast<pgo_types_e>(i);
    }
  }
  return TYPECOUNT;
}

const species* lookup_species(const char* name){
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    if(strcasecmp(sdex[i].name.c_str(), name) == 0){
      return &sdex[i];
    }
  }
  return NULL;
}

const species* lookup_species(unsigned idx){
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    if(sdex[i].idx == idx){
      return &sdex[i];
    }
  }
  return NULL;
}

// FIXME there can be more than one mega form! (e.g. charizard x and y)
const mega* lookup_mega(const char* name){
  const species *s = lookup_species(name);
  if(s){
    for(unsigned i = 0 ; i < MEGACOUNT ; ++i){
      if(megasdex[i].idx == s->idx){
        return &megasdex[i];
      }
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
    printf("\\calign{\\includegraphics[height=1.5em,keepaspectratio]{images/%s.png}}", tnames[t]);
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

static unsigned
learner_count_sdex(const attack *as, const species *dex, unsigned dcount){
  unsigned count = 0;
  for(unsigned u = 0 ; u < dcount ; ++u){
    const species* s = &dex[u];
    for(const auto &a : s->attacks){
      if(strcmp(a->name, as->name) == 0){
        ++count;
        break;
      }
    }
  }
  return count;
}

// how many species can learn this attack? we don't count mega/primal forms.
unsigned learner_count(const attack* as){
  unsigned count = 0;
  //for(auto &sd : sdexen){
    count += learner_count_sdex(as, sdex, SPECIESCOUNT);
  //}
  return count;
}

// used for species cards, always wants geometric mean
static void
print_optimal_latex(const species* sp){
  stats* s = find_optimal_set(sp, 2500, 0, false, calc_pok_gmean);
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
    s = find_optimal_set(sp, 1500, 0, false, calc_pok_gmean);
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

static inline int
escape_cpp_string(const std::string &s){
  for(char c : s){
    if(c != '%'){
      std::cout << c;
    }else{
      std::cout << "\\%";
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
    if(megasdex[i].idx == s->idx){
      return true;
    }
  }
  return false;
}

// does this species have a Dynamax form?
static bool
has_dmax(const species *s){
  return s->dmax;
}

// does this species have a Gigantamax form?
static bool
has_gmax(const species *s){
  return !!s->gmax;
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
}

static int
print_icons(const species *s, bool doprint, bool ismega){
  int count = 0;
  if(has_mega(s) && !ismega){
    ++count;
    if(doprint){
      printf("\\calign{\\includegraphics[height=2em,keepaspectratio]{images/mega.png}}");
    }
  }
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
  if(buff > 0){
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

static void
summarize_buffs(const attack *a){
  // need special case A+D as it takes too much space otherwise
  if(a->chance_user_attack && a->chance_user_attack == a->chance_user_defense
      && a->user_attack == a->user_defense){
    print_buff(a->chance_user_attack, a->user_attack, "A+D");
  }else{
    if(a->chance_user_attack){
      print_buff(a->chance_user_attack, a->user_attack, "A");
    }
    if(a->chance_user_defense){
      print_buff(a->chance_user_defense, a->user_defense, "D");
    }
  }
  if(a->chance_opp_attack && a->chance_opp_attack == a->chance_opp_defense
      && a->opp_attack == a->opp_defense){
    print_buff(a->chance_opp_attack, a->opp_attack, "OA+D");
  }else{
    if(a->chance_opp_attack){
      print_buff(a->chance_opp_attack, a->opp_attack, "OA");
    }
    if(a->chance_opp_defense){
      print_buff(a->chance_opp_defense, a->opp_defense, "OD");
    }
  }
}

static inline unsigned
get_stage(const species *s){
  const species *devol = get_previous_evolution(s);
  if(!devol){
    return 1;
  }
  return get_stage(devol) + 1;
}

static inline unsigned
stardust_reward(const species *s){
  if(s->name == "Audino"){
    return 2100;
  }else if(s->name == "Cloyster"){
    return 1200;
  }else if(s->name == "Shellder" || s->name == "Chimecho"){
    return 1000;
  }
  static const char *sd950s[] = {
    "Alolan Persian",
    "Starmie",
    "Vespiquen",
    "Garbodor",
    nullptr
  };
  for(const char **n = sd950s ; *n ; ++n){
    if(strcmp(s->name.c_str(), *n) == 0){
      return 950;
    }
  }
  static const char *sd750s[] = {
    "Alolan Meowth",
    "Staryu",
    "Sableye",
    "Combee",
    "Trubbish",
  };
  for(const char **n = sd750s ; *n ; ++n){
    if(strcmp(s->name.c_str(), *n) == 0){
      return 750;
    }
  }
  static const char *sd700s[] = {
    "Parasect",
    "Persian",
    "Breloom",
    "Amoonguss",
    "Shiinotic",
  };
  for(const char **n = sd700s ; *n ; ++n){
    if(strcmp(s->name.c_str(), *n) == 0){
      return 700;
    }
  }
  static const char *sd500s[] = {
    "Paras",
    "Meowth",
    "Delibird",
    "Shroomish",
    "Foongus",
    "Morelull",
  };
  for(const char **n = sd500s ; *n ; ++n){
    if(strcmp(s->name.c_str(), *n) == 0){
      return 500;
    }
  }
  auto stage = get_stage(s);
  if(stage == 3){
    return 500;
  }else if(stage == 2){
    return 300;
  }
  return 100;
}

static inline void
emit_attack(const species *s, const attack *a){
  bool stab = has_stab_p(s, a);
  bool excl = exclusive_attack_p(s, a);
  if(!stab){
    std::cout << "\\textit{";
  }
  if(excl){
    std::cout << "\\textbf{";
  }
  std::cout << a->name;
  if(a->user_attack || a->user_defense || a->opp_attack || a->opp_defense){
    std::cout << " ";
  }
  summarize_buffs(a);
  if(!stab){
    std::cout << "}";
  }
  if(excl){
    std::cout << "}";
  }
}

void print_species_latex(const species* s, bool overzoom, bool bg, bool mainform){
  printf("\\vfill\n");
  // these are fragile inferences, yuck
  bool gmax = !overzoom && s->gmax;
  bool ismega = overzoom && !mainform; // maybe do name check?
  printf("\\begin{speciesbox}[title=\\#%04u ", s->idx);
  if(gmax){
    printf("Gmax ");
  }
  // have to special case this or it runs two lines =[
  if(s->name == "Crowned Shield Zamazenta"){
    escape_string("Crowned Zamazenta");
  }else{
    escape_string(s->name.c_str());
  }
  if(mainform){
    printf(",before title={\\phantomsection\\label{species:");
    label_string(s->name.c_str());
    printf("}}");
  }
  printf(",title style={left color=%s,right color=%s},after title={",
          TNames[s->t1], s->t2 == TYPECOUNT ? TNames[s->t1] : TNames[s->t2]);
  if(s->shiny){
    printf("\\calign{\\includegraphics[height=1em,keepaspectratio]{images/shiny.png}}");
  }
  float avg = calc_amean(s->atk, s->def, s->sta);
  printf("\\hfill%u %u %u %.1f %.1f}", s->atk, s->def, s->sta, avg, calc_gmean(s->atk, s->def, s->sta));
  //if(overzoom){
    printf(",interior style={fill overzoom image=images/highres/");
    escape_filename(s->name.c_str());
    printf(",fill image opacity=0.2}");
  //}
  printf("]{\\footnotesize");

  if(bg){
    printf("\\pagecolor{%s!25!white}", TNames[s->t1]);
  }
  // the table containing image and attack data
  printf("\\begin{tabularx}{\\linewidth}{@{}c X @{}}");
  printf("\\includegraphics[width=0.3\\linewidth,valign=c,keepaspectratio]{images/highres/");
  if(gmax){ // get the gmax image
    printf("Gmax ");
  }
  for(const char* curs = s->name.c_str() ; *curs ; ++curs){
    if(*curs != '%' && *curs != '\''){
      printf("%c", *curs);
    }
  }
  printf(".png} &\\begingroup\\setlength{\\tabcolsep}{4pt}\\begin{tabular}{lrrrrr}\n");
  for(const auto &a : s->attacks){
    unsigned stab = has_stab_p(s, a);
    float power = a->powertrain;
    if(stab){
      power = power * 6 / 5;
    }
    print_type(a->type);
    if(a->type == TYPECOUNT){
      printf("\\hspace{1em}");
    }
    const char *itb, *ite;
    if(has_stab_p(s, a)){
      itb = ite = "";
    }else{
      itb = "\\textit{";
      ite = "}";
    }
    if(charged_attack_p(a)){
      const float dpe = power / -a->energytrain;
      if(exclusive_attack_p(s, a)){
        printf(" \\textbf{%s%s%s} & & \\textbf{%s%g%s} & \\textbf{%s%d%s} & \\textbf{%s%.2f%s} &",
            itb, a->name, ite, itb, power, ite, itb, a->energytrain, ite, itb, dpe, ite);
      }else{
        printf(" %s%s%s & & %s%g%s & %s%d%s & %s%.2f%s &",
            itb, a->name, ite, itb, power, ite, itb, a->energytrain, ite, itb, dpe, ite);
      }
      if(a->chance_user_attack || a->chance_user_defense ||
          a->chance_opp_attack || a->chance_opp_defense){
        printf("{\\scriptsize{}");
        if(exclusive_attack_p(s, a)){
          printf("\\textbf{");
        }
        if(!has_stab_p(s, a)){
          printf("\\textit{");
        }
        summarize_buffs(a);
        if(!has_stab_p(s, a)){
          printf("}");
        }
        if(exclusive_attack_p(s, a)){
          printf("}");
        }
        printf("}");
      }
      printf("\\\\\n");
    }else{ // fast attacks
      const float dpt = power / a->turns;
      const float ept = static_cast<float>(a->energytrain) / a->turns;
      if(exclusive_attack_p(s, a)){
        printf(" \\textbf{%s%s%s} & \\textbf{%s%u%s} & \\textbf{%s%g%s} & \\textbf{%s%d%s} & \\textbf{%s%.2f%s} & \\textbf{%s%.2f%s}\\\\\n",
            itb, a->name, ite, itb, a->turns, ite,
            itb, power, ite, itb, a->energytrain, ite,
            itb, dpt, ite, itb, ept, ite);
      }else{
        printf(" %s%s%s & %s%u%s & %s%g%s & %s%d%s & %s%.2f%s & %s%.2f%s \\\\\n",
            itb, a->name, ite, itb, a->turns, ite,
            itb, power, ite, itb, a->energytrain, ite,
            itb, dpt, ite, itb, ept, ite);
      }
    }
  }
  printf("\\end{tabular}\\endgroup\\end{tabularx}\n");

  // the minipages with icons and cp data
  // left side is larger for gmax/mega, which don't show evolutionary lines
  printf("\\noindent\\begin{minipage}{0.%d\\linewidth}", gmax ? 4 : 3);
  int largeicons = print_types_big(s->t1, s->t2);
  // for the gmax cards, don't print the max icons --- we know it's max-capable
  if(!gmax){
    if(has_gmax(s)){
      ++largeicons;
      printf(" \\calign{\\includegraphics[height=1.5em,keepaspectratio]{images/gigantamax.png}}");
    }
    if(has_dmax(s)){
      ++largeicons;
      printf(" \\calign{\\includegraphics[height=1.5em,keepaspectratio]{images/dynamax.png}}");
    }
    // we never want some small icons on both lines if we have to have two lines,
    // but we want only one line if we can get away with it. four small icons
    // are too many to put with two large icons.
    // we never need split the line for gmax cards.
    if(largeicons + print_icons(s, false, ismega) > 5){
      printf("\\\\");
    }
    print_icons(s, true, ismega);
    printf("\\scriptsize{}");
    if(s->categorystr() && strcmp(s->categorystr(), "")){
      printf("%s\n", s->categorystr());
    }
    //printf("Max CP: %u\n", calc_max_cp(s));
  }
  printf("\\end{minipage}\n");
  if(mainform){ // evolutionary lineage (only for main forms)
    printf("\\begin{minipage}{0.%d\\linewidth}\\raggedleft{}", gmax ? 6 : 7);
    printf("\\vfill{}\\scriptsize{}");
    print_optimal_latex(s);
    printf("\\end{minipage}\\\\");

    printf("\\scriptsize{}");
    printf("%u CG %d Gen %s %s\\hfill{}", stardust_reward(s), a2cost_to_cgroup(s->a2cost),
                idx_to_generation(s->idx), idx_to_region(s->idx));
    const species *devol = get_previous_evolution(s);
    const species *evol = get_persistent_evolution(s);
    if(devol || evol){
      if(devol){
        print_previous_species(devol);
      }
      printf("\\textbf{");
      escape_string(s->name.c_str());
      printf("}");
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
  }else{ // other than main forms
    if(gmax){
      printf("\\hfill");
      print_type(s->t1);
      printf(" %s", s->gmax);
    }
  }

  // shadow is implemented as subtitle
  if(mainform && s->shadow){
    printf("\\tcbsubtitle[before skip=1pt,bottomrule=0pt]{Shadow ");
    escape_string(s->name.c_str());
    printf("\\hfill{}");
    const float atk = s->atk * 6 / 5.0;
    const float def = s->def * 5 / 6.0;
    const float savg = calc_amean(atk, def, s->sta);
    // we don't show geometric mean as it'll always be the same as the non-shadow
    // form. instead, show delta for arithmetic mean.
    printf("%g %g %u %.1f ", atk, def, s->sta, savg);
    if(avg > savg){
      printf("(-%.1f\\%%)", (avg - savg) * 100 / avg);
    }else if(avg < savg){
      printf("(+%.1f\\%%)", (savg - avg) * 100 / avg);
    }else{
      printf("(no change)");
    }
    printf("}\n");
  }

  printf("}");
  printf("\\end{speciesbox}\n");
}

// print those entries containing type(s). pass TYPECOUNT for a wildcard on t2.
// pass the same type twice for only that base type. LaTeX output.
void filter_by_types(int t1, int t2, const species* dex, unsigned count, bool overzoom, bool mainform){
  for(unsigned i = 0 ; i < count ; ++i){
    bool printit = false;
    if(dex[i].t1 == t1){
      if(dex[i].t2 == t2 || t2 == TYPECOUNT || (t1 == t2 && dex[i].t2 == TYPECOUNT)){
        printit = true;
      }
    }
    if(printit){
      print_species_latex(&dex[i], overzoom, true, mainform);
    }
  }
}

// return the named charged attack iff s can learn it
const attack *species_charged_attack(const species *s, const char *aname){
  for(const auto &a : s->attacks){
    if(a->energytrain >= 0){
      continue;
    }
    if(strcasecmp(aname, a->name)){
      continue;
    }
    return a;
  }
  return NULL;
}

// return the named fast attack iff s can learn it
const attack *species_fast_attack(const species *s, const char *aname){
  for(const auto &a : s->attacks){
    if(a->energytrain <= 0){
      continue;
    }
    if(strcasecmp(aname, a->name)){
      continue;
    }
    return a;
  }
  return NULL;
}

// lex out iv and level in the form iva-ivd-ivs@l or optCP
static int
lex_ivlevel(const char* ivl, stats* s, bool shadow){
  int r;
  unsigned cp;
  // allow leading whitespace
  while(isspace(*ivl)){
    ++ivl;
  }
  if((r = sscanf(ivl, "opt%u", &cp)) == 1){
    stats *st = find_optimal_set(s->s, cp, 0, shadow, calc_pok_gmean);
    if(st == NULL){
      fprintf(stderr, "couldn't find optimal config for cp %u\n", cp);
      return -1;
    }
    s->ia = st->ia;
    s->id = st->id;
    s->is = st->is;
    s->hlevel = st->hlevel;
  }else if(strcmp(ivl, "max") == 0){
    s->ia = s->id = s->is = MAXIVELEM;
    s->hlevel = MAX_HALFLEVEL;
  }else if((r = sscanf(ivl, " %u-%u-%u@", &s->ia, &s->id, &s->is)) == 3){
    ivl = strchr(ivl, '@');
    if(!ivl || !isdigit(*++ivl)){
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

static const attack *
lex_species_charged_attacks(const species *s, const char *spec, const attack **ca2){
  *ca2 = NULL;
  const char *sep = strchr(spec, '/');
  if(sep){
    char *fspec = strndup(spec, sep - spec);
    const attack *ca1 = species_charged_attack(s, fspec);
    *ca2 = species_charged_attack(s, sep + 1);
    free(fspec);
    if(*ca2 == NULL){
      return NULL;
    }
    return ca1;
  }
  const attack *ca1 = species_charged_attack(s, spec);
  return ca1;
}

// fill in a stats structure given only species, IVs, and level
static void
fill_stats(stats* s, bool shadow){
  // these are actually a_raw and d_raw, used only for CMP (so can we kill effd?)
  s->effa = calc_eff_a(s->s->atk + s->ia, s->hlevel, false);
  s->effd = calc_eff_d(s->s->def + s->id, s->hlevel, false);
  s->mhp = calc_mhp(s->s->sta + s->is, s->hlevel);
  s->geommean = calc_gmean(s->effa, s->effd, s->mhp);
  s->average = calc_amean(s->effa, s->effd, s->mhp);
  s->cp = calccp(s->s->atk + s->ia, s->s->def + s->id, s->s->sta + s->is, s->hlevel);
  s->shadow = shadow;
  s->next = NULL;
}

// pass in argv at the start of the pmon spec with argc downadjusted
int lex_pmon(pmon* p, uint16_t *hp, int *argc, char ***argv){
  if(*argc < 4){
    std::cerr << "expected 4 arguments, " << *argc << " left" << std::endl;
    return -1;
  }
  const char *spstr = **argv;
  while(isspace(*spstr)){
    ++spstr;
  }
  bool shadow = false;
#define SHADOWSTR "shadow "
  if(!strncasecmp(spstr, SHADOWSTR, strlen(SHADOWSTR))){
    shadow = true;
    spstr += strlen(SHADOWSTR);
  }
  if((p->s.s = lookup_species(spstr)) == NULL){
    std::cerr << "no such species: " << spstr << std::endl;
    return -1;
  }
  if(shadow){
    if(!p->s.s->shadow){ // still allow it, but warn
      std::cerr << "warning: " << spstr << " does not have a shadow form" << std::endl;
    }
  }
  if(lex_ivlevel((*argv)[1], &p->s, shadow)){
    std::cerr << "invalid IV@level in " << (*argv)[1] << std::endl;
    return -1;
  }
  p->fa = species_fast_attack(p->s.s, (*argv)[2]);
  p->ca1 = lex_species_charged_attacks(p->s.s, (*argv)[3], &p->ca2);
  if(!p->fa || !p->ca1){
    fprintf(stderr, "invalid attacks for %s: '%s' '%s'\n", p->s.s->name.c_str(),
            (*argv)[2], (*argv)[3]);
    return -1;
  }
  fill_stats(&p->s, shadow);
  *hp = p->s.mhp;
  (*argv) += 4;
  *argc -= 4;
  return 0;
}

struct typeset {
  pgo_types_e t0;
  pgo_types_e t1; // can be the same as t1 if we only have one attack type
  int totals[6];  // we range from -3 to 2, inclusive
  unsigned pop;   // population that can learn a charged attack of these types
  float ara;

  typeset(pgo_types_e T0, pgo_types_e T1, const int Totals[],
          unsigned Pop, float ARA) :
    t0(T0),
    t1(T1),
    pop(Pop),
    ara(ARA) {
      memcpy(totals, Totals, sizeof(totals));
  }

  friend bool operator<(const typeset &l, const typeset &r) {
    return l.ara < r.ara ? true :
            r.ara < l.ara ? false :
            l.pop < r.pop ? true :
            r.pop < l.pop ? false :
            l.t0 < r.t0 ? true :
            r.t0 < l.t0 ? false :
            l.t1 < r.t1 ? true : false;
  }

  friend bool operator>(const typeset &l, const typeset &r) {
    return !(l < r);
  }

};

static unsigned
dualcharge_pop(pgo_types_e t0, pgo_types_e t1){
  unsigned pop = 0;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    bool b0 = false;
    bool b1 = false;
    for(const auto &a : sdex[u].attacks){
      if(a->energytrain >= 0){
        continue;
      }
      if(a->type == t0){
        b0 = true;
      }
      if(a->type == t1){
        b1 = true;
      }
    }
    if(b0 && (b1 || t1 == TYPECOUNT)){
      ++pop;
    }
  }
  return pop;
}

static inline void
build_tset(std::vector<typeset> &tsets, pgo_types_e t0, pgo_types_e t1){
  int totals[6] = {};
  for(int tt0 = 0 ; tt0 < TYPECOUNT ; ++tt0){
    for(int tt1 = tt0 ; tt1 < TYPECOUNT ; ++tt1){
      int e0 = typing_relation(t0, static_cast<pgo_types_e>(tt0), static_cast<pgo_types_e>(tt1));
      int e1 = typing_relation(t1, static_cast<pgo_types_e>(tt0), static_cast<pgo_types_e>(tt1));
      int e = e0 > e1 ? e0 : e1;
      ++totals[e + 3];
    }
  }
  float ara = 0;
  for(unsigned i = 0 ; i < sizeof(totals) / sizeof(*totals) ; ++i){
    ara += type_effectiveness_mult(static_cast<int>(i) - 3) * totals[i];
  }
  ara /= TYPINGCOUNT;
  unsigned pop = dualcharge_pop(t0, t1);
  tsets.emplace(tsets.end(), t0, t1, totals, pop, ara);
}

#endif
