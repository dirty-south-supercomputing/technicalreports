#include <math.h>
#include <locale.h>
#include <unistd.h>
#include <pthread.h>
#include <libavcodec/avcodec.h>
#include <notcurses/notcurses.h>

static const int ROWS_PER_GROW = 2;
static const int COLS_PER_GCOL = 4;

#include "tetrimino-databox.h"
#include "tetrimino-displayutf8.h"
#include "tetrimino-mouse.h"
#include "tetrimino-thread.h"
#include "tetrimino-drawcircle.h"
#include "tetrimino-switch.h"
#include "tetrimino-background.h"
#include "tetrimino-box.h"
#include "tetrimino-inputmain.h"
