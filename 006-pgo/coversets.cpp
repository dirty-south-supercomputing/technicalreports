#include "pgotypes.cpp"
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
    int min = print_coversets(j, sizeof(t) / sizeof(*t), t);
    if(min){
      printf("%d dual coversets of size %d\n", min, j);
      break;
    }else{
      printf("no dual coversets of size %d\n", j);
    }
  }
}

// print coverset of all typings including certain types. t is a TYPECOUNT
// sized vector with 1 for the typings we care about, 0 otherwise.
static int
print_coversets_duals(const int *t){
  static int ty[TYPINGCOUNT][2];
  int pos = 0;
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    for(int j = i ; j < TYPECOUNT ; ++j){
      if(t[i] || t[j]){
        ty[pos][0] = i;
        ty[pos][1] = j;
        ++pos;
      }
    }
  }
  for(int j = 1 ; j <= TYPECOUNT ; ++j){
    int min = print_coversets(j, pos, ty);
    if(min){
      printf("%d dual coversets of size %d\n", min, j);
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
  std::cerr << "usage: " << argv0 << " [ type... ]" << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, const char **argv){
  if(argc < 2){
    print_complete_coversets();
    print_complete_coversets_duals();
  }
  int kern[TYPECOUNT] = {};
  for(const char **a = argv + 1 ; *a ; ++a){
    pgo_types_e t = lookup_type(*a);
    if(t == TYPECOUNT){
      std::cerr << "couldn't look up type for " << *a << std::endl;
      usage(argv[0]);
    }
    if(kern[t]){
      std::cerr << "already provided type " << *a << std::endl;
      usage(argv[0]);
    }
    kern[t] = 1;
  }
  print_coversets_duals(kern);
  return EXIT_SUCCESS;
}
