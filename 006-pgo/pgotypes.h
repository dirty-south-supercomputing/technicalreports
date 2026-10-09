#ifndef PGOTYPES_H
#define PGOTYPES_H

#include <map>
#include <cmath>
#include <print>
#include <vector>
#include <string>
#include <cstdio>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <forward_list>

// for greyscale images, define IMAGECOLOR as "g-". for full color, define it
// as an empty string.
//#define IMAGECOLOR "g-"
#define IMAGECOLOR ""

#define TYPESTART TYPE_BUG

// power for gmax and dmax level 1. levels 2 and 3 add 50 and 100, respectively.
const unsigned GMAX_POWER_BASE = 350;
const unsigned DMAX_POWER_BASE = 250;

constexpr unsigned TEAMSIZE = 3;
constexpr int MAXIVELEM = 15;
constexpr unsigned MINCP = 10; // minimum combat power
constexpr int MAXCHARGEDBUFF = 4;
constexpr unsigned ENERGY_MAX = 100;
// one can reach the 99th halflevel (50) through powering up.
// past that, one can get two temporary levels through active
// best buddy status, and two temporary levels by mega level 4.
constexpr unsigned MAX_HALFLEVEL_BASIC = 99;
constexpr unsigned MAX_HALFLEVEL = MAX_HALFLEVEL_BASIC + 4;
static constexpr auto GLCPCAP = 1500;
static constexpr auto ULCPCAP = 2500;
static constexpr auto MAXLEVEL = 80;

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

inline pgo_types_e& operator++(pgo_types_e& pt){ // ugh
  pt = static_cast<pgo_types_e>(static_cast<int>(pt) + 1);
  return pt;
}

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
  if(atype >= TYPECOUNT){
    throw std::invalid_argument("bad atype");
  }
  if(ttype >= TYPECOUNT){
    throw std::invalid_argument("bad ttype");
  }
  return trelations[atype][ttype];
}

// ranges from -3 to 2, inclusive
static inline int
typing_relation(pgo_types_e atype, pgo_types_e ttype1, pgo_types_e ttype2){
  int r1 = type_relation(atype, ttype1);
  if(ttype2 == ttype1 || ttype2 == TYPECOUNT){
    return r1;
  }
  int r2 = type_relation(atype, ttype2);
  return r1 + r2;
}

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

static inline float mapbuff(int bufflevel){
  static const float buffmap[9] = { 4/8, 4/7, 4/6, 4/5, 1, 5/4, 6/4, 7/4, 8/4 };
  return buffmap[bufflevel + 4];
}

struct attack {
  const char *name;
  pgo_types_e type;
  // 3x3 context
  unsigned powertrain;   // power in 3x3 battle context
  int energytrain;       // energy generated/consumed in trainer battle context
  unsigned turns;        // 0 for charged moves
  // chance (out of 1000) of having any of four buffing/debuffing effects
  unsigned chance_user_attack;
  unsigned chance_user_defense;
  unsigned chance_opp_attack;
  unsigned chance_opp_defense;
  // four possible effects (out of [-4, 4])
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
static const attack ATK_Bite = { "Bite", TYPE_DARK, 2, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 4, 1, false, };
static const attack ATK_Bubble = { "Bubble", TYPE_WATER, 8, 11, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 12, 2, false, };
static const attack ATK_Bug_Bite = { "Bug Bite", TYPE_BUG, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 6, 1, false, };
static const attack ATK_Bullet_Punch = { "Bullet Punch", TYPE_STEEL, 7, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 11, 2, false, };
static const attack ATK_Bullet_Seed = { "Bullet Seed", TYPE_GRASS, 5, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	7, 13, 2, false, };
static const attack ATK_Charge_Beam = { "Charge Beam", TYPE_ELECTRIC, 6, 11, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	7, 14, 2, false, };
static const attack ATK_Charm = { "Charm", TYPE_FAIRY, 12, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Geomancy = { "Geomancy", TYPE_FAIRY, 8, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	20, 14, 3, false, };
static const attack ATK_Gust = { "Gust", TYPE_FLYING, 16, 14, 4, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Infestation = { "Infestation", TYPE_BUG, 10, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Low_Kick = { "Low Kick", TYPE_FIGHTING, 6, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 1, false, };
static const attack ATK_Magical_Leaf = { "Magical Leaf", TYPE_GRASS, 10, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	17, 17, 3, false, };
static const attack ATK_Metal_Claw = { "Metal Claw", TYPE_STEEL, 5, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 5, 1, false, };
static const attack ATK_Metal_Sound = { "Metal Sound", TYPE_STEEL, 5, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Mud_Shot = { "Mud Shot", TYPE_GROUND, 3, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Mud_Slap = { "Mud-Slap", TYPE_GROUND, 11, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	19, 13, 3, false, };
static const attack ATK_Peck = { "Peck", TYPE_FLYING, 6, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 10, 2, false, };
static const attack ATK_Poison_Jab = { "Poison Jab", TYPE_POISON, 7, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 9, 2, false, };
static const attack ATK_Poison_Sting = { "Poison Sting", TYPE_POISON, 4, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Pound = { "Pound", TYPE_NORMAL, 4, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 5, 1, false, };
static const attack ATK_Powder_Snow = { "Powder Snow", TYPE_ICE, 6, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 15, 2, false, };
static const attack ATK_Present = { "Present", TYPE_NORMAL, 3, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 23, 3, false, };
static const attack ATK_Psycho_Cut = { "Psycho Cut", TYPE_PSYCHIC, 4, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Scratch = { "Scratch", TYPE_NORMAL, 3, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Steel_Wing = { "Steel Wing", TYPE_STEEL, 7, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	14, 8, 2, false, };
static const attack ATK_Struggle_Bug = { "Struggle Bug", TYPE_BUG, 9, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	15, 15, 3, false, };
static const attack ATK_Sucker_Punch = { "Sucker Punch", TYPE_DARK, 8, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 6, 1, false, };
static const attack ATK_Tackle = { "Tackle", TYPE_NORMAL, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 1, false, };
static const attack ATK_Take_Down = { "Take Down", TYPE_NORMAL, 14, 9, 3, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Waterfall = { "Waterfall", TYPE_WATER, 11, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 7, 2, false, };
static const attack ATK_Water_Shuriken = { "Water Shuriken", TYPE_WATER, 6, 14, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	9, 14, 2, false, };
static const attack ATK_Wing_Attack = { "Wing Attack", TYPE_FLYING, 5, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 11, 2, false, };
static const attack ATK_Yawn = { "Yawn", TYPE_NORMAL, 0, 12, 4, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 13, 3, false, };
static const attack ATK_Zen_Headbutt = { "Zen Headbutt", TYPE_PSYCHIC, 8, 6, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 9, 2, false, };
static const attack ATK_Acid_Spray = { "Acid Spray", TYPE_POISON, 20, -45, 0, 0, 0, 0, 1000, 0, 0, 0, -2,
	20, 50, 6, false, };
static const attack ATK_Acid_Spray_Plus = { "Acid Spray+", TYPE_POISON, 20, -40, 0, 0, 0, 0, 1000, 0, 0, 0, -2,
	160, 100, 6, false, };
static const attack ATK_Acrobatics = { "Acrobatics", TYPE_FLYING, 110, -55, 0, 125, 0, 0, 0, 2, 0, 0, 0,
	100, 100, 4, false, };
static const attack ATK_Aerial_Ace = { "Aerial Ace", TYPE_FLYING, 60, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 5, false, };
static const attack ATK_Aeroblast = { "Aeroblast", TYPE_FLYING, 170, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	200, 100, 7, false, };
static const attack ATK_Air_Cutter = { "Air Cutter", TYPE_FLYING, 60, -40, 0, 125, 0, 0, 0, 1, 0, 0, 0,
	55, 50, 5, false, };
static const attack ATK_Ancient_Power = { "Ancient Power", TYPE_ROCK, 60, -45, 0, 100, 100, 0, 0, 1, 1, 0, 0,
	70, 33, 7, false, };
static const attack ATK_Aqua_Jet = { "Aqua Jet", TYPE_WATER, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 5, false, };
static const attack ATK_Aqua_Step = { "Aqua Step", TYPE_WATER, 55, -40, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	55, 33, 7, false, };
static const attack ATK_Aqua_Tail = { "Aqua Tail", TYPE_WATER, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Aura_Sphere = { "Aura Sphere", TYPE_FIGHTING, 80, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 4, false, };
static const attack ATK_Aura_Wheel = { "Aura Wheel", TYPE_ELECTRIC, 100, -45, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	100, 50, 5, false, };
static const attack ATK_Aurora_Beam = { "Aurora Beam", TYPE_ICE, 80, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 7, false, };
static const attack ATK_Avalanche = { "Avalanche", TYPE_ICE, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 5, false, };
static const attack ATK_Beak_Blast = { "Beak Blast", TYPE_FLYING, 110, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 1, true, };
static const attack ATK_Behemoth_Bash = { "Behemoth Bash", TYPE_STEEL, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	125, 50, 3, true, };
static const attack ATK_Behemoth_Blade = { "Behemoth Blade", TYPE_STEEL, 100, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	200, 100, 7, true, };
static const attack ATK_Blast_Burn = { "Blast Burn", TYPE_FIRE, 110, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 50, 7, false, };
static const attack ATK_Blaze_Kick = { "Blaze Kick", TYPE_FIRE, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	40, 33, 2, false, };
static const attack ATK_Bleakwind_Storm = { "Bleakwind Storm", TYPE_FLYING, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	150, 100, 5, false, };
static const attack ATK_Blizzard = { "Blizzard", TYPE_ICE, 140, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	130, 100, 6, false, };
static const attack ATK_Body_Slam = { "Body Slam", TYPE_NORMAL, 65, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Boomburst = { "Boomburst", TYPE_NORMAL, 150, -70, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	150, 100, 5, false, };
static const attack ATK_Bone_Club = { "Bone Club", TYPE_GROUND, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	40, 33, 3, false, };
static const attack ATK_Brave_Bird = { "Brave Bird", TYPE_FLYING, 130, -55, 0, 0, 1000, 0, 0, 0, -3, 0, 0,
	130, 100, 4, false, };
static const attack ATK_Brave_Bird_Plus = { "Brave Bird+", TYPE_FLYING, 70, -40, 0, 0, 1000, 0, 0, 0, -3, 0, 0,
	150, 100, 4, false, };
static const attack ATK_Breaking_Swipe = { "Breaking Swipe", TYPE_DRAGON, 50, -50, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	45, 33, 2, false, };
static const attack ATK_Brick_Break = { "Brick Break", TYPE_FIGHTING, 50, -40, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	40, 33, 3, false, };
static const attack ATK_Brick_Break_Plus = { "Brick Break+", TYPE_FIGHTING, 40, -35, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	150, 100, 3, false, };
static const attack ATK_Brutal_Swing = { "Brutal Swing", TYPE_DARK, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 33, 4, false, };
static const attack ATK_Brine = { "Brine", TYPE_WATER, 100, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 5, false, };
static const attack ATK_Bubble_Beam = { "Bubble Beam", TYPE_WATER, 50, -50, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	45, 33, 4, false, };
static const attack ATK_Bug_Buzz = { "Bug Buzz", TYPE_BUG, 100, -60, 0, 0, 0, 0, 300, 0, 0, 0, -1,
	95, 50, 7, false, };
static const attack ATK_Bulldoze = { "Bulldoze", TYPE_GROUND, 80, -55, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
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
static const attack ATK_Crush_Grip = { "Crush Grip", TYPE_NORMAL, 110, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	210, 100, 4, false, };
static const attack ATK_Darkest_Lariat = { "Darkest Lariat", TYPE_DARK, 120, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 4, false, };
static const attack ATK_Dark_Pulse = { "Dark Pulse", TYPE_DARK, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 6, false, };
static const attack ATK_Dazzling_Gleam = { "Dazzling Gleam", TYPE_FAIRY, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  100, 50, 7, false, };
static const attack ATK_Dig = { "Dig", TYPE_GROUND, 70, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 9, false, };
static const attack ATK_Disarming_Voice = { "Disarming Voice", TYPE_FAIRY, 70, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 8, false, };
static const attack ATK_Discharge = { "Discharge", TYPE_ELECTRIC, 55, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 33, 5, false, };
// FIXME get real stats
static const attack ATK_Discharge_Plus = { "Discharge+", TYPE_ELECTRIC, 55, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 33, 5, false, };
static const attack ATK_Dive = { "Dive", TYPE_WATER, 50, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 7, false, };
static const attack ATK_Doom_Desire = { "Doom Desire", TYPE_STEEL, 80, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 33, 3, false, };
static const attack ATK_Double_Iron_Bash = { "Double Iron Bash", TYPE_STEEL, 70, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 4, false, };
static const attack ATK_Draco_Meteor = { "Draco Meteor", TYPE_DRAGON, 150, -65, 0, 1000, 0, 0, 0, -2, 0, 0, 0,
	150, 100, 7, false, };
static const attack ATK_Dragon_Ascent = { "Dragon Ascent", TYPE_FLYING, 110, -45, 0, 0, 1000, 0, 0, 0, -1, 0, 0,
	140, 50, 7, false, };
static const attack ATK_Dragon_Claw = { "Dragon Claw", TYPE_DRAGON, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 3, false, };
static const attack ATK_Dragon_Energy = { "Dragon Energy", TYPE_DRAGON, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	155, 50, 7, false, };
static const attack ATK_Dragon_Pulse = { "Dragon Pulse", TYPE_DRAGON, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 7, false, };
static const attack ATK_Drain_Punch = { "Drain Punch", TYPE_FIGHTING, 40, -40, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	50, 33, 5, false, };
static const attack ATK_Draining_Kiss = { "Draining Kiss", TYPE_FAIRY, 80, -55, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	60, 50, 5, false, };
static const attack ATK_Drill_Peck = { "Drill Peck", TYPE_FLYING, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 5, false, };
static const attack ATK_Drill_Peck_Plus = { "Drill Peck+", TYPE_FLYING, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	170, 100, 5, false, };
static const attack ATK_Drill_Run = { "Drill Run", TYPE_GROUND, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 6, false, };
static const attack ATK_Drum_Beating = { "Drum Beating", TYPE_GRASS, 60, -35, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	115, 33, 8, false, };
static const attack ATK_Dynamax_Cannon = { "Dynamax Cannon", TYPE_DRAGON, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	215, 100, 3, true, };
static const attack ATK_Dynamic_Punch = { "Dynamic Punch", TYPE_FIGHTING, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 5, false, };
static const attack ATK_Dynamic_Punch_Plus = { "Dynamic Punch+", TYPE_FIGHTING, 130, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	130, 100, 5, false, };
static const attack ATK_Earthquake = { "Earthquake", TYPE_GROUND, 120, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 7, false, };
static const attack ATK_Earth_Power = { "Earth Power", TYPE_GROUND, 90, -50, 0, 0, 0, 0, 100, 0, 0, 0, -1,
	100, 50, 7, false, };
static const attack ATK_Energy_Ball = { "Energy Ball", TYPE_GRASS, 80, -45, 0, 0, 0, 0, 100, 0, 0, 0, -1,
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
static const attack ATK_Flash_Cannon = { "Flash Cannon", TYPE_STEEL, 110, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 100, 5, false, };
static const attack ATK_Flower_Trick = { "Flower Trick", TYPE_GRASS, 30, -35, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	75, 33, 5, false, };
static const attack ATK_Fly = { "Fly", TYPE_FLYING, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 4, false, };
static const attack ATK_Flying_Press = { "Flying Press", TYPE_FIGHTING, 90, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	115, 50, 5, false, };
static const attack ATK_Focus_Blast = { "Focus Blast", TYPE_FIGHTING, 150, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 7, false, };
static const attack ATK_Foul_Play = { "Foul Play", TYPE_DARK, 65, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Future_Sight_Plus = { "Future Sight+", TYPE_PSYCHIC, 130, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 5, false, };
static const attack ATK_Giga_Impact = { "Giga Impact", TYPE_NORMAL, 150, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	200, 100, 9, false, };
static const attack ATK_Gigaton_Hammer = { "Gigaton Hammer", TYPE_STEEL, 130, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	300, 100, 6, false, };
static const attack ATK_Glaciate = { "Glaciate", TYPE_ICE, 60, -40, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	160, 100, 5, false, };
static const attack ATK_Grass_Knot = { "Grass Knot", TYPE_GRASS, 90, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 5, false, };
static const attack ATK_Gunk_Shot = { "Gunk Shot", TYPE_POISON, 130, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	130, 100, 6, false, };
static const attack ATK_Gyro_Ball = { "Gyro Ball", TYPE_STEEL, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 7, false, };
static const attack ATK_Heat_Wave = { "Heat Wave", TYPE_FIRE, 75, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	95, 100, 6, false, };
static const attack ATK_Heavy_Slam = { "Heavy Slam", TYPE_STEEL, 70, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 50, 4, false, };
static const attack ATK_High_Horsepower = { "High Horsepower", TYPE_GROUND, 100, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	105, 100, 3, false, };
static const attack ATK_High_Jump_Kick = { "High Jump Kick", TYPE_FIGHTING, 110, -55, 0, 0, 100, 0, 0, 0, -4, 0, 0,
	90, 100, 3, false, };
static const attack ATK_Horn_Attack = { "Horn Attack", TYPE_NORMAL, 40, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 4, false, };
static const attack ATK_Hurricane = { "Hurricane", TYPE_FLYING, 110, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Icicle_Spear = { "Icicle Spear", TYPE_ICE, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 4, false, };
static const attack ATK_Icy_Wind = { "Icy Wind", TYPE_ICE, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	65, 33, 7, false, };
static const attack ATK_Iron_Head = { "Iron Head", TYPE_STEEL, 85, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Liquidation_Plus = { "Liquidation+", TYPE_WATER, 55, -40, 0, 0, 0, 0, 300, 0, 0, 0, -1,
	180, 100, 6, false, };
static const attack ATK_Low_Sweep = { "Low Sweep", TYPE_FIGHTING, 40, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	40, 33, 4, false, };
static const attack ATK_Lunge = { "Lunge", TYPE_BUG, 70, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	55, 33, 6, false, };
static const attack ATK_Luster_Purge = { "Luster Purge", TYPE_PSYCHIC, 120, -60, 0, 0, 0, 0, 500, 0, 0, 0, -1,
	100, 100, 3, false, };
static const attack ATK_Magma_Storm = { "Magma Storm", TYPE_FIRE, 65, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	75, 33, 5, false, };
static const attack ATK_Magnet_Bomb = { "Magnet Bomb", TYPE_STEEL, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	75, 33, 6, false, };
static const attack ATK_Megahorn = { "Megahorn", TYPE_BUG, 110, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	105, 100, 4, false, };
static const attack ATK_Meteor_Beam = { "Meteor Beam", TYPE_ROCK, 120, -60, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	140, 100, 4, false, };
static const attack ATK_Meteor_Mash = { "Meteor Mash", TYPE_STEEL, 100, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 5, false, };
static const attack ATK_Mind_Blown = { "Mind Blown", TYPE_FIRE, 90, -35, 0, 0, 1000, 0, 0, 0, -4, 0, 0,
  130, 33, 8, false, };
static const attack ATK_Mirror_Coat = { "Mirror Coat", TYPE_PSYCHIC, 75, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 5, false, };
static const attack ATK_Mirror_Shot = { "Mirror Shot", TYPE_STEEL, 35, -35, 0, 0, 0, 300, 0, 0, 0, -1, 0,
	50, 33, 5, false, };
static const attack ATK_Mist_Ball = { "Mist Ball", TYPE_PSYCHIC, 120, -60, 0, 0, 0, 500, 0, 0, 0, -1, 0,
	105, 100, 4, false, };
static const attack ATK_Moonblast = { "Moonblast", TYPE_FAIRY, 90, -50, 0, 0, 0, 100, 0, 0, 0, -1, 0,
	130, 100, 8, false, };
static const attack ATK_Moongeist_Beam = { "Moongeist Beam", TYPE_GHOST, 135, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	230, 100, 6, true, };
static const attack ATK_Mud_Bomb = { "Mud Bomb", TYPE_GROUND, 65, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 5, false, };
static const attack ATK_Muddy_Water = { "Muddy Water", TYPE_WATER, 35, -35, 0, 0, 0, 300, 0, 0, 0, -1, 0,
	45, 33, 4, false, };
static const attack ATK_Mystical_Fire = { "Mystical Fire", TYPE_FIRE, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	60, 33, 4, false, };
static const attack ATK_Mystical_Fire_Plus = { "Mystical Fire+", TYPE_FIRE, 50, -40, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	140, 100, 4, false, };
static const attack ATK_Natures_Madness = { "Nature's Madness", TYPE_FAIRY, 80, -50, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	90, 50, 4, false, };
static const attack ATK_Night_Shade = { "Night Shade", TYPE_GHOST, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 5, false, };
// FIXME get real stats
static const attack ATK_Night_Shade_Plus = { "Night Shade+", TYPE_GHOST, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 5, false, };
static const attack ATK_Night_Slash = { "Night Slash", TYPE_DARK, 50, -35, 0, 125, 0, 0, 0, 1, 0, 0, 0,
	45, 33, 4, false, };
static const attack ATK_Oblivion_Wing = { "Oblivion Wing", TYPE_FLYING, 85, -50, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
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
static const attack ATK_Outrage_Plus = { "Outrage+", TYPE_DRAGON, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	185, 100, 8, false, };
static const attack ATK_Overheat = { "Overheat", TYPE_FIRE, 130, -55, 0, 1000, 0, 0, 0, -2, 0, 0, 0,
	160, 100, 8, false, };
static const attack ATK_Parabolic_Charge = { "Parabolic Charge", TYPE_ELECTRIC, 70, -50, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	70, 50, 6, false, };
static const attack ATK_Payback = { "Payback", TYPE_DARK, 110, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	95, 100, 4, false, };
static const attack ATK_Petal_Blizzard = { "Petal Blizzard", TYPE_GRASS, 110, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	110, 100, 5, false, };
static const attack ATK_Plasma_Fists = { "Plasma Fists", TYPE_ELECTRIC, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	135, 50, 7, false, };
static const attack ATK_Play_Rough = { "Play Rough", TYPE_FAIRY, 90, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 6, false, };
static const attack ATK_Poison_Fang = { "Poison Fang", TYPE_POISON, 50, -40, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
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
static const attack ATK_Psybeam_Plus = { "Psybeam+", TYPE_PSYCHIC, 60, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	170, 100, 6, false, };
static const attack ATK_Psychic = { "Psychic", TYPE_PSYCHIC, 75, -55, 0, 0, 0, 0, 100, 0, 0, 0, -1,
	95, 50, 7, false, };
static const attack ATK_Psychic_Fangs = { "Psychic Fangs", TYPE_PSYCHIC, 40, -35, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	25, 33, 2, false, };
static const attack ATK_Psycho_Boost = { "Psycho Boost", TYPE_PSYCHIC, 85, -35, 0, 1000, 0, 0, 0, -2, 0, 0, 0,
	130, 33, 8, false, };
static const attack ATK_Psyshock = { "Psyshock", TYPE_PSYCHIC, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 5, false, };
static const attack ATK_Psystrike = { "Psystrike", TYPE_PSYCHIC, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	95, 50, 5, false, };
static const attack ATK_Pyro_Ball = { "Pyro Ball", TYPE_FIRE, 75, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	150, 100, 4, false, };
static const attack ATK_Rage_Fist = { "Rage Fist", TYPE_GHOST, 55, -40, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
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
static const attack ATK_Rock_Tomb = { "Rock Tomb", TYPE_ROCK, 75, -50, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	65, 50, 6, false, };
static const attack ATK_Rock_Wrecker = { "Rock Wrecker", TYPE_ROCK, 110, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	110, 50, 7, false, };
static const attack ATK_Sacred_Fire = { "Sacred Fire", TYPE_FIRE, 130, -65, 0, 0, 0, 500, 0, 0, 0, -1, 0,
	120, 100, 5, false, };
static const attack ATK_Sacred_Sword = { "Sacred Sword", TYPE_FIGHTING, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 2, false, };
static const attack ATK_Sand_Tomb = { "Sand Tomb", TYPE_GROUND, 55, -45, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	60, 33, 8, false, };
static const attack ATK_Sandsear_Storm = { "Sandsear Storm", TYPE_GROUND, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	150, 100, 5, false, };
// https://db.pokemongohub.net/move/134 -- no listed users, consider an error
// https://db.pokemongohub.net/move/106
static const attack ATK_Scald = { "Scald", TYPE_WATER, 85, -50, 0, 0, 0, 300, 0, 0, 0, -1, 0,
	75, 50, 7, false, };
static const attack ATK_Scorching_Sands = { "Scorching Sands", TYPE_GROUND, 80, -50, 0, 0, 0, 100, 0, 0, 0, -1, 0,
	90, 50, 6, false, };
static const attack ATK_Secret_Sword = { "Secret Sword", TYPE_FIGHTING, 70, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 50, 4, false, };
static const attack ATK_Seed_Bomb = { "Seed Bomb", TYPE_GRASS, 55, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 4, false, };
static const attack ATK_Seed_Bomb_Plus = { "Seed Bomb+", TYPE_GRASS, 60, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	150, 100, 4, false, };
static const attack ATK_Seed_Flare = { "Seed Flare", TYPE_GRASS, 130, -75, 0, 0, 0, 0, 400, 0, 0, 0, -2,
	115, 100, 5, false, };
static const attack ATK_Shadow_Ball = { "Shadow Ball", TYPE_GHOST, 90, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 6, false, };
static const attack ATK_Shadow_Bone = { "Shadow Bone", TYPE_GHOST, 80, -45, 0, 0, 0, 0, 200, 0, 0, 0, -1,
	85, 50, 6, false, };
static const attack ATK_Shadow_Force = { "Shadow Force", TYPE_GHOST, 120, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, -4, false, };
static const attack ATK_Shadow_Punch = { "Shadow Punch", TYPE_GHOST, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	35, 33, 3, false, };
static const attack ATK_Shadow_Sneak = { "Shadow Sneak", TYPE_GHOST, 75, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 6, false, };
static const attack ATK_Signal_Beam = { "Signal Beam", TYPE_BUG, 75, -55, 0, 0, 0, 200, 200, 0, 0, -1, -1,
	75, 50, 6, false, };
static const attack ATK_Silver_Wind = { "Silver Wind", TYPE_BUG, 75, -45, 0, 100, 100, 0, 0, 1, 1, 0, 0,
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
static const attack ATK_Snipe_Shot = { "Snipe Shot", TYPE_WATER, 65, -35, 0, 125, 0, 0, 0, 2, 0, 0, 0,
	100, 33, 7, false, };
static const attack ATK_Solar_Beam = { "Solar Beam", TYPE_GRASS, 150, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	180, 100, 10, false, };
static const attack ATK_Spacial_Rend = { "Spacial Rend", TYPE_DRAGON, 95, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	160, 100, 5, true, };
static const attack ATK_Sparkling_Aria = { "Sparkling Aria", TYPE_WATER, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 33, 6, false, };
static const attack ATK_Spirit_Shackle = { "Spirit Shackle", TYPE_GHOST, 50, -40, 0, 0, 0, 0, 330, 0, 0, 0, -1,
	70, 33, 5, false, };
static const attack ATK_Stomp = { "Stomp", TYPE_NORMAL, 55, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 50, 3, false, };
static const attack ATK_Stone_Edge = { "Stone Edge", TYPE_ROCK, 100, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	105, 100, 5, false, };
// FIXME pokemon go hub claims 0 energy. pokebase claims 1.06 DPE which works out to 33.
static const attack ATK_Struggle = { "Struggle", TYPE_NORMAL, 35, -100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	35, 33, 4, false, };
static const attack ATK_Submission = { "Submission", TYPE_FIGHTING, 60, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 50, 4, false, };
static const attack ATK_Sunsteel_Strike = { "Sunsteel Strike", TYPE_STEEL, 135, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	230, 100, 6, true, };
static const attack ATK_Superpower = { "Superpower", TYPE_FIGHTING, 85, -40, 0, 1000, 1000, 0, 0, -1, -1, 0, 0,
	85, 50, 6, false, };
static const attack ATK_Surf = { "Surf", TYPE_WATER, 75, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 3, false, };
static const attack ATK_Surf_Plus = { "Surf+", TYPE_WATER, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	130, 100, 3, false, };
static const attack ATK_Swift = { "Swift", TYPE_NORMAL, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 6, false, };
static const attack ATK_Synchronoise = { "Synchronoise", TYPE_PSYCHIC, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 5, false, };
static const attack ATK_Techno_Blast_Electric = { "Techno Blast ⚡", TYPE_ELECTRIC, 120, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 4, false, };
static const attack ATK_Techno_Blast_Fire = { "Techno Blast 🔥", TYPE_FIRE, 120, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 4, false, };
static const attack ATK_Techno_Blast_Ice = { "Techno Blast 🧊", TYPE_ICE, 120, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 4, false, };
static const attack ATK_Techno_Blast_Normal = { "Techno Blast", TYPE_NORMAL, 120, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 4, false, };
static const attack ATK_Techno_Blast_Water = { "Techno Blast 🌊", TYPE_WATER, 120, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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
static const attack ATK_Twister = { "Twister", TYPE_DRAGON, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 6, false, };
static const attack ATK_Upper_Hand = { "Upper Hand", TYPE_FIGHTING, 70, -40, 0, 0, 0, 0, 300, 0, 0, 0, -1,
	50, 33, 4, false, };
static const attack ATK_V_Create = { "V-Create", TYPE_FIRE, 95, -40, 0, 0, 1000, 0, 0, 0, -3, 0, 0,
	105, 33, 6, false, };
static const attack ATK_Vise_Grip = { "Vise Grip", TYPE_NORMAL, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	35, 33, 4, false, };
static const attack ATK_Volt_Tackle = { "Volt Tackle", TYPE_ELECTRIC, 90, -40, 0, 0, 1000, 0, 0, 0, -1, 0, 0,
	90, 33, 7, false, };
static const attack ATK_Volt_Tackle_Plus = { "Volt Tackle+", TYPE_ELECTRIC, 65, -35, 0, 0, 1000, 0, 0, 0, -1, 0, 0,
	170, 100, 7, false, };
static const attack ATK_Water_Pulse = { "Water Pulse", TYPE_WATER, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 6, false, };
static const attack ATK_Weather_Ball_Fire = { "Weather Ball 🔥", TYPE_FIRE, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 3, false, };
static const attack ATK_Weather_Ball_Ice = { "Weather Ball 🧊", TYPE_ICE, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 3, false, };
static const attack ATK_Weather_Ball_Rock = { "Weather Ball 🪨", TYPE_ROCK, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 4, false, };
static const attack ATK_Weather_Ball_Normal = { "Weather Ball", TYPE_NORMAL, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 3, false, };
static const attack ATK_Weather_Ball_Water = { "Weather Ball 🌊", TYPE_WATER, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 3, false, };
static const attack ATK_Wild_Charge = { "Wild Charge", TYPE_ELECTRIC, 100, -45, 0, 0, 1000, 0, 0, 0, -2, 0, 0,
	90, 50, 5, false, };
static const attack ATK_Wildbolt_Storm = { "Wildbolt Storm", TYPE_ELECTRIC, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	150, 100, 5, false, };
static const attack ATK_Wrap = { "Wrap", TYPE_NORMAL, 70, -45, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	25, 33, 6, false, };
static const attack ATK_X_Scissor = { "X-Scissor", TYPE_BUG, 65, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 3, false, };
static const attack ATK_Zap_Cannon = { "Zap Cannon", TYPE_ELECTRIC, 150, -80, 0, 0, 0, 330, 0, 0, 0, -1, 0,
	140, 100, 7, false, };
static const attack ATK_Zap_Cannon_Plus = { "Zap Cannon+", TYPE_ELECTRIC, 70, -45, 0, 0, 0, 330, 0, 0, 0, -1, 0,
	160, 100, 7, false, };


// return the dmax attack type corresponding to this fast attack. it is simply
// the attack's type, except for Hidden Power, which always maps to Normal.
static inline pgo_types_e
dmax_attack_type(const attack* a){
  if(a == &ATK_Hidden_Power){
    return TYPE_NORMAL;
  }
  return a->type;
}

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

// all gmax attacks are 350, 400, 450, 550 damage (level 4 is achieved via the
// dynamax cannon adventure effect).
struct gmaxattack {
  const char *sname;      // species name
  const std::string name; // attack name
  pgo_types_e type;       // attack type
  bool shiny;             // shiny gmax available?
};

// lives outside lookup_gmax_attack() so it can be unit tested.
static gmaxattack GMaxAttacks[] = {
  { "Venusaur", "G-Max Vine Lash", TYPE_GRASS, true, },
  { "Charizard", "G-Max Wildfire", TYPE_FIRE, true, },
  { "Blastoise", "G-Max Cannonade", TYPE_WATER, true, },
  { "Butterfree", "G-Max Befuddle", TYPE_BUG, true, },
  { "Pikachu", "G-Max Volt Crash", TYPE_ELECTRIC, true, },
  { "Meowth", "G-Max Gold Rush", TYPE_NORMAL, true, },
  { "Machamp", "G-Max Chi Strike", TYPE_FIGHTING, true, },
  { "Gengar", "G-Max Terror", TYPE_GHOST, true, },
  { "Kingler", "G-Max Foam Burst", TYPE_WATER, true, },
  { "Lapras", "G-Max Resonance", TYPE_ICE, true, },
  { "Snorlax", "G-Max Replenish", TYPE_NORMAL, true, },
  { "Garbodor", "G-Max Malodor", TYPE_POISON, true, },
  { "Rillaboom", "G-Max Drum Solo", TYPE_GRASS, true, },
  { "Cinderace", "G-Max Fireball", TYPE_FIRE, true, },
  { "Inteleon", "G-Max Hydrosnipe", TYPE_WATER, false, },
  { "Toxtricity", "G-Max Stun Shock", TYPE_ELECTRIC, true, },
  { "Duraludon", "G-Max Depletion", TYPE_DRAGON, false, }, // FIXME speculated
  { "Grimmsnarl", "G-Max Snooze", TYPE_DARK, true, },
  //{ "Sandaconda", "G-Max Sandblast", TYPE_GROUND, false, },
  //{ "Appletun", "G-Max Sweetness", TYPE_GRASS, false, },
  //{ "Flapple", "G-Max Tartness", TYPE_GRASS, false, },
  //{ "Coalossal", "G-Max Volcality", TYPE_ROCK, false, },
  //{ "Drednaw", "G-Max Stonesurge", TYPE_WATER, false, },
  //{ "Orbeetle", "G-Max Gravitas", TYPE_PSYCHIC, false, },
  //{ "Corviknight", "G-Max Wind Rage", TYPE_FLYING, false, },
  //{ "Melmetal", "G-Max Meltdown", TYPE_STEEL, false, },
  //{ "Centiskorch", "G-Max Centiferno", TYPE_FIRE, false, },
  //{ "Hatterene", "G-Max Smite", TYPE_FAIRY, false, },
  //{ "Alcremie", "G-Max Finale", TYPE_FAIRY, false, },
  //{ "Copperajah", "G-Max Steelsurge", TYPE_STEEL, false, },
  //{ "Urshifu Single Strike", "G-Max One Blow", TYPE_DARK, false, },
  //{ "Urshifu Rapid Strike", "G-Max Rapid Flow", TYPE_WATER, false, },
  { nullptr, "", TYPECOUNT, false, },
};

struct mega {
  std::string name;       // can't just add "Mega " prefix; there are X and Y etc
  pgo_types_e t1, t2;     // typing and stats can be different than base form
  unsigned atk;
  unsigned def;
  unsigned sta;
  unsigned initialcost;   // mega energy needed to go to mega level 1
  const attack* plusatk;  // megas which can reach Super Mega level (level 4)
                          // have an additional "Foo+" charged attack

  mega() {
  }

  mega(const std::string& s)
      : name(s) {
  }

  mega(const char *n, pgo_types_e T1, pgo_types_e T2,
       unsigned A, unsigned D, unsigned S,
       unsigned Initialcost, const attack* Plusatk)
    : name(n),
    t1(T1),
    t2(T2),
    atk(A),
    def(D),
    sta(S),
    initialcost(Initialcost),
    plusatk(Plusatk)
  { }
};

float cpm(int halflevel);

// atk, def, and sta all ought be mod forms (i.e. sum of base and IV)
static int
calccp(unsigned atk, unsigned def, unsigned sta, unsigned halflevel){
  float cand = (atk * sqrt(def) * sqrt(sta) * pow(cpm(halflevel), 2)) / 10;
  return cand < 10 ? 10 : floor(cand);
}

struct species {
  unsigned idx; // pokedex index, not unique
  std::string name;
  pgo_types_e t1, t2;
  unsigned atk;
  unsigned def;
  unsigned sta;
  std::string from;   // evolves from what? empty string for nothing
  std::vector<const attack*> attacks; // attacks we can learn
  bool shiny;         // is there a shiny form?
  bool shadow;        // is there a shadow form?
  unsigned dmax;      // is there a dynamax form? if so, non-zero battle tier.
                      // if the species does not show up in max battles, set to UINT_MAX.
                      // 5 is legendaries, 6 is gigantamax (if gmax *only*) and eternatus.
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
  enum evol_item {    // item required to evolve *into* this mon
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
    EVOL_MAGLURE,
    EVOL_RAINLURE,
  } evolitem;
  enum mon_region {    // region-specific pokémon
    REGION_ALL,
    REGION_EASTASIA,
    REGION_AUSTRALIA,
    REGION_NORTHAM,
    REGION_SOUTHEASTHEMI,
    REGION_SOUTHHEMI,
    REGION_NORTHHEMI,
    REGION_EASTHEMI,
    REGION_WESTHEMI,
    REGION_TROPICS,
    REGION_EAA,
    REGION_AA,
    REGION_SOUTHASIA,
    REGION_TROPIUS,
    REGION_UTC13,
    REGION_NORTHARCTIC,
    REGION_EAPAC,
    REGION_EMEAINDIA,
    REGION_AMERICAS,
    REGION_AMLAND,
    REGION_ISLANDS,
    REGION_EGYPTGREECE,
    REGION_IBERIAN,
    REGION_FRANCE,
    REGION_MEXICO,
    REGION_UK,
    REGION_SEUSA,
    REGION_NY,
    REGION_HAWAII,
  } monregion;
  unsigned evolkm;
  std::vector<mega> mforms;

  species() {
  }

  species(const std::string& s)
      : name(s) {
  }

  // synthesize into sbacking a new species made of the mega and us
  void synth_mega_species(const species *s, const mega &m) {
    idx = s->idx;
    name = m.name;
    t1 = m.t1;
    t2 = m.t2;
    atk = m.atk;
    atk = m.atk;
    def = m.def;
    sta = m.sta;
    from = s->name;
    attacks = s->attacks;
    if(m.plusatk){
      attacks.emplace_back(m.plusatk);
    }
    shiny = s->shiny;
    shadow = false;
    dmax = false;
    elite = s->elite;
    category = s->category;
    a2cost = s->a2cost;
    evolitem = s->evolitem;
    monregion = s->monregion;
    evolkm = s->evolkm;
  }

  species(unsigned i, const std::string &n, pgo_types_e T1, pgo_types_e T2,
          unsigned A, unsigned D, unsigned S, const std::string &From,
          const std::vector<const attack*> &Attacks,
          bool Shiny, bool Shadow, unsigned Dmax,
          const std::vector<const attack*>& Elite,
          species_cat Category, int A2Cost,
          evol_item Evolitem,
          mon_region MonRegion,
          unsigned Evolkm,
          const std::vector<mega>& Mforms)
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
    evolitem(Evolitem),
    monregion(MonRegion),
    evolkm(Evolkm),
    mforms(Mforms)
  { }

  species(const species *s, const mega &m)
    : species(s->idx, m.name, m.t1, m.t2,
              m.atk, m.def, m.sta, s->name,
              s->attacks,
              s->shiny, false, 0,
              s->elite, s->category, s->a2cost,
              s->evolitem, s->monregion, s->evolkm, {}) {
    if(m.plusatk){
      attacks.emplace_back(m.plusatk);
    }
    if(s->shiny){
      attacks.emplace_back(&ATK_Return);
    }
  }

  species(const species *s, const gmaxattack &gm)
    : species(s->idx, "G-Max " + s->name, s->t1, s->t2,
              s->atk, s->def, s->sta, "",
              s->attacks,
              gm.shiny, false, 0,
              s->elite, s->category, s->a2cost,
              s->evolitem, s->monregion, s->evolkm, {}) {
  }

  // effectiveness of attack a on our typing
  float type_effectiveness(const attack *a) const {
    return ::type_effectiveness(a->type, t1, t2);
  }

  int maxcp() const {
    return calccp(atk + 15, def + 15, sta + 15, MAX_HALFLEVEL_BASIC);
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

  // return LaTeX-ready string corresponding to region in which this form
  // spawns, or nullptr if it spawns worldwide (or not at all)
  const char *regionstr() const {
    switch(monregion){
      case REGION_ALL: return nullptr;
      case REGION_EASTASIA: return "East Asia";
      case REGION_AUSTRALIA: return "Australia";
      case REGION_NORTHAM: return "Eastern Asia";
      case REGION_SOUTHEASTHEMI: return "Eastern hemisphere south of \\textasciitilde\\ang{26}N";
      case REGION_SOUTHHEMI: return "Southern hemisphere";
      case REGION_NORTHHEMI: return "Northern hemisphere";
      case REGION_EASTHEMI: return "Eastern hemisphere";
      case REGION_WESTHEMI: return "Western hemisphere";
      case REGION_TROPICS: return "Tropic of Cancer---Tropic of Capricorn";
      case REGION_EAA: return "Europe, Asia, Australia";
      case REGION_AA: return "Americas, Africa";
      case REGION_SOUTHASIA: return "South Asia";
      case REGION_TROPIUS: return "Africa, the Levant, Malta, southern Spain";
      case REGION_UTC13: return "UTC+13";
      case REGION_NORTHARCTIC: return "North Arctic";
      case REGION_EAPAC: return "East Asia, the Pacific";
      case REGION_EMEAINDIA: return "EMEA, India";
      case REGION_AMERICAS: return "Americas";
      case REGION_AMLAND: return "Americas excluding Caribbean islands";
      case REGION_ISLANDS: return "African, Asian, Pacific, Caribbean islands";
      case REGION_EGYPTGREECE: return "Egypt, Greece";
      case REGION_IBERIAN: return "Iberian peninsula";
      case REGION_FRANCE: return "France";
      case REGION_MEXICO: return "Mexico";
      case REGION_UK: return "United Kingdom";
      case REGION_SEUSA: return "Southeastern United States";
      case REGION_NY: return "New York";
      case REGION_HAWAII: return "Hawaii";
      default: throw std::exception();
    }
  }

};

std::vector<species>::const_iterator species_begin(void);
std::vector<species>::const_iterator species_end(void);

// each gmax attack is associated with a single species, which can use only
// that attack. unit tests verify that the species are valid.
static gmaxattack*
lookup_gmax_attack(const species &s){
  for(auto gatk = GMaxAttacks ; gatk->sname ; ++gatk){
    if(s.name == gatk->sname){
      return gatk;
    }
  }
  return nullptr;
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

struct stats {
  const species *s;
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

  stats() :
    s(nullptr),
    next(nullptr)
  {
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
statscmp_cp(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  int cp1 = calccp(st1->effa, st1->effd, st1->mhp, st1->hlevel);
  int cp2 = calccp(st2->effa, st2->effd, st2->mhp, st2->hlevel);
  return cp1 < cp2 ? -1 : cp1 > cp2 ? 1 : 0;
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

static inline unsigned
has_stab_raw_p(const species &s, pgo_types_e atype){
  if(atype == TYPECOUNT){ // handle Hidden Power as no-stab
    return false;
  }
  return atype == s.t1 || atype == s.t2;
}

static inline unsigned
has_stab_p(const species &s, const attack *a){
  return has_stab_raw_p(s, a->type);
}

// apply the 1.2x stab multiplier to a damage calculation
static inline float
calc_stab(float d){
  return d * 6 / 5;
}

// get the power of the attack considering STAB (if it applies)
static inline float
calc_eff_power(const species &s, const attack* a){
  unsigned stab = has_stab_p(s, a);
  float power = a->powertrain;
  if(stab){
    power = calc_stab(power);
  }
  return power;
}

static inline float
calc_eff_power_nx1(const species &s, const attack* a){
  unsigned stab = has_stab_p(s, a);
  float power = a->powerraid;
  if(stab){
    power = calc_stab(power);
  }
  return power;
}

// FIXME binary search on it
static unsigned
maxlevel_cp_bounded(unsigned atk, unsigned def, unsigned sta, int cpceil, int *cp){
  unsigned lastgood = 0;
  *cp = 0;
  for(unsigned hl = 1 ; hl <= MAX_HALFLEVEL_BASIC ; ++hl){
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
calc_pok_cp(const stats *s){
  return calccp(s->effa, s->effd, s->mhp, s->hlevel);
}

static inline float
calc_pok_effa(const stats *s){
  return s->effa;
}

static inline float
calc_pok_effd(const stats *s){
  return s->effd;
}

static inline float
calc_pok_mhp(const stats *s){
  return s->mhp;
}

static inline float
calc_pok_bulk(const stats *s){
  return s->bulk();
}

static void
order_ivs_internal(const species &s, int cpceil, stats *svec, bool shadow){
  unsigned idx = 0;
  for(int iva = 0 ; iva < 16 ; ++iva){
    for(int ivd = 0 ; ivd < 16 ; ++ivd){
      for(int ivs = 0 ; ivs < 16 ; ++ivs){
        auto &st = svec[idx];
        const unsigned moda = s.atk + iva;
        const unsigned modd = s.def + ivd;
        const unsigned mods = s.sta + ivs;
        st.hlevel = maxlevel_cp_bounded(moda, modd, mods, cpceil, &st.cp);
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
order_ivs(const species &s, int cpceil, int(*cmpfxn)(const void*, const void*),
          unsigned *vcount){
  *vcount = IVLEVVEC;
  if(s.shadow){
    *vcount *= 2;
  }
  stats *svec = new stats[*vcount];
  order_ivs_internal(s, cpceil, svec, false);
  if(s.shadow){
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
calc_fitfxn(float (*fitfxn)(const stats *), const species &s,
            unsigned ia, unsigned id, unsigned is, unsigned hl,
            bool shadow){
  stats st{&s, hl, ia, id, is, shadow};
//std::cerr << s->name << " " << ia << " " << id << " " << is << " " << hl << std::endl;
  return fitfxn(&st);
}

// optimize on comparable fitness function subject to floor
static int
update_optset(stats** osets, const species &s, unsigned ia, unsigned id,
              unsigned is, unsigned hl, float floor, float* minfit,
              bool isshadow, float (*fitfxn)(const stats *)){
  stats **prev = osets;
  stats *cur;
  float m = calc_fitfxn(fitfxn, s, ia, id, is, hl, isshadow);
  unsigned moda = s.atk + ia;
  unsigned modd = s.def + id;
  float effa = calc_eff_a(moda, hl, isshadow);
  float effd = calc_eff_d(modd, hl, isshadow);
  unsigned mods = s.sta + is;
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
  cur = new stats(&s, hl, ia, id, is, isshadow);
  cur->next = *prev;
  *prev = cur;
  return 0;
}

// returns the optimal levels+ivs (using provided comparable fitness function)
// with a CP less than or equal to cpceil and fitness function greater than or
// equal to floor.
static stats *
find_optimal_set(const species &s, int cpceil, float floor, bool isshadow, float(*fitfxn)(const stats *)){
  stats* optsets = NULL;
  float minfit = -1;
  for(int iva = 0 ; iva < 16 ; ++iva){
    for(int ivd = 0 ; ivd < 16 ; ++ivd){
      for(int ivs = 0 ; ivs < 16 ; ++ivs){
        int cp;
        unsigned hl = maxlevel_cp_bounded(s.atk + iva, s.def + ivd, s.sta + ivs, cpceil, &cp);
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
    stats *cur;
    cur = optsets;
    optsets = cur->next;
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

// t must match some type (case insensitive). there cannot be leading or trailing
// characters, even whitespace.
static inline pgo_types_e
lookup_type(const char *t){
  for(unsigned i = 0 ; i < TYPECOUNT ; ++i){
    if(strcasecmp(t, tnames[i]) == 0){
      return static_cast<pgo_types_e>(i);
    }
  }
  return TYPECOUNT;
}

static inline const species*
lookup_species(const char* name){
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(strcasecmp(s->name.c_str(), name) == 0){
      return &*s;
    }
  }
  return nullptr;
}

static inline const species*
lookup_species(unsigned idx){
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(s->idx == idx){
      return &*s;
    }
  }
  return nullptr;
}

// look up the mega by name. if it is found, return a species synthesized from
// the mega in sbacking
static inline bool
lookup_mega(const char *name, species *sbacking){
  for(auto s = species_begin() ; s != species_end() ; ++s){
    for(const auto &m : s->mforms){
      if(strcasecmp(m.name.c_str(), name) == 0){
        sbacking->synth_mega_species(&*s, m);
        return true;
      }
    }
  }
  return false;
}

static inline void
print_type(pgo_types_e t){
  if(t != TYPECOUNT){
    printf("\\calign{\\includegraphics[height=1em,keepaspectratio]{images/%s.png}}", tnames[t]);
  }
}

// emit the symbols for some typing. nothing is shown for TYPECOUNT, and
// monotypes are only displayed once.
static inline void
print_types(pgo_types_e t1, pgo_types_e t2){
  print_type(t1);
  if(t1 != t2 && t2 != TYPECOUNT){
    putc(' ', stdout);
    print_type(t2);
  }
}

static inline void
print_type_big(pgo_types_e t){
  if(t != TYPECOUNT){
    printf("\\calign{\\includegraphics[height=1.5em,keepaspectratio]{images/%s.png}}", tnames[t]);
  }
}

static inline int
print_types_big(pgo_types_e t1, pgo_types_e t2){
  print_type_big(t1);
  if(t1 != t2){
    putc(' ', stdout);
    print_type_big(t2);
    return 2;
  }
  return 1;
}

// how many species can learn this attack? we don't count mega/primal forms.
// number that have STAB written to *stab.
static inline unsigned
learner_count(const attack* as, unsigned* stab){
  *stab = 0;
  unsigned count = 0;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    for(const auto &a : s->attacks){
      if(strcmp(a->name, as->name) == 0){
        if(has_stab_p(*s, as)){
          ++*stab;
        }
        ++count;
        break;
      }
    }
  }
  return count;
}

// observes order, which might be unexpected. i.e. TYPE_BUG, TYPE_FIGHTING
// matches only bug+fighting, not the functionally equivalent fighting+bug.
static inline unsigned
typing_popcount(pgo_types_e t1, pgo_types_e t2){
  if(t2 == t1){
    t2 = TYPECOUNT;
  }
  unsigned pcnt = 0;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if((s->t1 == t1 && s->t2 == t2)){// || (s->t2 == t1 && s->t1 == t2)){
      ++pcnt;
    }
  }
  return pcnt;
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

static inline bool
exclusive_attack_p(const species &s, const attack *a){
  for(const auto &atk : s.elite){
    if(strcmp(a->name, atk->name) == 0){
      return true;
    }
  }
  return false;
}

// does this species have one or more mega form?
static inline bool
has_mega(const species &s){
  if(s.mforms.size()){
    return true;
  }
  return false;
}

// does this species have a Dynamax form?
static inline bool
has_dmax(const species &s){
  return s.dmax;
}

// does this species have a Gigantamax form?
static inline bool
has_gmax(const species &s){
  return !!lookup_gmax_attack(s);
}

// fill in vec with persistent evolution target(s), if there are any.
// return true if there was at least one, and false otherwise.
static bool
get_persistent_evolutions(const species &s, std::vector<const species*>& vec){
  for(auto t = species_begin() ; t != species_end() ; ++t){
    if(t->from == s.name){
      vec.push_back(&*t);
    }
  }
  return vec.size() > 0;
}

// fill in vec with evolution target(s), if there are any.
// return true if there was at least one, and false otherwise.
static inline bool
get_evolutions(const species *s, std::vector<const species*> &vec,
               std::forward_list<species> &store){
  for(auto t = species_begin() ; t != species_end() ; ++t){
    if(t->from == s->name){
      vec.push_back(&*t);
    }
  }
  for(const auto &m : s->mforms){
    store.emplace_front(s, m);
    vec.push_back(&store.front());
  }
  return vec.size() > 0;
}

static const species *
get_previous_evolution(const species &s){
  if(s.from.empty()){
    return nullptr;
  }
  for(auto t = species_begin() ; t != species_end() ; ++t){
    if(s.from == t->name){
      return &*t;
    }
  }
  return nullptr;
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

static inline unsigned
get_stage(const species &s){
  const species *devol = get_previous_evolution(s);
  if(!devol){
    return 1;
  }
  return get_stage(*devol) + 1;
}

static bool
name_in_list(const species &s, const char **l){
  for(const char **n = l ; *n ; ++n){
    if(strcmp(s.name.c_str(), *n) == 0){
      return true;
    }
  }
  return false;
}

static inline unsigned
stardust_reward(const species &s){
  // some legendaries are technically evolutions, but they ought be 100 across the board
  if(s.category == species::CAT_LEGENDARY){
    return 100;
  }
  if(s.name == "Audino"){
    return 2100;
  }else if(s.name == "Cloyster"){
    return 1200;
  }else if(s.name == "Shellder" || s.name == "Chimecho"){
    return 1000;
  }
  static const char *sd950s[] = {
    "Alolan Persian",
    "Starmie",
    "Vespiquen",
    "Garbodor",
    nullptr
  };
  if(name_in_list(s, sd950s)){
    return 950;
  }
  static const char *sd750s[] = {
    "Alolan Meowth",
    "Staryu",
    "Sableye",
    "Combee",
    "Trubbish",
  };
  if(name_in_list(s, sd750s)){
    return 750;
  }
  static const char *sd700s[] = {
    "Parasect",
    "Persian",
    "Breloom",
    "Amoonguss",
    "Shiinotic",
  };
  if(name_in_list(s, sd700s)){
    return 700;
  }
  static const char *sd600s[] = {
    "Flamigo",
  };
  if(name_in_list(s, sd600s)){
    return 600;
  }
  static const char *sd500s[] = {
    "Paras",
    "Meowth",
    "Delibird",
    "Shroomish",
    "Foongus",
    "Morelull",
  };
  if(name_in_list(s, sd500s)){
    return 500;
  }
  auto stage = get_stage(s);
  if(stage == 3){
    return 500;
  }else if(stage == 2){
    return 300;
  }
  return 100;
}

// get the number of persistent evolutions above this species. e.g. for ralts
// return 3 (kirlia, gallade, gardevoir), for kirlia return 2, for gallade
// return 0. the vector is populated with all evolutions, topologically sorted,
// and the return value is equal to its size.
static inline unsigned
get_evolution_count(const species &s, std::vector<const species*>& vec){
  unsigned ret = 0;
  std::vector<const species*> evols;
  get_persistent_evolutions(s, evols);
  ret += evols.size();
  for(const auto e : evols){
    vec.push_back(e);
    ret += get_evolution_count(*e, vec);
  }
  return ret;
}

// does the species s specify a mega form (according to its name)?
static inline bool
ismega_p(const species &s){
#define MEGASTR "Mega "
  if(!s.name.compare(0, strlen(MEGASTR), MEGASTR)){
    return true;
  }
#undef MEGASTR
#define PRIMALSTR "Primal "
  if(!s.name.compare(0, strlen(PRIMALSTR), PRIMALSTR)){
    return true;
  }
#undef PRIMALSTR
  return false;
}

// return the named charged attack iff s can learn it
static inline const attack *
species_charged_attack(const species *s, const char *aname){
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
static const attack *
species_fast_attack(const species *s, const char *aname){
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
lex_ivlevel(const char* ivl, stats* s, const species &sp, bool shadow){
  int r;
  unsigned cp;
  // allow leading whitespace
  while(isspace(*ivl)){
    ++ivl;
  }
  if((r = sscanf(ivl, "opt%u", &cp)) == 1){
    stats *st = find_optimal_set(sp, cp, 0, shadow, calc_pok_gmean);
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
    s->hlevel = MAX_HALFLEVEL_BASIC;
  }else if((r = sscanf(ivl, " %u-%u-%u@", &s->ia, &s->id, &s->is)) == 3){
    ivl = strchr(ivl, '@');
    if(!ivl || !isalnum(*++ivl)){
      fprintf(stderr, "error lexing L from %s\n", ivl);
      return -1;
    }
    if(strcmp(ivl, "gl") == 0){
      int cp;
      s->hlevel = maxlevel_cp_bounded(sp.atk + s->ia, sp.def + s->id, sp.sta + s->is, GLCPCAP, &cp);
    }else if(strcmp(ivl, "ul") == 0){
      int cp;
      s->hlevel = maxlevel_cp_bounded(sp.atk + s->ia, sp.def + s->id, sp.sta + s->is, ULCPCAP, &cp);
    }else{
      char *endptr;
      s->hlevel = strtoul(ivl, &endptr, 10);
      while(*endptr){
        if(!isspace(*endptr)){
          fprintf(stderr, "invalid characters after level %s\n", endptr);
          return -1;
        }
        ++endptr;
      }
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
fill_stats(stats* s, const species &sp, bool shadow){
  // these are actually a_raw and d_raw, used only for CMP (so can we kill effd?)
  s->effa = calc_eff_a(sp.atk + s->ia, s->hlevel, false);
  s->effd = calc_eff_d(sp.def + s->id, s->hlevel, false);
  s->mhp = calc_mhp(sp.sta + s->is, s->hlevel);
  s->geommean = calc_gmean(s->effa, s->effd, s->mhp);
  s->average = calc_amean(s->effa, s->effd, s->mhp);
  s->cp = calccp(sp.atk + s->ia, sp.def + s->id, sp.sta + s->is, s->hlevel);
  s->shadow = shadow;
  s->next = NULL;
}

// pass in argv at the start of the pmon spec with argc downadjusted
static inline int
lex_pmon(pmon* p, uint16_t *hp, int *argc, char ***argv){
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
  const species *sp;
  if((sp = lookup_species(spstr)) == nullptr){
    std::cerr << "no such species: " << spstr << std::endl;
    return -1;
  }
  if(shadow){
    if(!sp->shadow){ // still allow it, but warn
      std::cerr << "warning: " << spstr << " does not have a shadow form" << std::endl;
    }
  }
  if(lex_ivlevel((*argv)[1], &p->s, *sp, shadow)){
    std::cerr << "invalid IV@level in " << (*argv)[1] << std::endl;
    return -1;
  }
  p->fa = species_fast_attack(sp, (*argv)[2]);
  p->ca1 = lex_species_charged_attacks(sp, (*argv)[3], &p->ca2);
  if(!p->fa || !p->ca1){
    fprintf(stderr, "invalid attacks for %s: '%s' '%s'\n", sp->name.c_str(),
            (*argv)[2], (*argv)[3]);
    return -1;
  }
  fill_stats(&p->s, *sp, shadow);
  *hp = p->s.mhp;
  (*argv) += 4;
  *argc -= 4;
  return 0;
}

static inline bool
aset_can_throw_p(const std::vector<const attack*>& atks, pgo_types_e t0, pgo_types_e t1){
  bool b0 = false;
  bool b1 = false;
  for(const auto &a : atks){
    if(fast_attack_p(a)){
      continue;
    }
    if(a->type == t0){
      b0 = true;
    }
    if(a->type == t1){
      b1 = true;
    }
  }
  return b0 && (b1 || t1 == TYPECOUNT);
}

// can the specified species throw a charged attack of type t0, and (if
// t1 is not TYPECOUNT) a charged attack of type t1?
static inline bool
species_can_throw_p(const species *s, pgo_types_e t0, pgo_types_e t1){
  return aset_can_throw_p(s->attacks, t0, t1);
}

struct typeset {
  pgo_types_e t0;
  pgo_types_e t1; // can be the same as t1 if we only have one attack type
  pgo_types_e plustype; // megas can have a third "plus" attack; TYPECOUNT indicates no such thing
  int totals[6];  // we range from -3 to 2, inclusive
  // population that can learn a charged attack of these types
  std::vector<const species*> learnpop;
  std::vector<const mega*> learnpopm;
  float ara;

  typeset(pgo_types_e T0, pgo_types_e T1, pgo_types_e Plustype,
          const int Totals[], float ARA) :
      t0(T0),
      t1(T1),
      plustype(Plustype),
      ara(ARA) {
    memcpy(totals, Totals, sizeof(totals));
    // for 3-sets, only consider the megas
    if(plustype != TYPECOUNT){
    for(auto t = species_begin() ; t != species_end() ; ++t){
        if(!species_can_throw_p(&*t, t0, t1)){
          continue;
        }
        for(const auto &m : t->mforms){
          if(!m.plusatk){
            continue;
          }
          if(m.plusatk->type != plustype){
            continue;
          }
          learnpopm.emplace_back(&m);
        }
      }
    }else{
      // do *not* check the megas; mega evolution doesn't otherwise change attack sets
      for(auto t = species_begin() ; t != species_end() ; ++t){
        if(species_can_throw_p(&*t, t0, t1)){
          learnpop.emplace_back(&*t);
        }
      }
    }
  }

  friend bool operator<(const typeset &l, const typeset &r) {
    return l.ara < r.ara ? true :
            r.ara < l.ara ? false :
            l.learnpop.size() < r.learnpop.size() ? true :
            r.learnpop.size() < l.learnpop.size() ? false :
            l.t0 < r.t0 ? true :
            r.t0 < l.t0 ? false :
            l.t1 < r.t1 ? true : false;
  }

  friend bool operator>(const typeset &l, const typeset &r) {
    return !(l < r);
  }

};

static inline void
build_tset(std::vector<typeset> &tsets, pgo_types_e t0, pgo_types_e t1, pgo_types_e plustype){
  int totals[6] = {};
  for(int tt0 = 0 ; tt0 < TYPECOUNT ; ++tt0){
    for(int tt1 = tt0 ; tt1 < TYPECOUNT ; ++tt1){
      int e0 = typing_relation(t0, static_cast<pgo_types_e>(tt0), static_cast<pgo_types_e>(tt1));
      int e1 = typing_relation(t1, static_cast<pgo_types_e>(tt0), static_cast<pgo_types_e>(tt1));
      int e = e0 > e1 ? e0 : e1;
      if(plustype != TYPECOUNT){
        int e2 = typing_relation(plustype, static_cast<pgo_types_e>(tt0), static_cast<pgo_types_e>(tt1));
        if(e2 > e){
          e = e2;
        }
      }
      ++totals[e + 3];
    }
  }
  float ara = 0;
  for(unsigned i = 0 ; i < sizeof(totals) / sizeof(*totals) ; ++i){
    ara += type_effectiveness_mult(static_cast<int>(i) - 3) * totals[i];
  }
  ara /= TYPINGCOUNT;
  tsets.emplace_back(t0, t1, plustype, totals, ara);
}

// build the 155 diadic typings or the 18 monotypes
static inline void
build_tsets(std::vector<typeset> &tsets, bool monomode){
  for(pgo_types_e t0 = TYPESTART ; t0 < TYPECOUNT ; ++t0){
    pgo_types_e lbound, ubound;
    if(monomode){
      lbound = t0;
      ubound = t0;
      ++ubound;
    }else{
      lbound = t0;
      ++lbound;
      ubound = TYPECOUNT;
    }
    for(pgo_types_e t1 = lbound ; t1 < ubound ; ++t1){
      build_tset(tsets, t0, t1, TYPECOUNT);
    }
  }
}

// return count of shadow forms and shadow forms with normal type
static inline unsigned
shadow_count(unsigned* shadnormals){
  unsigned shadows = 0;
  *shadnormals = 0;
  for(auto t = species_begin() ; t != species_end() ; ++t){
    if(t->shadow){
      ++shadows;
      if(t->t1 == TYPE_NORMAL || t->t2 == TYPE_NORMAL){
        ++*shadnormals;
      }
    }
  }
  return shadows;
}

static inline int
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

static inline int
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

static inline void
print_buff_html(std::ostream &o, unsigned chance, int buff, const char *sig){
  if(!chance){
    return;
  }
  if(chance != 1000){ // don't print chance if it's 100%
    std::print(o, "{:g}% ", chance / 10.0);
  }
  o << sig;
  if(buff > 0){
    o << "↑";
    if(buff > 1){
      o << buff;
    }
  }else{
    o << "↓";
    if(buff < -1){
      o << -buff;
    }
  }
}

struct candidate {
  const species* s;   // species
  std::string aname;  // attack name
  unsigned hlevel;    // halflevel
  bool gmaxpower;     // gmax/eternatus power scale? if not, dmax/crowned.
  bool hasstab;       // do we have stab for the attack?
  unsigned iva;       // attack iv 0..15
  float teffective;   // type effectiveness
  pgo_types_e atype;  // attack type

  float powprod(void) const {
    unsigned p = gmaxpower ? GMAX_POWER_BASE : DMAX_POWER_BASE;
    float rp = hasstab ? calc_stab(p) : p;
    rp *= teffective;
    return rp * calc_eff_a(s->atk + iva, hlevel, false);
  }

  bool operator<(const candidate& r) const {
    if(powprod() < r.powprod()){
      return true;
    }
    return false;
  }

  bool operator>(const candidate& r) const {
    if(powprod() > r.powprod()){
      return true;
    }
    return false;
  }
};

void emit_cand(const candidate& c, unsigned maxp);
void print_species_latex(const species &s, bool overzoom, bool bg, bool mainform);
void filter_by_types(int t1, int t2, bool overzoom, bool mainform);
void add_candidate(std::vector<candidate>& cands, const species* s,
                   const char* aname, bool gmaxpower, bool stab,
                   pgo_types_e atype, float teffect);
void emit_typing_list(pgo_types_e i, pgo_types_e j);
uint64_t pgo_xp_for_level(int l);
const char* max_attack_name(pgo_types_e t);
std::vector<const attack*>::const_iterator attacks_begin(void);
std::vector<const attack*>::const_iterator attacks_end(void);
void emit_attack(const species &s, const attack *a);
void summarize_buffs(const attack *a);

const char* tname_capitalized(pgo_types_e t);

static inline const char*
tname_capitalized(int i){
  if(i < static_cast<int>(TYPESTART) || i >= static_cast<int>(TYPECOUNT)){
    return nullptr;
  }
  return tname_capitalized(static_cast<pgo_types_e>(i));
}

#endif
