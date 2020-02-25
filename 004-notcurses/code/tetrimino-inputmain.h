int main(void){
  if(!setlocale(LC_ALL, "")){
    return EXIT_FAILURE;
  }
  struct notcurses* nc;
  if((nc = notcurses_init(NULL, stdout)) == NULL){
    return EXIT_FAILURE;
  }
  int dimy, dimx, y, x;
  notcurses_term_dim_yx(nc, &dimy, &dimx);
  struct ncplane* minos[sizeof(tetriminos) / sizeof(*tetriminos)];
  int failed = draw_tetriminos(nc, minos, dimy, dimx) || notcurses_render(nc);
  int tidx = 0;
  ncplane_yx(minos[tidx], &y, &x);
  while((failed |= (highlight(minos, tidx) || notcurses_render(nc))) == 0){
    ncinput ni; // necessary for mouse
    char32_t key = notcurses_getc_blocking(nc, &ni);
    if(key == (char32_t)-1){
      failed = true;
    }else if(key == 'q'){
      break;
    }else if(key == ' '){ // select previous
      reduce(minos, tidx);
      tidx = (tidx + 6) % (sizeof(tetriminos) / sizeof(*tetriminos));
      ncplane_yx(minos[tidx], &y, &x);
    }else if(key == '\t'){ // select next
      reduce(minos, tidx);
      tidx = (tidx + 1) % (sizeof(tetriminos) / sizeof(*tetriminos));
      ncplane_yx(minos[tidx], &y, &x);
    }else if(key == '('){ // rotate counterclockwise
    }else if(key == ')'){ // rotate clockwise
    }else if(key == NCKEY_LEFT || key == 'h'){
      ncplane_move_yx(minos[tidx], y, --x < 0 ? x = 0 : x);
    }else if(key == NCKEY_RIGHT || key == 'l'){
      ncplane_move_yx(minos[tidx], y, (++x + ncplane_dim_x(minos[tidx]) > dimx) ? x = dimx - ncplane_dim_x(minos[tidx]) : x);
    }else if(key == NCKEY_UP || key == 'k'){
      ncplane_move_yx(minos[tidx], --y < 0 ? y = 0 : y, x);
    }else if(key == NCKEY_DOWN || key == 'j'){
      ncplane_move_yx(minos[tidx], (++y + 2 > dimy ) ? y = dimy - 2 : y, x);
    }else if(nckey_mouse_p(key)){
      failed = handle_mouse_event(nc, &ni);
    }
  }
  return (notcurses_stop(nc) || failed) ? EXIT_FAILURE : EXIT_SUCCESS;
}
