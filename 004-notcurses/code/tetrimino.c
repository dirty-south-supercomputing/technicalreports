#include <locale.h>
#include <notcurses.h>

static const int TETRIMINO_WIDTH = 4;
static const int TETRIMINO_HEIGHT = 4;

static struct ncplane*
tetrimino_plane(struct notcurses* nc){
  struct ncplane* n = ncplane_new(nc, TETRIMINO_HEIGHT, TETRIMINO_WIDTH, 0, 0, NULL);
  return n;
}

int main(void){
  if(!setlocale(LC_ALL, "")){
    return EXIT_FAILURE;
  }
  notcurses_options nopts;
  memset(&nopts, 0, sizeof(nopts));
  struct notcurses* nc;
  if((nc = notcurses_init(&nopts, stdout)) == NULL){
    return EXIT_FAILURE;
  }
  // FIXME
  return notcurses_stop(nc) ? EXIT_FAILURE : EXIT_SUCCESS;
}
