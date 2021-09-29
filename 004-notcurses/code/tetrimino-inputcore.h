#include <uchar.h>

static bool handle_input(struct notcurses* nc, struct tetmarsh* marsh, int dimy, int dimx, int* y, int* x){
  ncinput ni; // necessary for mouse
  pthread_mutex_unlock(&marsh->lock);
  char32_t key = notcurses_getc_blocking(nc, &ni);
  pthread_mutex_lock(&marsh->lock);
  struct ncplane** ps = marsh->minos;
  const int p = marsh->p;
  if(key == (char32_t)-1){
    return true;
  }else if(key == 'q'){
    marsh->p = -1;
  }else if(key == ' ' || key == '\t'){ // select previous/next
    reduce(ps, p);
    marsh->p = (p + (key == ' ' ? (TETRIMINO_COUNT - 1) : 1)) % TETRIMINO_COUNT;
    ncplane_yx(ps[p], y, x);
  }else if(key == '('){ // rotate counterclockwise
    return ncplane_rotate_ccw(ps[p]);
  }else if(key == ')'){ // rotate clockwise
    return ncplane_rotate_cw(ps[p]);
  }else if(key == NCKEY_LEFT || key == 'h'){
    return ncplane_move_yx(ps[p], *y, --*x < 0 ? *x = 0 : *x);
  }else if(key == NCKEY_RIGHT || key == 'l'){
    return ncplane_move_yx(ps[p], *y, *x = (*x + ncplane_dim_x(ps[p]) > dimx ? *x : *x + 1));
  }else if(key == NCKEY_UP || key == 'k'){
    return ncplane_move_yx(ps[p], --*y < 0 ? *y = 0 : *y, *x);
  }else if(key == NCKEY_DOWN || key == 'j'){
    return ncplane_move_yx(ps[p], ++*y + 2 > dimy ? *y = dimy - 2 : *y, *x);
  }else if(nckey_mouse_p(key)){
    return handle_mouse_event(nc, &ni);
  }
  return false;
}
