#include "html.h"

struct cand {
  const species *s;
  const attack *a;
};

// generate table of max attackers by fast move time(s)
int main(void){
  std::vector<cand> cands;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(s->name.contains("Crowned Sword")){
      cands.emplace_back(&*s, &ATK_Behemoth_Blade);
    }else if(s->name.contains("Crowned Shield")){
      cands.emplace_back(&*s, &ATK_Behemoth_Bash);
    }else if(s->name == "Eternatus"){
      cands.emplace_back(&*s, &ATK_Dynamax_Cannon);
    }else{
      if(!s->dmax){
        continue;
      }
      for(const auto *a : s->attacks){
        if(!fast_attack_p(*a)){
          continue;
        }
        cands.emplace_back(&*s, a);
      }
    }
  }
  std::sort(cands.begin(), cands.end(), [](const cand &lhs, const cand &rhs){
        if(lhs.a->animdur < rhs.a->animdur){
          return true; // more desirable
        }else if(rhs.a->animdur < lhs.a->animdur){
          return false;
        }
        auto lbulk = lhs.s->def * lhs.s->sta;
        auto rbulk = rhs.s->def * rhs.s->sta;
        if(lbulk < rbulk){
          return false; // less desirable
        }else if(rbulk < lbulk){
          return true;
        }
        if(lhs.a->name < rhs.a->name){
          return true;
        }
        return false;
      });
  std::cout << "<table id=\"dynafast\">" << std::endl;
  std::cout << "<tr><th>T</th><th>Form</th><th>Fast attack</th><th>Turns</th></tr>" << std::endl;
  for(const auto &c : cands){
    std::cout << "<tr>";
    std::cout << "<td>";
    html_types(c.s->t1, c.s->t2);
    std::cout << "</td>";
    std::cout << "<td>";
    link_to_name(std::cout, c.s->name, true);
    std::cout << "</td>";
    std::cout << "<td>";
    html_type(dmax_attack_type(c.a));
    std::cout << ' ';
    std::string prefix, suffix;
    emit_html_attack_fancy(std::cout, *c.s, c.a, prefix, suffix, true, false);
    std::cout << "</td>";
    std::cout << "<td>" << c.a->animdur << "</td>";
    std::cout << "</tr>" << std::endl;
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
