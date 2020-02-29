// makes an opaque box to highlight the selected piece
static struct ncplane*
makebox(struct notcurses* nc){
  struct ncplane* ret = ncplane_new(nc, 2 * ROWS_PER_GROW + 2, 4 * COLS_PER_GCOL + 4, 0, 0, NULL);
  if(ret){
    uint64_t tl = 0, br = 0, m = 0;
    channels_set_fg(&tl, 0xffffff); channels_set_fg(&br, 0xffffff); channels_set_fg(&m, 0xffffff);
    channels_set_bg(&tl, 0x000000); channels_set_bg(&br, 0x000000); channels_set_bg(&m, 0x808080);
    if(!ncplane_gradient_sized(ret, " ", 0, tl, m, m, br, 2 * ROWS_PER_GROW + 2, 4 * COLS_PER_GCOL + 4)){
      #define CTI(cname) cell cname = CELL_TRIVIAL_INITIALIZER
      CTI(cul); CTI(cur); CTI(cbl); CTI(cbr); CTI(chl); CTI(cvl);
      #undef CTI
      if(cells_double_box(ret, 0, br, &cul, &cur, &cbl, &cbr, &chl, &cvl) == 0){
        int r = ncplane_perimeter(ret, &cul, &cur, &cbl, &cbr, &chl, &cvl, 0);
        if(r == 0){
          ncplane_cursor_move_yx(ret, 0, 0);
          return ret;
        }
        cell_release(ret, &cul); cell_release(ret, &cur); cell_release(ret, &chl);
        cell_release(ret, &cbl); cell_release(ret, &cbr); cell_release(ret, &cvl);
      }
    }
    ncplane_destroy(ret);
  }
  return NULL;
}

static int
highlight_enbox(struct ncplane** minos, int tidx, struct ncplane* box){
  int r, g, b, ny, nx;
  uint64_t corig = 0, c = 0;
  r = channel_r(tetriminos[tidx].color);
  g = channel_g(tetriminos[tidx].color);
  b = channel_b(tetriminos[tidx].color);
  channels_set_fg(&corig, tetriminos[tidx].color);
  channels_set_fg_rgb_clipped(&c, g + 128, b + 128, r + 128);
  ncplane_yx(minos[tidx], &ny, &nx);
  ncplane_move_yx(box, ny - 2, nx - 2 * COLS_PER_GCOL);
  return blast(minos[tidx], NCSTYLE_BOLD, corig, c, c, corig);
}
