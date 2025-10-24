#ifndef W_INDEX
#define W_INDEX

#include <string>
#include <cstdlib>
#include <cstring>

int write_index(int fd);

// use c string, as we need feed it to open(2)
static inline char *
file_escape_str(const std::string &s){
  auto slen = s.length();
  char *esc = static_cast<char*>(malloc(slen + 1));
  if(esc){
    size_t off = 0;
    for(size_t curs = 0 ; curs < slen ; ++curs){
      if(s[curs] != '\'' && s[curs] != '%'){
        esc[off++] = s[curs];
      }
    }
    esc[off] = '\0';
  }
  return esc;
}

#endif
