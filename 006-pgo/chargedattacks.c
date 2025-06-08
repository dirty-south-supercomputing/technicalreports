#include "pgotypes.c"
#include <memory>
#include <cstring>
#include <cstdio>
#include <cstdlib>

int cmpchargedatk(const void* va1, const void* va2){
  auto a1 = static_cast<const attack*>(va1);
  auto a2 = static_cast<const attack*>(va2);
  return a1->energytrain < a2->energytrain ? -1 : a1->energytrain == a2->energytrain ? 0 : 1;
}

// emit table of fast attacks by duration x energy x power
int main(void){
  const size_t acount = sizeof(attacks) / sizeof(*attacks);
  auto charged = std::make_unique<attack[]>(acount);
  unsigned ccount = 0;
  for(unsigned i = 0 ; i < acount ; ++i){
    const attack* a = attacks[i];
    if(a->energytrain < 0){
      memcpy(&charged[ccount], a, sizeof(*a));
      ++ccount;
    }
  }
  qsort(charged.get(), ccount, sizeof(*charged.get()), cmpchargedatk);
  for(unsigned c = 0 ; c < ccount ; ++c){
    const attack* a = &charged[c];
    printf("%s %s %u %d %u\n", a->name, TNames[a->type], a->powertrain, a->energytrain, a->turns);
  }
  return EXIT_SUCCESS;
}
