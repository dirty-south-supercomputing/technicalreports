#include "pgotypes.cpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>

// take the provided species, and generate a new species object reflecting
// the shadow bonus. this object will persist.
static species *
create_shadow(const species* s){
  if(!s->shadow){
    std::cerr << "no shadow form exists for " << s->name << std::endl;
    return NULL;
  }
#define SHADPREFIX "Shadow "
  species *news = new species(SHADPREFIX + s->name);
#undef SHADPREFIX
  news->idx = s->idx;
  news->t1 = s->t1;
  news->t2 = s->t2;
  news->atk = s->atk;
  news->def = s->def;
  news->sta = s->sta;
  news->from = s->from;
  news->attacks = s->attacks;
  news->shiny = s->shiny;
  news->shadow = false;
  return news;
}

// insert list s into list pointed to by head according to s->geommean
static void
insert_opt_stat(stats **head, stats *s){
    while(s){
      stats **prev = head;
      stats *tmp;
      while( (tmp = *prev) ){
        if(s->geommean > tmp->geommean){
          break;
        }
        prev = &tmp->next;
      }
      stats* tmps = s->next;
      s->next = tmp;
      *prev = s;
      s = tmps;
    }
}

// print optimal sets bounded by CP of |bound| above and geometric mean of |lbound| below
void print_bounded_table(int bound, float lbound){
  printf("\\begingroup\n");
  printf("\\nohyphenation\n");
  printf("\\footnotesize\n");
  printf("\\setlength{\\tabcolsep}{1pt}\n");
  printf("\\begin{longtable}{lrrrrrrrr}\n");
  printf("Species & L & IVs & HP & $Eff_A$ & $Eff_D$ & $\\sqrt[3]{BP}$ & CP & A\\%% \\\\\n");
  printf("\\Midrule\n");
  printf("\\endhead\n");
  stats *sols = NULL;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const species *sp = &sdex[i];
    stats *s = find_optimal_set(sp, bound, lbound);
    if(sp->shadow){
      const species *shads = create_shadow(sp);
      stats *shadsets = find_optimal_set(shads, bound, lbound);
      insert_opt_stat(&sols, shadsets);
    }
    insert_opt_stat(&sols, s);
  }
  const species* lastspecies = NULL;
  while(sols){
    stats *tmp = sols;
    unsigned half;
    unsigned l = halflevel_to_level(tmp->hlevel, &half);
    if(tmp->s != lastspecies){
      for(const char* curs = tmp->s->name.c_str() ; *curs ; ++curs){
        if(*curs == '%'){
          putchar('\\');
        }
        putchar(*curs);
      }
      printf(" & %2u%s & %u/%u/%u & %u & %.2f & %.2f & %.2f & %4u & %.1f\\\\\n",
              l, half ? ".5" : "",
              tmp->ia, tmp->id, tmp->is,
              tmp->mhp, tmp->effa, tmp->effd, tmp->geommean,
              tmp->cp,
              tmp->apercent);
    }else{
      printf(" & & %u/%u/%u & & & & & %4u\\\\\n", tmp->ia, tmp->id, tmp->is, tmp->cp);
    }
    lastspecies = tmp->s;
    sols = sols->next;
    free(tmp);
  }
  printf("\\caption{Optimal solutions bounded by %d CP}\n", bound);
  printf("\\label{table:cp%d}\n", bound);
  printf("\\end{longtable}\n");
  printf("\\endgroup\n");
}

// print LaTeX longtables of all species' 2500 and 1500 optimal configurations
int main(int argc, char** argv){
  if(argc != 3){
    fprintf(stderr, "usage: highcp lowGM\n");
    return EXIT_FAILURE;
  }
  int hcp = atoi(argv[1]);
  int lgm = atoi(argv[2]);
  print_bounded_table(hcp, lgm);
  return EXIT_SUCCESS;
}
