static const attack *
lex_species_charged_attacks(const species *s, const char *spec, const attack **ca2){
  *ca2 = NULL;
  const char *sep = strchr(spec, '/');
  if(sep){
    char *fspec = strndup(spec, sep - spec);
    const attack *ca1 = species_charged_attack(s, fspec);
    *ca2 = species_charged_attack(s, sep + 1);
    free(fspec);
    if(*ca2 == NULL){
      return NULL;
    }
    return ca1;
  }
  const attack *ca1 = species_charged_attack(s, spec);
  return ca1;
}

// fill in a stats structure given only species, IVs, and level
static void
fill_stats(stats* s, bool shadow){
  // these are actually a_raw and d_raw, used only for CMP (so can we kill effd?)
  s->effa = calc_eff_a(s->s->atk + s->ia, s->hlevel, false);
  s->effd = calc_eff_d(s->s->def + s->id, s->hlevel, false);
  s->mhp = calc_mhp(s->s->sta + s->is, s->hlevel);
  s->geommean = calc_gmean(s->effa, s->effd, s->mhp);
  s->cp = calccp(s->s->atk + s->ia, s->s->def + s->id, s->s->sta + s->is, s->hlevel);
  s->shadow = shadow;
  s->next = NULL;
}

// pass in argv at the start of the pmon spec with argc downadjusted
static int
lex_pmon(pmon* p, uint16_t *hp, int *argc, char ***argv){
  if(*argc < 4){
    std::cerr << "expected 4 arguments, " << *argc << " left" << std::endl;
    return -1;
  }
  const char *spstr = **argv;
  while(isspace(*spstr)){
    ++spstr;
  }
  bool shadow = false;
#define SHADOWSTR "shadow "
  if(!strncasecmp(spstr, SHADOWSTR, strlen(SHADOWSTR))){
    shadow = true;
    spstr += strlen(SHADOWSTR);
  }
  if((p->s.s = lookup_species(spstr)) == NULL){
    std::cerr << "no such species: " << spstr << std::endl;
    return -1;
  }
  if(shadow){
    if(!p->s.s->shadow){ // still allow it, but warn
      std::cerr << "warning: " << spstr << " does not have a shadow form" << std::endl;
    }
  }
  if(lex_ivlevel((*argv)[1], &p->s, shadow)){
    std::cerr << "invalid IV@level in " << (*argv)[1] << std::endl;
    return -1;
  }
  p->fa = species_fast_attack(p->s.s, (*argv)[2]);
  p->ca1 = lex_species_charged_attacks(p->s.s, (*argv)[3], &p->ca2);
  if(!p->fa || !p->ca1){
    fprintf(stderr, "invalid attacks for %s: '%s' '%s'\n", p->s.s->name.c_str(),
            (*argv)[2], (*argv)[3]);
    return -1;
  }
  fill_stats(&p->s, shadow);
  *hp = p->s.mhp;
  (*argv) += 4;
  *argc -= 4;
  return 0;
}
