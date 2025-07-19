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

static inline float mapbuff(int bufflevel){
  static const float buffmap[9] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 }; // FIXME
  return buffmap[bufflevel + 4];
}

// calculate the damage inflicted by p with attack buff abuff on o with defense
// buff dbuff using a, with no shield in play.
static unsigned calc_damage(const pmon *p, const pmon *o, const attack *a,
                            int abuff, int dbuff){
  float d = p->effa * mapbuff(abuff);
  d *= a->powertrain;
  d *= 13; // first half of the 0.65 multiplier
  if(has_stab_p(p->s.s, a)){
    d = calc_stab(d);
  }
  if(p->shadow && !o->shadow){
    d *= 6;
  }else if(o->shadow && !p->shadow){
    d *= 5;
  }
  d *= o->s.s->type_effectiveness(a);
  d /= p->effd * mapbuff(dbuff);
  d /= 20; // second half of the 0.65 multiplier
  if(p->shadow && !o->shadow){
    d /= 5;
  }else if(o->shadow && !p->shadow){
    d /= 6;
  }
  //printf("damage: %f\n", d);
  return static_cast<unsigned>(floor(d)) + 1;
}

static void accumulate_energy(energy *e, int addend){
  if((*e += addend) > ENERGY_MAX){
    *e = ENERGY_MAX;
  }
}

// returns true if we KO the opponent. a must be a charged move, and the
// player must have sufficient energy. oshield may only be set if the
// opponent has a shield. aid is index into s->dam[player].
static inline bool
throw_charged_move(simulstate *s, int player, const attack *a, int aid, bool oshield){
  int op = other_player(player);
  accumulate_energy(&s->e[player][s->active[player]], a->energytrain);
  if(oshield){
    --s->shields[other_player(player)];
    return inflict_damage(&s->hp[op][s->active[op]], 1);
  }
  return inflict_damage(&s->hp[op][s->active[op]], s->dam[player][aid]);
}

// whenever players change or a buff/debuff takes effect we must recalculate
// the damages of the [4..6] attacks currently in play.
static void calculate_damages(simulstate *s){
  const pmon *p0 = &pmons[0][s->active[0]];
  const pmon *p1 = &pmons[1][s->active[1]];
  s->dam[0][0] = calc_damage(p0, p1, p0->fa, s->buffleva[0], s->bufflevd[1]);
  s->dam[0][1] = calc_damage(p0, p1, p0->ca1, s->buffleva[0], s->bufflevd[1]);
  if(p0->ca2){
    s->dam[0][2] = calc_damage(p0, p1, p0->ca2, s->buffleva[0], s->bufflevd[1]);
  }
  s->dam[1][0] = calc_damage(p1, p0, p1->fa, s->buffleva[1], s->bufflevd[0]);
  s->dam[1][1] = calc_damage(p1, p0, p1->ca1, s->buffleva[1], s->bufflevd[0]);
  if(p1->ca2){
    s->dam[1][2] = calc_damage(p1, p0, p1->ca2, s->buffleva[1], s->bufflevd[0]);
  }
  const float a0 = p0->effa;
  const float a1 = p1->effa;
  s->cmp = a0 > a1 ? -1 : a1 > a0 ? 1 : 0;
}
