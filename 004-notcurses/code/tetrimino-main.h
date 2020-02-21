int main(void){
  if(!setlocale(LC_ALL, "")){
    return EXIT_FAILURE;
  }
  notcurses_options nopts;
  memset(&nopts, 0, sizeof(nopts));
  nopts.inhibit_alternate_screen = true;
  struct notcurses* nc;
  if((nc = notcurses_init(&nopts, stdout)) == NULL){
    return EXIT_FAILURE;
  }
  int dimy, dimx;
  notcurses_term_dim_yx(nc, &dimy, &dimx);
  struct ncplane* minos[sizeof(tetriminos) / sizeof(*tetriminos)]; // 2-3-2 
  int failed = draw_tetriminos(nc, minos, dimy, dimx) || notcurses_render(nc);
  return (notcurses_stop(nc) || failed) ? EXIT_FAILURE : EXIT_SUCCESS;
}
