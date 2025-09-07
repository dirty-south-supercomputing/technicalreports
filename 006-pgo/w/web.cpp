#include "../pgotypes.cpp"
#include <fcntl.h>
#include <unistd.h>

static int
open_output(int dfd, const char *node){
  int indexfd = openat(dfd, node, O_CREAT);
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
write_page(int dfd){
  int indexfd = open_output(dfd, "index.html");
  if(indexfd < 0){
    return -1;
  }
  if(close_output(indexfd)){
    return -1;
  }
  return 0;
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
  int dfd = open(argv[1], O_DIRECTORY);
  if(dfd < 0){
    std::cerr << "error opening " << argv[1] << " (" << strerror(errno) << ")" << std::endl;
    usage(argv0);
  }
  int r = write_page(dfd);
  if(close(dfd)){
    std::cerr << "error opening " << argv[1] << " (" << strerror(errno) << ")" << std::endl;
    return EXIT_FAILURE;
  }
  return r ? EXIT_FAILURE : EXIT_SUCCESS;
}
