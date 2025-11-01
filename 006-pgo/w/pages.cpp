#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include "pages.h"

std::ostringstream& write_header(std::ostringstream& oss, const std::string& pagename){
  oss << "<!DOCTYPE html><html><head><title>";
  oss << pagename;
  oss << "</title>";
  oss << "</head><body>" << std::endl;
  return oss;
}

int write_footer(int fd, std::ostringstream& oss){
  oss << "</body></html>";
  auto osstr = oss.str();
  ssize_t wr;
  if((wr = write(fd, osstr.c_str(), osstr.length())) < 0){
    std::cerr << "error writing " << osstr.length() << " " << strerror(errno) << std::endl;
    return -1;
  }else if(static_cast<size_t>(wr) != osstr.length()){
    std::cerr << "short write of " << osstr.length() << " " << strerror(errno) << std::endl;
    return -1;
  }
  return 0;
}

std::string html_escape_str(const char* s){
  return s;
}
