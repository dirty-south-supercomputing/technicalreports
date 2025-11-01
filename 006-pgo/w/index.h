#ifndef W_INDEX
#define W_INDEX

#include <string>
#include <cstdlib>
#include <cstring>

// write toplevel index.html
int write_index(int dfd, int fd);

// use c string, as we need feed it to open(2), possibly deleting
// characters, appending suffix if non-NULL
static inline char *
file_escape_str(const std::string &s, const char* suffix){
  auto slen = s.length();
  size_t sfxlen = suffix ? strlen(suffix) + 1 : 0;
  char *esc = static_cast<char*>(malloc(slen + 1 + sfxlen));
  if(esc){
    size_t off = 0;
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
