#include <unistd.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sstream>
#include <cstring>
#include "pages.h"

int open_output(int dfd, const char *node){
  int indexfd = openat(dfd, node, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if(indexfd < 0){
    std::cerr << "error opening " << node << " (" << strerror(errno) << ")" << std::endl;
    return -1;
  }
  return indexfd;
}

int close_output(int fd){
  if(close(fd)){
    std::cerr << "error closing " << fd << " (" << strerror(errno) << ")" << std::endl;
    return -1;
  }
  return 0;
}

int write_page(int dfd, const char *name, int (*fxn)(int, int)){
  int fd = open_output(dfd, name);
  if(fd < 0){
    return -1;
  }
  int r = fxn(dfd, fd);
  if(close_output(fd)){
    return -1;
  }
  return r;
}

std::ostringstream& write_header(std::ostringstream& oss, const std::string& pagename){
  oss << "<!DOCTYPE html><html><head><title>";
  oss << pagename;
  oss << "</title>";
  oss << "</head><body>" << std::endl;
  oss << "<h1>" << pagename << "</h1><br/>" << std::endl;
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
