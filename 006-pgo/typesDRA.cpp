#include "html.h"

static void
print_pop(pgo_types_e t1, pgo_types_e t2){
  if(t2 == t1){
    t2 = TYPECOUNT;
  }
  bool printed = false;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(s->t1 != t1){
      continue;
    }
    if(s->t2 != t2){
      continue;
    }
    if(printed){
      std::cout << ", ";
    }else{
      printed = true;
    }
    link_to_name(std::cout, s->name);
  }
}

struct typing {
  pgo_types_e t1, t2; // for monotypes, t2 == t1
  unsigned tras[6];   // number of types with typing relation [ -3 .. 2 ]
  float dra;
  unsigned pop1, pop2;// population of {t1, t2} and {t2, t1}
                      // pop2 == 0 for all monotypes

  typing(pgo_types_e T1, pgo_types_e T2) :
    t1(T1),
    t2(T2) {
    memset(tras, 0, sizeof(tras));
    for(pgo_types_e at = TYPESTART ; at < TYPECOUNT ; ++at){
      int tr = typing_relation(at, t1, t2);
      ++tras[tr + 3];
    }
    dra = 0;
    for(int i = -3 ; i < 3 ; ++i){
      dra += tras[i + 3] * pow(1.6, i);
    }
    dra /= static_cast<int>(TYPECOUNT);
    pop1 = typing_popcount(t1, t2);
    pop2 = typing_popcount(t2, t1);
  }

  void print() const {
    std::cout << "<tr>";
    std::cout << "<td>";
    html_types(t1, t2);
    if(t1 != t2){
      std::cout << "<br/>";
      html_types(t2, t1);
    }
    std::cout << "</td>";
    for(unsigned i = 0 ; i < sizeof(tras) / sizeof(*tras) ; ++i){
      std::cout << "<td>";
      if(tras[i]){
        std::cout << tras[i];
      }
      std::cout << "</td>";
    }
    std::cout << "<td>" << dra << "</td>";
    std::cout << "<td>";
    std::cout << pop1 << " ";
    print_pop(t1, t2);
    if(t1 != t2){
      std::cout << "<br/>" << pop2 << " ";
      print_pop(t2, t1);
    }
    std::cout << "</td>";
    std::cout << "</tr>" << std::endl;
  }

  inline bool operator<(const typing &rhs){
    if(dra < rhs.dra){
      return true;
    }else if(dra == rhs.dra){
      if(t1 < rhs.t1){
        return true;
      }else if(t1 == rhs.t1){
        if(t2 < rhs.t2){
          return true;
        }
      }
    }
    return false;
  }

};

// table of typings sorted by DRA
int main(){
  std::vector<typing> typings;
  for(pgo_types_e t1 = TYPESTART ; t1 < TYPECOUNT ; ++t1){
    for(pgo_types_e t2 = t1 ; t2 < TYPECOUNT ; ++t2){
      typings.emplace_back(t1, t2);
    }
  }
  std::sort(typings.begin(), typings.end());
  std::cout << std::fixed << std::setprecision(3);
  std::cout << "<table class=\"evenshade\" id=\"typesdra\">" << std::endl;
  std::cout << "<tr>";
  std::cout << "<th>T</th><th>E<sub>-3</sub></th><th>E<sub>-2</sub></th><th>E<sub>-1</sub></th><th>E<sub>0</sub></th><th>E<sub>1</sub></th><th>E<sub>2</sub></th>";
  std::cout << "<th>DRA</th><th style=\"width: 80%\">Pop</th>";
  std::cout << "</tr>" << std::endl;
  for(const auto &t : typings){
    t.print();
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
