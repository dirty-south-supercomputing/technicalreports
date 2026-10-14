#include "html.h"
#include <getopt.h>
#include <memory>

static void
copy_tvec(int* tnew, const int tvec[]){
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    tnew[i] = tvec[i];
  }
}

// level is the filter of effectiveness -- we must be greater
// tocc: TYPECOUNT-sized vector of ints -- our attacking types
//  print tocc iff it covers tvec
// tvec is a v-sized vector of 2-vectors of ints (2v total elements) -- our target typings
//  if the first int is TYPECOUNT, it's not populated (we don't care)
//  we must cover each valid typing at level + 1
static int
print_coverset(const std::string &title, const int tocc[], int v,
               const pgo_types_e tvec[][2], int level, int setidx, bool html){
  for(int i = 0 ; i < v ; ++i){
    if(tvec[i][0] != TYPECOUNT){ // it's populated; ensure we cover
      int covered = 0;
      for(pgo_types_e j = TYPESTART ; j < TYPECOUNT ; ++j){
        if(tocc[j]){ // we have this attacking type; check the typing relation
          int tr = typing_relation(j, tvec[i][0], tvec[i][1]);
          if(tr > level){
            covered = 1;
            break;
          }
        }
      }
      if(!covered){
        return 0;
      }
    }
  }
  // we found one or more covers, yay
  if(html){
    if(setidx == 1){
      std::cout << "<table>" << std::endl;
      std::cout << "<tr><th></th><th>" << title << "</th></tr>" << std::endl;
    }
    std::cout << "<tr><td>" << setidx << "</td><td>";
    for(pgo_types_e i = TYPESTART ; i < TYPECOUNT ; ++i){
      if(tocc[i]){
        html_type(std::cout, i);
        std::cout << ' ';
      }
    }
    std::cout << "</td></tr>" << std::endl;
  }else{
    printf(" cover: %d ", setidx);
    for(int i = 0 ; i < TYPECOUNT ; ++i){
      if(tocc[i]){
        printf("%s ", tnames[i]);
      }
    }
    printf("\n");
  }
  return 1;
}

// we are placing n 1s among the last m slots of tocc
static int
place_n_in_m(const std::string &title, int n, int m, const int tocc[],
             int v, const pgo_types_e tvec[][2], int level, int *setidx, bool html){
  auto tcopy = std::make_unique<int[]>(TYPECOUNT);
  copy_tvec(tcopy.get(), tocc);
  // base case m == 1
  if(m == 1){
    if(n){
      tcopy[TYPECOUNT - m] = 1;
    }
    int r = print_coverset(title, tcopy.get(), v, tvec, level, *setidx, html);
    if(r){
      ++*setidx;
    }
    return r;
  }
  int ret = 0;
  // test with our bit off unless we must place this bit
  if(n < m){
    ret += place_n_in_m(title, n, m - 1, tcopy.get(), v, tvec, level, setidx, html);
  }
  // test with our bit on if we have a bit to place
  if(n){
    tcopy[TYPECOUNT - m] = 1;
    ret += place_n_in_m(title, n - 1, m - 1, tcopy.get(), v, tvec, level, setidx, html);
  }
  return ret;
}

// print the sets of size n or less which cover the specified vector
static int
print_coversets(const std::string &title, int n, int v,
                const pgo_types_e tvec[][2], int level, bool html){
  int tocc[TYPECOUNT] = {};
  int setidx = 1;
  int r = place_n_in_m(title, n, TYPECOUNT, tocc, v, tvec, level, &setidx, html);
  if(html && r){
    std::cout << "</table>" << std::endl;
  }
  return r;
}

// print minimal coversets of the monotypes, and for each monotype, the minimal
// coversets for all monotypes except that one
/*static void
print_complete_coversets(int level){
  int t[TYPECOUNT][2];
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    t[i][0] = i;
    t[i][1] = i;
  }
  for(int j = 1 ; j <= TYPECOUNT ; ++j){
    int min = print_coversets(tname_capitalized(j), j, sizeof(t) / sizeof(*t), t, level, false);
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
      int min = print_coversets("All Pokémon", j, sizeof(t) / sizeof(*t), t, level, false);
      if(min){
        printf("missing %s: %d coversets of size %d\n", tnames[i], min, j);
        break;
      }
    }
  }
}*/

static void
print_participant_attacks(const species &s, bool fast){
  bool one = false;
  bool atypes[TYPECOUNT] = {};
  for(const auto a : s.attacks){
    if(fast_attack_p(*a) != fast){
      continue;
    }
    if(a->type != TYPECOUNT){
      if(atypes[a->type]){
        continue;
      }
    }
    if(one){
      std::cout << ", ";
    }
    if(a->type == TYPECOUNT){
      std::cout << "HiddenPower";
    }else{
      std::cout << tname_capitalized(a->type);
      atypes[a->type] = true;
    }
    one = true;
  }
}

// print all species represented in the typing vector t of size tcount
static void
print_participants(int tcount, const pgo_types_e t[TYPINGCOUNT][2]){
  for(auto s = species_begin() ; s != species_end() ; ++s){
    bool valid = false;
    for(int ti = 0 ; ti < tcount ; ++ti){
      if((s->t1 == t[ti][0] && (s->t2 == t[ti][1] || (s->t2 == TYPECOUNT && s->t1 == t[ti][1]))) ||
          (s->t1 == t[ti][1] && s->t2 == t[ti][0])){
        valid = true;
        break;
      }
    }
    if(valid){
      std::cout << " " << s->name << " (" << tname_capitalized(s->t1);
      if(s->t2 != TYPECOUNT){
        std::cout << ", " << tname_capitalized(s->t2);
      }
      std::cout << ") → ";
      print_participant_attacks(*s, true);
      std::cout << " : ";
      print_participant_attacks(*s, false);
      std::cout << std::endl;
    }
  }
}

// print minimal coversets of all typings
/*static void
print_complete_coversets_duals(int level, bool html){
  static pgo_types_e t[TYPINGCOUNT][2];
  int pos = 0;
  for(pgo_types_e i = TYPESTART ; i < TYPECOUNT ; ++i){
    for(pgo_types_e j = i ; j < TYPECOUNT ; ++j){
      t[pos][0] = i;
      t[pos][1] = j;
      ++pos;
    }
  }
  for(int j = 1 ; j <= TYPECOUNT ; ++j){
    int min = print_coversets("All Pokémon", j, pos, t, level, html);
    if(min){
      fprintf(stderr, "%d dual coversets of size %d\n", min, j);
      break;
    }else{
      fprintf(stderr, "no dual coversets of size %d\n", j);
    }
  }
}*/

// print coverset of all typings including/excluding certain types. t is a
// TYPECOUNT sized vector with 1 for the typings we care about, 0 otherwise.
static int
print_coversets_duals(const std::string &title, const int *t, bool exclude,
                      int *pos, pgo_types_e ty[TYPINGCOUNT][2], int level, bool html){
  *pos = 0;
  for(pgo_types_e i = TYPESTART ; i < TYPECOUNT ; ++i){
    for(pgo_types_e j = i ; j < TYPECOUNT ; ++j){
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
    int min = print_coversets(title, j, *pos, ty, level, html);
    if(min){
      fprintf(stderr, "%d dual coversets of size %d\n", min, j);
      if(!html){
        print_participants(*pos, ty);
      }
      return 0;
    }else{
      fprintf(stderr, "no dual coversets of size %d\n", j);
    }
  }
  std::cout << "couldn't find coverset" << std::endl;
  return -1;
}

[[noreturn]] static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " [ -h ] [ -n name ] [ -t charged ] [ [ -x ] poptype[,poptype...] ]" << std::endl;
  std::cerr << " poptype[,poptype...]: require one of these types" << std::endl;
  std::cerr << " -x poptype[,poptype...]: exclude these types" << std::endl;
  std::cerr << " -n name: name of coverset (for html)" << std::endl;
  std::cerr << " -l type effectiveness filter (default 0)" << std::endl;
  std::cerr << " -h: generate html" << std::endl;
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
  int kern[TYPECOUNT] = {};
  int reqchargedtype[TYPECOUNT] = {};
  const char *title = nullptr;
  const char* argv0 = argv[0];
  bool exclude = false;
  bool reqcharged = false; // FIXME not yet implemented
  bool html = false;
  int level = 0;
  int go;
  while((go = getopt(argc, argv, ":hl:n:x:t:")) > 0){
    switch(go){
      case 'x': // exclude anything containing these types
        if(exclude){
          std::cerr << "can't provide -x twice" << std::endl;
          usage(argv0); break;
        }
        if(lex_typelist(optarg, kern)){
          usage(argv0); break;
        }
        exclude = true;
        break;
      case 'l': // level filter
        if(level){
          std::cerr << "can't provide -l twice" << std::endl;
          usage(argv0); break;
        }
        level = atoi(optarg);
        if(level == 0){
          std::cerr << "invalid level: " << optarg << std::endl;
          usage(argv0); break;
        }
        break;
      case 't': // show population members that can use these types of charged attacks
        if(reqcharged){
          std::cerr << "can't provide -t twice" << std::endl;
          usage(argv0); break;
        }
        if(lex_typelist(optarg, reqchargedtype)){
          usage(argv0); break;
        }
        reqcharged = true;
        break;
      case 'n':
        if(title){
          std::cerr << "can't provide -n twice" << std::endl;
          usage(argv0); break;
        }
        title = optarg;
        break;
      case 'h':
        html = true;
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
    if(lex_typelist(*(argv + optind), kern)){
      usage(argv0);
    }
  }else{
    for(pgo_types_e t = TYPESTART ; t < TYPECOUNT ; ++t){
      kern[t] = 1;
    }
  }
  int pos;
  static pgo_types_e ty[TYPINGCOUNT][2];
  if(!title){
    title = "Custom";
  }
  print_coversets_duals(title, kern, exclude, &pos, ty, level, html);
  return EXIT_SUCCESS;
}
