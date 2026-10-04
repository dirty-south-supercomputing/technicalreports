#include "pgotypes.h"
#include <fstream>
#include <unistd.h>

static void
write_header(std::ofstream &fp, const std::string &title){
  fp << "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  fp << "<link rel=\"stylesheet\" type=\"text/css\" href=\"../list.css\">";
  fp << "<title>" << title << "</title></head>" << std::endl;
  fp << "<body>" << std::endl;
}

static void
write_footer_and_close(std::ofstream &fp){
  fp << "<hr/><a href=\"https://goldandappelpub.com/pgo-quantitative.html\"><i>Pokémon GO: A Quantitative Approach</i></a>";
  fp << "</body></html>" << std::endl;
  fp.close(); // FIXME check errors or enable exceptions
}

static void
encode_name(std::ostream &fp, const std::string &s){
  for(char c : s){
    if(!isspace(c) && !ispunct(c)){
      fp << c;
    }
  }
}

static void
encode_name(const std::string &s, std::string &encname){
  for(char c : s){
    if(!isspace(c) && !ispunct(c)){
      encname.push_back(c);
    }
  }
}

static int
write_mon_page(const species &s){
  std::string encname;
  encode_name(s.name, encname);
  std::ofstream fp(encname + ".html");
  if(!fp.is_open()){
    std::cerr << "error opening " << encname << ".html for writing" << std::endl;
    return -1;
  }
  write_header(fp, s.name);
  fp << "<img src=\"../images/mon/" << encname << ".png\" height=\"512\" width=\"512\" alt=\"" << s.name << "\"/>" << std::endl;
  fp << "<h1 id=\"monname\">#" << std::format("{:04d} ", s.idx) << s.name << "</h1>" << std::endl;
  // FIXME
  write_footer_and_close(fp);
  for(const auto &m : s.mforms){
    if(write_mon_page({&s, m})){
      return -1;
    }
  }
  return 0;
}

static void
write_summary(std::ostream &fp, const species &s){
  fp << "<tr>";
  fp << "<td>" << std::format("{:04d}", s.idx) << "</td><td>";
  html_types_pdir(fp, s.t1, s.t2);
  fp << "</td><td>";
  fp << "<a href=\"";
  encode_name(fp, s.name);
  fp << ".html\">" << s.name << "</a>";
  fp << "</td>";
  fp << "<td>";
  if(s.shiny){
    fp << "✓";
  }
  fp << "</td>";
  fp << "<td>";
  if(s.shadow){
    fp << "✓";
  }
  fp << "</td>";
  fp << "<td>";
  if(s.dmax){
    fp << "✓";
  }
  fp << "</td>";
  fp << "<td><img loading=\"lazy\" src=\"../images/mon/";
  encode_name(fp, s.name);
  fp << ".png\" height=\"64\" width=\"64\" alt=\"" << s.name << "\"/></td>";
  fp << "</tr>" << std::endl;
  for(const auto &m : s.mforms){
    write_summary(fp, {&s, m});
  }
}

// assume we're in the target directory
static int
write_index(){
  std::ofstream fp("index.html");
  if(!fp.is_open()){
    std::cerr << "error opening index.html for writing" << std::endl;
    return -1;
  }
  write_header(fp, "Pokémon GO forms");
  fp << "<div class=\"intro\">The following forms are available in Pokémon GO. ";
  fp << "Functionally equivalent forms are not distinguished. ";
  fp << "Battle-only forms are not listed.</div>" << std::endl;
  fp << "<table>" << std::endl;
  fp << "<tr><th>Dex#</th><th>T</th><th>Form</th><th>";
  fp << "<img src=\"../images/shiny.png\" class=\"type\" alt=\"Shiny\"/>";
  fp << "</th><th>";
  fp << "<img src=\"../images/shadow.png\" class=\"type\" alt=\"Shadow\"/>";
  fp << "</th><th>";
  fp << "<img src=\"../images/dynamax.png\" class=\"type\" alt=\"Dynamax\"/>";
  fp << "</th><th>Sprite</th></tr>" << std::endl;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    write_summary(fp, sdex[u]);
  }
  fp << "</table>" << std::endl;
  write_footer_and_close(fp);
  return 0;
}

static void
usage(const char *argv0, int ret){
  std::cerr << "usage: " << argv0 << " targetdir" << std::endl;
  exit(ret);
}

// generate a directory full of species pages, overwriting any that exist.
// an index.html is also generated.
int main(int argc, const char **argv){
  if(argc != 2){
    usage(argv[0], EXIT_FAILURE);
  }
  if(chdir(argv[1])){
    std::cerr << "error changing directory to " << argv[1] << ": " << strerror(errno) << std::endl;
    usage(argv[0], EXIT_FAILURE);
  }
  if(write_index()){
    return EXIT_FAILURE;
  }
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    if(write_mon_page(sdex[u])){
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
