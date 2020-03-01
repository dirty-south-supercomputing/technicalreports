struct tetmarsh {
  pthread_mutex_t lock;
  struct ncplane* minos, *coaster;
};

// wakes up every .1s and rotates appropriate pieces pi/2. the selected piece
// is not rotated. fastest rate is 4Hz, aka 0.0625s per step (of 16 steps). in
// addition, the whole gang is rotated through their circle (at one speed).
static void* rotator_thread(void* vmarsh){
  struct tetmarsh* marsh = vmarsh;
  const unsigned MAXSTEPS = 16; // 1 / ((2pi / pi/2) * 4)
  int steps[TETRIMINO_COUNT]; // steps per second
  for(int i = 0 ; i < sizeof(steps) / sizeof(*steps) ; ++i){
    steps[i] = random() % MAXSTEPS + 1;
  }
  struct timespec stime;
  clock_gettime(CLOCK_MONOTONIC_RAW, &stime);
  const uint64_t NSTOS = 1000000000ull;
  uint32_t iterations = 0;
  while(true){
    const uint64_t dlinens = stime.tv_sec * NSTOS + stime.tv_nsec + iterations * (NSTOS / MAXSTEPS);
    struct timespec dline = { .tv_sec = dlinens / NSTOS, .tv_nsec = dlinens % NSTOS, };
    while(clock_nanosleep(CLOCK_MONOTONIC_RAW, TIMER_ABSTIME, &dline, NULL) < 0){
      if(errno != EINTR){
        return NULL;
      }
    }
    // FIXME rotate those fuckers
  }
}
