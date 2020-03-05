// tidx is an index into tetriminos. yoff and xoff are relative to the
// terminal's origin. returns colored north-facing tetrimino on a plane.
static struct ncplane* tetrimino_plane(struct notcurses* nc, int tidx, int yoff, int xoff){
  const struct tetrimino* t = &tetriminos[tidx];
  const size_t cols = strlen(t->texture);
  struct ncplane* n = ncplane_new(nc, ROWS_PER_GROW, cols, yoff, xoff, NULL);
  if(n){
    uint64_t channels = 0;
    channels_set_bg_alpha(&channels, CELL_ALPHA_TRANSPARENT);
    channels_set_fg_alpha(&channels, CELL_ALPHA_TRANSPARENT);
    ncplane_set_fg(n, t->color);
    ncplane_set_base(n, channels, 0, "");
    size_t y = 0, x = 0;
    for(size_t i = 0 ; i < strlen(t->texture) ; ++i){
      if(t->texture[i] == '*'){
        if(ncplane_putstr_yx(n, y, x, "██") < 0){
          ncplane_destroy(n);
          return NULL;
        }
      }
      y += ((x = ((x + 2) % cols)) == 0);
    }
  }
  return n;
}
