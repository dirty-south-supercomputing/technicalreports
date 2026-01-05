// handle the case where one and only one player was knocked out.
static void handle_one_ko(simulstate *s, results *r, int player){
  bool replaced = false;
  if(s->turns[other_player(player)]){ // end fast move and credit any energy
    s->turns[other_player(player)] = 0;
    accumulate_energy(&s->e[other_player(player)][s->active[other_player(player)]],
      activemon(s, other_player(player))->fa->energytrain);
  }
  for(unsigned p = 0 ; p < TEAMSIZE ; ++p){
    if(s->hp[player][p]){
      simulstate snew = *s;
      subin(&snew, player, p);
      tophalf(&snew, r);
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
  s->turns[0] = s->turns[1] = 0;
  for(unsigned p = 0 ; p < TEAMSIZE ; ++p){
    if(s->hp[0][p]){
      foundp = true;
      for(unsigned q = 0 ; q < TEAMSIZE ; ++q){
        if(s->hp[1][q]){
          foundq = true;
          simulstate snew = *s;
          subin(&snew, 0, p);
          subin(&snew, 1, q); // runs calculate_damages twice =[
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
