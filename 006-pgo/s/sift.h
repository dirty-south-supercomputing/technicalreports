static inline bool can_charged(const simulstate *s, int p, const attack *c){
  if(s->turns[p]){
    return false; // if we're in a fast move, we can only wait
  }
  return s->e[p][s->active[p]] >= -c->energytrain;
}

static inline bool can_charged1(const simulstate *s, int p){
  return can_charged(s, p, activemon(s, p)->ca1);
}

static inline bool can_charged2(const simulstate *s, int p){
  if(!pmons[p][s->active[p]].ca2){
    return false;
  }
  return can_charged(s, p, activemon(s, p)->ca2);
}

// can the specified player sub in the pokémon offset mod 3 down?
static inline bool can_sub(const simulstate *s, int p, int offset){
  if(s->turns[p] || s->subtimer[p]){
    return false; // need wait out fast move / timer
  }
  return !!s->hp[p][(s->active[p] + offset) % 3];
}

static inline bool can_sub1(const simulstate *s, int player){
  return can_sub(s, player, 1);
}

static inline bool can_sub2(const simulstate *s, int player){
  return can_sub(s, player, 2);
}
