#define bottomhalf_p0charged(cnum, cond, a) do{ \
  if(cond){ \
    const auto c = pmons[0][s->active[0]].a; \
    scopy = *s; \
    bottomhalf_charged_fast(&scopy, r, 0, c, cnum, sh1); \
    if(p1c1){ \
      scopy = *s; \
      bottomhalf_charged_charged(&scopy, r, c, p1ca1, cnum, 1, sh0, sh1); \
    } \
    if(p1c2){ \
      scopy = *s; \
      bottomhalf_charged_charged(&scopy, r, c, p1ca2, cnum, 2, sh0, sh1); \
    } \
  } \
}while(0);

static void tophalf(simulstate *s, results *r){
  static unsigned depth = 0;
  const int hp0 = s->hp[0][s->active[0]];
  const int hp1 = s->hp[1][s->active[1]];
  ++depth;
  if((r->wins[0] + r->wins[1] + r->ties) % 10000000 == 0){
    printf("%'lu %'lu %'lu %d %d %u\n", r->wins[0], r->wins[1], r->ties, hp0, hp1, depth);
  }
  if(!hp0){
    if(hp1){
      handle_one_ko(s, r, 0);
    }else{
      handle_dual_kos(s, r);
    }
  }else if(!hp1){
    handle_one_ko(s, r, 1);
  }else{
    uint32_t h;
    int ret = check_cache(s, r, &h);
    if(ret){
      const auto sh0 = s->shields[0];
      const auto sh1 = s->shields[1];
      const auto p0c1 = can_charged1(s, 0);
      const auto p1c1 = can_charged1(s, 1);
      const auto p1ca1 = pmons[1][s->active[1]].ca1;
      const auto p0c2 = can_charged2(s, 0);
      const auto p1c2 = can_charged2(s, 1);
      const auto p1ca2 = pmons[1][s->active[1]].ca2;
      simulstate scopy;
      bottomhalf_p0charged(1, p0c1, ca1);
      bottomhalf_p0charged(2, p0c2, ca2);
      if(p1c1){ // now p1c1 and p0 fast, if p1c1 can happen
        scopy = *s;
        bottomhalf_charged_fast(&scopy, r, 1, p1ca1, 1, sh0);
      }
      if(p1c2){ // now p1c2 and p0 fast, if p1c2 can happen
        scopy = *s;
        bottomhalf_charged_fast(&scopy, r, 1, p1ca2, 2, sh0);
      } // always handle the fast-fast case
      bottomhalf_allfast(s, r);
      if(ret < 0){
        update_cache(h, r);
      }
    }
  }
  --depth;
}
