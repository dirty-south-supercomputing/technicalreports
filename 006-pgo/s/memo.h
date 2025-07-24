#ifndef PSIMUL_MEMO
#define PSIMUL_MEMO

// We use the lower two bits of both fast attack turn counters, the
// lower five bits of each energy tally, and the lower six bits of
// each HP to get a lookup. We must then verify that the full state matches.
static inline uint32_t
hash_simulstate(const simulstate *s){
  const uint32_t hp0 = s->hp[0][s->active[0]] % 64u;
  const uint32_t hp1 = (s->hp[1][s->active[1]] % 64u) << 6u;
  const uint32_t e0 = (s->e[0][s->active[0]] % 32u) << 12u;
  const uint32_t e1 = (s->e[1][s->active[1]] % 32u) << 17u;
  const uint32_t f0 = (s->turns[0] % 4u) << 22u;
  const uint32_t f1 = (s->turns[1] % 4u) << 24u;
  return f1 | f0 | e1 | e0 | hp1 | hp0;
}

int init_cache(void);
int stop_cache(void);

// returns 0 iff we were already present in the cache, in which case we can
// use the stored results and elide execution. returns -1 if we were entered
// into cache, in which case we must call update_cache() on all exit paths
// to record the results delta. returns 1 if we were neither present nor
// entered (this would be due to aliasing an open entry), in which case we
// don't use the cache.
int check_cache(const simulstate *s, results *r, uint32_t *h);

// close the open entry, recording the delta between results.
void update_cache(uint32_t h, const results *r);

#endif
