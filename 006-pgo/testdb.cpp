#include "pgotypes.cpp"

static bool
test_species(const species *s){
  // verify that all attacks are in attack table, and that there is at least
  // one fast attack and one charged attack.
  bool sawf = false;
  bool sawc = false;
  for(const attack **a = s->attacks ; *a ; ++a){
    if(fast_attack_p(*a)){
      sawf = true;
    }else if(charged_attack_p(*a)){
      sawc = true;
    }
    bool atkingtable = false;
    for(unsigned ai = 0 ; ai < ATTACKCOUNT ; ++ai){
      if(attacks[ai] == *a){
        atkingtable = true;
        break;
      }
    }
    if(!atkingtable){
    std::cerr << "missing on " << s->name << " " << (*a)->name << std::endl;
      throw std::exception();
    }
  }
  if(!sawf || !sawc){
    std::cerr << "missing on " << s->name << std::endl;
    throw std::exception();
  }
  return true;
}

static bool
test_sdex(const spokedex &sd){
  for(unsigned i = 0 ; i < sd.dcount ; ++i){
    const species *s = &sd.dex[i];
    if(!test_species(s)){
      return false;
    }
  }
  return true;
}

static bool
test_attack(const attack *a){
  if(charged_attack_p(a)){
    if(a->energytrain >= 0){
      std::cerr << "invalid 3x3 energy for charged attack " << a->name << std::endl;
      throw std::exception();
    }
  }else if(fast_attack_p(a)){
    if(a->energytrain < 0){ // transform builds zero energy
      std::cerr << "invalid 3x3 energy for fast attack " << a->name << std::endl;
      throw std::exception();
    }
  }else{
    std::cerr << "uncategorized attack " << a->name << std::endl;
    throw std::exception();
  }
  // check attack power
  if(charged_attack_p(a)){ // FIXME
    if(a->powerraid <= 0){
      std::cerr << "invalid Nx1 power for attack " << a->name << std::endl;
      throw std::exception();
    }
  }
  return true;
}

// sanity check the pgotypes db
int main(void){
  for(const auto &sd : sdexen){
    if(!test_sdex(sd)){
      exit(EXIT_FAILURE);
    }
  }
  for(unsigned i = 0 ; i < ATTACKCOUNT ; ++i){
    const attack *a = attacks[i];
    if(!test_attack(a)){
      exit(EXIT_FAILURE);
    }
  }
}
