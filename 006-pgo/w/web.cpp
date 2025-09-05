#include "../pgotypes.cpp"
#include <fcntl.h>

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
  }
  return EXIT_SUCCESS;
}
