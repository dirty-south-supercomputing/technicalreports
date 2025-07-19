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

// both active contestants just got knocked out
static void handle_dual_kos(simulstate *s, results *r){
  s->buffleva[0] = s->buffleva[1] = 0;
  s->bufflevd[0] = s->buffleva[1] = 0;
  bool foundq = false;
  bool foundp = false;
  for(unsigned p = 0 ; p < TEAMSIZE ; ++p){
    if(s->hp[0][p]){
      foundp = true;
      for(unsigned q = 0 ; q < TEAMSIZE ; ++q){
        if(s->hp[1][q]){
          foundq = true;
          simulstate snew = *s;
          snew.active[0] = p;
          snew.active[1] = q;
          calculate_damages(&snew);
          tophalf(&snew, r);
        }
      }
    }
  }
  if(foundp){ // player 0 had more
    if(!foundq){ // player 1 did not; p0 wins
      ++r->wins[0];
    } // player 1 also had more, no result
  }else{ // need check to see if p1 had more
    for(unsigned q = 0 ; q < TEAMSIZE ; ++q){
      if(s->hp[1][q]){ // p1 did, they win
        ++r->wins[1];
        return;
      }
    }
    ++r->ties; // zorn, all dead
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
    handle_dual_kos(s, r);
  }
  return true;
}
