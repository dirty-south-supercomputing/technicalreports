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
    0.778933, 0.7817900548, 0.784637, 0.7874736075,
    0.7903, 0.792803968, // 40--40.5
    0.79530001, 0.797800015, 0.8003, 0.802799995, // 41--42.5
    0.8053, 0.8078, 0.8103, 0.8128, // 43--44.5
    0.8153, 0.8178, 0.8203, 0.8228, // 45--46.5
    0.8253, 0.8278, 0.8303, 0.8328, // 47--48.5
    0.8353, 0.8378, // 49--49.5
    0.8403, 0.8428, // 50--50.5
    0.8453, // 51
    0.8478, // 51.5
    0.8503, // 52
  };
  if(halflevel <= 0
      || static_cast<unsigned>(halflevel) > sizeof(cpms) / sizeof(*cpms)
      || static_cast<unsigned>(halflevel) > MAX_HALFLEVEL){
    throw std::invalid_argument("invalid halflevel");
  }
  return cpms[halflevel - 1];
}
