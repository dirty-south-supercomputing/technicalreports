#ifndef PGOTYPES_HTML
#define PGOTYPES_HTML

#include "pgotypes.h"
#include "fstream"

// use only for generating URIs, not filesystem paths nor display
static inline void
encode_name_uri(std::ostream &fp, const std::string &s){
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
link_to_name(std::ostream &fp, const std::string &s, bool subdir = false, int pdir = 0){
  fp << "<a href=\"";
  for(int i = 0 ; i < pdir ; ++i){
    fp << "../";
  }
  if(subdir){
    fp << "mon/";
  }
  if(s.starts_with("Shadow ")){
    encode_name_uri(fp, s.substr(7, s.length() - 7));
  }else{
    encode_name_uri(fp, s);
  }
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
    encode_name_uri(fp, s.substr(0, s.length() - 1));
  }else{
    encode_name_uri(fp, s);
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

// prints the typing icon(s), optionally a </td><td> pair (if closetd s true),
// and then the species name with link.
void species_with_icons(std::ostream &fp, const species &s, bool closetd, int pdir);

void html_icon(std::ostream &fp, const std::string &name, int pdir);

static inline void
html_type(std::ostream &fp, pgo_types_e t, int pdir){
  if(t != TYPECOUNT){
    html_icon(fp, tnames[t], pdir);
  }
}

// emit the symbols for some typing. nothing is shown for TYPECOUNT, and
// monotypes are only displayed once.
static inline void
html_types(std::ostream &fp, pgo_types_e t1, pgo_types_e t2, int pdir){
  html_type(fp, t1, pdir);
  if(t1 != t2 && t2 != TYPECOUNT){
    fp << ' ';
    html_type(fp, t2, pdir);
  }
}

static inline void
html_types(pgo_types_e t1, pgo_types_e t2, int pdir){
  html_types(std::cout, t1, t2, pdir);
}

void write_header(std::ostream &fp, const std::string &title, const std::string &headcheese, int pdir = 0);
void write_navbar(std::ostream &fp, int pdir = 0);
void write_navbar_selectopts(std::ostream &fp);
void write_footer(std::ostream &fp, int pdir = 0);
void write_footer_and_close(std::ofstream &fp, int pdir = 0);

void html_weather(std::ostream &fp, pgo_weather_t w, bool pdir = false);
void html_weather(std::ostream &fp, pgo_types_e t1, pgo_types_e t2, bool pdir = false);

extern const std::string css[];
extern const std::string reversecss[];

void summarize_buffs_html(std::ostream &o, const attack &a, const std::string &prefix, const std::string &suffix);

// results for max battle tank calculations. these summarize what happens
// during a single complete main phase. damage suffered may be a great
// deal more than we can actually take; damage inflicted could be more
// than the opponent can take.
struct maxtankcandidate {
  stats s;            // species / stats
  unsigned turns;     // turns until phase ends
  // dinf and dsuf are (for now) abstract numbers which can be sorted but ought
  // not be considered in terms of absolute values!
  double dinf;        // total damage inflicted (can exceed opponent HP)
  double dsuf;        // total damage suffered (can exceed our HP)
  const attack *fa;   // fast attack used
  const attack *ca;   // charged attack used, might be nullptr

  maxtankcandidate(const stats &S, unsigned Turns, double Dinf, double Dsuf,
                   const attack *Fa, const attack *Ca) :
    s(S),
    turns(Turns),
    dinf(Dinf),
    dsuf(Dsuf),
    fa(Fa),
    ca(Ca)
  { }

  void emit(std::ostream &fp, bool useca, bool pdir) const;

  // lesser means "is a worse max tank candidate"
  bool operator<(const maxtankcandidate& r) const {
    // prefer lower (turns * damage suffered)
    auto tdprod = turns * dsuf;
    auto rtdprod = r.turns * r.dsuf;
    if(tdprod > rtdprod){
      return true;
    }else if(rtdprod > tdprod){
      return false;
    }
    // prefer more damage inflicted
    if(dinf > r.dinf){
      return false;
    }else if(r.dinf > dinf){
      return true;
    }
    // use species name then fast attack name for tiebreaker
    auto c = std::tie(s.s->name, fa->name) < std::tie(r.s.s->name, r.fa->name);
    if(c){
      return c;
    }
    // finally, compare ca names, remembering ca might be null
    if(ca && !r.ca){
      return true;
    }else if(!ca && r.ca){
      return false;
    }else if(!ca && !r.ca){
      return false;
    }
    // ultimate tie shouldn't happen
    return ca->name < r.ca->name;
  }
};

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
int max_tank_counters(std::ostream &fp, const species &s, bool pdir);
int raid_counters(std::ostream &fp, const species &s, bool pdir);

// look up any unique battle mechanics for the species
std::string unique_mechanics(const std::string& name);

int write_family_iv_pages(const species &base, const std::string &prefix);

// javascript that hooks to keyboard events and, upon recognizing an IV,
// loads the family IV page corresponding to the current page.
// pass nullptr as encfam to remain in the current directory.
std::string ivsearchscript(const std::string *encfam);

#endif
