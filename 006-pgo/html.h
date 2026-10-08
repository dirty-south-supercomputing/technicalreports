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

#endif
