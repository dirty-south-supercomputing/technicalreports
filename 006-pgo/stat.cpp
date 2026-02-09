#include "pgotypes.h"
#include <vector>
#include <cassert>
#include <algorithm>

// list of forms+configs ordered by experimental stat:
//
// MHP * Eff_D * Eff_A * D_cycle / T_cycle

// chance is out of 1000
static inline float halfbuff(int chance, int bufflevel){
  if(!chance){
    return 1;
  }
  float b = mapbuff(bufflevel);
  if(b > 1){
    b = 1 + ((b - 1) / 2);
  }else if(b < 1){
    b = 1 - ((1 - b) / 2);
  }
  std::cerr << "b: " << b << " halfbuff: " << (b * (chance / 1000.0)) << std::endl;
  return b * (chance / 1000.0);
}

struct timetofirst {
  const species *s;
  unsigned turns;  // turns until first charged attack
  float powerfast; // cumulative power delivered by fast attacks (includes STAB)
  float powercharged; // power of charged attack (includes STAB)
  float dam;       // total power of cycle (includes STAB)
  const attack *fa;
  const attack *ca;
  unsigned ia, id, is, hlevel;
  float effa, effd;
  float aprod; // effa * dam
  float pppt;  // aprod / (T*T/bulk)
  float abuff, dbuff; // multiplier, half of any buff due charged attack
  unsigned mhp;
  float bulk;  // geommean(mhp, effd)
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
      effa = calc_eff_a(s->atk + ia, hlevel, false);
      effd = calc_eff_d(s->def + id, hlevel, false);
      excesse = ((turns - 1) / fa->turns * fa->energytrain) % -ca->energytrain;
      powercharged = has_stab_p(s, ca) ? calc_stab(ca->powertrain) : ca->powertrain;
      abuff = ca->user_attack;
      dbuff = ca->user_defense;
      dam = powerfast + powercharged;
      aprod = sqrt(dam * effa * halfbuff(ca->chance_user_attack, ca->user_attack));
      mhp = calc_mhp(s->sta + is, hlevel);
      bulk = sqrt(mhp * effd * halfbuff(ca->chance_user_defense, ca->user_defense));
      pppt = (aprod * bulk) / pow(turns, 0.7);
      std::cerr << S->name << " aprod: " << aprod << " bulk: " << bulk << " pppt: " << pppt << std::endl;
    }

  friend bool operator <(const timetofirst &l, const timetofirst& r) {
    return l.pppt > r.pppt ? true
      : (l.pppt == r.pppt && l.s->name < r.s->name) ? true
      : false;
    /*return l.turns < r.turns ? true : // least to most turns
      (l.turns == r.turns && l.aprod > r.aprod) ? true : // most to least powerful
      (l.turns == r.turns && l.aprod == r.aprod && l.s->name < r.s->name) ? true : false;*/
  }
};

// find the number of turns necessary to reach e energy
static inline unsigned
turns_until_e(const attack *a, unsigned e){
  return (e + (a->energytrain - 1)) / a->energytrain * a->turns;
}

static void
calctimespecies(const species &s, std::vector<timetofirst> &ttfs, int bound){
  stats maxstat(&s, MAX_HALFLEVEL, MAXIVELEM, MAXIVELEM, MAXIVELEM, false);
  stats *st;
  if(bound){
    st = find_optimal_set(&s, bound, 0, false, calc_pok_gmean);
    if(!st){
      std::cerr << "couldn't find optimal config for " << s.name << " with " << bound << " bound" << std::endl;
      throw std::exception();
    }
  }else{
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
    if(s.name == "Aegislash"){
      continue; // FIXME
    }
    calctimespecies(s, ttfs, bound);
  }
}

static void usage(const char *argv0){
  std::cerr << "usage: " << argv0 << std::endl;
  exit(EXIT_FAILURE);
}

static void header(bool configcolumn){
  std::cout << "\\begin{longtable}{c";
  if(configcolumn){
    std::cout << "l";
  }
  std::cout << "lrrrlrrrrrrr}" << std::endl;
  std::cout << "\\textbf{T} & ";
  if(configcolumn){
    std::cout << "\\textbf{Config} & ";
  }
  std::cout << "\\textbf{Pokémon} & \\textbf{HP} & \\textbf{$\\textrm{Eff}\\textsubscript{D}$} & \\textbf{DR} & \\textbf{Attack pair} & \\textbf{T} & ";
  std::cout << "\\textbf{Power} & \\textbf{$\\textrm{Eff}\\textsubscript{A}$} & \\textbf{DI} & ";
  std::cout << "\\textbf{\\textit{e}} & ";
  std::cout << "\\textbf{Dank} & \\textbf{\\\%c} \\\\" << std::endl;
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
  }
}

static void emit_line(const timetofirst &t, bool configcolumn){
  out_type(t.s->t1);
  out_type(t.s->t2);
  if(configcolumn){
    unsigned hl;
    unsigned l = halflevel_to_level(t.hlevel, &hl);
    std::cout << " & \\ivlev{" << t.ia << "}{" << t.id << "}{" << t.is << "}{" << l;
    if(hl){
      std::cout << ".5";
    }
    std::cout << "}";
  }
  std::cout << "&";
  emit_name(t.s->name);
  std::cout << " & ";
  std::cout << t.mhp << " & ";
  std::cout << t.effd << " & ";
  std::cout << t.bulk << " & ";
  out_type(t.fa->type);
  emit_attack(t.s, t.fa);
  std::cout << " ";
  out_type(t.ca->type);
  emit_attack(t.s, t.ca);
  std::cout << " & ";
  std::cout << t.turns << " & ";
  std::cout << t.dam << " & ";
  std::cout << t.effa << " & ";
  std::cout << t.aprod << " & ";
  if(t.excesse){
    std::cout << t.excesse;
  }
  std::cout << " & " << t.pppt << " & "
    << t.powercharged * 100 / t.dam
    << "\\\\" << std::endl;
}

static void footer(void){
  std::cout << "\\end{longtable}" << std::endl;
}

int main(int argc, char **argv){
  int bound;
  if(argc > 2){
    usage(argv[0]);
  }else if(argc < 2){
    bound = 0;
    std::cout << "\\textbf{No CP bound.}" << std::endl;
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
  header(!!bound);
  calctimetoall(smain, ttfs, bound);
  std::sort(ttfs.begin(), ttfs.end());
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout.precision(1);
  std::cout << std::noshowpoint;
  for(const auto &t : ttfs){
    emit_line(t, !!bound);
  }
  footer();
  std::cout << "\\clearpage" << std::endl;
  return EXIT_SUCCESS;
}
