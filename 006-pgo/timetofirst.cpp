// determine number of turns before first charged attack
#include "pgotypes.cpp"
#include <vector>
#include <algorithm>

struct timetofirst {
  std::string name;
  unsigned turns;     // turns until first charged attack
  unsigned powerfast; // cumulative power delivered by fast attacks
  const attack *fa;
  const attack *ca;

  timetofirst(const std::string &Name, unsigned Turns, unsigned Powerfast,
              const attack *FA, const attack *CA) :
    name(Name),
    turns(Turns),
    powerfast(Powerfast),
    fa(FA),
    ca(CA)
    { }

  friend bool operator <(const timetofirst &l, const timetofirst& r) {
    unsigned ltdam = l.powerfast + l.ca->powertrain;
    unsigned rtdam = r.powerfast + r.ca->powertrain;
    return l.turns < r.turns ? true : // least to most turns
      (l.turns == r.turns && ltdam > rtdam) ? true : // most to least powerful
      (l.turns == r.turns && ltdam == rtdam && l.name < r.name) ? true : false;
  }
};

// find the attack which reaches e energy in the fewest turns, returning the
// total number of turns in *minturns.
static const attack *
fastest_attack(const species &s, unsigned e, unsigned *minturns){
  const attack *fastest = nullptr;
  unsigned turns = 0;
  for(unsigned ai = 0 ; s.attacks[ai] ; ++ai){
    const attack *a = s.attacks[ai];
    if(charged_attack_p(a) || !a->energytrain){
      continue;
    }
    unsigned t = (e + (a->energytrain - 1)) / a->energytrain * a->turns;
    if(!fastest || t < turns){
      turns = t;
      fastest = a;
    }
  }
  if(!fastest){
    return nullptr;
  }
  *minturns = turns;
  return fastest;
}

// find the charged attack requiring the minimum energy
static const attack *
lowest_energy_attack(const species &s){
  const attack *mine = NULL;
  for(unsigned ai = 0 ; s.attacks[ai] ; ++ai){
    const attack *a = s.attacks[ai];
    if(fast_attack_p(a)){
      continue;
    }
    if(!mine || -a->energytrain < -mine->energytrain){
      mine = a;
    }
  }
  return mine;
}

static void
calctimetofirst(const struct spokedex &sd, std::vector<timetofirst> &ttfs){
  for(unsigned si = 0 ; si < sd.dcount ; ++si){
    const auto &s = sd.dex[si];
    unsigned ttf;
    auto ca = lowest_energy_attack(s);
    auto fa = fastest_attack(s, -ca->energytrain, &ttf);
    if(!fa){
      continue;
    }
    unsigned pfast = ttf / fa->turns * fa->powertrain; 
    ttfs.emplace_back(s.name, ttf, pfast, fa, ca);
  }
}

int main(void){
  std::vector<timetofirst> ttfs;
  // we don't want max nor mega
  struct spokedex smain = { sdex, SPECIESCOUNT, };
  calctimetofirst(smain, ttfs);
  std::sort(ttfs.begin(), ttfs.end());
  for(const auto &t : ttfs){
    unsigned dam = t.powerfast + t.ca->powertrain;
    std::cout << t.name << ": " << t.turns << " " << t.powerfast << " "
      << " " << t.fa->energytrain << " " << t.fa->turns << " " << " "
      << t.ca->energytrain << " " << t.ca->powertrain << " (" << t.fa->name << " + " << t.ca->name << " = "
      << dam << ")"
      << " dpt: " << dam / (float)t.turns
      << std::endl;
  }
  return EXIT_SUCCESS;
}
