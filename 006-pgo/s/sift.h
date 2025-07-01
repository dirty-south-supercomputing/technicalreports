// determine which of the moves can be taken this turn by this player
static void
sift_choices(const simulstate *s, bool *m, int player){
  if(s->turns[player]){ // if we're in a fast move, we can only wait
    m[MOVE_WAIT] = true;
    if(s->shields[player]){
      m[MOVE_WAIT_SHIELD] = true;
    }
    return;
  }
  // FIXME might want to do nothing if opp is in media fastmove
  m[MOVE_FAST] = true; // we can launch a fast move
  if(s->shields[player]){
    m[MOVE_FAST_SHIELD] = true;
  }
  if(s->e[player][s->active[player]] >=
      -pmons[player][s->active[player]].ca1->energytrain){
    m[MOVE_CHARGED1] = true;
    if(s->shields[player]){
      m[MOVE_CHARGED1_SHIELD] = true;
    }
  }
  if(pmons[player][s->active[player]].ca2){
    if(s->e[player][s->active[player]] >=
        -pmons[player][s->active[player]].ca2->energytrain){
      m[MOVE_CHARGED2] = true;
      if(s->shields[player]){
        m[MOVE_CHARGED2_SHIELD] = true;
      }
    }
  }
  if(s->subtimer[player] == 0){
    // FIXME need check that they've not fainted
    m[MOVE_SUB1] = true;
    m[MOVE_SUB2] = true;
  }
}

