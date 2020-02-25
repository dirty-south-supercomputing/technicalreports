// tidx is an index into tetriminos. yoff and xoff are relative to the
// terminals's origin. returns colored north-facing tetrimino on a plane.
static struct ncplane*
tetrimino_plane(struct notcurses* nc, int tidx, int yoff, int xoff){
  const struct tetrimino* t = &tetriminos[tidx];
  const size_t cols = strlen(t->texture);
  struct ncplane* n = ncplane_new(nc, 2 * ROWS_PER_GROW, cols, yoff, xoff, NULL);
  uint64_t channels;
  channels_set_bg_alpha(&channels, CELL_ALPHA_TRANSPARENT);
  channels_set_fg_alpha(&channels, CELL_ALPHA_TRANSPARENT);
  if(n){
    ncplane_set_fg(n, t->color);
    ncplane_set_base(n, channels, 0, "");
    size_t y = 0, x = 0;
    for(size_t i = 0 ; i < strlen(t->texture) ; ++i){
      if(t->texture[i] == '*'){
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
