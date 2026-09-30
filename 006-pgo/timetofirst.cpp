// determine number of turns before first charged attack
#include "pgotypes.h"
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
      (l.turns == r.turns && l.dam == r.dam && l.s->idx < r.s->idx) ? true : false;
  }
};

// find the number of turns necessary to reach e energy
static inline unsigned
turns_until_e(const attack *a, unsigned e){
  return (e + (a->energytrain - 1)) / a->energytrain * a->turns;
}

static void
calctimetos(std::vector<timetofirst> &ttfs, const species& s){
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

// get time to first and damage for all fast+charged pairs
static void
calctimetoall(std::vector<timetofirst> &ttfs, std::vector<species> &megaspecs){
  for(unsigned si = 0 ; si < SPECIESCOUNT ; ++si){
    calctimetos(ttfs, sdex[si]);
  }
  for(unsigned mi = 0 ; mi < MEGACOUNT ; ++mi){
    megaspecs.emplace_back(megasdex[mi]);
  }
  for(const auto &m : megaspecs){
    calctimetos(ttfs, m);
  }
}

static void usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " [ extrema ]" << std::endl;
  exit(EXIT_FAILURE);
}

static void html_header(void){
  std::cout << "<table>" << std::endl;
  std::cout << "<tr>";
  std::cout << "<th>T</th><th>Pokémon</th><th>Attack pair</th><th>Turns</th><th>Power</th><th><i>e</i></th><th>PPT</th><th>%c</th>";
  std::cout << "</tr>" << std::endl;
}

// don't want a turns column if extrema
static void header(bool extrema){
  if(extrema){
    std::cout << "\\begingroup\\setlength{\\tabcolsep}{2pt}\\footnotesize\\begin{longtable}{ll";
  }else{
    std::cout << "\\begingroup\\footnotesize\\begin{longtable}{ll";
  }
  std::cout << "rrrrr}Pokémon & Attacks & Turns & Power & ";
  std::cout << "\\textit{e} & ";
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

// don't elide matching mon type for html (as we do latex)
static void emit_row(const timetofirst &t){
  std::cout << "<tr>";
  std::cout << "<td>";
  html_types(t.s->t1, t.s->t2);
  std::cout << "</td>";
  std::cout << "<td>" << t.s->name << "</td>";
  std::cout << "<td>";
  html_type(t.fa->type);
  std::cout << ' ';
  emit_html_attack(t.s, t.fa);
  std::cout << " + ";
  html_type(t.ca->type);
  std::cout << ' ';
  emit_html_attack(t.s, t.ca);
  summarize_buffs_html(t.ca);
  std::cout << "</td>";
  std::cout << "<td>" << t.turns << "</td>";
  std::cout.precision(1);
  std::cout << "<td>" << t.dam << "</td>";
  std::cout.precision(2);
  std::cout << "<td>";
  if(t.excesse){
    std::cout << t.excesse;
  }
  std::cout << "</td>";
  std::cout << "<td>" << t.dpt << "</td>";
  std::cout << "<td>" << (t.powercharged * 100 / t.dam) << "</td>";
  std::cout << "</tr>" << std::endl;
}

static void emit_line(const timetofirst &t, const std::string &prevname){
  if(prevname != t.s->name){
    emit_name(t.s->name);
  }
  std::cout << " & ";
  emit_attack(t.s, t.fa);
  std::cout << " + ";
  emit_attack(t.s, t.ca);
  std::cout << " & ";
  std::cout << t.turns << " & ";
  std::cout << t.dam << " & ";
  if(t.excesse){
    std::cout << t.excesse;
  }
  std::cout << " & ";
  std::cout << t.dpt << " & "
    << t.powercharged * 100 / t.dam
    << "\\\\" << std::endl;
}

static void footer(bool extrema, bool powertbl, unsigned fastest){
  if(powertbl && extrema){
    std::cout << "\\caption{Attack cycles with highest PPT\\label{table:pptcycles}}\\end{longtable}\\endgroup" << std::endl;
  }else if(extrema){
    std::cout << "\\caption{Fastest (" << fastest << " turn) attack cycles\\label{table:fastcycles}}\\end{longtable}\\endgroup" << std::endl;
  }else{
    std::cout << "</table>" << std::endl;
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
// otherwise a table of all cycles...in HTML, ugh.
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
  if(!extrema && !powertbl){
    html_header();
  }else{
    header(extrema);
  }
  std::vector<species> megaspecs; // backing storage for species boosted from megas
  calctimetoall(ttfs, megaspecs);
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
  constexpr float PPT_THRESHOLD = 13;
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
    if(!powertbl && !extrema){
      emit_row(t);
    }else{
      emit_line(t, prevname);
    }
    prevname = t.s->name;
  }
  footer(extrema, powertbl, fastest);
  return EXIT_SUCCESS;
}
