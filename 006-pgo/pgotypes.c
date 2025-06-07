#include <math.h>
#include <stdio.h>

#define TYPESTART TYPE_BUG

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

#define MAX_HALFLEVEL 99

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

// currently only have trainer battle stats; need to add raid
typedef struct attack {
  const char *name;
  pgo_types_e type;
  unsigned powertrain;   // power in trainer battle context
  int energytrain;       // energy generated/consumed in trainer battle context
  unsigned turns;
} attack;

static const attack ATK_Acid = { "Acid", TYPE_POISON, 6, 8, 2, };
static const attack ATK_Air_Slash = { "Air Slash", TYPE_FLYING, 9, 9, 3, };
static const attack ATK_Astonish = { "Astonish", TYPE_GHOST, 12, 10, 3, };
static const attack ATK_Bite = { "Bite", TYPE_DARK, 4, 2, 1, };
static const attack ATK_Bubble = { "Bubble", TYPE_WATER, 8, 11, 3, };
static const attack ATK_Bug_Bite = { "Bug Bite", TYPE_BUG, 3, 3, 1, };
static const attack ATK_Bullet_Punch = { "Bullet Punch", TYPE_STEEL, 6, 7, 2, };
static const attack ATK_Bullet_Seed = { "Bullet Seed", TYPE_GRASS, 5, 13, 3, };
static const attack ATK_Charge_Beam = { "Charge Beam", TYPE_ELECTRIC, 5, 11, 3, };
static const attack ATK_Charm = { "Charm", TYPE_FAIRY, 15, 6, 3, };
static const attack ATK_Confusion = { "Confusion", TYPE_PSYCHIC, 16, 12, 4, };
static const attack ATK_Counter = { "Counter", TYPE_FIGHTING, 8, 6, 2, };
static const attack ATK_Cut = { "Cut", TYPE_NORMAL, 3, 2, 1, };
static const attack ATK_Double_Kick = { "Double Kick", TYPE_FIGHTING, 8, 12, 3, };
static const attack ATK_Dragon_Breath = { "Dragon Breath", TYPE_DRAGON, 4, 3, 1, };
static const attack ATK_Dragon_Tail = { "Dragon Tail", TYPE_DRAGON, 13, 9, 3, };
static const attack ATK_Ember = { "Ember", TYPE_FIRE, 7, 6, 2, };
static const attack ATK_Extrasensory = { "Extrasensory", TYPE_PSYCHIC, 8, 10, 3, };
static const attack ATK_Feint_Attack = { "Feint Attack", TYPE_DARK, 6, 6, 2, };
static const attack ATK_Fire_Fang = { "Fire Fang", TYPE_FIRE, 8, 6, 2, };
static const attack ATK_Fairy_Wind = { "Fairy Wind", TYPE_FAIRY, 4, 9, 2, };
static const attack ATK_Fire_Spin = { "Fire Spin", TYPE_FIRE, 11, 10, 3, };
static const attack ATK_Force_Palm = { "Force Palm", TYPE_FIGHTING, 13, 10, 3, };
static const attack ATK_Frost_Breath = { "Frost Breath", TYPE_ICE, 7, 5, 2, };
static const attack ATK_Fury_Cutter = { "Fury Cutter", TYPE_BUG, 2, 4, 1, };
static const attack ATK_Geomancy = { "Geomancy", TYPE_FAIRY, 4, 13, 3, };
static const attack ATK_Gust = { "Gust", TYPE_FLYING, 16, 12, 4, };
static const attack ATK_Hex = { "Hex", TYPE_GHOST, 7, 13, 3, };
static const attack ATK_Hidden_Power = { "Hidden Power", TYPECOUNT, 9, 8, 3, };
static const attack ATK_Ice_Fang = { "Ice Fang", TYPE_ICE, 8, 6, 2, };
static const attack ATK_Ice_Shard = { "Ice Shard", TYPE_ICE, 9, 10, 3, };
static const attack ATK_Incinerate = { "Incinerate", TYPE_FIRE, 20, 20, 5, };
static const attack ATK_Infestation = { "Infestation", TYPE_BUG, 6, 12, 3, };
static const attack ATK_Iron_Tail = { "Iron Tail", TYPE_STEEL, 10, 7, 3, };
static const attack ATK_Karate_Chop = { "Karate Chop", TYPE_FIGHTING, 5, 9, 2, };
static const attack ATK_Leafage = { "Leafage", TYPE_GRASS, 6, 7, 2, };
static const attack ATK_Lick = { "Lick", TYPE_GHOST, 3, 3, 1, };
static const attack ATK_Lock_On = { "Lock On", TYPE_NORMAL, 1, 5, 1, };
static const attack ATK_Low_Kick = { "Low Kick", TYPE_FIGHTING, 4, 5, 2, };
static const attack ATK_Magical_Leaf = { "Magical Leaf", TYPE_GRASS, 10, 10, 3, };
static const attack ATK_Metal_Claw = { "Metal Claw", TYPE_STEEL, 5, 7, 2, };
static const attack ATK_Metal_Sound = { "Metal Sound", TYPE_STEEL, 3, 8, 2, };
static const attack ATK_Mud_Shot = { "Mud Shot", TYPE_GROUND, 4, 8, 2, };
static const attack ATK_Mud_Slap = { "Mud Slap", TYPE_GROUND, 12, 10, 3, };
static const attack ATK_Peck = { "Peck", TYPE_FLYING, 6, 5, 2, };
static const attack ATK_Poison_Jab = { "Poison Jab", TYPE_POISON, 7, 7, 2, };
static const attack ATK_Poison_Sting = { "Poison Sting", TYPE_POISON, 4, 9, 2, };
static const attack ATK_Pound = { "Pound", TYPE_NORMAL, 4, 4, 2, };
static const attack ATK_Powder_Snow = { "Powder Snow", TYPE_ICE, 5, 8, 2, };
static const attack ATK_Present = { "Present", TYPE_NORMAL, 3, 12, 3, };
static const attack ATK_Psycho_Cut = { "Psycho Cut", TYPE_PSYCHIC, 3, 9, 2, };
static const attack ATK_Psywave = { "Psywave", TYPE_PSYCHIC, 3, 4, 1, };
static const attack ATK_Quick_Attack = { "Quick Attack", TYPE_NORMAL, 5, 8, 2, };
static const attack ATK_Razor_Leaf = { "Razor Leaf", TYPE_GRASS, 9, 4, 2, };
static const attack ATK_Rock_Smash = { "Rock Smash", TYPE_FIGHTING, 9, 7, 3, };
static const attack ATK_Rock_Throw = { "Rock Throw", TYPE_ROCK, 8, 5, 2, };
static const attack ATK_Rollout = { "Rollout", TYPE_ROCK, 7, 13, 3, };
static const attack ATK_Sand_Attack = { "Sand Attack", TYPE_GROUND, 2, 4, 1, };
static const attack ATK_Scratch = { "Scratch", TYPE_NORMAL, 4, 2, 1, };
static const attack ATK_Shadow_Claw = { "Shadow Claw", TYPE_GHOST, 6, 8, 2, };
static const attack ATK_Smack_Down = { "Smack Down", TYPE_ROCK, 11, 8, 3, };
static const attack ATK_Snarl = { "Snarl", TYPE_DARK, 5, 13, 3, };
static const attack ATK_Spark = { "Spark", TYPE_ELECTRIC, 5, 7, 2, };
static const attack ATK_Splash = { "Splash", TYPE_WATER, 0, 12, 4, };
static const attack ATK_Steel_Wing = { "Steel Wing", TYPE_STEEL, 7, 6, 2, };
static const attack ATK_Struggle_Bug = { "Struggle Bug", TYPE_BUG, 9, 8, 3, };
static const attack ATK_Sucker_Punch = { "Sucker Punch", TYPE_DARK, 8, 7, 2, };
static const attack ATK_Tackle = { "Tackle", TYPE_NORMAL, 3, 3, 1, };
static const attack ATK_Take_Down = { "Take Down", TYPE_NORMAL, 5, 8, 3, };
static const attack ATK_Thunder_Fang = { "Thunder Fang", TYPE_ELECTRIC, 8, 6, 2, };
static const attack ATK_Thunder_Shock = { "Thunder Shock", TYPE_ELECTRIC, 4, 9, 2, };
static const attack ATK_Vine_Whip = { "Vine Whip", TYPE_GRASS, 5, 8, 2, };
static const attack ATK_Volt_Switch = { "Volt Switch", TYPE_ELECTRIC, 12, 16, 4, };
static const attack ATK_Water_Gun = { "Water Gun", TYPE_WATER, 3, 3, 1, };
static const attack ATK_Water_Gun_Fast = { "Water Gun Fast", TYPE_WATER, 6, 4, 2, };
static const attack ATK_Waterfall = { "Waterfall", TYPE_WATER, 12, 8, 3, };
static const attack ATK_Water_Shuriken = { "Water Shuriken", TYPE_WATER, 6, 14, 3, };
static const attack ATK_Wing_Attack = { "Wing Attack", TYPE_FLYING, 5, 7, 2, };
static const attack ATK_Yawn = { "Yawn", TYPE_NORMAL, 0, 12, 4, };
static const attack ATK_Zen_Headbutt = { "Zen Headbutt", TYPE_PSYCHIC, 8, 6, 3, };
static const attack ATK_Acid_Spray = { "Acid Spray", TYPE_POISON, 20, -45, 0 };
static const attack ATK_Acrobatics = { "Acrobatics", TYPE_FLYING, 110, -55, 0 };
static const attack ATK_Aerial_Ace = { "Aerial Ace", TYPE_FLYING, 55, -40, 0 };
static const attack ATK_Aeroblast = { "Aeroblast", TYPE_FLYING, 170, -75, 0 };
static const attack ATK_Air_Cutter = { "Air Cutter", TYPE_FLYING, 45, -35, 0 };
static const attack ATK_Ancient_Power = { "Ancient Power", TYPE_ROCK, 60, -45, 0 };
static const attack ATK_Aqua_Jet = { "Aqua Jet", TYPE_WATER, 70, -40, 0 };
static const attack ATK_Aqua_Tail = { "Aqua Tail", TYPE_WATER, 55, -35, 0 };
static const attack ATK_Aura_Sphere = { "Aura Sphere", TYPE_FIGHTING, 100, -55, 0 };
static const attack ATK_Aura_Wheel_Electric = { "Aura Wheel (Electric)", TYPE_ELECTRIC, 100, -45, 0 };
static const attack ATK_Aura_Wheel_Dark = { "Aura Wheel (Dark)", TYPE_DARK, 100, -45, 0 };
static const attack ATK_Aurora_Beam = { "Aurora Beam", TYPE_ICE, 80, -60, 0 };
static const attack ATK_Avalanche = { "Avalanche", TYPE_ICE, 90, -45, 0 };
static const attack ATK_Behemoth_Bash = { "Behemoth Bash", TYPE_STEEL, 80, -45, 0 };
static const attack ATK_Behemoth_Blade = { "Behemoth Blade", TYPE_STEEL, 100, -55, 0 };
static const attack ATK_Blast_Burn = { "Blast Burn", TYPE_FIRE, 110, -50, 0 };
static const attack ATK_Bleakwind_Storm = { "Bleakwind Storm", TYPE_FLYING, 60, -45, 0 };
static const attack ATK_Blizzard = { "Blizzard", TYPE_ICE, 140, -75, 0 };
static const attack ATK_Body_Slam = { "Body Slam", TYPE_NORMAL, 50, -35, 0 };
static const attack ATK_Boomburst = { "Boomburst", TYPE_NORMAL, 150, -70, 0 };
static const attack ATK_Bone_Club = { "Bone Club", TYPE_GROUND, 55, -35, 0 };
static const attack ATK_Brave_Bird = { "Brave Bird", TYPE_FLYING, 130, -55, 0 };
static const attack ATK_Breaking_Swipe = { "Breaking Swipe", TYPE_DRAGON, 50, -35, 0 };
static const attack ATK_Brick_Break = { "Brick Break", TYPE_FIGHTING, 40, -40, 0 };
static const attack ATK_Brutal_Swing = { "Brutal Swing", TYPE_DARK, 55, -35, 0 };
static const attack ATK_Brine = { "Brine", TYPE_WATER, 60, -50, 0 };
static const attack ATK_Bubble_Beam = { "Bubble Beam", TYPE_WATER, 25, -40, 0 };
static const attack ATK_Bug_Buzz = { "Bug Buzz", TYPE_BUG, 100, -60, 0 };
static const attack ATK_Bulldoze = { "Bulldoze", TYPE_GROUND, 45, -45, 0 };
static const attack ATK_Close_Combat = { "Close Combat", TYPE_FIGHTING, 100, -45, 0 };
static const attack ATK_Crabhammer = { "Crabhammer", TYPE_WATER, 85, -50, 0 };
static const attack ATK_Cross_Chop = { "Cross Chop", TYPE_FIGHTING, 55, -35, 0 };
static const attack ATK_Cross_Poison = { "Cross Poison", TYPE_POISON, 50, -35, 0 };
static const attack ATK_Crunch = { "Crunch", TYPE_DARK, 70, -45, 0 };
static const attack ATK_Darkest_Lariat = { "Darkest Lariat", TYPE_DARK, 120, -60, 0 };
static const attack ATK_Dark_Pulse = { "Dark Pulse", TYPE_DARK, 80, -50, 0 };
static const attack ATK_Dazzling_Gleam = { "Dazzling Gleam", TYPE_FAIRY, 90, -55, 0 };
static const attack ATK_Dig = { "Dig", TYPE_GROUND, 70, -50, 0 };
static const attack ATK_Disarming_Voice = { "Disarming Voice", TYPE_FAIRY, 70, -45, 0 };
static const attack ATK_Discharge = { "Discharge", TYPE_ELECTRIC, 55, -40, 0 };
static const attack ATK_Doom_Desire = { "Doom Desire", TYPE_STEEL, 75, -40, 0 };
static const attack ATK_Double_Iron_Bash = { "Double Iron Bash", TYPE_STEEL, 50, -35, 0 };
static const attack ATK_Draco_Meteor = { "Draco Meteor", TYPE_DRAGON, 150, -65, 0 };
static const attack ATK_Dragon_Ascent = { "Dragon Ascent", TYPE_FLYING, 150, -70, 0 };
static const attack ATK_Dragon_Claw = { "Dragon Claw", TYPE_DRAGON, 50, -35, 0 };
static const attack ATK_Dragon_Pulse = { "Dragon Pulse", TYPE_DRAGON, 90, -60, 0 };
static const attack ATK_Drain_Punch = { "Drain Punch", TYPE_FIGHTING, 20, -40, 0 };
static const attack ATK_Draining_Kiss = { "Draining Kiss", TYPE_FAIRY, 60, -55, 0 };
static const attack ATK_Drill_Peck = { "Drill Peck", TYPE_FLYING, 65, -40, 0 };
static const attack ATK_Drill_Run = { "Drill Run", TYPE_GROUND, 80, -45, 0 };
static const attack ATK_Dynamic_Punch = { "Dynamic Punch", TYPE_FIGHTING, 90, -45, 0 };
static const attack ATK_Earthquake = { "Earthquake", TYPE_GROUND, 110, -65, 0 };
static const attack ATK_Earth_Power = { "Earth Power", TYPE_GROUND, 90, -55, 0 };
static const attack ATK_Energy_Ball = { "Energy Ball", TYPE_GRASS, 90, -55, 0 };
static const attack ATK_Fell_Stinger = { "Fell Stinger", TYPE_BUG, 20, -35, 0 };
static const attack ATK_Fire_Blast = { "Fire Blast", TYPE_FIRE, 140, -80, 0 };
static const attack ATK_Fire_Punch = { "Fire Punch", TYPE_FIRE, 60, -40, 0 };
static const attack ATK_Blaze_Kick = { "Blaze Kick", TYPE_FIRE, 60, -40, 0 };
static const attack ATK_Feather_Dance = { "Feather Dance", TYPE_FLYING, 35, -50, 0 };
static const attack ATK_Flame_Burst = { "Flame Burst", TYPE_FIRE, 70, -55, 0 };
static const attack ATK_Flame_Charge = { "Flame Charge", TYPE_FIRE, 65, -50, 0 };
static const attack ATK_Flame_Wheel = { "Flame Wheel", TYPE_FIRE, 60, -55, 0 };
static const attack ATK_Flamethrower = { "Flamethrower", TYPE_FIRE, 90, -55, 0 };
static const attack ATK_Flash_Cannon = { "Flash Cannon", TYPE_STEEL, 110, -70, 0 };
static const attack ATK_Flower_Trick = { "Flower Trick", TYPE_GRASS, 30, -35, 0 };
static const attack ATK_Fly = { "Fly", TYPE_FLYING, 80, -45, 0 };
static const attack ATK_Flying_Press = { "Flying Press", TYPE_FIGHTING, 90, -40, 0 };
static const attack ATK_Focus_Blast = { "Focus Blast", TYPE_FIGHTING, 150, -75, 0 };
static const attack ATK_Foul_Play = { "Foul Play", TYPE_DARK, 60, -40, 0 };
static const attack ATK_Freeze_Shock = { "Freeze Shock", TYPE_ICE, 120, -60, 0 };
static const attack ATK_Frenzy_Plant = { "Frenzy Plant", TYPE_GRASS, 100, -45, 0 };
static const attack ATK_Frustration = { "Frustration", TYPE_NORMAL, 10, -70, 0 };
static const attack ATK_Fusion_Bolt = { "Fusion Bolt", TYPE_ELECTRIC, 90, -45, 0 };
static const attack ATK_Fusion_Flare = { "Fusion Flare", TYPE_FIRE, 90, -45, 0 };
static const attack ATK_Future_Sight = { "Future Sight", TYPE_PSYCHIC, 110, -65, 0 };
static const attack ATK_Giga_Drain = { "Giga Drain", TYPE_GRASS, 50, -80, 0 };
static const attack ATK_Giga_Impact = { "Giga Impact", TYPE_NORMAL, 150, -80, 0 };
static const attack ATK_Glaciate = { "Glaciate", TYPE_ICE, 60, -40, 0 };
static const attack ATK_Grass_Knot = { "Grass Knot", TYPE_GRASS, 90, -50, 0 };
static const attack ATK_Gunk_Shot = { "Gunk Shot", TYPE_POISON, 130, -75, 0 };
static const attack ATK_Gyro_Ball = { "Gyro Ball", TYPE_STEEL, 80, -60, 0 };
static const attack ATK_Heart_Stamp = { "Heart Stamp", TYPE_PSYCHIC, 40, -40, 0 };
static const attack ATK_Heat_Wave = { "Heat Wave", TYPE_FIRE, 95, -75, 0 };
static const attack ATK_Heavy_Slam = { "Heavy Slam", TYPE_STEEL, 70, -50, 0 };
static const attack ATK_High_Horsepower = { "High Horsepower", TYPE_GROUND, 100, -60, 0 };
static const attack ATK_High_Jump_Kick = { "High Jump Kick", TYPE_FIGHTING, 110, -55, 0 };
static const attack ATK_Horn_Attack = { "Horn Attack", TYPE_NORMAL, 40, -35, 0 };
static const attack ATK_Hurricane = { "Hurricane", TYPE_FLYING, 110, -65, 0 };
static const attack ATK_Hydro_Cannon = { "Hydro Cannon", TYPE_WATER, 80, -40, 0 };
static const attack ATK_Hydro_Pump = { "Hydro Pump", TYPE_WATER, 130, -75, 0 };
static const attack ATK_Hydro_Pump_Blastoise = { "Hydro Pump Blastoise", TYPE_WATER, 90, -80, 0 };
static const attack ATK_Hyper_Beam = { "Hyper Beam", TYPE_NORMAL, 150, -80, 0 };
static const attack ATK_Hyper_Fang = { "Hyper Fang", TYPE_NORMAL, 80, -50, 0 };
static const attack ATK_Ice_Beam = { "Ice Beam", TYPE_ICE, 90, -55, 0 };
static const attack ATK_Ice_Burn = { "Ice Burn", TYPE_ICE, 120, -60, 0 };
static const attack ATK_Ice_Punch = { "Ice Punch", TYPE_ICE, 60, -40, 0 };
static const attack ATK_Icicle_Spear = { "Icicle Spear", TYPE_ICE, 65, -40, 0 };
static const attack ATK_Icy_Wind = { "Icy Wind", TYPE_ICE, 60, -45, 0 };
static const attack ATK_Iron_Head = { "Iron Head", TYPE_STEEL, 70, -50, 0 };
static const attack ATK_Last_Resort = { "Last Resort", TYPE_NORMAL, 90, -55, 0 };
static const attack ATK_Leaf_Blade = { "Leaf Blade", TYPE_GRASS, 70, -35, 0 };
static const attack ATK_Leaf_Tornado = { "Leaf Tornado", TYPE_GRASS, 45, -40, 0 };
static const attack ATK_Leaf_Storm = { "Leaf Storm", TYPE_GRASS, 130, -55, 0 };
static const attack ATK_Liquidation = { "Liquidation", TYPE_WATER, 70, -45, 0 };
static const attack ATK_Low_Sweep = { "Low Sweep", TYPE_FIGHTING, 40, -40, 0 };
static const attack ATK_Lunge = { "Lunge", TYPE_BUG, 60, -45, 0 };
static const attack ATK_Luster_Purge = { "Luster Purge", TYPE_PSYCHIC, 120, -60, 0 };
static const attack ATK_Magma_Storm = { "Magma Storm", TYPE_FIRE, 65, -40, 0 };
static const attack ATK_Magnet_Bomb = { "Magnet Bomb", TYPE_STEEL, 70, -45, 0 };
static const attack ATK_Mega_Drain = { "Mega Drain", TYPE_GRASS, 25, -55, 0 };
static const attack ATK_Megahorn = { "Megahorn", TYPE_BUG, 110, -55, 0 };
static const attack ATK_Meteor_Beam = { "Meteor Beam", TYPE_ROCK, 120, -60, 0 };
static const attack ATK_Meteor_Mash = { "Meteor Mash", TYPE_STEEL, 100, -50, 0 };
static const attack ATK_Mirror_Coat = { "Mirror Coat", TYPE_PSYCHIC, 60, -55, 0 };
static const attack ATK_Mirror_Shot = { "Mirror Shot", TYPE_STEEL, 35, -35, 0 };
static const attack ATK_Mist_Ball = { "Mist Ball", TYPE_PSYCHIC, 120, -60, 0 };
static const attack ATK_Moonblast = { "Moonblast", TYPE_FAIRY, 110, -60, 0 };
static const attack ATK_Moongeist_Beam = { "Moongeist Beam", TYPE_GHOST, 135, -65, 0 };
static const attack ATK_Mud_Bomb = { "Mud Bomb", TYPE_GROUND, 60, -40, 0 };
static const attack ATK_Muddy_Water = { "Muddy Water", TYPE_WATER, 35, -35, 0 };
static const attack ATK_Mystical_Fire = { "Mystical Fire", TYPE_FIRE, 60, -45, 0 };
static const attack ATK_Natures_Madness = { "Nature's Madness", TYPE_FAIRY, 80, -50, 0 };
static const attack ATK_Night_Shade = { "Night Shade", TYPE_GHOST, 70, -45, 0 };
static const attack ATK_Night_Slash = { "Night Slash", TYPE_DARK, 50, -35, 0 };
static const attack ATK_Oblivion_Wing = { "Oblivion Wing", TYPE_FLYING, 85, -50, 0 };
static const attack ATK_Obstruct = { "Obstruct", TYPE_DARK, 15, -40, 0 };
static const attack ATK_Octazooka = { "Octazooka", TYPE_WATER, 50, -50, 0 };
static const attack ATK_Ominous_Wind = { "Ominous Wind", TYPE_GHOST, 45, -45, 0 };
static const attack ATK_Origin_Pulse = { "Origin Pulse", TYPE_WATER, 130, -60, 0 };
static const attack ATK_Outrage = { "Outrage", TYPE_DRAGON, 110, -60, 0 };
static const attack ATK_Overheat = { "Overheat", TYPE_FIRE, 130, -55, 0 };
static const attack ATK_Parabolic_Charge = { "Parabolic Charge", TYPE_ELECTRIC, 70, -50, 0 };
static const attack ATK_Payback = { "Payback", TYPE_DARK, 110, -60, 0 };
static const attack ATK_Petal_Blizzard = { "Petal Blizzard", TYPE_GRASS, 110, -65, 0 };
static const attack ATK_Play_Rough = { "Play Rough", TYPE_FAIRY, 90, -60, 0 };
static const attack ATK_Poison_Fang = { "Poison Fang", TYPE_POISON, 45, -40, 0 };
static const attack ATK_Poltergeist = { "Poltergeist", TYPE_GHOST, 150, -75, 0 };
static const attack ATK_Power_Gem = { "Power Gem", TYPE_ROCK, 85, -50, 0 };
static const attack ATK_Power_Up_Punch = { "Power-Up Punch", TYPE_FIGHTING, 20, -35, 0 };
static const attack ATK_Power_Whip = { "Power Whip", TYPE_GRASS, 90, -50, 0 };
static const attack ATK_Precipice_Blades = { "Precipice Blades", TYPE_GROUND, 130, -60, 0 };
static const attack ATK_Psybeam = { "Psybeam", TYPE_PSYCHIC, 70, -60, 0 };
static const attack ATK_Psychic = { "Psychic", TYPE_PSYCHIC, 75, -55, 0 };
static const attack ATK_Psychic_Fangs = { "Psychic Fangs", TYPE_PSYCHIC, 40, -35, 0 };
static const attack ATK_Psycho_Boost = { "Psycho Boost", TYPE_PSYCHIC, 70, -35, 0 };
static const attack ATK_Psyshock = { "Psyshock", TYPE_PSYCHIC, 70, -40, 0 };
static const attack ATK_Psystrike = { "Psystrike", TYPE_PSYCHIC, 90, -45, 0 };
static const attack ATK_Rage_Fist = { "Rage Fist", TYPE_GHOST, 50, -35, 0 };
static const attack ATK_Razor_Shell = { "Razor Shell", TYPE_WATER, 35, -35, 0 };
static const attack ATK_Rest = { "Rest", TYPE_NORMAL, 50, -35, 0 };
static const attack ATK_Return = { "Return", TYPE_NORMAL, 130, -70, 0 };
static const attack ATK_Roar_of_Time = { "Roar of Time", TYPE_DRAGON, 150, -65, 0 };
static const attack ATK_Rock_Blast = { "Rock Blast", TYPE_ROCK, 50, -40, 0 };
static const attack ATK_Rock_Slide = { "Rock Slide", TYPE_ROCK, 65, -45, 0 };
static const attack ATK_Rock_Tomb = { "Rock Tomb", TYPE_ROCK, 80, -50, 0 };
static const attack ATK_Rock_Wrecker = { "Rock Wrecker", TYPE_ROCK, 110, -50, 0 };
static const attack ATK_Sacred_Fire = { "Sacred Fire", TYPE_FIRE, 130, -65, 0 };
static const attack ATK_Sacred_Sword = { "Sacred Sword", TYPE_FIGHTING, 60, -35, 0 };
static const attack ATK_Sand_Tomb = { "Sand Tomb", TYPE_GROUND, 25, -40, 0 };
static const attack ATK_Sandsear_Storm = { "Sandsear Storm", TYPE_GROUND, 60, -45, 0 };
static const attack ATK_Scald = { "Scald", TYPE_WATER, 85, -50, 0 };
static const attack ATK_Scorching_Sands = { "Scorching Sands", TYPE_GROUND, 80, -50, 0 };
static const attack ATK_Seed_Bomb = { "Seed Bomb", TYPE_GRASS, 65, -45, 0 };
static const attack ATK_Seed_Flare = { "Seed Flare", TYPE_GRASS, 130, -75, 0 };
static const attack ATK_Shadow_Ball = { "Shadow Ball", TYPE_GHOST, 100, -55, 0 };
static const attack ATK_Shadow_Bone = { "Shadow Bone", TYPE_GHOST, 80, -45, 0 };
static const attack ATK_Shadow_Force = { "Shadow Force", TYPE_GHOST, 120, -90, 0 };
static const attack ATK_Shadow_Punch = { "Shadow Punch", TYPE_GHOST, 55, -35, 0 };
static const attack ATK_Shadow_Sneak = { "Shadow Sneak", TYPE_GHOST, 50, -45, 0 };
static const attack ATK_Signal_Beam = { "Signal Beam", TYPE_BUG, 75, -55, 0 };
static const attack ATK_Silver_Wind = { "Silver Wind", TYPE_BUG, 60, -45, 0 };
static const attack ATK_Skull_Bash = { "Skull Bash", TYPE_NORMAL, 130, -75, 0 };
static const attack ATK_Sky_Attack = { "Sky Attack", TYPE_FLYING, 85, -55, 0 };
static const attack ATK_Sludge = { "Sludge", TYPE_POISON, 70, -40, 0 };
static const attack ATK_Sludge_Bomb = { "Sludge Bomb", TYPE_POISON, 80, -50, 0 };
static const attack ATK_Sludge_Wave = { "Sludge Wave", TYPE_POISON, 110, -65, 0 };
static const attack ATK_Solar_Beam = { "Solar Beam", TYPE_GRASS, 150, -80, 0 };
static const attack ATK_Spacial_Rend = { "Spacial Rend", TYPE_DRAGON, 95, -50, 0 };
static const attack ATK_Sparkling_Aria = { "Sparkling Aria", TYPE_WATER, 80, -45, 0 };
static const attack ATK_Spirit_Shackle = { "Spirit Shackle", TYPE_GHOST, 50, -40, 0 };
static const attack ATK_Stomp = { "Stomp", TYPE_NORMAL, 55, -40, 0 };
static const attack ATK_Stone_Edge = { "Stone Edge", TYPE_ROCK, 100, -55, 0 };
static const attack ATK_Struggle = { "Struggle", TYPE_NORMAL, 35, -100, 0 };
static const attack ATK_Submission = { "Submission", TYPE_FIGHTING, 60, -50, 0 };
static const attack ATK_Sunsteel_Strike = { "Sunsteel Strike", TYPE_STEEL, 135, -65, 0 };
static const attack ATK_Superpower = { "Superpower", TYPE_FIGHTING, 85, -40, 0 };
static const attack ATK_Surf = { "Surf", TYPE_WATER, 75, -45, 0 };
static const attack ATK_Swift = { "Swift", TYPE_NORMAL, 55, -35, 0 };
static const attack ATK_Synchronoise = { "Synchronoise", TYPE_PSYCHIC, 80, -50, 0 };
static const attack ATK_Techno_Blast_Normal = { "Techno Blast (Normal)", TYPE_NORMAL, 120, -55, 0 };
static const attack ATK_Techno_Blast_Burn = { "Techno Blast (Burn)", TYPE_FIRE, 120, -55, 0 };
static const attack ATK_Techno_Blast_Chill = { "Techno Blast (Chill)", TYPE_ICE, 120, -55, 0 };
static const attack ATK_Techno_Blast_Douse = { "Techno Blast (Douse)", TYPE_WATER, 120, -55, 0 };
static const attack ATK_Techno_Blast_Shock = { "Techno Blast (Shock)", TYPE_ELECTRIC, 120, -55, 0 };
static const attack ATK_Thunder = { "Thunder", TYPE_ELECTRIC, 100, -60, 0 };
static const attack ATK_Thunder_Punch = { "Thunder Punch", TYPE_ELECTRIC, 60, -40, 0 };
static const attack ATK_Thunderbolt = { "Thunderbolt", TYPE_ELECTRIC, 90, -55, 0 };
static const attack ATK_Torch_Song = { "Torch Song", TYPE_FIRE, 70, -45, 0 };
static const attack ATK_Trailblaze = { "Trailblaze", TYPE_GRASS, 65, -45, 0 };
static const attack ATK_Tri_Attack = { "Tri Attack", TYPE_NORMAL, 65, -50, 0 };
static const attack ATK_Triple_Axel = { "Triple Axel", TYPE_ICE, 60, -45, 0 };
static const attack ATK_Twister = { "Twister", TYPE_DRAGON, 45, -45, 0 };
static const attack ATK_Upper_Hand = { "Upper Hand", TYPE_FIGHTING, 70, -40, 0 };
static const attack ATK_V_Create = { "V-Create", TYPE_FIRE, 95, -40, 0 };
static const attack ATK_Vise_Grip = { "Vise Grip", TYPE_NORMAL, 40, -40, 0 };
static const attack ATK_Volt_Tackle = { "Volt Tackle", TYPE_ELECTRIC, 90, -50, 0 };
static const attack ATK_Water_Pulse = { "Water Pulse", TYPE_WATER, 80, -55, 0 };
static const attack ATK_Weather_Ball_Fire = { "Weather Ball (Fire)", TYPE_FIRE, 55, -35, 0 };
static const attack ATK_Weather_Ball_Ice = { "Weather Ball (Ice)", TYPE_ICE, 55, -35, 0 };
static const attack ATK_Weather_Ball_Rock = { "Weather Ball (Rock)", TYPE_ROCK, 55, -35, 0 };
static const attack ATK_Weather_Ball_Normal = { "Weather Ball (Normal)", TYPE_NORMAL, 55, -35, 0 };
static const attack ATK_Weather_Ball_Water = { "Weather Ball (Water)", TYPE_WATER, 55, -35, 0 };
static const attack ATK_Wildbolt_Storm = { "Wildbolt Storm", TYPE_ELECTRIC, 60, -45, 0 };
static const attack ATK_Wild_Charge = { "Wild Charge", TYPE_ELECTRIC, 100, -45, 0 };
static const attack ATK_Wrap = { "Wrap", TYPE_NORMAL, 60, -45, 0 };
static const attack ATK_Wrap_Green = { "Wrap Green", TYPE_NORMAL, 25, -45, 0 };
static const attack ATK_Wrap_Pink = { "Wrap Pink", TYPE_NORMAL, 25, -45, 0 };
static const attack ATK_XScissor = { "X-Scissor", TYPE_BUG, 65, -40, 0 };
static const attack ATK_Zap_Cannon = { "Zap Cannon", TYPE_ELECTRIC, 150, -80, 0 };

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
  &ATK_Vine_Whip,
  &ATK_Volt_Switch,
  &ATK_Water_Gun,
  &ATK_Water_Gun_Fast,
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
  &ATK_Aqua_Tail,
  &ATK_Aura_Sphere,
  &ATK_Aura_Wheel_Electric,
  &ATK_Aura_Wheel_Dark,
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
  &ATK_Dragon_Pulse,
  &ATK_Drain_Punch,
  &ATK_Draining_Kiss,
  &ATK_Drill_Peck,
  &ATK_Drill_Run,
  &ATK_Dynamic_Punch,
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
  &ATK_Giga_Drain,
  &ATK_Giga_Impact,
  &ATK_Glaciate,
  &ATK_Grass_Knot,
  &ATK_Gunk_Shot,
  &ATK_Gyro_Ball,
  &ATK_Heart_Stamp,
  &ATK_Heat_Wave,
  &ATK_Heavy_Slam,
  &ATK_High_Horsepower,
  &ATK_High_Jump_Kick,
  &ATK_Horn_Attack,
  &ATK_Hurricane,
  &ATK_Hydro_Cannon,
  &ATK_Hydro_Pump,
  &ATK_Hydro_Pump_Blastoise,
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
  &ATK_Mega_Drain,
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
  &ATK_Rest,
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
  &ATK_Techno_Blast_Normal,
  &ATK_Techno_Blast_Burn,
  &ATK_Techno_Blast_Chill,
  &ATK_Techno_Blast_Douse,
  &ATK_Techno_Blast_Shock,
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
  &ATK_Wrap_Green,
  &ATK_Wrap_Pink,
  &ATK_XScissor,
  &ATK_Zap_Cannon,
};

static const attack* BULBASAUR_ATTACKS[] = {
  &ATK_Vine_Whip,
  &ATK_Tackle,
  &ATK_Seed_Bomb,
  &ATK_Sludge_Bomb,
  &ATK_Power_Whip,
  NULL
};

static const attack* IVYSAUR_ATTACKS[] = {
  &ATK_Vine_Whip,
  &ATK_Razor_Leaf,
  &ATK_Sludge_Bomb,
  &ATK_Solar_Beam,
  &ATK_Power_Whip,
  NULL
};

static const attack* VENUSAUR_ATTACKS[] = {
  &ATK_Vine_Whip,
  &ATK_Razor_Leaf,
  &ATK_Petal_Blizzard,
  &ATK_Sludge_Bomb,
  &ATK_Solar_Beam,
  &ATK_Frenzy_Plant,
  NULL
};

static const attack* CHARMANDER_ATTACKS[] = {
  &ATK_Ember,
  &ATK_Scratch,
  &ATK_Flamethrower,
  &ATK_Flame_Charge,
  &ATK_Flame_Burst,
  NULL
};

static const attack* CHARMELEON_ATTACKS[] = {
  &ATK_Ember,
  &ATK_Scratch,
  &ATK_Fire_Fang,
  &ATK_Flamethrower,
  &ATK_Flame_Burst,
  &ATK_Fire_Punch,
  NULL
};

static const attack* CHARIZARD_ATTACKS[] = {
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

typedef struct species {
  unsigned idx; // pokedex index, not unique
  const char* name;
  pgo_types_e t1, t2;
  unsigned atk;
  unsigned def;
  unsigned sta;
  const char *from;    // from what does it evolve? NULL for nothing
  const attack** attacks;  // array of attack indices this form can learn
} species;

static const species sdex[] = {
  {    1, "Bulbasaur", TYPE_GRASS, TYPE_POISON, 118, 111, 128, NULL, BULBASAUR_ATTACKS, },
  {    2, "Ivysaur", TYPE_GRASS, TYPE_POISON, 151, 143, 155, "Bulbasaur", IVYSAUR_ATTACKS, },
  {    3, "Venusaur", TYPE_GRASS, TYPE_POISON, 198, 189, 190, "Ivysaur", VENUSAUR_ATTACKS, },
  {    3, "Mega Venusaur", TYPE_GRASS, TYPE_POISON, 241, 246, 190, "Venusaur", VENUSAUR_ATTACKS, },
  {    4, "Charmander", TYPE_FIRE, TYPECOUNT, 116, 93, 118, NULL, CHARMANDER_ATTACKS, },
  {    5, "Charmeleon", TYPE_FIRE, TYPECOUNT, 158, 126, 151, "Charmander", CHARMELEON_ATTACKS, },
  {    6, "Charizard", TYPE_FIRE, TYPE_FLYING, 223, 173, 186, "Charmeleon", CHARIZARD_ATTACKS, },
  {    6, "Mega Charizard X", TYPE_FIRE, TYPE_DRAGON, 273, 213, 186, NULL, NULL, },
  {    6, "Mega Charizard Y", TYPE_FIRE, TYPE_FLYING, 319, 212, 186, NULL, NULL, },
  {    7, "Squirtle", TYPE_WATER, TYPECOUNT, 94, 121, 127, NULL, NULL, },
  {    8, "Wartortle", TYPE_WATER, TYPECOUNT, 126, 155, 153, NULL, NULL, },
  {    9, "Blastoise", TYPE_WATER, TYPECOUNT, 171, 207, 188, NULL, NULL, },
  {    9, "Mega Blastoise", TYPE_WATER, TYPECOUNT, 264, 237, 188, NULL, NULL, },
  {   10, "Caterpie", TYPE_BUG, TYPECOUNT, 55, 55, 128, NULL, NULL, },
  {   11, "Metapod", TYPE_BUG, TYPECOUNT, 45, 80, 137, NULL, NULL, },
  {   12, "Butterfree", TYPE_BUG, TYPE_FLYING, 167, 137, 155, NULL, NULL, },
  {   13, "Weedle", TYPE_BUG, TYPE_POISON, 63, 50, 120, NULL, NULL, },
  {   14, "Kakuna", TYPE_BUG, TYPE_POISON, 46, 75, 128, NULL, NULL, },
  {   15, "Beedrill", TYPE_BUG, TYPE_POISON, 169, 130, 163, NULL, NULL, },
  {   15, "Mega Beedrill", TYPE_BUG, TYPE_POISON, 303, 148, 163, NULL, NULL, },
  {   16, "Pidgey", TYPE_NORMAL, TYPE_FLYING, 85, 73, 120, NULL, NULL, },
  {   17, "Pidgeotto", TYPE_NORMAL, TYPE_FLYING, 117, 105, 160, NULL, NULL, },
  {   18, "Pidgeot", TYPE_NORMAL, TYPE_FLYING, 166, 154, 195, NULL, NULL, },
  {   18, "Mega Pidgeot", TYPE_NORMAL, TYPE_FLYING, 280, 175, 195, NULL, NULL, },
  {   19, "Rattata", TYPE_NORMAL, TYPECOUNT, 103, 70, 102, NULL, NULL, },
  {   19, "Alolan Rattata", TYPE_DARK, TYPE_NORMAL, 103, 70, 102, NULL, NULL, },
  {   20, "Raticate", TYPE_NORMAL, TYPECOUNT, 161, 139, 146, NULL, NULL, },
  {   20, "Alolan Raticate", TYPE_DARK, TYPE_NORMAL, 135, 154, 181, NULL, NULL, },
  {   21, "Spearow", TYPE_NORMAL, TYPE_FLYING, 112, 60, 120, NULL, NULL, },
  {   22, "Fearow", TYPE_NORMAL, TYPE_FLYING, 182, 133, 163, NULL, NULL, },
  {   23, "Ekans", TYPE_POISON, TYPECOUNT, 110, 97, 111, NULL, NULL, },
  {   24, "Arbok", TYPE_POISON, TYPECOUNT, 167, 153, 155, NULL, NULL, },
  {   25, "Pikachu", TYPE_ELECTRIC, TYPECOUNT, 112, 96, 111, NULL, NULL, },
  {   26, "Raichu", TYPE_ELECTRIC, TYPECOUNT, 193, 151, 155, NULL, NULL, },
  {   26, "Alolan Raichu", TYPE_ELECTRIC, TYPE_PSYCHIC, 201, 154, 155, NULL, NULL, },
  {   27, "Sandshrew", TYPE_GROUND, TYPECOUNT, 126, 120, 137, NULL, NULL, },
  {   27, "Alolan Sandshrew", TYPE_ICE, TYPE_STEEL, 125, 129, 137, NULL, NULL, },
  {   28, "Sandslash", TYPE_GROUND, TYPECOUNT, 182, 175, 181, NULL, NULL, },
  {   28, "Alolan Sandslash", TYPE_ICE, TYPE_STEEL, 177, 195, 181, NULL, NULL, },
  {   29, "Nidoran♀", TYPE_POISON, TYPECOUNT, 86, 89, 146, NULL, NULL, },
  {   30, "Nidorina", TYPE_POISON, TYPECOUNT, 117, 120, 172, NULL, NULL, },
  {   31, "Nidoqueen", TYPE_POISON, TYPE_GROUND, 180, 173, 207, NULL, NULL, },
  {   32, "Nidoran♂", TYPE_POISON, TYPECOUNT, 105, 76, 130, NULL, NULL, },
  {   33, "Nidorino", TYPE_POISON, TYPECOUNT, 137, 111, 156, NULL, NULL, },
  {   34, "Nidoking", TYPE_POISON, TYPE_GROUND, 204, 156, 191, NULL, NULL, },
  {   35, "Clefairy", TYPE_FAIRY, TYPECOUNT, 107, 108, 172, NULL, NULL, },
  {   36, "Clefable", TYPE_FAIRY, TYPECOUNT, 178, 162, 216, NULL, NULL, },
  {   37, "Vulpix", TYPE_FIRE, TYPECOUNT, 96, 109, 116, NULL, NULL, },
  {   37, "Alolan Vulpix", TYPE_ICE, TYPECOUNT, 96, 109, 116, NULL, NULL, },
  {   38, "Ninetales", TYPE_FIRE, TYPECOUNT, 169, 190, 177, NULL, NULL, },
  {   38, "Alolan Ninetales", TYPE_ICE, TYPE_FAIRY, 170, 193, 177, NULL, NULL, },
  {   39, "Jigglypuff", TYPE_NORMAL, TYPE_FAIRY, 80, 41, 251, NULL, NULL, },
  {   40, "Wigglytuff", TYPE_NORMAL, TYPE_FAIRY, 156, 90, 295, NULL, NULL, },
  {   41, "Zubat", TYPE_POISON, TYPE_FLYING, 83, 73, 120, NULL, NULL, },
  {   42, "Golbat", TYPE_POISON, TYPE_FLYING, 161, 150, 181, NULL, NULL, },
  {   43, "Oddish", TYPE_GRASS, TYPE_POISON, 131, 112, 128, NULL, NULL, },
  {   44, "Gloom", TYPE_GRASS, TYPE_POISON, 153, 136, 155, NULL, NULL, },
  {   45, "Vileplume", TYPE_GRASS, TYPE_POISON, 202, 167, 181, NULL, NULL, },
  {   46, "Paras", TYPE_BUG, TYPE_GRASS, 121, 99, 111, NULL, NULL, },
  {   47, "Parasect", TYPE_BUG, TYPE_GRASS, 165, 146, 155, NULL, NULL, },
  {   48, "Venonat", TYPE_BUG, TYPE_POISON, 100, 100, 155, NULL, NULL, },
  {   49, "Venomoth", TYPE_BUG, TYPE_POISON, 179, 143, 172, NULL, NULL, },
  {   50, "Diglett", TYPE_GROUND, TYPECOUNT, 109, 78, 67, NULL, NULL, },
  {   50, "Alolan Diglett", TYPE_GROUND, TYPE_STEEL, 109, 82, 67, NULL, NULL, },
  {   51, "Dugtrio", TYPE_GROUND, TYPECOUNT, 167, 134, 111, NULL, NULL, },
  {   51, "Alolan Dugtrio", TYPE_GROUND, TYPE_STEEL, 201, 145, 111, NULL, NULL, },
  {   52, "Meowth", TYPE_NORMAL, TYPECOUNT, 92, 78, 120, NULL, NULL, },
  {   52, "Alolan Meowth", TYPE_DARK, TYPECOUNT, 99, 78, 120, NULL, NULL, },
  {   52, "Galarian Meowth", TYPE_STEEL, TYPECOUNT, 115, 92, 137, NULL, NULL, },
  {   53, "Persian", TYPE_NORMAL, TYPECOUNT, 150, 136, 163, NULL, NULL, },
  {   53, "Alolan Persian", TYPE_DARK, TYPECOUNT, 158, 136, 163, NULL, NULL, },
  {   54, "Psyduck", TYPE_WATER, TYPECOUNT, 122, 95, 137, NULL, NULL, },
  {   55, "Golduck", TYPE_WATER, TYPECOUNT, 191, 162, 190, NULL, NULL, },
  {   56, "Mankey", TYPE_FIGHTING, TYPECOUNT, 148, 82, 120, NULL, NULL, },
  {   57, "Primeape", TYPE_FIGHTING, TYPECOUNT, 207, 138, 163, NULL, NULL, },
  {   58, "Growlithe", TYPE_FIRE, TYPECOUNT, 136, 93, 146, NULL, NULL, },
  {   58, "Hisuian Growlithe", TYPE_FIRE, TYPE_ROCK, 142, 92, 155, NULL, NULL, },
  {   59, "Arcanine", TYPE_FIRE, TYPECOUNT, 227, 166, 207, NULL, NULL, },
  {   59, "Hisuian Arcanine", TYPE_FIRE, TYPE_ROCK, 232, 165, 216, NULL, NULL, },
  {   60, "Poliwag", TYPE_WATER, TYPECOUNT, 101, 82, 120, NULL, NULL, },
  {   61, "Poliwhirl", TYPE_WATER, TYPECOUNT, 130, 123, 163, NULL, NULL, },
  {   62, "Poliwrath", TYPE_WATER, TYPE_FIGHTING, 182, 184, 207, NULL, NULL, },
  {   63, "Abra", TYPE_PSYCHIC, TYPECOUNT, 195, 82, 93, NULL, NULL, },
  {   64, "Kadabra", TYPE_PSYCHIC, TYPECOUNT, 232, 117, 120, NULL, NULL, },
  {   65, "Alakazam", TYPE_PSYCHIC, TYPECOUNT, 271, 167, 146, NULL, NULL, },
  {   65, "Mega Alakazam", TYPE_PSYCHIC, TYPECOUNT, 367, 207, 146, NULL, NULL, },
  {   66, "Machop", TYPE_FIGHTING, TYPECOUNT, 137, 82, 172, NULL, NULL, },
  {   67, "Machoke", TYPE_FIGHTING, TYPECOUNT, 177, 125, 190, NULL, NULL, },
  {   68, "Machamp", TYPE_FIGHTING, TYPECOUNT, 234, 159, 207, NULL, NULL, },
  {   69, "Bellsprout", TYPE_GRASS, TYPE_POISON, 139, 61, 137, NULL, NULL, },
  {   70, "Weepinbell", TYPE_GRASS, TYPE_POISON, 172, 92, 163, NULL, NULL, },
  {   71, "Victreebel", TYPE_GRASS, TYPE_POISON, 207, 135, 190, NULL, NULL, },
  {   72, "Tentacool", TYPE_WATER, TYPE_POISON, 97, 149, 120, NULL, NULL, },
  {   73, "Tentacruel", TYPE_WATER, TYPE_POISON, 166, 209, 190, NULL, NULL, },
  {   74, "Geodude", TYPE_ROCK, TYPE_GROUND, 132, 132, 120, NULL, NULL, },
  {   74, "Alolan Geodude", TYPE_ROCK, TYPE_ELECTRIC, 132, 132, 120, NULL, NULL, },
  {   75, "Graveler", TYPE_ROCK, TYPE_GROUND, 164, 164, 146, NULL, NULL, },
  {   75, "Alolan Graveler", TYPE_ROCK, TYPE_ELECTRIC, 164, 164, 146, NULL, NULL, },
  {   76, "Golem", TYPE_ROCK, TYPE_GROUND, 211, 198, 190, NULL, NULL, },
  {   76, "Alolan Golem", TYPE_ROCK, TYPE_ELECTRIC, 211, 198, 190, NULL, NULL, },
  {   77, "Ponyta", TYPE_FIRE, TYPECOUNT, 170, 127, 137, NULL, NULL, },
  {   77, "Galarian Ponyta", TYPE_PSYCHIC, TYPECOUNT, 170, 127, 137, NULL, NULL, },
  {   78, "Rapidash", TYPE_FIRE, TYPECOUNT, 207, 162, 163, NULL, NULL, },
  {   78, "Galarian Rapidash", TYPE_PSYCHIC, TYPE_FAIRY, 207, 162, 163, NULL, NULL, },
  {   79, "Slowpoke", TYPE_WATER, TYPE_PSYCHIC, 109, 98, 207, NULL, NULL, },
  {   79, "Galarian Slowpoke", TYPE_PSYCHIC, TYPECOUNT, 109, 98, 207, NULL, NULL, },
  {   80, "Slowbro", TYPE_WATER, TYPE_PSYCHIC, 177, 180, 216, NULL, NULL, },
  {   80, "Mega Slowbro", TYPE_WATER, TYPE_PSYCHIC, 224, 259, 216, NULL, NULL, },
  {   80, "Galarian Slowbro", TYPE_POISON, TYPE_PSYCHIC, 182, 156, 216, NULL, NULL, },
  {   81, "Magnemite", TYPE_ELECTRIC, TYPE_STEEL, 165, 121, 93, NULL, NULL, },
  {   82, "Magneton", TYPE_ELECTRIC, TYPE_STEEL, 223, 169, 137, NULL, NULL, },
  {   83, "Farfetch'd", TYPE_NORMAL, TYPE_FLYING, 124, 115, 141, NULL, NULL, },
  {   83, "Galarian Farfetch'd", TYPE_FIGHTING, TYPECOUNT, 174, 114, 141, NULL, NULL, },
  {   84, "Doduo", TYPE_NORMAL, TYPE_FLYING, 158, 83, 111, NULL, NULL, },
  {   85, "Dodrio", TYPE_NORMAL, TYPE_FLYING, 218, 140, 155, NULL, NULL, },
  {   86, "Seel", TYPE_WATER, TYPECOUNT, 85, 121, 163, NULL, NULL, },
  {   87, "Dewgong", TYPE_WATER, TYPE_ICE, 139, 177, 207, NULL, NULL, },
  {   88, "Grimer", TYPE_POISON, TYPECOUNT, 135, 90, 190, NULL, NULL, },
  {   88, "Alolan Grimer", TYPE_POISON, TYPE_DARK, 135, 90, 190, NULL, NULL, },
  {   89, "Muk", TYPE_POISON, TYPECOUNT, 190, 172, 233, NULL, NULL, },
  {   89, "Alolan Muk", TYPE_POISON, TYPE_DARK, 190, 172, 233, NULL, NULL, },
  {   90, "Shellder", TYPE_WATER, TYPECOUNT, 116, 134, 102, NULL, NULL, },
  {   91, "Cloyster", TYPE_WATER, TYPE_ICE, 186, 256, 137, NULL, NULL, },
  {   92, "Gastly", TYPE_GHOST, TYPE_POISON, 186, 67, 102, NULL, NULL, },
  {   93, "Haunter", TYPE_GHOST, TYPE_POISON, 223, 107, 128, NULL, NULL, },
  {   94, "Gengar", TYPE_GHOST, TYPE_POISON, 261, 149, 155, NULL, NULL, },
  {   94, "Mega Gengar", TYPE_GHOST, TYPE_POISON, 349, 199, 155, NULL, NULL, },
  {   95, "Onix", TYPE_ROCK, TYPE_GROUND, 85, 232, 111, NULL, NULL, },
  {   96, "Drowzee", TYPE_PSYCHIC, TYPECOUNT, 89, 136, 155, NULL, NULL, },
  {   97, "Hypno", TYPE_PSYCHIC, TYPECOUNT, 144, 193, 198, NULL, NULL, },
  {   98, "Krabby", TYPE_WATER, TYPECOUNT, 181, 124, 102, NULL, NULL, },
  {   99, "Kingler", TYPE_WATER, TYPECOUNT, 240, 181, 146, NULL, NULL, },
  {  100, "Voltorb", TYPE_ELECTRIC, TYPECOUNT, 109, 111, 120, NULL, NULL, },
  {  100, "Hisuian Voltorb", TYPE_ELECTRIC, TYPE_GRASS, 109, 111, 120, NULL, NULL, },
  {  101, "Electrode", TYPE_ELECTRIC, TYPECOUNT, 173, 173, 155, NULL, NULL, },
  {  101, "Hisuian Electrode", TYPE_ELECTRIC, TYPE_GRASS, 176, 176, 155, NULL, NULL, },
  {  102, "Exeggcute", TYPE_GRASS, TYPE_PSYCHIC, 107, 125, 155, NULL, NULL, },
  {  103, "Exeggutor", TYPE_GRASS, TYPE_PSYCHIC, 233, 149, 216, NULL, NULL, },
  {  103, "Alolan Exeggutor", TYPE_GRASS, TYPE_DRAGON, 230, 153, 216, NULL, NULL, },
  {  104, "Cubone", TYPE_GROUND, TYPECOUNT, 90, 144, 137, NULL, NULL, },
  {  105, "Marowak", TYPE_GROUND, TYPECOUNT, 144, 186, 155, NULL, NULL, },
  {  105, "Alolan Marowak", TYPE_FIRE, TYPE_GHOST, 144, 186, 155, NULL, NULL, },
  {  106, "Hitmonlee", TYPE_FIGHTING, TYPECOUNT, 224, 181, 137, NULL, NULL, },
  {  107, "Hitmonchan", TYPE_FIGHTING, TYPECOUNT, 193, 197, 137, NULL, NULL, },
  {  108, "Lickitung", TYPE_NORMAL, TYPECOUNT, 108, 137, 207, NULL, NULL, },
  {  109, "Koffing", TYPE_POISON, TYPECOUNT, 119, 141, 120, NULL, NULL, },
  {  110, "Weezing", TYPE_POISON, TYPECOUNT, 174, 197, 163, NULL, NULL, },
  {  110, "Galarian Weezing", TYPE_POISON, TYPE_FAIRY, 174, 197, 163, NULL, NULL, },
  {  111, "Rhyhorn", TYPE_GROUND, TYPE_ROCK, 140, 127, 190, NULL, NULL, },
  {  112, "Rhydon", TYPE_GROUND, TYPE_ROCK, 222, 171, 233, NULL, NULL, },
  {  113, "Chansey", TYPE_NORMAL, TYPECOUNT, 60, 128, 487, NULL, NULL, },
  {  114, "Tangela", TYPE_GRASS, TYPECOUNT, 183, 169, 163, NULL, NULL, },
  {  115, "Kangaskhan", TYPE_NORMAL, TYPECOUNT, 181, 165, 233, NULL, NULL, },
  {  115, "Mega Kangaskhan", TYPE_NORMAL, TYPECOUNT, 246, 210, 233, NULL, NULL, },
  {  116, "Horsea", TYPE_WATER, TYPECOUNT, 129, 103, 102, NULL, NULL, },
  {  117, "Seadra", TYPE_WATER, TYPECOUNT, 187, 156, 146, NULL, NULL, },
  {  118, "Goldeen", TYPE_WATER, TYPECOUNT, 123, 110, 128, NULL, NULL, },
  {  119, "Seaking", TYPE_WATER, TYPECOUNT, 175, 147, 190, NULL, NULL, },
  {  120, "Staryu", TYPE_WATER, TYPECOUNT, 137, 112, 102, NULL, NULL, },
  {  121, "Starmie", TYPE_WATER, TYPE_PSYCHIC, 210, 184, 155, NULL, NULL, },
  {  122, "Mr. Mime", TYPE_PSYCHIC, TYPE_FAIRY, 192, 205, 120, NULL, NULL, },
  {  122, "Galarian Mr. Mime", TYPE_ICE, TYPE_PSYCHIC, 183, 169, 137, NULL, NULL, },
  {  123, "Scyther", TYPE_BUG, TYPE_FLYING, 218, 170, 172, NULL, NULL, },
  {  124, "Jynx", TYPE_ICE, TYPE_PSYCHIC, 223, 151, 163, NULL, NULL, },
  {  125, "Electabuzz", TYPE_ELECTRIC, TYPECOUNT, 198, 158, 163, NULL, NULL, },
  {  126, "Magmar", TYPE_FIRE, TYPECOUNT, 206, 154, 163, NULL, NULL, },
  {  127, "Pinsir", TYPE_BUG, TYPECOUNT, 238, 182, 163, NULL, NULL, },
  {  127, "Mega Pinsir", TYPE_BUG, TYPE_FLYING, 305, 231, 163, NULL, NULL, },
  {  128, "Tauros", TYPE_NORMAL, TYPECOUNT, 198, 183, 181, NULL, NULL, },
  {  128, "Combat Breed Tauros", TYPE_FIGHTING, TYPECOUNT, 210, 193, 181, NULL, NULL, },
  {  128, "Blaze Breed Tauros", TYPE_FIGHTING, TYPE_FIRE, 210, 193, 181, NULL, NULL, },
  {  128, "Aqua Breed Tauros", TYPE_FIGHTING, TYPE_WATER, 210, 193, 181, NULL, NULL, },
  {  129, "Magikarp", TYPE_WATER, TYPECOUNT, 29, 85, 85, NULL, NULL, },
  {  130, "Gyarados", TYPE_WATER, TYPE_FLYING, 237, 186, 216, NULL, NULL, },
  {  130, "Mega Gyarados", TYPE_WATER, TYPE_DARK, 292, 247, 216, NULL, NULL, },
  {  131, "Lapras", TYPE_WATER, TYPE_ICE, 165, 174, 277, NULL, NULL, },
  {  132, "Ditto", TYPE_NORMAL, TYPECOUNT, 91, 91, 134, NULL, NULL, },
  {  133, "Eevee", TYPE_NORMAL, TYPECOUNT, 104, 114, 146, NULL, NULL, },
  {  134, "Vaporeon", TYPE_WATER, TYPECOUNT, 205, 161, 277, NULL, NULL, },
  {  135, "Jolteon", TYPE_ELECTRIC, TYPECOUNT, 232, 182, 163, NULL, NULL, },
  {  136, "Flareon", TYPE_FIRE, TYPECOUNT, 246, 179, 163, NULL, NULL, },
  {  137, "Porygon", TYPE_NORMAL, TYPECOUNT, 153, 136, 163, NULL, NULL, },
  {  138, "Omanyte", TYPE_ROCK, TYPE_WATER, 155, 153, 111, NULL, NULL, },
  {  139, "Omastar", TYPE_ROCK, TYPE_WATER, 207, 201, 172, NULL, NULL, },
  {  140, "Kabuto", TYPE_ROCK, TYPE_WATER, 148, 140, 102, NULL, NULL, },
  {  141, "Kabutops", TYPE_ROCK, TYPE_WATER, 220, 186, 155, NULL, NULL, },
  {  142, "Aerodactyl", TYPE_ROCK, TYPE_FLYING, 221, 159, 190, NULL, NULL, },
  {  142, "Mega Aerodactyl", TYPE_ROCK, TYPE_FLYING, 292, 210, 190, NULL, NULL, },
  {  143, "Snorlax", TYPE_NORMAL, TYPECOUNT, 190, 169, 330, NULL, NULL, },
  {  144, "Articuno", TYPE_ICE, TYPE_FLYING, 192, 236, 207, NULL, NULL, },
  {  144, "Galarian Articuno", TYPE_PSYCHIC, TYPE_FLYING, 250, 197, 207, NULL, NULL, },
  {  145, "Zapdos", TYPE_ELECTRIC, TYPE_FLYING, 253, 185, 207, NULL, NULL, },
  {  145, "Galarian Zapdos", TYPE_FIGHTING, TYPE_FLYING, 252, 189, 207, NULL, NULL, },
  {  146, "Moltres", TYPE_FIRE, TYPE_FLYING, 251, 181, 207, NULL, NULL, },
  {  146, "Galarian Moltres", TYPE_DARK, TYPE_FLYING, 202, 231, 207, NULL, NULL, },
  {  147, "Dratini", TYPE_DRAGON, TYPECOUNT, 119, 91, 121, NULL, NULL, },
  {  148, "Dragonair", TYPE_DRAGON, TYPECOUNT, 163, 135, 156, NULL, NULL, },
  {  149, "Dragonite", TYPE_DRAGON, TYPE_FLYING, 263, 198, 209, NULL, NULL, },
  {  150, "Mewtwo", TYPE_PSYCHIC, TYPECOUNT, 300, 182, 214, NULL, NULL, },
  {  150, "Armored Mewtwo", TYPE_PSYCHIC, TYPECOUNT, 182, 278, 214, NULL, NULL, },
  {  151, "Mew", TYPE_PSYCHIC, TYPECOUNT, 210, 210, 225, NULL, NULL, },
  {  152, "Chikorita", TYPE_GRASS, TYPECOUNT, 92, 122, 128, NULL, NULL, },
  {  153, "Bayleef", TYPE_GRASS, TYPECOUNT, 122, 155, 155, NULL, NULL, },
  {  154, "Meganium", TYPE_GRASS, TYPECOUNT, 168, 202, 190, NULL, NULL, },
  {  155, "Cyndaquil", TYPE_FIRE, TYPECOUNT, 116, 93, 118, NULL, NULL, },
  {  156, "Quilava", TYPE_FIRE, TYPECOUNT, 158, 126, 151, NULL, NULL, },
  {  157, "Typhlosion", TYPE_FIRE, TYPECOUNT, 223, 173, 186, NULL, NULL, },
  {  157, "Hisuian Typhlosion", TYPE_FIRE, TYPE_GHOST, 238, 172, 177, NULL, NULL, },
  {  158, "Totodile", TYPE_WATER, TYPECOUNT, 117, 109, 137, NULL, NULL, },
  {  159, "Croconaw", TYPE_WATER, TYPECOUNT, 150, 142, 163, NULL, NULL, },
  {  160, "Feraligatr", TYPE_WATER, TYPECOUNT, 205, 188, 198, NULL, NULL, },
  {  161, "Sentret", TYPE_NORMAL, TYPECOUNT, 79, 73, 111, NULL, NULL, },
  {  162, "Furret", TYPE_NORMAL, TYPECOUNT, 148, 125, 198, NULL, NULL, },
  {  163, "Hoothoot", TYPE_NORMAL, TYPE_FLYING, 67, 88, 155, NULL, NULL, },
  {  164, "Noctowl", TYPE_NORMAL, TYPE_FLYING, 145, 156, 225, NULL, NULL, },
  {  165, "Ledyba", TYPE_BUG, TYPE_FLYING, 72, 118, 120, NULL, NULL, },
  {  166, "Ledian", TYPE_BUG, TYPE_FLYING, 107, 179, 146, NULL, NULL, },
  {  167, "Spinarak", TYPE_BUG, TYPE_POISON, 105, 73, 120, NULL, NULL, },
  {  168, "Ariados", TYPE_BUG, TYPE_POISON, 161, 124, 172, NULL, NULL, },
  {  169, "Crobat", TYPE_POISON, TYPE_FLYING, 194, 178, 198, NULL, NULL, },
  {  170, "Chinchou", TYPE_WATER, TYPE_ELECTRIC, 106, 97, 181, NULL, NULL, },
  {  171, "Lanturn", TYPE_WATER, TYPE_ELECTRIC, 146, 137, 268, NULL, NULL, },
  {  172, "Pichu", TYPE_ELECTRIC, TYPECOUNT, 77, 53, 85, NULL, NULL, },
  {  173, "Cleffa", TYPE_FAIRY, TYPECOUNT, 75, 79, 137, NULL, NULL, },
  {  174, "Igglybuff", TYPE_NORMAL, TYPE_FAIRY, 69, 32, 207, NULL, NULL, },
  {  175, "Togepi", TYPE_FAIRY, TYPECOUNT, 67, 116, 111, NULL, NULL, },
  {  176, "Togetic", TYPE_FAIRY, TYPE_FLYING, 139, 181, 146, NULL, NULL, },
  {  177, "Natu", TYPE_PSYCHIC, TYPE_FLYING, 134, 89, 120, NULL, NULL, },
  {  178, "Xatu", TYPE_PSYCHIC, TYPE_FLYING, 192, 146, 163, NULL, NULL, },
  {  179, "Mareep", TYPE_ELECTRIC, TYPECOUNT, 114, 79, 146, NULL, NULL, },
  {  180, "Flaaffy", TYPE_ELECTRIC, TYPECOUNT, 145, 109, 172, NULL, NULL, },
  {  181, "Ampharos", TYPE_ELECTRIC, TYPECOUNT, 211, 169, 207, NULL, NULL, },
  {  181, "Mega Ampharos", TYPE_ELECTRIC, TYPE_DRAGON, 294, 203, 207, NULL, NULL, },
  {  182, "Bellossom", TYPE_GRASS, TYPECOUNT, 169, 186, 181, NULL, NULL, },
  {  183, "Marill", TYPE_WATER, TYPE_FAIRY, 37, 93, 172, NULL, NULL, },
  {  184, "Azumarill", TYPE_WATER, TYPE_FAIRY, 112, 152, 225, NULL, NULL, },
  {  185, "Sudowoodo", TYPE_ROCK, TYPECOUNT, 167, 176, 172, NULL, NULL, },
  {  186, "Politoed", TYPE_WATER, TYPECOUNT, 174, 179, 207, NULL, NULL, },
  {  187, "Hoppip", TYPE_GRASS, TYPE_FLYING, 67, 94, 111, NULL, NULL, },
  {  188, "Skiploom", TYPE_GRASS, TYPE_FLYING, 91, 120, 146, NULL, NULL, },
  {  189, "Jumpluff", TYPE_GRASS, TYPE_FLYING, 118, 183, 181, NULL, NULL, },
  {  190, "Aipom", TYPE_NORMAL, TYPECOUNT, 136, 112, 146, NULL, NULL, },
  {  191, "Sunkern", TYPE_GRASS, TYPECOUNT, 55, 55, 102, NULL, NULL, },
  {  192, "Sunflora", TYPE_GRASS, TYPECOUNT, 185, 135, 181, NULL, NULL, },
  {  193, "Yanma", TYPE_BUG, TYPE_FLYING, 154, 94, 163, NULL, NULL, },
  {  194, "Wooper", TYPE_WATER, TYPE_GROUND, 75, 66, 146, NULL, NULL, },
  {  194, "Paldean Wooper", TYPE_POISON, TYPE_GROUND, 75, 66, 146, NULL, NULL, },
  {  195, "Quagsire", TYPE_WATER, TYPE_GROUND, 152, 143, 216, NULL, NULL, },
  {  196, "Espeon", TYPE_PSYCHIC, TYPECOUNT, 261, 175, 163, NULL, NULL, },
  {  197, "Umbreon", TYPE_DARK, TYPECOUNT, 126, 240, 216, NULL, NULL, },
  {  198, "Murkrow", TYPE_DARK, TYPE_FLYING, 175, 87, 155, NULL, NULL, },
  {  199, "Slowking", TYPE_WATER, TYPE_PSYCHIC, 177, 180, 216, NULL, NULL, },
  {  199, "Galarian Slowking", TYPE_POISON, TYPE_PSYCHIC, 190, 180, 216, NULL, NULL, },
  {  200, "Misdreavus", TYPE_GHOST, TYPECOUNT, 167, 154, 155, NULL, NULL, },
  {  201, "Unown", TYPE_PSYCHIC, TYPECOUNT, 136, 91, 134, NULL, NULL, },
  {  202, "Wobbuffet", TYPE_PSYCHIC, TYPECOUNT, 60, 106, 382, NULL, NULL, },
  {  203, "Girafarig", TYPE_NORMAL, TYPE_PSYCHIC, 182, 133, 172, NULL, NULL, },
  {  204, "Pineco", TYPE_BUG, TYPECOUNT, 108, 122, 137, NULL, NULL, },
  {  205, "Forretress", TYPE_BUG, TYPE_STEEL, 161, 205, 181, NULL, NULL, },
  {  206, "Dunsparce", TYPE_NORMAL, TYPECOUNT, 131, 128, 225, NULL, NULL, },
  {  207, "Gligar", TYPE_GROUND, TYPE_FLYING, 143, 184, 163, NULL, NULL, },
  {  208, "Steelix", TYPE_STEEL, TYPE_GROUND, 148, 272, 181, NULL, NULL, },
  {  208, "Mega Steelix", TYPE_STEEL, TYPE_GROUND, 212, 327, 181, NULL, NULL, },
  {  209, "Snubbull", TYPE_FAIRY, TYPECOUNT, 137, 85, 155, NULL, NULL, },
  {  210, "Granbull", TYPE_FAIRY, TYPECOUNT, 212, 131, 207, NULL, NULL, },
  {  211, "Qwilfish", TYPE_WATER, TYPE_POISON, 184, 138, 163, NULL, NULL, },
  {  211, "Hisuian Qwilfish", TYPE_DARK, TYPE_POISON, 184, 151, 163, NULL, NULL, },
  {  212, "Scizor", TYPE_BUG, TYPE_STEEL, 236, 181, 172, NULL, NULL, },
  {  212, "Mega Scizor", TYPE_BUG, TYPE_STEEL, 279, 250, 172, NULL, NULL, },
  {  213, "Shuckle", TYPE_BUG, TYPE_ROCK, 17, 396, 85, NULL, NULL, },
  {  214, "Heracross", TYPE_BUG, TYPE_FIGHTING, 234, 179, 190, NULL, NULL, },
  {  214, "Mega Heracross", TYPE_BUG, TYPE_FIGHTING, 334, 223, 190, NULL, NULL, },
  {  215, "Sneasel", TYPE_DARK, TYPE_ICE, 189, 146, 146, NULL, NULL, },
  {  215, "Hisuian Sneasel", TYPE_FIGHTING, TYPE_POISON, 189, 146, 146, NULL, NULL, },
  {  216, "Teddiursa", TYPE_NORMAL, TYPECOUNT, 142, 93, 155, NULL, NULL, },
  {  217, "Ursaring", TYPE_NORMAL, TYPECOUNT, 236, 144, 207, NULL, NULL, },
  {  218, "Slugma", TYPE_FIRE, TYPECOUNT, 118, 71, 120, NULL, NULL, },
  {  219, "Magcargo", TYPE_FIRE, TYPE_ROCK, 139, 191, 137, NULL, NULL, },
  {  220, "Swinub", TYPE_ICE, TYPE_GROUND, 90, 69, 137, NULL, NULL, },
  {  221, "Piloswine", TYPE_ICE, TYPE_GROUND, 181, 138, 225, NULL, NULL, },
  {  222, "Corsola", TYPE_WATER, TYPE_ROCK, 118, 156, 146, NULL, NULL, },
  {  222, "Galarian Corsola", TYPE_GHOST, TYPECOUNT, 116, 182, 155, NULL, NULL, },
  {  223, "Remoraid", TYPE_WATER, TYPECOUNT, 127, 69, 111, NULL, NULL, },
  {  224, "Octillery", TYPE_WATER, TYPECOUNT, 197, 141, 181, NULL, NULL, },
  {  225, "Delibird", TYPE_ICE, TYPE_FLYING, 128, 90, 128, NULL, NULL, },
  {  226, "Mantine", TYPE_WATER, TYPE_FLYING, 148, 226, 163, NULL, NULL, },
  {  227, "Skarmory", TYPE_STEEL, TYPE_FLYING, 148, 226, 163, NULL, NULL, },
  {  228, "Houndour", TYPE_DARK, TYPE_FIRE, 152, 83, 128, NULL, NULL, },
  {  229, "Houndoom", TYPE_DARK, TYPE_FIRE, 224, 144, 181, NULL, NULL, },
  {  229, "Mega Houndoom", TYPE_DARK, TYPE_FIRE, 289, 194, 181, NULL, NULL, },
  {  230, "Kingdra", TYPE_WATER, TYPE_DRAGON, 194, 194, 181, NULL, NULL, },
  {  231, "Phanpy", TYPE_GROUND, TYPECOUNT, 107, 98, 207, NULL, NULL, },
  {  232, "Donphan", TYPE_GROUND, TYPECOUNT, 214, 185, 207, NULL, NULL, },
  {  233, "Porygon2", TYPE_NORMAL, TYPECOUNT, 198, 180, 198, NULL, NULL, },
  {  234, "Stantler", TYPE_NORMAL, TYPECOUNT, 192, 131, 177, NULL, NULL, },
  {  235, "Smeargle", TYPE_NORMAL, TYPECOUNT, 40, 83, 146, NULL, NULL, },
  {  236, "Tyrogue", TYPE_FIGHTING, TYPECOUNT, 64, 64, 111, NULL, NULL, },
  {  237, "Hitmontop", TYPE_FIGHTING, TYPECOUNT, 173, 207, 137, NULL, NULL, },
  {  238, "Smoochum", TYPE_ICE, TYPE_PSYCHIC, 153, 91, 128, NULL, NULL, },
  {  239, "Elekid", TYPE_ELECTRIC, TYPECOUNT, 135, 101, 128, NULL, NULL, },
  {  240, "Magby", TYPE_FIRE, TYPECOUNT, 151, 99, 128, NULL, NULL, },
  {  241, "Miltank", TYPE_NORMAL, TYPECOUNT, 157, 193, 216, NULL, NULL, },
  {  242, "Blissey", TYPE_NORMAL, TYPECOUNT, 129, 169, 496, NULL, NULL, },
  {  243, "Raikou", TYPE_ELECTRIC, TYPECOUNT, 241, 195, 207, NULL, NULL, },
  {  244, "Entei", TYPE_FIRE, TYPECOUNT, 235, 171, 251, NULL, NULL, },
  {  245, "Suicune", TYPE_WATER, TYPECOUNT, 180, 235, 225, NULL, NULL, },
  {  246, "Larvitar", TYPE_ROCK, TYPE_GROUND, 115, 93, 137, NULL, NULL, },
  {  247, "Pupitar", TYPE_ROCK, TYPE_GROUND, 155, 133, 172, NULL, NULL, },
  {  248, "Tyranitar", TYPE_ROCK, TYPE_DARK, 251, 207, 225, NULL, NULL, },
  {  248, "Mega Tyranitar", TYPE_ROCK, TYPE_DARK, 309, 276, 225, NULL, NULL, },
  {  249, "Lugia", TYPE_PSYCHIC, TYPE_FLYING, 193, 310, 235, NULL, NULL, },
  {  250, "Ho-Oh", TYPE_FIRE, TYPE_FLYING, 239, 244, 214, NULL, NULL, },
  {  251, "Celebi", TYPE_PSYCHIC, TYPE_GRASS, 210, 210, 225, NULL, NULL, },
  {  252, "Treecko", TYPE_GRASS, TYPECOUNT, 124, 94, 120, NULL, NULL, },
  {  253, "Grovyle", TYPE_GRASS, TYPECOUNT, 172, 120, 137, NULL, NULL, },
  {  254, "Sceptile", TYPE_GRASS, TYPECOUNT, 223, 169, 172, NULL, NULL, },
  {  254, "Mega Sceptile", TYPE_GRASS, TYPE_DRAGON, 320, 186, 172, NULL, NULL, },
  {  255, "Torchic", TYPE_FIRE, TYPECOUNT, 130, 87, 128, NULL, NULL, },
  {  256, "Combusken", TYPE_FIRE, TYPE_FIGHTING, 163, 115, 155, NULL, NULL, },
  {  257, "Blaziken", TYPE_FIRE, TYPE_FIGHTING, 240, 141, 190, NULL, NULL, },
  {  257, "Mega Blaziken", TYPE_FIRE, TYPE_FIGHTING, 329, 168, 190, NULL, NULL, },
  {  258, "Mudkip", TYPE_WATER, TYPECOUNT, 126, 93, 137, NULL, NULL, },
  {  259, "Marshtomp", TYPE_WATER, TYPE_GROUND, 156, 133, 172, NULL, NULL, },
  {  260, "Swampert", TYPE_WATER, TYPE_GROUND, 208, 175, 225, NULL, NULL, },
  {  260, "Mega Swampert", TYPE_WATER, TYPE_GROUND, 283, 218, 225, NULL, NULL, },
  {  261, "Poochyena", TYPE_DARK, TYPECOUNT, 96, 61, 111, NULL, NULL, },
  {  262, "Mightyena", TYPE_DARK, TYPECOUNT, 171, 132, 172, NULL, NULL, },
  {  263, "Zigzagoon", TYPE_NORMAL, TYPECOUNT, 58, 80, 116, NULL, NULL, },
  {  263, "Galarian Zigzagoon", TYPE_DARK, TYPE_NORMAL, 58, 80, 116, NULL, NULL, },
  {  264, "Linoone", TYPE_NORMAL, TYPECOUNT, 142, 128, 186, NULL, NULL, },
  {  264, "Galarian Linoone", TYPE_DARK, TYPE_NORMAL, 142, 128, 186, NULL, NULL, },
  {  265, "Wurmple", TYPE_BUG, TYPECOUNT, 75, 59, 128, NULL, NULL, },
  {  266, "Silcoon", TYPE_BUG, TYPECOUNT, 60, 77, 137, NULL, NULL, },
  {  267, "Beautifly", TYPE_BUG, TYPE_FLYING, 189, 98, 155, NULL, NULL, },
  {  268, "Cascoon", TYPE_BUG, TYPECOUNT, 60, 77, 137, NULL, NULL, },
  {  269, "Dustox", TYPE_BUG, TYPE_POISON, 98, 162, 155, NULL, NULL, },
  {  270, "Lotad", TYPE_WATER, TYPE_GRASS, 71, 77, 120, NULL, NULL, },
  {  271, "Lombre", TYPE_WATER, TYPE_GRASS, 112, 119, 155, NULL, NULL, },
  {  272, "Ludicolo", TYPE_WATER, TYPE_GRASS, 173, 176, 190, NULL, NULL, },
  {  273, "Seedot", TYPE_GRASS, TYPECOUNT, 71, 77, 120, NULL, NULL, },
  {  274, "Nuzleaf", TYPE_GRASS, TYPE_DARK, 134, 78, 172, NULL, NULL, },
  {  275, "Shiftry", TYPE_GRASS, TYPE_DARK, 200, 121, 207, NULL, NULL, },
  {  276, "Taillow", TYPE_NORMAL, TYPE_FLYING, 106, 61, 120, NULL, NULL, },
  {  277, "Swellow", TYPE_NORMAL, TYPE_FLYING, 185, 124, 155, NULL, NULL, },
  {  278, "Wingull", TYPE_WATER, TYPE_FLYING, 106, 61, 120, NULL, NULL, },
  {  279, "Pelipper", TYPE_WATER, TYPE_FLYING, 175, 174, 155, NULL, NULL, },
  {  280, "Ralts", TYPE_PSYCHIC, TYPE_FAIRY, 79, 59, 99, NULL, NULL, },
  {  281, "Kirlia", TYPE_PSYCHIC, TYPE_FAIRY, 117, 90, 116, NULL, NULL, },
  {  282, "Gardevoir", TYPE_PSYCHIC, TYPE_FAIRY, 237, 195, 169, NULL, NULL, },
  {  282, "Mega Gardevoir", TYPE_PSYCHIC, TYPE_FAIRY, 326, 229, 169, NULL, NULL, },
  {  283, "Surskit", TYPE_BUG, TYPE_WATER, 93, 87, 120, NULL, NULL, },
  {  284, "Masquerain", TYPE_BUG, TYPE_FLYING, 192, 150, 172, NULL, NULL, },
  {  285, "Shroomish", TYPE_GRASS, TYPECOUNT, 74, 110, 155, NULL, NULL, },
  {  286, "Breloom", TYPE_GRASS, TYPE_FIGHTING, 241, 144, 155, NULL, NULL, },
  {  287, "Slakoth", TYPE_NORMAL, TYPECOUNT, 104, 92, 155, NULL, NULL, },
  {  288, "Vigoroth", TYPE_NORMAL, TYPECOUNT, 159, 145, 190, NULL, NULL, },
  {  289, "Slaking", TYPE_NORMAL, TYPECOUNT, 290, 166, 284, NULL, NULL, },
  {  290, "Nincada", TYPE_BUG, TYPE_GROUND, 80, 126, 104, NULL, NULL, },
  {  291, "Ninjask", TYPE_BUG, TYPE_FLYING, 199, 112, 156, NULL, NULL, },
  {  292, "Shedinja", TYPE_BUG, TYPE_GHOST, 153, 73, 1, NULL, NULL, },
  {  293, "Whismur", TYPE_NORMAL, TYPECOUNT, 92, 42, 162, NULL, NULL, },
  {  294, "Loudred", TYPE_NORMAL, TYPECOUNT, 134, 81, 197, NULL, NULL, },
  {  295, "Exploud", TYPE_NORMAL, TYPECOUNT, 179, 137, 232, NULL, NULL, },
  {  296, "Makuhita", TYPE_FIGHTING, TYPECOUNT, 99, 54, 176, NULL, NULL, },
  {  297, "Hariyama", TYPE_FIGHTING, TYPECOUNT, 209, 114, 302, NULL, NULL, },
  {  298, "Azurill", TYPE_NORMAL, TYPE_FAIRY, 36, 71, 137, NULL, NULL, },
  {  299, "Nosepass", TYPE_ROCK, TYPECOUNT, 82, 215, 102, NULL, NULL, },
  {  300, "Skitty", TYPE_NORMAL, TYPECOUNT, 84, 79, 137, NULL, NULL, },
  {  301, "Delcatty", TYPE_NORMAL, TYPECOUNT, 132, 127, 172, NULL, NULL, },
  {  302, "Sableye", TYPE_DARK, TYPE_GHOST, 141, 136, 137, NULL, NULL, },
  {  302, "Mega Sableye", TYPE_DARK, TYPE_GHOST, 151, 216, 137, NULL, NULL, },
  {  303, "Mawile", TYPE_STEEL, TYPE_FAIRY, 155, 141, 137, NULL, NULL, },
  {  303, "Mega Mawile", TYPE_STEEL, TYPE_FAIRY, 188, 217, 137, NULL, NULL, },
  {  304, "Aron", TYPE_STEEL, TYPE_ROCK, 121, 141, 137, NULL, NULL, },
  {  305, "Lairon", TYPE_STEEL, TYPE_ROCK, 158, 198, 155, NULL, NULL, },
  {  306, "Aggron", TYPE_STEEL, TYPE_ROCK, 198, 257, 172, NULL, NULL, },
  {  306, "Mega Aggron", TYPE_STEEL, TYPECOUNT, 247, 331, 172, NULL, NULL, },
  {  307, "Meditite", TYPE_FIGHTING, TYPE_PSYCHIC, 78, 107, 102, NULL, NULL, },
  {  308, "Medicham", TYPE_FIGHTING, TYPE_PSYCHIC, 121, 152, 155, NULL, NULL, },
  {  308, "Mega Medicham", TYPE_FIGHTING, TYPE_PSYCHIC, 205, 179, 155, NULL, NULL, },
  {  309, "Electrike", TYPE_ELECTRIC, TYPECOUNT, 123, 78, 120, NULL, NULL, },
  {  310, "Manectric", TYPE_ELECTRIC, TYPECOUNT, 215, 127, 172, NULL, NULL, },
  {  310, "Mega Manectric", TYPE_ELECTRIC, TYPECOUNT, 286, 179, 172, NULL, NULL, },
  {  311, "Plusle", TYPE_ELECTRIC, TYPECOUNT, 167, 129, 155, NULL, NULL, },
  {  312, "Minun", TYPE_ELECTRIC, TYPECOUNT, 147, 150, 155, NULL, NULL, },
  {  313, "Volbeat", TYPE_BUG, TYPECOUNT, 143, 166, 163, NULL, NULL, },
  {  314, "Illumise", TYPE_BUG, TYPECOUNT, 143, 166, 163, NULL, NULL, },
  {  315, "Roselia", TYPE_GRASS, TYPE_POISON, 186, 131, 137, NULL, NULL, },
  {  316, "Gulpin", TYPE_POISON, TYPECOUNT, 80, 99, 172, NULL, NULL, },
  {  317, "Swalot", TYPE_POISON, TYPECOUNT, 140, 159, 225, NULL, NULL, },
  {  318, "Carvanha", TYPE_WATER, TYPE_DARK, 171, 39, 128, NULL, NULL, },
  {  319, "Sharpedo", TYPE_WATER, TYPE_DARK, 243, 83, 172, NULL, NULL, },
  {  320, "Wailmer", TYPE_WATER, TYPECOUNT, 136, 68, 277, NULL, NULL, },
  {  321, "Wailord", TYPE_WATER, TYPECOUNT, 175, 87, 347, NULL, NULL, },
  {  322, "Numel", TYPE_FIRE, TYPE_GROUND, 119, 79, 155, NULL, NULL, },
  {  323, "Camerupt", TYPE_FIRE, TYPE_GROUND, 194, 136, 172, NULL, NULL, },
  {  324, "Torkoal", TYPE_FIRE, TYPECOUNT, 151, 203, 172, NULL, NULL, },
  {  325, "Spoink", TYPE_PSYCHIC, TYPECOUNT, 125, 122, 155, NULL, NULL, },
  {  326, "Grumpig", TYPE_PSYCHIC, TYPECOUNT, 171, 188, 190, NULL, NULL, },
  {  327, "Spinda", TYPE_NORMAL, TYPECOUNT, 116, 116, 155, NULL, NULL, },
  {  328, "Trapinch", TYPE_GROUND, TYPECOUNT, 162, 78, 128, NULL, NULL, },
  {  329, "Vibrava", TYPE_GROUND, TYPE_DRAGON, 134, 99, 137, NULL, NULL, },
  {  330, "Flygon", TYPE_GROUND, TYPE_DRAGON, 205, 168, 190, NULL, NULL, },
  {  331, "Cacnea", TYPE_GRASS, TYPECOUNT, 156, 74, 137, NULL, NULL, },
  {  332, "Cacturne", TYPE_GRASS, TYPE_DARK, 221, 115, 172, NULL, NULL, },
  {  333, "Swablu", TYPE_NORMAL, TYPE_FLYING, 76, 132, 128, NULL, NULL, },
  {  334, "Altaria", TYPE_DRAGON, TYPE_FLYING, 141, 201, 181, NULL, NULL, },
  {  334, "Mega Altaria", TYPE_DRAGON, TYPE_FAIRY, 222, 218, 181, NULL, NULL, },
  {  335, "Zangoose", TYPE_NORMAL, TYPECOUNT, 222, 124, 177, NULL, NULL, },
  {  336, "Seviper", TYPE_POISON, TYPECOUNT, 196, 118, 177, NULL, NULL, },
  {  337, "Lunatone", TYPE_ROCK, TYPE_PSYCHIC, 178, 153, 207, NULL, NULL, },
  {  338, "Solrock", TYPE_ROCK, TYPE_PSYCHIC, 178, 153, 207, NULL, NULL, },
  {  339, "Barboach", TYPE_WATER, TYPE_GROUND, 93, 82, 137, NULL, NULL, },
  {  340, "Whiscash", TYPE_WATER, TYPE_GROUND, 151, 141, 242, NULL, NULL, },
  {  341, "Corphish", TYPE_WATER, TYPECOUNT, 141, 99, 125, NULL, NULL, },
  {  342, "Crawdaunt", TYPE_WATER, TYPE_DARK, 224, 142, 160, NULL, NULL, },
  {  343, "Baltoy", TYPE_GROUND, TYPE_PSYCHIC, 77, 124, 120, NULL, NULL, },
  {  344, "Claydol", TYPE_GROUND, TYPE_PSYCHIC, 140, 229, 155, NULL, NULL, },
  {  345, "Lileep", TYPE_ROCK, TYPE_GRASS, 105, 150, 165, NULL, NULL, },
  {  346, "Cradily", TYPE_ROCK, TYPE_GRASS, 152, 194, 200, NULL, NULL, },
  {  347, "Anorith", TYPE_ROCK, TYPE_BUG, 176, 100, 128, NULL, NULL, },
  {  348, "Armaldo", TYPE_ROCK, TYPE_BUG, 222, 174, 181, NULL, NULL, },
  {  349, "Feebas", TYPE_WATER, TYPECOUNT, 29, 85, 85, NULL, NULL, },
  {  350, "Milotic", TYPE_WATER, TYPECOUNT, 192, 219, 216, NULL, NULL, },
  {  351, "Castform", TYPE_NORMAL, TYPECOUNT, 139, 139, 172, NULL, NULL, },
  {  351, "Castform", TYPE_FIRE, TYPECOUNT, 139, 139, 172, NULL, NULL, },
  {  351, "Castform", TYPE_WATER, TYPECOUNT, 139, 139, 172, NULL, NULL, },
  {  351, "Castform", TYPE_ICE, TYPECOUNT, 139, 139, 172, NULL, NULL, },
  {  352, "Kecleon", TYPE_NORMAL, TYPECOUNT, 161, 189, 155, NULL, NULL, },
  {  353, "Shuppet", TYPE_GHOST, TYPECOUNT, 138, 65, 127, NULL, NULL, },
  {  354, "Banette", TYPE_GHOST, TYPECOUNT, 218, 126, 162, NULL, NULL, },
  {  354, "Mega Banette", TYPE_GHOST, TYPECOUNT, 312, 160, 162, NULL, NULL, },
  {  355, "Duskull", TYPE_GHOST, TYPECOUNT, 70, 162, 85, NULL, NULL, },
  {  356, "Dusclops", TYPE_GHOST, TYPECOUNT, 124, 234, 120, NULL, NULL, },
  {  357, "Tropius", TYPE_GRASS, TYPE_FLYING, 136, 163, 223, NULL, NULL, },
  {  358, "Chimecho", TYPE_PSYCHIC, TYPECOUNT, 175, 170, 181, NULL, NULL, },
  {  359, "Absol", TYPE_DARK, TYPECOUNT, 246, 120, 163, NULL, NULL, },
  {  359, "Mega Absol", TYPE_DARK, TYPECOUNT, 314, 130, 163, NULL, NULL, },
  {  360, "Wynaut", TYPE_PSYCHIC, TYPECOUNT, 41, 86, 216, NULL, NULL, },
  {  361, "Snorunt", TYPE_ICE, TYPECOUNT, 95, 95, 137, NULL, NULL, },
  {  362, "Glalie", TYPE_ICE, TYPECOUNT, 162, 162, 190, NULL, NULL, },
  {  362, "Mega Glalie", TYPE_ICE, TYPECOUNT, 252, 168, 190, NULL, NULL, },
  {  363, "Spheal", TYPE_ICE, TYPE_WATER, 95, 90, 172, NULL, NULL, },
  {  364, "Sealeo", TYPE_ICE, TYPE_WATER, 137, 132, 207, NULL, NULL, },
  {  365, "Walrein", TYPE_ICE, TYPE_WATER, 182, 176, 242, NULL, NULL, },
  {  366, "Clamperl", TYPE_WATER, TYPECOUNT, 133, 135, 111, NULL, NULL, },
  {  367, "Huntail", TYPE_WATER, TYPECOUNT, 197, 179, 146, NULL, NULL, },
  {  368, "Gorebyss", TYPE_WATER, TYPECOUNT, 211, 179, 146, NULL, NULL, },
  {  369, "Relicanth", TYPE_WATER, TYPE_ROCK, 162, 203, 225, NULL, NULL, },
  {  370, "Luvdisc", TYPE_WATER, TYPECOUNT, 81, 128, 125, NULL, NULL, },
  {  371, "Bagon", TYPE_DRAGON, TYPECOUNT, 134, 93, 128, NULL, NULL, },
  {  372, "Shelgon", TYPE_DRAGON, TYPECOUNT, 172, 155, 163, NULL, NULL, },
  {  373, "Salamence", TYPE_DRAGON, TYPE_FLYING, 277, 168, 216, NULL, NULL, },
  {  373, "Mega Salamence", TYPE_DRAGON, TYPE_FLYING, 310, 251, 216, NULL, NULL, },
  {  374, "Beldum", TYPE_STEEL, TYPE_PSYCHIC, 96, 132, 120, NULL, NULL, },
  {  375, "Metang", TYPE_STEEL, TYPE_PSYCHIC, 138, 176, 155, NULL, NULL, },
  {  376, "Metagross", TYPE_STEEL, TYPE_PSYCHIC, 257, 228, 190, NULL, NULL, },
  {  377, "Regirock", TYPE_ROCK, TYPECOUNT, 179, 309, 190, NULL, NULL, },
  {  378, "Regice", TYPE_ICE, TYPECOUNT, 179, 309, 190, NULL, NULL, },
  {  379, "Registeel", TYPE_STEEL, TYPECOUNT, 143, 285, 190, NULL, NULL, },
  {  380, "Latias", TYPE_DRAGON, TYPE_PSYCHIC, 228, 246, 190, NULL, NULL, },
  {  380, "Mega Latias", TYPE_DRAGON, TYPE_PSYCHIC, 289, 297, 190, NULL, NULL, },
  {  381, "Latios", TYPE_DRAGON, TYPE_PSYCHIC, 268, 212, 190, NULL, NULL, },
  {  381, "Mega Latios", TYPE_DRAGON, TYPE_PSYCHIC, 335, 241, 190, NULL, NULL, },
  {  382, "Kyogre", TYPE_WATER, TYPECOUNT, 270, 228, 205, NULL, NULL, },
  {  382, "Primal Kyogre", TYPE_WATER, TYPECOUNT, 353, 268, 218, NULL, NULL, },
  {  383, "Groudon", TYPE_GROUND, TYPECOUNT, 270, 228, 205, NULL, NULL, },
  {  383, "Primal Groudon", TYPE_GROUND, TYPE_FIRE, 353, 268, 218, NULL, NULL, },
  {  384, "Rayquaza", TYPE_DRAGON, TYPE_FLYING, 284, 170, 213, NULL, NULL, },
  {  384, "Mega Rayquaza", TYPE_DRAGON, TYPE_FLYING, 377, 210, 227, NULL, NULL, },
  {  385, "Jirachi", TYPE_STEEL, TYPE_PSYCHIC, 210, 210, 225, NULL, NULL, },
  {  386, "Deoxys", TYPE_PSYCHIC, TYPECOUNT, 345, 115, 137, NULL, NULL, },
  {  386, "Deoxys Attack", TYPE_PSYCHIC, TYPECOUNT, 414, 46, 137, NULL, NULL, },
  {  386, "Deoxys Defense", TYPE_PSYCHIC, TYPECOUNT, 144, 330, 137, NULL, NULL, },
  {  386, "Deoxys Speed", TYPE_PSYCHIC, TYPECOUNT, 230, 218, 137, NULL, NULL, },
  {  387, "Turtwig", TYPE_GRASS, TYPECOUNT, 119, 110, 146, NULL, NULL, },
  {  388, "Grotle", TYPE_GRASS, TYPECOUNT, 157, 143, 181, NULL, NULL, },
  {  389, "Torterra", TYPE_GRASS, TYPE_GROUND, 202, 188, 216, NULL, NULL, },
  {  390, "Chimchar", TYPE_FIRE, TYPECOUNT, 113, 86, 127, NULL, NULL, },
  {  391, "Monferno", TYPE_FIRE, TYPE_FIGHTING, 158, 105, 162, NULL, NULL, },
  {  392, "Infernape", TYPE_FIRE, TYPE_FIGHTING, 222, 151, 183, NULL, NULL, },
  {  393, "Piplup", TYPE_WATER, TYPECOUNT, 112, 102, 142, NULL, NULL, },
  {  394, "Prinplup", TYPE_WATER, TYPECOUNT, 150, 139, 162, NULL, NULL, },
  {  395, "Empoleon", TYPE_WATER, TYPE_STEEL, 210, 186, 197, NULL, NULL, },
  {  396, "Starly", TYPE_NORMAL, TYPE_FLYING, 101, 58, 120, NULL, NULL, },
  {  397, "Staravia", TYPE_NORMAL, TYPE_FLYING, 142, 94, 146, NULL, NULL, },
  {  398, "Staraptor", TYPE_NORMAL, TYPE_FLYING, 234, 140, 198, NULL, NULL, },
  {  399, "Bidoof", TYPE_NORMAL, TYPECOUNT, 80, 73, 153, NULL, NULL, },
  {  400, "Bibarel", TYPE_NORMAL, TYPE_WATER, 162, 119, 188, NULL, NULL, },
  {  401, "Kricketot", TYPE_BUG, TYPECOUNT, 45, 74, 114, NULL, NULL, },
  {  402, "Kricketune", TYPE_BUG, TYPECOUNT, 160, 100, 184, NULL, NULL, },
  {  403, "Shinx", TYPE_ELECTRIC, TYPECOUNT, 117, 64, 128, NULL, NULL, },
  {  404, "Luxio", TYPE_ELECTRIC, TYPECOUNT, 159, 95, 155, NULL, NULL, },
  {  405, "Luxray", TYPE_ELECTRIC, TYPECOUNT, 232, 156, 190, NULL, NULL, },
  {  406, "Budew", TYPE_GRASS, TYPE_POISON, 91, 109, 120, NULL, NULL, },
  {  407, "Roserade", TYPE_GRASS, TYPE_POISON, 243, 185, 155, NULL, NULL, },
  {  408, "Cranidos", TYPE_ROCK, TYPECOUNT, 218, 71, 167, NULL, NULL, },
  {  409, "Rampardos", TYPE_ROCK, TYPECOUNT, 295, 109, 219, NULL, NULL, },
  {  410, "Shieldon", TYPE_ROCK, TYPE_STEEL, 76, 195, 102, NULL, NULL, },
  {  411, "Bastiodon", TYPE_ROCK, TYPE_STEEL, 94, 286, 155, NULL, NULL, },
  {  412, "Burmy", TYPE_BUG, TYPECOUNT, 53, 83, 120, NULL, NULL, },
  {  413, "Plant Wormadam", TYPE_BUG, TYPE_GRASS, 141, 180, 155, NULL, NULL, },
  {  413, "Sandy Wormadam", TYPE_BUG, TYPE_GROUND, 141, 180, 155, NULL, NULL, },
  {  413, "Trash Wormadam", TYPE_BUG, TYPE_STEEL, 127, 175, 155, NULL, NULL, },
  {  414, "Mothim", TYPE_BUG, TYPE_FLYING, 185, 98, 172, NULL, NULL, },
  {  415, "Combee", TYPE_BUG, TYPE_FLYING, 59, 83, 102, NULL, NULL, },
  {  416, "Vespiquen", TYPE_BUG, TYPE_FLYING, 149, 190, 172, NULL, NULL, },
  {  417, "Pachirisu", TYPE_ELECTRIC, TYPECOUNT, 94, 172, 155, NULL, NULL, },
  {  418, "Buizel", TYPE_WATER, TYPECOUNT, 132, 67, 146, NULL, NULL, },
  {  419, "Floatzel", TYPE_WATER, TYPECOUNT, 221, 114, 198, NULL, NULL, },
  {  420, "Cherubi", TYPE_GRASS, TYPECOUNT, 108, 92, 128, NULL, NULL, },
  {  421, "Cherrim", TYPE_GRASS, TYPECOUNT, 170, 153, 172, NULL, NULL, },
  {  422, "Shellos", TYPE_WATER, TYPECOUNT, 103, 105, 183, NULL, NULL, },
  {  423, "Gastrodon", TYPE_WATER, TYPE_GROUND, 169, 143, 244, NULL, NULL, },
  {  424, "Ambipom", TYPE_NORMAL, TYPECOUNT, 205, 143, 181, NULL, NULL, },
  {  425, "Drifloon", TYPE_GHOST, TYPE_FLYING, 117, 80, 207, NULL, NULL, },
  {  426, "Drifblim", TYPE_GHOST, TYPE_FLYING, 180, 102, 312, NULL, NULL, },
  {  427, "Buneary", TYPE_NORMAL, TYPECOUNT, 130, 105, 146, NULL, NULL, },
  {  428, "Lopunny", TYPE_NORMAL, TYPECOUNT, 156, 194, 163, NULL, NULL, },
  {  428, "Mega Lopunny", TYPE_NORMAL, TYPE_FIGHTING, 282, 214, 163, NULL, NULL, },
  {  429, "Mismagius", TYPE_GHOST, TYPECOUNT, 211, 187, 155, NULL, NULL, },
  {  430, "Honchkrow", TYPE_DARK, TYPE_FLYING, 243, 103, 225, NULL, NULL, },
  {  431, "Glameow", TYPE_NORMAL, TYPECOUNT, 109, 82, 135, NULL, NULL, },
  {  432, "Purugly", TYPE_NORMAL, TYPECOUNT, 172, 133, 174, NULL, NULL, },
  {  433, "Chingling", TYPE_PSYCHIC, TYPECOUNT, 114, 94, 128, NULL, NULL, },
  {  434, "Stunky", TYPE_POISON, TYPE_DARK, 121, 90, 160, NULL, NULL, },
  {  435, "Skuntank", TYPE_POISON, TYPE_DARK, 184, 132, 230, NULL, NULL, },
  {  436, "Bronzor", TYPE_STEEL, TYPE_PSYCHIC, 43, 154, 149, NULL, NULL, },
  {  437, "Bronzong", TYPE_STEEL, TYPE_PSYCHIC, 161, 213, 167, NULL, NULL, },
  {  438, "Bonsly", TYPE_ROCK, TYPECOUNT, 124, 133, 137, NULL, NULL, },
  {  439, "Mime Jr.", TYPE_PSYCHIC, TYPE_FAIRY, 125, 142, 85, NULL, NULL, },
  {  440, "Happiny", TYPE_NORMAL, TYPECOUNT, 25, 77, 225, NULL, NULL, },
  {  441, "Chatot", TYPE_NORMAL, TYPE_FLYING, 183, 91, 183, NULL, NULL, },
  {  442, "Spiritomb", TYPE_GHOST, TYPE_DARK, 169, 199, 137, NULL, NULL, },
  {  443, "Gible", TYPE_DRAGON, TYPE_GROUND, 124, 84, 151, NULL, NULL, },
  {  444, "Gabite", TYPE_DRAGON, TYPE_GROUND, 172, 125, 169, NULL, NULL, },
  {  445, "Garchomp", TYPE_DRAGON, TYPE_GROUND, 261, 193, 239, NULL, NULL, },
  {  445, "Mega Garchomp", TYPE_DRAGON, TYPE_GROUND, 339, 222, 239, NULL, NULL, },
  {  446, "Munchlax", TYPE_NORMAL, TYPECOUNT, 137, 117, 286, NULL, NULL, },
  {  447, "Riolu", TYPE_FIGHTING, TYPECOUNT, 127, 78, 120, NULL, NULL, },
  {  448, "Lucario", TYPE_FIGHTING, TYPE_STEEL, 236, 144, 172, NULL, NULL, },
  {  448, "Mega Lucario", TYPE_FIGHTING, TYPE_STEEL, 310, 175, 172, NULL, NULL, },
  {  449, "Hippopotas", TYPE_GROUND, TYPECOUNT, 124, 118, 169, NULL, NULL, },
  {  450, "Hippowdon", TYPE_GROUND, TYPECOUNT, 201, 191, 239, NULL, NULL, },
  {  451, "Skorupi", TYPE_POISON, TYPE_BUG, 93, 151, 120, NULL, NULL, },
  {  452, "Drapion", TYPE_POISON, TYPE_DARK, 180, 202, 172, NULL, NULL, },
  {  453, "Croagunk", TYPE_POISON, TYPE_FIGHTING, 116, 76, 134, NULL, NULL, },
  {  454, "Toxicroak", TYPE_POISON, TYPE_FIGHTING, 211, 133, 195, NULL, NULL, },
  {  455, "Carnivine", TYPE_GRASS, TYPECOUNT, 187, 136, 179, NULL, NULL, },
  {  456, "Finneon", TYPE_WATER, TYPECOUNT, 96, 116, 135, NULL, NULL, },
  {  457, "Lumineon", TYPE_WATER, TYPECOUNT, 142, 170, 170, NULL, NULL, },
  {  458, "Mantyke", TYPE_WATER, TYPE_FLYING, 105, 179, 128, NULL, NULL, },
  {  459, "Snover", TYPE_GRASS, TYPE_ICE, 115, 105, 155, NULL, NULL, },
  {  460, "Abomasnow", TYPE_GRASS, TYPE_ICE, 178, 158, 207, NULL, NULL, },
  {  460, "Mega Abomasnow", TYPE_GRASS, TYPE_ICE, 240, 191, 207, NULL, NULL, },
  {  461, "Weavile", TYPE_DARK, TYPE_ICE, 243, 171, 172, NULL, NULL, },
  {  462, "Magnezone", TYPE_ELECTRIC, TYPE_STEEL, 238, 205, 172, NULL, NULL, },
  {  463, "Lickilicky", TYPE_NORMAL, TYPECOUNT, 161, 181, 242, NULL, NULL, },
  {  464, "Rhyperior", TYPE_GROUND, TYPE_ROCK, 241, 190, 251, NULL, NULL, },
  {  465, "Tangrowth", TYPE_GRASS, TYPECOUNT, 207, 184, 225, NULL, NULL, },
  {  466, "Electivire", TYPE_ELECTRIC, TYPECOUNT, 249, 163, 181, NULL, NULL, },
  {  467, "Magmortar", TYPE_FIRE, TYPECOUNT, 247, 172, 181, NULL, NULL, },
  {  468, "Togekiss", TYPE_FAIRY, TYPE_FLYING, 225, 217, 198, NULL, NULL, },
  {  469, "Yanmega", TYPE_BUG, TYPE_FLYING, 231, 156, 200, NULL, NULL, },
  {  470, "Leafeon", TYPE_GRASS, TYPECOUNT, 216, 219, 163, NULL, NULL, },
  {  471, "Glaceon", TYPE_ICE, TYPECOUNT, 238, 205, 163, NULL, NULL, },
  {  472, "Gliscor", TYPE_GROUND, TYPE_FLYING, 185, 222, 181, NULL, NULL, },
  {  473, "Mamoswine", TYPE_ICE, TYPE_GROUND, 247, 146, 242, NULL, NULL, },
  {  474, "Porygon-Z", TYPE_NORMAL, TYPECOUNT, 264, 150, 198, NULL, NULL, },
  {  475, "Gallade", TYPE_PSYCHIC, TYPE_FIGHTING, 237, 195, 169, NULL, NULL, },
  {  475, "Mega Gallade", TYPE_PSYCHIC, TYPE_FIGHTING, 326, 230, 169, NULL, NULL, },
  {  476, "Probopass", TYPE_ROCK, TYPE_STEEL, 135, 275, 155, NULL, NULL, },
  {  477, "Dusknoir", TYPE_GHOST, TYPECOUNT, 180, 254, 128, NULL, NULL, },
  {  478, "Froslass", TYPE_ICE, TYPE_GHOST, 171, 150, 172, NULL, NULL, },
  {  479, "Rotom", TYPE_ELECTRIC, TYPE_GHOST, 185, 159, 137, NULL, NULL, },
  {  479, "Heat Rotom", TYPE_ELECTRIC, TYPE_FIRE, 204, 219, 137, NULL, NULL, },
  {  479, "Wash Rotom", TYPE_ELECTRIC, TYPE_WATER, 204, 219, 137, NULL, NULL, },
  {  479, "Frost Rotom", TYPE_ELECTRIC, TYPE_ICE, 204, 219, 137, NULL, NULL, },
  {  479, "Fan Rotom", TYPE_ELECTRIC, TYPE_FLYING, 204, 219, 137, NULL, NULL, },
  {  479, "Mow Rotom", TYPE_ELECTRIC, TYPE_GRASS, 204, 219, 137, NULL, NULL, },
  {  480, "Uxie", TYPE_PSYCHIC, TYPECOUNT, 156, 270, 181, NULL, NULL, },
  {  481, "Mesprit", TYPE_PSYCHIC, TYPECOUNT, 212, 212, 190, NULL, NULL, },
  {  482, "Azelf", TYPE_PSYCHIC, TYPECOUNT, 270, 151, 181, NULL, NULL, },
  {  483, "Dialga", TYPE_STEEL, TYPE_DRAGON, 275, 211, 205, NULL, NULL, },
  {  483, "Origin Forme Dialga", TYPE_STEEL, TYPE_DRAGON, 270, 225, 205, NULL, NULL, },
  {  484, "Palkia", TYPE_WATER, TYPE_DRAGON, 280, 215, 189, NULL, NULL, },
  {  484, "Origin Forme Palkia", TYPE_WATER, TYPE_DRAGON, 286, 223, 189, NULL, NULL, },
  {  485, "Heatran", TYPE_FIRE, TYPE_STEEL, 251, 213, 209, NULL, NULL, },
  {  486, "Regigigas", TYPE_NORMAL, TYPECOUNT, 287, 210, 221, NULL, NULL, },
  {  487, "Altered Forme Giratina", TYPE_GHOST, TYPE_DRAGON, 187, 225, 284, NULL, NULL, },
  {  487, "Origin Forme Giratina", TYPE_GHOST, TYPE_DRAGON, 225, 187, 284, NULL, NULL, },
  {  488, "Cresselia", TYPE_PSYCHIC, TYPECOUNT, 152, 258, 260, NULL, NULL, },
  {  491, "Darkrai", TYPE_DARK, TYPECOUNT, 285, 198, 172, NULL, NULL, },
  {  492, "Shaymin", TYPE_GRASS, TYPECOUNT, 210, 210, 225, NULL, NULL, },
  {  492, "Sky Shaymin", TYPE_GRASS, TYPE_FLYING, 261, 166, 225, NULL, NULL, },
  {  494, "Victini", TYPE_PSYCHIC, TYPE_FIRE, 210, 210, 225, NULL, NULL, },
  {  495, "Snivy", TYPE_GRASS, TYPECOUNT, 88, 107, 128, NULL, NULL, },
  {  496, "Servine", TYPE_GRASS, TYPECOUNT, 122, 152, 155, NULL, NULL, },
  {  497, "Serperior", TYPE_GRASS, TYPECOUNT, 161, 204, 181, NULL, NULL, },
  {  498, "Tepig", TYPE_FIRE, TYPECOUNT, 115, 85, 163, NULL, NULL, },
  {  499, "Pignite", TYPE_FIRE, TYPE_FIGHTING, 173, 106, 207, NULL, NULL, },
  {  500, "Emboar", TYPE_FIRE, TYPE_FIGHTING, 235, 127, 242, NULL, NULL, },
  {  501, "Oshawott", TYPE_WATER, TYPECOUNT, 117, 85, 146, NULL, NULL, },
  {  502, "Dewott", TYPE_WATER, TYPECOUNT, 159, 116, 181, NULL, NULL, },
  {  503, "Samurott", TYPE_WATER, TYPECOUNT, 212, 157, 216, NULL, NULL, },
  {  503, "Hisuian Samurott", TYPE_WATER, TYPE_DARK, 218, 152, 207, NULL, NULL, },
  {  504, "Patrat", TYPE_NORMAL, TYPECOUNT, 98, 73, 128, NULL, NULL, },
  {  505, "Watchog", TYPE_NORMAL, TYPECOUNT, 165, 139, 155, NULL, NULL, },
  {  506, "Lillipup", TYPE_NORMAL, TYPECOUNT, 107, 86, 128, NULL, NULL, },
  {  507, "Herdier", TYPE_NORMAL, TYPECOUNT, 145, 126, 163, NULL, NULL, },
  {  508, "Stoutland", TYPE_NORMAL, TYPECOUNT, 206, 182, 198, NULL, NULL, },
  {  509, "Purrloin", TYPE_DARK, TYPECOUNT, 98, 73, 121, NULL, NULL, },
  {  510, "Liepard", TYPE_DARK, TYPECOUNT, 187, 106, 162, NULL, NULL, },
  {  511, "Pansage", TYPE_GRASS, TYPECOUNT, 104, 94, 137, NULL, NULL, },
  {  512, "Simisage", TYPE_GRASS, TYPECOUNT, 206, 133, 181, NULL, NULL, },
  {  513, "Pansear", TYPE_FIRE, TYPECOUNT, 104, 94, 137, NULL, NULL, },
  {  514, "Simisear", TYPE_FIRE, TYPECOUNT, 206, 133, 181, NULL, NULL, },
  {  515, "Panpour", TYPE_WATER, TYPECOUNT, 104, 94, 137, NULL, NULL, },
  {  516, "Simipour", TYPE_WATER, TYPECOUNT, 206, 133, 181, NULL, NULL, },
  {  517, "Munna", TYPE_PSYCHIC, TYPECOUNT, 111, 92, 183, NULL, NULL, },
  {  518, "Musharna", TYPE_PSYCHIC, TYPECOUNT, 183, 166, 253, NULL, NULL, },
  {  519, "Pidove", TYPE_NORMAL, TYPE_FLYING, 98, 80, 137, NULL, NULL, },
  {  520, "Tranquill", TYPE_NORMAL, TYPE_FLYING, 144, 107, 158, NULL, NULL, },
  {  521, "Unfezant", TYPE_NORMAL, TYPE_FLYING, 226, 146, 190, NULL, NULL, },
  {  522, "Blitzle", TYPE_ELECTRIC, TYPECOUNT, 118, 64, 128, NULL, NULL, },
  {  523, "Zebstrika", TYPE_ELECTRIC, TYPECOUNT, 211, 136, 181, NULL, NULL, },
  {  524, "Roggenrola", TYPE_ROCK, TYPECOUNT, 121, 110, 146, NULL, NULL, },
  {  525, "Boldore", TYPE_ROCK, TYPECOUNT, 174, 143, 172, NULL, NULL, },
  {  526, "Gigalith", TYPE_ROCK, TYPECOUNT, 226, 201, 198, NULL, NULL, },
  {  527, "Woobat", TYPE_PSYCHIC, TYPE_FLYING, 107, 85, 163, NULL, NULL, },
  {  528, "Swoobat", TYPE_PSYCHIC, TYPE_FLYING, 161, 119, 167, NULL, NULL, },
  {  529, "Drilbur", TYPE_GROUND, TYPECOUNT, 154, 85, 155, NULL, NULL, },
  {  530, "Excadrill", TYPE_GROUND, TYPE_STEEL, 255, 129, 242, NULL, NULL, },
  {  531, "Audino", TYPE_NORMAL, TYPECOUNT, 114, 163, 230, NULL, NULL, },
  {  531, "Mega Audino", TYPE_NORMAL, TYPE_FAIRY, 147, 239, 230, NULL, NULL, },
  {  532, "Timburr", TYPE_FIGHTING, TYPECOUNT, 134, 87, 181, NULL, NULL, },
  {  533, "Gurdurr", TYPE_FIGHTING, TYPECOUNT, 180, 134, 198, NULL, NULL, },
  {  534, "Conkeldurr", TYPE_FIGHTING, TYPECOUNT, 243, 158, 233, NULL, NULL, },
  {  535, "Tympole", TYPE_WATER, TYPECOUNT, 98, 78, 137, NULL, NULL, },
  {  536, "Palpitoad", TYPE_WATER, TYPE_GROUND, 128, 109, 181, NULL, NULL, },
  {  537, "Seismitoad", TYPE_WATER, TYPE_GROUND, 188, 150, 233, NULL, NULL, },
  {  538, "Throh", TYPE_FIGHTING, TYPECOUNT, 172, 160, 260, NULL, NULL, },
  {  539, "Sawk", TYPE_FIGHTING, TYPECOUNT, 231, 153, 181, NULL, NULL, },
  {  540, "Sewaddle", TYPE_BUG, TYPE_GRASS, 96, 124, 128, NULL, NULL, },
  {  541, "Swadloon", TYPE_BUG, TYPE_GRASS, 115, 162, 146, NULL, NULL, },
  {  542, "Leavanny", TYPE_BUG, TYPE_GRASS, 205, 165, 181, NULL, NULL, },
  {  543, "Venipede", TYPE_BUG, TYPE_POISON, 83, 99, 102, NULL, NULL, },
  {  544, "Whirlipede", TYPE_BUG, TYPE_POISON, 100, 173, 120, NULL, NULL, },
  {  545, "Scolipede", TYPE_BUG, TYPE_POISON, 203, 175, 155, NULL, NULL, },
  {  546, "Cottonee", TYPE_GRASS, TYPE_FAIRY, 71, 111, 120, NULL, NULL, },
  {  547, "Whimsicott", TYPE_GRASS, TYPE_FAIRY, 164, 176, 155, NULL, NULL, },
  {  548, "Petilil", TYPE_GRASS, TYPECOUNT, 119, 91, 128, NULL, NULL, },
  {  549, "Lilligant", TYPE_GRASS, TYPECOUNT, 214, 155, 172, NULL, NULL, },
  {  550, "Basculin", TYPE_WATER, TYPECOUNT, 189, 129, 172, NULL, NULL, },
  {  551, "Sandile", TYPE_GROUND, TYPE_DARK, 132, 69, 137, NULL, NULL, },
  {  552, "Krokorok", TYPE_GROUND, TYPE_DARK, 155, 90, 155, NULL, NULL, },
  {  553, "Krookodile", TYPE_GROUND, TYPE_DARK, 229, 158, 216, NULL, NULL, },
  {  554, "Darumaka", TYPE_FIRE, TYPECOUNT, 153, 86, 172, NULL, NULL, },
  {  554, "Galarian Darumaka", TYPE_ICE, TYPECOUNT, 153, 86, 172, NULL, NULL, },
  {  555, "Darmanitan", TYPE_FIRE, TYPECOUNT, 263, 114, 233, NULL, NULL, },
  {  555, "Galarian Darmanitan", TYPE_ICE, TYPECOUNT, 263, 114, 233, NULL, NULL, },
  {  556, "Maractus", TYPE_GRASS, TYPECOUNT, 201, 130, 181, NULL, NULL, },
  {  557, "Dwebble", TYPE_BUG, TYPE_ROCK, 118, 128, 137, NULL, NULL, },
  {  558, "Crustle", TYPE_BUG, TYPE_ROCK, 188, 200, 172, NULL, NULL, },
  {  559, "Scraggy", TYPE_DARK, TYPE_FIGHTING, 132, 132, 137, NULL, NULL, },
  {  560, "Scrafty", TYPE_DARK, TYPE_FIGHTING, 163, 222, 163, NULL, NULL, },
  {  561, "Sigilyph", TYPE_PSYCHIC, TYPE_FLYING, 204, 167, 176, NULL, NULL, },
  {  562, "Yamask", TYPE_GHOST, TYPECOUNT, 95, 141, 116, NULL, NULL, },
  {  562, "Galarian Yamask", TYPE_GROUND, TYPE_GHOST, 95, 141, 116, NULL, NULL, },
  {  563, "Cofagrigus", TYPE_GHOST, TYPECOUNT, 163, 237, 151, NULL, NULL, },
  {  564, "Tirtouga", TYPE_WATER, TYPE_ROCK, 134, 146, 144, NULL, NULL, },
  {  565, "Carracosta", TYPE_WATER, TYPE_ROCK, 192, 197, 179, NULL, NULL, },
  {  566, "Archen", TYPE_ROCK, TYPE_FLYING, 213, 89, 146, NULL, NULL, },
  {  567, "Archeops", TYPE_ROCK, TYPE_FLYING, 292, 139, 181, NULL, NULL, },
  {  568, "Trubbish", TYPE_POISON, TYPECOUNT, 96, 122, 137, NULL, NULL, },
  {  569, "Garbodor", TYPE_POISON, TYPECOUNT, 181, 164, 190, NULL, NULL, },
  {  570, "Zorua", TYPE_DARK, TYPECOUNT, 153, 78, 120, NULL, NULL, },
  {  571, "Zoroark", TYPE_DARK, TYPECOUNT, 250, 127, 155, NULL, NULL, },
  {  572, "Minccino", TYPE_NORMAL, TYPECOUNT, 98, 80, 146, NULL, NULL, },
  {  573, "Cinccino", TYPE_NORMAL, TYPECOUNT, 198, 130, 181, NULL, NULL, },
  {  574, "Gothita", TYPE_PSYCHIC, TYPECOUNT, 98, 112, 128, NULL, NULL, },
  {  575, "Gothorita", TYPE_PSYCHIC, TYPECOUNT, 137, 153, 155, NULL, NULL, },
  {  576, "Gothitelle", TYPE_PSYCHIC, TYPECOUNT, 176, 205, 172, NULL, NULL, },
  {  577, "Solosis", TYPE_PSYCHIC, TYPECOUNT, 170, 83, 128, NULL, NULL, },
  {  578, "Duosion", TYPE_PSYCHIC, TYPECOUNT, 208, 103, 163, NULL, NULL, },
  {  579, "Reuniclus", TYPE_PSYCHIC, TYPECOUNT, 214, 148, 242, NULL, NULL, },
  {  580, "Ducklett", TYPE_WATER, TYPE_FLYING, 84, 96, 158, NULL, NULL, },
  {  581, "Swanna", TYPE_WATER, TYPE_FLYING, 182, 132, 181, NULL, NULL, },
  {  582, "Vanillite", TYPE_ICE, TYPECOUNT, 118, 106, 113, NULL, NULL, },
  {  583, "Vanillish", TYPE_ICE, TYPECOUNT, 151, 138, 139, NULL, NULL, },
  {  584, "Vanilluxe", TYPE_ICE, TYPECOUNT, 218, 184, 174, NULL, NULL, },
  {  585, "Deerling", TYPE_NORMAL, TYPE_GRASS, 115, 100, 155, NULL, NULL, },
  {  586, "Sawsbuck", TYPE_NORMAL, TYPE_GRASS, 198, 146, 190, NULL, NULL, },
  {  587, "Emolga", TYPE_ELECTRIC, TYPE_FLYING, 158, 127, 146, NULL, NULL, },
  {  588, "Karrablast", TYPE_BUG, TYPECOUNT, 137, 87, 137, NULL, NULL, },
  {  589, "Escavalier", TYPE_BUG, TYPE_STEEL, 223, 187, 172, NULL, NULL, },
  {  590, "Foongus", TYPE_GRASS, TYPE_POISON, 97, 91, 170, NULL, NULL, },
  {  591, "Amoonguss", TYPE_GRASS, TYPE_POISON, 155, 139, 249, NULL, NULL, },
  {  592, "Frillish", TYPE_WATER, TYPE_GHOST, 115, 134, 146, NULL, NULL, },
  {  593, "Jellicent", TYPE_WATER, TYPE_GHOST, 159, 178, 225, NULL, NULL, },
  {  594, "Alomomola", TYPE_WATER, TYPECOUNT, 138, 131, 338, NULL, NULL, },
  {  595, "Joltik", TYPE_BUG, TYPE_ELECTRIC, 110, 98, 137, NULL, NULL, },
  {  596, "Galvantula", TYPE_BUG, TYPE_ELECTRIC, 201, 128, 172, NULL, NULL, },
  {  597, "Ferroseed", TYPE_GRASS, TYPE_STEEL, 82, 155, 127, NULL, NULL, },
  {  598, "Ferrothorn", TYPE_GRASS, TYPE_STEEL, 158, 223, 179, NULL, NULL, },
  {  599, "Klink", TYPE_STEEL, TYPECOUNT, 98, 121, 120, NULL, NULL, },
  {  600, "Klang", TYPE_STEEL, TYPECOUNT, 150, 174, 155, NULL, NULL, },
  {  601, "Klinklang", TYPE_STEEL, TYPECOUNT, 199, 214, 155, NULL, NULL, },
  {  602, "Tynamo", TYPE_ELECTRIC, TYPECOUNT, 105, 78, 111, NULL, NULL, },
  {  603, "Eelektrik", TYPE_ELECTRIC, TYPECOUNT, 156, 130, 163, NULL, NULL, },
  {  604, "Eelektross", TYPE_ELECTRIC, TYPECOUNT, 217, 152, 198, NULL, NULL, },
  {  605, "Elgyem", TYPE_PSYCHIC, TYPECOUNT, 148, 100, 146, NULL, NULL, },
  {  606, "Beheeyem", TYPE_PSYCHIC, TYPECOUNT, 221, 163, 181, NULL, NULL, },
  {  607, "Litwick", TYPE_GHOST, TYPE_FIRE, 108, 98, 137, NULL, NULL, },
  {  608, "Lampent", TYPE_GHOST, TYPE_FIRE, 169, 115, 155, NULL, NULL, },
  {  609, "Chandelure", TYPE_GHOST, TYPE_FIRE, 271, 182, 155, NULL, NULL, },
  {  610, "Axew", TYPE_DRAGON, TYPECOUNT, 154, 101, 130, NULL, NULL, },
  {  611, "Fraxure", TYPE_DRAGON, TYPECOUNT, 212, 123, 165, NULL, NULL, },
  {  612, "Haxorus", TYPE_DRAGON, TYPECOUNT, 284, 172, 183, NULL, NULL, },
  {  613, "Cubchoo", TYPE_ICE, TYPECOUNT, 128, 74, 146, NULL, NULL, },
  {  614, "Beartic", TYPE_ICE, TYPECOUNT, 233, 152, 216, NULL, NULL, },
  {  615, "Cryogonal", TYPE_ICE, TYPECOUNT, 190, 218, 190, NULL, NULL, },
  {  616, "Shelmet", TYPE_BUG, TYPECOUNT, 72, 140, 137, NULL, NULL, },
  {  617, "Accelgor", TYPE_BUG, TYPECOUNT, 220, 120, 190, NULL, NULL, },
  {  618, "Stunfisk", TYPE_GROUND, TYPE_ELECTRIC, 144, 171, 240, NULL, NULL, },
  {  618, "Galarian Stunfisk", TYPE_GROUND, TYPE_STEEL, 144, 171, 240, NULL, NULL, },
  {  619, "Mienfoo", TYPE_FIGHTING, TYPECOUNT, 160, 98, 128, NULL, NULL, },
  {  620, "Mienshao", TYPE_FIGHTING, TYPECOUNT, 258, 127, 163, NULL, NULL, },
  {  621, "Druddigon", TYPE_DRAGON, TYPECOUNT, 213, 170, 184, NULL, NULL, },
  {  622, "Golett", TYPE_GROUND, TYPE_GHOST, 127, 92, 153, NULL, NULL, },
  {  623, "Golurk", TYPE_GROUND, TYPE_GHOST, 222, 154, 205, NULL, NULL, },
  {  624, "Pawniard", TYPE_DARK, TYPE_STEEL, 154, 114, 128, NULL, NULL, },
  {  625, "Bisharp", TYPE_DARK, TYPE_STEEL, 232, 176, 163, NULL, NULL, },
  {  626, "Bouffalant", TYPE_NORMAL, TYPECOUNT, 195, 182, 216, NULL, NULL, },
  {  627, "Rufflet", TYPE_NORMAL, TYPE_FLYING, 150, 97, 172, NULL, NULL, },
  {  628, "Braviary", TYPE_NORMAL, TYPE_FLYING, 232, 152, 225, NULL, NULL, },
  {  628, "Hisuian Braviary", TYPE_PSYCHIC, TYPE_FLYING, 213, 137, 242, NULL, NULL, },
  {  629, "Vullaby", TYPE_DARK, TYPE_FLYING, 105, 139, 172, NULL, NULL, },
  {  630, "Mandibuzz", TYPE_DARK, TYPE_FLYING, 129, 205, 242, NULL, NULL, },
  {  631, "Heatmor", TYPE_FIRE, TYPECOUNT, 204, 129, 198, NULL, NULL, },
  {  632, "Durant", TYPE_BUG, TYPE_STEEL, 217, 188, 151, NULL, NULL, },
  {  633, "Deino", TYPE_DARK, TYPE_DRAGON, 116, 93, 141, NULL, NULL, },
  {  634, "Zweilous", TYPE_DARK, TYPE_DRAGON, 159, 135, 176, NULL, NULL, },
  {  635, "Hydreigon", TYPE_DARK, TYPE_DRAGON, 256, 188, 211, NULL, NULL, },
  {  636, "Larvesta", TYPE_BUG, TYPE_FIRE, 156, 107, 146, NULL, NULL, },
  {  637, "Volcarona", TYPE_BUG, TYPE_FIRE, 264, 189, 198, NULL, NULL, },
  {  638, "Cobalion", TYPE_STEEL, TYPE_FIGHTING, 192, 229, 209, NULL, NULL, },
  {  639, "Terrakion", TYPE_ROCK, TYPE_FIGHTING, 260, 192, 209, NULL, NULL, },
  {  640, "Virizion", TYPE_GRASS, TYPE_FIGHTING, 192, 229, 209, NULL, NULL, },
  {  641, "Incarnate Forme Tornadus", TYPE_FLYING, TYPECOUNT, 266, 164, 188, NULL, NULL, },
  {  641, "Therian Forme Tornadus", TYPE_FLYING, TYPECOUNT, 238, 189, 188, NULL, NULL, },
  {  642, "Incarnate Forme Thundurus", TYPE_ELECTRIC, TYPE_FLYING, 266, 164, 188, NULL, NULL, },
  {  642, "Therian Forme Thundurus", TYPE_ELECTRIC, TYPE_FLYING, 295, 161, 188, NULL, NULL, },
  {  643, "Reshiram", TYPE_DRAGON, TYPE_FIRE, 275, 211, 205, NULL, NULL, },
  {  644, "Zekrom", TYPE_DRAGON, TYPE_ELECTRIC, 275, 211, 205, NULL, NULL, },
  {  645, "Incarnate Forme Landorus", TYPE_GROUND, TYPE_FLYING, 261, 182, 205, NULL, NULL, },
  {  645, "Therian Forme Landorus", TYPE_GROUND, TYPE_FLYING, 289, 179, 205, NULL, NULL, },
  {  646, "Kyurem", TYPE_DRAGON, TYPE_ICE, 246, 170, 245, NULL, NULL, },
  {  646, "White Kyurem", TYPE_DRAGON, TYPE_ICE, 310, 183, 245, NULL, NULL, },
  {  646, "Black Kyurem", TYPE_DRAGON, TYPE_ICE, 310, 183, 245, NULL, NULL, },
  {  647, "Keldeo", TYPE_WATER, TYPE_FIGHTING, 260, 192, 209, NULL, NULL, },
  {  648, "Meloetta", TYPE_NORMAL, TYPE_PSYCHIC, 250, 225, 225, NULL, NULL, },
  {  649, "Genesect", TYPE_BUG, TYPE_STEEL, 252, 199, 174, NULL, NULL, },
  {  650, "Chespin", TYPE_GRASS, TYPECOUNT, 110, 106, 148, NULL, NULL, },
  {  651, "Quilladin", TYPE_GRASS, TYPECOUNT, 146, 156, 156, NULL, NULL, },
  {  652, "Chesnaught", TYPE_GRASS, TYPE_FIGHTING, 201, 204, 204, NULL, NULL, },
  {  653, "Fennekin", TYPE_FIRE, TYPECOUNT, 116, 102, 120, NULL, NULL, },
  {  654, "Braixen", TYPE_FIRE, TYPECOUNT, 171, 130, 153, NULL, NULL, },
  {  655, "Delphox", TYPE_FIRE, TYPE_PSYCHIC, 230, 189, 181, NULL, NULL, },
  {  656, "Froakie", TYPE_WATER, TYPECOUNT, 122, 84, 121, NULL, NULL, },
  {  657, "Frogadier", TYPE_WATER, TYPECOUNT, 168, 114, 144, NULL, NULL, },
  {  658, "Greninja", TYPE_WATER, TYPE_DARK, 223, 152, 176, NULL, NULL, },
  {  659, "Bunnelby", TYPE_NORMAL, TYPECOUNT, 68, 72, 116, NULL, NULL, },
  {  660, "Diggersby", TYPE_NORMAL, TYPE_GROUND, 112, 155, 198, NULL, NULL, },
  {  661, "Fletchling", TYPE_NORMAL, TYPE_FLYING, 95, 80, 128, NULL, NULL, },
  {  662, "Fletchinder", TYPE_FIRE, TYPE_FLYING, 145, 110, 158, NULL, NULL, },
  {  663, "Talonflame", TYPE_FIRE, TYPE_FLYING, 176, 155, 186, NULL, NULL, },
  {  664, "Scatterbug", TYPE_BUG, TYPECOUNT, 63, 63, 116, NULL, NULL, },
  {  665, "Spewpa", TYPE_BUG, TYPECOUNT, 48, 89, 128, NULL, NULL, },
  {  666, "Vivillon", TYPE_BUG, TYPE_FLYING, 176, 103, 190, NULL, NULL, },
  {  667, "Litleo", TYPE_FIRE, TYPE_NORMAL, 139, 112, 158, NULL, NULL, },
  {  668, "Pyroar", TYPE_FIRE, TYPE_NORMAL, 221, 149, 200, NULL, NULL, },
  {  669, "Flabébé", TYPE_FAIRY, TYPECOUNT, 108, 120, 127, NULL, NULL, },
  {  670, "Floette", TYPE_FAIRY, TYPECOUNT, 136, 151, 144, NULL, NULL, },
  {  671, "Florges", TYPE_FAIRY, TYPECOUNT, 212, 244, 186, NULL, NULL, },
  {  672, "Skiddo", TYPE_GRASS, TYPECOUNT, 123, 102, 165, NULL, NULL, },
  {  673, "Gogoat", TYPE_GRASS, TYPECOUNT, 196, 146, 265, NULL, NULL, },
  {  674, "Pancham", TYPE_FIGHTING, TYPECOUNT, 145, 107, 167, NULL, NULL, },
  {  675, "Pangoro", TYPE_FIGHTING, TYPE_DARK, 226, 146, 216, NULL, NULL, },
  {  676, "Furfrou", TYPE_NORMAL, TYPECOUNT, 164, 167, 181, NULL, NULL, },
  {  677, "Espurr", TYPE_PSYCHIC, TYPECOUNT, 120, 114, 158, NULL, NULL, },
  {  678, "Meowstic", TYPE_PSYCHIC, TYPECOUNT, 166, 167, 179, NULL, NULL, },
  {  682, "Spritzee", TYPE_FAIRY, TYPECOUNT, 110, 113, 186, NULL, NULL, },
  {  683, "Aromatisse", TYPE_FAIRY, TYPECOUNT, 173, 150, 226, NULL, NULL, },
  {  684, "Swirlix", TYPE_FAIRY, TYPECOUNT, 109, 119, 158, NULL, NULL, },
  {  685, "Slurpuff", TYPE_FAIRY, TYPECOUNT, 168, 163, 193, NULL, NULL, },
  {  686, "Inkay", TYPE_DARK, TYPE_PSYCHIC, 98, 95, 142, NULL, NULL, },
  {  687, "Malamar", TYPE_DARK, TYPE_PSYCHIC, 177, 165, 200, NULL, NULL, },
  {  688, "Binacle", TYPE_ROCK, TYPE_WATER, 96, 120, 123, NULL, NULL, },
  {  689, "Barbaracle", TYPE_ROCK, TYPE_WATER, 194, 205, 176, NULL, NULL, },
  {  690, "Skrelp", TYPE_POISON, TYPE_WATER, 109, 109, 137, NULL, NULL, },
  {  691, "Dragalge", TYPE_POISON, TYPE_DRAGON, 177, 207, 163, NULL, NULL, },
  {  692, "Clauncher", TYPE_WATER, TYPECOUNT, 108, 117, 137, NULL, NULL, },
  {  693, "Clawitzer", TYPE_WATER, TYPECOUNT, 221, 171, 174, NULL, NULL, },
  {  694, "Helioptile", TYPE_ELECTRIC, TYPE_NORMAL, 115, 78, 127, NULL, NULL, },
  {  695, "Heliolisk", TYPE_ELECTRIC, TYPE_NORMAL, 219, 168, 158, NULL, NULL, },
  {  696, "Tyrunt", TYPE_ROCK, TYPE_DRAGON, 158, 123, 151, NULL, NULL, },
  {  697, "Tyrantrum", TYPE_ROCK, TYPE_DRAGON, 227, 191, 193, NULL, NULL, },
  {  698, "Amaura", TYPE_ROCK, TYPE_ICE, 124, 109, 184, NULL, NULL, },
  {  699, "Aurorus", TYPE_ROCK, TYPE_ICE, 186, 163, 265, NULL, NULL, },
  {  700, "Sylveon", TYPE_FAIRY, TYPECOUNT, 203, 205, 216, NULL, NULL, },
  {  701, "Hawlucha", TYPE_FIGHTING, TYPE_FLYING, 195, 153, 186, NULL, NULL, },
  {  702, "Dedenne", TYPE_ELECTRIC, TYPE_FAIRY, 164, 134, 167, NULL, NULL, },
  {  703, "Carbink", TYPE_ROCK, TYPE_FAIRY, 95, 285, 137, NULL, NULL, },
  {  704, "Goomy", TYPE_DRAGON, TYPECOUNT, 101, 112, 128, NULL, NULL, },
  {  705, "Sliggoo", TYPE_DRAGON, TYPECOUNT, 159, 176, 169, NULL, NULL, },
  {  706, "Goodra", TYPE_DRAGON, TYPECOUNT, 220, 242, 207, NULL, NULL, },
  {  707, "Klefki", TYPE_STEEL, TYPE_FAIRY, 160, 179, 149, NULL, NULL, },
  {  708, "Phantump", TYPE_GHOST, TYPE_GRASS, 125, 103, 125, NULL, NULL, },
  {  709, "Trevenant", TYPE_GHOST, TYPE_GRASS, 201, 154, 198, NULL, NULL, },
  {  710, "Pumpkaboo", TYPE_GHOST, TYPE_GRASS, 121, 123, 135, NULL, NULL, },
  {  710, "Pumpkaboo Small", TYPE_GHOST, TYPE_GRASS, 122, 124, 127, NULL, NULL, },
  {  710, "Pumpkaboo Large", TYPE_GHOST, TYPE_GRASS, 120, 122, 144, NULL, NULL, },
  {  710, "Pumpkaboo Super", TYPE_GHOST, TYPE_GRASS, 118, 120, 153, NULL, NULL, },
  {  711, "Gourgeist", TYPE_GHOST, TYPE_GRASS, 175, 213, 163, NULL, NULL, },
  {  711, "Gourgeist Small", TYPE_GHOST, TYPE_GRASS, 171, 219, 146, NULL, NULL, },
  {  711, "Gourgeist Large", TYPE_GHOST, TYPE_GRASS, 179, 206, 181, NULL, NULL, },
  {  711, "Gourgeist Super", TYPE_GHOST, TYPE_GRASS, 182, 200, 198, NULL, NULL, },
  {  712, "Bergmite", TYPE_ICE, TYPECOUNT, 117, 120, 146, NULL, NULL, },
  {  713, "Avalugg", TYPE_ICE, TYPECOUNT, 196, 240, 216, NULL, NULL, },
  {  713, "Hisuian Avalugg", TYPE_ICE, TYPE_ROCK, 214, 238, 216, NULL, NULL, },
  {  714, "Noibat", TYPE_FLYING, TYPE_DRAGON, 83, 73, 120, NULL, NULL, },
  {  715, "Noivern", TYPE_FLYING, TYPE_DRAGON, 205, 175, 198, NULL, NULL, },
  {  716, "Xerneas", TYPE_FAIRY, TYPECOUNT, 250, 185, 246, NULL, NULL, },
  {  717, "Yveltal", TYPE_DARK, TYPE_FLYING, 250, 185, 246, NULL, NULL, },
  {  718, "Zygarde 10%", TYPE_DRAGON, TYPE_GROUND, 205, 173, 144, NULL, NULL, },
  {  718, "Zygarde 50%", TYPE_DRAGON, TYPE_GROUND, 203, 232, 239, NULL, NULL, },
  {  718, "Zygarde Complete", TYPE_DRAGON, TYPE_GROUND, 184, 207, 389, NULL, NULL, },
  {  719, "Diancie", TYPE_ROCK, TYPE_FAIRY, 190, 285, 137, NULL, NULL, },
  {  719, "Mega Diancie", TYPE_ROCK, TYPE_FAIRY, 342, 235, 137, NULL, NULL, },
  {  720, "Hoopa Confined", TYPE_PSYCHIC, TYPE_GHOST, 261, 187, 173, NULL, NULL, },
  {  720, "Hoopa Unbound", TYPE_PSYCHIC, TYPE_DARK, 311, 191, 173, NULL, NULL, },
  {  722, "Rowlet", TYPE_GRASS, TYPE_FLYING, 102, 99, 169, NULL, NULL, },
  {  723, "Dartrix", TYPE_GRASS, TYPE_FLYING, 142, 139, 186, NULL, NULL, },
  {  724, "Decidueye", TYPE_GRASS, TYPE_GHOST, 210, 179, 186, NULL, NULL, },
  {  724, "Hisuian Decidueye", TYPE_GRASS, TYPE_FIGHTING, 213, 174, 204, NULL, NULL, },
  {  725, "Litten", TYPE_FIRE, TYPECOUNT, 128, 79, 128, NULL, NULL, },
  {  726, "Torracat", TYPE_FIRE, TYPECOUNT, 174, 103, 163, NULL, NULL, },
  {  727, "Incineroar", TYPE_FIRE, TYPE_DARK, 214, 175, 216, NULL, NULL, },
  {  728, "Popplio", TYPE_WATER, TYPECOUNT, 120, 103, 137, NULL, NULL, },
  {  729, "Brionne", TYPE_WATER, TYPECOUNT, 168, 145, 155, NULL, NULL, },
  {  730, "Primarina", TYPE_WATER, TYPE_FAIRY, 232, 195, 190, NULL, NULL, },
  {  731, "Pikipek", TYPE_NORMAL, TYPE_FLYING, 136, 59, 111, NULL, NULL, },
  {  732, "Trumbeak", TYPE_NORMAL, TYPE_FLYING, 159, 100, 146, NULL, NULL, },
  {  733, "Toucannon", TYPE_NORMAL, TYPE_FLYING, 222, 146, 190, NULL, NULL, },
  {  734, "Yungoos", TYPE_NORMAL, TYPECOUNT, 122, 56, 134, NULL, NULL, },
  {  735, "Gumshoos", TYPE_NORMAL, TYPECOUNT, 194, 113, 204, NULL, NULL, },
  {  736, "Grubbin", TYPE_BUG, TYPECOUNT, 115, 85, 132, NULL, NULL, },
  {  737, "Charjabug", TYPE_BUG, TYPE_ELECTRIC, 145, 161, 149, NULL, NULL, },
  {  738, "Vikavolt", TYPE_BUG, TYPE_ELECTRIC, 254, 158, 184, NULL, NULL, },
  {  739, "Crabrawler", TYPE_FIGHTING, TYPECOUNT, 150, 104, 132, NULL, NULL, },
  {  740, "Crabominable", TYPE_FIGHTING, TYPE_ICE, 231, 138, 219, NULL, NULL, },
  {  741, "Oricorio", TYPE_FIRE, TYPE_FLYING, 196, 145, 181, NULL, NULL, },
  {  742, "Cutiefly", TYPE_BUG, TYPE_FAIRY, 110, 81, 120, NULL, NULL, },
  {  743, "Ribombee", TYPE_BUG, TYPE_FAIRY, 198, 146, 155, NULL, NULL, },
  {  744, "Rockruff", TYPE_ROCK, TYPECOUNT, 117, 78, 128, NULL, NULL, },
  {  745, "Lycanric Midday", TYPE_ROCK, TYPECOUNT, 231, 140, 181, NULL, NULL, },
  {  745, "Lycanroc Midnight", TYPE_ROCK, TYPECOUNT, 218, 152, 198, NULL, NULL, },
  {  745, "Lycanroc Dusk", TYPE_ROCK, TYPECOUNT, 234, 139, 181, NULL, NULL, },
  {  747, "Mareanie", TYPE_POISON, TYPE_WATER, 98, 110, 137, NULL, NULL, },
  {  748, "Toxapex", TYPE_POISON, TYPE_WATER, 114, 273, 137, NULL, NULL, },
  {  749, "Mudbray", TYPE_GROUND, TYPECOUNT, 175, 121, 172, NULL, NULL, },
  {  750, "Mudsdale", TYPE_GROUND, TYPECOUNT, 214, 174, 225, NULL, NULL, },
  {  751, "Dewpider", TYPE_WATER, TYPE_BUG, 72, 117, 116, NULL, NULL, },
  {  752, "Araquanid", TYPE_WATER, TYPE_BUG, 126, 219, 169, NULL, NULL, },
  {  753, "Fomantis", TYPE_GRASS, TYPECOUNT, 100, 64, 120, NULL, NULL, },
  {  754, "Lurantis", TYPE_GRASS, TYPECOUNT, 192, 169, 172, NULL, NULL, },
  {  755, "Morelull", TYPE_GRASS, TYPE_FAIRY, 108, 119, 120, NULL, NULL, },
  {  756, "Shiinotic", TYPE_GRASS, TYPE_FAIRY, 154, 168, 155, NULL, NULL, },
  {  757, "Salandit", TYPE_POISON, TYPE_FIRE, 136, 80, 134, NULL, NULL, },
  {  758, "Salazzle", TYPE_POISON, TYPE_FIRE, 228, 130, 169, NULL, NULL, },
  {  759, "Stufful", TYPE_NORMAL, TYPE_FIGHTING, 136, 95, 172, NULL, NULL, },
  {  760, "Bewear", TYPE_NORMAL, TYPE_FIGHTING, 226, 141, 260, NULL, NULL, },
  {  761, "Bounsweet", TYPE_GRASS, TYPECOUNT, 55, 69, 123, NULL, NULL, },
  {  762, "Steenee", TYPE_GRASS, TYPECOUNT, 78, 94, 141, NULL, NULL, },
  {  763, "Tsareena", TYPE_GRASS, TYPECOUNT, 222, 195, 176, NULL, NULL, },
  {  764, "Comfey", TYPE_FAIRY, TYPECOUNT, 165, 215, 139, NULL, NULL, },
  {  765, "Oranguru", TYPE_NORMAL, TYPE_PSYCHIC, 168, 192, 207, NULL, NULL, },
  {  766, "Passimian", TYPE_FIGHTING, TYPECOUNT, 222, 160, 225, NULL, NULL, },
  {  767, "Wimpod", TYPE_BUG, TYPE_WATER, 67, 74, 93, NULL, NULL, },
  {  768, "Golisopod", TYPE_BUG, TYPE_WATER, 218, 226, 181, NULL, NULL, },
  {  769, "Sandygast", TYPE_GHOST, TYPE_GROUND, 120, 118, 146, NULL, NULL, },
  {  770, "Palossand", TYPE_GHOST, TYPE_GROUND, 178, 178, 198, NULL, NULL, },
  {  775, "Komala", TYPE_NORMAL, TYPECOUNT, 216, 165, 163, NULL, NULL, },
  {  776, "Turtonator", TYPE_FIRE, TYPE_DRAGON, 165, 215, 155, NULL, NULL, },
  {  777, "Togedemaru", TYPE_ELECTRIC, TYPE_STEEL, 190, 145, 163, NULL, NULL, },
  {  779, "Bruxish", TYPE_WATER, TYPE_PSYCHIC, 208, 145, 169, NULL, NULL, },
  {  780, "Drampa", TYPE_NORMAL, TYPE_DRAGON, 231, 164, 186, NULL, NULL, },
  {  781, "Dhelmise", TYPE_GHOST, TYPE_GRASS, 233, 179, 172, NULL, NULL, },
  {  782, "Jangmo-o", TYPE_DRAGON, TYPECOUNT, 102, 108, 128, NULL, NULL, },
  {  783, "Hakamo-o", TYPE_DRAGON, TYPE_FIGHTING, 145, 162, 146, NULL, NULL, },
  {  784, "Kommo-o", TYPE_DRAGON, TYPE_FIGHTING, 222, 240, 181, NULL, NULL, },
  {  785, "Tapu Koko", TYPE_ELECTRIC, TYPE_FAIRY, 250, 181, 172, NULL, NULL, },
  {  786, "Tapu Lele", TYPE_PSYCHIC, TYPE_FAIRY, 259, 208, 172, NULL, NULL, },
  {  787, "Tapu Bulu", TYPE_GRASS, TYPE_FAIRY, 249, 215, 172, NULL, NULL, },
  {  788, "Tapu Fini", TYPE_WATER, TYPE_FAIRY, 189, 254, 172, NULL, NULL, },
  {  789, "Cosmog", TYPE_PSYCHIC, TYPECOUNT, 54, 57, 125, NULL, NULL, },
  {  790, "Cosmoem", TYPE_PSYCHIC, TYPECOUNT, 54, 242, 125, NULL, NULL, },
  {  791, "Solgaleo", TYPE_PSYCHIC, TYPE_STEEL, 255, 191, 264, NULL, NULL, },
  {  792, "Lunala", TYPE_PSYCHIC, TYPE_GHOST, 255, 191, 264, NULL, NULL, },
  {  793, "Nihilego", TYPE_ROCK, TYPE_POISON, 249, 210, 240, NULL, NULL, },
  {  794, "Buzzwole", TYPE_BUG, TYPE_FIGHTING, 236, 196, 216, NULL, NULL, },
  {  795, "Pheromosa", TYPE_BUG, TYPE_FIGHTING, 316, 85, 174, NULL, NULL, },
  {  796, "Xurkitree", TYPE_ELECTRIC, TYPECOUNT, 330, 144, 195, NULL, NULL, },
  {  797, "Celesteela", TYPE_STEEL, TYPE_FLYING, 207, 199, 219, NULL, NULL, },
  {  798, "Kartana", TYPE_GRASS, TYPE_STEEL, 323, 182, 139, NULL, NULL, },
  {  799, "Guzzlord", TYPE_DARK, TYPE_DRAGON, 188, 99, 440, NULL, NULL, },
  {  800, "Necrozma", TYPE_PSYCHIC, TYPECOUNT, 251, 195, 219, NULL, NULL, },
  {  800, "Necrozma Dusk Mane", TYPE_PSYCHIC, TYPE_STEEL, 277, 220, 200, NULL, NULL, },
  {  800, "Necrozma Dawn Wings", TYPE_PSYCHIC, TYPE_GHOST, 277, 220, 200, NULL, NULL, },
  {  802, "Marshadow", TYPE_FIGHTING, TYPE_GHOST, 265, 190, 207, NULL, NULL, },
  {  803, "Poipole", TYPE_POISON, TYPECOUNT, 145, 133, 167, NULL, NULL, },
  {  804, "Naganadel", TYPE_POISON, TYPE_DRAGON, 263, 159, 177, NULL, NULL, },
  {  805, "Stakataka", TYPE_ROCK, TYPE_STEEL, 213, 298, 156, NULL, NULL, },
  {  806, "Blacephalon", TYPE_FIRE, TYPE_GHOST, 315, 148, 142, NULL, NULL, },
  {  808, "Meltan", TYPE_STEEL, TYPECOUNT, 118, 99, 130, NULL, NULL, },
  {  809, "Melmetal", TYPE_STEEL, TYPECOUNT, 226, 190, 264, NULL, NULL, },
  {  810, "Grookey", TYPE_GRASS, TYPECOUNT, 122, 91, 137, NULL, NULL, },
  {  811, "Thwackey", TYPE_GRASS, TYPECOUNT, 165, 134, 172, NULL, NULL, },
  {  812, "Rillaboom", TYPE_GRASS, TYPECOUNT, 239, 168, 225, NULL, NULL, },
  {  813, "Scorbunny", TYPE_FIRE, TYPECOUNT, 132, 79, 137, NULL, NULL, },
  {  814, "Raboot", TYPE_FIRE, TYPECOUNT, 170, 125, 163, NULL, NULL, },
  {  815, "Cinderace", TYPE_FIRE, TYPECOUNT, 238, 163, 190, NULL, NULL, },
  {  816, "Sobble", TYPE_WATER, TYPECOUNT, 132, 79, 137, NULL, NULL, },
  {  817, "Drizzile", TYPE_WATER, TYPECOUNT, 186, 113, 163, NULL, NULL, },
  {  818, "Inteleon", TYPE_WATER, TYPECOUNT, 262, 142, 172, NULL, NULL, },
  {  819, "Skwovet", TYPE_NORMAL, TYPECOUNT, 95, 86, 172, NULL, NULL, },
  {  820, "Greedent", TYPE_NORMAL, TYPECOUNT, 160, 156, 260, NULL, NULL, },
  {  821, "Rookidee", TYPE_FLYING, TYPECOUNT, 88, 67, 116, NULL, NULL, },
  {  822, "Corvisquire", TYPE_FLYING, TYPECOUNT, 129, 110, 169, NULL, NULL, },
  {  823, "Corviknight", TYPE_FLYING, TYPE_STEEL, 163, 192, 221, NULL, NULL, },
  {  827, "Nickit", TYPE_DARK, TYPECOUNT, 85, 82, 120, NULL, NULL, },
  {  828, "Thievul", TYPE_DARK, TYPECOUNT, 172, 164, 172, NULL, NULL, },
  {  829, "Gossifleur", TYPE_GRASS, TYPECOUNT, 70, 104, 120, NULL, NULL, },
  {  830, "Eldegoss", TYPE_GRASS, TYPECOUNT, 148, 211, 155, NULL, NULL, },
  {  831, "Wooloo", TYPE_NORMAL, TYPECOUNT, 76, 97, 123, NULL, NULL, },
  {  832, "Dubwool", TYPE_NORMAL, TYPECOUNT, 159, 198, 176, NULL, NULL, },
  {  835, "Yamper", TYPE_ELECTRIC, TYPECOUNT, 80, 90, 153, NULL, NULL, },
  {  836, "Boltund",TYPE_ELECTRIC, TYPECOUNT, 197, 131, 170, NULL, NULL, },
  {  840, "Applin", TYPE_GRASS, TYPE_DRAGON, 71, 116, 120, NULL, NULL, },
  {  841, "Flapple", TYPE_GRASS, TYPE_DRAGON, 214, 144, 172, NULL, NULL, },
  {  842, "Appletun", TYPE_GRASS, TYPE_DRAGON, 178, 146, 242, NULL, NULL, },
  {  848, "Toxel", TYPE_ELECTRIC, TYPE_POISON, 97, 65, 120, NULL, NULL, },
  {  849, "Toxtricity", TYPE_ELECTRIC, TYPE_POISON, 224, 140, 181, NULL, NULL, },
  {  850, "Sizzlipede", TYPE_FIRE, TYPE_BUG, 118, 90, 137, NULL, NULL, },
  {  851, "Centiskorch", TYPE_FIRE, TYPE_BUG, 220, 158, 225, NULL, NULL, },
  {  854, "Sinistea", TYPE_GHOST, TYPECOUNT, 134, 96, 120, NULL, NULL, },
  {  855, "Polteageist", TYPE_GHOST, TYPECOUNT, 248, 189, 155, NULL, NULL, },
  {  856, "Hatenna", TYPE_PSYCHIC, TYPECOUNT, 98, 93, 123, NULL, NULL, },
  {  857, "Hattrem", TYPE_PSYCHIC, TYPECOUNT, 153, 133, 149, NULL, NULL, },
  {  858, "Hatterene", TYPE_PSYCHIC, TYPE_FAIRY, 237, 182, 149, NULL, NULL, },
  {  862, "Obstagoon", TYPE_DARK, TYPE_NORMAL, 180, 194, 212, NULL, NULL, },
  {  863, "Perrserker", TYPE_STEEL, TYPECOUNT, 195, 162, 172, NULL, NULL, },
  {  864, "Cursola", TYPE_GHOST, TYPECOUNT, 253, 182, 155, NULL, NULL, },
  {  865, "Sirfetch'd", TYPE_FIGHTING, TYPECOUNT, 248, 176, 158, NULL, NULL, },
  {  866, "Mr. Rime", TYPE_ICE, TYPE_PSYCHIC, 212, 179, 190, NULL, NULL, },
  {  867, "Runerigus", TYPE_GROUND, TYPE_GHOST, 163, 237, 151, NULL, NULL, },
  {  870, "Falinks", TYPE_FIGHTING, TYPECOUNT, 193, 170, 163, NULL, NULL, },
  {  874, "Stonjourner", TYPE_ROCK, TYPECOUNT, 222, 182, 225, NULL, NULL, },
  {  877, "Morpeko", TYPE_ELECTRIC, TYPE_DARK, 192, 121, 151, NULL, NULL, },
  {  885, "Dreepy", TYPE_DRAGON, TYPE_GHOST, 117, 61, 99, NULL, NULL, },
  {  886, "Drakloak", TYPE_DRAGON, TYPE_GHOST, 163, 105, 169, NULL, NULL, },
  {  887, "Dragapult", TYPE_DRAGON, TYPE_GHOST, 266, 170, 204, NULL, NULL, },
  {  888, "Zacian", TYPE_FAIRY, TYPECOUNT, 254, 236, 192, NULL, NULL, },
  {  889, "Zamazenta", TYPE_FIGHTING, TYPECOUNT, 254, 236, 192, NULL, NULL, },
  {  891, "Kubfu", TYPE_FIGHTING, TYPECOUNT, 170, 112, 155, NULL, NULL, },
  {  892, "Single Strike Style Urshifu", TYPE_FIGHTING, TYPE_DARK, 254, 177, 225, NULL, NULL, },
  {  892, "Rapid Strike Style Urshifu", TYPE_FIGHTING, TYPE_WATER, 254, 177, 225, NULL, NULL, },
  {  893, "Zarude", TYPE_DARK, TYPE_GRASS, 242, 215, 233, NULL, NULL, },
  {  894, "Regieleki", TYPE_ELECTRIC, TYPECOUNT, 250, 125, 190, NULL, NULL, },
  {  895, "Regidrago", TYPE_DRAGON, TYPECOUNT, 202, 101, 400, NULL, NULL, },
  {  899, "Wyrdeer", TYPE_NORMAL, TYPE_PSYCHIC, 206, 145, 230, NULL, NULL, },
  {  900, "Kleavor", TYPE_BUG, TYPE_ROCK, 253, 174, 172, NULL, NULL, },
  {  901, "Ursaluna", TYPE_GROUND, TYPE_NORMAL, 243, 181, 277, NULL, NULL, },
  {  903, "Sneasler", TYPE_FIGHTING, TYPE_POISON, 259, 158, 190, NULL, NULL, },
  {  904, "Overqwil", TYPE_DARK, TYPE_POISON, 222, 171, 198, NULL, NULL, },
  {  905, "Incarnate Forme Enamorus", TYPE_FAIRY, TYPE_FLYING, 281, 162, 179, NULL, NULL, },
  {  906, "Sprigatito", TYPE_GRASS, TYPECOUNT, 116, 99, 120, NULL, NULL, },
  {  907, "Floragato", TYPE_GRASS, TYPECOUNT, 157, 128, 156, NULL, NULL, },
  {  908, "Meowscarada", TYPE_GRASS, TYPE_DARK, 233, 153, 183, NULL, NULL, },
  {  909, "Fuecoco", TYPE_FIRE, TYPECOUNT, 112, 96, 167, NULL, NULL, },
  {  910, "Crocalor", TYPE_FIRE, TYPECOUNT, 162, 134, 191, NULL, NULL, },
  {  911, "Skeledirge", TYPE_FIRE, TYPE_GHOST, 207, 178, 232, NULL, NULL, },
  {  912, "Quaxly", TYPE_WATER, TYPECOUNT, 120, 86, 146, NULL, NULL, },
  {  913, "Quaxwell", TYPE_WATER, TYPECOUNT, 162, 123, 172, NULL, NULL, },
  {  914, "Quaquaval", TYPE_WATER, TYPE_FIGHTING, 236, 159, 198, NULL, NULL, },
  {  915, "Lechonk", TYPE_NORMAL, TYPECOUNT, 81, 79, 144, NULL, NULL, },
  {  916, "Male Oinkologne", TYPE_NORMAL, TYPECOUNT, 186, 153, 242, NULL, NULL, },
  {  916, "Female Oinkologne", TYPE_NORMAL, TYPECOUNT, 169, 162, 251, NULL, NULL, },
  {  919, "Nymble", TYPE_BUG, TYPECOUNT, 81, 65, 107, NULL, NULL, },
  {  920, "Lokix", TYPE_BUG, TYPE_DARK, 199, 144, 174, NULL, NULL, },
  {  921, "Pawmi", TYPE_ELECTRIC, TYPECOUNT, 95, 45, 128, NULL, NULL, },
  {  922, "Pawmo", TYPE_ELECTRIC, TYPE_FIGHTING, 147, 82, 155, NULL, NULL, },
  {  923, "Pawmot", TYPE_ELECTRIC, TYPE_FIGHTING, 232, 141, 172, NULL, NULL, },
  {  924, "Tandemaus", TYPE_NORMAL, TYPECOUNT, 98, 90, 137, NULL, NULL, },
  {  925, "Maushold", TYPE_NORMAL, TYPECOUNT, 159, 157, 179, NULL, NULL, },
  {  926, "Fidough", TYPE_FAIRY, TYPECOUNT, 102, 126, 114, NULL, NULL, },
  {  927, "Dachsbun", TYPE_FAIRY, TYPECOUNT, 159, 212, 149, NULL, NULL, },
  {  928, "Smoliv", TYPE_GRASS, TYPE_NORMAL, 100, 89, 121, NULL, NULL, },
  {  929, "Dolliv", TYPE_GRASS, TYPE_NORMAL, 137, 131, 141, NULL, NULL, },
  {  930, "Arboliva", TYPE_GRASS, TYPE_NORMAL, 219, 189, 186, NULL, NULL, },
  {  935, "Charcadet", TYPE_FIRE, TYPECOUNT, 92, 74, 120, NULL, NULL, },
  {  936, "Armarouge", TYPE_FIRE, TYPE_PSYCHIC, 234, 185, 198, NULL, NULL, },
  {  937, "Ceruledge", TYPE_FIRE, TYPE_GHOST, 239, 189, 181, NULL, NULL, },
  {  938, "Tadbulb", TYPE_ELECTRIC, TYPECOUNT, 104, 73, 156, NULL, NULL, },
  {  939, "Bellibolt", TYPE_ELECTRIC, TYPECOUNT, 184, 165, 240, NULL, NULL, },
  {  944, "Shroodle", TYPE_POISON, TYPE_NORMAL, 124, 70, 120, NULL, NULL, },
  {  945, "Grafaiai", TYPE_POISON, TYPE_NORMAL, 199, 149, 160, NULL, NULL, },
  {  957, "Tinkatink", TYPE_FAIRY, TYPE_STEEL, 85, 110, 137, NULL, NULL, },
  {  958, "Tinkatuff", TYPE_FAIRY, TYPE_STEEL, 109, 145, 163, NULL, NULL, },
  {  959, "Tinkaton", TYPE_FAIRY, TYPE_STEEL, 155, 196, 198, NULL, NULL, },
  {  960, "Wiglett", TYPE_WATER, TYPECOUNT, 109, 52, 67, NULL, NULL, },
  {  961, "Wugtrio", TYPE_WATER, TYPECOUNT, 205, 136, 111, NULL, NULL, },
  {  962, "Bombirdier", TYPE_FLYING, TYPE_DARK, 198, 172, 172, NULL, NULL, },
  {  965, "Varoom", TYPE_STEEL, TYPE_POISON, 123, 107, 128, NULL, NULL, },
  {  966, "Revavroom", TYPE_STEEL, TYPE_POISON, 229, 168, 190, NULL, NULL, },
  {  971, "Greavard", TYPE_GHOST, TYPECOUNT, 105, 106, 137, NULL, NULL, },
  {  972, "Houndstone", TYPE_GHOST, TYPECOUNT, 186, 195, 176, NULL, NULL, },
  {  974, "Cetoddle", TYPE_ICE, TYPECOUNT, 119, 80, 239, NULL, NULL, },
  {  975, "Cetitan", TYPE_ICE, TYPECOUNT, 208, 123, 347, NULL, NULL, },
  {  979, "Annihilape", TYPE_FIGHTING, TYPE_GHOST, 220, 178, 242, NULL, NULL, },
  {  980, "Clodsire", TYPE_POISON, TYPE_GROUND, 127, 151, 277, NULL, NULL, },
  {  983, "Kingambit", TYPE_DARK, TYPE_STEEL, 238, 203, 225, NULL, NULL, },
  {  996, "Frigibax", TYPE_DRAGON, TYPE_ICE, 134, 86, 163, NULL, NULL, },
  {  997, "Arctibax", TYPE_DRAGON, TYPE_ICE, 173, 128, 207, NULL, NULL, },
  {  998, "Baxcalibur", TYPE_DRAGON, TYPE_ICE, 254, 168, 229, NULL, NULL, },
  {  999, "Gimmighoul", TYPE_GHOST, TYPECOUNT, 140, 76, 128, NULL, NULL, },
  { 1000, "Gholdengo", TYPE_STEEL, TYPE_GHOST, 252, 190, 202, NULL, NULL, }
};

#define SPECIESCOUNT (sizeof(sdex) / sizeof(*sdex))

typedef struct stats {
  const species* s;
  unsigned atk, def, sta;   // base stats for the Form
  unsigned hlevel;          // halflevel 1..99
  unsigned ia, id, is;      // individual vector components
  float effa, effd;         // effective attack and defense
  unsigned mhp;             // max hit points
  unsigned cp;              // combat power
  float geommean;           // geometric mean of effa, effd, mhp
  struct stats* next;
} stats;

// atk, def, and sta all ought be mod forms (i.e. sum of base and IV)
static int
calccp(unsigned atk, unsigned def, unsigned sta, unsigned halflevel){
  float cand = (atk * sqrt(def) * sqrt(sta) * pow(cpm(halflevel), 2)) / 10;
  return cand < 10 ? 10 : floor(cand);
}

static inline float
calc_eff_a(unsigned atk, unsigned halflevel){
  return cpm(halflevel) * atk;
}

static inline float
calc_eff_d(unsigned def, unsigned halflevel){
  return cpm(halflevel) * def;
}

static inline unsigned
calc_mhp(unsigned sta, unsigned halflevel){
  return floor(cpm(halflevel) * sta);
}

static inline float
calc_fit(float effa, float effd, unsigned mhp){
  return cbrt(effa * effd * mhp);
}

// returns integer part, sets *half to 1 if it's a +0.5
// (aren't guaranteed exact representation with floats)
static inline unsigned
halflevel_to_level(unsigned hl, unsigned* half){
  if(hl % 2 == 0){
    *half = 1;
  }else{
    *half = 0;
  }
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

static int
update_optset(stats** osets, const species* s, unsigned ia, unsigned id, unsigned is, unsigned hl){
  stats **prev = osets;
  stats *cur;
  unsigned moda = s->atk + ia;
  unsigned modd = s->def + id;
  float effa = calc_eff_a(moda, hl);
  float effd = calc_eff_d(modd, hl);
  unsigned mods = s->sta + is;
  unsigned mhp = calc_mhp(s->sta + is, hl);
  while( (cur = *prev) ){
    if(hl < cur->hlevel){
      break; // we're a lower level than any on the list; insert
    }
    if(hl == cur->hlevel){ // need compare
      if(effa == cur->effa && effd == cur->effd && mhp == cur->mhp){
        // we're equal to something on the list; insert here
        break;
      }else if(effa <= cur->effa && effd <= cur->effd && mhp <= cur->mhp){ // we're worse, exit
        // we're strictly less than something on the list; exit
        return -1;
      }else if(effa >= cur->effa && effd >= cur->effd && mhp >= cur->mhp){
        // we're strictly better than something on the list; remove it and continue
        *prev = cur->next;
        free(cur);
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
  cur->geommean = calc_fit(cur->effa, cur->effd, cur->mhp);
  cur->next = *prev;
  *prev = cur;
  return 0;
}
