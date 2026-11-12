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
  fp << "</option>" << std::endl;
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
  fp << "<select id=\"formselect\">";
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

void candidate::emit_p0(std::ostream &fp, unsigned maxp, bool unified, bool pdir) const {
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

void candidate::emit(std::ostream &fp, unsigned maxp, bool unified, bool pdir) const {
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
  float power = calc_eff_power(s, f);
  float pfast = t / f->turns * power;
  ++t; // account for the charged attack
  ttfs.emplace_back(&s, t, pfast, f, c, false);
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
calctimefandcNx1(std::vector<timetofirst> &ttfs, const species &s, const attack *f, const attack *c){
  unsigned t = turns_until_eNx1(f, c->energyraid);
  float power = calc_eff_power_nx1(s, f);
  float pfast = t / f->animdur * power;
  t += c->animdur; // account for the charged attack
  ttfs.emplace_back(&s, t, pfast, f, c, true);
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
  if(!learnpop.size() && !learnpopm.size()){
    return;
  }
  fp << "<tr>";
  emitcore(fp, pdir);
  if(learnpopm.size()){
    fp << "<td>" << learnpopm.size() << " ";
    bool printed = false;
    for(const auto m : learnpopm){
      if(printed){
        fp << ", ";
      }else{
        printed = true;
      }
      link_to_name(fp, m->name, subdir, pdir);
    }
  }else{
    fp << "<td>" << learnpop.size() << " ";
    bool printed = false;
    for(const auto s : learnpop){
      if(printed){
        fp << ", ";
      }else{
        printed = true;
      }
      link_to_name(fp, s->name, subdir, pdir);
    }
  }
  fp << "</td></tr>";
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
void add_candidate(std::vector<candidate>& cands, const species* s,
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

// build up candidates for the given species attacking something having typing
// ttype1+ttype2
static void
max_counters_species(const species &s, pgo_types_e ttype1, pgo_types_e ttype2,
                     std::vector<candidate>& cands){
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
        add_candidate(cands, &s, max_attack_name(at), false, stab, at, teffect, true);
      }
    }
  }
  auto gma = lookup_gmax_attack(s);
  if(gma){
    bool stab = has_stab_raw_p(s, gma->type);
    float teffect = type_effectiveness(gma->type, ttype1, ttype2);
    add_candidate(cands, &s, gma->name, true, stab, gma->type, teffect, true);
  }
}

int max_counters(std::ostream &fp, const species &s, bool pdir){
  constexpr unsigned EMITMAX = 50;
  std::vector<candidate> cands;
  for(auto a = species_begin() ; a != species_end() ; ++a){
    max_counters_species(*a, s.t1, s.t2, cands);
  }
  float teffect = type_effectiveness(TYPE_STEEL, s.t1, s.t2);
  const auto zac = lookup_species("Zacian Crowned Sword");
  if(!zac){
    return -1;
  }
  add_candidate(cands, zac, "Behemoth Blade", false, true, TYPE_STEEL, teffect, true);
  const auto zam = lookup_species("Zamazenta Crowned Shield");
  if(!zam){
    return -1;
  }
  add_candidate(cands, zam, "Behemoth Bash", false, true, TYPE_STEEL, teffect, true);
  const auto e = lookup_species("Eternatus");
  if(!e){
    return -1;
  }
  teffect = type_effectiveness(TYPE_DRAGON, s.t1, s.t2);
  add_candidate(cands, e, "Dynamax Cannon", true, true, TYPE_DRAGON, teffect, true);
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
    if(++emitted == EMITMAX){
      break;
    }
  }
  fp << "</table>";
  return 0;
}
