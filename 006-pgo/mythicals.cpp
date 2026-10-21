#include "html.h"

static void
encode_catstr(const std::string &str){
  for(auto c : str){
    if(!isspace(c)){
      std::cout << c;
    }
  }
}

static void
emit_cat_table(species::species_cat cat){
  std::cout << "<h2>" << species::categorystr(cat) << "</h2>" << std::endl;
  std::cout << "<table id=\"";
  encode_catstr(species::categorystr(cat));
  std::cout << "\">" << std::endl;
  std::cout << "<tr><th>T</th><th>Form</th><th>";
  std::cout << "<img src=\"images/shiny.png\" class=\"type\" alt=\"Shiny\"/>";
  std::cout << "</th><th>";
  std::cout << "<img src=\"images/shadow.png\" class=\"type\" alt=\"Shadow\"/>";
  std::cout << "</th><th>";
  std::cout << "<img src=\"images/dynamax.png\" class=\"type\" alt=\"Dynamax\"/>";
  std::cout << "</th></tr>" << std::endl;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(s->category == cat){
      std::cout << "<tr>";
      std::cout << "<td>";
      html_types(s->t1, s->t2);
      std::cout << "</td>";
      std::cout << "<td>";
      link_to_name(std::cout, s->name, true);
      std::cout << "</td>";
      std::cout << "<td>";
      if(s->shiny){
        std::cout << "✓";
      }
      std::cout << "</td>";
      std::cout << "<td>";
      if(s->shadow){
        std::cout << "✓";
      }
      std::cout << "</td>";
      std::cout << "<td>";
      if(s->dmax){
        std::cout << "✓";
      }
      std::cout << "</td>";
      std::cout << "</tr>" << std::endl;
    }
  }
  std::cout << "</table>" << std::endl;
}

// generate html tables of mythicals, legendaries, and ultra beasts
int main(void){
  const species::species_cat cats[] = {
    species::CAT_MYTHICAL,
    species::CAT_LEGENDARY,
    species::CAT_FUSION,
    species::CAT_CROWNED,
    species::CAT_ULTRABEAST,
  };
  for(auto c : cats){
    emit_cat_table(c);
  }
  return EXIT_SUCCESS;
}
