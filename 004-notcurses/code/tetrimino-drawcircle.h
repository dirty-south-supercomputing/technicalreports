static int draw_tetriminos(struct ncplane* nc, struct ncplane** minos, int dimy, int dimx){
  const int centy = dimy / 2, centx = dimx / 2;
  const int radius = dimy < dimx ? dimy / 3 : dimx / 3;
  const float aspect = dimx / (float)dimy;
  const float sector = M_PI * 2 / TETRIMINO_COUNT;
  for(int i = 0 ; i < TETRIMINO_COUNT ; ++i){
    const bool lift = strspn(tetriminos[i].texture, " ") == strlen(tetriminos[i].texture) / 2;
    if(!(minos[i] = tetrimino_plane(nc, i, centy + radius * sin(sector * i) - (ROWS_PER_GROW + lift * 2),
        centx + aspect * radius * cos(sector * i) - strlen(tetriminos[i].texture) / (2 * COLS_PER_GCOL)))){
      while(--i){
        ncplane_destroy(minos[i]);
      }
      return -1;
    }
  }
  return 0;
}
