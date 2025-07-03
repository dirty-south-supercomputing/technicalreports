// determine which of the moves can be taken this turn by this player
static inline void sift_choices(const simulstate *s, unsigned *m, int player){
  *m = 0;
  if(s->turns[player]){ // if we're in a fast move, we can only wait
    *m |= MOVE_WAIT; return;
  }
  // FIXME might want to do nothing if opp is in media fastmove
  *m |= MOVE_FAST;
  if(s->e[player][s->active[player]] >=
      -pmons[player][s->active[player]].ca1->energytrain){
    *m |= MOVE_CHARGED1;
  }
  if(pmons[player][s->active[player]].ca2){
    if(s->e[player][s->active[player]] >=
        -pmons[player][s->active[player]].ca2->energytrain){
      *m |= MOVE_CHARGED2;
    }
  }
  if(s->subtimer[player] == 0){
    // FIXME need check that they've not fainted
    *m |= MOVE_SUB1;
    *m |= MOVE_SUB2;
  }
}

