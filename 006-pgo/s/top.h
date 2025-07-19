// each turn has a top half and bottom halves.
// in the top half we determine which choice-pairs can be performed.
// in the bottom halves, we simulate a choice-pair.
static void tophalf(simulstate *s, results *r){
  if(handle_ko(s, r)){
    return;
  }
  const auto shielded0 = s->shields[0];
  const auto shielded1 = s->shields[1];
  const auto p0c1 = can_charged1(s, 0);
  const auto p1c1 = can_charged1(s, 1);
  const auto p0ca1 = pmons[0][s->active[0]].ca1;
  const auto p1ca1 = pmons[1][s->active[1]].ca1;
  const auto p0c2 = can_charged2(s, 0);
  const auto p1c2 = can_charged2(s, 1);
  const auto p0ca2 = pmons[0][s->active[0]].ca2;
  const auto p1ca2 = pmons[1][s->active[1]].ca2;
  simulstate scopy = *s;
  // each bottomhalf_charged_charged() might FO up to 4 ways
  // (simulate both sides of CMP tie, simulate shields)
  if(p0c1){
    bottomhalf_charged_fast(&scopy, r, 0, p0ca1, 1, shielded1);
    if(p1c1){
      scopy = *s;
      bottomhalf_charged_charged(&scopy, r, p0ca1, p1ca1, 1, 1, shielded0, shielded1);
    }
    if(p1c2){
      scopy = *s;
      bottomhalf_charged_charged(&scopy, r, p0ca1, p1ca2, 1, 2, shielded0, shielded1);
    }
  }
  if(p0c2){
    bottomhalf_charged_fast(&scopy, r, 0, p0ca2, 2, shielded1);
    if(p1c1){
      scopy = *s;
      bottomhalf_charged_charged(&scopy, r, p0ca2, p1ca1, 2, 1, shielded0, shielded1);
    }
    if(p1c2){
      scopy = *s;
      bottomhalf_charged_charged(&scopy, r, p0ca2, p1ca2, 2, 2, shielded0, shielded1);
    }
  }
  if(p1c1){
    scopy = *s;
    bottomhalf_charged_fast(&scopy, r, 1, p1ca1, 1, shielded0);
  }
  if(p1c2){
    scopy = *s;
    bottomhalf_charged_fast(&scopy, r, 1, p1ca2, 2, shielded0);
  }
  bottomhalf_allfast(s, r);
}
