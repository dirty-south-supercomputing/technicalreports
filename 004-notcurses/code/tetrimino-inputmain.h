int main(void){
  if(!setlocale(LC_ALL, "")){
    return EXIT_FAILURE;
  }
  struct notcurses* nc;
  if((nc = notcurses_init(NULL, stdout)) == NULL){
    return EXIT_FAILURE;
  }
  int dimy, dimx, y, x, p = 0;
  notcurses_term_dim_yx(nc, &dimy, &dimx);
  struct ncplane* minos[TETRIMINO_COUNT], *coaster;
  if(!(coaster = makebox(nc)) || draw_tetriminos(nc, minos, dimy, dimx)){
    notcurses_stop(nc);
    return EXIT_FAILURE;
  }
  ncplane_yx(minos[p], &y, &x);
  int failed = notcurses_render(nc);
  struct ncvisual* ncv = background(nc, "media/Tetris_NES_cover_art.jpg");
  pthread_t tid;
void* marsh = NULL; // FIXME
  if(!failed && (failed = pthread_create(&tid, NULL, rotator_thread, marsh)) == 0){
    while((failed |= (highlight_enbox(minos, p, coaster) || notcurses_render(nc))) == 0){
      failed |= handle_input(nc, minos, dimy, dimx, &y, &x, &p);
      if(p < 0){
        break;
      }
    }
    void* result;
    failed |= (pthread_cancel(tid) || pthread_join(tid, &result) || result != PTHREAD_CANCELED);
  }
  return (notcurses_stop(nc) || failed) ? EXIT_FAILURE : EXIT_SUCCESS;
}
