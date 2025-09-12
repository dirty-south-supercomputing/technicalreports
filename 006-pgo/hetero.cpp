#include "pgotypes.h"
#include <algorithm>
#include <vector>

static bool
hetero_p(const species *s){
  if(s->from == NULL){
    return false;
  }
  const species* from = lookup_species(s->from);
  if(from->t1 == s->t1 && from->t2 == s->t2){
    return false;
  }
  return true;
}

static int
print_hetero_evols(const species* dex, unsigned dexcount, unsigned* pcount){
  std::vector<const species *> hetero;
  for(unsigned u = 0 ; u < dexcount ; ++u){
    const species* s = &dex[u];
    if(hetero_p(s)){
      hetero.emplace_back(s);
    }
  }
  std::sort(hetero.begin(), hetero.end(),
      [](const species *l, const species *r){
        return lookup_species(l->from)->name < lookup_species(r->from)->name;
      });
  for(const auto &s : hetero){
    const species* from = lookup_species(s->from);
    print_types(from->t1, from->t2);
    printf(" %s", from->name.c_str());
    #define GLAR "Galarian"
    #define CROW "Crowned S"
    if(!strncmp(from->name.c_str(), GLAR, strlen(GLAR))
        || !strncmp(s->name.c_str(), GLAR, strlen(GLAR))
        || !strncmp(s->name.c_str(), CROW, strlen(CROW))){
      printf("\\newline");
    }
    #undef CROW
    #undef GLAR
    printf(" → ");
    print_types(s->t1, s->t2);
    printf(" %s ", s->name.c_str());
    if(++*pcount % 2){
      printf(" & ");
    }else{
      printf("\\\\\n");
    }
  }
  return 0;
}

static bool
hetero_p(const mega *m){
  const species* from = lookup_species(m->idx);
  if(from->t1 == m->t1 && from->t2 == m->t2){
    return false;
  }
  return true;
}

static int
print_hetero_evols(const mega* dex, unsigned dexcount, unsigned* pcount){
  std::vector<const mega *> hetero;
  for(unsigned u = 0 ; u < dexcount ; ++u){
    const mega* m = &dex[u];
    if(hetero_p(m)){
      hetero.emplace_back(m);
    }
  }
  std::sort(hetero.begin(), hetero.end(),
      [](const mega *l, const mega *r){
        return lookup_species(l->idx)->name < lookup_species(r->idx)->name;
      });
  for(const auto &m : hetero){
    const species* from = lookup_species(m->idx);
    print_types(from->t1, from->t2);
    printf(" %s", from->name.c_str());
    printf(" → ");
    print_types(m->t1, m->t2);
    printf(" %s ", m->name.c_str());
    if(++*pcount % 2){
      printf(" & ");
    }else{
      printf("\\\\\n");
    }
  }
  return 0;
}

static void type_heterotable(void){
  printf("\\begingroup");
  printf("\\footnotesize");
  printf("\\begin{longtable}{p{.5\\textwidth}|p{.5\\textwidth}}");
  unsigned count = 0;
  puts("Evolution & Evolution\\\\\\Midrule");
  if(print_hetero_evols(sdex, SPECIESCOUNT, &count)){
    exit(EXIT_FAILURE);
  }
  if(print_hetero_evols(megasdex, MEGACOUNT, &count)){
    exit(EXIT_FAILURE);
  }
  if(count % 2){
    printf("\\\\\n");
  }
  printf("\\caption{Type-changing evolutions and form changes\\label{table:heteroevolve}}");
  printf("\\end{longtable}");
  printf("\\endgroup");
}

static int
print_hetero_costs(const species* dex, unsigned dexcount, unsigned* pcount){
  for(unsigned u = 0 ; u < dexcount ; ++u){
    const species* s = &dex[u];
    if(s->from == NULL){
      continue;
    }
    const species* from = lookup_species(s->from);
    if(from == NULL){
      fprintf(stderr, "Bad ancestor (%s) for %s, exiting\n", s->from, s->name.c_str());
      return -1;
    }
    if(from->a2cost == s->a2cost){
      continue;
    }
    printf("%s → %s & %d → %d", from->name.c_str(), s->name.c_str(),
        a2cost_to_cgroup(from->a2cost), a2cost_to_cgroup(s->a2cost));
    if(++*pcount % 2){
      printf(" & ");
    }else{
      printf("\\\\\n");
    }
  }
  return 0;
}

static void cost_heterotable(void){
  printf("\n\n\\begin{table}[b]\\centering\\footnotesize");
  printf("\\begin{tabular}{lr|lr}");
  unsigned count = 0;
  puts("Evolution & Change & Evolution & Change\\\\\\Midrule");
  for(const auto &dex : sdexen){
    if(print_hetero_costs(dex.dex, dex.dcount, &count)){
      exit(EXIT_FAILURE);
    }
  }
  if(count % 2){
    printf("&\\\\\n");
  }
  printf("\\end{tabular}");
  printf("\\caption{Evolutions that change cost group\\label{table:heterocost}}");
  printf("\\end{table}");
}

static void
usage(const char *a0){
  std::cerr << "usage: " << a0 << " cost | type" << std::endl;
  exit(EXIT_FAILURE);
}

// print a latex table of evolutions which change types
int main(int argc, const char **argv){
  if(argc != 2){
    usage(argv[0]);
  }
  if(strcmp(argv[1], "cost") == 0){
    cost_heterotable();
  }else if(strcmp(argv[1], "type") == 0){
    type_heterotable();
  }else{
    usage(argv[0]);
  }
  return EXIT_SUCCESS;
}
