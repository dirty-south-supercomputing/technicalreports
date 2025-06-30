#include "pgotypes.cpp"
#include <iostream>

static void
mmhpvsomhp(const struct spokedex &dex){
  for(unsigned i = 0 ; i < dex.dcount ; ++i){
    const species *s = &dex.dex[i];
    stats *st = find_optimal_set(s, 1500, 0, false);
    unsigned half;
    unsigned optl = halflevel_to_level(st->hlevel, &half);
    auto omhp = calc_mhp(s->sta + st->is, optl);
    auto mhp = calc_mhp(s->sta + 15, 99);
    std::cout << mhp << ' ' << omhp << ' ' << ' ' << st->is << std::endl;
  }
}

int main(void){
  for(const auto &dex : sdexen){
    mmhpvsomhp(dex);
  }
}
