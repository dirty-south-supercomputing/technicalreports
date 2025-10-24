#include "../pgotypes.h"
#include "mon.h"
#include "index.h"
#include <fcntl.h>
#include <unistd.h>

static int
open_output(int dfd, const char *node){
  int indexfd = openat(dfd, node, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if(indexfd < 0){
    std::cerr << "error opening " << node << " (" << strerror(errno) << ")" << std::endl;
    return -1;
  }
  return indexfd;
}

static int
close_output(int fd){
  if(close(fd)){
    std::cerr << "error closing " << fd << " (" << strerror(errno) << ")" << std::endl;
    return -1;
  }
  return 0;
}

static int
write_page(int dfd, const char *name, int (*fxn)(int)){
  int fd = open_output(dfd, name);
  if(fd < 0){
    return -1;
  }
  int r = fxn(fd);
  if(close_output(fd)){
    return -1;
  }
  return r;
}

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " outdir" << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, const char **argv){
  const char *argv0 = *argv;
  if(argc != 2){
    usage(argv0);
  }
  int dfd = open(argv[1], O_DIRECTORY | O_PATH | O_CLOEXEC | O_RDWR);
  if(dfd < 0){
    std::cerr << "error opening " << argv[1] << " (" << strerror(errno) << ")" << std::endl;
    usage(argv0);
  }
  int r = write_page(dfd, "index.html", write_index);
  if(write_mon_pages(dfd)){
    return EXIT_FAILURE;
  }
  if(close(dfd)){
    std::cerr << "error closing " << argv[1] << " (" << strerror(errno) << ")" << std::endl;
    return EXIT_FAILURE;
  }
  return r ? EXIT_FAILURE : EXIT_SUCCESS;
}
