// we have 28 bits of discriminant
constexpr auto MEMOMASK = 0xfffffff;
static constexpr unsigned long CACHEELEMS = MEMOMASK + 1;

struct cacheelem {
  simulstate s;
  results r;
  enum {
    ELEMINIT, // element has never been touched
    ELEMOPEN, // element is open and waiting to be closed
    ELEMSET,  // element is fully prepared for use
  } state;
};

static cacheelem *elems;

static uint64_t cache_opens;  // opened a cache element
static uint64_t cache_hits;   // was set and valid for us
static uint64_t cache_misses; // was set but not us
static uint64_t cache_late;   // was open when we looked it up
static uint64_t cache_used;   // elements used

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

int init_cache(void){
  elems = new cacheelem[CACHEELEMS];
  memset(elems, 0, sizeof(*elems) * CACHEELEMS);
  cache_opens = cache_hits = cache_misses = cache_late = cache_used = 0;
  return 0;
}

static void
init_elem(cacheelem &elem, const simulstate *s, const results *r){
  memcpy(&elem.s, s, sizeof(*s));
  memcpy(&elem.r, r, sizeof(*r));
  elem.state = cacheelem::ELEMOPEN;
  ++cache_opens;
}

int check_cache(const simulstate *s, results *r, uint32_t *h){
  *h = hash_simulstate(s);
  if(*h >= CACHEELEMS){
    std::cerr << "specified element " << *h << " of " << CACHEELEMS << std::endl;
    throw std::exception();
  }
  cacheelem &elem = elems[*h];
  if(elem.state == cacheelem::ELEMINIT){
    init_elem(elem, s, r);
    ++cache_used;
    return -1;
  }
  if(elem.state == cacheelem::ELEMOPEN){
    ++cache_late;
    return 1;
  }
  // if we got here, element must be set. check if it's valid for us.
  if(elem.s != *s){
    ++cache_misses;
    init_elem(elem, s, r);
    return -1;
  }
  if(~0ULL - r->wins[0] < elem.r.wins[0]){
    std::cerr << "team 0 wins overflowed" << std::endl;
    throw std::exception();
  }
  r->wins[0] += elem.r.wins[0];
  if(~0ULL - r->wins[1] < elem.r.wins[1]){
    std::cerr << "team 1 wins overflowed" << std::endl;
    throw std::exception();
  }
  r->wins[1] += elem.r.wins[1];
  if(~0ULL - r->ties < elem.r.ties){
    std::cerr << "ties overflowed" << std::endl;
    throw std::exception();
  }
  r->ties += elem.r.ties;
  ++cache_hits;
  return 0;
}

void update_cache(uint32_t h, const results *r){
  cacheelem &elem = elems[h];
  if(elem.state != cacheelem::ELEMOPEN){
    std::cerr << "error: state was " << elem.state << " for " << h << std::endl;
    throw std::exception();
  }
  elem.r.wins[0] = r->wins[0] - elem.r.wins[0];
  elem.r.wins[1] = r->wins[1] - elem.r.wins[1];
  elem.r.ties = r->ties - elem.r.ties;
  elem.state = cacheelem::ELEMSET;
}

int stop_cache(bool verbose){
  if(verbose){
    printf("hits: %'lu misses: %'lu opens: %'lu late: %'lu\n",
            cache_hits, cache_misses, cache_opens, cache_late);
    printf(" used: %'lu/%'lu\n", cache_used, CACHEELEMS);
  }
  delete[] elems;
  return 0;
}
