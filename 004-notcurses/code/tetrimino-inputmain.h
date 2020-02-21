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
  while((failed |= !!notcurses_render(nc)) == 0){
    ncinput ni; // necessary for mouse
    char32_t key = notcurses_getc_blocking(nc, &ni);
    if(key == (char32_t)-1){
      failed = true;
    }else if(key == ' '){ // select previous
    }else if(key == '\t'){ // select next
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
