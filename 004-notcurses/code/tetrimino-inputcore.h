static bool handle_input(struct notcurses* nc, struct ncplane** ps,
                         int dimy, int dimx, int* y, int* x, int* p){
  ncinput ni; // necessary for mouse
  char32_t key = notcurses_getc_blocking(nc, &ni);
  if(key == (char32_t)-1){
    return true;
  }else if(key == 'q'){
    *p = -1;
  }else if(key == ' ' || key == '\t'){ // select previous/next
    reduce(ps, *p);
    *p = (*p + (key == ' ' ? (TETRIMINO_COUNT - 1) : 1)) % TETRIMINO_COUNT;
    ncplane_yx(ps[*p], y, x);
  }else if(key == '('){ // rotate counterclockwise
  }else if(key == ')'){ // rotate clockwise
  }else if(key == NCKEY_LEFT || key == 'h'){
    ncplane_move_yx(ps[*p], *y, --*x < 0 ? *x = 0 : *x);
  }else if(key == NCKEY_RIGHT || key == 'l'){
    ncplane_move_yx(ps[*p], *y, *x = (*x + ncplane_dim_x(ps[*p]) > dimx ? *x : *x + 1));
  }else if(key == NCKEY_UP || key == 'k'){
    ncplane_move_yx(ps[*p], --*y < 0 ? *y = 0 : *y, *x);
  }else if(key == NCKEY_DOWN || key == 'j'){
    ncplane_move_yx(ps[*p], ++*y + 2 > dimy ? *y = dimy - 2 : *y, *x);
  }else if(nckey_mouse_p(key)){
    return handle_mouse_event(nc, &ni);
  }
  return false;
}
