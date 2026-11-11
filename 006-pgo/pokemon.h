#ifndef POKEMON_H
#define POKEMON_H

#include <string>
#include <vector>
#include <iostream>

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

#define TYPESTART TYPE_BUG

inline pgo_types_e& operator++(pgo_types_e& pt){ // ugh
  if(pt == TYPECOUNT){
    std::cerr << "attempted to increment maximum pgo_type_e" << std::endl;
    throw std::exception();
  }
  pt = static_cast<pgo_types_e>(static_cast<int>(pt) + 1);
  return pt;
}

double cpm(int halflevel);

constexpr unsigned MINCP = 10; // minimum combat power

// atk, def, and sta all ought be mod forms (i.e. sum of base and IV)
static int
calccp(unsigned atk, unsigned def, unsigned sta, unsigned halflevel){
  float cand = (atk * sqrt(def) * sqrt(sta) * pow(cpm(halflevel), 2)) / 10;
  return cand < MINCP ? MINCP : floor(cand);
}

constexpr unsigned MAX_HALFLEVEL_BASIC = 99;

int type_relation(pgo_types_e atype, pgo_types_e ttype);

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

// the range of possible type effect is [ -3 .. 2 ]
static constexpr int TEFFECTRANGE = 6;

static inline float
type_effectiveness_mult(int te){
  if(te < -3 || te > 2){
    throw std::invalid_argument("bad type effectiveness");
  }
  static const float pow16[TEFFECTRANGE] = { 0.244, 0.390625, 0.625, 1, 1.6, 2.56 };
  return pow16[te + 3];
}

// calculate type relation of at on dt0 + dt1
static inline float
type_effectiveness(pgo_types_e at, pgo_types_e dt0, pgo_types_e dt1){
  return type_effectiveness_mult(typing_relation(at, dt0, dt1));
}

struct attack {
  const std::string name;
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

  attack(const std::string &Name, pgo_types_e Type,
      unsigned Powertrain, int Energytrain, unsigned Turns,
      unsigned Chance_user_attack, unsigned Chance_user_defense, unsigned Chance_opp_attack, unsigned Chance_opp_defense,
      int User_attack, int User_defense, int Opp_attack, int Opp_defense,
      int Powerraid, int Energyraid, int Animdur, bool Adveffect) :
    name(Name),
    type(Type),
    powertrain(Powertrain),
    energytrain(Energytrain),
    turns(Turns),
    chance_user_attack(Chance_user_attack),
    chance_user_defense(Chance_user_defense),
    chance_opp_attack(Chance_opp_attack),
    chance_opp_defense(Chance_opp_defense),
    user_attack(User_attack),
    user_defense(User_defense),
    opp_attack(Opp_attack),
    opp_defense(Opp_defense),
    powerraid(Powerraid),
    energyraid(Energyraid),
    animdur(Animdur),
    adveffect(Adveffect)
  {
  }

  attack(attack& a) = default;

  // synthesize a level{2, 3, 4} super mega attack
  attack(const attack& a, int mlevel) :
    name(a.name + std::to_string(mlevel)),
    type(a.type),
    powertrain(a.powertrain),
    energytrain(a.energytrain),
    turns(a.turns),
    chance_user_attack(a.chance_user_attack),
    chance_user_defense(a.chance_user_defense),
    chance_opp_attack(a.chance_opp_attack),
    chance_opp_defense(a.chance_opp_defense),
    user_attack(a.user_attack),
    user_defense(a.user_defense),
    opp_attack(a.opp_attack),
    opp_defense(a.opp_defense),
    powerraid(a.powerraid),
    energyraid(a.energyraid),
    animdur(a.animdur),
    adveffect(a.adveffect)
  {
    assert(mlevel >= 2 && mlevel <= 4);
  }

};

static const attack ATK_Acid{ "Acid", TYPE_POISON, 6, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 10, 2, false, };
static const attack ATK_Air_Slash{ "Air Slash", TYPE_FLYING, 9, 9, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 8, 2, false, };
static const attack ATK_Astonish{ "Astonish", TYPE_GHOST, 12, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	7, 13, 2, false, };
static const attack ATK_Bite{ "Bite", TYPE_DARK, 2, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 4, 1, false, };
static const attack ATK_Bubble{ "Bubble", TYPE_WATER, 8, 11, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 12, 2, false, };
static const attack ATK_Bug_Bite{ "Bug Bite", TYPE_BUG, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 6, 1, false, };
static const attack ATK_Bullet_Punch{ "Bullet Punch", TYPE_STEEL, 7, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 11, 2, false, };
static const attack ATK_Bullet_Seed{ "Bullet Seed", TYPE_GRASS, 5, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	7, 13, 2, false, };
static const attack ATK_Charge_Beam{ "Charge Beam", TYPE_ELECTRIC, 6, 11, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	7, 14, 2, false, };
static const attack ATK_Charm{ "Charm", TYPE_FAIRY, 12, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	20, 11, 3, false, };
static const attack ATK_Confusion{ "Confusion", TYPE_PSYCHIC, 16, 14, 4, 0, 0, 0, 0, 0, 0, 0, 0,
	19, 14, 3, false, };
static const attack ATK_Counter{ "Counter", TYPE_FIGHTING, 8, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 9, 2, false, };
static const attack ATK_Cut{ "Cut", TYPE_NORMAL, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 1, false, };
static const attack ATK_Double_Kick{ "Double Kick", TYPE_FIGHTING, 8, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 13, 2, false, };
static const attack ATK_Dragon_Breath{ "Dragon Breath", TYPE_DRAGON, 3, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 4, 1, false, };
static const attack ATK_Dragon_Tail{ "Dragon Tail", TYPE_DRAGON, 9, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	14, 8, 2, false, };
static const attack ATK_Ember{ "Ember", TYPE_FIRE, 4, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 10, 2, false, };
static const attack ATK_Extrasensory{ "Extrasensory", TYPE_PSYCHIC, 8, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 11, 2, false, };
static const attack ATK_Feint_Attack{ "Feint Attack", TYPE_DARK, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 10, 2, false, };
static const attack ATK_Fire_Fang{ "Fire Fang", TYPE_FIRE, 8, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 9, 2, false, };
static const attack ATK_Fairy_Wind{ "Fairy Wind", TYPE_FAIRY, 4, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	9, 13, 2, false, };
static const attack ATK_Fire_Spin{ "Fire Spin", TYPE_FIRE, 11, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 9, 2, false, };
static const attack ATK_Force_Palm{ "Force Palm", TYPE_FIGHTING, 13, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 16, 2, false, };
static const attack ATK_Frost_Breath{ "Frost Breath", TYPE_ICE, 7, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 9, 2, false, };
static const attack ATK_Fury_Cutter{ "Fury Cutter", TYPE_BUG, 3, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 8, 1, false, };
static const attack ATK_Geomancy{ "Geomancy", TYPE_FAIRY, 8, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	20, 14, 3, false, };
static const attack ATK_Gust{ "Gust", TYPE_FLYING, 16, 14, 4, 0, 0, 0, 0, 0, 0, 0, 0,
	25, 20, 4, false, };
static const attack ATK_Hex{ "Hex", TYPE_GHOST, 7, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	8, 13, 2, false, };
static const attack ATK_Hidden_Power{ "Hidden Power", TYPECOUNT, 9, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	15, 15, 3, false, };
static const attack ATK_Ice_Fang{ "Ice Fang", TYPE_ICE, 8, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 20, 3, false, };
static const attack ATK_Ice_Shard{ "Ice Shard", TYPE_ICE, 9, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 10, 2, false, };
static const attack ATK_Incinerate{ "Incinerate", TYPE_FIRE, 20, 20, 5, 0, 0, 0, 0, 0, 0, 0, 0,
	32, 22, 5, false, };
static const attack ATK_Infestation{ "Infestation", TYPE_BUG, 10, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	9, 13, 2, false, };
static const attack ATK_Iron_Tail{ "Iron Tail", TYPE_STEEL, 10, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	14, 6, 2, false, };
static const attack ATK_Karate_Chop{ "Karate Chop", TYPE_FIGHTING, 5, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 13, 2, false, };
static const attack ATK_Leafage{ "Leafage", TYPE_GRASS, 6, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 4, 1, false, };
static const attack ATK_Lick{ "Lick", TYPE_GHOST, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 6, 1, false, };
static const attack ATK_Lock_On{ "Lock On", TYPE_NORMAL, 1, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	2, 10, 1, false, };
static const attack ATK_Low_Kick{ "Low Kick", TYPE_FIGHTING, 6, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 1, false, };
static const attack ATK_Magical_Leaf{ "Magical Leaf", TYPE_GRASS, 10, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	17, 17, 3, false, };
static const attack ATK_Metal_Claw{ "Metal Claw", TYPE_STEEL, 5, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 5, 1, false, };
static const attack ATK_Metal_Sound{ "Metal Sound", TYPE_STEEL, 5, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Mud_Shot{ "Mud Shot", TYPE_GROUND, 3, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Mud_Slap{ "Mud-Slap", TYPE_GROUND, 11, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	19, 13, 3, false, };
static const attack ATK_Peck{ "Peck", TYPE_FLYING, 6, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 10, 2, false, };
static const attack ATK_Poison_Jab{ "Poison Jab", TYPE_POISON, 7, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 9, 2, false, };
static const attack ATK_Poison_Sting{ "Poison Sting", TYPE_POISON, 4, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Pound{ "Pound", TYPE_NORMAL, 4, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 5, 1, false, };
static const attack ATK_Powder_Snow{ "Powder Snow", TYPE_ICE, 6, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 15, 2, false, };
static const attack ATK_Present{ "Present", TYPE_NORMAL, 3, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 23, 3, false, };
static const attack ATK_Psycho_Cut{ "Psycho Cut", TYPE_PSYCHIC, 4, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 7, 1, false, };
static const attack ATK_Psywave{ "Psywave", TYPE_PSYCHIC, 3, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 7, 1, false, };
static const attack ATK_Quick_Attack{ "Quick Attack", TYPE_NORMAL, 5, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 13, 2, false, };
static const attack ATK_Razor_Leaf{ "Razor Leaf", TYPE_GRASS, 9, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 7, 2, false, };
static const attack ATK_Rock_Smash{ "Rock Smash", TYPE_FIGHTING, 9, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	17, 12, 3, false, };
static const attack ATK_Rock_Throw{ "Rock Throw", TYPE_ROCK, 8, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 8, 2, false, };
static const attack ATK_Rollout{ "Rollout", TYPE_ROCK, 7, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	15, 19, 3, false, };
static const attack ATK_Sand_Attack{ "Sand Attack", TYPE_GROUND, 2, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 7, 1, false, };
static const attack ATK_Scratch{ "Scratch", TYPE_NORMAL, 3, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 4, 1, false, };
static const attack ATK_Shadow_Claw{ "Shadow Claw", TYPE_GHOST, 6, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 4, 1, false, };
static const attack ATK_Smack_Down{ "Smack Down", TYPE_ROCK, 11, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 7, 2, false, };
static const attack ATK_Snarl{ "Snarl", TYPE_DARK, 5, 13, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 13, 2, false, };
static const attack ATK_Soak{ "Soak", TYPE_WATER, 6, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 13, 2, false, };
static const attack ATK_Spark{ "Spark", TYPE_ELECTRIC, 5, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 6, 1, false, };
static const attack ATK_Splash{ "Splash", TYPE_WATER, 0, 12, 4, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 17, 3, false, };
static const attack ATK_Steel_Wing{ "Steel Wing", TYPE_STEEL, 7, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	14, 8, 2, false, };
static const attack ATK_Struggle_Bug{ "Struggle Bug", TYPE_BUG, 9, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	15, 15, 3, false, };
static const attack ATK_Sucker_Punch{ "Sucker Punch", TYPE_DARK, 8, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 6, 1, false, };
static const attack ATK_Tackle{ "Tackle", TYPE_NORMAL, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 1, false, };
static const attack ATK_Take_Down{ "Take Down", TYPE_NORMAL, 14, 9, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	7, 8, 2, false, };
static const attack ATK_Thunder_Fang{ "Thunder Fang", TYPE_ELECTRIC, 8, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 13, 2, false, };
static const attack ATK_Thunder_Shock{ "Thunder Shock", TYPE_ELECTRIC, 4, 9, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 7, 1, false, };
static const attack ATK_Transform{ "Transform", TYPE_NORMAL, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 4, false, };
static const attack ATK_Vine_Whip{ "Vine Whip", TYPE_GRASS, 5, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	6, 5, 1, false, };
static const attack ATK_Volt_Switch{ "Volt Switch", TYPE_ELECTRIC, 14, 16, 4, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 20, 3, false, };
static const attack ATK_Water_Gun{ "Water Gun", TYPE_WATER, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 1, false, };
static const attack ATK_Waterfall{ "Waterfall", TYPE_WATER, 11, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	13, 7, 2, false, };
static const attack ATK_Water_Shuriken{ "Water Shuriken", TYPE_WATER, 6, 14, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	9, 14, 2, false, };
static const attack ATK_Wing_Attack{ "Wing Attack", TYPE_FLYING, 5, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 11, 2, false, };
static const attack ATK_Yawn{ "Yawn", TYPE_NORMAL, 0, 12, 4, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 13, 3, false, };
static const attack ATK_Zen_Headbutt{ "Zen Headbutt", TYPE_PSYCHIC, 8, 6, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	11, 9, 2, false, };
static const attack ATK_Acid_Spray{ "Acid Spray", TYPE_POISON, 20, -45, 0, 0, 0, 0, 1000, 0, 0, 0, -2,
	20, 50, 6, false, };
static const attack ATK_Acid_Spray_Plus{ "Acid Spray+", TYPE_POISON, 20, -40, 0, 0, 0, 0, 1000, 0, 0, 0, -2,
	160, 100, 6, false, };
static const attack ATK_Acrobatics{ "Acrobatics", TYPE_FLYING, 110, -55, 0, 125, 0, 0, 0, 2, 0, 0, 0,
	100, 100, 4, false, };
static const attack ATK_Aerial_Ace{ "Aerial Ace", TYPE_FLYING, 60, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 5, false, };
static const attack ATK_Aeroblast{ "Aeroblast", TYPE_FLYING, 170, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	200, 100, 7, false, };
static const attack ATK_Air_Cutter{ "Air Cutter", TYPE_FLYING, 60, -40, 0, 125, 0, 0, 0, 1, 0, 0, 0,
	55, 50, 5, false, };
static const attack ATK_Ancient_Power{ "Ancient Power", TYPE_ROCK, 60, -45, 0, 100, 100, 0, 0, 1, 1, 0, 0,
	70, 33, 7, false, };
static const attack ATK_Aqua_Jet{ "Aqua Jet", TYPE_WATER, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 5, false, };
static const attack ATK_Aqua_Step{ "Aqua Step", TYPE_WATER, 55, -40, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	55, 33, 7, false, };
static const attack ATK_Aqua_Tail{ "Aqua Tail", TYPE_WATER, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Aura_Sphere{ "Aura Sphere", TYPE_FIGHTING, 80, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 4, false, };
static const attack ATK_Aura_Wheel{ "Aura Wheel", TYPE_ELECTRIC, 100, -45, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	100, 50, 5, false, };
static const attack ATK_Aurora_Beam{ "Aurora Beam", TYPE_ICE, 80, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 7, false, };
static const attack ATK_Avalanche{ "Avalanche", TYPE_ICE, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 5, false, };
static const attack ATK_Beak_Blast{ "Beak Blast", TYPE_FLYING, 110, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 1, true, };
static const attack ATK_Behemoth_Bash{ "Behemoth Bash", TYPE_STEEL, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	125, 50, 3, true, };
static const attack ATK_Behemoth_Blade{ "Behemoth Blade", TYPE_STEEL, 100, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	200, 100, 7, true, };
static const attack ATK_Blast_Burn{ "Blast Burn", TYPE_FIRE, 110, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 50, 7, false, };
static const attack ATK_Blaze_Kick{ "Blaze Kick", TYPE_FIRE, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	40, 33, 2, false, };
static const attack ATK_Bleakwind_Storm{ "Bleakwind Storm", TYPE_FLYING, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	150, 100, 5, false, };
static const attack ATK_Blizzard{ "Blizzard", TYPE_ICE, 140, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	130, 100, 6, false, };
static const attack ATK_Body_Slam{ "Body Slam", TYPE_NORMAL, 65, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Boomburst{ "Boomburst", TYPE_NORMAL, 150, -70, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	150, 100, 5, false, };
static const attack ATK_Bone_Club{ "Bone Club", TYPE_GROUND, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	40, 33, 3, false, };
static const attack ATK_Brave_Bird{ "Brave Bird", TYPE_FLYING, 130, -55, 0, 0, 1000, 0, 0, 0, -3, 0, 0,
	130, 100, 4, false, };
static const attack ATK_Brave_Bird_Plus{ "Brave Bird+", TYPE_FLYING, 70, -40, 0, 0, 1000, 0, 0, 0, -3, 0, 0,
	150, 100, 4, false, };
static const attack ATK_Breaking_Swipe{ "Breaking Swipe", TYPE_DRAGON, 50, -50, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	45, 33, 2, false, };
static const attack ATK_Brick_Break{ "Brick Break", TYPE_FIGHTING, 50, -40, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	40, 33, 3, false, };
static const attack ATK_Brick_Break_Plus{ "Brick Break+", TYPE_FIGHTING, 40, -35, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	150, 100, 3, false, };
static const attack ATK_Brutal_Swing{ "Brutal Swing", TYPE_DARK, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 33, 4, false, };
static const attack ATK_Brine{ "Brine", TYPE_WATER, 100, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 5, false, };
static const attack ATK_Bubble_Beam{ "Bubble Beam", TYPE_WATER, 50, -50, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	45, 33, 4, false, };
static const attack ATK_Bug_Buzz{ "Bug Buzz", TYPE_BUG, 100, -60, 0, 0, 0, 0, 300, 0, 0, 0, -1,
	95, 50, 7, false, };
static const attack ATK_Bulldoze{ "Bulldoze", TYPE_GROUND, 80, -55, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	80, 50, 7, false, };
static const attack ATK_Chilling_Water{ "Chilling Water", TYPE_WATER, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	65, 33, 7, false, };
static const attack ATK_Clanging_Scales{ "Clanging Scales", TYPE_DRAGON, 120, -45, 0, 0, 1000, 0, 0, 0, -1, 0, 0,
	120, 100, 7, false, };
static const attack ATK_Close_Combat{ "Close Combat", TYPE_FIGHTING, 100, -45, 0, 0, 1000, 0, 0, 0, -2, 0, 0,
	105, 100, 5, false, };
static const attack ATK_Crabhammer{ "Crabhammer", TYPE_WATER, 85, -50, 0, 125, 0, 0, 0, 2, 0, 0, 0,
	85, 50, 4, false, };
static const attack ATK_Cross_Chop{ "Cross Chop", TYPE_FIGHTING, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 50, 3, false, };
static const attack ATK_Cross_Poison{ "Cross Poison", TYPE_POISON, 50, -35, 0, 125, 0, 0, 0, 2, 0, 0, 0,
	40, 33, 3, false, };
static const attack ATK_Crunch{ "Crunch", TYPE_DARK, 70, -45, 0, 0, 0, 0, 200, 0, 0, 0, -1,
	65, 33, 6, false, };
static const attack ATK_Crush_Grip{ "Crush Grip", TYPE_NORMAL, 110, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	210, 100, 4, false, };
static const attack ATK_Darkest_Lariat{ "Darkest Lariat", TYPE_DARK, 120, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 4, false, };
static const attack ATK_Dark_Pulse{ "Dark Pulse", TYPE_DARK, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 6, false, };
static const attack ATK_Dark_Pulse_Plus{ "Dark Pulse+", TYPE_DARK, 60, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	150, 100, 6, false, };
static const attack ATK_Dazzling_Gleam{ "Dazzling Gleam", TYPE_FAIRY, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  100, 50, 7, false, };
static const attack ATK_Dig{ "Dig", TYPE_GROUND, 70, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 9, false, };
static const attack ATK_Disarming_Voice{ "Disarming Voice", TYPE_FAIRY, 70, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 8, false, };
static const attack ATK_Discharge{ "Discharge", TYPE_ELECTRIC, 55, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 33, 5, false, };
static const attack ATK_Discharge_Plus{ "Discharge+", TYPE_ELECTRIC, 50, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	150, 100, 5, false, };
static const attack ATK_Dive{ "Dive", TYPE_WATER, 50, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 7, false, };
static const attack ATK_Doom_Desire{ "Doom Desire", TYPE_STEEL, 80, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 33, 3, false, };
static const attack ATK_Double_Iron_Bash{ "Double Iron Bash", TYPE_STEEL, 70, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 4, false, };
static const attack ATK_Draco_Meteor{ "Draco Meteor", TYPE_DRAGON, 150, -65, 0, 1000, 0, 0, 0, -2, 0, 0, 0,
	150, 100, 7, false, };
static const attack ATK_Dragon_Ascent{ "Dragon Ascent", TYPE_FLYING, 110, -45, 0, 0, 1000, 0, 0, 0, -1, 0, 0,
	140, 50, 7, false, };
static const attack ATK_Dragon_Claw{ "Dragon Claw", TYPE_DRAGON, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 3, false, };
static const attack ATK_Dragon_Energy{ "Dragon Energy", TYPE_DRAGON, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	155, 50, 7, false, };
static const attack ATK_Dragon_Pulse{ "Dragon Pulse", TYPE_DRAGON, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 7, false, };
static const attack ATK_Drain_Punch{ "Drain Punch", TYPE_FIGHTING, 40, -40, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	50, 33, 5, false, };
static const attack ATK_Draining_Kiss{ "Draining Kiss", TYPE_FAIRY, 80, -55, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	60, 50, 5, false, };
static const attack ATK_Drill_Peck{ "Drill Peck", TYPE_FLYING, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 5, false, };
static const attack ATK_Drill_Peck_Plus{ "Drill Peck+", TYPE_FLYING, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	170, 100, 5, false, };
static const attack ATK_Drill_Run{ "Drill Run", TYPE_GROUND, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 6, false, };
static const attack ATK_Drum_Beating{ "Drum Beating", TYPE_GRASS, 60, -35, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	115, 33, 8, false, };
static const attack ATK_Dynamax_Cannon{ "Dynamax Cannon", TYPE_DRAGON, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	215, 100, 3, true, };
static const attack ATK_Dynamic_Punch{ "Dynamic Punch", TYPE_FIGHTING, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 5, false, };
static const attack ATK_Dynamic_Punch_Plus{ "Dynamic Punch+", TYPE_FIGHTING, 130, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	130, 100, 5, false, };
static const attack ATK_Earthquake{ "Earthquake", TYPE_GROUND, 120, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 7, false, };
static const attack ATK_Earth_Power{ "Earth Power", TYPE_GROUND, 90, -50, 0, 0, 0, 0, 100, 0, 0, 0, -1,
	100, 50, 7, false, };
static const attack ATK_Energy_Ball{ "Energy Ball", TYPE_GRASS, 80, -45, 0, 0, 0, 0, 100, 0, 0, 0, -1,
	90, 50, 8, false, };
static const attack ATK_Feather_Dance{ "Feather Dance", TYPE_FLYING, 35, -50, 0, 0, 0, 1000, 0, 0, 0, -2, 0,
	35, 50, 6, false, };
static const attack ATK_Fell_Stinger{ "Fell Stinger", TYPE_BUG, 20, -35, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	45, 33, 4, false, };
static const attack ATK_Fire_Blast{ "Fire Blast", TYPE_FIRE, 140, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 8, false, };
static const attack ATK_Fire_Punch{ "Fire Punch", TYPE_FIRE, 60, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Flame_Burst{ "Flame Burst", TYPE_FIRE, 70, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 50, 5, false, };
static const attack ATK_Flame_Charge{ "Flame Charge", TYPE_FIRE, 65, -50, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	70, 33, 8, false, };
static const attack ATK_Flame_Wheel{ "Flame Wheel", TYPE_FIRE, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 50, 5, false, };
static const attack ATK_Flamethrower{ "Flamethrower", TYPE_FIRE, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 4, false, };
static const attack ATK_Flash_Cannon{ "Flash Cannon", TYPE_STEEL, 110, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 100, 5, false, };
static const attack ATK_Flower_Trick{ "Flower Trick", TYPE_GRASS, 30, -35, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	75, 33, 5, false, };
static const attack ATK_Fly{ "Fly", TYPE_FLYING, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 4, false, };
static const attack ATK_Flying_Press{ "Flying Press", TYPE_FIGHTING, 90, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	115, 50, 5, false, };
static const attack ATK_Focus_Blast{ "Focus Blast", TYPE_FIGHTING, 150, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 7, false, };
static const attack ATK_Foul_Play{ "Foul Play", TYPE_DARK, 65, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 50, 4, false, };
static const attack ATK_Freeze_Shock{ "Freeze Shock", TYPE_ICE, 120, -60, 0, 0, 0, 300, 0, 0, 0, -1, 0,
	160, 100, 3, true, };
static const attack ATK_Frenzy_Plant{ "Frenzy Plant", TYPE_GRASS, 100, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 5, false, };
static const attack ATK_Frustration{ "Frustration", TYPE_NORMAL, 10, -70, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 33, 4, false, };
static const attack ATK_Fusion_Bolt{ "Fusion Bolt", TYPE_ELECTRIC, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 4, false, };
static const attack ATK_Fusion_Flare{ "Fusion Flare", TYPE_FIRE, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 4, false, };
static const attack ATK_Future_Sight{ "Future Sight", TYPE_PSYCHIC, 110, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	115, 100, 5, false, };
static const attack ATK_Future_Sight_Plus{ "Future Sight+", TYPE_PSYCHIC, 130, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 5, false, };
static const attack ATK_Giga_Impact{ "Giga Impact", TYPE_NORMAL, 150, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	200, 100, 9, false, };
static const attack ATK_Gigaton_Hammer{ "Gigaton Hammer", TYPE_STEEL, 130, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	300, 100, 6, false, };
static const attack ATK_Glaciate{ "Glaciate", TYPE_ICE, 60, -40, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	160, 100, 5, false, };
static const attack ATK_Grass_Knot{ "Grass Knot", TYPE_GRASS, 90, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 5, false, };
static const attack ATK_Gunk_Shot{ "Gunk Shot", TYPE_POISON, 130, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	130, 100, 6, false, };
static const attack ATK_Gyro_Ball{ "Gyro Ball", TYPE_STEEL, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 7, false, };
static const attack ATK_Heat_Wave{ "Heat Wave", TYPE_FIRE, 75, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	95, 100, 6, false, };
static const attack ATK_Heavy_Slam{ "Heavy Slam", TYPE_STEEL, 70, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 50, 4, false, };
static const attack ATK_High_Horsepower{ "High Horsepower", TYPE_GROUND, 100, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	105, 100, 3, false, };
static const attack ATK_High_Jump_Kick{ "High Jump Kick", TYPE_FIGHTING, 110, -55, 0, 0, 100, 0, 0, 0, -4, 0, 0,
	90, 100, 3, false, };
static const attack ATK_Horn_Attack{ "Horn Attack", TYPE_NORMAL, 40, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 4, false, };
static const attack ATK_Hurricane{ "Hurricane", TYPE_FLYING, 110, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	105, 100, 5, false, };
static const attack ATK_Hydro_Cannon{ "Hydro Cannon", TYPE_WATER, 80, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 4, false, };
static const attack ATK_Hydro_Pump{ "Hydro Pump", TYPE_WATER, 130, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	135, 100, 7, false, };
static const attack ATK_Hyper_Beam{ "Hyper Beam", TYPE_NORMAL, 150, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	150, 100, 8, false, };
static const attack ATK_Hyper_Fang{ "Hyper Fang", TYPE_NORMAL, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 5, false, };
static const attack ATK_Ice_Beam{ "Ice Beam", TYPE_ICE, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	95, 50, 7, false, };
static const attack ATK_Ice_Burn{ "Ice Burn", TYPE_ICE, 120, -60, 0, 0, 0, 0, 300, 0, 0, 0, -1,
	90, 50, 4, true, };
static const attack ATK_Ice_Punch{ "Ice Punch", TYPE_ICE, 60, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Icicle_Spear{ "Icicle Spear", TYPE_ICE, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 4, false, };
static const attack ATK_Icy_Wind{ "Icy Wind", TYPE_ICE, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	65, 33, 7, false, };
static const attack ATK_Iron_Head{ "Iron Head", TYPE_STEEL, 85, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 4, false, };
static const attack ATK_Kowtow_Cleave{ "Kowtow Cleave", TYPE_DARK, 75, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 4, false, };
static const attack ATK_Last_Resort{ "Last Resort", TYPE_NORMAL, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 6, false, };
static const attack ATK_Leaf_Blade{ "Leaf Blade", TYPE_GRASS, 70, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	70, 33, 5, false, };
static const attack ATK_Leaf_Storm{ "Leaf Storm", TYPE_GRASS, 130, -55, 0, 1000, 0, 0, 0, -2, 0, 0, 0,
	130, 100, 5, false, };
static const attack ATK_Leaf_Tornado{ "Leaf Tornado", TYPE_GRASS, 45, -40, 0, 0, 0, 500, 0, 0, 0, -2, 0,
	45, 33, 6, false, };
static const attack ATK_Liquidation{ "Liquidation", TYPE_WATER, 70, -45, 0, 0, 0, 0, 300, 0, 0, 0, -1,
	70, 33, 6, false, };
static const attack ATK_Liquidation_Plus{ "Liquidation+", TYPE_WATER, 55, -40, 0, 0, 0, 0, 300, 0, 0, 0, -1,
	180, 100, 6, false, };
static const attack ATK_Low_Sweep{ "Low Sweep", TYPE_FIGHTING, 40, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	40, 33, 4, false, };
static const attack ATK_Lunge{ "Lunge", TYPE_BUG, 70, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	55, 33, 6, false, };
static const attack ATK_Luster_Purge{ "Luster Purge", TYPE_PSYCHIC, 120, -60, 0, 0, 0, 0, 500, 0, 0, 0, -1,
	100, 100, 3, false, };
static const attack ATK_Magma_Storm{ "Magma Storm", TYPE_FIRE, 65, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	75, 33, 5, false, };
static const attack ATK_Magnet_Bomb{ "Magnet Bomb", TYPE_STEEL, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	75, 33, 6, false, };
static const attack ATK_Megahorn{ "Megahorn", TYPE_BUG, 110, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	105, 100, 4, false, };
static const attack ATK_Meteor_Beam{ "Meteor Beam", TYPE_ROCK, 120, -60, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	140, 100, 4, false, };
static const attack ATK_Meteor_Mash{ "Meteor Mash", TYPE_STEEL, 100, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 5, false, };
static const attack ATK_Mind_Blown{ "Mind Blown", TYPE_FIRE, 90, -35, 0, 0, 1000, 0, 0, 0, -4, 0, 0,
  130, 33, 8, false, };
static const attack ATK_Mirror_Coat{ "Mirror Coat", TYPE_PSYCHIC, 75, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 5, false, };
static const attack ATK_Mirror_Shot{ "Mirror Shot", TYPE_STEEL, 35, -35, 0, 0, 0, 300, 0, 0, 0, -1, 0,
	50, 33, 5, false, };
static const attack ATK_Mist_Ball{ "Mist Ball", TYPE_PSYCHIC, 120, -60, 0, 0, 0, 500, 0, 0, 0, -1, 0,
	105, 100, 4, false, };
static const attack ATK_Moonblast{ "Moonblast", TYPE_FAIRY, 90, -50, 0, 0, 0, 100, 0, 0, 0, -1, 0,
	130, 100, 8, false, };
static const attack ATK_Moonblast_Plus{ "Moonblast+", TYPE_FAIRY, 80, -50, 0, 0, 0, 100, 0, 0, 0, -1, 0,
	160, 100, 8, false, };
static const attack ATK_Moongeist_Beam{ "Moongeist Beam", TYPE_GHOST, 135, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	230, 100, 6, true, };
static const attack ATK_Mud_Bomb{ "Mud Bomb", TYPE_GROUND, 65, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 5, false, };
static const attack ATK_Muddy_Water{ "Muddy Water", TYPE_WATER, 35, -35, 0, 0, 0, 300, 0, 0, 0, -1, 0,
	45, 33, 4, false, };
static const attack ATK_Mystical_Fire{ "Mystical Fire", TYPE_FIRE, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	60, 33, 4, false, };
static const attack ATK_Mystical_Fire_Plus{ "Mystical Fire+", TYPE_FIRE, 50, -40, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	140, 100, 4, false, };
static const attack ATK_Natures_Madness{ "Nature's Madness", TYPE_FAIRY, 80, -50, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	90, 50, 4, false, };
static const attack ATK_Night_Shade{ "Night Shade", TYPE_GHOST, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 5, false, };
static const attack ATK_Night_Shade_Plus{ "Night Shade+", TYPE_GHOST, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 100, 5, false, };
static const attack ATK_Night_Slash{ "Night Slash", TYPE_DARK, 50, -35, 0, 125, 0, 0, 0, 1, 0, 0, 0,
	45, 33, 4, false, };
static const attack ATK_Oblivion_Wing{ "Oblivion Wing", TYPE_FLYING, 85, -50, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	85, 50, 4, false, };
static const attack ATK_Obstruct{ "Obstruct", TYPE_DARK, 15, -40, 0, 0, 1000, 0, 1000, 0, 1, 0, -1,
	20, 33, 3, false, };
static const attack ATK_Octazooka{ "Octazooka", TYPE_WATER, 50, -50, 0, 0, 0, 500, 0, 0, 0, -2, 0,
	55, 50, 5, false, };
static const attack ATK_Ominous_Wind{ "Ominous Wind", TYPE_GHOST, 45, -45, 0, 100, 100, 0, 0, 1, 1, 0, 0,
	55, 33, 5, false, };
static const attack ATK_Ominous_Wind_Plus{ "Ominous Wind+", TYPE_GHOST, 50, -40, 0, 100, 100, 0, 0, 1, 1, 0, 0,
	145, 100, 5, false, };
static const attack ATK_Origin_Pulse{ "Origin Pulse", TYPE_WATER, 130, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 3, false, };
static const attack ATK_Outrage{ "Outrage", TYPE_DRAGON, 110, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	110, 50, 8, false, };
static const attack ATK_Outrage_Plus{ "Outrage+", TYPE_DRAGON, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	185, 100, 8, false, };
static const attack ATK_Overheat{ "Overheat", TYPE_FIRE, 130, -55, 0, 1000, 0, 0, 0, -2, 0, 0, 0,
	160, 100, 8, false, };
static const attack ATK_Parabolic_Charge{ "Parabolic Charge", TYPE_ELECTRIC, 70, -50, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	70, 50, 6, false, };
static const attack ATK_Payback{ "Payback", TYPE_DARK, 110, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	95, 100, 4, false, };
static const attack ATK_Petal_Blizzard{ "Petal Blizzard", TYPE_GRASS, 110, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	110, 100, 5, false, };
static const attack ATK_Plasma_Fists{ "Plasma Fists", TYPE_ELECTRIC, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	135, 50, 7, false, };
static const attack ATK_Play_Rough{ "Play Rough", TYPE_FAIRY, 90, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 6, false, };
static const attack ATK_Poison_Fang{ "Poison Fang", TYPE_POISON, 50, -40, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	30, 33, 3, false, };
static const attack ATK_Poltergeist{ "Poltergeist", TYPE_GHOST, 150, -75, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 7, false, };
static const attack ATK_Power_Gem{ "Power Gem", TYPE_ROCK, 85, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 6, false, };
static const attack ATK_Power_Up_Punch{ "Power-Up Punch", TYPE_FIGHTING, 20, -35, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Power_Whip{ "Power Whip", TYPE_GRASS, 90, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	90, 50, 5, false, };
static const attack ATK_Precipice_Blades{ "Precipice Blades", TYPE_GROUND, 130, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 3, false, };
static const attack ATK_Psybeam{ "Psybeam", TYPE_PSYCHIC, 70, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 6, false, };
static const attack ATK_Psybeam_Plus{ "Psybeam+", TYPE_PSYCHIC, 60, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	170, 100, 6, false, };
static const attack ATK_Psychic{ "Psychic", TYPE_PSYCHIC, 75, -55, 0, 0, 0, 0, 100, 0, 0, 0, -1,
	95, 50, 7, false, };
static const attack ATK_Psychic_Fangs{ "Psychic Fangs", TYPE_PSYCHIC, 40, -35, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	25, 33, 2, false, };
static const attack ATK_Psycho_Boost{ "Psycho Boost", TYPE_PSYCHIC, 85, -35, 0, 1000, 0, 0, 0, -2, 0, 0, 0,
	130, 33, 8, false, };
static const attack ATK_Psyshock{ "Psyshock", TYPE_PSYCHIC, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 5, false, };
static const attack ATK_Psystrike{ "Psystrike", TYPE_PSYCHIC, 90, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	95, 50, 5, false, };
static const attack ATK_Pyro_Ball{ "Pyro Ball", TYPE_FIRE, 75, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	150, 100, 4, false, };
static const attack ATK_Rage_Fist{ "Rage Fist", TYPE_GHOST, 55, -40, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	100, 50, 6, false, };
static const attack ATK_Razor_Shell{ "Razor Shell", TYPE_WATER, 35, -35, 0, 0, 0, 0, 500, 0, 0, 0, -1,
	55, 33, 3, false, };
static const attack ATK_Return{ "Return", TYPE_NORMAL, 130, -70, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	25, 33, 1, false, };
static const attack ATK_Roar_of_Time{ "Roar of Time", TYPE_DRAGON, 150, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	160, 100, 4, true, };
static const attack ATK_Rock_Blast{ "Rock Blast", TYPE_ROCK, 50, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Rock_Slide{ "Rock Slide", TYPE_ROCK, 75, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	75, 50, 5, false, };
static const attack ATK_Rock_Tomb{ "Rock Tomb", TYPE_ROCK, 75, -50, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	65, 50, 6, false, };
static const attack ATK_Rock_Wrecker{ "Rock Wrecker", TYPE_ROCK, 110, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	110, 50, 7, false, };
static const attack ATK_Sacred_Fire{ "Sacred Fire", TYPE_FIRE, 130, -65, 0, 0, 0, 500, 0, 0, 0, -1, 0,
	120, 100, 5, false, };
static const attack ATK_Sacred_Sword{ "Sacred Sword", TYPE_FIGHTING, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 2, false, };
static const attack ATK_Sand_Tomb{ "Sand Tomb", TYPE_GROUND, 55, -45, 0, 0, 0, 0, 1000, 0, 0, 0, -1,
	60, 33, 8, false, };
static const attack ATK_Sandsear_Storm{ "Sandsear Storm", TYPE_GROUND, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	150, 100, 5, false, };
static const attack ATK_Scald{ "Scald", TYPE_WATER, 85, -50, 0, 0, 0, 300, 0, 0, 0, -1, 0,
	75, 50, 7, false, };
static const attack ATK_Scorching_Sands{ "Scorching Sands", TYPE_GROUND, 80, -50, 0, 0, 0, 100, 0, 0, 0, -1, 0,
	90, 50, 6, false, };
static const attack ATK_Secret_Sword{ "Secret Sword", TYPE_FIGHTING, 70, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 50, 4, false, };
static const attack ATK_Seed_Bomb{ "Seed Bomb", TYPE_GRASS, 55, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 4, false, };
static const attack ATK_Seed_Bomb_Plus{ "Seed Bomb+", TYPE_GRASS, 60, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	150, 100, 4, false, };
static const attack ATK_Seed_Flare{ "Seed Flare", TYPE_GRASS, 130, -75, 0, 0, 0, 0, 400, 0, 0, 0, -2,
	115, 100, 5, false, };
static const attack ATK_Shadow_Ball{ "Shadow Ball", TYPE_GHOST, 90, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 50, 6, false, };
static const attack ATK_Shadow_Bone{ "Shadow Bone", TYPE_GHOST, 80, -45, 0, 0, 0, 0, 200, 0, 0, 0, -1,
	85, 50, 6, false, };
static const attack ATK_Shadow_Force{ "Shadow Force", TYPE_GHOST, 120, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	140, 100, 4, false, };
static const attack ATK_Shadow_Punch{ "Shadow Punch", TYPE_GHOST, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	35, 33, 3, false, };
static const attack ATK_Shadow_Sneak{ "Shadow Sneak", TYPE_GHOST, 75, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 6, false, };
static const attack ATK_Signal_Beam{ "Signal Beam", TYPE_BUG, 75, -55, 0, 0, 0, 200, 200, 0, 0, -1, -1,
	75, 50, 6, false, };
static const attack ATK_Silver_Wind{ "Silver Wind", TYPE_BUG, 75, -45, 0, 100, 100, 0, 0, 1, 1, 0, 0,
	65, 33, 7, false, };
static const attack ATK_Skull_Bash{ "Skull Bash", TYPE_NORMAL, 130, -75, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	130, 100, 6, false, };
static const attack ATK_Sky_Attack{ "Sky Attack", TYPE_FLYING, 75, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 4, false, };
static const attack ATK_Sludge{ "Sludge", TYPE_POISON, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Sludge_Bomb{ "Sludge Bomb", TYPE_POISON, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 50, 5, false, };
static const attack ATK_Sludge_Wave{ "Sludge Wave", TYPE_POISON, 110, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	105, 100, 6, false, };
static const attack ATK_Snipe_Shot{ "Snipe Shot", TYPE_WATER, 65, -35, 0, 125, 0, 0, 0, 2, 0, 0, 0,
	100, 33, 7, false, };
static const attack ATK_Solar_Beam{ "Solar Beam", TYPE_GRASS, 150, -80, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	180, 100, 10, false, };
static const attack ATK_Spacial_Rend{ "Spacial Rend", TYPE_DRAGON, 95, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	160, 100, 5, true, };
static const attack ATK_Sparkling_Aria{ "Sparkling Aria", TYPE_WATER, 80, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	85, 33, 6, false, };
static const attack ATK_Spirit_Shackle{ "Spirit Shackle", TYPE_GHOST, 50, -40, 0, 0, 0, 0, 330, 0, 0, 0, -1,
	70, 33, 5, false, };
static const attack ATK_Stomp{ "Stomp", TYPE_NORMAL, 55, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 50, 3, false, };
static const attack ATK_Stone_Edge{ "Stone Edge", TYPE_ROCK, 100, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	105, 100, 5, false, };
// FIXME pokemon go hub claims 0 energy. pokebase claims 1.06 DPE which works out to 33.
static const attack ATK_Struggle{ "Struggle", TYPE_NORMAL, 35, -100, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	35, 33, 4, false, };
static const attack ATK_Submission{ "Submission", TYPE_FIGHTING, 60, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 50, 4, false, };
static const attack ATK_Sunsteel_Strike{ "Sunsteel Strike", TYPE_STEEL, 135, -65, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	230, 100, 6, true, };
static const attack ATK_Superpower{ "Superpower", TYPE_FIGHTING, 85, -40, 0, 1000, 1000, 0, 0, -1, -1, 0, 0,
	85, 50, 6, false, };
static const attack ATK_Surf{ "Surf", TYPE_WATER, 75, -45, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 50, 3, false, };
static const attack ATK_Surf_Plus{ "Surf+", TYPE_WATER, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	130, 100, 3, false, };
static const attack ATK_Swift{ "Swift", TYPE_NORMAL, 55, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 6, false, };
static const attack ATK_Synchronoise{ "Synchronoise", TYPE_PSYCHIC, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 5, false, };
static const attack ATK_Techno_Blast_Electric{ "Techno Blast ⚡", TYPE_ELECTRIC, 120, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 4, false, };
static const attack ATK_Techno_Blast_Fire{ "Techno Blast 🔥", TYPE_FIRE, 120, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 4, false, };
static const attack ATK_Techno_Blast_Ice{ "Techno Blast 🧊", TYPE_ICE, 120, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 4, false, };
static const attack ATK_Techno_Blast_Normal{ "Techno Blast", TYPE_NORMAL, 120, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 4, false, };
static const attack ATK_Techno_Blast_Water{ "Techno Blast 🌊", TYPE_WATER, 120, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	120, 100, 4, false, };
static const attack ATK_Thunder{ "Thunder", TYPE_ELECTRIC, 100, -60, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	100, 100, 7, false, };
static const attack ATK_Thunder_Punch{ "Thunder Punch", TYPE_ELECTRIC, 60, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 4, false, };
static const attack ATK_Thunderbolt{ "Thunderbolt", TYPE_ELECTRIC, 90, -55, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	80, 50, 5, false, };
static const attack ATK_Torch_Song{ "Torch Song", TYPE_FIRE, 70, -45, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	100, 50, 7, false, };
static const attack ATK_Trailblaze{ "Trailblaze", TYPE_GRASS, 65, -45, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	65, 50, 4, false, };
static const attack ATK_Tri_Attack{ "Tri Attack", TYPE_NORMAL, 65, -50, 0, 0, 0, 500, 500, 0, 0, -1, -1,
	75, 50, 5, false, };
static const attack ATK_Triple_Axel{ "Triple Axel", TYPE_ICE, 60, -45, 0, 1000, 0, 0, 0, 1, 0, 0, 0,
	60, 33, 4, false, };
static const attack ATK_Twister{ "Twister", TYPE_DRAGON, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	50, 33, 6, false, };
static const attack ATK_Upper_Hand{ "Upper Hand", TYPE_FIGHTING, 70, -40, 0, 0, 0, 0, 300, 0, 0, 0, -1,
	50, 33, 4, false, };
static const attack ATK_V_Create{ "V-Create", TYPE_FIRE, 95, -40, 0, 0, 1000, 0, 0, 0, -3, 0, 0,
	105, 33, 6, false, };
static const attack ATK_Vise_Grip{ "Vise Grip", TYPE_NORMAL, 70, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	35, 33, 4, false, };
static const attack ATK_Volt_Tackle{ "Volt Tackle", TYPE_ELECTRIC, 90, -40, 0, 0, 1000, 0, 0, 0, -1, 0, 0,
	90, 33, 7, false, };
static const attack ATK_Volt_Tackle_Plus{ "Volt Tackle+", TYPE_ELECTRIC, 65, -35, 0, 0, 1000, 0, 0, 0, -1, 0, 0,
	170, 100, 7, false, };
static const attack ATK_Water_Pulse{ "Water Pulse", TYPE_WATER, 80, -50, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	65, 50, 6, false, };
static const attack ATK_Weather_Ball_Fire{ "Weather Ball 🔥", TYPE_FIRE, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 3, false, };
static const attack ATK_Weather_Ball_Ice{ "Weather Ball 🧊", TYPE_ICE, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 3, false, };
static const attack ATK_Weather_Ball_Rock{ "Weather Ball 🪨", TYPE_ROCK, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 4, false, };
static const attack ATK_Weather_Ball_Normal{ "Weather Ball", TYPE_NORMAL, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	60, 33, 3, false, };
static const attack ATK_Weather_Ball_Water{ "Weather Ball 🌊", TYPE_WATER, 60, -35, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	55, 33, 3, false, };
static const attack ATK_Wild_Charge{ "Wild Charge", TYPE_ELECTRIC, 100, -45, 0, 0, 1000, 0, 0, 0, -2, 0, 0,
	90, 50, 5, false, };
static const attack ATK_Wildbolt_Storm{ "Wildbolt Storm", TYPE_ELECTRIC, 60, -45, 0, 0, 0, 1000, 0, 0, 0, -1, 0,
	150, 100, 5, false, };
static const attack ATK_Wrap{ "Wrap", TYPE_NORMAL, 70, -45, 0, 0, 1000, 0, 0, 0, 1, 0, 0,
	25, 33, 6, false, };
static const attack ATK_X_Scissor{ "X-Scissor", TYPE_BUG, 65, -40, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	45, 33, 3, false, };
static const attack ATK_Zap_Cannon{ "Zap Cannon", TYPE_ELECTRIC, 150, -80, 0, 0, 0, 330, 0, 0, 0, -1, 0,
	140, 100, 7, false, };
static const attack ATK_Zap_Cannon_Plus{ "Zap Cannon+", TYPE_ELECTRIC, 70, -45, 0, 0, 0, 330, 0, 0, 0, -1, 0,
	160, 100, 7, false, };

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

bool plus_attack_p(const attack &);

// all gmax attacks are 350, 400, 450, 550 damage (level 4 is achieved via the
// dynamax cannon adventure effect).
struct gmaxattack {
  const std::string sname;// species name
  const std::string name; // attack name
  pgo_types_e type;       // attack type
  bool shiny;             // shiny gmax available?
};

constexpr unsigned GMAX_TIER = 6;

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
                      // 5 is legendaries, 6 is gigantamax (if gmax *only*).
                      // crowned forms and zacian *do not* have this set!
  std::vector<const attack*> elite; // exclusive attacks requiring an elite tm
  enum species_cat {
    CAT_NORMAL,
    CAT_MYTHICAL,
    CAT_LEGENDARY,
    CAT_FUSION,
    CAT_CROWNED,
    CAT_MEGA,     // only in synthesized species
    CAT_ULTRABEAST,
    CAT_BABY,
    CAT_PARADOX,
    CAT_FPARTNER, // first partners (previously "starters")
  } category;
  int a2cost;         // cost in kStardust to teach second attack { 10, 50, 75, 100 }
                      // or to purify { maps to 1, 3, 5, 20 }
  enum evol_item {    // item required to evolve *into* this mon
    EVOL_NOITEM,
    EVOL_FEMALE,
    EVOL_MALE,
    EVOL_SUNSTONE,
    EVOL_KINGSROCK,
    EVOL_METALCOAT,
    EVOL_DRAGONSCALE,
    EVOL_UPGRADE,
    EVOL_SINNOHSTONE,
    EVOL_SINNOHSTONEFEMALE,
    EVOL_SINNOHSTONEMALE,
    EVOL_UNOVASTONE,
    EVOL_TARTAPPLE,
    EVOL_SWEETAPPLE,
    EVOL_SYRUPYAPPLE,
    EVOL_GIMMICOINS,
    EVOL_ZYGARDECELL50,
    EVOL_ZYGARDECELL200,
    EVOL_BUDDYHEARTS10NIGHT,
    EVOL_BUDDYHEARTS20,
    EVOL_BUDDYHEARTS70,
    EVOL_TREATS25,
    EVOL_EXCELLENT10,
    EVOL_RAIDDARKSTEEL15,
    EVOL_RAIDMAXWATER30,
    EVOL_RAIDMAXDARK30,
    EVOL_DEFEATPSYCHICGHOST30,
    EVOL_DEFEATPSYCHIC30,
    EVOL_DEFEATGHOST30,
    EVOL_STARDUST2K,
    EVOL_STARDUST10K,
    EVOL_CAPTUREDRAGON7,
    EVOL_CAPTUREDRAGON20,
    EVOL_CAPTUREPOISON30,
    EVOL_CAPTUREPSYCHIC30,
    EVOL_CAPTUREDARK32,
    EVOL_RAID10,
    EVOL_MAGLURE,
    EVOL_RAINLURE,     // also satisfied by rainy weather
    EVOL_GLACIALLURE,
    EVOL_MOSSYLURE,
    EVOL_ROTATEDEVICE,
    EVOL_DAY,
    EVOL_DUSK,
    EVOL_NIGHT,
    EVOL_FULLMOON,
    EVOL_INCENSE,
    EVOL_CROWNSWORD1000,
    EVOL_CROWNSHIELD1000,
    EVOL_VOLT1000,
    EVOL_BLAZE1000,
    EVOL_SOLAR1000,
    EVOL_LUNAR1000,
    EVOL_DRAGONASCENT,
  } evolitem;
  enum mon_region {    // region-specific pokémon
    REGION_ALL,
    REGION_EUROPE,
    REGION_EASTASIA,
    REGION_AUSTRALIA,
    REGION_NORTHAM,
    REGION_SOUTHEASTHEMI,
    REGION_SOUTHHEMI,
    REGION_NORTHHEMI,
    REGION_EASTHEMI,
    REGION_EASTHEMI26,
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

  // if the species has a plus attack in its attack set, expand it to four
  // versions, adding levels 2, 3, and 4. maybe we ought always do this in
  // synth_mega_species(), idk. this is to be used on a species synthesized
  // from an mform, not the database species containing the mform.
  void expand_plus_atk(std::forward_list<attack> &store){
    for(const auto pa : attacks){
      if(plus_attack_p(*pa)){
        for(int l = 2 ; l < 5 ; ++l){
          float plusmult = 1.0 + (l - 1) * 0.1;
          store.emplace_front(*pa, l);
          auto *a = &store.front();
          a->powerraid *= plusmult;
          a->powertrain *= plusmult;
          attacks.emplace_back(a);
        }
        // this will invalidate iterators, so we need leave immediately
        break;
      }
    }
  }

  // synthesize into sbacking a new species made of the mega and us
  void synth_mega_species(const species *s, const mega &m);

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
  {
    synth_mega_species(s, m);
  }

  species(const species *s, const gmaxattack &gm)
    : species(s->idx, "G-Max " + s->name, s->t1, s->t2,
              s->atk, s->def, s->sta, "",
              s->attacks,
              gm.shiny, false, GMAX_TIER,
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

  static const char *evolitemstr(species::evol_item);
  static const char *categorystr(species::species_cat);

  const char *evolitemstr() const {
    return evolitemstr(evolitem);
  }

  const char *categorystr() const {
    return categorystr(category);
  }

  // return LaTeX-ready string corresponding to region in which this form
  // spawns, or nullptr if it spawns worldwide (or not at all)
  const char *regionstr() const {
    switch(monregion){
      case REGION_ALL: return nullptr;
      case REGION_EUROPE: return "Europe";
      case REGION_EASTASIA: return "East Asia";
      case REGION_AUSTRALIA: return "Australia";
      case REGION_NORTHAM: return "Eastern Asia";
      case REGION_SOUTHEASTHEMI: return "Eastern hemisphere south of \\textasciitilde\\ang{26}N";
      case REGION_SOUTHHEMI: return "Southern hemisphere";
      case REGION_NORTHHEMI: return "Northern hemisphere";
      case REGION_EASTHEMI: return "Eastern hemisphere";
      case REGION_EASTHEMI26: return "Eastern hemisphere south of 26°";
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

// the behemoth blade adventure effect boosts max battle attacks by 5%, and
// raid attacks by 10%.
constexpr float BBLADE_MAX_BOOST = 1.05;
constexpr float BBLADE_RAID_BOOST = 1.1;

#endif
