// tidx is an index into tetriminos. yoff and xoff are relative to the
// terminal's origin. returns colored north-facing tetrimino on a plane.
static struct ncplane* tetrimino_plane(struct ncplane* nc, int tidx, int yoff, int xoff){
  const struct tetrimino* t = &tetriminos[tidx];
  const size_t cols = strlen(t->texture);
  struct ncplane_options nopts = {
    .y = yoff, .x = xoff, .rows = ROWS_PER_GROW, .cols = cols,
  };
  struct ncplane* n = ncplane_create(nc, &nopts);
  if(n){
    uint64_t channels = 0;
    ncchannels_set_bg_alpha(&channels, NCALPHA_TRANSPARENT);
    ncchannels_set_fg_alpha(&channels, NCALPHA_TRANSPARENT);
    ncplane_set_fg_rgb(n, t->color);
    ncplane_set_base(n, "", 0, channels);
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
