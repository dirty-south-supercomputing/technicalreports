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
