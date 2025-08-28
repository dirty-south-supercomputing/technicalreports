#include <cstdio>
#include <memory>
#include <cstdlib>
#include "../pgotypes.cpp"
#include "simul.h"
#include "sift.h"
#include "damage.h"
#include "memo.h"
#include "in.h"
static void tophalf(simulstate *s, results *r);
#include "bottom.h"
#include "ko.h"
#include "top.h"

pmon pmons[2][TEAMSIZE] = {};

// we have 26 bits of discriminant
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

int init_cache(void){
  elems = new cacheelem[CACHEELEMS];
  memset(elems, 0, sizeof(*elems) * CACHEELEMS);
  return 0;
}

static uint64_t cache_opens;  // opened a cache element
static uint64_t cache_hits;   // was set and valid for us
static uint64_t cache_misses; // was set but not us
static uint64_t cache_late;   // was open when we looked it up
static uint64_t cache_used;   // elements used

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
  r->wins[0] += elem.r.wins[0];
  r->wins[1] += elem.r.wins[1];
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

int stop_cache(void){
  printf("hits: %'lu misses: %'lu opens: %'lu late: %'lu\n",
          cache_hits, cache_misses, cache_opens, cache_late);
  printf(" used: %'lu/%'lu\n", cache_used, CACHEELEMS);
  delete[] elems;
  return 0;
}

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " pokémonN iv@level fast charged..." << std::endl;
  std::cerr << " both teams must have the same number of pokémon (not more than 3)" << std::endl;
  exit(EXIT_FAILURE);
}

static int
ucode_type(pgo_types_e t){
  const char *u;
  switch(t){
    case TYPE_BUG: u = "Bu"; break;
    case TYPE_DARK: u = "Da"; break;
    case TYPE_DRAGON: u = "Dr"; break;
    case TYPE_ELECTRIC: u = "El"; break;
    case TYPE_FAIRY: u = "Fa"; break;
    case TYPE_FIGHTING: u = "Fg"; break;
    case TYPE_FIRE: u = "Fr"; break;
    case TYPE_FLYING: u = "Fl"; break;
    case TYPE_GHOST: u = "Gh"; break;
    case TYPE_GRASS: u = "Gs"; break;
    case TYPE_GROUND: u = "Go"; break;
    case TYPE_ICE: u = "Ic"; break;
    case TYPE_NORMAL: u = "No"; break;
    case TYPE_POISON: u = "Po"; break;
    case TYPE_PSYCHIC: u = "Ps"; break;
    case TYPE_ROCK: u = "Ro"; break;
    case TYPE_STEEL: u = "St"; break;
    case TYPE_WATER: u = "Wa"; break;
    default: throw std::invalid_argument("invalid ptype"); return -1;
  }
  return printf("%s", u);
}

static void
print_pmon(const pmon *p){
  if(p->s.s->t2 != p->s.s->t1 && p->s.s->t2 != TYPECOUNT){
    ucode_type(p->s.s->t1);
    ucode_type(p->s.s->t2);
  }else{
    printf(" ");
    ucode_type(p->s.s->t1);
    printf(" ");
  }
  unsigned hl;
  unsigned l = halflevel_to_level(p->s.hlevel, &hl);
  printf(" %s %s%u%s effa: %g effd: %g mhp: %u cp %u\n",
        p->s.s->name.c_str(),
        p->s.shadow ? "(shadow) " : "",
        l, hl ? ".5" : "",
        p->s.effa, p->s.effd, p->s.mhp, p->s.cp);
  printf(" ");
  ucode_type(p->fa->type);
  printf(" %s%20s %3u %3d %u\n", has_stab_p(p->s.s, p->fa) ? "(*)" : "   ",
          p->fa->name, p->fa->powertrain, p->fa->energytrain, p->fa->turns);
  printf(" ");
  ucode_type(p->ca1->type);
  printf(" %s%20s %3u %3d\n", has_stab_p(p->s.s, p->ca1) ? "(*)" : "   ",
          p->ca1->name, p->ca1->powertrain, -p->ca1->energytrain);
  if(p->ca2){
    printf(" ");
    ucode_type(p->ca2->type);
    printf(" %s%20s %3u %3d\n", has_stab_p(p->s.s, p->ca2) ? "(*)" : "   ",
          p->ca2->name, p->ca2->powertrain, -p->ca2->energytrain);
  }
}

static void
simul(simulstate *s, results *r){
  s->turns[0] = s->turns[1] = 0u;
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    s->e[0][i] = s->e[1][i] = 0;
  }
  s->buffleva[0] = s->buffleva[1] = 0;
  s->bufflevd[0] = s->bufflevd[1] = 0;
  s->subtimer[0] = s->subtimer[1] = 1;
  s->shields[0] = s->shields[1] = 2;
  s->active[0] = s->active[1] = 0;
  // calculate eff_a and eff_d for all players
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    const species *s;
    pmon *p = &pmons[0][i];
    if( (s = p->s.s) ){
      p->effa = calc_eff_a(s->atk + p->s.ia, p->s.hlevel, p->s.shadow);
      p->effd = calc_eff_d(s->def + p->s.id, p->s.hlevel, p->s.shadow);
    }
    p = &pmons[1][i];
    if( (s = p->s.s) ){
      p->effa = calc_eff_a(s->atk + p->s.ia, p->s.hlevel, p->s.shadow);
      p->effd = calc_eff_d(s->def + p->s.id, p->s.hlevel, p->s.shadow);
    }
  }
  calculate_damages(s);
  /*for(int p = 0 ; p < 2 ; ++p){
    for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
      printf("dam[%d][%u] = %d\n", p, i, s->dam[p][i]);
    }
  }*/
  tophalf(s, r);
}

static void
print_team(int player){
  printf("TEAM %d\n", player + 1);
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    if(pmons[player][i].s.s){
      print_pmon(&pmons[player][i]);
    }
  }
}

static void
print_intro(void){
  printf("pokémon exhaustive simulator state is %zuB\n", sizeof(simulstate));
}

int main(int argc, char** argv){
  setlocale(LC_ALL, "");
  const char* argv0 = *argv;
  simulstate sstate = {};
  --argc;
  ++argv;
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    if(!argc && i){
      break;
    }
    if(lex_pmon(&pmons[0][i], &sstate.hp[0][i], &argc, &argv)){
      usage(argv0);
    }
    if(lex_pmon(&pmons[1][i], &sstate.hp[1][i], &argc, &argv)){
      usage(argv0);
    }
  }
  print_intro();
  print_team(0);
  print_team(1);
  if(argc){
    std::cerr << "unexpected argument: " << *argv << std::endl;
    usage(argv0);
  }
  if(init_cache()){
    exit(EXIT_FAILURE);
  }
  results r;
  r.wins[0] = r.wins[1] = r.ties = 0;
  simul(&sstate, &r);
  stop_cache();
  unsigned long total = r.wins[0] + r.wins[1] + r.ties;
  printf("p0 wins: %'lu p1 wins: %'lu ties: %'lu\n", r.wins[0], r.wins[1], r.ties);
  printf(" total: %'lu p0 %.04f%% p1 %.04f%% t %.04f%%\n", total,
        r.wins[0] * 100.0 / total, r.wins[1] * 100.0 / total, r.ties * 100.0 / total);
  return EXIT_SUCCESS;
}
