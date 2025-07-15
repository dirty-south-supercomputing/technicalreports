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
    return l.turns < r.turns ? true : l.powerfast < r.powerfast ? true : false;
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
  *minturns = turns * fastest->powertrain;
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
    if(!mine || a->energytrain < mine->energytrain){
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
    auto fa = fastest_attack(s, ca->energytrain, &ttf);
    if(!fa){
      continue;
    }
    unsigned pfast = ttf / fa->turns * fa->powertrain; 
    std::cout << "adding " << ttf << " " << pfast << std::endl;
    ttfs.emplace(ttfs.end(), s.name, ttf, pfast, fa, ca);
  }
}

int main(void){
  std::vector<timetofirst> ttfs;
  for(const auto &sdex : sdexen){
    calctimetofirst(sdex, ttfs);
  }
  std::sort(ttfs.begin(), ttfs.end());
  for(const auto &t : ttfs){
    std::cout << t.name << ": " << t.turns << " " << t.powerfast << std::endl;
  }
  return EXIT_SUCCESS;
}
