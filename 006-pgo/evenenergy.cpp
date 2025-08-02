#include "pgotypes.cpp"
#include <unordered_set>

using amap = std::unordered_set<const attack*>;

// for the fast attack a, list all charged attacks it evenly divides
// *which are paired with it for some pokémon from dex*
static int
even_attacks_sdex(const attack *a, const spokedex &dex, amap &am){
  for(unsigned i = 0 ; i < dex.dcount ; ++i){
    const auto s = dex.dex[i];
    // check to see if the species has this fast attack
    bool haveattack = false;
    for(const auto &f : s.attacks){
      if(f->name == a->name){
        haveattack = true;
        break;
      }
    }
    if(!haveattack){
      continue;
    }
    for(const auto &c : s.attacks){
      if(c->energytrain >= 0){ // only consider charged attacks
        continue;
      }
      if(-c->energytrain % a->energytrain == 0){
        am.emplace(c);
      }
    }
  }
  return 0;
}

// for the fast attack a, list all charged attacks it evenly divides
// *which are paired with it for some pokémon*
static int
even_attacks(const attack *a){
  amap charged;
  for(const auto &s : sdexen){
    even_attacks_sdex(a, s, charged);
  }
  for(const auto ap : charged){
    std::cout << a->name << " " << ap->name << " " << -ap->energytrain / a->energytrain << std::endl;
  }
  return 0;
}

// generate list of charged attacks that are evenly divided by fast attacks
int main(void){
  for(unsigned i = 0 ; i < ATTACKCOUNT ; ++i){
    const auto a = attacks[i];
    if(a->energytrain > 0){
      even_attacks(a);
    }
  }
  return EXIT_SUCCESS;
}
