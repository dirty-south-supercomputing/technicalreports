#include "pgotypes.h"
#include <memory>

static void
copy_tvec(int* tnew, const int tvec[]){
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    tnew[i] = tvec[i];
  }
}

// print tocc iff it covers tvec
// tvec is a v-sized vector of 2-vectors of ints
// if the first int is TYPECOUNT, it's not populated (we don't care)
static int
print_coverset(const int tocc[], int v, const int tvec[][2]){
  for(int i = 0 ; i < v ; ++i){
    if(tvec[i][0] != TYPECOUNT){ // it's populated; ensure we cover
      int covered = 0;
      for(int j = 0 ; j < TYPECOUNT ; ++j){
        if(tocc[j]){
          if(trelations[j][tvec[i][0]] > 0){
            // tocc[j] covers tvec[i][0] (others might also do so).
            // ensure it isn't invalidated by tvec[i][1]
            if(trelations[j][tvec[i][1]] >= 0){
              covered = 1;
              break;
            }
          }else if(trelations[j][tvec[i][0]] == 0){
            // tocc[j] isn't ineffective, so check if we cover the other
            if(trelations[j][tvec[i][1]] > 0){
              covered = 1;
              break;
            }
          }
        }
      }
      if(!covered){
        return 0;
      }
    }
  }
  // we were a cover, yay
  printf(" cover: ");
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    if(tocc[i]){
      printf("%s ", tnames[i]);
    }
  }
  printf("\n");
  return 1;
}

// we are placing n 1s among the last m slots of tocc
static int
place_n_in_m(int n, int m, const int tocc[], int v, const int tvec[][2]){
  auto tcopy = std::make_unique<int[]>(TYPECOUNT);
  copy_tvec(tcopy.get(), tocc);
  // base case m == 1
  if(m == 1){
    if(n){
      tcopy[TYPECOUNT - m] = 1;
    }
    int r = print_coverset(tcopy.get(), v, tvec);
    return r;
  }
  int ret = 0;
  // test with our bit off unless we must place this bit
  if(n < m){
    ret += place_n_in_m(n, m - 1, tcopy.get(), v, tvec);
  }
  // test with our bit on if we have a bit to place
  if(n){
    tcopy[TYPECOUNT - m] = 1;
    ret += place_n_in_m(n - 1, m - 1, tcopy.get(), v, tvec);
  }
  return ret;
}

// print the sets of size n or less which cover the specified vector
static int
print_coversets(int n, int v, const int tvec[][2]){
  int tocc[TYPECOUNT] = {};
  int r = place_n_in_m(n, TYPECOUNT, tocc, v, tvec);
  return r;
}

// print minimal coversets of the monotypes, and for each monotype, the minimal
// coversets for all monotypes except that one
static void
print_complete_coversets(void){
  int t[TYPECOUNT][2];
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    t[i][0] = i;
    t[i][1] = i;
  }
  for(int j = 1 ; j <= TYPECOUNT ; ++j){
    int min = print_coversets(j, sizeof(t) / sizeof(*t), t);
    if(min){
      printf("%d coversets of size %d\n", min, j);
      break;
    }
  }
  int prev[2];
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    if(i > 0){ // turn previous back on
      t[i - 1][0] = prev[0];
      t[i - 1][1] = prev[0];
    }
    prev[0] = t[i][0];
    prev[1] = t[i][1];
    t[i][0] = t[i][1] = TYPECOUNT; // turn off each one in succession
    for(int j = 1 ; j <= TYPECOUNT ; ++j){
      int min = print_coversets(j, sizeof(t) / sizeof(*t), t);
      if(min){
        printf("missing %s: %d coversets of size %d\n", tnames[i], min, j);
        break;
      }
    }
  }
}

// print all species represented in the typing vector t of size tcount
static void
print_participants(int tcount, const int t[TYPINGCOUNT][2]){
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const species &s = sdex[u];
    bool valid = false;
    for(int ti = 0 ; ti < tcount ; ++ti){
      if((s.t1 == t[ti][0] && (s.t2 == t[ti][1] || (s.t2 == TYPECOUNT && s.t1 == t[ti][1]))) ||
          (s.t1 == t[ti][1] && s.t2 == t[ti][0])){
        valid = true;
        break;
      }
    }
    if(valid){
      std::cout << " " << s.name << " (" << TNames[s.t1];
      if(s.t2 != TYPECOUNT){
        std::cout << ", " << TNames[s.t2];
      }
      std::cout << ")" << std::endl;
    }
  }
}

// print minimal coversets of all typings
static void
print_complete_coversets_duals(void){
  static int t[TYPINGCOUNT][2];
  int pos = 0;
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    for(int j = i ; j < TYPECOUNT ; ++j){
      t[pos][0] = i;
      t[pos][1] = j;
      ++pos;
    }
  }
  for(int j = 1 ; j <= TYPECOUNT ; ++j){
    int min = print_coversets(j, pos, t);
    if(min){
      printf("%d dual coversets of size %d\n", min, j);
      break;
    }else{
      printf("no dual coversets of size %d\n", j);
    }
  }
}

// print coverset of all typings including/excluding certain types. t is a
// TYPECOUNT sized vector with 1 for the typings we care about, 0 otherwise.
static int
print_coversets_duals(const int *t, bool exclude, int* pos, int ty[TYPINGCOUNT][2]){
  *pos = 0;
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    for(int j = i ; j < TYPECOUNT ; ++j){
      if(exclude){
        if(!t[i] && !t[j]){
          ty[*pos][0] = i;
          ty[*pos][1] = j;
          ++*pos;
        }
      }else{
        if(t[i] || t[j]){
          ty[*pos][0] = i;
          ty[*pos][1] = j;
          ++*pos;
        }
      }
    }
  }
  for(int j = 1 ; j <= TYPECOUNT ; ++j){
    int min = print_coversets(j, *pos, ty);
    if(min){
      printf("%d dual coversets of size %d\n", min, j);
      print_participants(*pos, ty);
      return 0;
    }else{
      printf("no dual coversets of size %d\n", j);
    }
  }
  std::cout << "couldn't find coverset" << std::endl;
  return -1;
}

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " [ -t charged ] [ [ -x ] poptype[,poptype...] ]" << std::endl;
  std::cerr << " poptype[,poptype...]: require one of these types" << std::endl;
  std::cerr << " -x poptype[,poptype...]: exclude these types" << std::endl;
  std::cerr << " -t charged: require a charged attack of this type" << std::endl;
  exit(EXIT_FAILURE);
}

static int
lex_type(const char* tstr, int tlist[TYPECOUNT]){
  pgo_types_e t = lookup_type(tstr);
  if(t == TYPECOUNT){
    std::cerr << "couldn't look up type for " << tstr << std::endl;
    return -1;
  }
  if(tlist[t]){
    std::cerr << "already provided type " << tstr << std::endl;
    return -1;
  }
  tlist[t] = 1;
  return 0;
}

// lex a comma-delimited listed of types. it is an error to be empty, or to
// repeat a type, or to specify an invalid type. case is ignored.
static int
lex_typelist(const char* arg, int tlist[TYPECOUNT]){
  for(unsigned z = 0 ; z < TYPECOUNT ; ++z){
    tlist[z] = 0;
  }
  const char* start = arg;
  unsigned tcount = 0;
  char curtype[20]; // enough to hold all type names (and then some)
  unsigned curl = 0; // length in curtype
  while(*arg){
    if(isalpha(*arg)){
      if(curl >= sizeof(curtype) / sizeof(*curtype)){
        std::cerr << "invalid type: " << start << std::endl;
        return -1;
      }
      curtype[curl++] = tolower(*arg);
    }else if(*arg == ','){
      curtype[curl] = '\0';
      if(lex_type(curtype, tlist)){
        return -1;
      }
      ++tcount;
      curl = 0;
    }else{
      std::cerr << "invalid type: " << start << std::endl;
      return -1;
    }
    ++arg;
  }
  if(curl){
    curtype[curl] = '\0';
    if(lex_type(curtype, tlist)){
      return -1;
    }
  }else if(tcount == 0){
    std::cerr << "empty typelist: " << start << std::endl;
    return -1;
  }
  return 0;
}

int main(int argc, char* const* argv){
  if(argc < 2){
    print_complete_coversets();
    print_complete_coversets_duals();
  }
  int kern[TYPECOUNT] = {};
  const char* argv0 = argv[0];
  bool exclude = false;
  int go;
  while((go = getopt(argc, argv, ":x:t:")) > 0){
    switch(go){
      case 'x': // exclude anything containing these types
        exclude = true;
        if(lex_typelist(optarg, kern)){
          usage(argv0); break;
        }
        break;
      case 't': // show population members that can use these types of charged attacks
        // FIXME
        break;
      case ':':
        std::cerr << "option requires argument: " << std::endl;
        usage(argv0); break;
      case '?':
        std::cerr << "unknown argument: " << std::endl;
        usage(argv0); break;
    }
  }
  // if an argument remains, it ought be a typelist specifying required typings
  if(*(argv + optind)){
    if(*(argv + optind + 1)){
      std::cerr << "too many arguments" << std::endl;
      usage(argv0);
    }
    if(exclude){
      // FIXME
      std::cerr << "exclude not yet supported from typelist" << std::endl;
      usage(argv0);
    }
    if(lex_typelist(*(argv + 1), kern)){
      usage(argv0);
    }
  }
  int pos;
  static int ty[TYPINGCOUNT][2];
  print_coversets_duals(kern, exclude, &pos, ty);
  return EXIT_SUCCESS;
}
