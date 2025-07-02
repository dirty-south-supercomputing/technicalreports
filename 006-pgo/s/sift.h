// determine which of the moves can be taken this turn by this player
static void
sift_choices(const simulstate *s, unsigned *m, int player){
  *m = 0;
  if(s->turns[player]){ // if we're in a fast move, we can only wait
    *m |= MOVE_WAIT;
    if(s->shields[player]){
      *m |= MOVE_WAIT_SHIELD;
    }
    return;
  }
  // FIXME might want to do nothing if opp is in media fastmove
  *m |= MOVE_FAST;
  if(s->shields[player]){
    *m |= MOVE_FAST_SHIELD;
  }
  if(s->e[player][s->active[player]] >=
      -pmons[player][s->active[player]].ca1->energytrain){
    *m |= MOVE_CHARGED1;
    if(s->shields[player]){
      *m |= MOVE_CHARGED1_SHIELD;
    }
  }
  if(pmons[player][s->active[player]].ca2){
    if(s->e[player][s->active[player]] >=
        -pmons[player][s->active[player]].ca2->energytrain){
      *m |= MOVE_CHARGED2;
      if(s->shields[player]){
        *m |= MOVE_CHARGED2_SHIELD;
      }
    }
  }
  if(s->subtimer[player] == 0){
    // FIXME need check that they've not fainted
    *m |= MOVE_SUB1;
    *m |= MOVE_SUB2;
  }
}

