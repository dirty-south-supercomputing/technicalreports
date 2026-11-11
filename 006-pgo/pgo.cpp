#include "pgotypes.h"
#include <cstdint>

// each row is an attacking Type
const int trelations[TYPECOUNT][TYPECOUNT] = {
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
int type_relation(pgo_types_e atype, pgo_types_e ttype){
  if(atype >= TYPECOUNT){
    throw std::invalid_argument("bad atype");
  }
  if(ttype >= TYPECOUNT){
    throw std::invalid_argument("bad ttype");
  }
  return trelations[atype][ttype];
}

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
double cpm(int halflevel){
  // many of these *cannot be stored accurately* as a 32-bit value!
  static const double cpms[] = {
    0.094, 0.1351374318, 0.166398, 0.192650919, 0.215732, 0.2365726613, // 1
    0.25572, 0.2735303812, 0.29025, 0.3060573775, 0.321088,
    0.3354450362, 0.349213, 0.3624577511, 0.375236, 0.387592416, 0.399567,
    0.4111935514,
    0.4225, 0.4329264091, 0.443108, 0.4530599591, 0.462798, // 10
    0.472336093, 0.481685, 0.4908558003, 0.499858, 0.508701765, 0.517394,
    0.5259425113, 0.534354, 0.5426357375, 0.550793, 0.5588305862, 0.566755,
    0.5745691333, 0.582279, 0.5898879072,
    0.5974, 0.6048236651, 0.612157, // 20
    0.6194041216, 0.626567, 0.6336491432, 0.640653, 0.6475809666,
    0.654436, 0.6612192524, 0.667934, 0.6745818959, 0.681165, 0.6876849038,
    0.694144, 0.70054287, 0.706884, 0.7131691091, 0.719399, 0.7255756136,
    0.7317, 0.7347410093, 0.737769, 0.7407855938, 0.743789, 0.7467812109, // 30
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
  &ATK_Soak,
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
  &ATK_Kowtow_Cleave,
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

const char *species::categorystr(species::species_cat c){
  switch(c){
    case CAT_NORMAL: return "";
    case CAT_MYTHICAL: return "Mythical";
    case CAT_LEGENDARY: return "Legendary";
    case CAT_FUSION: return "Fusion";
    case CAT_CROWNED: return "Crowned";
    case CAT_MEGA: return "Mega Evolution";
    case CAT_ULTRABEAST: return "Ultra Beast";
    case CAT_BABY: return "Baby";
    case CAT_PARADOX: return "Paradox";
    case CAT_FPARTNER: return "First Partner";
    default: throw std::exception();
  }
}

const char *species::evolitemstr(species::evol_item eitem){
  switch(eitem){
    case species::EVOL_NOITEM: return "";
    case species::EVOL_FEMALE: return "must be female"; break;
    case species::EVOL_MALE: return "must be male"; break;
    case species::EVOL_SUNSTONE: return "Sun stone"; break;
    case species::EVOL_KINGSROCK: return "King's rock"; break;
    case species::EVOL_METALCOAT: return "Metal coat"; break;
    case species::EVOL_DRAGONSCALE: return "Dragon's scale"; break;
    case species::EVOL_UPGRADE: return "Upgrade"; break;
    case species::EVOL_SINNOHSTONE: return "Sinnoh stone"; break;
    case species::EVOL_SINNOHSTONEMALE: return "Sinnoh stone, must be male"; break;
    case species::EVOL_SINNOHSTONEFEMALE: return "Sinnoh stone, must be female"; break;
    case species::EVOL_UNOVASTONE: return "Unova stone"; break;
    case species::EVOL_TARTAPPLE: return "20 Tart apples"; break;
    case species::EVOL_SWEETAPPLE: return "20 Sweet apples"; break;
    case species::EVOL_SYRUPYAPPLE: return "20 Syrupy apples"; break;
    case species::EVOL_GIMMICOINS: return "999 Gimmicoins"; break;
    case species::EVOL_ZYGARDECELL50: return "50 Zygarde cells"; break;
    case species::EVOL_ZYGARDECELL200: return "200 Zygarde cells"; break;
    case species::EVOL_BUDDYHEARTS10NIGHT: return "earn 10 Buddy hearts and evolve at night"; break;
    case species::EVOL_BUDDYHEARTS20: return "earn 20 Buddy hearts"; break;
    case species::EVOL_BUDDYHEARTS70: return "earn 70 Buddy hearts"; break;
    case species::EVOL_TREATS25: return "feed 25 treats"; break;
    case species::EVOL_RAIDDARKSTEEL15: return "defeat 15 Dark or Steel raid bosses"; break;
    case species::EVOL_RAIDMAXWATER30: return "defeat 30 Water raid bosses or Max Battles"; break;
    case species::EVOL_RAIDMAXDARK30: return "defeat 30 Dark raid bosses or Max Battles"; break;
    case species::EVOL_DEFEATPSYCHICGHOST30: return "defeat 30 Psychic or Ghost Pokémon"; break;
    case species::EVOL_DEFEATPSYCHIC30: return "defeat 30 Psychic Pokémon"; break;
    case species::EVOL_DEFEATGHOST30: return "defeat 30 Ghost Pokémon"; break;
    case species::EVOL_STARDUST2K: return "2k Stardust"; break;
    case species::EVOL_STARDUST10K: return "10k Stardust"; break;
    case species::EVOL_CAPTUREDRAGON7: return "capture 7 Dragon Pokémon"; break;
    case species::EVOL_CAPTUREDRAGON20: return "capture 20 Dragon Pokémon"; break;
    case species::EVOL_CAPTUREPOISON30: return "capture 30 Poison Pokémon"; break;
    case species::EVOL_CAPTUREPSYCHIC30: return "capture 30 Psychic Pokémon"; break;
    case species::EVOL_CAPTUREDARK32: return "capture 32 Dark Pokémon"; break;
    case species::EVOL_RAID10: return "win 10 raids"; break;
    case species::EVOL_EXCELLENT10: return "10 Excellent throws"; break;
    case species::EVOL_MAGLURE: return "active Magnetic lure"; break;
    case species::EVOL_RAINLURE: return "rain or active Rainy lure"; break;
    case species::EVOL_GLACIALLURE: return "active Glacial lure"; break;
    case species::EVOL_MOSSYLURE: return "active Mossy lure"; break;
    case species::EVOL_ROTATEDEVICE: return "rotate the device"; break;
    case species::EVOL_DAY: return "evolve during the day"; break;
    case species::EVOL_DUSK: return "evolve during dusk"; break;
    case species::EVOL_NIGHT: return "evolve at night"; break;
    case species::EVOL_FULLMOON: return "evolve during a full moon"; break;
    case species::EVOL_INCENSE: return "use (regular) incense"; break;
    case species::EVOL_CROWNSHIELD1000: return "1000 Crown Shield energy, must know Iron Head"; break;
    case species::EVOL_CROWNSWORD1000: return "1000 Crown Sword energy, must known Iron Head"; break;
    case species::EVOL_VOLT1000: return "1000 Volt Fusion energy"; break;
    case species::EVOL_BLAZE1000: return "1000 Blaze Fusion energy"; break;
    case species::EVOL_SOLAR1000: return "1000 Solar Fusion energy"; break;
    case species::EVOL_LUNAR1000: return "1000 Lunar Fusion energy"; break;
    case species::EVOL_DRAGONASCENT: return "must know Dragon Ascent"; break;
    default: throw std::exception();
  }
}

std::string adventure_effect(const std::string& name){
  if(name == ATK_Roar_of_Time.name){
    return "Pause incense, Lucky Egg, and Star Piece timers.";
  }else if(name == ATK_Spacial_Rend.name){
    return "Extend the distance at which wild Pokémon can be encountered.";
  }else if(name == ATK_Ice_Burn.name){
    return "Slow down catch circles and enhance catch rate by 50%.";
  }else if(name == ATK_Freeze_Shock.name){
    return "Immobilize Pokemon in encounters and enhance catch rate by 25%.";
  }else if(name == ATK_Sunsteel_Strike.name){
    return "Spawn and evolve daytime Pokémon.";
  }else if(name == ATK_Moongeist_Beam.name){
    return "Spawn and evolve nighttime Pokémon.";
  }else if(name == ATK_Behemoth_Blade.name){
    return "Enhance base attack by 10% in raids and 5% in Max Battles.";
  }else if(name == ATK_Behemoth_Bash.name){
    return "Enhance base defense by 10% in raids and 5% in Max Battles.";
  }else if(name == ATK_Dynamic_Punch_Plus.name){
    return "Break two shields in Super Mega Raids.";
  }else if(name == ATK_Future_Sight_Plus.name){
    return "Indicate 3🟉+ Pokémon.";
  }else if(name == ATK_Dynamax_Cannon.name){
    return "Boost Max Move levels by one for Dynamax/G-Max Pokémon and Eternatus.";
  }
  return "";
}

// is the form banned from all 3x3?
bool pvp_banned_p(const species &s){
  if(s.name == "Shedinja"){
    return true;
  }
  if(s.name == "Ditto"){
    return true;
  }
  return false;
}

bool plus_attack_p(const attack &a){
  if(a.name.back() == '+'){
    return true;
  }
  return false;
}

bool multiple_charged_attacks(const species &s){
  unsigned ccount = 0;
  for(const auto &a : s.attacks){
    if(!charged_attack_p(*a)){
      continue;
    }
    if(++ccount > 1){
      break;
    }
  }
  return ccount > 1;
}

bool bidirectional_evol_p(const species &s){
  if(s.category == species::CAT_FUSION || s.category == species::CAT_CROWNED
      || s.category == species::CAT_MEGA){
    return true;
  }
  if(s.name.contains("Zygarde") || s.name.contains("Shaymin")
      || s.name.contains("Hoopa") || s.name.contains("Keldeo")){
    return true;
  }
  return false;
}

// each type is boosted by some type of weather
pgo_weather_t type_weather_boost(pgo_types_e t){
  switch(t){
    case TYPE_BUG: return WEATHER_RAIN;
    case TYPE_DARK: return WEATHER_FOG;
    case TYPE_DRAGON: return WEATHER_WINDY;
    case TYPE_ELECTRIC: return WEATHER_RAIN;
    case TYPE_FAIRY: return WEATHER_CLOUDY;
    case TYPE_FIGHTING: return WEATHER_CLOUDY;
    case TYPE_FIRE: return WEATHER_CLEAR;
    case TYPE_FLYING: return WEATHER_WINDY;
    case TYPE_GHOST: return WEATHER_FOG;
    case TYPE_GRASS: return WEATHER_CLEAR;
    case TYPE_GROUND: return WEATHER_CLEAR;
    case TYPE_ICE: return WEATHER_SNOW;
    case TYPE_NORMAL: return WEATHER_PARTLY_CLOUDY;
    case TYPE_POISON: return WEATHER_CLOUDY;
    case TYPE_PSYCHIC: return WEATHER_WINDY;
    case TYPE_ROCK: return WEATHER_PARTLY_CLOUDY;
    case TYPE_STEEL: return WEATHER_SNOW;
    case TYPE_WATER: return WEATHER_RAIN;
    case TYPECOUNT:
      break;
  }
  std::cerr << "invalid type " << t << std::endl;
  throw std::exception();
}

const char* WNames[WEATHERCOUNT] = {
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
const char* WSNames[WEATHERCOUNT] = {
  "Clear Night",
  NULL,
  "Partly Cloudy Night",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

const unsigned GMAX_POWER[4] = { 350, 400, 450, 550 };
const unsigned DMAX_POWER[4] = { 250, 300, 350, 450 };

const species *get_previous_evolution(const species &s){
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

static const species *
family_root(const species &s){
  const auto *root = get_previous_evolution(s);
  while(root){
    const auto *tmp = get_previous_evolution(*root);
    if(!tmp){
      return root;
    }
    root = tmp;
  }
  return &s;
}

bool trade_assisted_evol_p(const species &s){
  const auto *root = family_root(s);
  static const std::vector<std::string> fams = {
    "Abra", "Machop", "Geodude", "Alolan Geodude", "Gastly",
    "Roggenrola", "Timburr", "Karrablast", "Shelmet",
    "Phantump", "Pumpkaboo",
  };
  for(const auto &n : fams){
    if(root->name == n){
      return true;
    }
  }
  return false;
}

// cost in candy to transition to this form
unsigned evolcost(const species &s){
  if(ismega_p(s)){
    return 0;
  }
  int order = 0; // our position in family 0..3
  const auto *root = get_previous_evolution(s);
  while(root){
    ++order;
    const auto *tmp = get_previous_evolution(*root);
    if(tmp){
      root = tmp;
    }else{
      break;
    }
  }
  if(!root){
    root = &s;
  }
  if(order <= 0 || order >= 3){
    return 0;
  }
  if(root->name == "Azurill"){ // special case: 25 for either evolution
    return 25;
  }
  bool babyfamily = root->category == species::CAT_BABY;
  // these families are all 12 and 50.
  const std::vector<std::string> evol12s = {
    "Weedle", "Caterpie", "Pidgey", "Wurmple", "Whismur", "Pidove",
  };
  for(const auto &n : evol12s){
    if(root->name == n){
      if(order == 1){
        return 12;
      }
      return 50; // order == 2
    }
  }
  // these two-stage families are all 400
  const std::vector<std::string> evol400s = {
    "Gyarados", "Wailord", "Altaria", "Volcarona", "Noivern",
    "Bewear", "Golisopod", "Melmetal", "Toxtricity", "Frosmoth",
    "Hydrapple",
  };
  for(const auto &n : evol400s){
    if(s.name == n){
      return 400;
    }
  }
  // these two-stage families are all 100
  const std::vector<std::string> evol100s = {
    "Sneasler", "Glalie", "Froslass", "Milotic",
  };
  for(const auto &n : evol100s){
    if(s.name == n){
      return 100;
    }
  }
  // if we have a trade discount, we are 200 for two-stage families, and 50:200
  // for three stage families, except for exceptions at 25:100. check the
  // exceptions first.
  const std::vector<std::string> evol25trades = {
    "Abra", "Machop", "Geodude", "Alolan Geodude", "Gastly",
  };
  for(const auto &n : evol25trades){
    if(root->name == n){
      if(order == 1){
        return 25;
      }
      return 100; // order == 2
    }
  }
  // maxorder will not account for temporary evolutions (i.e. megas)
  int maxorder = order;
  std::vector<const species *> evols;
  if(get_persistent_evolutions(s, evols)){
    ++maxorder;
    // we only need check one level, and it can only count once
    for(const auto &e : evols){
      std::vector<const species *> store; // don't invalidate evols
      if(get_persistent_evolutions(*e, store)){
        ++maxorder;
        break;
      }
    }
  }
  // now apply the general rule for trade assisted families.
  if(trade_assisted_evol_p(*root)){
    if(order == 1 && maxorder == 2){
      return 50;
    }
    return 200; // either second of two-stage or third of three-stage
  }
  // otherwise, if we are a two-stage family, the cost is 50, and if we are
  // a three-stage family, the costs are 25:100, unless we are a baby-rooted
  // family, in which case they're 25:50.
  if(order == 2){
    // exceptions to the baby family rule (going back up to 100) include roserade
    if(babyfamily && root->name != "Budew"){
      return 50;
    }
    return 100;
  }
  // otherwise, order must be 1
  if(maxorder == 1){ // two-stage family
    // tyrogue, smoochum, and wynaut are exceptions to the babyfamily rule,
    // and cost 25 rather than 50
    if(root->name == "Tyrogue" || root->name == "Smoochum" || root->name == "Wynaut"){
      return 25;
    }
    return 50;
  }
  // otherwise, three-stage family
  return 25;
}

int purification_cost(const species &s){
  switch(s.a2cost){
    case 10: return 1;
    case 50: return 3;
    case 75 : return 5;
    case 100: return 20;
    default: break;
  }
  std::cerr << "bad a2cost " << s.a2cost << " for " << s.name << std::endl;
  throw std::exception();
}

void generate_ivs_maxlevel(const species &s, int cpceil, stats *svec, bool shadow){
  unsigned idx = 0;
  for(int iva = 0 ; iva < 16 ; ++iva){
    for(int ivd = 0 ; ivd < 16 ; ++ivd){
      for(int ivs = 0 ; ivs < 16 ; ++ivs){
        auto &st = svec[idx];
        const unsigned moda = s.atk + iva;
        const unsigned modd = s.def + ivd;
        const unsigned mods = s.sta + ivs;
        auto hlevel = maxlevel_cp_bounded(moda, modd, mods, cpceil, &st.cp);
        st.init(&s, hlevel, iva, ivd, ivs, shadow);
        ++idx;
      }
    }
  }
}

stats *order_ivs(const species &s, int cpceil, int(*cmpfxn)(const void*, const void*), unsigned *vcount){
  *vcount = IVLEVVEC;
  if(s.shadow){
    *vcount *= 2;
  }
  stats *svec = new stats[*vcount];
  generate_ivs_maxlevel(s, cpceil, svec, false);
  if(s.shadow){
    generate_ivs_maxlevel(s, cpceil, svec + IVLEVVEC, true);
  }
  qsort(svec, *vcount, sizeof(*svec), cmpfxn);
  return svec;
}

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
  { "Duraludon", "G-Max Depletion", TYPE_DRAGON, true, },
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
  { "", "", TYPECOUNT, false, },
};

// each gmax attack is associated with a single species, which can use only
// that attack. unit tests verify that the species are valid.
gmaxattack *lookup_gmax_attack(const species &s){
  for(auto gatk = GMaxAttacks ; gatk->sname.length() ; ++gatk){
    if(s.name == gatk->sname){
      return gatk;
    }
  }
  return nullptr;
}

gmaxattack *gmaxattacks_begin(){
  return &GMaxAttacks[0];
}

gmaxattack *gmaxattacks_end(){
  return &GMaxAttacks[sizeof(GMaxAttacks) / sizeof(*GMaxAttacks) - 1];
}
