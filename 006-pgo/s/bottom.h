// if player has an ongoing fast move, decrement turns by one. if the fast
// attack concludes as a result, inflict damage and add energy. returns true
// in the case of a KO.
static bool account_fast_move(simulstate *s, int player){
  const pmon *p = &pmons[player][s->active[player]];
  if(s->turns[player]){
    if(!--s->turns[player]){
      accumulate_energy(&s->e[player][s->active[player]], p->fa->energytrain);
      int op = other_player(player);
      return inflict_damage(&s->hp[op][s->active[op]],
              calc_buffed_damage(calc_damage(&pmons[player][s->active[player]],
                                  &pmons[op][s->active[op]],
                                  pmons[player][s->active[player]].fa),
                                 s->buffleva[player], s->bufflevd[op]));
    }
  }
  return false;
}

// return -1 iff p0 wins cmp, 1 for p1, 0 for tie (coin flip: simul both paths)
static int p0_wins_cmp(const simulstate *s){
  float moda0 = pmons[0][s->active[0]].s.atk + pmons[0][s->active[0]].s.ia;
  float moda1 = pmons[1][s->active[1]].s.atk + pmons[1][s->active[1]].s.ia;
  return moda0 > moda1 ? -1 : moda1 > moda0 ? 1 : 0;
}

// run a single choice-pair, which ought be known to be viable (i.e. if we
// request a shielded move, that player ought have a shield). we ought receive
// out own simulstate in which we can scribble. corecurses back into tophalf().
// return true if anyone was ko'd.
static inline bool
bottomhalf(simulstate *s, results *r, pgo_move_e m0, pgo_move_e m1,
            bool m0shield, bool m1shield){
  ++r->nodes;
  if(sub_move_p(m0) || sub_move_p(m1)){
    return false;
  }
  int cmp = p0_wins_cmp(s);
  if(cmp < 0){
    if(charged_move_p(m0)){
      if(throw_charged_move(s, 0, m0, m1shield)){
        return true;
      }
    }
    if(charged_move_p(m1)){
      if(throw_charged_move(s, 1, m1, m0shield)){
        return true;
      }
    }
  }else{ // FIXME: if cmp == 0, simulate both paths
    if(charged_move_p(m1)){
      if(throw_charged_move(s, 1, m1, m0shield)){
        return true;
      }
    }
    if(charged_move_p(m0)){
      if(throw_charged_move(s, 0, m0, m1shield)){
        return true;
      }
    }
  }
  if(s->turns[0] == 0){
    s->turns[0] = pmons[0][s->active[0]].fa->turns;
  }
  if(s->turns[1] == 0){
    s->turns[1] = pmons[1][s->active[1]].fa->turns;
  }
  bool k0 = account_fast_move(s, 0);
  bool k1 = account_fast_move(s, 1);
  if(k0 || k1){
    return true;
  }
  return false;
}
