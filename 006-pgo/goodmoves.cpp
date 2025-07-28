#include "pgotypes.cpp"

// show fast moves according to some condition
// for now, condition is fixed: ept > 3 and ppt > 3
int main(void){
  constexpr int eptfloor = 3;
  constexpr int pptfloor = 3;
  for(const attack * const *as = attacks ; *as ; ++as){
    const attack *a = *as;
    if(a->energytrain <= 0){
      continue;
    }
    if(a->energytrain / static_cast<float>(a->turns) < eptfloor){
      continue;
    }
    if(a->powertrain / static_cast<float>(a->turns) < pptfloor){
      continue;
    }
    printf("%s %u %u %u\n", a->name, a->powertrain, a->energytrain, a->turns);
  }
  return EXIT_SUCCESS;
}
