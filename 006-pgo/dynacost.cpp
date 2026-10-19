#include "html.h"

int main(void){
  std::vector<const species*> dynas;
  std::forward_list<species> gmax;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(s->dmax && s->dmax != UINT_MAX){
      dynas.push_back(&*s);
    }
    const auto gma = lookup_gmax_attack(s->name);
    if(gma){
      gmax.emplace_front(&*s, *gma);
      dynas.push_back(&gmax.front());
    }
  }
  std::sort(dynas.begin(), dynas.end(), [](const species *lhs, const species *rhs){
        if(lhs->dmax > rhs->dmax){
          return true;
        }else if(lhs->dmax == rhs->dmax){
          if(lhs->name > rhs->name){
            return true;
          }
        }
        return false;
      });
  std::cout << "<table class=\"evenshade\" id=\"dynacost\">" << std::endl;
  std::cout << "<tr><th>Dex#</th><th>T</th><th>Pokémon</th><th>Tier</th><th>Cost (MP)</th></tr>" << std::endl;
  for(const auto s : dynas){
    std::cout << "<tr><td>" << s->idx << "</td><td>";
    html_types(s->t1, s->t2);
    std::cout << "</td><td>" << s->name << "</td><td>" << s->dmax << "</td><td>";
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
