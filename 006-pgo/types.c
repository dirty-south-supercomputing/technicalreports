#include <stdio.h>
#include <stdlib.h>

enum {
  TYPE_BUG,
  TYPE_DARK,
  TYPE_DRAGON,
  TYPE_ELECTRIC,
  TYPE_FAIRY,
  TYPE_FIGHTING,
  TYPE_FIRE,
  TYPE_FLYING,
  TYPE_GHOST,
  TYPE_GRASS,
  TYPE_GROUND,
  TYPE_ICE,
  TYPE_NORMAL,
  TYPE_POISON,
  TYPE_PSYCHIC,
  TYPE_ROCK,
  TYPE_STEEL,
  TYPE_WATER,
  TYPECOUNT
} types_e;

static const char* tnames[TYPECOUNT] = {
  "bug",
  "dark",
  "dragon",
  "electric",
  "fairy",
  "fighting",
  "fire",
  "flying",
  "ghost",
  "grass",
  "ground",
  "ice",
  "normal",
  "poison",
  "psychic",
  "rock",
  "steel",
  "water"
};

// each row is an attacking Type
static const int trelations[TYPECOUNT][TYPECOUNT] = {
  // bug     dragon  fairy   fire    ghost   ground  normal  psychic steel
  //   dark      elec    fight   fly     grass   ice     poison  rock    water
  {  0,  1,  0,  0, -1, -1, -1, -1, -1,  1,  0,  0,  0, -1,  1,  0, -1,  0 }, // bug
  {  0, -1,  0,  0, -1, -1,  0,  0,  1,  0,  0,  0,  0,  0,  1,  0,  0,  0 }, // dark
  {  0,  0,  1,  0, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1,  0 }, // dragon
  {  0,  0, -1, -1,  0,  0,  0,  1,  0, -1, -2,  0,  0,  0,  0,  0,  0,  1 }, // electric
  {  0,  1,  1,  0,  0,  1, -1,  0,  0,  0,  0,  0,  0, -1,  0,  0, -1,  0 }, // fairy
  { -1,  1,  0,  0, -1,  0,  0, -1, -2,  0,  0,  1,  1, -1, -1,  1,  1,  0 }, // fighting
  {  1,  0, -1,  0,  0,  0, -1,  0,  0,  1,  0,  1,  0,  0,  0, -1,  1, -1 }, // fire
  {  1,  0,  0, -1,  0,  1,  0,  0,  0,  1,  0,  0,  0,  0,  0, -1, -1,  0 }, // flying
  {  0, -1,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0, -2,  0,  1,  0,  0,  0 }, // ghost
  { -1,  0, -1,  0,  0,  0, -1, -1,  0, -1,  1,  0,  0, -1,  0,  1, -1,  1 }, // grass
  { -1,  0,  0,  1,  0,  0,  1, -2,  0, -1,  0,  0,  0,  1,  0,  1,  1,  0 }, // ground
  {  0,  0,  1,  0,  0,  0, -1,  1,  0,  1,  1, -1,  0,  0,  0,  0, -1, -1 }, // ice
  {  0,  0,  0,  0,  0,  0,  0,  0, -2,  0,  0,  0,  0,  0,  0, -1, -1,  0 }, // normal
  {  0,  0,  0,  0,  1,  0,  0,  0, -1,  1, -1,  0,  0, -1,  0, -1, -2,  0 }, // poison
  {  0, -2,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  1, -1,  0, -1,  0 }, // psychic
  {  1,  0,  0,  0,  0, -1,  1,  1,  0,  0, -1,  1,  0,  0,  0,  0, -1,  0 }, // rock
  {  0,  0,  0, -1,  1,  0, -1,  0,  0,  0,  0,  1,  0,  0,  0,  1, -1, -1 }, // steel
  {  0,  0, -1,  0,  0,  0,  1,  0,  0, -1,  1,  0,  0,  0,  0,  1,  0, -1 }  // water
};

static void
print_vector_ints(const int rels[]){
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    printf("%2d ", rels[i]);
  }
}

static int
vector_match_count(const int rels[], int match){
  int c = 0;
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    if(rels[i] == match){
      ++c;
    }
  }
  return c;
}

static int
choose2(int n){
  return n * (n - 1) / 2;
}

int main(void){
  printf("Attack efficiencies\n");
  printf("          Bu Da Dr El Fa Fg Fi Fl Gh Gs Gd Ic No Po Py Ro St Wa\n");
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    printf("%8s: ", tnames[i]);
    print_vector_ints(trelations[i]);
    printf("\n");
  }

  printf("\n");
  printf("Attack efficiency summaries\n");
  printf("          -2 -1  0  1  T -4 -3 -2 -1   0  1  2  T\n");
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    printf("%8s: ", tnames[i]);
    int aneg2 = vector_match_count(trelations[i], -2);
    int aneg1 = vector_match_count(trelations[i], -1);
    int azero = vector_match_count(trelations[i], 0);
    int apos1 = vector_match_count(trelations[i], 1);
    int asum = aneg2 * -2 - aneg1 + apos1;
    printf("%2d %2d %2d %2d %2d", aneg2, aneg1, azero, apos1, asum);
    int tneg4 = choose2(aneg2);
    int tneg3 = aneg2 * aneg1;
    int tneg2 = aneg2 * (azero + 1) + choose2(aneg1);
    int tneg1 = aneg1 * (azero + 1) + aneg2 * apos1;
    int tzero = azero + choose2(azero) + apos1 * aneg1;
    int tpos1 = apos1 * (azero + 1);
    int tpos2 = choose2(apos1);
    int tsum = tneg4 * -4 + tneg3 * -3 + tneg2 * -2 - tneg1 + tpos1 + tpos2 * 2;
    printf(" %2d %2d %2d %2d %3d %2d %2d %3d", tneg4, tneg3, tneg2, tneg1, tzero, tpos1, tpos2, tsum);
    printf("\n");
  }
  return EXIT_SUCCESS;
}
