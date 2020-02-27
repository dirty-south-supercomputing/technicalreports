// makes an opaque box to highlight the selected piece
static struct ncplane* makebox(struct notcurses* nc){
  struct ncplane* ret = ncplane_new(nc, 2 * ROWS_PER_GROW + 2, 4 * COLS_PER_GCOL + 4, 0, 0, NULL);
  if(ret){
    uint64_t channels = 0;
    channels_set_bg_alpha(&channels, CELL_ALPHA_BLEND);
    channels_set_fg_alpha(&channels, CELL_ALPHA_BLEND);
    if(ncplane_set_base(ret, channels, 0, "") == 0){
      return ret;
    }
    ncplane_destroy(ret);
  }
  return NULL;
}

// move the highlight box to the specified plane
static int boxit(struct ncplane* box, const struct ncplane* n){
}
