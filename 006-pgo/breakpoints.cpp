#include <cstdio>
#include <memory>
#include <cstdlib>
#include "pgotypes.cpp"
#include "s/simul.h"
#include "s/damage.h"
#include "lex.cpp"

static pmon pm[2];

static void
print_dbreak_table(pmon *p, pmon *atk, const attack *a, int tableno){
  printf("\\begin{table}\\setlength{\\tabcolsep}{2pt}\\footnotesize\\centering\\begin{tabular}{rcccccccccccccccc}\n");
  printf("\\IV{D} & 0 & 1 & 2 & 3 & 4 & 5 & 6 & 7 & 8 & 9 & 10 & 11 & 12 & 13 & 14 & 15\\\\\n");
  printf("\\Midrule\n");
  int firstd = -1;
  for(int ivd = 0 ; ivd < 16 ; ++ivd){
    printf("%d", ivd);
    p->s.id = ivd;
    for(int iva = 0 ; iva < 16 ; ++iva){
      atk->s.ia = iva;
      int d = static_cast<int>(calc_damage(atk, p, a, 0, 0));
      if(firstd < 0){
        firstd = d;
      }
      printf(" & \\textcolor[RGB]{%d,%d,%d}{%d}", 128 + 14 * (d - firstd),
              128 + 14 * (firstd - d), 128 + 14 * (firstd - d), d);
    }
    printf("\\\\\n");
  }
  printf("\\end{tabular}\\caption{%s using %s against %s\\label{table:bpoints%d}}\\end{table}\n",
          atk->s.s->name.c_str(), a->name, p->s.s->name.c_str(), tableno);
}

// print the d breakpoints for p
static void
print_dbreaks(pmon *p, pmon *atk){
  print_dbreak_table(p, atk, atk->ca1, 1);
  if(atk->ca2){
    print_dbreak_table(p, atk, atk->ca2, 2);
  }
}

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " pokémonN iv@level fast charged..." << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char** argv){
  const char* argv0 = *argv;
  --argc;
  ++argv;
  uint16_t hp0, hp1;
  if(lex_pmon(&pm[0], &hp0, &argc, &argv)){
    usage(argv0);
  }
  if(lex_pmon(&pm[1], &hp1, &argc, &argv)){
    usage(argv0);
  }
  print_dbreaks(&pm[1], &pm[0]);
  if(argc){
    std::cerr << "unexpected argument: " << *argv << std::endl;
    usage(argv0);
  }
  return EXIT_SUCCESS;
}
