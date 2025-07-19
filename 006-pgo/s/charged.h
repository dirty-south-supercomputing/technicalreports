// returns true on a KO
static bool inflict_damage(hpoints *hp, int damage){
  //printf("inflicting %d damage on %d hp\n", damage, *hp);
  if(*hp < damage){
    *hp = 0;
  }else{
    *hp -= damage;
  }
  return !*hp;
}

// calculate the damage inflicted by p with attack buff abuff on o with defense
// buff dbuff using a, with no shield in play.
static float calc_damage(const pmon *p, const pmon *o, const attack *a,
                          float abuff, float dbuff){
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

static void accumulate_energy(energy *e, int addend){
  if((*e += addend) > ENERGY_MAX){
    *e = ENERGY_MAX;
  }
}

// returns true if we KO the opponent
// mt must be a charged move, and the player must have sufficient energy
// oshield may only be set if the opponent has a shield.
static inline bool
throw_charged_move(simulstate *s, int player, const attack *a, bool oshield){
  int op = other_player(player);
  accumulate_energy(&s->e[player][s->active[player]], a->energytrain);
  if(oshield){
    --s->shields[other_player(player)];
    return inflict_damage(&s->hp[op][s->active[op]], 1);
  }
  return inflict_damage(&s->hp[op][s->active[op]],
          calc_buffed_damage(
            calc_damage(&pmons[player][s->active[player]],
             &pmons[op][s->active[op]], a),
            s->buffleva[player], s->bufflevd[op]));
}
