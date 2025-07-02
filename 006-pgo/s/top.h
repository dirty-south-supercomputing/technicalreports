static int mons_left(const simulstate *s, int player){
  int alive = 0;
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    if(s->hp[player][i] > 0){
      ++alive;
    }
  }
  return alive;
}

// at least one and possibly both mons were knocked out at the end of the
// previous turn. simulate all possible replacement configurations (four in
// the worst case of a dual-ko of the first mons to fall).
static void
handle_ko(const simulstate *s, results *r){
  int hp0 = s->hp[0][s->active[0]];
  int hp1 = s->hp[1][s->active[1]];
  if(!hp0 && !hp1){
    if(mons_left(s, 0) == 0 && mons_left(s, 1) == 0){
      ++r->ties; return;
    }else if(mons_left(s, 0) == 0){
      ++r->wins[1]; return;
    }else if(mons_left(s, 1) == 0){
      ++r->wins[0]; return;
    }
  }else if(!hp0){
    if(mons_left(s, 0) == 0){
      ++r->wins[1]; return;
    }
  }else if(!hp1){
    if(mons_left(s, 1) == 0){
      ++r->wins[0]; return;
    }
  }
  std::cout << "we make holes in teeth FIXME" << std::endl;
  // FIXME recurse with new mons
}

// each turn has a top half and bottom halves.
// in the top half we determine which choice-pairs can be performed.
// in the bottom halves, we simulate a choice-pair.
static void
tophalf(const simulstate *s, results *r){
  bool m0[MOVEMAX] = {};
  bool m1[MOVEMAX] = {};
  sift_choices(s, m0, 0);
  sift_choices(s, m1, 1);
  for(int c0 = 0 ; c0 < MOVEMAX ; ++c0){
    if(m0[c0]){
      for(int c1 = 0 ; c1 < MOVEMAX ; ++c1){
        if(m1[c1]){
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
          auto cs = std::make_unique<simulstate>();
          *cs.get() = *s;
          ++cs->turn;
          if(bottomhalf(cs.get(), r, static_cast<pgo_move_e>(c0),
                          static_cast<pgo_move_e>(c1))){
            // a ko brings in a new mon, if there are any on that side. we'll
            // need recurse in the case of two remaining.
            handle_ko(cs.get(), r); // pass the modified state
          }
        }
      }
    }
  }
}

