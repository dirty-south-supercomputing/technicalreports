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

// table through which one can iterate over all attacks. i'd love to get
// rid of this--can we not build it dynamically when we actually need it?
// we don't list the Super Mega "Plus" attacks here.
const std::vector<const attack*> attacks = {
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
  &ATK_Gigaton_Hammer,
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
  &ATK_Beak_Blast,
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
  &ATK_Crush_Grip,
  &ATK_Darkest_Lariat,
  &ATK_Dark_Pulse,
  &ATK_Dazzling_Gleam,
  &ATK_Dig,
  &ATK_Disarming_Voice,
  &ATK_Discharge,
  &ATK_Dive,
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
  &ATK_Drum_Beating,
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
  &ATK_Mind_Blown,
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
  &ATK_Plasma_Fists,
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
  &ATK_Pyro_Ball,
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
  &ATK_Secret_Sword,
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
  &ATK_Snipe_Shot,
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
  &ATK_Techno_Blast_Electric,
  &ATK_Techno_Blast_Fire,
  &ATK_Techno_Blast_Ice,
  &ATK_Techno_Blast_Normal,
  &ATK_Techno_Blast_Water,
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

std::vector<const attack*>::const_iterator attacks_begin(void){
  return attacks.cbegin();
}

std::vector<const attack*>::const_iterator attacks_end(void){
  return attacks.cend();
}
