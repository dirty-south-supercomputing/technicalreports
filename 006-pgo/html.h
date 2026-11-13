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
encode_name(const std::string &s, std::string &encname){
  for(char c : s){
    if(!isspace(c) && (c == '+' || !ispunct(c))){
      encname.push_back(c);
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

// prints the type icon prior to the link to the attack, using a nonbreaking
// space between them to suppress line breaking.
void attack_with_icon(std::ostream &fp, const species &s, const attack *a,
                      bool subdir, bool pdir);

static inline void
html_type(std::ostream &fp, pgo_types_e t, bool pdir){
  if(t != TYPECOUNT){
    fp << "<img src=\"";
    if(pdir){
      fp << "../";
    }
    fp << "images/" << tnames[t] << ".png\" class=\"type\" alt=\""<< tnames[t] << "\"/>";
  }
}

static inline void
html_type(pgo_types_e t, bool pdir){
  html_type(std::cout, t, pdir);
}

// emit the symbols for some typing. nothing is shown for TYPECOUNT, and
// monotypes are only displayed once.
static inline void
html_types(std::ostream &fp, pgo_types_e t1, pgo_types_e t2, bool pdir){
  html_type(fp, t1, pdir);
  if(t1 != t2 && t2 != TYPECOUNT){
    fp << ' ';
    html_type(fp, t2, pdir);
  }
}

static inline void
html_types(pgo_types_e t1, pgo_types_e t2, bool pdir){
  html_types(std::cout, t1, t2, pdir);
}

void write_header(std::ostream &fp, const std::string &title, const std::string &style, bool pdir = false);
void write_navbar(std::ostream &fp, bool pdir = false);
void write_footer(std::ostream &fp, bool pdir = false);
void write_footer_and_close(std::ofstream &fp, bool pdir = false);

void html_weather(std::ostream &fp, pgo_weather_t w, bool pdir = false);
void html_weather(std::ostream &fp, pgo_types_e t1, pgo_types_e t2, bool pdir = false);

extern const std::string css[];
extern const std::string reversecss[];

void summarize_buffs_html(std::ostream &o, const attack &a, const std::string &prefix, const std::string &suffix);

struct timetofirst {
  const species *s;
  unsigned turns;     // turns through first charged attack
  float powerfast;    // cumulative power delivered by fast attacks (includes STAB)
  float powercharged; // power of charged attack (includes STAB)
  float dam;          // total power of cycle (includes STAB)
  float dpt;          // dam / turns
  const attack *fa;
  const attack *ca;
  unsigned excesse;   // excess energy following charged move

  timetofirst(const species *S, unsigned Turns, float Powerfast,
              float Powercharged, const attack *FA, const attack *CA,
              bool modeNx1) :
    s(S),
    turns(Turns),
    powerfast(Powerfast),
    powercharged(Powercharged),
    fa(FA),
    ca(CA)
    {
      unsigned ftime = modeNx1 ? fa->animdur : fa->turns;
      unsigned fenergy = modeNx1 ? fa->energyraid : fa->energytrain;
      unsigned cenergy = modeNx1 ? ca->energyraid : -ca->energytrain;
      excesse = ((turns - 1) / ftime * fenergy) % cenergy;
      dam = powerfast + powercharged;
      dpt = dam / static_cast<float>(turns);
    }

  friend bool operator <(const timetofirst &l, const timetofirst& r) {
    return l.turns < r.turns ? true : // least to most turns
      (l.turns == r.turns && l.dam > r.dam) ? true : // most to least powerful
      (l.turns == r.turns && l.dam == r.dam && l.s->idx < r.s->idx) ? true : false;
  }

  void emit(std::ostream &fp, double maxp, bool pdir) const;

};

void add_raid_candidate(std::vector<timetofirst> &cands, const species *s, unsigned turns,
                        float powerfast, float powercharged,
                        const attack *fa, const attack *ca);
void calctimetos3x3(std::vector<timetofirst> &ttfs, const species &s, bool addreturn);
void calctimetosNx1(std::vector<timetofirst> &ttfs, const species &s, bool addreturn);

struct cmprange {
  std::string sname;
  double effamin;
  double effamax;

  void emit(std::ostream &fp, int index) const {
    auto y = index * 20;
    auto h = 10;
    auto r = effamax - effamin;
    fp << "<rect x=\"" << effamax << "\" y=\"" << y << "\" height=\"" << h << "px\" width=\"" << r << "\"/>" << std::endl;
    auto textx = effamax + r + 10;
    auto texty = y - h;
    fp << "<text class=\"tlabel\" y=\"" << texty << "\" x=\"";
    fp << textx << "\">";
    fp << sname;
    fp << "</text>" << std::endl;
  }

  friend bool operator<(const cmprange &lhs, const cmprange &rhs){
    if(lhs.effamax < rhs.effamax){
      return true;
    }else if(rhs.effamax < lhs.effamax){
      return false;
    }else if(lhs.effamin < rhs.effamin){
      return true;
    }else if(rhs.effamin < lhs.effamin){
      return false;
    }
    return false; // equal?
  }
};

void calc_cmp_graph(std::vector<cmprange> &effas, int cpthresh, int cpbound);
void emit_cmp_graph(std::ostream &fp, const std::vector<cmprange> &effas);

int max_counters(std::ostream &fp, const species &s, bool pdir);
int raid_counters(std::ostream &fp, const species &s, bool pdir);

#endif
