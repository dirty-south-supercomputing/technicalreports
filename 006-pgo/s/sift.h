static inline bool can_charged(const simulstate *s, int player, const attack *c){
  if(!c){
    return false;
  }
  if(s->turns[player]){
    return false; // if we're in a fast move, we can only wait
  }
  if(s->e[player][s->active[player]] < -c->energytrain){
    return false;
  }
  return true;
}

static inline bool can_charged1(const simulstate *s, int player){
  return can_charged(s, player, pmons[player][s->active[player]].ca1);
}

static inline bool can_charged2(const simulstate *s, int player){
  return can_charged(s, player, pmons[player][s->active[player]].ca2);
}

static inline bool can_sub(const simulstate *s, int player, int offset){
  if(s->turns[player]){
    return false; // if we're in a fast move, we can only wait
  }
  if(s->subtimer[player]){
    return false; // we've subbed too recently
  }
  if(s->hp[player][(s->active[player] + offset) % 3] == 0){
    return false; // desired sub is not available
  }
  return true;
}

static inline bool can_sub1(const simulstate *s, int player){
  return can_sub(s, player, 1);
}

static inline bool can_sub2(const simulstate *s, int player){
  return can_sub(s, player, 2);
}
