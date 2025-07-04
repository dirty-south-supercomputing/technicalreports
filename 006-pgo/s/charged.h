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
static float calc_damage(const pmon *p, const pmon *o, const attack *a){
  float d = calc_eff_a(p->s.s->atk + p->s.ia, p->s.hlevel, p->shadow);
  d *= a->powertrain;
  d *= 13; // first half of the 0.65 multiplier
  if(has_stab_p(p->s.s, a)){
    d = calc_stab(d);
  }
  d /= calc_eff_d(o->s.s->def + o->s.id, o->s.hlevel, o->shadow);
  d /= 20; // second half of the 0.65 multiplier
  d *= o->s.s->type_effectiveness(a);
  //printf("damage: %f\n", d);
  return floor(d);
}

// calculate real damage given precalc + buffs
static int calc_buffed_damage(float d, float abuff, float dbuff){
  return floor(d);
}

static void accumulate_energy(int *e, int energy){
  if((*e += energy) > ENERGY_MAX){
    *e = ENERGY_MAX;
  }
}

// returns true if we KO the opponent
// mt must be a charged move, and the player must have sufficient energy
// oshield may only be set if the opponent has a shield.
static inline bool
throw_charged_move(simulstate *s, int player, pgo_move_e mt, bool oshield){
  const attack *a;
  int didx;
  if(mt == MOVE_CHARGED1){
    a = pmons[player][s->active[player]].ca1;
    didx = 1;
  }else{
    a = pmons[player][s->active[player]].ca2;
    didx = 2;
  }
  accumulate_energy(&s->e[player][s->active[player]], a->energytrain);
  int op = other_player(player);
  if(oshield){
    --s->shields[other_player(player)];
    return inflict_damage(&s->hp[op][s->active[op]], 1);
  }
  return inflict_damage(&s->hp[op][s->active[op]],
          calc_buffed_damage(pmons[player][s->active[player]].damage[didx][s->active[op]], 1, 1));
}
