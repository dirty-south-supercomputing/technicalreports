// cartesian of player 1 with uncharged move of player 0
static void innerhalf(const simulstate *s, results *r, pgo_move_e c0, unsigned m1mask){
  for(unsigned c1 = 1 ; c1 < MOVEMAX ; c1 <<= 1){
    if(m1mask & c1){ // valid move for c1; cartesian with c0
      bool c1charged = charged_move_p(static_cast<pgo_move_e>(c1));
      simulstate cs = *s;
      if(bottomhalf(&cs, r, static_cast<pgo_move_e>(c0), static_cast<pgo_move_e>(c1), false, false)){
        // a ko brings in a new mon, if there are any on that side. we'll
        // need recurse in the case of two remaining.
        handle_ko(&cs, r); // pass the modified state
      }
      if(c1charged && s->shields[0]){
        cs = *s;
        --cs.shields[0];
        if(bottomhalf(&cs, r, static_cast<pgo_move_e>(c0), static_cast<pgo_move_e>(c1), true, false)){
          handle_ko(&cs, r); // pass the modified state
        }
      }
    }
  }
}

// cartesian of player 1 with a charged move of player 0
static void innercharged(const simulstate *s, results *r, pgo_move_e c0, unsigned m1mask){
  for(unsigned c1 = 1 ; c1 < MOVEMAX ; c1 <<= 1){
    if(m1mask & c1){ // valid move for c1; cartesian with c0
      bool c1charged = charged_move_p(static_cast<pgo_move_e>(c1));
      if(s->shields[1]){
        simulstate cs = *s;
        --cs.shields[1];
        if(bottomhalf(&cs, r, static_cast<pgo_move_e>(c0), static_cast<pgo_move_e>(c1), false, true)){
          handle_ko(&cs, r);
        }
        if(c1charged && s->shields[0]){
          cs = *s;
          --cs.shields[0];
          --cs.shields[1];
          if(bottomhalf(&cs, r, static_cast<pgo_move_e>(c0), static_cast<pgo_move_e>(c1), true, true)){
            handle_ko(&cs, r); // pass the modified state
          }
        }
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
    innercharged(s, r, MOVE_CHARGED1, m1mask);
  }
  if(pmons[0][s->active[0]].ca2){
    if(s->e[0][s->active[0]] >= -pmons[0][s->active[0]].ca2->energytrain){
      innercharged(s, r, MOVE_CHARGED2, m1mask);
    }
  }
  /*if(s->subtimer[0] == 0){
    innerhalf(s, r, MOVE_SUB1, m1mask);
    innerhalf(s, r, MOVE_SUB2, m1mask);
  }*/
}
