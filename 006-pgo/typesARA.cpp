#include "pgotypes.h"

int choose2(int n){
  return n * (n - 1) / 2;
}

struct atype {
  pgo_types_e at;
  unsigned tas[4]; // type relationships [ -2 .. 1 ]
  unsigned tras[6]; // typing relationships [ -3 .. 2 ]
  float ara;

  float solve_ara(){
    memset(tas, 0, sizeof(tas));
    memset(tras, 0, sizeof(tras));
    // number of type relations for each relation [ -2 .. 1 ]
    for(pgo_types_e t = TYPESTART ; t < TYPECOUNT ; ++t){
      int tr = type_relation(at, t);
      ++tas[tr + 2];
    }
    // number of typing relations for each relation [ -3 .. 2 ]
    tras[0] = tas[0] * tas[1]; // -3 = r(-2) * r(-1)
    tras[1] = tas[0] * (tas[2] + 1) + choose2(tas[1]); // -2 = r(-2) * (r(0) + 1) + choose(r(-1))
    tras[2] = tas[1] * (tas[2] + 1) + tas[0] * tas[3];
    tras[3] = tas[2] + choose2(tas[2]) + tas[3] * tas[1];
    tras[4] = tas[3] * (tas[2] + 1);
    tras[5] = choose2(tas[3]);
    for(int i = -3 ; i < 3 ; ++i){
      ara += (tras[i + 3] * pow(1.6, i));
    }
    ara /= TYPINGCOUNT;
    return ara;
  }

  atype(pgo_types_e At) :
    at(At) {
    solve_ara();
  }

  bool operator>(const atype& rhs){
    if(ara > rhs.ara){
      return true;
    }else if(ara == rhs.ara){
      if(at > rhs.at){
        return true;
      }
    }
    return false;
  }

  void print() const {
    std::cout << "<tr>";
    std::cout << "<td>";
    html_type(at);
    std::cout << "</td>";
    for(int i = -2 ; i < 2 ; ++i){
      std::cout << "<td>" << tas[i + 2] << "</td>";
    }
    for(int i = -3 ; i < 3 ; ++i){
      std::cout << "<td>" << tras[i + 3] << "</td>";
    }
    std::cout << "<td>" << ara << "</td>";
    std::cout << "</tr>" << std::endl;
  }

};

// generate table of attack types and their ARAs
int main(void){
  std::cout << std::fixed << std::setprecision(3);
  std::cout << "<table class=\"alignright\">" << std::endl;
  std::cout << "<tr>";
  std::cout << "<th>T</th><th>R<sub>-2</sub></th><th>R<sub>-1</sub></th><th>R<sub>0</sub></th><th>R<sub>1</sub></th>";
  std::cout << "<th>E<sub>-3</sub></th><th>E<sub>-2</sub></th><th>E<sub>-1</sub></th><th>E<sub>0</sub></th><th>E<sub>1</sub></th><th>E<sub>2</sub></th>";
  std::cout << "<th>ARA</th>";
  std::cout << "</tr>" << std::endl;
  std::vector<atype> atypes;
  for(pgo_types_e a = TYPESTART ; a < TYPECOUNT ; ++a){
    atypes.emplace_back(a);
  }
  std::sort(atypes.begin(), atypes.end(), std::greater());
  for(const auto &at : atypes){
    at.print();
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
