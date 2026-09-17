#include "pgotypes.h"
#include <cstdint>

static const uint32_t TColors[TYPECOUNT] = {
  0xA6B91A, 0x575063, 0x6F35FC, 0xF7D02C, 0xD685AD, 0xC22E28, 0xEE8130, 0x8D87DB, 0x5066A3,
  0x7AC74C, 0xD97A4B, 0x96D9D6, 0x9CA5AB, 0xA33EA1, 0xF95587, 0xB6A136, 0x5E91A4, 0x6390F0,
};

static const uint64_t LEVELREQS[MAXLEVEL] = {
  // 0 2.5 3 3.5 4 5 6 7 8 9
  0, 2500, 5500, 9000, 13000, 18000, 24000, 31000, 39000, 48000,
  58000, 70000, 84000, 100000, 118000, 139000, 163500, 191500, 223000, 258000,
  // 0.096 0.109 0.122 0.135 0.158
  300000, 349000, 405000, 468000, 538000, 621000, 717000, 826000, 948000, 1083000,
  // 0.181 0.204 0.227 0.25 0.29 0.33 0.37 0.41 0.45 0.52
  1241000, 1422000, 1626000, 1853000, 2103000, 2393000, 2723000, 3093000, 3503000, 3953000,
  // 0.59 0.66 0.73 0.8 0.9 1 1.1 1.2 1.3
  4473000, 5063000, 5723000, 6453000, 7253000, 8153000, 9153000, 10253000, 11453000, 12753000,
  // 1.44 1.58 1.72 1.86 2 2.2 2.4 2.6 2.8 3M
  14193000, 15773000, 17493000, 19353000, 21353000, 23553000, 25953000, 28553000, 31353000, 34353000,
  // 3.35 3.7 4.05 4.4 4.75 5.25 5.75 6.25 6.75 7.25M
  37703000, 41403000, 45453000, 49853000, 54603000, 59853000, 65603000, 71853000, 78603000, 85853000,
  // 8M 8.75M 9.5M 10.25M 11M 12M 13M 14M 15M 16M
  93853000, 102603000, 112103000, 122353000, 133353000, 145353000, 158353000, 172353000, 187353000, 203353000,
};

// return the amount of xp needed to reach level l from the previous level.
// 1 <= l <= MAXLEVEL
uint64_t pgo_xp_for_level(int l){
  if(l <= 0 || l > MAXLEVEL){
    throw std::domain_error("invalid level");
  }
  return LEVELREQS[l - 1];
}

// dynamax Max Attack names. dynamax attack type is matched to fast attack type
// (Hidden Power always becomes normal, aka Max Strike). max attacks are 250,
// 300, 350, 450 damage (level 4 achieved via dynamax cannon adventure effect).
const char* max_attack_name(pgo_types_e t){
  static const char* MaxAttackNames[TYPECOUNT] = {
    "Max Flutterby",
    "Max Darkness",
    "Max Wyrmwind",
    "Max Lightning",
    "Max Starfall",
    "Max Knuckle",
    "Max Flare",
    "Max Airstream",
    "Max Phantasm",
    "Max Overgrowth",
    "Max Quake",
    "Max Hailstorm",
    "Max Strike",
    "Max Ooze",
    "Max Mindstorm",
    "Max Rockfall",
    "Max Steelspike",
    "Max Geyser"
  };
  if(t < TYPESTART || t >= TYPECOUNT){
    throw std::exception();
  }
  return MaxAttackNames[t];
}

const char* tname_capitalized(pgo_types_e t){
  static const char* TNames[TYPECOUNT] = {
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
  if(t < TYPESTART || t >= TYPECOUNT){
    throw std::exception();
  }
  return TNames[t];
}
