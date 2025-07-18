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

// sanity check the pgotypes db
int main(void){
  for(const auto &sd : sdexen){
    if(!test_sdex(sd)){
      exit(EXIT_FAILURE);
    }
  }
}
