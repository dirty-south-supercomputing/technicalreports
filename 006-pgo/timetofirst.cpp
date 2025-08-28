// determine number of turns before first charged attack
#include "pgotypes.cpp"
#include <vector>
#include <cassert>
#include <algorithm>

struct timetofirst {
  const species *s;
  unsigned turns;  // turns until first charged attack
  float powerfast; // cumulative power delivered by fast attacks (includes STAB)
  float powercharged; // power of charged attack (includes STAB)
  float dam;       // total power of cycle (includes STAB)
  float dpt;       // dam / turns
  const attack *fa;
  const attack *ca;
  unsigned excesse;// excess energy following charged move

  timetofirst(const species *S, unsigned Turns, float Powerfast,
              const attack *FA, const attack *CA) :
    s(S),
    turns(Turns),
    powerfast(Powerfast),
    fa(FA),
    ca(CA)
    {
        excesse = ((turns - 1) / fa->turns * fa->energytrain) % -ca->energytrain;
        powercharged = has_stab_p(s, ca) ? calc_stab(ca->powertrain) : ca->powertrain;
        dam = powerfast + powercharged;
        dpt = dam / static_cast<float>(turns);
    }

  friend bool operator <(const timetofirst &l, const timetofirst& r) {
    return l.turns < r.turns ? true : // least to most turns
      (l.turns == r.turns && l.dam > r.dam) ? true : // most to least powerful
      (l.turns == r.turns && l.dam == r.dam && l.s->name < r.s->name) ? true : false;
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
    for(const auto &f : s.attacks){
      if(f->energytrain <= 0){
        continue;
      }
      for(const auto &c : s.attacks){
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

static void usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " [ extrema ]" << std::endl;
  exit(EXIT_FAILURE);
}

// don't want a turns column if extrema && !powertbl
static void header(bool extrema, bool powertbl){
  if(extrema){
    std::cout << "\\begin{table}\\setlength{\\tabcolsep}{2pt}\\raggedright\\footnotesize\\centering\\begin{tabular}{ll";
  }else{
    std::cout << "\\begin{longtable}{ll";
    //\\setlength{\\tabcolsep}{2pt}\\raggedright\\footnotesize\\centering\\begin{tabular}{ll";
  }
  if(!extrema || powertbl){
    std::cout << "rr";
  }
  std::cout << "rrr}Pokémon & Attacks & " << std::endl;
  if(!extrema || powertbl){
    std::cout << "Turns & ";
  }
  std::cout << "Power & ";
  if(!extrema || powertbl){
    std::cout << "\\textit{e} & ";
  }
  std::cout << "PPT & \\\%c \\\\" << std::endl;
  std::cout << "\\Midrule" << std::endl;
}

static void emit_name(const std::string &s){
  for(char c : s){
    if(c == '%'){
      std::cout << "\\%";
    }else{
      std::cout << c;
    }
  }
}

static void emit_line(bool extrema, bool powertbl, const timetofirst &t, const std::string &prevname){
  if(prevname != t.s->name){
    emit_name(t.s->name);
  }
  std::cout << " & ";
  emit_attack(t.s, t.fa);
  std::cout << " + ";
  emit_attack(t.s, t.ca);
  std::cout << " & ";
  if(!extrema || powertbl){
    std::cout << t.turns << " & ";
  }
  std::cout << t.dam << " & ";
  if(!extrema || powertbl){
    if(t.excesse){
      std::cout << t.excesse;
    }
    std::cout << " & ";
  }else{
    // we remove this column from the output because all 9 turn cycles
    // have the same excess energy: 1. so check that.
    assert(1 == t.excesse);
  }
  std::cout << t.dpt << " & "
    << t.powercharged * 100 / t.dam
    << "\\\\" << std::endl;
}

static void footer(bool extrema, bool powertbl, unsigned fastest){
  if(powertbl && extrema){
    std::cout << "\\end{tabular}\\caption{Attack cycles with highest PPT\\label{table:pptcycles}}\\end{table}" << std::endl;
  }else if(extrema){
    std::cout << "\\end{tabular}\\caption{Fastest (" << fastest << " turn) attack cycles\\label{table:fastcycles}}\\end{table}" << std::endl;
  }else{
    std::cout << "\\caption{Power and time of attack cycles\\label{table:cycles}}\\end{longtable}" << std::endl;
  }
}

static bool damagecmp(timetofirst &l, timetofirst &r){
  if(l.dpt > r.dpt){
    return true;
  }
  return false;
}

// if given the argument "extrema", generate table of only the fastest cycles.
// if given the argument "damage", generate table of only the most powerful cycles.
// otherwise a table of all cycles.
int main(int argc, char **argv){
  bool extrema = false;
  bool powertbl = false;
  const char *argv0 = *argv;
  if(argc < 1 || argc > 3){
    usage(argv0);
  }
  while(*++argv){
    if(strcmp(*argv, "extrema") == 0){
      extrema = true;
    }else if(strcmp(*argv, "damage") == 0){
      powertbl = true;
    }else{
      usage(argv0);
    }
  }
  std::vector<timetofirst> ttfs;
  // we don't want max nor mega
  struct spokedex smain = { sdex, SPECIESCOUNT, };
  header(extrema, powertbl);
  calctimetoall(smain, ttfs);
  if(powertbl){
    std::sort(ttfs.begin(), ttfs.end(), damagecmp);
  }else{
    std::sort(ttfs.begin(), ttfs.end());
  }
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout.precision(2);
  std::cout << std::noshowpoint;
  std::string prevname;
  unsigned fastest = 0;
  constexpr float PPT_THRESHOLD = 12.65;
  for(const auto &t : ttfs){
    if(!powertbl){
      if(extrema && fastest && t.turns > fastest){
        break;
      }else if(!fastest){
        fastest = t.turns;
      }
    }else{
      if(extrema && t.dpt < PPT_THRESHOLD){
        break;
      }
    }
    emit_line(extrema, powertbl, t, prevname);
    prevname = t.s->name;
  }
  footer(extrema, powertbl, fastest);
  return EXIT_SUCCESS;
}
