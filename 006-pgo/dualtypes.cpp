#include "pgotypes.cpp"
#include <math.h>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*static void
print_vector_ints(const int rels[]){
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    printf("%2d ", rels[i]);
  }
}

static int
vector_match_count(const int rels[], int match){
  int c = 0;
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    if(rels[i] == match){
      ++c;
    }
  }
  return c;
}

static int
choose2(int n){
  return n * (n - 1) / 2;
}

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

static void
print_complete_coversets_duals(void){
  static int t[TYPECOUNT * (TYPECOUNT + 1) / 2][2];
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
}*/

static void
melt_column(int* vec, const int* col){
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    vec[i] = *col;
    col += TYPECOUNT;
  }
}

// represents some typing, either a dualtype or monotype.
typedef struct typing {
  pgo_types_e types[2];           // our types; both are the same for monotypes
  int atypes[TYPECOUNT];  // reaction to attack type, ranging from -3 to 2
} typing;

// convert a type effectiveness exponent {-4..2} to a float
static float
typeeff_multiplier(int texp){
  if(texp < -4 || texp > 2){
    fprintf(stderr, "invalid type relation %d\n", texp);
    return NAN;
  }
  return pow(1.6, texp);
}

static int
typing_compare(const void* a, const void* b){
  float atot = 0;
  float btot = 0;
  const typing* ta = static_cast<const typing*>(a);
  const typing* tb = static_cast<const typing*>(b);
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    atot += typeeff_multiplier(ta->atypes[i]);
    btot += typeeff_multiplier(tb->atypes[i]);
  }
  if(atot < btot){
    return -1;
  }else if(btot < atot){
    return 1;
  }
  return 0;
}

// create an array of TYPINGCOUNT typings, sorted by type resistance
static std::unique_ptr<typing[]>
setup_typings(void){
  auto dtypes = std::make_unique<typing[]>(TYPINGCOUNT);
  if(dtypes){
    int pos = 0;
    for(int i = 0 ; i < TYPECOUNT ; ++i){
      for(int j = i ; j < TYPECOUNT ; ++j){
        dtypes[pos].types[0] = static_cast<pgo_types_e>(i);
        dtypes[pos].types[1] = static_cast<pgo_types_e>(j);
        melt_column(dtypes[pos].atypes, &trelations[0][i]);
        if(i != j){
          int vec[TYPECOUNT];
          melt_column(vec, &trelations[0][j]);
          for(int k = 0 ; k < TYPECOUNT ; ++k){
            dtypes[pos].atypes[k] += vec[k];
          }
        }
        ++pos;
      }
    }
  }
  return dtypes;
}

static unsigned
typing_popcount(pgo_types_e t1, pgo_types_e t2){
  unsigned pcnt = 0;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const species* s = &sdex[u];
    if((s->t1 == t1 && s->t2 == t2) || (s->t2 == t1 && s->t1 == t2)){
      ++pcnt;
    }
  }
  for(unsigned u = 0 ; u < MEGACOUNT ; ++u){
    const species* s = &megasdex[u];
    if((s->t1 == t1 && s->t2 == t2) || (s->t2 == t1 && s->t1 == t2)){
      ++pcnt;
    }
  }
  return pcnt;
}

static void
defensive_summaries_latex(const typing* t){
  printf("\\begingroup\n");
  printf("\\setlength{\\tabcolsep}{0.5em}");
  int totals[6];
  const int offset = -3;
  // defensive typing summaries
  printf("\\begin{longtable}{crrrrrrrrr}\n");
  printf("& -3 & -2 & -1 & 0 & 1 & 2 & DRA & Pop & Page\\\\\n");
  printf("\\Midrule\n");
  printf("\\endhead\n");
  bool grey = false;
  for(int i = 0 ; i < TYPINGCOUNT ; ++i){
    unsigned pcnt = typing_popcount(t[i].types[0], t[i].types[1] == t[i].types[0] ? TYPECOUNT : t[i].types[1]);
    if(pcnt == 0){
      printf("\\rowcolor{Red!25}\n");
    }
    if( (grey = !grey) && pcnt){
      printf("\\rowcolor{Gray!25}");
    }
    print_types(t[i].types[0], t[i].types[1]);
    printf(" & ");
    float dra = 0;
    for(unsigned j = 0 ; j < sizeof(totals) / sizeof(*totals) ; ++j){
      totals[j] = 0;
      for(int k = 0 ; k < TYPECOUNT ; ++k){
        if(t[i].atypes[k] == offset + (int)j){
          ++totals[t[i].atypes[k] - offset];
        }
      }
      dra += pow(1.6, (offset + (int)j)) * totals[j];
      if(totals[j]){ // don't print zeroes
        printf("%d &", totals[j]);
      }else{
        printf("&");
      }
    }
    printf("%.3f & %u & ", dra / 18, pcnt);
    if(pcnt){ // no page if no population
      printf("\\pageref{types:%s%s}", tnames[t[i].types[0]],
              t[i].types[1] == t[i].types[0] ? "" : tnames[t[i].types[1]]);
    }
    printf("\\\\\n");
  }
  printf("\\caption[Defender effectiveness summaries]{Defender effectiveness summaries (lower is better)}\n");
  printf("\\label{table:defenders}\n");
  printf("\\end{longtable}\n");
  printf("\\endgroup\n");
}

static void
defensive_relations_latex(const typing* t){
  printf("\\begingroup\n");
  printf("\\setlength{\\tabcolsep}{1pt}");
  printf("\\begin{longtable}{cgcgcgcgcgcgcgcgcgc}\n");
  for(int k = 0 ; k < TYPECOUNT ; ++k){
    printf("& \\includegraphics[width=1em,keepaspectratio]{images/%s.png}", tnames[k]);
  }
  printf("\\\\\n");
  printf("\\endhead\n");
  bool grey = true;
  for(int i = 0 ; i < TYPINGCOUNT ; ++i){
    if( (grey = !grey) ){
      printf("\\rowcolor{Gray!25}");
    }
    print_types(t[i].types[0], t[i].types[1]);
    for(int k = 0 ; k < TYPECOUNT ; ++k){
      if(t[i].atypes[k]){
        printf("& %d", t[i].atypes[k]);
      }else{
        printf("& ");
      }
    }
    printf("\\\\\n");
  }
  printf("\\caption{Defender typing effectiveness}\n");
  printf("\\end{longtable}\n");
  printf("\\endgroup\n");
}

/*static void
attack_summaries(void){
  printf("Attack efficiency summaries\n");
  printf("          -2 -1  0  1  T -4 -3 -2 -1   0  1  2   T  A\n");
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    printf("%8s: ", tnames[i]);
    int aneg2 = vector_match_count(trelations[i], -2);
    int aneg1 = vector_match_count(trelations[i], -1);
    int azero = vector_match_count(trelations[i], 0);
    int apos1 = vector_match_count(trelations[i], 1);
    int asum = aneg2 * -2 - aneg1 + apos1;
    printf("%2d %2d %2d %2d %2d", aneg2, aneg1, azero, apos1, asum);
    int tneg4 = choose2(aneg2);
    int tneg3 = aneg2 * aneg1;
    int tneg2 = aneg2 * (azero + 1) + choose2(aneg1);
    int tneg1 = aneg1 * (azero + 1) + aneg2 * apos1;
    int tzero = azero + choose2(azero) + apos1 * aneg1;
    int tpos1 = apos1 * (azero + 1);
    int tpos2 = choose2(apos1);
    int tsum = tneg4 * -4 + tneg3 * -3 + tneg2 * -2 - tneg1 + tpos1 + tpos2 * 2;
    int tavg = tsum / 18; // always a multiple of 18
    printf(" %2d %2d %2d %2d %3d %2d %2d %3d %2d",
        tneg4, tneg3, tneg2, tneg1, tzero, tpos1, tpos2, tsum, tavg);
    printf("\n");
  }
  printf("\n");
}*/

static void
usage(const char* argv0){
  fprintf(stderr, "usage: %s [ summary ]\n", argv0);
  exit(EXIT_FAILURE);
}

int main(int argc, const char** argv){
  /*printf("Attack efficiencies\n");
  printf("          Bu Da Dr El Fa Fg Fi Fl Gh Gs Gd Ic No Po Py Ro St Wa\n");
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    printf("%8s: ", tnames[i]);
    print_vector_ints(trelations[i]);
    printf("\n");
  }
  printf("\n");

  print_complete_coversets();
  printf("\n");

  print_complete_coversets_duals();
  printf("\n");

  attack_summaries();*/

  auto t = setup_typings();
  if(argc == 2){
    if(strcmp(argv[1], "summary")){
      usage(argv[0]);
    }
    qsort(t.get(), TYPINGCOUNT, sizeof(typing), typing_compare);
    defensive_summaries_latex(t.get());
    return EXIT_SUCCESS;
  }else if(argc != 1){
    usage(argv[0]);
  }
  defensive_relations_latex(t.get());
  return EXIT_SUCCESS;
}
