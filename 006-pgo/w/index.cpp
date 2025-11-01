#include "pages.h"
#include "index.h"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>

// a top level page, all of which are linked to from the index page
struct toppage {
  const std::string link;
  const std::string name;
  int (*fxn)(int dfd, int fd);
};

constexpr auto AllFormsPage = "all forms";
constexpr auto WhoThrowsPage = "who throws these charged types?";

int write_allforms(int dfd, int fd){
  std::ostringstream os;
  write_header(os, AllFormsPage);
  // FIXME write links
  return write_footer(fd, os);
}

int write_whothrows(int dfd, int fd){
  std::ostringstream os;
  write_header(os, WhoThrowsPage);
  // FIXME write
  return write_footer(fd, os);
}

const std::vector<toppage> toppages = {
  { "allforms.html", AllFormsPage, write_allforms, },
  { "whothrowschargedset.html", WhoThrowsPage, write_whothrows, },
};

int write_index(int dfd, int fd){
  constexpr auto Pagename = "pokémon go analysis";
  std::ostringstream os;
  write_header(os, Pagename);
  for(const auto &tp : toppages){
    os << "<a href=\"" << tp.link << "\">" << tp.name << "</a><br/>" << std::endl;
    if(tp.fxn){
      if(write_page(dfd, tp.link.c_str(), tp.fxn)){
        return -1;
      }
    }
  }
  return write_footer(fd, os);
}
