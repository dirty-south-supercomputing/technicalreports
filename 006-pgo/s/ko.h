static inline int mons_left(const simulstate *s, int player){
  int alive = 0;
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    if(s->hp[player][i] > 0){
      ++alive;
    }
  }
  return alive;
}

static inline void subin(simulstate *s, int player, int pos){
  s->active[player] = pos;
  s->buffleva[player] = 0;
  s->bufflevd[player] = 0;
  calculate_damages(s);
}

// handle the case where one and only one player was knocked out.
static void handle_one_ko(simulstate *s, results *r, int player){
  bool replaced = false;
  for(unsigned p = 0 ; p < TEAMSIZE ; ++p){
    if(s->hp[player][p]){
      subin(s, 0, p);
      tophalf(s, r);
      replaced = true;
    }
  }
  if(!replaced){
    ++r->wins[other_player(player)];
  }
}

// both players just got knocked out, but both have another pokémon.
static void handle_dual_kos(simulstate *s, results *r){
  s->buffleva[0] = s->buffleva[1] = 0;
  s->bufflevd[0] = s->buffleva[1] = 0;
  for(unsigned p = 0 ; p < TEAMSIZE ; ++p){
    if(s->hp[0][p]){
      for(unsigned q = 0 ; q < TEAMSIZE ; ++q){
        if(s->hp[1][q]){
          simulstate snew = *s;
          snew.active[0] = p;
          snew.active[1] = q;
          calculate_damages(&snew);
          tophalf(&snew, r);
        }
      }
    }
  }
}

// if a player was ko'd coming into the top half, we must substitute for them,
// or the match is over. this can split up to four ways, depending on how many
// pokémon each side has left. if it returns true, someone was ko'd: the
// tophalf must not run, and we update the results table.
static bool handle_ko(simulstate *s, results *r){
  int hp0 = s->hp[0][s->active[0]];
  int hp1 = s->hp[1][s->active[1]];
  if(hp0 && hp1){
    return false;
  }
  if(hp1){ // must replace player 0
    handle_one_ko(s, r, 0);
  }else if(hp0){ // must replace player 1
    handle_one_ko(s, r, 1);
  }else{ // must replace both, if we can
    int l0 = mons_left(s, 0);
    int l1 = mons_left(s, 1);
    if(l0 == 0 && l1 == 0){
      ++r->ties;
    }else if(l0 == 0){
      ++r->wins[1];
    }else if(l1 == 0){
      ++r->wins[0];
    }else{
      handle_dual_kos(s, r);
    }
  }
  return true;
}
