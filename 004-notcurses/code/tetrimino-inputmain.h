int main(void){
  if(!setlocale(LC_ALL, "")){
    return EXIT_FAILURE;
  }
  struct notcurses* nc;
  if((nc = notcurses_init(NULL, stdout)) == NULL){
    return EXIT_FAILURE;
  }
  const size_t pcount = sizeof(tetriminos) / sizeof(*tetriminos);
  int dimy, dimx, y, x, p = 0;
  notcurses_term_dim_yx(nc, &dimy, &dimx);
  struct ncplane* minos[pcount];
  int failed = draw_tetriminos(nc, minos, dimy, dimx) || notcurses_render(nc);
  struct ncvisual* ncv = background(nc, "media/Tetris_NES_cover_art.jpg");
  ncplane_yx(minos[p], &y, &x);
  while((failed |= (highlight(minos, p) || notcurses_render(nc))) == 0){
    ncinput ni; // necessary for mouse
    char32_t key = notcurses_getc_blocking(nc, &ni);
    if(key == (char32_t)-1){
      failed = true;
    }else if(key == 'q'){
      break;
    }else if(key == ' ' || key == '\t'){ // select previous/next
      reduce(minos, p);
      p = (p + (key == ' ' ? (pcount - 1) : 1)) % pcount;
      ncplane_yx(minos[p], &y, &x);
    }else if(key == '('){ // rotate counterclockwise
    }else if(key == ')'){ // rotate clockwise
    }else if(key == NCKEY_LEFT || key == 'h'){
      ncplane_move_yx(minos[p], y, --x < 0 ? x = 0 : x);
    }else if(key == NCKEY_RIGHT || key == 'l'){
      ncplane_move_yx(minos[p], y, x = (x + ncplane_dim_x(minos[p]) > dimx ? x : x + 1));
    }else if(key == NCKEY_UP || key == 'k'){
      ncplane_move_yx(minos[p], --y < 0 ? y = 0 : y, x);
    }else if(key == NCKEY_DOWN || key == 'j'){
      ncplane_move_yx(minos[p], ++y + 2 > dimy ? y = dimy - 2 : y, x);
    }else if(nckey_mouse_p(key)){
      failed = handle_mouse_event(nc, &ni);
    }
  }
  return (notcurses_stop(nc) || failed) ? EXIT_FAILURE : EXIT_SUCCESS;
}
