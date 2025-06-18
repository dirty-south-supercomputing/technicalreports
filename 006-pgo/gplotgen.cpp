#include "pgotypes.cpp"

// plot atk against bulk (sta * def)
static void
atkvbulk(void){

}

static const struct {
  const char *name;
  void(*fxn)(void);
} plots[] = {
  { "atkvbulk", atkvbulk, },
};

static void
usage(const char *argv0){
  fprintf(stderr, "usage: %s graphname\n", argv0);
  exit(EXIT_FAILURE);
}

int main(int argc, const char **argv){
  if(argc != 2){
    usage(argv[0]);
  }
  for(auto &p : plots){ 
    if(strcmp(p.name, argv[1]) == 0){
      p.fxn();
      return EXIT_SUCCESS;
    }
  }
  usage(argv[0]);
}
