#include "html.h"

/* css
div.speciesivs {
  margin: auto;
}

table.speciesivs {
  margin: auto;
}
*/

static int
write_species_iv_sec(std::ostream &fp, const species &s, int iva, int ivd, int ivs){
  const auto atk = s.atk + iva;
  const auto def = s.def + ivd;
  const auto sta = s.sta + ivs;
  fp << "<div class=\"speciesivs\">";
  fp << s.name << "<br/>" << std::endl;
  fp << "<table class=\"speciesivs\">" << std::endl;
  constexpr int PERROW = 3;
  fp << "<tr>";
  for(int j = 0 ; j < PERROW ; ++j){
    fp << "<th>L</th><th>CP</th>";
  }
  fp << "</tr>" << std::endl;
  for(unsigned i = 0 ; i < (MAX_HALFLEVEL + PERROW - 1) / PERROW ; ++i){
    fp << "<tr>";
    for(int j = 0 ; j < PERROW ; ++j){
      auto hl = i * 3 + j + 1;
      fp << "<td>";
      if(hl <= MAX_HALFLEVEL){
        emit_halflevel_as_level(fp, hl);
        fp << "</td><td>";
        fp << calccp(atk, def, sta, hl);
      }else{
        fp << "</td><td>";
      }
      fp << "</td>";
    }
    fp << "</tr>" << std::endl;
  }
  fp << "</table>";
  fp << "</div>" << std::endl;
  return 0;
}

// each family has 4,096 IV pages. each one details:
//
//  - for each species, that IV's rank and stats
//  - if there are shadows, the same for purified stats
//  - CP at each level for each species
//
// call with the base member of the family. we'll handle all upwards transitions.
int write_family_iv_page(std::ostream &fp, const species &base, int iva, int ivd, int ivs){
  std::vector<const species *> evols;
  evols.push_back(&base);
  do{
    const auto &s = *evols.front();
    write_species_iv_sec(fp, s, iva, ivd, ivs);
    get_persistent_evolutions(s, evols);
    evols.erase(evols.begin());
  }while(evols.size());
  return 0;
}

int main(int argc, const char **argv){
  if(argc != 2){
    return EXIT_FAILURE;
  }
  const auto *s = lookup_species(argv[1]);
  if(!s){
    std::cerr << "error looking up " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }
  write_family_iv_page(std::cout, *s, 15, 15, 15);
  return EXIT_SUCCESS;
}
