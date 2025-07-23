static void tophalf(simulstate *s, results *r){
  if(handle_ko(s, r)){
    return; // it played out the games itself, and mutated s
  }
  const auto sh0 = s->shields[0];
  const auto sh1 = s->shields[1];
  const auto p0c1 = can_charged1(s, 0);
  const auto p1c1 = can_charged1(s, 1);
  const auto p0ca1 = pmons[0][s->active[0]].ca1;
  const auto p1ca1 = pmons[1][s->active[1]].ca1;
  const auto p0c2 = can_charged2(s, 0);
  const auto p1c2 = can_charged2(s, 1);
  const auto p0ca2 = pmons[0][s->active[0]].ca2;
  const auto p1ca2 = pmons[1][s->active[1]].ca2;
  simulstate scopy;
  if(p0c1){
    scopy = *s;
    bottomhalf_charged_fast(&scopy, r, 0, p0ca1, 1, sh1);
    if(p1c1){
      scopy = *s;
      bottomhalf_charged_charged(&scopy, r, p0ca1, p1ca1, 1, 1, sh0, sh1);
    }
    if(p1c2){
      scopy = *s;
      bottomhalf_charged_charged(&scopy, r, p0ca1, p1ca2, 1, 2, sh0, sh1);
    }
  }
  if(p0c2){
    scopy = *s;
    bottomhalf_charged_fast(&scopy, r, 0, p0ca2, 2, sh1);
    if(p1c1){
      scopy = *s;
      bottomhalf_charged_charged(&scopy, r, p0ca2, p1ca1, 2, 1, sh0, sh1);
    }
    if(p1c2){
      scopy = *s;
      bottomhalf_charged_charged(&scopy, r, p0ca2, p1ca2, 2, 2, sh0, sh1);
    }
  }
  if(p1c1){
    scopy = *s;
    bottomhalf_charged_fast(&scopy, r, 1, p1ca1, 1, sh0);
  }
  if(p1c2){
    scopy = *s;
    bottomhalf_charged_fast(&scopy, r, 1, p1ca2, 2, sh0);
  }
  bottomhalf_allfast(s, r);
}
