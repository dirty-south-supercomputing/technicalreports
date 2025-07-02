// if player has an ongoing fast move, decrement turns by one. if the fast
// attack concludes as a result, inflict damage and add energy. returns true
// in the case of a KO.
static bool account_fast_move(simulstate *s, int player){
  if(s->turns[player]){
    if(!--s->turns[player]){
      accumulate_energy(&s->e[player][s->active[player]],
          pmons[player][s->active[player]].fa->energytrain);
      int op = other_player(player);
      return inflict_damage(&s->hp[op][s->active[op]],
              calc_damage(s, player, pmons[player][s->active[player]].fa));
    }
  }
  return false;
}

// return true iff p0 wins cmp (false indicates p1 won it)
static bool p0_wins_cmp(const simulstate *s){
  float moda0 = pmons[0][s->active[0]].s.atk + pmons[0][s->active[0]].s.ia;
  float moda1 = pmons[1][s->active[1]].s.atk + pmons[1][s->active[1]].s.ia;
  bool cmp0 = moda0 > moda1 ? true : moda1 > moda0 ? false : rand() % 2;
  return cmp0;
}

// run a single choice-pair, which ought be known to be viable (i.e. if we
// request a shielded move, that player ought have a shield). we ought receive
// out own simulstate in which we can scribble. corecurses back into tophalf().
// return true if anyone was ko'd.
static inline bool
bottomhalf(simulstate *s, results *r, pgo_move_e m0, pgo_move_e m1){
  ++r->nodes;
  /*if(++r->nodes % 10000000 == 0){
    std::cout << "node " << r->nodes << " " << r->wins[0] << " " << r->wins[1] << " " << r->ties << std::endl;
  }*/
  if(sub_move_p(m0) || sub_move_p(m1)){
    //std::cout << "substitution is not yet handled!" << std::endl; FIXME
    return true;
  }
  bool p0cmp = p0_wins_cmp(s);
  if(p0cmp){
    if(charged_move_p(m0)){
      if(throw_charged_move(s, 0, m0, m1)){
        return true;
      }
    }
    if(charged_move_p(m1)){
      if(throw_charged_move(s, 1, m1, m0)){
        return true;
      }
    }
  }else{
    if(charged_move_p(m1)){
      if(throw_charged_move(s, 1, m1, m0)){
        return true;
      }
    }
    if(charged_move_p(m0)){
      if(throw_charged_move(s, 0, m0, m1)){
        return true;
      }
    }
  }
  if(fast_move_p(m0)){
    s->turns[0] = pmons[0][s->active[0]].fa->turns;
  }
  if(fast_move_p(m1)){
    s->turns[1] = pmons[1][s->active[1]].fa->turns;
  }
  bool k0 = account_fast_move(s, 0);
  bool k1 = account_fast_move(s, 1);
  if(k0 || k1){
    return true;
  }
  tophalf(s, r); // no one got knocked out; recurse to next turn
  return false;
}
