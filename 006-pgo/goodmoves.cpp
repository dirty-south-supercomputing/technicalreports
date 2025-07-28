#include "pgotypes.cpp"

// show fast moves according to some condition
// for now, condition is fixed: ept > 3 and ppt > 3
int main(void){
  constexpr int eptfloor = 4;
  constexpr int pptfloor = 4;
  for(const attack * const *as = attacks ; *as ; ++as){
    const attack *a = *as;
    if(a->energytrain <= 0){
      continue;
    }
    if(a->energytrain / static_cast<float>(a->turns) <= eptfloor){
      printf("%s fails ept test\n", a->name);
      continue;
    }
    if(a->powertrain / static_cast<float>(a->turns) <= pptfloor){
      printf("%s fails ppt test\n", a->name);
      continue;
    }
    printf("%s\n", a->name);
  }
  return EXIT_SUCCESS;
}
