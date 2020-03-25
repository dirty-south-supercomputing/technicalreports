// "North-facing" tetrimino forms (the form in which they are released from the top) are expressed in terms of
// two rows having between two and four columns. We map each game column to four columns and each game column
// to two rows. strlen(texture) / COLS_PER_GCOL -> columns per (g)row for the tetrimino.
static const int ROWS_PER_GROW = 2;
static const int COLS_PER_GCOL = 4;

static const struct tetrimino {
  unsigned color;
  const char* texture;
} tetriminos[] = {
  { 0xfecb00, "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" },
  { 0x009fda, "                                IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII",
},{ 0x952d98, "    TTTT        TTTT    TTTTTTTTTTTTTTTTTTTTTTTT",
},{ 0xff7900, "        LLLL        LLLLLLLLLLLLLLLLLLLLLLLLLLLL"
},{ 0x0065bd, "JJJJ        JJJJ        JJJJJJJJJJJJJJJJJJJJJJJJ"
},{ 0x69be28, "    SSSSSSSS    SSSSSSSSSSSSSSSS    SSSSSSSS    "
},{ 0xed2939, "ZZZZZZZZ    ZZZZZZZZ        ZZZZZZZZ    ZZZZZZZZ" } };

static const size_t TETRIMINO_COUNT = sizeof(tetriminos) / sizeof(*tetriminos);
