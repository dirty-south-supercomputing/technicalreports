// whenever players change or a buff/debuff takes effect we must recalculate
// the damages of the [4..6] attacks currently in play.
static inline void calculate_damages(simulstate *s){
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
  const float a0 = p0->s.effa;
  const float a1 = p1->s.effa;
  s->cmp = a0 > a1 ? -1 : a1 > a0 ? 1 : 0;
}

static inline void subin(simulstate *s, int player, int pos){
  s->active[player] = pos;
  s->buffleva[player] = 0;
  s->bufflevd[player] = 0;
  calculate_damages(s);
}
