#ifndef PSIMUL_MEMO
#define PSIMUL_MEMO

#include <murmurhash.h>

int init_cache(void);
int stop_cache(void);

// returns -1 iff we were entered into cache, in which case caller must call
// update_cache() at the end of this subtree to record results.
//
// returns 0 iff we were already present in the cache, in which case we can
// use the stored results and elide execution of this subtree. dom is set.
//
// returns 1 if we were neither present nor entered (this would be due to
// aliasing an open entry), in which case we don't use the cache. run the
// subtree, but don't call update_cache() on terminal nodes.
int check_cache(const simulstate *s, results *r, uint32_t *h, bool &dom);

// close the open entry, recording the delta between results.
void update_cache(uint32_t h, const results *r, bool dom);

#endif
