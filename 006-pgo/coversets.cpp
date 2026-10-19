#include "html.h"
#include <getopt.h>
#include <memory>
#include <algorithm>

using tarray = std::array<bool, TYPECOUNT>;

// tocc -- array indicating membership of attack types in this coverset
static void
emit_coverset(const std::vector<pgo_types_e> &tocc, int setidx, bool html){
  if(html){
    std::cout << "<tr><td>" << setidx << "</td><td>";
    for(auto p : tocc){
      html_type(std::cout, p);
      std::cout << ' ';
    }
    std::cout << "</td></tr>" << std::endl;
  }else{
    printf(" cover: %d ", setidx);
    for(auto p : tocc){
      printf("%s ", tnames[p]);
    }
    printf("\n");
  }
}

// sort the coversets so that the most common types are to the left and ideally
// in a single column. in particular, any type that shows up in all the coversets
// must be in the same column in all of them.
// out is a set of size equal to tarrays, but vectors of types having size equal
// to the coverset size (i.e. number of occupied elements in the tarrays), sorted.
// at the conclusion, out is populated, and tarrays is reset to all 0s.
static void
organize_coversets(std::vector<tarray> &tarrays, std::vector<std::vector<pgo_types_e>> &out){
  int counts[TYPECOUNT]{};
  int maxcount = 0; // count of whichever type shows up the most, <= tarrays.size
  pgo_types_e bigtype = TYPECOUNT; // type corresponding to maxcount
  unsigned setsize = 0;
  for(const auto &t : tarrays){
    for(pgo_types_e p = TYPESTART ; p < TYPECOUNT ; ++p){
      if(t[p]){
        ++setsize;
        if(++counts[p] > maxcount){
          maxcount = counts[p];
          bigtype = p;
        }
      }
    }
  }
  if(!setsize){
    std::cerr << "coversets were empty" << std::endl;
    throw std::exception();
  }
  if(setsize % tarrays.size()){
    std::cerr << "coversets varied in size" << std::endl;
    throw std::exception();
  }
  setsize /= tarrays.size();
  // bubble each column. first will be bigtype.
  while(bigtype != TYPECOUNT){
std::cerr << tname_capitalized(bigtype) << " count: " << counts[bigtype] << std::endl;
    for(unsigned i = 0 ; i < tarrays.size() ; ++i){
      if(tarrays[i][bigtype]){
        out[i].push_back(bigtype);
      }
    }
    counts[bigtype] = 0;
    maxcount = 0;
    bigtype = TYPECOUNT;
    for(pgo_types_e p = TYPESTART ; p < TYPECOUNT ; ++p){
      if(counts[p] > maxcount){
        maxcount = counts[p];
        bigtype = p;
      }
    }
  }
}

static void
emit_coversets(std::vector<tarray> &tarrays, const std::string &title, bool html){
  if(tarrays.empty()){
    return;
  }
  std::vector<std::vector<pgo_types_e>> out(tarrays.size());
  organize_coversets(tarrays, out);
  if(html){
    std::cout << "<h2>" << title << "</h2>" << std::endl;
    std::cout << "<table>" << std::endl;
    std::cout << "<tr><th></th><th></th></tr>" << std::endl;
  }
  int setidx = 1;
  for(const auto &clean : out){
    emit_coverset(clean, setidx, html);
    ++setidx;
  }
  if(html){
    std::cout << "</table>" << std::endl;
  }
}

// level is the filter of effectiveness -- we must be greater
// tocc: TYPECOUNT-sized vector of ints -- our attacking types
//  print tocc iff it covers tvec
// tvec is a v-sized vector of 2-vectors of ints (2v total elements) -- our target typings
//  if the first int is TYPECOUNT, it's not populated (we don't care)
//  we must cover each valid typing at level + 1
static int
print_coverset(const tarray &tocc, int v, const pgo_types_e tvec[][2], int level,
               std::vector<tarray> &setvec){
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
  // we found a coverset, yay
  setvec.emplace_back(tocc);
  return 1;
}

// we are placing n 1s among the last m slots of tocc
static int
place_n_in_m(int n, int m, const tarray &tocc, int v, const pgo_types_e tvec[][2],
             int level, std::vector<tarray> &setvec){
  tarray tcopy(tocc);
  // base case m == 1
  if(m == 1){
    if(n){
      tcopy[TYPECOUNT - m] = 1;
    }
    int r = print_coverset(tcopy, v, tvec, level, setvec);
    return r;
  }
  int ret = 0;
  // test with our bit off unless we must place this bit
  if(n < m){
    ret += place_n_in_m(n, m - 1, tcopy, v, tvec, level, setvec);
  }
  // test with our bit on if we have a bit to place
  if(n){
    tcopy[TYPECOUNT - m] = 1;
    ret += place_n_in_m(n - 1, m - 1, tcopy, v, tvec, level, setvec);
  }
  return ret;
}

// print the sets of size n or less which cover the specified vector
static int
gen_coversets(int n, int v, const pgo_types_e tvec[][2], int level,
              std::vector<tarray> &setvec){
  tarray tocc{};
  return place_n_in_m(n, TYPECOUNT, tocc, v, tvec, level, setvec);
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
    int min = gen_coversets(j, sizeof(t) / sizeof(*t), t, level);
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
      int min = gen_coversets(j, sizeof(t) / sizeof(*t), t, level);
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

static bool
excluded_p(const species &s, const std::vector<const species *> &excludelist){
  for(const auto es : excludelist){
    if(s.name == es->name){
      return true;
    }
  }
  if(pvp_banned_p(s)){
    return true;
  }
  return false;
}

// print all species represented in the typing vector t of size tcount
static void
print_participants(int tcount, const pgo_types_e t[TYPINGCOUNT][2],
                   const std::vector<const species *> &excludelist){
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(excluded_p(*s, excludelist)){
      continue;
    }
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
    int min = gen_coversets(j, pos, t, level);
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
                      int *pos, pgo_types_e ty[TYPINGCOUNT][2],
                      int level, std::vector<const species *> &excludelist,
                      bool html){
  std::vector<tarray> setvec;
  *pos = 0;
  // FIXME this ought take into account empty typings, especially with the excludelist
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
    int min = gen_coversets(j, *pos, ty, level, setvec);
    if(min){
      fprintf(stderr, "%d dual coversets of size %d\n", min, j);
      emit_coversets(setvec, title, html);
      if(!html){
        print_participants(*pos, ty, excludelist);
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
  std::cerr << "usage: " << argv0 << " [ options... ] " << std::endl;
  std::cerr << " poptype[,poptype...]: require one of these types" << std::endl;
  std::cerr << " -x poptype[,poptype...]: exclude these types" << std::endl;
  std::cerr << " -X form: exclude this form, may be supplied multiple times" << std::endl;
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
  std::vector<const species *> excludelist;
  while((go = getopt(argc, argv, ":hl:n:X:x:t:")) > 0){
    switch(go){
      case 'X':{ // exclude this form
        const auto s = lookup_species(optarg);
        if(!s){
          std::cerr << "couldn't find form: " << optarg << std::endl;
          usage(argv0); break;
        }
        if(std::ranges::contains(excludelist, s)){
          std::cerr << "specified form twice: " << optarg << std::endl;
          usage(argv0); break;
        }
        excludelist.push_back(s);
        break;
      }case 'x': // exclude anything containing these types
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
  print_coversets_duals(title, kern, exclude, &pos, ty, level, excludelist, html);
  return EXIT_SUCCESS;
}
