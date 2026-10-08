#include "html.h"

int main(void){
  std::vector<const species*> dynas;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(s->dmax && s->dmax != UINT_MAX){
      dynas.push_back(&*s);
    }
  }
  // FIXME add gmaxen
  std::sort(dynas.begin(), dynas.end(), [](const species *lhs, const species *rhs){
        if(lhs->dmax < rhs->dmax){
          return true;
        }else if(lhs->dmax == rhs->dmax){
          if(lhs->name < rhs->name){
            return true;
          }
        }
        return false;
      });
  std::cout << "<table>" << std::endl;
  std::cout << "<tr><th>Dex#</th><th>Pokémon</th><th>Tier</th><th>Cost (MP)</th></tr>" << std::endl;
  for(const auto s : dynas){
    std::cout << "<tr><td>" << s->idx << "</td><td>";
    std::cout << s->name << "</td><td>" << s->dmax << "</td><td>";
    switch(s->dmax){
      case 1: std::cout << 250; break;
      case 2: std::cout << 400; break;
      case 3: std::cout << 400; break;
      case 4: std::cout << 800; break;
      case 5: std::cout << 800; break;
      case 6: std::cout << 800; break;
      default:
        std::cerr << "invalid dmax tier " << s->dmax << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "</td></tr>" << std::endl;
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
