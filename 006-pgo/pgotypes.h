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
#include "pokemon.h"

constexpr unsigned TEAMSIZE = 3;
constexpr int MAXIVELEM = 15;
constexpr int MAXCHARGEDBUFF = 4;
constexpr unsigned ENERGY_MAX = 100;
// one can reach the 99th halflevel (50) through powering up.
// past that, one can get two temporary half-levels through active
// best buddy status, and two temporary half-levels by mega level 4,
// for a total of four extra half-levels.
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

// there are 171 distinct species types (18 + C(18, 2))
constexpr unsigned TYPINGCOUNT = 171;

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

extern const char* WNames[WEATHERCOUNT];
extern const char* WSNames[WEATHERCOUNT]; // secondary names for weather

extern const int trelations[TYPECOUNT][TYPECOUNT]; // FIXME hide this (currently used by dualtypes.cpp)

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
  static const float buffmap[9] = { 4.0 / 8, 4.0 / 7, 4.0 / 6, 4.0 / 5,
                                    1,
                                    5 / 4.0, 6 / 4.0, 7 / 4.0, 8 / 4.0 };
  return buffmap[bufflevel + 4];
}

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
fast_attack_p(const attack &a){
  return a.energytrain >= 0; // need 0 to pick up Transform
}

static inline bool
charged_attack_p(const attack &a){
  return a.energytrain < 0;
}

using pairmap = std::map<std::string, attackset>;

// return the dmax attack type corresponding to this fast attack. it is simply
// the attack's type, except for Hidden Power, which always maps to Normal.
static inline pgo_types_e
dmax_attack_type(const attack *a){
  if(a->name == "Hidden Power"){
    return TYPE_NORMAL;
  }
  return a->type;
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

int purification_cost(const species &s);

gmaxattack *lookup_gmax_attack(const species &s);
gmaxattack *gmaxattacks_begin();
gmaxattack *gmaxattacks_end();

static inline bool
gmax_attack_p(const attack &a){
  const std::string prefix = "G-Max";
  if(a.name.compare(0, prefix.length(), prefix) == 0){
    return true;
  }
  return false;
}

static inline bool
max_attack_p(const attack &a){
  const std::string prefix = "Max";
  if(a.name.compare(0, prefix.length(), prefix) == 0){
    return true;
  }
  return false;
}

static inline double
calc_eff_a_raw(unsigned atk, unsigned halflevel){
  return cpm(halflevel) * atk;
}

// calculate eff_a and apply the shadow bonus, if appropriate.
// provide mod_a as atk.
static inline double
calc_eff_a(unsigned atk, unsigned halflevel, bool isshadow){
  auto effa = calc_eff_a_raw(atk, halflevel);
  if(isshadow){
    effa = effa * 6 / 5;
  }
  return effa;
}

// calculate eff_d and apply the shadow penalty, if appropriate.
// provide mod_d as def.
static inline double
calc_eff_d(unsigned def, unsigned halflevel, bool isshadow){
  auto s = cpm(halflevel) * def;
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

static inline double
calc_amean(double effa, double effd, unsigned mhp){
  return (effa + effd + mhp) / 3;
}

static inline double
calc_gmean(double effa, double effd, unsigned mhp){
  return cbrt(effa * effd * mhp);
}

static inline float
calc_ppe(const attack *a){
  return a->powertrain / static_cast<float>(-a->energytrain);
}

struct stats {
  const species *s;
  unsigned hlevel;          // halflevel 1..99
  unsigned ia, id, is;      // individual vector components
  double effa, effd;        // effective attack and defense
  unsigned mhp;             // max hit points
  int cp;                   // combat power
  double average;           // arithemetic mean of effa, effd, mhp
  double geommean;          // geometric mean of effa, effd, mhp
  float apercent;           // geommean advantage over pessimal level-maxed iv
                            // (not filled out at creation/init time)
  bool shadow;              // is this the shadow variant?
  struct stats* next;

  stats() :
    s(nullptr),
    next(nullptr)
  {
  }


  void init(const species *S, unsigned Hlevel, unsigned IA, unsigned ID, unsigned IS, bool Shadow){
    s = S;
    hlevel = Hlevel;
    ia = IA;
    id = ID;
    is = IS;
    shadow = Shadow;
    auto moda = s->atk + ia;
    auto modd = s->def + id;
    auto mods = s->sta + is;
    effa = calc_eff_a(moda, hlevel, shadow);
    effd = calc_eff_d(modd, hlevel, shadow);
    mhp = calc_mhp(mods, hlevel);
    cp = calccp(moda, modd, mods, hlevel);
    average = calc_amean(effa, effd, mhp);
    geommean = calc_gmean(effa, effd, mhp);
    next = nullptr;
  }

  stats(const species *S, unsigned Hlevel, unsigned IA, unsigned ID, unsigned IS, bool Shadow) :
    s(S),
    hlevel(Hlevel),
    ia(IA),
    id(ID),
    is(IS),
    shadow(Shadow)
  {
    init(S, Hlevel, IA, ID, IS, Shadow);
  }

  double bulk() const {
    return sqrt(mhp * effd);
  }

  double statprod() const {
    return effa * effd * mhp;
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
statscmp_sprod(const void *vst1, const void *vst2){
  const stats *st1 = static_cast<const stats*>(vst1);
  const stats *st2 = static_cast<const stats*>(vst2);
  return st1->statprod() < st2->statprod() ? -1 :
          st1->statprod() > st2->statprod() ? 1 : 0;
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
  const auto b1 = st1->bulk();
  const auto b2 = st2->bulk();
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
  return d * 6.0 / 5.0;
}

// weather boost in Nx1 mode is 20%
static inline float
calc_wboost(float d){
  return d * 6.0 / 5.0;
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

static inline double
calc_pok_sprod(const stats *s){
  return s->statprod();
}

static inline double
calc_pok_amean(const stats *s){
  return s->average;
}

static inline double
calc_pok_gmean(const stats *s){
  return s->geommean;
}

static inline double
calc_pok_cp(const stats *s){
  return s->cp;
}

static inline double
calc_pok_effa(const stats *s){
  return s->effa;
}

static inline double
calc_pok_effd(const stats *s){
  return s->effd;
}

static inline double
calc_pok_mhp(const stats *s){
  return s->mhp;
}

static inline double
calc_pok_bulk(const stats *s){
  return s->bulk();
}

static constexpr unsigned IVLEVVEC =
  (MAXIVELEM + 1) * (MAXIVELEM + 1) * (MAXIVELEM + 1);

// generate stats for each possible IV at its maximum level (subject to cpceil)
// the resulting vector is ordered with ivs changing most rapidly, then ivd.
void generate_ivs_maxlevel(const species &s, int cpceil, stats *svec, bool shadow);

// generate the stats for each possible IV at its maximum level subject to the
// cpceiling (-1 for no ceiling), ordered according to fitfxn.
stats *order_ivs(const species &s, int cpceil,
                 int(*cmpfxn)(const void*, const void*),
                 unsigned *vcount);

// instantiation of a pokémon -- species, IVs, and known attacks
struct pmon { // static elements
  struct stats s;
  const attack *fa, *ca1, *ca2;
};

static inline double
calc_fitfxn(double(*fitfxn)(const stats *), const species &s,
            unsigned ia, unsigned id, unsigned is, unsigned hl,
            bool shadow){
  stats st{&s, hl, ia, id, is, shadow};
//std::cerr << s->name << " " << ia << " " << id << " " << is << " " << hl << std::endl;
  return fitfxn(&st);
}

// optimize on comparable fitness function subject to floor
static int
update_optset(stats** osets, const species &s, unsigned ia, unsigned id,
              unsigned is, unsigned hl, double floor, double* minfit,
              bool isshadow, double(*fitfxn)(const stats *)){
  stats **prev = osets;
  stats *cur;
  auto m = calc_fitfxn(fitfxn, s, ia, id, is, hl, isshadow);
  unsigned moda = s.atk + ia;
  unsigned modd = s.def + id;
  auto effa = calc_eff_a(moda, hl, isshadow);
  auto effd = calc_eff_d(modd, hl, isshadow);
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
find_optimal_set(const species &s, int cpceil, double floor, bool isshadow, double(*fitfxn)(const stats *)){
  stats* optsets = nullptr;
  double minfit = -1;
  for(int iva = 0 ; iva < 16 ; ++iva){
    for(int ivd = 0 ; ivd < 16 ; ++ivd){
      for(int ivs = 0 ; ivs < 16 ; ++ivs){
        int cp;
        unsigned hl = maxlevel_cp_bounded(s.atk + iva, s.def + ivd, s.sta + ivs, cpceil, &cp);
        if(update_optset(&optsets, s, iva, ivd, ivs, hl, floor, &minfit, isshadow, fitfxn) < 0){
          return nullptr;
        }
      }
    }
  }
  stats* collectopt = nullptr;
  stats** qopt = &collectopt;
  double maxm = -1;
  // print the optimal frontier (large), and extract the truly optimal sets (small)
  while(optsets){
    stats *cur;
    cur = optsets;
    optsets = cur->next;
    auto m = fitfxn(cur);
    //printf(" %u/%u/%u: %2u %4u %.3f %.3f %u %.3f\n", cur->ia, cur->id, cur->is,
    //    cur->hlevel, cur->cp, cur->effa, cur->effd, cur->mhp, cur->geommean);
    if(m > maxm){ // new optimal
      stats* c;
      // clean out existing true optimals
      while( (c = collectopt) ){
        collectopt = c->next;
        delete c;
      }
      collectopt = cur;
      qopt = &cur->next;
      cur->next = nullptr;
      maxm = m;
    }else if(m == maxm){
      *qopt = cur;
      qopt = &cur->next;
      cur->next = nullptr;
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
lookup_species(const std::string &name){
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(std::ranges::equal(s->name, name, {},
        [](const auto& c){ return tolower(c); },
        [](const auto& c){ return tolower(c); })){
      return &*s;
    }
  }
  return nullptr;
}

// look up the mega by name. if it is found, return a species synthesized from
// the mega in sbacking
static inline bool
lookup_mega(const std::string &name, species *sbacking){
  for(auto s = species_begin() ; s != species_end() ; ++s){
    for(const auto &m : s->mforms){
      if(std::ranges::equal(m.name, name, {},
          [](const auto& c){ return tolower(c); },
          [](const auto& c){ return tolower(c); })){
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
      if(a->name == as->name){
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
    if(a->name == atk->name){
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

// fill in vec with immediate evolution target(s), if there are any.
// return true if there was at least one, and false otherwise.
static inline bool
get_evolutions(const species &s, std::vector<const species*> &vec,
               std::forward_list<species> &store){
  for(auto t = species_begin() ; t != species_end() ; ++t){
    if(t->from == s.name){
      vec.push_back(&*t);
    }
  }
  for(const auto &m : s.mforms){
    store.emplace_front(&s, m);
    vec.push_back(&store.front());
  }
  return vec.size() > 0;
}

// fill in vec with entire family, starting with s.
static inline void
get_family(const species &s, std::vector<const species*> &vec,
           std::forward_list<species> &store){
  vec.push_back(&s);
  for(auto t = species_begin() ; t != species_end() ; ++t){
    if(t->from == s.name){
      get_family(*t, vec, store);
    }
  }
  for(const auto &m : s.mforms){
    store.emplace_front(&s, m);
    get_family(store.front(), vec, store);
  }
}

// get the immediate ancestor of this form, or nullptr if none exists.
// returns the base form when called on a synthesized mega.
const species *get_previous_evolution(const species &s);

// get the earliest form of the family (which might be us)
static inline const species &
get_base_evolution(const species &s){
  if(s.from.empty()){
    return s;
  }
  auto t = species_begin();
  while(t != species_end()){
    if(s.from == t->name){
      break;
    }
    ++t;
  }
  return get_base_evolution(*t);
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
  if(ismega_p(s)){
    return get_stage(*devol);
  }
  return get_stage(*devol) + 1;
}

static bool
name_in_list(const species &s, const char **l){
  const char *name = s.name.c_str();
  if(strncmp(name, "Mega ", 5) == 0){
    name += 5;
  }
  for(const char **n = l ; *n ; ++n){
    if(strcmp(name, *n) == 0){
      return true;
    }
  }
  return false;
}

static inline unsigned
stardust_reward(const species &s){
  // some legendaries are technically evolutions, but they ought be 100 across the board
  if(s.category == species::CAT_LEGENDARY
      || s.category == species::CAT_FUSION
      || s.category == species::CAT_CROWNED){
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

// case-insensitive check of attack name
static inline bool
attack_named_p(const attack *a, const std::string &name){
  return std::ranges::equal(a->name, name, {},
      [](const auto& c){ return tolower(c); },
      [](const auto& c){ return tolower(c); }
      );
}

// return the named charged attack iff s can learn it
static inline const attack *
species_charged_attack(const species *s, const char *aname){
  for(const auto a : s->attacks){
    if(!charged_attack_p(*a)){
      continue;
    }
    if(!attack_named_p(a, aname)){
      continue;
    }
    return a;
  }
  return nullptr;
}

// return the named fast attack iff s can learn it
static inline const attack *
species_fast_attack(const species *s, const char *aname){
  for(const auto a : s->attacks){
    if(!fast_attack_p(*a)){
      continue;
    }
    if(!attack_named_p(a, aname)){
      continue;
    }
    return a;
  }
  return nullptr;
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
    if(!st){
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
  *ca2 = nullptr;
  const char *sep = strchr(spec, '/');
  if(sep){
    char *fspec = strndup(spec, sep - spec);
    const attack *ca1 = species_charged_attack(s, fspec);
    *ca2 = species_charged_attack(s, sep + 1);
    free(fspec);
    if(*ca2 == nullptr){
      return nullptr;
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
  s->s = &sp;
  s->effa = calc_eff_a(sp.atk + s->ia, s->hlevel, false);
  s->effd = calc_eff_d(sp.def + s->id, s->hlevel, false);
  s->mhp = calc_mhp(sp.sta + s->is, s->hlevel);
  s->geommean = calc_gmean(s->effa, s->effd, s->mhp);
  s->average = calc_amean(s->effa, s->effd, s->mhp);
  s->cp = calccp(sp.atk + s->ia, sp.def + s->id, sp.sta + s->is, s->hlevel);
  s->shadow = shadow;
  s->next = nullptr;
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
  for(const auto a : atks){
    if(fast_attack_p(*a)){
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

static inline float
calc_ara(pgo_types_e t0, pgo_types_e t1, pgo_types_e plustype, int totals[TEFFECTRANGE]){
  memset(totals, 0, sizeof(*totals) * TEFFECTRANGE);
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
  for(unsigned i = 0 ; i < TEFFECTRANGE ; ++i){
    ara += type_effectiveness_mult(static_cast<int>(i) - 3) * totals[i];
  }
  ara /= TYPINGCOUNT;
  return ara;
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

  typeset(pgo_types_e T0, pgo_types_e T1, pgo_types_e Plustype) :
      t0(T0),
      t1(T1),
      plustype(Plustype)
  {
    ara = calc_ara(t0, t1, plustype, totals);
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

  // only emits if there is a population
  void emit_with_pop(std::ostream &fp, bool subdir, bool pdir) const;

  // always emits, and leaves out population info
  void emit(std::ostream &fp, bool pdir) const;

  friend bool operator<(const typeset &l, const typeset &r) {
    if(l.ara < r.ara){
      return true;
    }else if(r.ara < l.ara){
      return false;
    }
    if(l.learnpop.size() < r.learnpop.size()){
      return true;
    }else if(r.learnpop.size() < l.learnpop.size()){
      return false;
    }
    if(l.t0 < r.t0){
      return true;
    }else if(r.t0 < l.t0){
      return false;
    }
    if(l.t1 < r.t1){
      return true;
    }else if(r.t1 < l.t1){
      return false;
    }
    if(l.plustype < r.plustype){
      return true;
    }else if(r.plustype < l.plustype){
      return false;
    }
    throw std::exception(); // we had a duplicate
  }

  friend bool operator>(const typeset &l, const typeset &r) {
    return !(l < r);
  }

private:
  void emitcore(std::ostream &fp, bool pdir) const;

};

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
      tsets.emplace_back(t0, t1, TYPECOUNT);
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

struct raidcandidate {
  const species* s;   // species
  unsigned hlevel;    // halflevel
  unsigned iva;       // attack iv 0..15
  const attack *ca;   // charged attack

  raidcandidate(const species *S, const attack *Ca, unsigned Hlevel, unsigned Iva) :
    s(S),
    hlevel(Hlevel),
    iva(Iva),
    ca(Ca)
  { }

  void emit(std::ostream &fp, unsigned maxp, bool unified, bool pdir) const;

  bool operator<(const raidcandidate& r) const {
    // FIXME
    return false;
  }

  bool operator>(const raidcandidate& r) const {
    // FIXME
    return false;
  }
};


struct maxcandidate {
  const species* s;   // species
  std::string aname;  // attack name
  unsigned hlevel;    // halflevel
  bool gmaxpower;     // gmax/eternatus power scale? if not, dmax/crowned.
  bool hasstab;       // do we have stab for the attack?
  unsigned iva;       // attack iv 0..15
  float teffective;   // type effectiveness
  pgo_types_e atype;  // attack type
  unsigned maxlevel;  // [ 1..4 ]

  maxcandidate(const species *S, const std::string &Aname, unsigned Hlevel,
               bool Gmaxpower, bool Hasstab, unsigned Iva, float Teffective,
               pgo_types_e Atype, int Maxlevel) :
    s(S),
    aname(Aname),
    hlevel(Hlevel),
    gmaxpower(Gmaxpower),
    hasstab(Hasstab),
    iva(Iva),
    teffective(Teffective),
    atype(Atype),
    maxlevel(Maxlevel)
  { }

  float powprod(void) const {
    unsigned p = gmaxpower ? GMAX_POWER[maxlevel - 1] : DMAX_POWER[maxlevel - 1];
    float rp = hasstab ? calc_stab(p) : p;
    rp *= teffective;
    return rp * calc_eff_a(s->atk + iva, hlevel, false);
  }

  void emit(std::ostream &fp, unsigned maxp, bool unified, bool pdir) const;

  // only print a value in that column which corresponds to our typing
  // effectiveness, and do not scale it.
  void emit_p0(std::ostream &fp, unsigned maxp, bool unified, bool pdir) const;

  bool operator<(const maxcandidate& r) const {
    if(powprod() < r.powprod()){
      return true;
    }
    return false;
  }

  bool operator>(const maxcandidate& r) const {
    if(powprod() > r.powprod()){
      return true;
    }
    return false;
  }
};

bool pvp_banned_p(const species &s);
void emit_max_cand(const maxcandidate& c, unsigned maxp);
void print_species_latex(const species &s, bool overzoom, bool bg, bool mainform);
void filter_by_types(int t1, int t2, bool overzoom, bool mainform);
void add_raid_candidate(std::vector<raidcandidate>& cands, const species* s,
                        const attack *ca, bool unified);
void add_max_candidate(std::vector<maxcandidate>& cands, const species* s,
                       const std::string &aname, bool gmaxpower, bool stab,
                       pgo_types_e atype, float teffect, bool unified);
void emit_typing_list(pgo_types_e i, pgo_types_e j);
uint64_t pgo_xp_for_level(int l);
const char* max_attack_name(pgo_types_e t);
std::vector<const attack*>::const_iterator attacks_begin(void);
std::vector<const attack*>::const_iterator attacks_end(void);
void emit_attack(const species &s, const attack *a);
void summarize_buffs(const attack *a);

// can this species learn more than one charged attack?
bool multiple_charged_attacks(const species &s);

// look up any adventure effect for the attack name
std::string adventure_effect(const std::string& name);

const char *tname_capitalized(pgo_types_e t);

// can this species revert to a previous form?
bool bidirectional_evol_p(const species &s);

static inline const char*
tname_capitalized(int i){
  if(i < static_cast<int>(TYPESTART) || i >= static_cast<int>(TYPECOUNT)){
    return nullptr;
  }
  return tname_capitalized(static_cast<pgo_types_e>(i));
}

pgo_weather_t type_weather_boost(pgo_types_e t);

// a full typing can be boosted by one or two different types of weather.
// w1 and w2 will both hold valid weather typings, but they might be the same.
static inline void
typing_weather_boosts(pgo_types_e t1, pgo_types_e t2, pgo_weather_t &w1, pgo_weather_t &w2){
  w1 = type_weather_boost(t1);
  w2 = type_weather_boost(t2);
}

// how much candy to evolve *into* this form? returns 0 if we don't evolve
// using candy (including all megas).
unsigned evolcost(const species &s);

// emit a string fit for a latex file label
int label_string(const char *s);

#endif
