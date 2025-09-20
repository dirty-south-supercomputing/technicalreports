#include "simul.h"
#include "cache.h"

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
    case TYPECOUNT: u = "**"; break; // Hidden Power
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
