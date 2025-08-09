#include "pgotypes.cpp"

static void elite_fast_table(const species *dex, unsigned scount){
  for(unsigned i = 0 ; i < scount ; ++i){
    const species *s = &dex[i];
    for(const auto *a : s->attacks){
      if(a->energytrain < 0){
        continue;
      }
      for(const auto *e : s->elite){
        if(a == e){
          std::cout << "elite: " << s->name << " " << a->name << std::endl;
        }
      }
    }
  }
}

static void elite_charged_table(const species *dex, unsigned scount){
  for(unsigned i = 0 ; i < scount ; ++i){
    const species *s = &dex[i];
    for(const auto *a : s->attacks){
      if(a->energytrain >= 0){
        continue;
      }
      for(const auto *e : s->elite){
        if(a == e){
          std::cout << "elite: " << s->name << " " << a->name << std::endl;
        }
      }
    }
  }
}

int main(void){
  elite_fast_table(sdex, SPECIESCOUNT);
  elite_charged_table(sdex, SPECIESCOUNT);
  return EXIT_SUCCESS;
}
