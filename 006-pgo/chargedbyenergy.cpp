#include "pgotypes.cpp"
#include <map>

using cmap = std::map<int, std::vector<const attack *>>;

// bin charged attacks by energy, then sort within by ppe
int main(void){
  cmap chargedmap;
  for(const auto &a : attacks){
    if(!charged_attack_p(a)){
      continue;
    }
    auto p = chargedmap.try_emplace(-a->energytrain);
    p.first->second.push_back(a);
  }
  for(auto & [k, v] : chargedmap){
    std::sort(v.begin(), v.end(), [](const attack *l, const attack *r){return l->powertrain < r->powertrain;});
    for(const auto &a : v){
      std::cout << -a->energytrain << " " << a->powertrain << " "
                << a->name << " " << TNames[a->type] << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
