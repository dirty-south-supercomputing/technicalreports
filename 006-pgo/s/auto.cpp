#include "simul.h"
#include "cache.h"

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
print_team(int player){
  printf("TEAM %d\n", player + 1);
  for(unsigned i = 0 ; i < TEAMSIZE ; ++i){
    if(pmons[player][i].s.s){
      print_pmon(&pmons[player][i]);
    }
  }
}

static void
run(void){
  simulstate sstate = {};
  results r;
  r.wins[0] = r.wins[1] = r.ties = 0;
  print_team(0);
  print_team(1);
  simul(&sstate, &r);
}

static void
autofight_sopp(const species &sopp){
  // run through attack sets for sopp; su has already chosen attacks
  for(const auto sfa : sopp.attacks){
    if(charged_attack_p(sfa)){
      continue;
    }
    for(const auto sca : sopp.attacks){
      if(charged_attack_p(sca)){
        pmons[1][0].fa = sfa;
        pmons[1][0].ca1 = sca;
        pmons[1][0].ca2 = nullptr;
        std::cout << "\t\"" << sopp.name << "\" \"" << sfa->name << "\" \"" << sca->name << "\"" << std::endl;
        //run();
      }
    }
  }
}

static void
autofight_su(void){
  for(unsigned opp = 0 ; opp < SPECIESCOUNT ; ++opp){
    const species &sopp = sdex[opp];
    stats *st = find_optimal_set(&sopp, 1500, 0, false, calc_pok_gmean);
    pmons[1][0].s.ia = st->ia;
    pmons[1][0].s.id = st->id;
    pmons[1][0].s.is = st->is;
    pmons[1][0].s.hlevel = st->hlevel;
    pmons[1][0].s.s = &sopp;
    fill_stats(&pmons[1][0].s, false);
    while(st){
      stats *tmp = st->next;
      delete st;
      st = tmp;
    }
    autofight_sopp(sopp);
  }
}

static void
autofight(const species &su){
  // run through all attack sets for su
  for(const auto sfa : su.attacks){
    if(charged_attack_p(sfa)){
      continue;
    }
    for(const auto sca : su.attacks){
      if(charged_attack_p(sca)){
        pmons[0][0].fa = sfa;
        pmons[0][0].ca1 = sca;
        pmons[0][0].ca2 = nullptr;
        pmons[0][0].s.s = &su;
        stats *st = find_optimal_set(&su, 1500, 0, false, calc_pok_gmean);
        pmons[0][0].s.ia = st->ia;
        pmons[0][0].s.id = st->id;
        pmons[0][0].s.is = st->is;
        pmons[0][0].s.hlevel = st->hlevel;
        fill_stats(&pmons[0][0].s, false);
        while(st){
          stats *tmp = st->next;
          delete st;
          st = tmp;
        }
        std::cout << "\"" << su.name << "\" \"" << sfa->name << "\" \"" << sca->name << "\" vs." << std::endl;
        autofight_su();
      }
    }
  }
}

int main(void){
  memset(&pmons, 0, sizeof(pmons));
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    const species &su = sdex[u];
    autofight(su);
  }
  return EXIT_SUCCESS;
}
