#include <algorithm>
#include <vector>
#include "html.h"

static bool
hetero_p(const species &s){
  if(s.from.empty()){
    return false;
  }
  const species* from = lookup_species(s.from.c_str());
  if(from->name == "Eevee"){
    return false;
  }
  if(from->t1 == s.t1 && from->t2 == s.t2){
    return false;
  }
  return true;
}

static int
print_hetero_evol(const species &base, const species &evol){
  std::cout << "<tr>";
  std::cout << "<td>";
  species_with_icons(std::cout, base, true, false);
  std::cout << "</td>";
  std::cout << "<td>";
  species_with_icons(std::cout, evol, true, false);
  std::cout << "</td>";
  std::cout << "</tr>";
  return 0;
}

static int
print_hetero_evols(unsigned* pcount, bool html){
  std::vector<const species *> hetero;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(hetero_p(*s)){
      hetero.emplace_back(&*s);
    }
  }
  std::sort(hetero.begin(), hetero.end(),
      [](const species *l, const species *r){
        return lookup_species(l->from.c_str())->name < lookup_species(r->from.c_str())->name;
      });
  for(const auto &s : hetero){
    const species* from = lookup_species(s->from.c_str());
    if(html){
      print_hetero_evol(*from, *s);
    }else{
      print_types(from->t1, from->t2);
      printf(" %s", from->name.c_str());
      #define GLAR "Galarian"
      #define CROW "Crowned Shield Zamazenta"
      #define WORM "Wormadam"
      if(!strncmp(from->name.c_str(), GLAR, strlen(GLAR))
          || !strncmp(s->name.c_str(), GLAR, strlen(GLAR))
          || !strncmp(s->name.c_str(), CROW, strlen(CROW))
          || strstr(s->name.c_str(), WORM)
          ){
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
  }
  return 0;
}

static int
print_hetero_evols_mega(bool html){
  unsigned pcount = 0;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    for(const mega &m : s->mforms){
      if(m.t1 != s->t1 || m.t2 != s->t2){
        if(html){
          species smeg{&*s, m};
          print_hetero_evol(*s, smeg);
        }else{
          print_types(s->t1, s->t2);
          printf(" %s", s->name.c_str());
          printf(" → ");
          print_types(m.t1, m.t2);
          printf(" %s ", m.name.c_str());
          if(++pcount % 2){
            printf(" & ");
          }else{
            printf("\\\\\n");
          }
        }
      }
    }
  }
  if(!html){
    if(pcount % 2){
      printf("\\\\\n");
    }
  }
  return 0;
}

static int
type_heterotable(bool html){
  if(html){
    std::cout << "<table class=\"evenshade\" id=\"heteroevols\">";
    std::cout << "<tr><th/><th>Base</th><th/><th>Evolution</th></tr>";
  }else{
    printf("\\begingroup");
    printf("\\footnotesize");
    printf("\\begin{longtable}{p{.5\\textwidth}|p{.5\\textwidth}}");
    puts("Evolution & Evolution\\\\\\Midrule");
  }
  unsigned count = 0;
  if(print_hetero_evols(&count, html)){
    return -1;
  }
  if(html){
    if(print_hetero_evols_mega(html)){
      return -1;
    }
    std::cout << "</table>";
  }else{
    if(count % 2){
      printf("\\\\\n");
    }
    printf("\\caption{Type-changing evolutions and form changes (Eevee excluded)\\label{table:heteroevolve}}");
    printf("\\end{longtable}");
    printf("\\endgroup\n");

    printf("\\begingroup");
    printf("\\footnotesize");
    printf("\\begin{longtable}{p{.5\\textwidth}|p{.5\\textwidth}}");
    puts("Mega evolution & Mega evolution\\\\\\Midrule");
    if(print_hetero_evols_mega(html)){
      return -1;
    }
    printf("\\caption{Type-changing Mega evolutions\\label{table:heteroevolvemega}}");
    printf("\\end{longtable}");
    printf("\\endgroup");
  }
  return 0;
}

static int
print_hetero_costs(unsigned* pcount){
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(s->from.empty()){
      continue;
    }
    const species *from = lookup_species(s->from.c_str());
    if(!from){
      fprintf(stderr, "Bad ancestor (%s) for %s, exiting\n", s->from.c_str(), s->name.c_str());
      return -1;
    }
    if(from->a2cost == s->a2cost){
      continue;
    }
    printf("%s → %s & %d → %d", from->name.c_str(), s->name.c_str(), from->a2cost, s->a2cost);
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
  if(print_hetero_costs(&count)){
    exit(EXIT_FAILURE);
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
  std::cerr << "usage: " << a0 << " cost | type | typehtml" << std::endl;
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
    type_heterotable(false);
  }else if(strcmp(argv[1], "typehtml") == 0){
    type_heterotable(true);
  }else{
    usage(argv[0]);
  }
  return EXIT_SUCCESS;
}
