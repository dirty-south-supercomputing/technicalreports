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
  bool failed = !(minos[0] = tetrimino_plane(nc, 0, dimy / 3, dimx / 3));
  failed |= !(minos[1] = tetrimino_plane(nc, 1, dimy / 3, dimx * 2 / 3));
  failed |= !(minos[2] = tetrimino_plane(nc, 2, dimy / 2, dimx / 4));
  failed |= !(minos[3] = tetrimino_plane(nc, 3, dimy / 2, dimx / 2));
  failed |= !(minos[4] = tetrimino_plane(nc, 4, dimy / 2, dimx * 3 / 4));
  failed |= !(minos[5] = tetrimino_plane(nc, 5, dimy * 2 / 3, dimx / 3));
  failed |= !(minos[6] = tetrimino_plane(nc, 6, dimy * 2 / 3, dimx * 2 / 3));
  failed |= !!notcurses_render(nc);
  return (notcurses_stop(nc) || failed) ? EXIT_FAILURE : EXIT_SUCCESS;
}
