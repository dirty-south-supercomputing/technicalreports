#include "pgotypes.h"
#include <fstream>
#include <unistd.h>

static void
write_header(std::ofstream &fp, const char *title){
  fp << "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  fp << "<title>" << title << "</title></head>" << std::endl;
}

static void
write_footer_and_close(std::ofstream &fp){
  fp << "<hr/><a href=\"https://goldandappelpub.com/pgo-quantitative.html\"><i>Pokémon GO: A Quantitative Approach</i></a>";
  fp << "</body></html>" << std::endl;
  fp.close(); // FIXME check errors or enable exceptions
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
  fp << "<body>" << std::endl;
  fp << "<div class=\"intro\">The following forms are available in Pokémon GO. ";
  fp << "Functionally equivalent forms are not distinguished.</div>" << std::endl;
  fp << "<table>" << std::endl;
  fp << "<tr><th>Dex#</th><th>T</th><th>Form</th></tr>" << std::endl;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const auto &s = sdex[u];
    fp << "<tr>";
    fp << "<td>" << std::format("{:04d}", s.idx) << "</td><td>";
    html_types(fp, s.t1, s.t2);
    fp << "</td><td>" << s.name << "</td>";
    fp << "</tr>" << std::endl;
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
  return EXIT_SUCCESS;
}
