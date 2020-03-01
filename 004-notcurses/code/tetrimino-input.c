#include <math.h>
#include <locale.h>
#include <unistd.h>
#include <pthread.h>
#include <libavcodec/avcodec.h>
#include <notcurses/notcurses.h>

static const int ROWS_PER_GROW = 2;
static const int COLS_PER_GCOL = 4;

#include "tetrimino-databox.h"
static const size_t TETRIMINO_COUNT = sizeof(tetriminos) / sizeof(*tetriminos);
#include "tetrimino-displayutf8.h"
#include "tetrimino-mouse.h"
#include "tetrimino-thread.h"
#include "tetrimino-drawcircle.h"
#include "tetrimino-switchbox.h"
#include "tetrimino-box.h"
#include "tetrimino-background.h"
#include "tetrimino-inputcore.h"
#include "tetrimino-inputmain.h"
