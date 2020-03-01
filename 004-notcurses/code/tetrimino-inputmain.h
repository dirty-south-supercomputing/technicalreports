int main(void){
  if(!setlocale(LC_ALL, "")){
    return EXIT_FAILURE;
  }
  struct notcurses* nc;
  if((nc = notcurses_init(NULL, stdout)) == NULL){
    return EXIT_FAILURE;
  }
  int dimy, dimx, y, x, p = 0;
  struct tetmarsh marsh = { .lock = PTHREAD_MUTEX_INITIALIZER, .nc = nc, };
  notcurses_stddim_yx(nc, &dimy, &dimx);
  if(!(marsh.coaster = makebox(nc)) || draw_tetriminos(nc, marsh.minos, dimy, dimx)){
    notcurses_stop(nc);
    return EXIT_FAILURE;
  }
  ncplane_yx(marsh.minos[p], &y, &x);
  int failed = notcurses_render(nc);
  struct ncvisual* ncv = background(nc, "media/Tetris_NES_cover_art.jpg");
  pthread_t tid;
  if(!failed && (failed = pthread_create(&tid, NULL, rotator_thread, &marsh)) == 0){
    pthread_mutex_lock(&marsh.lock);
    while((failed |= (highlight_enbox(marsh.minos, p, marsh.coaster) || notcurses_render(nc))) == 0){
      failed |= handle_input(nc, marsh.minos, dimy, dimx, &y, &x, &p, &marsh.lock); // emerge locked
      if(p < 0){
        break;
      }
    }
    pthread_mutex_unlock(&marsh.lock);
    void* result;
    failed |= (pthread_cancel(tid) || pthread_join(tid, &result) || result != PTHREAD_CANCELED);
  }
  return (notcurses_stop(nc) || pthread_mutex_destroy(&marsh.lock) || failed) ? EXIT_FAILURE : EXIT_SUCCESS;
}
