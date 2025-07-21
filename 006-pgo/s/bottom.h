// decrement fast move turns by one. if the attack concludes as a result,
// inflict damage and add energy. returns true in the case of a KO.
static bool account_fast_move(simulstate *s, int player){
  if(!s->turns[player]){
    s->turns[player] = activemon(s, player)->fa->turns;
  }
  if(--s->turns[player]){
    return false;
  }
  const pmon *p = &pmons[player][s->active[player]];
  accumulate_energy(&s->e[player][s->active[player]], p->fa->energytrain);
  int op = other_player(player);
  return inflict_damage(&s->hp[op][s->active[op]], s->dam[player][0]);
}

static inline void
bottomhalf_allfast(simulstate *s, results *r){
  account_fast_move(s, 0);
  account_fast_move(s, 1);
  tophalf(s, r);
}

static inline void
bottomhalf_charged_fast(simulstate *s, results *r, int player, const attack *c,
                        int aid, int shielded){
  if(!throw_charged_move(s, player, c, aid, shielded)){
    account_fast_move(s, other_player(player));
  }
  tophalf(s, r);
}

// pass three 2-vectors of player id, attacks used/aids, and shield state
static inline void
bottomhalf_cc_ordered(simulstate *s, results *r, const int *ps,
                      const attack **as, const int *aids, const int *ss){
  if(!throw_charged_move(s, ps[0], as[0], aids[0], ss[0])){
    throw_charged_move(s, ps[1], as[1], aids[1], ss[1]);
  }
  tophalf(s, r);
}

static inline void
bottomhalf_charged_charged(simulstate *s, results *r, const attack *p0c,
                           const attack *p1c, int aid0, int aid1,
                           int p0shield, int p1shield){
  int ps[2], ss[2], aids[2];
  const attack *as[2];

  as[0] = p0c; as[1] = p1c;
  aids[0] = aid0; aids[1] = aid1;
  ss[0] = p1shield; ss[1] = p0shield;
  ps[0] = 0; ps[1] = 1;
  if(s->cmp == 0){ // simulate both paths
    simulstate s2 = *s;
    bottomhalf_cc_ordered(&s2, r, ps, as, aids, ss);
  }
  if(s->cmp >= 0){
    as[0] = p1c; as[1] = p0c;
    aids[0] = aid1; aids[1] = aid0;
    ss[0] = p0shield; ss[1] = p1shield;
    ps[0] = 1; ps[1] = 0;
  }
  bottomhalf_cc_ordered(s, r, ps, as, aids, ss);
}
