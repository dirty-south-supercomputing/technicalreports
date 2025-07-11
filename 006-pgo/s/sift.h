// determine which of the moves can be taken this turn by this player
static inline void sift_choices(const simulstate *s, unsigned *m, int player){
  *m = MOVE_FAST;
  if(s->turns[player] == 0){ // if we're in a fast move, we can only wait
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
      if(s->hp[player][(s->active[player] + 1) % 3]){
        *m |= MOVE_SUB1;
      }
      if(s->hp[player][(s->active[player] + 2) % 3]){
        *m |= MOVE_SUB2;
      }
    }
  }
}
