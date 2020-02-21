static int highlight(struct notcurses* nc, struct ncplane** minos, int tidx){
  struct ncplane* n = minos[tidx];
  int dimy, dimx, r, g, b;
  ncplane_dim_yx(n, &dimy, &dimx);
  uint64_t corig = 0, c = 0;
  r = channel_r(tetriminos[tidx].color);
  g = channel_g(tetriminos[tidx].color);
  b = channel_b(tetriminos[tidx].color);
  channels_set_fg(&corig, tetriminos[tidx].color);
  channels_set_fg_rgb_clipped(&c, r + 64, g + 64, b + 64);
  return ncplane_cursor_move_yx(n, 0, 0) ||
         ncplane_format(n, dimy - 1, dimx - 1, NCSTYLE_BOLD) ||
         ncplane_stain(n, dimy - 1, dimx - 1, corig, c, c, corig);
}

static int reduce(struct notcurses* nc, struct ncplane** minos, int tidx){
  struct ncplane* n = minos[tidx];
  int dimy, dimx, r, g, b;
  ncplane_dim_yx(n, &dimy, &dimx);
  uint64_t c = 0;
  channels_set_fg(&c, tetriminos[tidx].color);
  return ncplane_cursor_move_yx(n, 0, 0) ||
         ncplane_format(n, dimy - 1, dimx - 1, 0) ||
         ncplane_stain(n, dimy - 1, dimx - 1, c, c, c, c);
}

int main(void){
  if(!setlocale(LC_ALL, "")){
    return EXIT_FAILURE;
  }
  struct notcurses* nc;
  if((nc = notcurses_init(NULL, stdout)) == NULL){
    return EXIT_FAILURE;
  }
  int dimy, dimx;
  notcurses_term_dim_yx(nc, &dimy, &dimx);
  struct ncplane* minos[sizeof(tetriminos) / sizeof(*tetriminos)]; // 2-3-2 
  int failed = draw_tetriminos(nc, minos, dimy, dimx) || notcurses_render(nc);
  int tidx = 0;
  while((failed |= (highlight(nc, minos, tidx) || notcurses_render(nc))) == 0){
    ncinput ni; // necessary for mouse
    char32_t key = notcurses_getc_blocking(nc, &ni);
    if(key == (char32_t)-1){
      failed = true;
    }else if(key == 'q'){
      break;
    }else if(key == ' '){ // select previous
      reduce(nc, minos, tidx);
      tidx = (tidx + 6) % (sizeof(tetriminos) / sizeof(*tetriminos));
    }else if(key == '\t'){ // select next
      reduce(nc, minos, tidx);
      tidx = (tidx + 1) % (sizeof(tetriminos) / sizeof(*tetriminos));
    }else if(key == '('){ // rotate counterclockwise
    }else if(key == ')'){ // rotate clockwise
    }else if(key == NCKEY_LEFT || key == 'h'){
    }else if(key == NCKEY_RIGHT || key == 'l'){
    }else if(key == NCKEY_UP || key == 'k'){
    }else if(key == NCKEY_DOWN || key == 'j'){
    }else if(nckey_mouse_p(key)){
      failed = handle_mouse_event(nc, &ni);
    }
  }
  return (notcurses_stop(nc) || failed) ? EXIT_FAILURE : EXIT_SUCCESS;
}
