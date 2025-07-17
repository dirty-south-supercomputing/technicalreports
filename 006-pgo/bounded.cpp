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

// insert list s into list pointed to by head according to s->geommean or s->average
static void
insert_opt_stat(stats **head, stats *s, bool amean){
  while(s){
    stats **prev = head;
    stats *tmp;
    while( (tmp = *prev) ){
      float m = amean ? s->average : s->geommean;
      float om = amean ? s->geommean : s->average;
      float t = amean ? tmp->average : tmp->geommean;
      float ot = amean ? tmp->geommean : tmp->average;
      if(m > t){
        break;
      }else if(m == t){
        if(s->s->name == tmp->s->name){
          if(om > ot){
            break;
          }
        }
      }
      prev = &tmp->next;
    }
    stats* tmps = s->next;
    s->next = tmp;
    *prev = s;
    s = tmps;
  }
}

// print optimal sets bounded by CP of |bound| above and mean of |lbound| below
void print_bounded_table(int bound, float lbound, bool amean){
  printf("\\begingroup\n");
  printf("\\nohyphenation\n");
  printf("\\footnotesize\n");
  printf("\\setlength{\\tabcolsep}{1pt}\n");
  printf("\\begin{longtable}{lrrrrrrrr}\n");
  printf("Species & IV@L & HP & $Eff_A$ & $Eff_D$ & $\\frac{BS}{3}$ & $\\sqrt[3]{BP}$ & CP & A\\%% \\\\\n");
  printf("\\Midrule\n");
  printf("\\endhead\n");
  stats *sols = NULL;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const species *sp = &sdex[i];
    stats *s = find_optimal_set(sp, bound, lbound, false, amean);
    if(sp->shadow && amean){
      const species *shads = create_shadow(sp);
      stats *shadsets = find_optimal_set(shads, bound, lbound, true, amean);
      insert_opt_stat(&sols, shadsets, amean);
    }
    insert_opt_stat(&sols, s, amean);
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
      printf(" & \\ivlev{%u}{%u}{%u}{%2u%s} & %u & %.2f & %.2f & %.2f & %.2f & %u & %.1f\\\\\n",
              tmp->ia, tmp->id, tmp->is, l, half ? ".5" : "",
              tmp->mhp, tmp->effa, tmp->effd,
              tmp->average, tmp->geommean,
              tmp->cp, tmp->apercent);
    }else{
      printf(" & \\ivlev{%u}{%u}{%u}{%2u%s} & %u & %.2f & %.2f & & %.2f & %u & \\\\\n",
      //printf(" & %2u%s & %u/%u/%u & & & & & %.2f & %4u\\\\\n",
              tmp->ia, tmp->id, tmp->is, l, half ? ".5" : "",
              tmp->mhp, tmp->effa, tmp->effd,
              tmp->geommean, tmp->cp);
    }
    lastspecies = tmp->s;
    sols = sols->next;
    free(tmp);
  }
  printf("\\captionlistentry{Optimal solutions bounded by %d CP}\n", bound);
  printf("\\label{table:cp%d%c}\n", bound, amean ? 'a' : 'g');
  printf("\\end{longtable}\n");
  printf("\\endgroup\n");
}

static void usage(const char *argv0){
  fprintf(stderr, "usage: %s a|g highcp lowAM\n", argv0);
  exit(EXIT_FAILURE);
}

// print LaTeX longtables of all species' 2500 and 1500 optimal configurations
int main(int argc, char** argv){
  if(argc != 4){
    usage(argv[0]);
  }
  bool amean;
  if(strcmp(argv[1], "g") == 0){
    amean = false;
  }else if(strcmp(argv[1], "a") == 0){
    amean = true;
  }else{
    usage(argv[0]);
  }
  int hcp = atoi(argv[2]);
  float lam; // lower arithmetic mean bound
  if(sscanf(argv[3], "%f", &lam) != 1){
    fprintf(stderr, "couldn't get float from [%s]\n", argv[3]);
    usage(argv[0]);
  }
  print_bounded_table(hcp, lam, amean);
  return EXIT_SUCCESS;
}
