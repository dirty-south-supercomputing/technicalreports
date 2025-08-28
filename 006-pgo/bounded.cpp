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

static float
get_apercent(const stats *s){
  return s->apercent;
}

static float
get_bulk(const stats *s){
  return sqrt(s->effd * s->mhp);
}

static stats *
print_sol_set(stats *sols, float(*afxn)(const stats *s)){
  unsigned half;
  unsigned l = halflevel_to_level(sols->hlevel, &half);
  print_types(sols->s->t1, sols->s->t2);
  putc(' ', stdout);
  const char *name = sols->s->name.c_str();
  if(sols->shadow){
    if(strncmp(name, SHADPREFIX, strlen(SHADPREFIX))){
      std::cerr << "marked shadow but name was " << name << std::endl;
      throw std::exception();
    }
    name += strlen(SHADPREFIX);
  }
#undef SHADPREFIX
  escape_string(name);
  if(sols->shadow){
    printf("\\calign{\\includegraphics[height=1em,keepaspectratio]{images/shadow.png}}");
  }
  printf(" & \\ivlev{%u}{%u}{%u}{%2u%s} & %u & %.2f & %.2f & %.2f & %.2f & %u & %.1f\\\\\n",
          sols->ia, sols->id, sols->is, l, half ? ".5" : "",
          sols->mhp, sols->effa, sols->effd,
          sols->average, sols->geommean,
          sols->cp, afxn(sols));
  const species *last = sols->s;
  stats *tmp = sols;
  sols = sols->next;
  delete tmp;
  if(sols && sols->s == last){
    unsigned more = 0;
    auto *ki = sols->next;
    while( (tmp = ki) ){
      if(tmp->s != last){
        break;
      }
      ++more;
      ki = tmp->next;
      delete tmp;
    }
    sols->next = ki;
    if(more){
      printf("\\hspace{1em}\\textit{%u more not shown\\ldots}", more);
    }
    printf("& \\ivlev{%u}{%u}{%u}{%2u%s} & %u & %.2f & %.2f & & %.2f & %u & \\\\\n",
            sols->ia, sols->id, sols->is, l, half ? ".5" : "",
            sols->mhp, sols->effa, sols->effd,
            sols->geommean, sols->cp);
    tmp = sols->next;
    delete sols;
    sols = tmp;
  }
  return sols;
}

static void
print_bounded_bulktable(int bound, float lbound){
  printf("\\begingroup\n");
  printf("\\nohyphenation\n");
  printf("\\footnotesize\n");
  printf("\\setlength{\\tabcolsep}{1pt}\n");
  printf("\\begin{longtable}{lrrrrrrrr}\n");
  printf("Species & IV·L & \\HP & \\Eff{A} & \\Eff{D} & $\\frac{BS}{3}$ & $\\sqrt[3]{\\BP\\,}$ & \\CP & Bulk\\\\\n");
  printf("\\Midrule\n");
  printf("\\endhead\n");
  stats *sols = NULL;
  // FIXME want to maximize bulk, not geometric mean!
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const species *sp = &sdex[i];
    stats *s = find_optimal_set(sp, bound, lbound, false, calc_pok_gmean);
    if(s){
      s->next = sols;
      sols = s;
    }
  }
  // FIXME horrible o(n^2) sort
  stats *head = nullptr;
  stats **q = &head;
  while(sols){
    float maxbulk = 0;
    stats **candq = nullptr;
    stats **prev;
    for(prev = &sols ; *prev ; prev = &(*prev)->next){
      stats *cand = *prev;
      float bulk = cand->effd * cand->mhp;
      if(bulk > maxbulk){
        maxbulk = bulk;
        candq = prev;
      }
    }
    stats *cand = *candq;
    *candq = cand->next;
    cand->next = nullptr;
    *q = cand;
    q = &cand->next;
  }
  while( (head = print_sol_set(head, get_bulk)) ){
    ;
  }
  printf("\\captionlistentry{Bulk-optimal solutions bounded by %d \\CP}\n", bound);
  printf("\\label{table:cp%db}\n", bound);
  printf("\\end{longtable}\n");
  printf("\\endgroup\n");
}

// print optimal sets bounded by CP of |bound| above and mean of |lbound| below
static void
print_bounded_table(int bound, float lbound, float(*fitfxn)(const stats *), char fitchar){
  printf("\\begingroup\n");
  printf("\\nohyphenation\n");
  printf("\\footnotesize\n");
  printf("\\setlength{\\tabcolsep}{1pt}\n");
  printf("\\begin{longtable}{lrrrrrrrr}\n");
  printf("Species & IV·L & \\HP & \\Eff{A} & \\Eff{D} & $\\frac{BS}{3}$ & $\\sqrt[3]{\\BP\\,}$ & \\CP & A\\%% \\\\\n");
  printf("\\Midrule\n");
  printf("\\endhead\n");
  stats *sols = NULL;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const species *sp = &sdex[i];
    stats *s = find_optimal_set(sp, bound, lbound, false, fitfxn);
    if(sp->shadow && fitchar == 'a'){
      const species *shads = create_shadow(sp);
      stats *shadsets = find_optimal_set(shads, bound, lbound, true, fitfxn);
      insert_opt_stat(&sols, shadsets, fitchar == 'a');
    }
    insert_opt_stat(&sols, s, fitchar == 'a');
  }
  while( (sols = print_sol_set(sols, get_apercent)) ){
    ;
  }
  printf("\\captionlistentry{%cmean-optimal solutions bounded by %d \\CP\\label{table:cp%d%c}}\n",
            fitchar, bound, bound, fitchar);
  printf("\\end{longtable}");
  printf("\\endgroup\n");
}

static void usage(const char *argv0){
  fprintf(stderr, "usage: %s a|b|g highcp lowbound\n", argv0);
  fprintf(stderr, "\ta: arithemetic mean\n");
  fprintf(stderr, "\tb: bulk\n");
  fprintf(stderr, "\tg: geometric mean\n");
  exit(EXIT_FAILURE);
}

// print LaTeX longtables of all species' 2500 and 1500 optimal configurations
int main(int argc, char** argv){
  if(argc != 4){
    usage(argv[0]);
  }
  int hcp = atoi(argv[2]);
  float lam; // lower arithmetic mean bound
  if(sscanf(argv[3], "%f", &lam) != 1){
    fprintf(stderr, "couldn't get float from [%s]\n", argv[3]);
    usage(argv[0]);
  }
  float(*fitfxn)(const stats *);
  char fitchar;
  if(strcmp(argv[1], "g") == 0){
    fitfxn = calc_pok_gmean;
    fitchar = 'g';
  }else if(strcmp(argv[1], "a") == 0){
    fitfxn = calc_pok_amean;
    fitchar = 'a';
  }else if(strcmp(argv[1], "b") == 0){
    print_bounded_bulktable(hcp, lam);
    return EXIT_SUCCESS;
  }else{
    usage(argv[0]);
  }
  print_bounded_table(hcp, lam, fitfxn, fitchar);
  return EXIT_SUCCESS;
}
