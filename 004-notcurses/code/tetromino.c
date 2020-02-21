#include <locale.h>
#include <unistd.h>
#include <notcurses/notcurses.h>

// "North-facing" tetrimino forms (the form in which they are released from the
// top) are expressed in terms of two rows having between two and four columns.
// We map each game column to four columns and each game column to two rows.
// strlen(texture) / COLS_PER_GCOL -> columns per (g)row for the tetrimino.
static const int COLS_PER_GCOL = 4;
static const int ROWS_PER_GROW = 2;

static const struct tetrimino {
  char name;
  unsigned color;
  const char* texture;
} tetriminos[] = {
     { 'O', 0xfada5e, "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" },
     { 'I', 0x34ccff, "                                "
                      "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII",
  }, { 'T', 0x800080, "    TTTT        TTTT    "
                      "TTTTTTTTTTTTTTTTTTTTTTTT",
  }, { 'L', 0xff6900, "        LLLL        LLLL"
                      "LLLLLLLLLLLLLLLLLLLLLLLL"
  }, { 'J', 0x004bbb, "LLLL        LLLL        "
                      "LLLLLLLLLLLLLLLLLLLLLLLL"
  }, { 'S', 0x32cd32, "    SSSSSSSS    SSSSSSSS"
                      "SSSSSSSS    SSSSSSSS    "
  }, { 'Z', 0xff3333, "SSSSSSSS    SSSSSSSS    "
                      "    SSSSSSSS    SSSSSSSS"
  }
};

static struct ncplane*
tetrimino_plane(struct notcurses* nc, int tidx){
  if(tidx < 0 || tidx >= sizeof(tetriminos) / sizeof(*tetriminos)){
    return NULL;
  }
  const struct tetrimino* t = &tetriminos[tidx];
  const size_t cols = strlen(t->texture) / (2 * ROWS_PER_GROW);
  struct ncplane* n = ncplane_new(nc, 2 * ROWS_PER_GROW, cols, 0, 0, NULL);
  if(n){
    ncplane_set_fg(n, t->color);
    int y = 0;
    size_t x = 0;
    for(size_t i = 0 ; i < strlen(t->texture) ; ++i){
      if(x == 0){
        if(ncplane_cursor_move_yx(n, y, x)){
          ncplane_destroy(n);
          return NULL;
        }
      }
      if(ncplane_putsimple(n, t->texture[i]) < 0){
        ncplane_destroy(n);
        return NULL;
      }
      if(++x == cols){
        ++y;
        x = 0;
      }
    }
  }
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
  bool failed = true;
  while(true){
    struct ncplane* n = tetrimino_plane(nc, random() % 7);
    if(!n){
      break;
    }
    if(notcurses_render(nc)){
      ncplane_destroy(n);
      break;
    }
    sleep(1);
    ncplane_destroy(n);
  }
  return (notcurses_stop(nc) || failed) ? EXIT_FAILURE : EXIT_SUCCESS;
}
