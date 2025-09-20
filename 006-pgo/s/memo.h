#ifndef PSIMUL_MEMO
#define PSIMUL_MEMO

#include <murmurhash.h>

constexpr uint32_t MEMOMASK = 0xffff;
//constexpr uint32_t MEMOMASK = 0xfffffff;

// We use the lower two bits of both fast attack turn counters, the
// lower five bits of each energy tally, and the lower six bits of
// each HP to get a lookup. We must then verify that the full state matches.
static inline uint32_t
hash_simulstate(const simulstate *s){
  uint32_t h;
  lmmh_x86_32(s, sizeof(*s), 0, &h);
  h &= MEMOMASK;
  return h;
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
