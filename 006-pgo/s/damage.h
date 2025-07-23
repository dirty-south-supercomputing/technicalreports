static bool inflict_damage(uint16_t *hp, unsigned damage){
  *hp -= (*hp < damage ? *hp : damage);
  return !!*hp; // return true iff hp > 0
}

static inline float mapbuff(int bufflevel){
  static const float buffmap[9] = { 4/8, 4/7, 4/6, 4/5, 1, 5/4, 6/4, 7/4, 8/4 };
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

static void accumulate_energy(uint16_t *e, int addend){
  if((*e += addend) > ENERGY_MAX){
    *e = ENERGY_MAX;
  }
}
