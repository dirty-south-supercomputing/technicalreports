// determine number of turns before first charged attack
#include "pgotypes.cpp"
#include <vector>
#include <algorithm>

struct timetofirst {
  const species *s;
  unsigned turns;  // turns until first charged attack
  float powerfast; // cumulative power delivered by fast attacks (includes STAB)
  const attack *fa;
  const attack *ca;

  timetofirst(const species *S, unsigned Turns, float Powerfast,
              const attack *FA, const attack *CA) :
    s(S),
    turns(Turns),
    powerfast(Powerfast),
    fa(FA),
    ca(CA)
    { }

  friend bool operator <(const timetofirst &l, const timetofirst& r) {
    float ltdam = l.powerfast + has_stab_p(l.s, l.ca) ? calc_stab(l.ca->powertrain) : l.ca->powertrain;
    float rtdam = r.powerfast + has_stab_p(r.s, r.ca) ? calc_stab(r.ca->powertrain) : r.ca->powertrain;
    return l.turns < r.turns ? true : // least to most turns
      (l.turns == r.turns && ltdam > rtdam) ? true : // most to least powerful
      (l.turns == r.turns && ltdam == rtdam && l.s->name < r.s->name) ? true : false;
  }
};

// find the number of turns necessary to reach e energy
static inline unsigned
turns_until_e(const attack *a, unsigned e){
  return (e + (a->energytrain - 1)) / a->energytrain * a->turns;
}

// get time to first and damage for all fast+charged pairs
static void
calctimetoall(const struct spokedex &sd, std::vector<timetofirst> &ttfs){
  for(unsigned si = 0 ; si < sd.dcount ; ++si){
    const auto &s = sd.dex[si];
    const attack *f;
    for(unsigned af = 0 ; (f = s.attacks[af]) ; ++af){
      if(f->energytrain <= 0){
        continue;
      }
      const attack *c;
      for(unsigned ac = 0 ; (c = s.attacks[ac]) ; ++ac){
        if(c->energytrain >= 0){
          continue;
        }
        unsigned t = turns_until_e(f, -c->energytrain);
        float power = f->powertrain;
        if(has_stab_p(&s, f)){
          power = calc_stab(power);
        }
        float pfast = t / f->turns * power;
        ++t; // account for the charged attack
        ttfs.emplace_back(&s, t, pfast, f, c);
      }
    }
  }
}

int main(int argc, char **argv){
  std::vector<timetofirst> ttfs;
  // we don't want max nor mega
  struct spokedex smain = { sdex, SPECIESCOUNT, };
  calctimetoall(smain, ttfs);
  std::sort(ttfs.begin(), ttfs.end());
  for(const auto &t : ttfs){
    float cpower = t.ca->powertrain;
    if(has_stab_p(t.s, t.ca)){
      cpower = calc_stab(cpower);
    }
    float dam = t.powerfast + cpower;
    std::cout << t.s->name << ": " << t.turns << " "
      << (has_stab_p(t.s, t.fa) ? calc_stab(t.fa->powertrain) : t.fa->powertrain) << " "
      << t.powerfast << " "
      << " " << t.fa->energytrain << " " << t.fa->turns << " " << " "
      << t.ca->energytrain << " " << cpower << " (" << t.fa->name << " + " << t.ca->name << " = "
      << dam << ")"
      << " dpt: " << dam / (float)t.turns
      << " c%: " << cpower * 100 / static_cast<float>(dam)
      << std::endl;
  }
  return EXIT_SUCCESS;
}
