struct tetmarsh {
  struct ncplane* minos[TETRIMINO_COUNT];
  struct ncplane* coaster;
  pthread_mutex_t lock;
  struct notcurses* nc;
};
