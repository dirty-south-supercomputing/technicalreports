static void innerhalf(const simulstate *s, results *r, pgo_move_e c0, unsigned m1mask){
  for(unsigned c1 = 1 ; c1 < MOVEMAX ; c1 <<= 1){
    if(m1mask & c1){
      // FIXME handle shield cases when appropriate
      simulstate cs = *s;
      if(bottomhalf(&cs, r, static_cast<pgo_move_e>(c0), static_cast<pgo_move_e>(c1), false, false)){
        // a ko brings in a new mon, if there are any on that side. we'll
        // need recurse in the case of two remaining.
        handle_ko(&cs, r); // pass the modified state
      }
    }
  }
}

// each turn has a top half and bottom halves.
// in the top half we determine which choice-pairs can be performed.
// in the bottom halves, we simulate a choice-pair.
static void tophalf(const simulstate *s, results *r){
  unsigned m1mask;
  sift_choices(s, &m1mask, 1);
  if(s->turns[0]){
    innerhalf(s, r, MOVE_WAIT, m1mask);
    return;
  }
  innerhalf(s, r, MOVE_FAST, m1mask);
  if(s->e[0][s->active[0]] >= -pmons[0][s->active[0]].ca1->energytrain){
    innerhalf(s, r, MOVE_CHARGED1, m1mask);
  }
  if(pmons[0][s->active[0]].ca2){
    if(s->e[0][s->active[0]] >= -pmons[0][s->active[0]].ca2->energytrain){
      innerhalf(s, r, MOVE_CHARGED2, m1mask);
    }
  }
  if(s->subtimer[0] == 0){
    innerhalf(s, r, MOVE_SUB1, m1mask);
    innerhalf(s, r, MOVE_SUB2, m1mask);
  }
}
