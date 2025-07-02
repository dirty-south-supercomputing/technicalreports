// each turn has a top half and bottom halves.
// in the top half we determine which choice-pairs can be performed.
// in the bottom halves, we simulate a choice-pair.
static void
tophalf(const simulstate *s, results *r){
  unsigned m0mask, m1mask;
  sift_choices(s, &m0mask, 0);
  sift_choices(s, &m1mask, 1);
  for(unsigned c0 = 1 ; c0 < MOVEMAX ; c0 <<= 1){
    if(m0mask & c0){
      for(unsigned c1 = 1 ; c1 < MOVEMAX ; c1 <<= 1){
        if(m1mask & c1){
          // if c0 is a shielded move, it's only relevant if c1 is a charged
          // move, and vice versa
          if(shielded_move_p(static_cast<pgo_move_e>(c0))){
            if(!charged_move_p(static_cast<pgo_move_e>(c1))){
              continue;
            }
          }
          if(shielded_move_p(static_cast<pgo_move_e>(c1))){
            if(!charged_move_p(static_cast<pgo_move_e>(c0))){
              continue;
            }
          }
          simulstate cs = *s;
          ++cs.turn;
          if(bottomhalf(&cs, r, static_cast<pgo_move_e>(c0),
                          static_cast<pgo_move_e>(c1))){
            // a ko brings in a new mon, if there are any on that side. we'll
            // need recurse in the case of two remaining.
            handle_ko(&cs, r); // pass the modified state
          }
        }
      }
    }
  }
}

