// "North-facing" tetrimino forms (the form in which they are released from the
// top) are expressed in terms of two rows having between two and four columns.
// We map each game column to four columns and each game column to two rows.
// Each byte of the texture maps to one 4x4 component block.
static const int ROWS_PER_GROW = 2;
static const int COLS_PER_GCOL = 4;

static const struct tetrimino {
  unsigned color;
  const char* texture;
} tetriminos[] = { // OITLJSZ
  { 0xcbc900, "****"}, { 0x009caa, "    ****"}, { 0x952d98, " * ***"},
  { 0xcf7900, "  ****"}, { 0x0065bd, "*  ***"}, { 0x69be28, " **** "},
  { 0xbd2939, "**  **"} };
