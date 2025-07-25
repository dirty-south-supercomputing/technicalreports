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

// don't want a turns column if extrema
static void header(bool extrema){
  std::cout << "\\begin{table}\\raggedright\\footnotesize\\centering\\begin{tabular}{lp{.4\\textwidth}";
  if(!extrema){
    std::cout << "rr";
  }
  std::cout << "rrr}Pokémon & Attacks & " << std::endl;
  if(!extrema){
    std::cout << "Turns & ";
  }
  std::cout << "Power & ";
  if(!extrema){
    std::cout << "\\textit{e} & ";
  }
  std::cout << "PPT & \\\%c \\\\" << std::endl;
  std::cout << "\\Midrule" << std::endl;
}

static void emit_line(bool extrema, const timetofirst &t, const std::string &prevname){
  std::cout << (prevname == t.s->name ? "" : t.s->name) << " & "
    << t.fa->name << " + " << t.ca->name << " & ";
  if(!extrema){
    std::cout << t.turns << " & ";
  }
  std::cout << t.dam << " & ";
  if(!extrema){
    if(t.excesse){
      std::cout << t.excesse << " & ";
    }
  }else{
    // we remove this column from the output because all 9 turn cycles
    // have the same excess energy: 1. so check that.
    assert(1 == t.excesse);
  }
  std::cout << t.dam / static_cast<float>(t.turns) << " & "
    << t.powercharged * 100 / t.dam
    << "\\\\" << std::endl;
}

static void footer(bool extrema, unsigned fastest){
  if(extrema){
    std::cout << "\\end{tabular}\\caption{Fastest (" << fastest << " turn) attack cycles\\label{table:fastcycles}}\\end{table}" << std::endl;
  }else{
    std::cout << "\\end{tabular}\\caption{Power and time of attack cycles\\label{table:cycles}}\\end{table}" << std::endl;
  }
}

// if given the argument "extrema", generate table of only the fastest
// cycles, otherwise a table of all cycles.
int main(int argc, char **argv){
  bool extrema = false;
  if(argc != 1){
    if(argc != 2){
      usage(argv[0]);
    }
    if(strcmp(argv[1], "extrema")){
      usage(argv[0]);
    }
    extrema = true;
  }
  std::vector<timetofirst> ttfs;
  // we don't want max nor mega
  struct spokedex smain = { sdex, SPECIESCOUNT, };
  header(extrema);
  calctimetoall(smain, ttfs);
  std::sort(ttfs.begin(), ttfs.end());
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout.precision(2);
  std::cout << std::noshowpoint;
  std::string prevname;
  unsigned fastest = 0;
  for(const auto &t : ttfs){
    if(extrema && fastest && t.turns > fastest){
      break;
    }else if(!fastest){
      fastest = t.turns;
    }
    emit_line(extrema, t, prevname);
    prevname = t.s->name;
  }
  footer(extrema, fastest);
  return EXIT_SUCCESS;
}
