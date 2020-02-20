#include <locale.h>
#include <notcurses/notcurses.h>

// "North-facing" tetrimino forms (the form in which they are released from the
// top) are expressed in terms of two rows having between two and four columns.
// We map each game column to four columns and each game column to two rows.
// strlen(texture) / 2 / 4 -> game columns per game row for the tetrimino.
static const int COLS_PER_GCOL = 4;
static const int ROWS_PER_GROW = 2;

static const struct tetrimino {
  char name;
  unsigned color;
  const char* texture;
} tetriminos[] = {
     { 'O', 0xfada5e, "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" },
     { 'I', 0x34ccff, "                                                "
                      "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII",
  }, { 'T', 0x800080, "    TTTT        TTTT    "
                      "TTTTTTTTTTTTTTTTTTTTTTTT",
  }, { 'L', 0xff6900, "        LLLL        LLLL"
                      "LLLLLLLLLLLLLLLLLLLLLLLL"
  }, { 'J', 0x00008b, "LLLL        LLLL        "
                      "LLLLLLLLLLLLLLLLLLLLLLLL"
  }, { 'S', 0x32cd32, "    SSSSSSSS    SSSSSSSS"
                      "SSSSSSSS    SSSSSSSS    "
  }, { 'Z', 0xff3333, "SSSSSSSS    SSSSSSSS    "
                      "    SSSSSSSS    SSSSSSSS"
  }
};

static struct ncplane*
tetrimino_plane(struct notcurses* nc, int tidx){
  const struct tetrimino* t = &tetriminos[tidx];
  struct ncplane* n = ncplane_new(nc, 2 * ROWS_PER_GROW,
                                  strlen(t->texture) / 8, 0, 0, NULL);
  // FIXME draw it
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
