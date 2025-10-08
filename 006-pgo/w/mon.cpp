#include "../pgotypes.h"
#include "mon.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>
#include <iostream>

#define MONDIR "forms"

static int
write_mon(int fd){
  return 0;
}

int write_mon_pages(int dfd){
  if(mkdirat(dfd, MONDIR, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)){
    if(errno != EEXIST){
      std::cout << "error creating subdir " << MONDIR << " (" << std::strerror(errno) << ")" << std::endl;
      return -1;
    }
  }
  // FIXME need raii
  int mdfd = openat(dfd, MONDIR, O_RDWR | O_DIRECTORY | O_PATH);
  if(mdfd < 0){
    std::cout << "error opening subdir " << MONDIR << " (" << std::strerror(errno) << ")" << std::endl;
    return -1;
  }
  for(unsigned idx = 0 ; idx < SPECIESCOUNT ; ++idx){
    const species &s = sdex[idx];
    auto fname = file_escape_str(s.name);
    // FIXME need raii
    int fd = openat(mdfd, fname, O_RDWR | O_CREAT | S_IRUSR);
    if(fd < 0){
      std::cout << "error opening subdir " << MONDIR << " (" << std::strerror(errno) << ")" << std::endl;
      return -1;
    }
    if(write_mon(fd)){
      return -1;
    }
  }
  return 0;
}
