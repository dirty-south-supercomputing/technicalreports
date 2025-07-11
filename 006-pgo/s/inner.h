// cartesian of player 1 with uncharged move of player 0
static void innerhalf(const simulstate *s, results *r, pgo_move_e c0, unsigned m1mask){
  for(unsigned c1 = MOVE_FAST ; c1 < MOVEMAX ; c1 <<= 1){
    if(m1mask & c1){ // valid move for c1; cartesian with c0
      bool c1charged = charged_move_p(static_cast<pgo_move_e>(c1));
      simulstate cs = *s;
      if(!bottomhalf(&cs, r, c0, static_cast<pgo_move_e>(c1), false, false) ||
         !handle_ko(&cs, r)){
        tophalf(&cs, r);
      }
      if(c1charged && s->shields[0]){
        cs = *s;
        --cs.shields[0];
        if(!bottomhalf(&cs, r, c0, static_cast<pgo_move_e>(c1), true, false) ||
           !handle_ko(&cs, r)){
          tophalf(&cs, r);
        }
      }
    }
  }
}

// cartesian of player 1 with a charged move of player 0
static void innercharged(const simulstate *s, results *r, pgo_move_e c0, unsigned m1mask){
  for(unsigned c1 = MOVE_FAST ; c1 < MOVEMAX ; c1 <<= 1){
    if(m1mask & c1){ // valid move for c1; cartesian with c0
      bool c1charged = charged_move_p(static_cast<pgo_move_e>(c1));
      if(s->shields[1]){
        simulstate cs = *s;
        --cs.shields[1];
        if(!bottomhalf(&cs, r, static_cast<pgo_move_e>(c0), static_cast<pgo_move_e>(c1), false, true) ||
           !handle_ko(&cs, r)){
            tophalf(&cs, r);
        }
        if(c1charged && s->shields[0]){
          cs = *s;
          --cs.shields[0];
          --cs.shields[1];
          if(!bottomhalf(&cs, r, static_cast<pgo_move_e>(c0), static_cast<pgo_move_e>(c1), true, true) ||
             !handle_ko(&cs, r)){
            tophalf(&cs, r);
          }
        }
      }
    }
  }
}
