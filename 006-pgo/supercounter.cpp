#include "pgotypes.cpp"

// can atk hit def with some charged attack for a typing relation of at least rel?
static bool
can_hit_for(const species &atk, const species &def, int rel){
  for(const auto *ca : atk.attacks){
    if(!charged_attack_p(ca)){
      continue;
    }
    if(typing_relation(ca->type, def.t1, def.t2) >= rel){
      return true;
    }
  }
  return false;
}


// is ca1+ca2 a counter to opp?
static bool
counters_p(const attack *ca1, const attack *ca2, const species *opp){
  if(typing_relation(ca1->type, opp->t1, opp->t2) > 0){
//std::cout << " BUT " << s.name << " can hit " << opp->name << " with " << TNames[ca1->type] << " for " << typing_relation(ca1->type, opp->t1, opp->t2) << std::endl;
    return true;
  }
  if(ca2 && typing_relation(ca2->type, opp->t1, opp->t2) > 0){
//std::cout << " BUT2 " << s.name << " can hit " << opp->name << " with " << TNames[ca2->type] << " for " << typing_relation(ca2->type, opp->t1, opp->t2) << std::endl;
    return true;
  }
  return false;
}

// returns the number of species for which s is not an effective counter, <= canhit
static unsigned
supercounter_spec(const species &s, const attack *ca1, const attack *ca2,
                  unsigned *canhit){
  unsigned bad = 0;
  *canhit = 0;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const species &opp = sdex[u];
    if(!can_hit_for(opp, s, 1)){
      continue;
    }
    ++*canhit;
    if(!counters_p(ca1, ca2, &opp)){
      ++bad;
    }
  }
  // number we can't counter out of total, then number that can counter us out of total
  std::cout << bad * 100.0 / SPECIESCOUNT << "% (" << bad << "/" << SPECIESCOUNT << ") "
            << *canhit * 100.0 / SPECIESCOUNT << "% (" << *canhit << "/" << SPECIESCOUNT << ") "
            << s.name << " (" << ca1->name;
  if(ca2){
    std::cout << "+" << ca2->name;
  }
  std::cout << ")" << std::endl;
  return bad;
}

// does the species have some configuration that is a supercounter?
static bool
supercounter_p(const species &s){
  bool scounter = false;
  for(auto ca1 = s.attacks.begin() ; ca1 != s.attacks.end() ; ++ca1){
    if(!charged_attack_p(*ca1)){
      continue;
    }
    unsigned f, canhit;
    auto ca2 = ca1 + 1;
    if(ca2 == s.attacks.end()){
      if((f = supercounter_spec(s, *ca1, nullptr, &canhit)) == 0){
        scounter = true;
      }
    }else{
      do{
        if(!charged_attack_p(*ca2)){
          continue;
        }
        if((f = supercounter_spec(s, *ca1, *ca2, &canhit)) == 0){
          scounter = true;
        }
      }while(++ca2 != s.attacks.end());
    }
  }
  return scounter;
}

// generate list of who can hit for effect everyone who can hit them for effect
int main(void){
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const species &s = sdex[u];
    supercounter_p(s);
  }
  return EXIT_SUCCESS;
}
