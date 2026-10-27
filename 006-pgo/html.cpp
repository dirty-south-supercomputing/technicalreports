#include "html.h"

// it's always [ -3, 2 ], with 0 (neutral) at the fourth column, which must be white
const std::string reversecss[6] = { "teneg3", "teneg2", "teneg1", "tepos0", "tepos1", "tepos2", };
const std::string css[6] = { "tepos3", "tepos2", "tepos1", "tepos0", "teneg1", "teneg2", };

void write_navbar(std::ostream &fp, bool par){
  fp << "<nav><ul>" << std::endl;
  fp << "<li><a href=\"";
  if(par){
    fp << "../";
  }
  fp << "index.html\">main</a></li>" << std::endl;
  fp << "<li><a href=\"";
  if(par){
    fp << "../";
  }
  fp << "mon/index.html\">forms</a></li>" << std::endl;
  fp << "</ul></nav>" << std::endl;
}

void write_header(std::ofstream &fp, const std::string &title, bool par){
  fp << "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  fp << "<link rel=\"stylesheet\" type=\"text/css\" href=\"../pgo.css\">";
  fp << "<title>" << title << "</title></head>" << std::endl;
  fp << "<body>" << std::endl;
  write_navbar(fp, par);
}

void html_weather(std::ostream &fp, pgo_weather_t w, bool par){
  fp << "<img src=\"";
  if(par){
    fp << "../";
  }
  fp << "images/" << WNames[w] << "-icon.png\" class=\"type\" alt=\""<< WNames[w] << "\"/> (";
  fp << WNames[w] << ")";
}

void html_weather(std::ostream &fp, pgo_types_e t1, pgo_types_e t2, bool par){
  pgo_weather_t w1, w2;
  if(t2 == TYPECOUNT){
    t2 = t1;
  }
  typing_weather_boosts(t1, t2, w1, w2);
  html_weather(fp, w1, par);
  if(w2 != w1){
    fp << ", ";
    html_weather(fp, w2, par);
  }
}

void emit_cand_html(const candidate& c, unsigned maxp){
  auto rp = c.powprod();
  //unsigned hhalf;
  std::cout << "<tr>";
  std::cout << "<td>";
  html_types(c.s->t1, c.s->t2);
  std::cout << "</td>";
  std::cout << "<td>";
  link_to_name(std::cout, c.s->name, true);
  std::cout << "</td>";
  std::cout << "<td>";
  html_type(c.atype);
  std::cout << ' ';
  // we do not attempt to indicate that a max attack is based on a legacy attack,
  // since it is possible that more than one fast attack enable the same max
  // attack, and they could have different legacy status (this happens with e.g.
  // dynamax machamp, which could have max knuckle from karate chop or counter).
  if(!c.hasstab){
    std::cout << "<i>";
  }
  std::cout << c.aname;
  if(!c.hasstab){
    std::cout << "</i>";
  }
  std::cout << "</td>";
  std::cout << std::setprecision(0) << std::fixed;
  for(int i = -3 ; i < 3 ; ++i){
    std::cout << "<td class=\"" << css[i + 3] << "\">" << (rp * type_effectiveness_mult(i)) << "</td>";
  }
  std::cout << std::setprecision(2) << std::fixed << "<td>" << (rp * 100.0 / maxp) << "%</td>";
  std::cout << std::setprecision(0) << std::fixed;
  std::cout << "</tr>" << std::endl;
}
