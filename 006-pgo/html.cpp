#include "html.h"

// it's always [ -3, 2 ], with 0 (neutral) at the fourth column, which must be white
const std::string reversecss[6] = { "teneg3", "teneg2", "teneg1", "tepos0", "tepos1", "tepos2", };
const std::string css[6] = { "tepos3", "tepos2", "tepos1", "tepos0", "teneg1", "teneg2", };

static void
emit_option(std::ostream &fp, const std::string &value){
  fp << "<option value=\"";
  encode_name(fp, value); // this will be used as a URI
  fp << "\">";
  fp << value;
  fp << "</option>";
}

void write_navbar(std::ostream &fp, bool par){
  fp << "<nav><ul>" << std::endl;
  fp << "<li><a href=\"";
  if(par){
    fp << "../";
  }
  fp << "index.html\">main</a></li>" << std::endl;
  fp << "<li><a href=\"";
  if(par){
    fp << "../";
  }
  fp << "mon/index.html\">forms</a></li>" << std::endl;
  fp << "<li class=\"inputgroup\">";
  fp << "<select id=\"formselect\" autofocus>";
  for(auto s = species_begin() ; s != species_end() ; ++s){
    emit_option(fp, s->name);
    for(const auto &m : s->mforms){
      emit_option(fp, m.name);
    }
  }
  fp << "</select>" << std::endl;
  // implemented by javascript following the footer. we also listen for enter there.
  fp << "<button type=\"button\" onclick=\"handlesearch()\">go</button></li>";
  fp << "</ul></nav>" << std::endl;
}

void write_header(std::ostream &fp, const std::string &title, const std::string &style, bool pdir){
  fp << "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  fp << "<link rel=\"stylesheet\" type=\"text/css\" href=\"../pgonav.css\">";
  fp << "<title>" << title << "</title>";
  if(style.length()){
    fp << "<style>" << style << "</style>";
  }
  fp << "</head>" << std::endl;
  fp << "<body>" << std::endl;
  write_navbar(fp, pdir);
}

static void
html_wname(std::ostream &fp, const std::string &wname, bool pdir){
  fp << "<img src=\"";
  if(pdir){
    fp << "../";
  }
  fp << "images/";
  encode_name(fp, wname);
  fp << "-icon.png\" class=\"type\" alt=\""<< wname << "\"/>";
}

void html_weather(std::ostream &fp, pgo_weather_t w, bool pdir){
  html_wname(fp, WNames[w], pdir);
  if(WSNames[w]){
    fp << ' ';
    html_wname(fp, WSNames[w], pdir);
  }
  fp << " (" << WNames[w] << ")";
}

void html_weather(std::ostream &fp, pgo_types_e t1, pgo_types_e t2, bool pdir){
  pgo_weather_t w1, w2;
  if(t2 == TYPECOUNT){
    t2 = t1;
  }
  typing_weather_boosts(t1, t2, w1, w2);
  html_weather(fp, w1, pdir);
  if(w2 != w1){
    fp << ", ";
    html_weather(fp, w2, pdir);
  }
}

void timetofirst::emit(std::ostream &fp, double maxp, bool pdir) const {
  fp << "<tr>";
  fp << "<td>";
  html_types(fp, s->t1, s->t2, pdir);
  fp << "</td>";
  fp << "<td>";
  link_to_name(fp, s->name, true, pdir);
  fp << "</td>";
  fp << "<td>";
  attack_with_icon(fp, *s, fa, true, pdir);
  fp << " → ";
  attack_with_icon(fp, *s, ca, true, pdir);
  fp << "</td>";
  fp << "<td>" << turns << "</td>";
  fp << "<td>" << (dpt * 100 / maxp) << "%</td>";
  fp << "</tr>" << std::endl;
}

void maxtankcandidate::emit(std::ostream &fp, bool useca, bool pdir) const {
  fp << "<tr>";
  fp << "<td>";
  html_types(fp, s.s->t1, s.s->t2, pdir);
  fp << "</td>";
  fp << "<td>";
  link_to_name(fp, s.s->name, true, pdir);
  fp << "</td>";
  fp << "<td>";
  attack_with_icon(fp, *s.s, fa, true, pdir);
  if(useca){
    fp << " → ";
    if(!ca){
      fp << "None";
    }else{
      attack_with_icon(fp, *s.s, ca, true, pdir);
    }
  }
  fp << "</td>";
  fp << "</tr>" << std::endl;
}

void maxcandidate::emit_p0(std::ostream &fp, unsigned maxp, bool unified, bool pdir) const {
  auto rp = powprod();
  fp << "<tr>";
  fp << "<td>";
  html_types(fp, s->t1, s->t2, pdir);
  fp << "</td>";
  fp << "<td>";
  link_to_name(fp, s->name, true, pdir);
  if(!unified){
    fp << " L";
    emit_halflevel_as_level(fp, hlevel);
  }
  fp << "</td>";
  fp << "<td>";
  html_type(fp, atype, pdir);
  fp << ' ';
  // we do not attempt to indicate that a max attack is based on a legacy attack,
  // since it is possible that more than one fast attack enable the same max
  // attack, and they could have different legacy status (this happens with e.g.
  // dynamax machamp, which could have max knuckle from karate chop or counter).
  if(!hasstab){
    fp << "<i>";
  }
  link_to_attack(fp, aname, true, pdir);
  if(!unified){
    fp << " L" << maxlevel;
  }
  if(!hasstab){
    fp << "</i>";
  }
  fp << "</td>";
  fp << std::setprecision(2) << std::fixed << "<td>" << (rp * 100.0 / maxp) << "%</td>"; // relative
  fp << "</tr>" << std::endl;
}

void maxcandidate::emit(std::ostream &fp, unsigned maxp, bool unified, bool pdir) const {
  auto rp = powprod();
  fp << "<tr>";
  fp << "<td>";
  html_types(fp, s->t1, s->t2, pdir);
  fp << "</td>";
  fp << "<td>";
  link_to_name(fp, s->name, true, pdir);
  if(!unified){
    fp << " L";
    emit_halflevel_as_level(fp, hlevel);
  }
  fp << "</td>";
  fp << "<td>";
  html_type(fp, atype, pdir);
  fp << ' ';
  // we do not attempt to indicate that a max attack is based on a legacy attack,
  // since it is possible that more than one fast attack enable the same max
  // attack, and they could have different legacy status (this happens with e.g.
  // dynamax machamp, which could have max knuckle from karate chop or counter).
  if(!hasstab){
    fp << "<i>";
  }
  link_to_attack(fp, aname, true, pdir);
  if(!unified){
    fp << " L" << maxlevel;
  }
  if(!hasstab){
    fp << "</i>";
  }
  fp << "</td>";
  fp << std::setprecision(2) << std::fixed << "<td>" << (rp * 100.0 / maxp) << "%</td>"; // relative
  fp << std::setprecision(0) << std::fixed;
  for(int i = -3 ; i < 3 ; ++i){
    fp << "<td class=\"" << css[i + 3] << "\">" << (rp * type_effectiveness_mult(i)) << "</td>";
  }
  fp << "</tr>" << std::endl;
}

static void
print_buff_html(std::ostream &o, unsigned chance, int buff, const char *sig){
  if(!chance){
    return;
  }
  if(chance != 1000){ // don't print chance if it's 100%
    std::print(o, "{:g}% ", chance / 10.0);
  }
  o << sig;
  if(buff > 0){
    o << "↑";
    if(buff > 1){
      o << buff;
    }
  }else{
    o << "↓";
    if(buff < -1){
      o << -buff;
    }
  }
}

// FIXME ugh duplicates summarize_buffs() from latex code
void summarize_buffs_html(std::ostream &o, const attack &a, const std::string &prefix, const std::string &suffix){
  // need special case A+D as it takes too much space otherwise
  bool printed = false;
  if(a.chance_user_attack && a.chance_user_attack == a.chance_user_defense
      && a.user_attack == a.user_defense){
    printed = true;
    o << prefix;
    print_buff_html(o, a.chance_user_attack, a.user_attack, "A+D");
  }else{
    if(a.chance_user_attack){
      printed = true;
      o << prefix;
      print_buff_html(o, a.chance_user_attack, a.user_attack, "A");
    }
    if(a.chance_user_defense){
      printed = true;
      o << prefix;
      print_buff_html(o, a.chance_user_defense, a.user_defense, "D");
    }
  }
  if(a.chance_opp_attack && a.chance_opp_attack == a.chance_opp_defense
      && a.opp_attack == a.opp_defense){
    if(!printed){
      printed = true;
      o << prefix;
    }
    print_buff_html(o, a.chance_opp_attack, a.opp_attack, "OA+D");
  }else{
    if(a.chance_opp_attack){
      if(!printed){
        printed = true;
        o << prefix;
      }
      print_buff_html(o, a.chance_opp_attack, a.opp_attack, "OA");
    }
    if(a.chance_opp_defense){
      if(!printed){
        printed = true;
        o << prefix;
      }
      print_buff_html(o, a.chance_opp_defense, a.opp_defense, "OD");
    }
  }
  if(printed){
    o << suffix;
  }
}

// find the number of turns necessary to reach e energy
static inline unsigned
turns_until_e(const attack *a, unsigned e){
  return (e + (a->energytrain - 1)) / a->energytrain * a->turns;
}

static void
calctimefandc(std::vector<timetofirst> &ttfs, const species &s, const attack *f, const attack *c){
  unsigned t = turns_until_e(f, -c->energytrain);
  auto fpower = calc_eff_power(s, f);
  auto pfast = t / f->turns * fpower;
  ++t; // account for the charged attack
  auto pcharged = calc_eff_power(s, c);
  ttfs.emplace_back(&s, t, pfast, pcharged, f, c, false);
}

void calctimetos3x3(std::vector<timetofirst> &ttfs, const species &s, bool addreturn){
  for(const auto f : s.attacks){
    if(f->energytrain <= 0){
      continue;
    }
    for(const auto c : s.attacks){
      if(c->energytrain >= 0){
        continue;
      }
      calctimefandc(ttfs, s, f, c);
    }
    if(s.shadow && addreturn){
      calctimefandc(ttfs, s, f, &ATK_Return);
    }
  }
}

static inline unsigned
turns_until_eNx1(const attack *a, unsigned e){
  return (e + (a->energyraid - 1)) / a->energyraid * a->animdur;
}

static void
calctimefandc_counter(std::vector<timetofirst> &ttfs, const species &targ,
                      const species &s, const attack *f, const attack *c){
  if(f->type == TYPECOUNT){
    return; // hidden power ought be expanded into all possible types FIXME
  }
  unsigned t = turns_until_eNx1(f, c->energyraid);
  auto fteffect = targ.type_effectiveness(f);
  auto powerfast = calc_eff_power_nx1(s, f) * fteffect * s.atk;
  float pfast = t / f->animdur * powerfast;
  t += c->animdur; // account for the charged attack
  auto cteffect = targ.type_effectiveness(c);
  auto powercharged = calc_eff_power_nx1(s, c) * cteffect * s.atk;
  ttfs.emplace_back(&s, t, pfast, powercharged, f, c, true);
}

// calculate cycles of s attacking targ for all sets of fast+charged
void calc_raid_counters(std::vector<timetofirst> &ttfs, const species &targ,
                        const species &s, bool addreturn){
  for(const auto f : s.attacks){
    if(!fast_attack_p(*f)){
      continue;
    }
    if(f->energyraid <= 0){
      continue;
    }
    for(const auto c : s.attacks){
      if(!charged_attack_p(*c)){
        continue;
      }
      if(c->energyraid <= 0){
        continue;
      }
      calctimefandc_counter(ttfs, targ, s, f, c);
    }
    if(s.shadow && addreturn){
      calctimefandc_counter(ttfs, targ, s, f, &ATK_Return);
    }
  }
}

// no target typing to consider; calculate as if hitting for neutral damage
static void
calctimefandcNx1(std::vector<timetofirst> &ttfs, const species &s, const attack *f, const attack *c){
  unsigned t = turns_until_eNx1(f, c->energyraid);
  auto fpower = calc_eff_power_nx1(s, f);
  auto pfast = t / f->animdur * fpower;
  t += c->animdur; // account for the charged attack
  auto cpower = calc_eff_power_nx1(s, c);
  ttfs.emplace_back(&s, t, pfast, cpower, f, c, true);
}

void calctimetosNx1(std::vector<timetofirst> &ttfs, const species &s, bool addreturn){
  for(const auto f : s.attacks){
    if(!fast_attack_p(*f)){
      continue;
    }
    if(f->energyraid <= 0){
      continue;
    }
    for(const auto c : s.attacks){
      if(!charged_attack_p(*c)){
        continue;
      }
      if(c->energyraid <= 0){
        continue;
      }
      calctimefandcNx1(ttfs, s, f, c);
    }
    if(s.shadow && addreturn){
      calctimefandcNx1(ttfs, s, f, &ATK_Return);
    }
  }
}

static void
write_footer_scripts(std::ostream &fp, bool par){
  fp << "<script>function handlesearch(){ const selectv = document.getElementById('formselect'); window.location.assign(\"";
  if(par){
    fp << "../";
  }
  fp << "mon/\" + selectv.value + \".html\"); }</script>" << std::endl;
  fp << "<script>";
  fp << "const selecte = document.getElementById('formselect');";
  fp << "selecte.addEventListener(\"keydown\", function(e){";
  fp << "if(e.key == \"Enter\"){ handlesearch(); } })";
  fp << "</script>" << std::endl;
}

void write_footer(std::ostream &fp, bool par){
  fp << "<hr/>" << std::endl;
  fp << "<footer>" << std::endl;
  fp << "<span style=\"float:left;font-size:small\"><a href=\"https://goldandappelpub.com/pgo-quantitative.html\"><i>Pokémon GO: A Quantitative Approach</i></a></span>";
  fp << "<span style=\"float:right;font-size:small\">© 2026 <a href=\"mailto:nickblack@linux.com\">nick black</a></span>" << std::endl;
  fp << "</footer>" << std::endl;
  write_footer_scripts(fp, par);
  fp << "</body></html>" << std::endl;
}

void write_footer_and_close(std::ofstream &fp, bool par){
  write_footer(fp, par);
  fp.close();
  if(fp.rdstate() & std::ofstream::failbit){
    std::cerr << "error writing to file" << std::endl;
    throw std::exception();
  }
}

void typeset::emit_with_pop(std::ostream &fp, bool subdir, bool pdir) const {
  auto totsize = learnpop.size() + learnpopm.size();
  if(totsize == 0){
    return;
  }
  fp << "<tr>";
  emitcore(fp, pdir);
  fp << "<td>";
  bool printed = false;
  for(const auto m : learnpopm){
    if(printed){
      fp << ", ";
    }else{
      printed = true;
    }
    link_to_name(fp, m->name, subdir, pdir);
  }
  for(const auto s : learnpop){
    if(printed){
      fp << ", ";
    }else{
      printed = true;
    }
    link_to_name(fp, s->name, subdir, pdir);
  }
  fp << " (" << totsize << ")</td></tr>" << std::endl;
}

void typeset::emitcore(std::ostream &fp, bool pdir) const {
  fp << "<td>";
  html_types(fp, t0, t1, pdir);
  if(plustype != TYPECOUNT){
      fp << ' ';
      html_type(fp, plustype, pdir);
  }
  fp << "</td>";
  for(unsigned i = 0 ; i < sizeof(totals) / sizeof(*totals) ; ++i){
    fp << "<td>";
    if(totals[i]){
      fp << totals[i];
    }
    fp << "</td>";
  }
  fp << "<td>" << ara << "</td>";
}

void typeset::emit(std::ostream &fp, bool pdir) const {
  fp << "<tr>";
  emitcore(fp, pdir);
  fp << "</tr>" << std::endl;
}

static void
calc_cmp_ranges(std::vector<cmprange> &effas, int cpthresh, int cpbound, const species &s){
  unsigned ivcount;
  auto sets = order_ivs(s, cpbound, statscmp_gmean, &ivcount);
  cmprange crange{s.name, 0, 0};
  bool threshmet = false;
  for(unsigned i = 0 ; i < ivcount ; ++i){
    const auto &st = sets[i];
    if(st.shadow){
      continue;
    }
    if(st.cp < cpthresh){
      continue;
    }
    threshmet = true;
    if(st.effa < crange.effamin || crange.effamin == 0){
      crange.effamin = st.effa;
    }
    if(st.effa > crange.effamax){
      crange.effamax = st.effa;
    }
  }
  if(threshmet){
    effas.push_back(crange);
  }
  delete[] sets;
}

void calc_cmp_graph(std::vector<cmprange> &effas, int cpthresh, int cpbound){
  std::vector<species> megas;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(pvp_banned_p(*s)){
      continue;
    }
    calc_cmp_ranges(effas, cpthresh, cpbound, *s);
    for(const auto &m : s->mforms){
      calc_cmp_ranges(effas, cpthresh, cpbound, {&*s, m});
    }
  }
  std::sort(effas.begin(), effas.end());
}

void emit_cmp_graph(std::ostream &fp, const std::vector<cmprange> &effas){
  fp << "<svg role=\"img\" aria-label=\"CMP chart\"";
  fp << " xmlns=\"http://www.w3.org/2000/svg\"";
  fp << " viewBox=\"0 0 300 " << effas.size() * 20 << "\"";
  fp << " style=\"background-color: #dddddd;\">" << std::endl;
  fp << "<style>";
  fp << ".tlabel { font: 1em serif; }";
  fp << "</style>" << std::endl;
  int i = 0;
  for(const auto &e : effas){
    e.emit(fp, i);
    ++i;
  }
  fp << "</svg>" << std::endl;
}

// add at levels 20, 30, 40, and 50, with ATK IVs of 0 and 15
void add_max_candidate(std::vector<maxcandidate> &cands, const species *s,
                       const std::string &aname, bool gmaxpower, bool stab,
                       pgo_types_e atype, float teffect, bool unified){
  //const unsigned lowiv = 10;
  const unsigned highiv = 15;
  //cands.emplace_back(s, aname, MAX_HALFLEVEL_BASIC, gmaxpower, stab, lowiv); // level 50
  cands.emplace_back(s, aname, MAX_HALFLEVEL_BASIC, gmaxpower, stab, highiv, teffect, atype, 3); // level 50, max level 3
  /*cands.emplace_back(s, aname, 79, gmaxpower, stab, lowiv); // level 40
  cands.emplace_back(s, aname, 79, gmaxpower, stab, highiv); // level 40
  cands.emplace_back(s, aname, 59, gmaxpower, stab, lowiv); // level 30
  cands.emplace_back(s, aname, 59, gmaxpower, stab, highiv); // level 30
  cands.emplace_back(s, aname, 39, gmaxpower, stab, lowiv); // level 20*/
  if(!unified){
    cands.emplace_back(s, aname, MAX_HALFLEVEL_BASIC, gmaxpower, stab, highiv, teffect, atype, 1); // level 50, max level 1
    cands.emplace_back(s, aname, 39, gmaxpower, stab, highiv, teffect, atype, 3); // level 20, max level 3
  }
}

void add_max_tank_candidate(std::vector<maxtankcandidate> &cands, const stats &st, unsigned turns,
                            double dinf, double dsuf, const attack *fa, const attack *ca){
  cands.emplace_back(st, turns, dinf, dsuf, fa, ca);
}

void add_raid_candidate(std::vector<timetofirst> &cands, const species *s, unsigned turns,
                        float powerfast, float powercharged,
                        const attack *fa, const attack *ca){
  cands.emplace_back(s, turns, powerfast, powercharged, fa, ca, true);
}

static unsigned
max_meter_numerator(int tier){
  switch(tier){
    case 1:
    case 2:
    case 3:
    case 4:
      return 200;
    case 5: return 400;
    case 6: return 3000;
    default: break;
  }
  // FIXME we only use this because we're assuming tier 5 stats for max battles
  // without a known tier in max_tank_calc_noca(); we ought throw an exception
  return 400;
}

// calculate the number of max meter slots removed by attack a thrown by
// sa against sb. one charge is removed for every 0.5% (tier 1-4 max) /
// 0.25% (tier 5 max) / 0.033% (gmax) of the opponent's MHP, rounded down, with
// a minimum of one (so i'm told https://www.reddit.com/r/TheSilphRoad/s/KhWNrLAUwd).
static unsigned
max_meter_slots(const stats &sa, const species &sb, const attack *a,
                double &dinf, unsigned mhp){
  float teffect;
  if(a->type == TYPECOUNT){ // hidden power strikes again, treat as normal bleh
    teffect = type_effectiveness(TYPE_NORMAL, sb.t1, sb.t2);
  }else{
    teffect = sb.type_effectiveness(a);
  }
  dinf = calc_eff_power_nx1(*sa.s, a) * teffect;
  dinf *= sa.effa;
  dinf /= calc_eff_d(sb.def + MAXIVELEM, MAX_HALFLEVEL_BASIC, false);
  dinf = trunc(dinf);
  if(dinf == 0){
    dinf = 1;
  }
  auto slots = (dinf * max_meter_numerator(sb.dmax)) / mhp;
  if(slots < 1){
    slots = 1;
  }
  return slots;
}

constexpr unsigned MAXMETERSIZE = 100;

// calculate how many turns are required to reach the max phase (or win) using
// form sa employing attacks fa and ca against max battle opponent sb. this
// value is always positive. writes damage inflicted and suffered to dinf and
// dsub, which might exceed the actual hit points available. dsuf might be
// zero. dinf is always positive. for now, we assume level 50 and perfect IVs
// on both sides.
static unsigned
max_tank_calc(const stats &sa, const species &sb, const attack *fa,
              const attack *ca, double &dinf){
  auto mhp = max_battle_mhp(sb);
  unsigned turns = 0;
  unsigned used = 0;  // slots built up
  double pow;
  auto slots = max_meter_slots(sa, sb, fa, pow, mhp);
  double capow;
  auto caslots = max_meter_slots(sa, sb, ca, capow, mhp);
  unsigned energy = 0;
  dinf = 0;
  while(used < MAXMETERSIZE && dinf < mhp){
    // we can throw a charged attack after caturns
    auto caturns = turns_until_eNx1(fa, ca->energyraid - energy);
    turns += caturns;
    auto faiter = caturns / fa->animdur;
    used += slots * faiter;
    dinf += faiter * pow;
    energy += faiter * fa->energyraid;
    if(used >= MAXMETERSIZE || dinf >= mhp){
      break;
    }
    energy -= ca->energyraid;
    turns += ca->animdur;
    used += caslots;
    dinf += capow;
  }
  if(dinf == 0){ // handle splash, yawn, etc.
    dinf = MAXMETERSIZE;
  }
  return turns;
}

static unsigned
max_tank_calc_noca(const stats &sa, const species &sb, const attack *fa, double &dinf){
  dinf = 0;
  unsigned mhp;
  if(sb.dmax != UINT_MAX){
    mhp = max_battle_mhp(sb);
  }else{
    // we need an mhp to calculate slots for the fast attack. assume tier 5
    // for now but this is terrible FIXME
    mhp = MAXBATTLE_TIER5_MHP;
  }
  unsigned turns = 0;
  double pow;
  auto slots = max_meter_slots(sa, sb, fa, pow, mhp);
  turns = (MAXMETERSIZE + slots - 1) / slots * fa->animdur;
  dinf = MAXMETERSIZE * pow;
  // FIXME handle early knockout case (dinf >> mhp)
  if(dinf == 0){ // handle splash, yawn, etc.
    dinf = MAXMETERSIZE;
  }
  return turns;
}

// build up candidates for species sa attacking species sb as tank in max battle
static void
max_tank_counters_species(const species &sa, const species &sb,
                          std::vector<maxtankcandidate>& cands,
                          bool useca){
  struct stats st{&sa, MAX_HALFLEVEL_BASIC, MAXIVELEM, MAXIVELEM, MAXIVELEM, false};
  if(!has_max_attack(sa)){
    return;
  }
  double dsuf = 0;
  // for now, we compute damage suffered independent of the opponent's attack,
  // just assuming one instance of each charged attack, weighted by its power
  // and typing effectiveness. FIXME
  for(const auto *sbca : sb.attacks){
    if(!charged_attack_p(*sbca)){
      continue;
    }
    dsuf += calc_eff_power_nx1(sb, sbca) * sa.type_effectiveness(sbca);
  }
  // now divide dsuf by our bulk
  dsuf /= (st.mhp * st.effd);
  for(const auto fa : sa.attacks){
    if(fast_attack_p(*fa)){
      double dinf;
      auto turns = max_tank_calc_noca(st, sb, fa, dinf);
      add_max_tank_candidate(cands, st, turns, dinf, dsuf, fa, nullptr);
      if(useca){
        for(const auto ca : sa.attacks){
          if(charged_attack_p(*ca)){
            turns = max_tank_calc(st, sb, fa, ca, dinf);
            add_max_tank_candidate(cands, st, turns, dinf, dsuf, fa, ca);
          }
        }
      }
    }
  }
}

// build up candidates for the given species attacking something having typing
// ttype1+ttype2
static void
max_counters_species(const species &s, pgo_types_e ttype1, pgo_types_e ttype2,
                     std::vector<maxcandidate>& cands, std::forward_list<species> &store){
  if(has_dmax(s)){
    bool ts[TYPECOUNT]{};
    for(const auto a : s.attacks){
      if(fast_attack_p(*a)){
        auto at = dmax_attack_type(a);
        if(ts[at]){
          continue;
        }
        ts[at] = true;
        bool stab = has_stab_raw_p(s, at);
        auto teffect = type_effectiveness(at, ttype1, ttype2);
        add_max_candidate(cands, &s, max_attack_name(at), false, stab, at, teffect, true);
      }
    }
  }
  auto gma = lookup_gmax_attack(s);
  if(gma){
    bool stab = has_stab_raw_p(s, gma->type);
    float teffect = type_effectiveness(gma->type, ttype1, ttype2);
    auto &smeg = store.emplace_front(&s, *gma);
    add_max_candidate(cands, &smeg, gma->name, true, stab, gma->type, teffect, true);
  }
}

constexpr unsigned COUNTEREMITMAX = 50;

int raid_counters(std::ostream &fp, const species &targ, bool pdir){
  std::vector<timetofirst> cands;
  std::forward_list<species> store;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(!unique_mechanics(s->name).empty()){
      continue; // Pokémon with unique mechanics can't participate in max battles
    }
    calc_raid_counters(cands, targ, *s, s->shadow);
    for(const auto &m : s->mforms){
      store.emplace_front(&*s, m);
    }
    if(s->shadow){
      auto &sh = store.emplace_front(*s);
      sh.atk = (sh.atk * 6.0) / 5;
      sh.def = (sh.def * 5.0) / 6;
      sh.name = "Shadow " + sh.name;
    }
  }
  for(const auto &s : store){
    calc_raid_counters(cands, targ, s, s.shadow);
  }
  // FIXME need to at least consider defense
  std::sort(cands.begin(), cands.end(), [](const timetofirst &l, const timetofirst& r) -> bool {
      if(l.dpt < r.dpt){
        return false; // most to least powerful
      }else if(r.dpt < l.dpt){
        return true;
      }
      if(l.turns < r.turns){
        return true; // fastest to slowest
      }else if(r.turns < l.turns){
        return false;
      }
      if(l.s->idx < r.s->idx){
        return true;
      }
      return false;
      });
  fp << "<table class=\"evenshade raidcounters\">";
  fp << "<tr>";
  fp << "<th/>";
  fp << "<th>Form</th>";
  fp << "<th>Attack pair</th>";
  fp << "<th>Turns</th>";
  fp << "<th>RelPPT</th>";
  fp << "</tr>";
  auto maxp = cands.begin()->dpt;
  unsigned emitted = 0;
  for(const auto& c : cands){
    c.emit(fp, maxp, pdir);
    if(++emitted == COUNTEREMITMAX){
      break;
    }
  }
  fp << "</table>";
  return 0;
}

// determine the best tanks to use against this Max Battle opponent
int max_tank_counters(std::ostream &fp, const species &s, bool pdir){
  // without a dmax tier, we don't know how many hit points the opponent has.
  // without knowing that, we can't determine whether charged attacks are
  // useful. so leave them out in that case.
  bool useca = s.dmax != UINT_MAX;
  std::vector<maxtankcandidate> cands;
  for(auto a = species_begin() ; a != species_end() ; ++a){
    if(!unique_mechanics(a->name).empty()){
      continue; // Pokémon with unique mechanics can't participate in max battles
    }
    // g-max makes no difference for tanks (yet)
    max_tank_counters_species(*a, s, cands, useca);
  }
  std::sort(cands.rbegin(), cands.rend());
  fp << "<table class=\"evenshade maxtankcounters\">";
  fp << "<tr>";
  fp << "<th/>";
  fp << "<th>Form</th>";
  if(useca){
    fp << "<th>Attack pair</th>";
  }else{
    fp << "<th>Fast attack</th>";
  }
  fp << "</tr>";
  unsigned emitted = 0;
  for(const auto& c : cands){
    c.emit(fp, useca, pdir);
    if(++emitted == COUNTEREMITMAX){
      break;
    }
  }
  fp << "</table>";
  return 0;
}

// determine the best Max Attackers to use against this Max Battle opponent
int max_counters(std::ostream &fp, const species &s, bool pdir){
  std::forward_list<species> store;
  std::vector<maxcandidate> cands;
  for(auto a = species_begin() ; a != species_end() ; ++a){
    // handles the g-max form, if one exists
    max_counters_species(*a, s.t1, s.t2, cands, store);
  }
  float teffect = type_effectiveness(TYPE_STEEL, s.t1, s.t2);
  const auto zac = lookup_species("Zacian Crowned Sword");
  if(!zac){
    return -1;
  }
  add_max_candidate(cands, zac, "Behemoth Blade", false, true, TYPE_STEEL, teffect, true);
  const auto zam = lookup_species("Zamazenta Crowned Shield");
  if(!zam){
    return -1;
  }
  add_max_candidate(cands, zam, "Behemoth Bash", false, true, TYPE_STEEL, teffect, true);
  const auto e = lookup_species("Eternatus");
  if(!e){
    return -1;
  }
  teffect = type_effectiveness(TYPE_DRAGON, s.t1, s.t2);
  add_max_candidate(cands, e, "Dynamax Cannon", true, true, TYPE_DRAGON, teffect, true);
  std::sort(cands.begin(), cands.end(), std::greater<>());
  fp << "<table class=\"evenshade maxcounters\">";
  fp << "<tr>";
  fp << "<th>T</th>";
  fp << "<th>Form</th>";
  fp << "<th>Attack</th>";
  fp << "<th>Relative</th>";
  fp << "</tr>";
  auto maxp = cands.begin()->powprod();
  unsigned emitted = 0;
  for(const auto& c : cands){
    c.emit_p0(fp, maxp, true, pdir);
    if(++emitted == COUNTEREMITMAX){
      break;
    }
  }
  fp << "</table>";
  return 0;
}

void species_with_icons(std::ostream &fp, const species &s, bool closetd, bool pdir){
  html_types(fp, s.t1, s.t2, pdir);
  if(closetd){
    fp << "</td><td>";
  }else{
    fp << ' ';
  }
  link_to_name(fp, s.name, true, pdir);
}

void attack_with_icon(std::ostream &fp, const species &s, const attack *a,
                      bool subdir, bool pdir){
  html_type(fp, a->type, pdir);
  fp << "&nbsp;";
  emit_html_attack(fp, s, a, subdir, pdir);
}

std::string unique_mechanics(const std::string& name){
    if(name == "Minior"){
      return
 "Minior has two battle forms, Meteor and Core. "
 "It starts battle in the Meteor form, with higher defense than attack. "
 "Upon reduction to less than half its MHP, it enters Core form, with greater attack than defense. "
 "This form is persistent across swaps (and indeed across battles). "
 "Cramorant cannot defend gyms, assault gyms, or participate in raids.";
    }else if(name == "Cramorant"){
      return
 "When in its default form, use of either the <a href=\"../attacks/Surf.html\">Surf</a> or <a href=\"../attacks/Dive.html\">Dive</a> charged attack changes Cramorant's form. "
 "If used with at least half its HP remaining, Carmorant enters its Gulping form, with an <a href=\"Arrokuda.html\">Arrokuda</a> in its bill. "
 "It otherwise enters its Gorging form, with a <a href=\"Pikachu.html\">Pikachu</a>. "
 "Upon being hit with a charged move (a shielded move does not count), Cramorant will spit its prey at its opponent. "
 "This counterattack takes place even if Cramorant is knocked out by the charged attack, taking place "
  "“simultaneously”—if Cramorant is the last standing member of a team, the counterattack can still take "
  "place, knocking out the attacker to effect a tie. "
 "The Gulping form's Arrokuda debuffs the opponent defense by one step. "
 "The Gorging form's Pikachu debuffs the opponent's attack by two steps. "
 "In both cases, type-agnostic damage is dealt equivalent to 15% of the opponent's MHP, and Cramorant returns to default form (or faints). "
 "If Cramorant is knocked out by a fast attack (or by a shielded charged move), the counterattack does not occur. "
 "An alternate form is not changed or reset by a subsequent charged attack, but is reset upon swapping Cramorant out.";
    }else if(name == "Mimikyu"){
      return
 "Mimikyu is shrouded in a rag, and this rag has the ability to absorb one charged attack, just like a shield. "
 "When a charged attack is thrown at Mimikyu, the Trainer can choose to use a shield if any are available. "
 "If the move is not shielded, Mimikyu loses its disguise (“Busted Form”). "
 "This results in a second-level debuff of Mimikyu's defense. "
 "This debuff cannot be cleared by switching Pokémon.";
    }else if(name == "Morpeko"){
      return
 "Morpeko changes its form between “Full Belly Mode” and “Hangry Mode” each time it uses a charged attack. "
 "Upon becoming active, Morpeko is in Full Belly Mode, and Aura Wheel is an Electric attack. "
 "In Hangry Mode, Aura Wheel is a Dark attack. "
 "Morpeko retains Dark/Electric typing (and thus Aura Wheel STAB) across modes. "
 "Morpeko cannot defend gyms, assault gyms, or participate in raids.";
    }else if(name == "Aegislash"){
      return
 "Aegislash has Blade and Shield forms which switch the other's ATK and DEF stats. "
 "Outside of battle, it is always in the Shield form (97 ATK, 272 DEF). "
 "While in Shield form, its fast attacks have zero power (and thus inflict a single point of damage). "
 "Immediately before throwing a charged move, it takes on its Blade form. "
 "Upon using a shield, or being substituted, it reverts to Shield form. "
 "Since its Shield form (considered for League CP bounds) has much lower CP, this allows a high ATK to be snuck under a CP bound (though it will only be effective for charged attacks). "
 "Aegislash cannot defend gyms, assault gyms, or participate in raids.";
    }
    return "";
}
