static int
draw_tetriminos(struct notcurses* nc, struct ncplane** minos, int y, int x){
  if( (minos[0] = tetrimino_plane(nc, 0, y / 3, x / 3)) ){
    if( (minos[1] = tetrimino_plane(nc, 1, y / 3, x * 2 / 3)) ){
      if( (minos[2] = tetrimino_plane(nc, 2, y / 2, x / 4)) ){
        if( (minos[3] = tetrimino_plane(nc, 3, y / 2, x / 2)) ){
          if( (minos[4] = tetrimino_plane(nc, 4, y / 2, x * 3 / 4)) ){
            if( (minos[5] = tetrimino_plane(nc, 5, y * 2 / 3, x / 3)) ){
              if( (minos[6] = tetrimino_plane(nc, 6, y * 2 / 3, x * 2 / 3)) ){
                return 0;
              } ncplane_destroy(minos[5]);
            } ncplane_destroy(minos[4]);
          } ncplane_destroy(minos[3]);
        } ncplane_destroy(minos[2]);
      } ncplane_destroy(minos[1]);
    } ncplane_destroy(minos[0]);
  }
  return -1;
}
