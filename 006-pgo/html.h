#ifndef PGOTYPES_HTML
#define PGOTYPES_HTML

#include "pgotypes.h"
#include "fstream"

// use only for generating URIs, not filesystem paths nor display
static inline void
encode_name(std::ostream &fp, const std::string &s){
  for(unsigned char c : s){ // handle utf8
    if(c > 0x7f){
      fp << '%' << std::hex << static_cast<int>(c) << std::dec;
    }else if(!isspace(c) && (c == '+' || !ispunct(c))){ // these get left out
      fp << c;
    }
  }
}

static inline void
link_to_name(std::ostream &fp, const std::string &s, bool subdir = false, bool pardir = false){
  fp << "<a href=\"";
  if(pardir){
    fp << "../";
  }
  if(subdir){
    fp << "mon/";
  }
  encode_name(fp, s);
  fp << ".html\">" << s << "</a>";
}

// we chop off the super mega level, so they all go to the same page
static inline void
link_to_attack(std::ostream &fp, const std::string &s, bool subdir = false, bool pardir = false){
  fp << "<a href=\"";
  if(pardir){
    fp << "../";
  }
  if(subdir){
    fp << "attacks/";
  }
  if(s.ends_with("+2") || s.ends_with("+3") || s.ends_with("+4")){
    encode_name(fp, s.substr(0, s.length() - 1));
  }else{
    encode_name(fp, s);
  }
  fp << ".html\">" << s << "</a>";
}

// input: halflevel (0..10x)
static inline std::ostream &
emit_halflevel_as_level(std::ostream &fp, int hlevel){
  unsigned half;
  unsigned l = halflevel_to_level(hlevel, &half);
  fp << l;
  if(half){
    fp << ".5";
  }
  return fp;
}

static inline void
emit_html_attack_fancy(std::ostream &o, const species &s, const attack *a,
                       std::string &prefix, std::string &suffix,
                       bool subdir = false, bool pardir = false){
  bool stab = has_stab_p(s, a);
  bool excl = exclusive_attack_p(s, a);
  prefix = "";
  suffix = "";
  if(!stab){
    prefix += "<i>";
    suffix += "</i>";
  }
  if(excl){
    prefix += "<b>";
    suffix = "</b>" + suffix;
  }
  o << prefix;
  link_to_attack(o, a->name, subdir, pardir);
  o << suffix;
}

static inline void
emit_html_attack(std::ostream &o, const species &s, const attack *a,
                 bool subdir = false, bool pardir = false){
  std::string prefix, suffix;
  emit_html_attack_fancy(o, s, a, prefix, suffix, subdir, pardir);
}

static inline void
html_type(std::ostream &fp, pgo_types_e t){
  if(t != TYPECOUNT){
    fp << "<img src=\"images/" << tnames[t] << ".png\" class=\"type\" alt=\""<< tnames[t] << "\"/>";
  }
}

static inline void
html_type(pgo_types_e t){
  html_type(std::cout, t);
}

void html_type_pdir(std::ostream &fp, pgo_types_e t);

static inline void
html_types_pdir(std::ostream &fp, pgo_types_e t1, pgo_types_e t2){
  html_type_pdir(fp, t1);
  if(t1 != t2 && t2 != TYPECOUNT){
    fp << ' ';
    html_type_pdir(fp, t2);
  }
}

// emit the symbols for some typing. nothing is shown for TYPECOUNT, and
// monotypes are only displayed once.
static inline void
html_types(std::ostream &fp, pgo_types_e t1, pgo_types_e t2){
  html_type(fp, t1);
  if(t1 != t2 && t2 != TYPECOUNT){
    fp << ' ';
    html_type(fp, t2);
  }
}

static inline void
html_types(pgo_types_e t1, pgo_types_e t2){
  html_types(std::cout, t1, t2);
}

void write_header(std::ofstream &fp, const std::string &title, bool par = false);
void write_navbar(std::ostream &fp, bool par = false);

static inline void
write_footer_and_close(std::ofstream &fp){
  fp << "<hr/>" << std::endl;
  fp << "<span style=\"float: left\"><a href=\"https://goldandappelpub.com/pgo-quantitative.html\"><i>Pokémon GO: A Quantitative Approach</i></a></span>";
  fp << "<span style=\"float: right\">© 2026 <a href=\"mailto:nickblack@linux.com\">nick black</a></span>" << std::endl;
  fp << "</body></html>" << std::endl;
  fp.close();
  if(fp.rdstate() & std::ofstream::failbit){
    std::cerr << "error writing to file" << std::endl;
    throw std::exception();
  }
}

static inline void
encode_name(const std::string &s, std::string &encname){
  for(char c : s){
    if(!isspace(c) && (c == '+' || !ispunct(c))){
      encname.push_back(c);
    }
  }
}

void html_weather(std::ostream &fp, pgo_weather_t w, bool par = false);
void html_weather(std::ostream &fp, pgo_types_e t1, pgo_types_e t2, bool par = false);

extern const std::string css[];
extern const std::string reversecss[];

void emit_cand_html(const candidate& c, unsigned maxp);
void summarize_buffs_html(std::ostream &o, const attack &a, const std::string &prefix, const std::string &suffix);

struct timetofirst {
  const species *s;
  unsigned turns;  // turns through first charged attack
  float powerfast; // cumulative power delivered by fast attacks (includes STAB)
  float powercharged; // power of charged attack (includes STAB)
  float dam;       // total power of cycle (includes STAB)
  float dpt;       // dam / turns
  const attack *fa;
  const attack *ca;
  unsigned excesse;// excess energy following charged move

  timetofirst(const species *S, unsigned Turns, float Powerfast,
              const attack *FA, const attack *CA, bool modeNx1) :
    s(S),
    turns(Turns),
    powerfast(Powerfast),
    fa(FA),
    ca(CA)
    {
      unsigned ftime = modeNx1 ? fa->animdur : fa->turns;
      unsigned fenergy = modeNx1 ? fa->energyraid : fa->energytrain;
      unsigned cenergy = modeNx1 ? ca->energyraid : -ca->energytrain;
      excesse = ((turns - 1) / ftime * fenergy) % cenergy;
      powercharged = modeNx1 ? calc_eff_power_nx1(*s, ca) : calc_eff_power(*s, ca);
      dam = powerfast + powercharged;
      dpt = dam / static_cast<float>(turns);
    }

  friend bool operator <(const timetofirst &l, const timetofirst& r) {
    return l.turns < r.turns ? true : // least to most turns
      (l.turns == r.turns && l.dam > r.dam) ? true : // most to least powerful
      (l.turns == r.turns && l.dam == r.dam && l.s->idx < r.s->idx) ? true : false;
  }
};

void calctimetos3x3(std::vector<timetofirst> &ttfs, const species &s);
void calctimetosNx1(std::vector<timetofirst> &ttfs, const species &s);

#endif
