#include "pgotypes.cpp"
#include <vector>
#include <cassert>
#include <algorithm>

// list of forms+configs ordered by experimental stat:
//
// MHP * Eff_D * Eff_A * D_cycle / T_cycle

struct timetofirst {
  const species *s;
  unsigned turns;  // turns until first charged attack
  float powerfast; // cumulative power delivered by fast attacks (includes STAB)
  float powercharged; // power of charged attack (includes STAB)
  float dam;       // total power of cycle (includes STAB)
  const attack *fa;
  const attack *ca;
  unsigned ia, id, is, hlevel;
  unsigned excesse;// excess energy following charged move

  timetofirst(const species *S, const stats *St, unsigned Turns, float Powerfast,
              const attack *FA, const attack *CA) :
    s(S),
    turns(Turns),
    powerfast(Powerfast),
    fa(FA),
    ca(CA)
    {
      ia = St->ia;
      id = St->id;
      is = St->is;
      hlevel = St->hlevel;
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

static void
calctimespecies(const species &s, std::vector<timetofirst> &ttfs, int bound){
  stats *st, maxstat;
  if(bound){
    st = find_optimal_set(&s, bound, 0, false, false);
    if(!st){
      std::cerr << "couldn't find optimal config for " << s.name << " with " << bound << " bound" << std::endl;
      throw std::exception();
    }
  }else{
    maxstat.ia = maxstat.id = maxstat.is = MAXIVELEM;
    maxstat.hlevel = MAX_HALFLEVEL;
    st = &maxstat;
  }
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
      ttfs.emplace_back(&s, st, t, pfast, f, c);
    }
  }
}

// get time to first and damage for all fast+charged pairs
static void
calctimetoall(const struct spokedex &sd, std::vector<timetofirst> &ttfs, int bound){
  for(unsigned si = 0 ; si < sd.dcount ; ++si){
    const auto &s = sd.dex[si];
    calctimespecies(s, ttfs, bound);
  }
}

static void usage(const char *argv0){
  std::cerr << "usage: " << argv0 << std::endl;
  exit(EXIT_FAILURE);
}

static void header(void){
  std::cout << "\\begin{longtable}{lllrrrrr}Pokémon & Config & Attacks & ";
  std::cout << "Turns & ";
  std::cout << "Power & ";
  std::cout << "\\textit{e} & ";
  std::cout << "PPT & \\\%c \\\\" << std::endl;
  std::cout << "\\endhead" << std::endl;
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

static void out_type(pgo_types_e t){
  if(t != TYPECOUNT){
    std::cout << "\\calign{\\includegraphics[height=1em,keepaspectratio]{images/" << tnames[t] << ".png}}";
    //std::cout << "\\includegraphics{images/" << tnames[t] << ".png}";
  }
}

static void emit_line(const timetofirst &t, const std::string &prevname){
  if(prevname != t.s->name){
    emit_name(t.s->name);
  }
  std::cout << " & \\ivlev{" << t.ia << "}{" << t.id << "}{" << t.is << "}{" << t.hlevel << "}&";
  out_type(t.fa->type);
  std::cout << t.fa->name << " + ";
  out_type(t.ca->type);
  std::cout << t.ca->name << " & ";
  std::cout << t.turns << " & ";
  std::cout << t.dam << " & ";
  if(t.excesse){
    std::cout << t.excesse;
  }
  std::cout << " & " << t.dam / static_cast<float>(t.turns) << " & "
    << t.powercharged * 100 / t.dam
    << "\\\\" << std::endl;
}

static void footer(void){
  std::cout << "\\caption{Power and time of attack cycles\\label{table:cycles}}\\end{longtable}" << std::endl;
}

int main(int argc, char **argv){
  int bound;
  if(argc > 2){
    usage(argv[0]);
  }else if(argc < 2){
    bound = 0;
    std::cout << "No CP bound." << std::endl;
  }else{
    bound = atoi(argv[1]);
    if(bound <= 0){
      usage(argv[0]);
    }
    std::cout << "\\textbf{CP bound is " << bound << ".}" << std::endl;
  }
  std::vector<timetofirst> ttfs;
  // we don't want max nor mega
  struct spokedex smain = { sdex, SPECIESCOUNT, };
  header();
  calctimetoall(smain, ttfs, bound);
  std::sort(ttfs.begin(), ttfs.end());
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout.precision(1);
  std::cout << std::noshowpoint;
  std::string prevname;
  for(const auto &t : ttfs){
    emit_line(t, prevname);
    prevname = t.s->name;
  }
  footer();
  return EXIT_SUCCESS;
}
