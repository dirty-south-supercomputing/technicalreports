#include "pgotypes.h"

static bool
test_species(const species *s){
  // verify that all attacks are in attack table, and that there is at least
  // one fast attack and one charged attack.
  bool sawf = false;
  bool sawc = false;
  for(const auto &a : s->attacks){
    if(fast_attack_p(a)){
      if(sawc){
        std::cerr << "fast attack " << a->name << " followed charged attack in " << s->name << std::endl;
        throw std::exception();
      }
      sawf = true;
    }else if(charged_attack_p(a)){
      sawc = true;
    }
    bool atkingtable = false;
    for(unsigned ai = 0 ; ai < ATTACKCOUNT ; ++ai){
      if(attacks[ai] == a){
        atkingtable = true;
        break;
      }
    }
    if(!atkingtable){
      std::cerr << "table missing " << s->name << " from " << a->name << std::endl;
      throw std::exception();
    }
  }
  if(!sawf || !sawc){
    std::cerr << "missing fast/charged on " << s->name << std::endl;
    throw std::exception();
  }
  for(const auto &e : s->elite){
    bool found = false;
    for(const auto &a : s->attacks){
      if(a == e){
        found = true;
        break;
      }
    }
    if(!found){
      std::cerr << "elite attack " << e->name << " missing on " << s->name << std::endl;
      throw std::exception();
    }
  }
  // we claim in the typing chapter that ghost/rock is unpopulated when
  // discussing typings with triple resistances
  if(s->t1 == TYPE_GHOST || s->t2 == TYPE_GHOST){
    if(s->t1 == TYPE_ROCK || s->t2 == TYPE_ROCK){
      std::cerr << "ghost/rock became populated!" << std::endl;
      throw std::exception();
    }
  }
  switch(s->category){
    case species::CAT_NORMAL:
      break;
    case species::CAT_BABY:
      if(s->from){
        std::cerr << "baby pokémon " << s->name << " shouldn't have precursors" << std::endl;
        throw std::exception();
      }
      break;
    case species::CAT_LEGENDARY:
    case species::CAT_MYTHICAL:
    case species::CAT_ULTRABEAST:
      if(s->a2cost != 100){ // all ought be in cost group 4
        std::cerr << s->name << " has invalid costgroup" << std::endl;
        throw std::exception();
      }
      break;
    default:
      break;
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
    if(a->chance_opp_attack){
      if(!a->opp_attack){
        std::cerr << "chance opp attack without opp attack buff " << a->name << std::endl;
        throw std::exception();
      }
    }else if(a->opp_attack){
      std::cerr << "opp attack buff without chance " << a->name << std::endl;
      throw std::exception();
    }
    if(a->chance_opp_defense){
      if(!a->opp_defense){
        std::cerr << "chance opp defense without opp defense buff " << a->name << std::endl;
        throw std::exception();
      }
    }else if(a->opp_defense){
      std::cerr << "opp defense buff without chance " << a->name << std::endl;
      throw std::exception();
    }
    if(a->chance_user_attack){
      if(!a->user_attack){
        std::cerr << "chance user attack without user attack buff " << a->name << std::endl;
        throw std::exception();
      }
    }else if(a->user_attack){
      std::cerr << "user attack buff without chance " << a->name << std::endl;
      throw std::exception();
    }
    if(a->chance_user_defense){
      if(!a->user_defense){
        std::cerr << "chance user defense without user defense buff " << a->name << std::endl;
        throw std::exception();
      }
    }else if(a->user_defense){
      std::cerr << "user defense buff without chance " << a->name << std::endl;
      throw std::exception();
    }
    if(a->chance_opp_attack > 1000 || a->chance_opp_defense > 1000
        || a->chance_user_attack > 1000 || a->chance_user_defense > 1000){
      std::cerr << "invalid chance of buff " << a->name << std::endl;
      throw std::exception();
    }
    // we claim in attacks.tex that no attack buffs the opponent; if this
    // changes, we need remove that claim.
    if(a->opp_defense > 0 || a->opp_attack > 0){
      std::cerr << a->name << " buffs opponent" << std::endl;
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
  if(a->powerraid < 0){
    std::cerr << "invalid Nx1 power for attack " << a->name << std::endl;
    throw std::exception();
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
