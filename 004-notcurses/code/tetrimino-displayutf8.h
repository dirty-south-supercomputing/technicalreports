// tidx is an index into tetriminos. yoff and xoff are relative to the
// terminals's origin. returns colored north-facing tetrimino on a plane.
static struct ncplane*
tetrimino_plane(struct notcurses* nc, int tidx, int yoff, int xoff){
  const struct tetrimino* t = &tetriminos[tidx];
  const size_t cols = strlen(t->texture);
  struct ncplane* n = ncplane_new(nc, 2 * ROWS_PER_GROW, cols, yoff, xoff, NULL);
  if(n){
    ncplane_set_fg(n, t->color);
    size_t y = 0, x = 0;
    for(size_t i = 0 ; i < strlen(t->texture) ; ++i){
      if(t->texture[i] == '*'){
        //const bool full = '*' == t->texture[(i + strlen(t->texture) / 2) % strlen(t->texture)];
        ncplane_cursor_move_yx(n, y, x);
        if(ncplane_putstr(n, "██") < 0){
          ncplane_destroy(n);
          return NULL;
        }
      }
      if((x = ((x + 2) % cols)) == 0){
        ++y;
      }
    }
  }
  return n;
}
