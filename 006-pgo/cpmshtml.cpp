#include "html.h"

static inline double
cpmpct(int halflevel){
  return cpm(halflevel) * 100 / cpm(MAX_HALFLEVEL_BASIC);
}

static void
print_cpms(void){
  std::cout << "<table class=\"evenshade\" id=\"cpms\">" << std::endl;
  std::cout << "<tr><th>Level</th><th>CPM</th><th>%</th></tr>" << std::endl;
  for(unsigned i = 1 ; i <= MAX_HALFLEVEL ; ++i){
    std::cout << "<tr><td>";
    emit_halflevel_as_level(std::cout, i);
    std::cout << "</td><td>";
    std::cout << cpm(i);
    std::cout << "</td><td>";
    std::cout << cpmpct(i);
    std::cout << "</td></tr>" << std::endl;
  }
  std::cout << "</table>" << std::endl;
}

int main(void){
  print_cpms();
}
