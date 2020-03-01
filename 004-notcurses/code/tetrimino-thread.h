// wakes up every .1s and rotates appropriate pieces pi/2. the selected piece
// is not rotated. fastest rate is 4Hz, aka 0.0625s per step (of 16 steps). in
// addition, the whole gang is rotated through their circle (all at the same
// speed), making a full revolution in 36 turns.
static void* rotator_thread(void* vmarsh){
  struct tetmarsh* marsh = vmarsh;
  const unsigned MAXSTEPS = 16; // 1 / ((2pi / pi/2) * 4)
  int steps[TETRIMINO_COUNT]; // steps per second
  for(int i = 0 ; i < sizeof(steps) / sizeof(*steps) ; ++i){
    steps[i] = random() % MAXSTEPS + 1;
  }
  struct timespec stime;
  clock_gettime(CLOCK_MONOTONIC, &stime);
  const uint64_t NSTOS = 1000000000ull;
  uint32_t iterations = 0;
  while(++iterations, true){
    int dimy, dimx;
    struct ncplane* stdn = notcurses_stddim_yx(marsh->nc, &dimy, &dimx); // handles resize
    const float aspect = dimx / (float)dimy;
    int centy = dimy / 2;
    int centx = dimx / 2;
    const size_t piececount = TETRIMINO_COUNT;
    const float sector = M_PI * 2 / piececount;
    const uint64_t dlinens = stime.tv_sec * NSTOS + stime.tv_nsec + iterations * (NSTOS / MAXSTEPS);
    struct timespec dline = { .tv_sec = dlinens / NSTOS, .tv_nsec = dlinens % NSTOS, };
    while(clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &dline, NULL) < 0){
      if(errno != EINTR){
        return NULL;
      }
    }
    const int radius = dimy < dimx ? dimy / 3 : dimx / 3;
    for(int i = 0 ; i < TETRIMINO_COUNT ; ++i){
      const bool lift = strspn(tetriminos[i].texture, " ") == strlen(tetriminos[i].texture) / 2;
      int y = centy + radius * sin(sector * i) - (ROWS_PER_GROW + lift * 2);
      // FIXME rotate those fuckers
    }
  }
}
