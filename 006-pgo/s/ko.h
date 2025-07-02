static inline int mons_left(const simulstate *s, int player){
  int alive = 0;
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    if(s->hp[player][i] > 0){
      ++alive;
    }
  }
  return alive;
}

// at least one and possibly both mons were knocked out at the end of the
// previous turn. simulate all possible replacement configurations (four in
// the worst case of a dual-ko of the first mons to fall). returns true if
// the match is complete (there are insufficient replacement pokémon).
static bool
handle_ko(const simulstate *s, results *r){
  int hp0 = s->hp[0][s->active[0]];
  int hp1 = s->hp[1][s->active[1]];
  int l0 = mons_left(s, 0);
  int l1 = mons_left(s, 1);
  if(!hp0 && !hp1){
    if(l0 == 0 && l1 == 0){
      ++r->ties; return true;
    }else if(l0 == 0){
      ++r->wins[1]; return true;
    }else if(l1 == 0){
      ++r->wins[0]; return true;
    }
    for(int hi0 = 0 ; hi0 < l0 ; ++hi0){
      // FIXME switch to team 0's replacement
      for(int hi1 = 0 ; hi1 < l1 ; ++hi1){
        // FIXME switch to team 1's replacement
        tophalf(s, r);
      }
    }
  }else if(!hp0){
    if(l0 == 0){
      ++r->wins[1]; return true;
    }
    for(int hi0 = 0 ; hi0 < l0 ; ++hi0){
      // FIXME switch to team 0's replacement
      tophalf(s, r);
    }
  }else if(!hp1){
    if(l1 == 0){
      ++r->wins[0]; return true;
    }
    for(int hi1 = 0 ; hi1 < l1 ; ++hi1){
      // FIXME switch to team 1's replacement
      tophalf(s, r);
    }
  }
  return false;
}
