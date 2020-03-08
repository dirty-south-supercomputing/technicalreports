static void unlock_mutex(void* vlock){ pthread_mutex_unlock(vlock); }

// do a 1/2Hz rotation through the circle
static void* rotator_thread(void* vmarsh){
  struct tetmarsh* marsh = vmarsh;
  const float sector = M_PI * 2 / TETRIMINO_COUNT;
  const float ssector = M_PI * 2 / 36; // 10deg movements, so 18 sec/s
  const uint64_t NSTOS = 1000000000ull / 18;
  struct timespec stime;
  clock_gettime(CLOCK_MONOTONIC, &stime);
  uint32_t iterations = 0;
  while(++iterations, true){
    int dimy, dimx;
    struct ncplane* stdn = notcurses_stddim_yx(marsh->nc, &dimy, &dimx); // handles resize
    const float aspect = dimx / (float)dimy;
    int centy = dimy / 2;
    int centx = dimx / 2;
    struct timespec dline = { .tv_sec = 0, .tv_nsec = NSTOS, }; // FIXME abstime with catchup
    while(clock_nanosleep(CLOCK_MONOTONIC, 0, &dline, NULL) < 0){
      if(errno != EINTR){
        return NULL;
      }
    }
    pthread_cleanup_push(unlock_mutex, &marsh->lock);
    pthread_mutex_lock(&marsh->lock);
    const int radius = dimy < dimx ? dimy / 3 : dimx / 3;
    for(int i = 0 ; i < TETRIMINO_COUNT ; ++i){
      const bool lift = strspn(tetriminos[i].texture, " ") == strlen(tetriminos[i].texture) / 2;
      int y = centy + radius * sin(sector * i + ssector * iterations) - (ROWS_PER_GROW + lift * 2);
      int x = centx + aspect * radius * cos(sector * i + ssector * iterations) - strlen(tetriminos[i].texture) / (2 * COLS_PER_GCOL);
      if(ncplane_move_yx(marsh->minos[i], y, x)){
        pthread_mutex_unlock(&marsh->lock);
        return NULL;
      }
    }
    int ny, nx;
    ncplane_yx(marsh->minos[marsh->p], &ny, &nx);
    ncplane_move_yx(marsh->coaster, ny - 2, nx - 2 * COLS_PER_GCOL);
    notcurses_render(marsh->nc);
    pthread_cleanup_pop(1);
  }
}
