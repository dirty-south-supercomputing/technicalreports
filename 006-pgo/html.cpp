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
