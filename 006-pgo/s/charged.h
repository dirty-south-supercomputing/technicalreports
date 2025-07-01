// returns true on a KO
static bool inflict_damage(int *hp, int damage){
  //printf("inflicting %d damage on %d hp\n", damage, *hp);
  if(*hp < damage){
    *hp = 0;
  }else{
    *hp -= damage;
  }
  return !*hp;
}

// calculate the damage inflicted by player on other_player(player) using an
// attack having the specified power, with no shield in play.
static int calc_damage(const simulstate *s, int player, const attack *a){
  const int opp = other_player(player);
  const pmon *p = &pmons[player][s->active[player]];
  const pmon *o = &pmons[opp][s->active[opp]];
  float d = calc_eff_a(p->s.s->atk + p->s.ia, p->s.hlevel, p->shadow);
  // FIXME handle active buffs
  // FIXME handle typing
  d *= a->powertrain;
  if(has_stab_p(p->s.s, a)){
    d = calc_stab(d);
  }
  d /= calc_eff_d(o->s.s->def + o->s.id, o->s.hlevel, o->shadow);
  //printf("damage: %f\n", d);
  return floor(d);
}

static void accumulate_energy(int *e, int energy){
  const int ENERGY_MAX = 100;
  if((*e += energy) > ENERGY_MAX){
    *e = ENERGY_MAX;
  }
}

// returns true if we KO the opponent
// mt must be a charged move, and the player must have sufficient energy
// mt and mo can be shielded moves only if the appropriate player has a shield
static inline bool
throw_charged_move(simulstate *s, int player, pgo_move_e mt, pgo_move_e mo){
  const attack *a;
  if(mt == MOVE_CHARGED1 || mt == MOVE_CHARGED1_SHIELD){
    a = pmons[player][s->active[player]].ca1;
  }else{
    a = pmons[player][s->active[player]].ca2;
  }
  accumulate_energy(&s->e[player][s->active[player]], a->energytrain);
  if(shielded_move_p(mo)){
    --s->shields[other_player(player)];
    return inflict_damage(&s->hp[other_player(player)][s->active[other_player(player)]], 1);
  }
  return inflict_damage(&s->hp[other_player(player)][s->active[other_player(player)]],
          calc_damage(s, player, a));
}
