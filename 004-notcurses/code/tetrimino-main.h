int main(void){
  if(!setlocale(LC_ALL, "")){
    return EXIT_FAILURE;
  }
  notcurses_options nopts;
  memset(&nopts, 0, sizeof(nopts));
  nopts.flags = NCOPTION_NO_ALTERNATE_SCREEN;
  struct notcurses* nc;
  if((nc = notcurses_init(&nopts, stdout)) == NULL){
    return EXIT_FAILURE;
  }
  int dimy, dimx;
  struct ncplane* nstd = notcurses_stddim_yx(nc, &dimy, &dimx);
  struct ncplane* minos[TETRIMINO_COUNT]; // 2-3-2
  int failed = draw_tetriminos(nstd, minos, dimy, dimx) || notcurses_render(nc);
  return (notcurses_stop(nc) || failed) ? EXIT_FAILURE : EXIT_SUCCESS;
}
