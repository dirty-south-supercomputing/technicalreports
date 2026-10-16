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

static inline void
link_to_attack(std::ostream &fp, const std::string &s, bool subdir = false, bool pardir = false){
  fp << "<a href=\"";
  if(pardir){
    fp << "../";
  }
  if(subdir){
    fp << "attacks/";
  }
  encode_name(fp, s);
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

// FIXME ugh duplicates summarize_buffs() from latex code
static inline void
summarize_buffs_html(std::ostream &o, const attack &a, const std::string &prefix, const std::string &suffix){
  // need special case A+D as it takes too much space otherwise
  bool printed = false;
  if(a.chance_user_attack && a.chance_user_attack == a.chance_user_defense
      && a.user_attack == a.user_defense){
    printed = true;
    o << prefix;
    print_buff_html(o, a.chance_user_attack, a.user_attack, "A+D");
  }else{
    if(a.chance_user_attack){
      printed = true;
      o << prefix;
      print_buff_html(o, a.chance_user_attack, a.user_attack, "A");
    }
    if(a.chance_user_defense){
      printed = true;
      o << prefix;
      print_buff_html(o, a.chance_user_defense, a.user_defense, "D");
    }
  }
  if(a.chance_opp_attack && a.chance_opp_attack == a.chance_opp_defense
      && a.opp_attack == a.opp_defense){
    if(!printed){
      printed = true;
      o << prefix;
    }
    print_buff_html(o, a.chance_opp_attack, a.opp_attack, "OA+D");
  }else{
    if(a.chance_opp_attack){
      if(!printed){
        printed = true;
        o << prefix;
      }
      print_buff_html(o, a.chance_opp_attack, a.opp_attack, "OA");
    }
    if(a.chance_opp_defense){
      if(!printed){
        printed = true;
        o << prefix;
      }
      print_buff_html(o, a.chance_opp_defense, a.opp_defense, "OD");
    }
  }
  if(printed){
    o << suffix;
  }
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

static inline void
html_type_pdir(std::ostream &fp, pgo_types_e t){
  if(t != TYPECOUNT){
    fp << "<img src=\"../images/" << tnames[t] << ".png\" class=\"type\" alt=\""<< tnames[t] << "\"/>";
  }
}

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

static inline void
write_header(std::ofstream &fp, const std::string &title){
  fp << "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  fp << "<link rel=\"stylesheet\" type=\"text/css\" href=\"../list.css\">";
  fp << "<title>" << title << "</title></head>" << std::endl;
  fp << "<body>" << std::endl;
}

static inline void
write_footer_and_close(std::ofstream &fp){
  fp << "<hr/><a href=\"https://goldandappelpub.com/pgo-quantitative.html\"><i>Pokémon GO: A Quantitative Approach</i></a>";
  fp << "</body></html>" << std::endl;
  fp.close(); // FIXME check errors or enable iostream exceptions
}

static inline void
encode_name(const std::string &s, std::string &encname){
  for(char c : s){
    if(!isspace(c) && (c == '+' || !ispunct(c))){
      encname.push_back(c);
    }
  }
}

extern const std::string css[];

#endif
