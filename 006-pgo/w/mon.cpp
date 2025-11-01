#include "../pgotypes.h"
#include "pages.h"
#include "index.h"
#include "mon.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>

#define MONDIR "forms"

static int
write_mon(int fd, const species &s){
  std::ostringstream os;
  auto htmlname = html_escape_str(s.name.c_str());
  write_header(os, htmlname);
  // FIXME
  write_footer(fd, os);
  return 0;
}

int write_mon_pages(int dfd){
  if(mkdirat(dfd, MONDIR, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)){
    if(errno != EEXIST){
      std::cerr << "error creating subdir " << MONDIR << " (" << std::strerror(errno) << ")" << std::endl;
      return -1;
    }
  }
  // FIXME need raii
  int mdfd = openat(dfd, MONDIR, O_RDWR | O_DIRECTORY | O_PATH);
  if(mdfd < 0){
    std::cerr << "error opening subdir " << MONDIR << " (" << std::strerror(errno) << ")" << std::endl;
    return -1;
  }
  for(unsigned idx = 0 ; idx < SPECIESCOUNT ; ++idx){
    const species &s = sdex[idx];
    auto fname = file_escape_str(s.name, nullptr, ".html");
    // FIXME need raii
    int fd = openat(mdfd, fname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(fd < 0){
      std::cerr << "error opening file " << fname << " (" << std::strerror(errno) << ")" << std::endl;
      return -1;
    }
    if(write_mon(fd, s)){
      return -1;
    }
    if(close(fd) < 0){
      std::cerr << "error closing " << fname << " at " << fd << std::endl;
      return -1;
    }
  }
  return 0;
}
