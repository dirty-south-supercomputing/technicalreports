static int draw_tetriminos(struct notcurses* nc, struct ncplane** minos, int dimy, int dimx){
  int ret = !(minos[0] = tetrimino_plane(nc, 0, dimy / 3, dimx / 3));
  ret |= !(minos[1] = tetrimino_plane(nc, 1, dimy / 3, dimx * 2 / 3));
  ret |= !(minos[2] = tetrimino_plane(nc, 2, dimy / 2, dimx / 4));
  ret |= !(minos[3] = tetrimino_plane(nc, 3, dimy / 2, dimx / 2));
  ret |= !(minos[4] = tetrimino_plane(nc, 4, dimy / 2, dimx * 3 / 4));
  ret |= !(minos[5] = tetrimino_plane(nc, 5, dimy * 2 / 3, dimx / 3));
  return ret | !(minos[6] = tetrimino_plane(nc, 6, dimy * 2 / 3, dimx * 2 / 3));
}

