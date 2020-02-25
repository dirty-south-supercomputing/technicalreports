static int draw_tetriminos(struct notcurses* nc, struct ncplane** minos, int dimy, int dimx){
  if( (minos[0] = tetrimino_plane(nc, 0, dimy / 8, dimx / 2)) ){
    if( (minos[1] = tetrimino_plane(nc, 1, dimy / 4, dimx / 4)) ){
      if( (minos[2] = tetrimino_plane(nc, 2, dimy / 2, dimx / 5)) ){
        if( (minos[3] = tetrimino_plane(nc, 3, dimy * 3 / 4, dimx / 4)) ){
          if( (minos[4] = tetrimino_plane(nc, 4, dimy * 3 / 4, dimx * 3 / 4)) ){
            if( (minos[5] = tetrimino_plane(nc, 5, dimy / 2, dimx * 4 / 5)) ){
              if( (minos[6] = tetrimino_plane(nc, 6, dimy / 4, dimx * 3 / 4)) ){
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

