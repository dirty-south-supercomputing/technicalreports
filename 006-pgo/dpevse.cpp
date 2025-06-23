#include "pgotypes.cpp"

// produce data file of DPE vs energy
int main(void){
  for(unsigned i = 0 ; i < ATTACKCOUNT ; ++i){
    const attack *a = attacks[i];
    if(a->energytrain >= 0){
      continue;
    }
    printf("%f %d %d\n", a->powertrain / (float)-a->energytrain, -a->energytrain, a->type);
  }
  return EXIT_SUCCESS;
}
