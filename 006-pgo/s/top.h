// each turn has a top half and bottom halves.
// in the top half we determine which choice-pairs can be performed.
// in the bottom halves, we simulate a choice-pair.
static void tophalf(const simulstate *s, results *r){
  unsigned m1mask;
  sift_choices(s, &m1mask, 1);
  innerhalf(s, r, MOVE_FAST, m1mask);
  if(s->turns[0]){
    return;
  }
  if(s->e[0][s->active[0]] >= -pmons[0][s->active[0]].ca1->energytrain){
    innercharged(s, r, MOVE_CHARGED1, m1mask);
  }
  if(pmons[0][s->active[0]].ca2){
    if(s->e[0][s->active[0]] >= -pmons[0][s->active[0]].ca2->energytrain){
      innercharged(s, r, MOVE_CHARGED2, m1mask);
    }
  }
  if(s->subtimer[0] == 0){
    innerhalf(s, r, MOVE_SUB1, m1mask);
    innerhalf(s, r, MOVE_SUB2, m1mask);
  }
}
