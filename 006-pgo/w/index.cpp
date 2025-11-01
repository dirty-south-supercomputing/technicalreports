#include "pages.h"
#include "index.h"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>

constexpr auto Pagename = "pokémon go analysis";

// a top level page, all of which are linked to from the index page
struct toppage {
  const std::string link;
  const std::string name;
};

const std::vector<toppage> toppages = {
  { "whothrowschargedset.html", "who throws these charged types?" },
};

int write_index(int fd){
  std::ostringstream os;
  write_header(os, Pagename);
  for(const auto &tp : toppages){
    os << "<a href=\"" << tp.link << "\">" << tp.name << "</a><br/>" << std::endl;
  }
  return write_footer(fd, os);
}
