// tidx is an index into tetriminos. yoff and xoff are relative to the
// terminals's origin. returns colored north-facing tetrimino on a plane.
static struct ncplane* tetrimino_plane(struct ncplane* nc, int tidx, int yoff, int xoff){
  const struct tetrimino* t = &tetriminos[tidx];
  const size_t cols = strlen(t->texture) / (2 * ROWS_PER_GROW);
  struct ncplane* n = ncplane_new(nc, 2 * ROWS_PER_GROW, cols, yoff, xoff, NULL, NULL);
  if(n){
    ncplane_set_fg_rgb(n, t->color);
    size_t y = 0, x = 0;
    for(size_t i = 0 ; i < strlen(t->texture) ; ++i){
      if(ncplane_putchar_yx(n, y, x, t->texture[i]) < 0){
        ncplane_destroy(n);
        return NULL;
      }
      y += ((x = (x + 1) % cols) == 0);
    }
  }
  return n;
}
