static inline bool inflict_damage(uint16_t *hp, unsigned damage){
  *hp -= (*hp < damage ? *hp : damage);
  return !!*hp; // return true iff hp > 0
}

// calculate the damage inflicted by p with attack buff abuff on o with defense
// buff dbuff using a, with no shield in play.
static unsigned calc_damage(const pmon *p, const pmon *o, const attack *a,
                            int abuff, int dbuff){
  float d = p->s.effa * mapbuff(abuff);
  d *= a->powertrain;
  d *= 13; // first half of the 0.65 multiplier
  d *= p->s.shadow ? 6 : 1; // first half of shadow multipliers
  d *= o->s.shadow ? 6 : 1;
  if(has_stab_p(p->s.s, a)){
    d = calc_stab(d);
  }
  d *= o->s.s->type_effectiveness(a);
  d /= o->s.effd * mapbuff(dbuff);
  d /= p->s.shadow ? 5 : 1; // second half of shadow multipliers
  d /= o->s.shadow ? 5 : 1;
  d /= 20; // second half of the 0.65 multiplier
  return static_cast<unsigned>(floor(d)) + 1;
}

static inline void accumulate_energy(uint16_t *e, int addend){
  if((*e += addend) > ENERGY_MAX){
    *e = ENERGY_MAX;
  }
}
