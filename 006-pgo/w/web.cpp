#include "../pgotypes.h"
#include "mon.h"
#include "pages.h"
#include "index.h"
#include <fcntl.h>
#include <unistd.h>

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
