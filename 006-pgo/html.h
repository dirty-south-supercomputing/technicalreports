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

#endif
