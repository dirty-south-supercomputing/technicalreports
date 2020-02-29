static int blast(struct ncplane* n, uint32_t attrword, uint64_t ul,
                 uint64_t ur, uint64_t ll, uint64_t lr){
  int dimy, dimx;
  ncplane_dim_yx(n, &dimy, &dimx);
  return ncplane_cursor_move_yx(n, 0, 0) ||
         ncplane_format(n, dimy - 1, dimx - 1, attrword) ||
         ncplane_stain(n, dimy - 1, dimx - 1, ul, ur, ll, lr);
}

static int reduce(struct ncplane** minos, int tidx){
  struct ncplane* n = minos[tidx];
  int r, g, b;
  uint64_t c = 0;
  channels_set_fg(&c, tetriminos[tidx].color);
  return blast(minos[tidx], 0, c, c, c, c);
}
