#include "html.h"

// generate table of max attackers by bulk
int main(void){
  std::vector<const species *> cands;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(!s->dmax){
      if(!s->name.contains("Crowned") && !s->name.contains("Eternatus")){
        continue;
      }
    }
    cands.emplace_back(&*s);
  }
  std::sort(cands.begin(), cands.end(), [](const species *lhs, const species *rhs){
        auto lbulk = lhs->def * lhs->sta;
        auto rbulk = rhs->def * rhs->sta;
        if(lbulk < rbulk){
          return false; // less desirable
        }else if(rbulk < lbulk){
          return true;
        }
        if(lhs->name < rhs->name){
          return true;
        }else if(lhs->name > rhs->name){
          return false;
        }
        return false;
      });
  std::cout << "<table class=\"evenshade\" id=\"dynabulk\">" << std::endl;
  std::cout << "<tr><th/><th>Form</th><th>DEF</th><th>STA</th><th>Bulk</th></tr>" << std::endl;
  for(const auto &c : cands){
    std::cout << "<tr>";
    std::cout << "<td>";
    html_types(c->t1, c->t2, false);
    std::cout << "</td>";
    std::cout << "<td>";
    link_to_name(std::cout, c->name, true);
    std::cout << "</td>";
    std::cout << "<td>";
    std::cout << c->def;
    std::cout << "</td>";
    std::cout << "<td>";
    std::cout << c->sta;
    std::cout << "</td>";
    std::cout << "<td>" << c->def * c->sta << "</td>";
    std::cout << "</tr>" << std::endl;
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
