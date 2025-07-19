static inline int mons_left(const simulstate *s, int player){
  int alive = 0;
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    if(s->hp[player][i] > 0){
      ++alive;
    }
  }
  return alive;
}

static inline void subin(const simulstate *s, simulstate *snew, int player, int pos){
  *snew = *s;
  snew->active[player] = pos;
}

// handle the case where one and only one was knocked out. pleft is the number
// of pokémon remaining to that player [0..2]. if it is 0, the match is over.
static void handle_one_ko(const simulstate *s, results *r, int player){
  bool replaced = false;
  for(unsigned p = 0 ; p < TEAMSIZE ; ++p){
    if(s->hp[player][p]){
      simulstate snew;
      subin(s, &snew, 0, p);
      tophalf(&snew, r);
      replaced = true;
    }
  }
  if(!replaced){
    ++r->wins[other_player(player)];
  }
}

// if a player was ko'd coming into the top half, we must substitute for them,
// or the match is over. this can split up to four ways, depending on how many
// pokémon each side has left. if it returns true, someone was ko'd: the
// tophalf must not run, and we update the results table.
static bool handle_ko(const simulstate *s, results *r){
  int hp0 = s->hp[0][s->active[0]];
  int hp1 = s->hp[1][s->active[1]];
  if(hp0 && hp1){
    return false;
  }
  if(hp1){ // must replace player 0
    handle_one_ko(s, r, 0);
  }else if(hp0){ // must replace player 1
    handle_one_ko(s, r, 1);
  }else{ // must replace both
    int l0 = mons_left(s, 0);
    int l1 = mons_left(s, 1);
    if(l0 == 0 && l1 == 0){
      ++r->ties; return true;
    }else if(l0 == 0){
      ++r->wins[1]; return true;
    }else if(l1 == 0){
      ++r->wins[0]; return true;
    }
    // FIXME handle up to FO4
  }
  return true;
}
