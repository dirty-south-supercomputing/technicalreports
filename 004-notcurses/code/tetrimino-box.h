// makes an opaque box to highlight the selected piece
static struct ncplane* makebox(struct ncplane* nc){
  struct ncplane_options nopts = {
    .rows = 2 * ROWS_PER_GROW + 2, .cols = 4 * COLS_PER_GCOL + 4,
  };
  struct ncplane* ret = ncplane_create(nc, &nopts);
  if(ret){
    uint64_t tl = 0, br = 0, m = 0;
    ncchannels_set_fg_rgb(&tl, 0xffffff); ncchannels_set_fg_rgb(&br, 0xffffff); ncchannels_set_fg_rgb(&m, 0xffffff);
    ncchannels_set_bg_rgb(&tl, 0x000000); ncchannels_set_bg_rgb(&br, 0x000000); ncchannels_set_bg_rgb(&m, 0x808080);
    if(ncplane_gradient(ret, 0, 0, 2 * ROWS_PER_GROW + 2, 4 * COLS_PER_GCOL + 4, " ", 0, tl, m, m, br) >= 0){
      #define CTI(cname) nccell cname = NCCELL_TRIVIAL_INITIALIZER
      CTI(cul); CTI(cur); CTI(cbl); CTI(cbr); CTI(chl); CTI(cvl);
      #undef CTI
      if(nccells_double_box(ret, 0, br, &cul, &cur, &cbl, &cbr, &chl, &cvl) == 0){
        int r = ncplane_perimeter(ret, &cul, &cur, &cbl, &cbr, &chl, &cvl, 0);
        if(r == 0){
          ncplane_cursor_move_yx(ret, 0, 0);
          return ret;
        }
        nccell_release(ret, &cul); nccell_release(ret, &cur); nccell_release(ret, &chl);
        nccell_release(ret, &cbl); nccell_release(ret, &cbr); nccell_release(ret, &cvl);
      }
    }
    ncplane_destroy(ret);
  }
  return NULL;
}

static int highlight_enbox(struct ncplane** minos, int tidx, struct ncplane* box){
  int r, g, b, ny, nx;
  uint64_t corig = 0, c = 0;
  r = ncchannel_r(tetriminos[tidx].color);
  g = ncchannel_g(tetriminos[tidx].color);
  b = ncchannel_b(tetriminos[tidx].color);
  ncchannels_set_fg_rgb8_clipped(&c, g + 128, b + 128, r + 128);
  ncplane_yx(minos[tidx], &ny, &nx);
  ncplane_move_yx(box, ny - 2, nx - 2 * COLS_PER_GCOL);
  return blast(minos[tidx], NCSTYLE_BOLD, c, c, c, c);
}
