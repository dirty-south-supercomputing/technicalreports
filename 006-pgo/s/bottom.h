// decrement fast move turns by one. if the attack concludes as a result,
// inflict damage and add energy. returns true in the case of a KO.
static bool account_fast_move(simulstate *s, int player){
  if(s->turns[player] == 0){
    return false;
  }
  if(--s->turns[player]){
    return false;
  }
  const pmon *p = &pmons[player][s->active[player]];
  accumulate_energy(&s->e[player][s->active[player]], p->fa->energytrain);
  int op = other_player(player);
  return inflict_damage(&s->hp[op][s->active[op]],
          calc_buffed_damage(calc_damage(&pmons[player][s->active[player]],
                              &pmons[op][s->active[op]],
                              pmons[player][s->active[player]].fa),
                              s->buffleva[player], s->bufflevd[op]));
}

// return -1 iff p0 wins cmp, 1 for p1, 0 for tie (coin flip: simul both paths)
static int p0_wins_cmp(const simulstate *s){
  const float a0 = pmons[0][s->active[0]].effa;
  const float a1 = pmons[1][s->active[1]].effa;
  return a0 > a1 ? -1 : a1 > a0 ? 1 : 0;
}

static inline void
bottomhalf_allfast(simulstate *s, results *r){
  if(s->turns[0] == 0){ // launch new fast attack p0
    s->turns[0] = pmons[0][s->active[0]].fa->turns;
  }
  if(s->turns[1] == 0){ // launch new fast attack p1
    s->turns[1] = pmons[1][s->active[1]].fa->turns;
  }
  account_fast_move(s, 0);
  account_fast_move(s, 1);
  tophalf(s, r);
}

static inline void
bottomhalf_charged_fast(simulstate *s, results *r, int player, const attack *c, int shielded){
  if(!throw_charged_move(s, player, c, shielded)){
    account_fast_move(s, other_player(player));
  }
  tophalf(s, r);
}

// pass three 2-vectors of player id, attacks used, and shield state
static inline void
bottomhalf_cc_ordered(simulstate *s, results *r, const int *ps,
                      const attack **as, const int *ss){
  if(!throw_charged_move(s, ps[0], as[0], ss[0])){
    throw_charged_move(s, ps[1], as[1], ss[1]);
  }
  tophalf(s, r);
}

static inline void
bottomhalf_charged_charged(simulstate *s, results *r, const attack *p0c,
                           const attack *p1c, int p0shield, int p1shield){
  const int cmp = p0_wins_cmp(s);
  const attack *as[2];
  int ps[2];
  int ss[2];
  as[0] = p0c; as[1] = p1c;
  ss[0] = p1shield; ss[1] = p0shield;
  ps[0] = 0; ps[1] = 1;
  if(cmp == 0){ // simulate both paths
    simulstate s2 = *s;
    bottomhalf_cc_ordered(&s2, r, ps, as, ss);
  }
  if(cmp >= 0){
    as[0] = p1c; as[1] = p0c;
    ss[0] = p0shield; ss[1] = p1shield;
    ps[0] = 1; ps[1] = 0;
  }
  bottomhalf_cc_ordered(s, r, ps, as, ss);
}
