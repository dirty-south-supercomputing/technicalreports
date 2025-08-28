#include "pgotypes.cpp"

static pmon pmons[TEAMSIZE];

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " pokéspec [ pokéspec... ]" << std::endl;
  std::cerr << "supply up to three pokéspexen" << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
  setlocale(LC_ALL, "");
  const char *argv0 = *argv;
  --argc;
  ++argv;
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    if(!argc && i){
      break;
    }
    uint16_t mhp;
    if(lex_pmon(&pmons[i], &mhp, &argc, &argv)){
      usage(argv0);
    }
  }
  return EXIT_SUCCESS;
}
