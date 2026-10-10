#ifndef PGOTYPES_HTML
#define PGOTYPES_HTML

#include "pgotypes.h"

static inline void
encode_name(std::ostream &fp, const std::string &s){
  for(char c : s){
    if(!isspace(c) && !ispunct(c)){
      fp << c;
    }
  }
}

static inline int
link_to_name(std::ostream &fp, const std::string &s, bool subdir = false){
  fp << "<a href=\"";
  if(subdir){
    fp << "mon/";
  }
  encode_name(fp, s);
  fp << ".html\">" << s << "</a>";
  return 0;
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
                       bool pdir = false){
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
  o << prefix << "<a href=\"";
  if(pdir){
    o << "../";
  }
  o << "attacks/";
  encode_name(o, a->name);
  o << ".html\">" << a->name << "</a>" << suffix;
}

static inline void
emit_html_attack(std::ostream &o, const species &s, const attack *a){
  std::string prefix, suffix;
  emit_html_attack_fancy(o, s, a, prefix, suffix);
}

// FIXME ugh duplicates summarize_buffs() from latex code
static inline void
summarize_buffs_html(std::ostream &o, const attack *a){
  // need special case A+D as it takes too much space otherwise
  if(a->chance_user_attack && a->chance_user_attack == a->chance_user_defense
      && a->user_attack == a->user_defense){
    print_buff_html(o, a->chance_user_attack, a->user_attack, "A+D");
  }else{
    if(a->chance_user_attack){
      print_buff_html(o, a->chance_user_attack, a->user_attack, "A");
    }
    if(a->chance_user_defense){
      print_buff_html(o, a->chance_user_defense, a->user_defense, "D");
    }
  }
  if(a->chance_opp_attack && a->chance_opp_attack == a->chance_opp_defense
      && a->opp_attack == a->opp_defense){
    print_buff_html(o, a->chance_opp_attack, a->opp_attack, "OA+D");
  }else{
    if(a->chance_opp_attack){
      print_buff_html(o, a->chance_opp_attack, a->opp_attack, "OA");
    }
    if(a->chance_opp_defense){
      print_buff_html(o, a->chance_opp_defense, a->opp_defense, "OD");
    }
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

#endif
