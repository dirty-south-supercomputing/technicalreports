#ifndef W_INDEX
#define W_INDEX

#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>

// write toplevel index.html
int write_index(int dfd, int fd);

// use c string, as we need feed it to open(2), possibly deleting
// characters, appending prefix/suffix if non-null
static inline char *
file_escape_str(const std::string &s, const char* prefix, const char* suffix){
  auto slen = s.length();
  size_t sfxlen = suffix ? strlen(suffix) : 0;
  size_t pfxlen = prefix ? strlen(prefix) : 0;
  char *esc = static_cast<char*>(malloc(pfxlen + slen + 1 + sfxlen));
  if(esc){
    size_t off = 0;
    if(prefix){
      strcpy(esc, prefix);
      off += pfxlen;
    }
    for(size_t curs = 0 ; curs < slen ; ++curs){
      if(s[curs] != '\'' && s[curs] != '%'){
        esc[off++] = s[curs];
      }
    }
    esc[off] = '\0';
    if(suffix){
      strcpy(esc + off, suffix);
    }
  }
  return esc;
}

#endif
