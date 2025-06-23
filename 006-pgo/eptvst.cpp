#include "pgotypes.cpp"

// produce data file of EPT vs turns
int main(void){
  for(unsigned i = 0 ; i < ATTACKCOUNT ; ++i){
    const attack *a = attacks[i];
    if(a->energytrain < 0){
      continue;
    }
    printf("%f %d %d\n", a->energytrain / (float)a->turns, a->turns, a->type);
  }
  return EXIT_SUCCESS;
}
