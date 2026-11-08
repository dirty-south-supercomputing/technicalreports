#include "html.h"
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <algorithm>

static std::vector<cmprange> cmpsgl, cmpsul, cmpsml;

static void
write_iv_graphrefs(std::ostream &fp, const species &s, const char *leagueshort){
  std::string enc;
  if(s.name.starts_with("G-Max")){
    encode_name(s.name.substr(5), enc);
  }else{
    encode_name(s.name, enc);
  }
  fp << "<img class=\"monscatter\" src=\"graphs/" << enc << leagueshort << ".png\" alt=\"bulk vs effa plot for " << leagueshort << "\"/>";
  fp << "<img class=\"monscatter\" src=\"graphs/" << enc << leagueshort << "Gmean.png\" alt=\"bulk vs gmean plot for " << leagueshort << "\"/>";
  //fp << "<img class=\"monradar\" src=\"graphs/" << enc << leagueshort << "Radar.png\" alt=\"radarplot for " << leagueshort << "\"/>";
}

struct tristats {
  double min, max, tot;
};

static void
update_tristats(tristats &ts, float s){
  if(!ts.min || ts.min > s){
    ts.min = s;
  }
  if(ts.max < s){
    ts.max = s;
  }
  ts.tot += s;
}

static void
write_iv_summary(std::ostream &fp, const struct stats *sets, unsigned ivcount){
  struct tristats gmean{}, amean{}, effa{}, effd{}, mhp{};
  unsigned lmin = UINT_MAX, lmax = 0;
  unsigned count = 0;
  for(unsigned i = 0 ; i < ivcount ; ++i){
    const auto &st = sets[i];
    if(st.shadow){
      continue;
    }
    if(lmax < st.hlevel){
      lmax = st.hlevel;
    }
    if(lmin > st.hlevel){
      lmin = st.hlevel;
    }
    update_tristats(gmean, st.geommean);
    update_tristats(amean, st.average);
    update_tristats(effa, st.effa);
    update_tristats(effd, st.effd);
    update_tristats(mhp, st.mhp);
    ++count;
  }
  fp << std::fixed << std::setprecision(2);
  fp << "<table class=\"monleaguestats\">" << std::endl;
  fp << "<tr><th>Measure</th><th>Min</th><th>Mean</th><th>Max</th></tr>" << std::endl;
  fp << "<tr><td>Gmean</td><td>" << gmean.min << "</td><td>" << gmean.tot / count << "</td><td>" << gmean.max << "</td></tr>" << std::endl;
  fp << "<tr><td>Amean</td><td>" << amean.min << "</td><td>" << amean.tot / count << "</td><td>" << amean.max << "</td></tr>" << std::endl;
  fp << "<tr><td>Eff<sub>A</sub></td><td>" << effa.min << "</td><td>" << effa.tot / count << "</td><td>" << effa.max << "</td></tr>" << std::endl;
  fp << "<tr><td>Eff<sub>D</sub></td><td>" << effd.min << "</td><td>" << effd.tot / count << "</td><td>" << effd.max << "</td></tr>" << std::endl;
  fp << std::defaultfloat << std::setprecision(5);
  fp << "<tr><td>HP</td><td>" << mhp.min << "</td><td>" << mhp.tot / count << "</td><td>" << mhp.max << "</td></tr>" << std::endl;
  fp << "<tr><td>Level</td><td>";
  emit_halflevel_as_level(fp, lmin);
  fp << "</td><td>n/a</td><td>";
  emit_halflevel_as_level(fp, lmax);
  fp << "</td></tr>" << std::endl;
  fp << "</table>" << std::endl;
}

// returns true if we ought bother emitting the next highest table, false otherwise
// (we return true if any cp in the solution set equals the cpbound).
static bool
write_iv_table(std::ostream &fp, const species &s, const char *league,
               const char *leagueshort, int cpbound,
               const std::vector<cmprange> &cmps){
  unsigned ivcount;
  auto sets = order_ivs(s, cpbound, statscmp_sprod, &ivcount);
  fp << "<h2>" << league << "</h2>" << std::endl;
  write_iv_graphrefs(fp, s, leagueshort);
  fp << "<h3>IVs ordered by geometric mean</h3>" << std::endl;
  write_iv_summary(fp, sets, ivcount);
  fp << "<details class=\"monivcontainer\"><summary>expand table</summary>" << std::endl;
  fp << "<table class=\"evenshade monivs\">" << std::endl;
  fp << "<tr>";
  fp << "<th>#</th><th>IVs</th><th>L</th><th>CP</th>";
  fp << "<th>Eff<sub>A</sub></th>";
  fp << "<th>Eff<sub>D</sub></th>";
  fp << "<th>HP</th>";
  fp << "<th>Gmean</th>";
  fp << "<th>%</th>";
  fp << "</tr>";
  auto gmax = sets[ivcount - 1].geommean;
  double sp = 0;
  unsigned grank = ivcount;
  int id = 1;
  bool ret = false;
  // FIXME we want ties in a single table row
  do{
    const stats &s = sets[--grank];
    if(!s.shadow){
      fp << std::fixed << std::setprecision(2);
      fp << "<tr>";
      if(s.statprod() == sp){
        fp << "<td></td>";
      }else{
        fp << "<td>" << id << "</td>";
        sp = s.statprod();
      }
      ++id;
      fp << "<td>" << s.ia << '-' << s.id << '-' << s.is << "</td>";
      fp << "<td>";
      emit_halflevel_as_level(fp, s.hlevel);
      fp << "</td>";
      fp << "<td>" << s.cp << "</td>";
      if(s.cp == cpbound){
        ret = true;
      }
      fp << "<td>" << s.effa << "</td>";
      fp << "<td>" << s.effd << "</td>";
      fp << "<td>" << s.mhp << "</td>";
      fp << "<td>" << s.geommean << "</td>";
      std::setprecision(1);
      fp << "<td>" << (s.geommean * 100 / gmax) << "</td>";
      fp << "</tr>";
    }
  }while(grank);
  fp << "</table></details>" << std::endl;
  delete[] sets;
  //fp << "<img class=\"cmpgraph\" type=\"image/svg+xml\" width=\"100\" heigh=\"800\" src=\"graphs/cmp" << leagueshort << ".svg\"/>";
  return ret;
}

static int
write_cmp_graph(const std::string &leagueshort, const std::vector<cmprange> &cmps){
  std::string fname = "graphs/cmp" + leagueshort + ".svg";
  std::ofstream fp(fname);
  if(!fp.is_open()){
    std::cerr << "error opening " << fname << " for writing" << std::endl;
    return -1;
  }
  emit_cmp_graph(fp, cmps);
  fp.close();
  if(fp.rdstate() & std::ofstream::failbit){
    std::cerr << "error writing to file" << std::endl;
    return -1;
  }
  return 0;
}

static void
write_iv_tables(std::ostream &fp, const species &s){
  fp << "<div id=\"glivs\">";
  bool donext = write_iv_table(fp, s, "great league", "GL", GLCPCAP, cmpsgl);
  fp << "</div>" << std::endl;
  if(donext){
    fp << "<div id=\"ulivs\">";
    donext = write_iv_table(fp, s, "ultra league", "UL", ULCPCAP, cmpsul);
    fp << "</div>" << std::endl;
  }
  if(donext){
    fp << "<div id=\"mlivs\">";
    donext = write_iv_table(fp, s, "master league", "ML", 0, cmpsml);
    fp << "</div>" << std::endl;
  }
}

static void
write_fattacks_3x3(std::ostream &fp, const species &s, const std::vector<const attack*> &sortedatks){
  fp << "<table class=\"evenshade fastattacks\">" << std::endl;
  fp << "<tr><th>Attack</th><th>Turns</th><th>Power</th><th>Energy+</th><th>PPT</th><th>EPT</th></tr>" << std::endl;
  for(const auto *a : sortedatks){
    if(!fast_attack_p(*a)){
      continue;
    }
    auto cpow = calc_eff_power(s, a);
    fp << "<tr>";
    fp << "<td>";
    html_type(fp, a->type, true);
    std::string prefix, suffix;
    emit_html_attack_fancy(fp, s, a, prefix, suffix, true, true);
    fp << "</td>";
    fp << "<td>";
    if(a->turns){
      fp << prefix << a->turns << suffix;
    }
    fp << "</td>";
    fp << "<td>" << prefix << cpow << suffix << "</td>";
    if(charged_attack_p(*a)){
      fp << "<td>" << prefix << -a->energytrain << suffix << "</td>";
    }else{
      fp << "<td>" << prefix << a->energytrain << suffix << "</td>";
    }
    fp << "<td>" << prefix << (cpow / a->turns) << suffix << "</td>";
    fp << "<td>" << prefix << (static_cast<float>(a->energytrain) / a->turns) << suffix << "</td>";
    fp << "</tr>" << std::endl;
  }
  fp << "</table>" << std::endl;
}

static void
write_cattacks_3x3(std::ostream &fp, const species &s, const std::vector<const attack*> &sortedatks){
  fp << "<table class=\"evenshade chargedattacks\">" << std::endl;
  fp << "<tr><th>Attack</th><th>Buffs</th><th>Power</th><th>Energy-</th><th>PPE</th></tr>" << std::endl;
  for(const auto *a : sortedatks){
    if(!charged_attack_p(*a)){
      continue;
    }
    auto cpow = calc_eff_power(s, a);
    fp << "<tr>";
    fp << "<td>";
    html_type(fp, a->type, true);
    std::string prefix, suffix;
    emit_html_attack_fancy(fp, s, a, prefix, suffix, true, true);
    fp << "</td><td>";
    summarize_buffs_html(fp, *a, prefix, suffix);
    fp << "</td><td>" << prefix << cpow << suffix << "</td>";
    fp << "<td>" << prefix << -a->energytrain << suffix << "</td>";
    fp << "<td>" << prefix << (cpow / -a->energytrain) << suffix << "</td>";
    fp << "</tr>" << std::endl;
  }
  fp << "</table>" << std::endl;
}

// don't elide matching mon type for html (as we do latex)
static void emit_row(std::ofstream &fp, const timetofirst &t){
  fp << "<tr>";
  fp << "<td>";
  fp << (t.turns - 1) / t.fa->turns << "x ";
  html_type(fp, t.fa->type, true);
  fp << ' ';
  emit_html_attack(fp, *t.s, t.fa, true, true);
  fp << " + ";
  html_type(fp, t.ca->type, true);
  fp << ' ';
  std::string prefix, suffix;
  emit_html_attack_fancy(fp, *t.s, t.ca, prefix, suffix, true, true);
  fp << ' ';
  summarize_buffs_html(fp, *t.ca, prefix, suffix);
  fp << "</td>";
  fp << "<td>" << t.turns << "</td>";
  fp << "<td>" << t.dam << "</td>";
  fp << "<td>";
  if(t.excesse){
    fp << t.excesse;
  }
  fp << "</td>";
  fp << "<td>" << t.dpt << "</td>";
  fp << "<td>" << (t.powercharged * 100 / t.dam) << "</td>";
  fp << "</tr>" << std::endl;
}

static void
write_mon_ttf_3x3(std::ofstream &fp, const species &s){
  std::vector<timetofirst> ttfs;
  calctimetos3x3(ttfs, s, true);
  std::sort(ttfs.begin(), ttfs.end());
  fp << "<table class=\"evenshade apairs\">" << std::endl;
  fp << "<tr><th>Attack sequence</th><th>Turns</th><th>Power</th><th><i>e</i></th><th>PPT</th><th>%c</th></tr>" << std::endl;
  for(const auto &t : ttfs){
    emit_row(fp, t);
  }
  fp << "</table>" << std::endl;
}

static void
add_tset_if_new(std::vector<typeset> &tsets, pgo_types_e t0, pgo_types_e t1, pgo_types_e plust){
  for(const auto &t : tsets){
    if((t.t0 == t0 && t.t1 == t1) || (t.t1 == t0 && t.t0 == t1)){
      if(t.plustype == plust){
        return;
      }
    }
  }
  if(plust != TYPECOUNT){
    tsets.emplace_back(t0, t1, TYPECOUNT);
  }
  tsets.emplace_back(t0, t1, plust);
}


static void
calcarass3x3(std::vector<typeset> &tsets, const species &s){
  pgo_types_e plusatk = TYPECOUNT;
  for(const auto a : s.attacks){
    if(plus_attack_p(*a)){
      plusatk = a->type;
      break;
    }
  }
  std::vector<const attack *> atks{s.attacks};
  if(s.shadow){
    atks.emplace_back(&ATK_Return);
  }
  for(const auto c1 : atks){
    if(!charged_attack_p(*c1)){
      continue;
    }
    // c1 is a charged attack
    for(const auto c2 : s.attacks){
      if(!charged_attack_p(*c2)){
        continue;
      }
      // c2 is a charged attack
      if(c1->type == c2->type){
        continue;
      }
      add_tset_if_new(tsets, c1->type, c2->type, plusatk);
    }
    add_tset_if_new(tsets, c1->type, c1->type, plusatk);
  }
}

// show, for each pair of charged attacks, the ARA and possible type effectiveness
static void
write_mon_apairs(std::ofstream &fp, const species &s){
  fp << "<div id=\"asets\">";
  fp << "<h2>charged attack set effectiveness</h2>" << std::endl;
  std::vector<typeset> tsets;
  calcarass3x3(tsets, s);
  std::sort(tsets.begin(), tsets.end(), std::greater<typeset>());
  fp << "<table class=\"evenshade apairs\">" << std::endl;
  fp << "<tr><th>Attack set</th>";
  for(int i = -3 ; i < 3 ; ++i){
    fp << "<th>E<sub>" << i << "</sub></th>";
  }
  fp << "<th>ARA</th></tr>" << std::endl;
  for(const auto &t : tsets){
    t.emit(fp, true);
  }
  fp << "</table>" << std::endl;
  fp << "</div>" << std::endl;
}

static void emit_row_nx1(std::ofstream &fp, const timetofirst &t){
  fp << "<tr>";
  fp << "<td>";
  fp << (t.turns - 1) / t.fa->animdur << "x ";
  html_type(fp, t.fa->type, true);
  fp << ' ';
  emit_html_attack(fp, *t.s, t.fa, true, true);
  fp << " + ";
  html_type(fp, t.ca->type, true);
  fp << ' ';
  std::string prefix, suffix;
  emit_html_attack_fancy(fp, *t.s, t.ca, prefix, suffix, true, true);
  fp << "</td>";
  fp << "<td>" << t.turns << "</td>";
  fp << "<td>" << t.dam << "</td>";
  fp << "<td>";
  if(t.excesse){
    fp << t.excesse;
  }
  fp << "</td>";
  fp << "<td>" << t.dpt << "</td>";
  fp << "<td>" << (t.powercharged * 100 / t.dam) << "</td>";
  fp << "</tr>" << std::endl;
}

static void
write_mon_ttf_Nx1(std::ofstream &fp, const species &s){
  std::vector<timetofirst> ttfs;
  calctimetosNx1(ttfs, s, true);
  if(ttfs.size() == 0){
    return; // don't print empty table
  }
  std::sort(ttfs.begin(), ttfs.end());
  fp << "<table class=\"evenshade apairs\">" << std::endl;
  fp << "<tr><th>Attack sequence</th><th>Turns</th><th>Power</th><th><i>e</i></th><th>PPT</th><th>%c</th></tr>" << std::endl;
  for(const auto &t : ttfs){
    emit_row_nx1(fp, t);
  }
  fp << "</table>" << std::endl;
}

static void
write_mon_attacks_3x3(std::ofstream &fp, const species &s, std::vector<const attack *> &atks){
  std::sort(atks.begin(), atks.end(), [s](const attack *lhs, const attack *rhs){
      if(fast_attack_p(*lhs) && !fast_attack_p(*rhs)){
        return true;
      }else if(fast_attack_p(*rhs) && !fast_attack_p(*lhs)){
        return false;
      }
      float aratio = calc_eff_power(s, lhs) * 100;
      float bratio = calc_eff_power(s, rhs) * 100;
      // next, compare ppt for fast moves and ppe for charged moves
      if(fast_attack_p(*lhs)){
        aratio /= lhs->turns; // ppt
        bratio /= rhs->turns;
      }else{
        aratio /= -lhs->energytrain; // ppe
        bratio /= -rhs->energytrain;
      }
      // we multiply by 100 to ensure the significant figures of the
      // calculation are in the integer portion, then truncate, to kill
      // off any floating point error in the less significant digits
      aratio = trunc(aratio * 100);
      bratio = trunc(bratio * 100);
      if(aratio < bratio){
        return true;
      }else if(bratio < aratio){
        return false;
      }
      // next, compare ept for fast moves and energy for charged moves
      float ae = lhs->energytrain;
      float be = rhs->energytrain;
      if(fast_attack_p(*lhs)){
        ae /= lhs->turns;
        be /= rhs->turns;
      }else{
        ae = -ae;
        be = -be;
      }
      return ae < be;
      });
  fp << "<div id=\"atks3x3\">";
  fp << "<h2>attacks (3x3 stats)</h2>" << std::endl;
  write_fattacks_3x3(fp, s, atks);
  write_cattacks_3x3(fp, s, atks);
  write_mon_ttf_3x3(fp, s);
  fp << "</div>" << std::endl;
}

// atks duplicates s.attacks, but we can rearrange (sort) it
static void
write_mon_attacks_nx1(std::ofstream &fp, const species &s, std::vector<const attack *> &atks){
  std::sort(atks.begin(), atks.end(), [s](const attack *lhs, const attack *rhs){
        // next, compare ppt
        float aratio = calc_eff_power_nx1(s, lhs);
        float bratio = calc_eff_power_nx1(s, rhs);
        aratio /= lhs->animdur;
        bratio /= rhs->animdur;
        aratio = trunc(aratio * 100);
        bratio = trunc(bratio * 100);
        if(aratio < bratio){
          return true;
        }else if(bratio < aratio){
          return false;
        }
        // next, compare ept for fast moves and ppe for charged moves
        float ae;
        float be;
        if(fast_attack_p(*lhs)){
          ae = static_cast<float>(lhs->energyraid) / lhs->animdur;
          be = static_cast<float>(rhs->energyraid) / rhs->animdur;
        }else{
          ae = calc_eff_power_nx1(s, lhs) / lhs->animdur;
          be = calc_eff_power_nx1(s, rhs) / rhs->animdur;
        }
        return ae < be;
      });
  fp << "<div id=\"atksNx1\">";
  fp << "<h2>attacks (Nx1 stats)</h2>" << std::endl;
  fp << "<table class=\"evenshade fastattacks\">" << std::endl;
  fp << "<tr><th>Attack</th><th>Turns</th><th>Power</th><th>Energy+</th><th>PPT</th><th>EPT</th></tr>" << std::endl;
  for(const auto *a : atks){
    if(!fast_attack_p(*a)){
      continue;
    }
    auto cpow = calc_eff_power_nx1(s, a);
    fp << "<tr>";
    fp << "<td>";
    html_type(fp, a->type, true);
    std::string prefix, suffix;
    emit_html_attack_fancy(fp, s, a, prefix, suffix, true, true);
    fp << "</td>";
    fp << "<td>";
    if(a->animdur){
      fp << prefix << a->animdur << suffix;
    }
    fp << "</td>";
    fp << "<td>" << prefix << cpow << suffix << "</td>";
    fp << "<td>" << prefix << a->energyraid << suffix << "</td>";
    fp << "<td>" << prefix << (cpow / a->animdur) << suffix << "</td>";
    fp << "<td>" << prefix << (static_cast<float>(a->energyraid) / a->animdur) << suffix << "</td>";
    fp << "</tr>" << std::endl;
  }
  fp << "</table>" << std::endl;
  fp << "<table class=\"evenshade chargedattacks\">" << std::endl;
  fp << "<tr><th>Attack</th><th>Turns</th><th>Power</th><th>Energy-</th><th>PPT</th><th>PPE</th></tr>" << std::endl;
  for(const auto a : atks){
    if(!charged_attack_p(*a)){
      continue;
    }
    auto cpow = calc_eff_power_nx1(s, a);
    fp << "<tr>";
    fp << "<td>";
    html_type(fp, a->type, true);
    std::string prefix, suffix;
    emit_html_attack_fancy(fp, s, a, prefix, suffix, true, true);
    fp << "</td>";
    fp << "<td>";
    if(a->animdur){
      fp << prefix << a->animdur << suffix;
    }
    fp << "</td>";
    fp << "<td>" << prefix << cpow << suffix << "</td>";
    fp << "<td>" << prefix << a->energyraid << suffix << "</td>";
    fp << "<td>" << prefix << (cpow / a->animdur) << suffix << "</td>";
    fp << "<td>" << prefix << (cpow / a->energyraid) << suffix << "</td>";
    fp << "</tr>" << std::endl;
  }
  fp << "</table>" << std::endl;
  write_mon_ttf_Nx1(fp, s);
  fp << "</div>" << std::endl;
}

static int
write_atk_effectiveness(std::ostream &fp, const species &s){
  fp << "<div id=\"atkeffectiveness\">" << std::endl;
  fp << "<h2>attack effectiveness</h2>" << std::endl;
  fp << "<table id=\"atkeffect\">" << std::endl;
  fp << "<tr>";
  for(int i = -3 ; i < 3 ; ++i){
    fp << "<th class=\"" << reversecss[i + 3] << "\">" << i << "</th>";
  }
  fp << "</tr>" << std::endl;
  fp << "<tr>";
  for(int i = -3 ; i < 3 ; ++i){
    fp << "<td class=\"" << reversecss[i + 3] << "\">";
    unsigned printed = 0;
    for(pgo_types_e t = TYPESTART ; t < TYPECOUNT ; ++t){
      if(typing_relation(t, s.t1, s.t2) == i){
        html_type(fp, t, true);
        if(++printed % 3 == 0){
          fp << "<br/>";
        }else{
          fp << ' ';
        }
      }
    }
    if(printed == 0){
      fp << "🛇";
    }
    fp << "</td>";
  }
  fp << "</tr>" << std::endl;
  fp << "</table>" << std::endl;
  fp << "</div>" << std::endl;
  return 0;
}

static int
write_stats(std::ostream &fp, const species &s){
  fp << "<div id=\"basics\">";
  fp << "<h2>basics</h2>" << std::endl;
  fp << "Attack: " << s.atk << "<br/>";
  fp << "Defense: " << s.def << "<br/>";
  fp << "Stamina: " << s.sta << "<br/>";
  fp << "Max CP: " << s.maxcp() << "<br/>";
  fp << "Weather boosts: ";
  html_weather(fp, s.t1, s.t2, true);
  fp << "<br/>";
  fp << "Attack<sup>2</sup> / Bulk: " << (pow(s.atk, 2) / (s.def * s.sta)) << "<br/>";
  fp << "Catch reward: " << stardust_reward(s) << " SD<br/>";
  fp << "Second attack cost: ";
  if(multiple_charged_attacks(s)){
    fp << s.a2cost << " kSD";
    /*if(s.shadow){
      fp << " (" << s.a2cost * 6 / 5 << " kSD for Shadow)";
    }*/
  }else if(s.shadow){
    fp << s.a2cost * 6 / 5 << " kSD (Shadow only)";
  }else{
    fp << "n/a";
  }
  fp << "<br/>";
  const auto *rstr = s.regionstr();
  if(!rstr){
    rstr = "Worldwide";
  }
  fp << "Region: " << rstr << "<br/>";
  if(s.shiny){
    fp << "Shiny available<br/>";
  }else{
    fp << "Shiny <i>not</i> available<br/>";
  }
  if(s.shadow){
    fp << "Shadow available<br/>";
    // purification costs seem to be wrong, don't track 2nd attack costs
    // fp << "Purification cost: " << purification_cost(s) << " kSD<br/>";
  }else{
    fp << "Shadow <i>not</i> available<br/>";
  }
  if(s.dmax && s.dmax != GMAX_TIER){
    fp << "Dynamax available<br/>";
  }else{
    fp << "Dynamax <i>not</i> available<br/>";
  }
  const auto *cstr = s.categorystr();
  if(strcmp(cstr, "")){
    fp << cstr << "<br/>";
  }
  if(pvp_banned_p(s)){
    fp << "Not permitted in 3x3<br/>";
  }
  fp << "</div>" << std::endl;
  return 0;
}

// get the geometry of the evolution from idx out. this is done recursively.
// if the idx has no evolutions in fam, the geometry is 1x1, the base case.
// otherwise, call on our immediate evolutions. our height is then the
// sum of evol_geom() called on each of our immediate evolutions (plus
// one if the number of immediate evolutions is even), and our length is
// the max of the lenghts, plus one.
static unsigned
evol_geom(const std::vector<const species*> fam, int idx, unsigned &length,
          unsigned x, unsigned y, unsigned &forkcolumn, unsigned &forkidx){
  // do we have any immediate evolutions? if not, we're terminal.
  const species &s = *fam[idx];
  length = 1;
  unsigned maxsublen = 0;
  int totsubheight = 0;
  int immediates = 0;
  // find our immediate evolutions, and recurse
  for(unsigned idx2 = idx + 1 ; idx2 < fam.size() ; ++idx2){
    if(fam[idx2]->from == s.name){
      if(immediates && !forkidx){
        forkidx = idx + 1;
        forkcolumn = x + 1;
      }
      ++immediates;
      unsigned sublen;
      auto subheight = evol_geom(fam, idx2, sublen, x + 1, y, forkcolumn, forkidx);
      if(sublen > maxsublen){
        maxsublen = sublen;
      }
      y += subheight;
      totsubheight += subheight;
    }
  }
  length = maxsublen + 1;
  if(totsubheight == 0 || (immediates % 2 == 0)){
    ++totsubheight;
  }
  return totsubheight;
}

// return the number of rows to represent this family's complete evolution
static unsigned
family_geom(const std::vector<const species*> fam, unsigned &length,
            unsigned &forkcolumn, unsigned &forkidx){
  forkcolumn = forkidx = 0;
  return evol_geom(fam, 0, length, 0, 0, forkcolumn, forkidx);
}

// there are only four table geometries (currently):
//  a) eevee has eight evolutions (9 rows, 2 columns)
//  b) one path splits into two (3 rows, n columns) -- charizard, mewtwo
//  c) one path splits into three (3 rows, n columns) -- tyrogue, applin
//  d) single path (1 row, n columns)
// the family always starts in the middle row (1, 2, or 5) in the
// first column. this is all very fragile, and will have to change
// to accommodate any new table conformations.
// forkcolumn (0-indexed) is the column following forkbase, 0 if there is no fork
static void
emit_evol(std::ostream &fp, const std::string &us, const species &s){
  if(us == s.name){
    fp << "<b>" << s.name << "</b>";
  }else{
    link_to_name(fp, s.name);
  }
}

struct evolpoint {
  const species *s;
  int col;
  int y;

  friend bool operator<(const evolpoint &lhs, const evolpoint &rhs){
    if(lhs.col < rhs.col){
      return true;
    }else if(rhs.col < lhs.col){
      return false;
    }else if(lhs.y < rhs.y){
      return true;
    }
    return false;
  }
};

static void
emit_evol_cost(std::ostream &fp, const evolpoint &ep, int cols, bool &first){
  auto cost = evolcost(*ep.s);
  if(cost){
    fp << "<tr>";
    if(ep.col > 1){
      // we don't want the padding on the first td, for weird browser reasons
      fp << "<td colspan=\"" << ep.col - 1 << "\"/>";
    }
    std::string padding{};
    std::string longpadding{};
    if(first){
      longpadding = "style=\"padding: 1em 0 0 0;";
      padding = "style=\"padding: 1em 0 0 0\"";
      first = false;
    }else{
      longpadding = "style=\"";
    }
    fp << "<td " << longpadding << "text-align:right\">" << ep.s->from << "</td>";
    fp << "<td " << padding << "colspan=\"" << cols - ep.col << "\">";
    fp << " → " << ep.s->name << " costs " << cost << " Candy";
    const auto *needs = species::evolitemstr(ep.s->evolitem);
    if(strlen(needs)){
      fp << ", " << needs;
    }
    fp << ".</td></tr>" << std::endl;
  }
}

static void
emit_evol_table(std::ostream &fp, const std::string &us,
                const std::vector<const species*> fam,
                unsigned rows, unsigned length,
                unsigned forkcolumn, unsigned forkidx){
  const auto midrow = rows / 2;
  int i = 0;
  int pathends = forkidx;
  const std::string *prev;
  std::vector<evolpoint> bases;
  for(unsigned y = 0 ; y < rows ; ++y){
    prev = nullptr;
    fp << "<tr>";
    std::string arrow;
    for(unsigned x = 0 ; x < length ; ++x){
      fp << "<td>"; // we must have a td for each cell
      if(y != midrow){ // rows off the main path, implies a fork
        if(x == forkcolumn){
          if(y < midrow){
            if(bidirectional_evol_p(*fam[forkidx])){
              arrow = "⤢ ";
            }else{
              arrow = "↗ ";
            }
          }else{
            if(bidirectional_evol_p(*fam[forkidx])){
              arrow = "⤡ ";
            }else{
              arrow = "↘ ";
            }
          }
          fp << arrow;
          emit_evol(fp, us, *fam[forkidx]);
          bases.emplace_back(fam[forkidx], x, y);
          prev = &fam[forkidx]->name;
          ++forkidx;
        }else if(x > forkcolumn){
          if(forkidx < fam.size()){
            if(fam[forkidx]->from == *prev){
              if(bidirectional_evol_p(*fam[forkidx])){
                arrow = "↔ ";
              }else{
                arrow = "→ ";
              }
              fp << arrow;
              emit_evol(fp, us, *fam[forkidx]);
              bases.emplace_back(fam[forkidx], x, y);
              prev = &fam[forkidx]->name;
              ++forkidx;
            }
          }
        }
      }else{ // main path
        if(!pathends || i < pathends){
          if(x > 0){
            if(bidirectional_evol_p(*fam[i])){
              arrow = "↔ ";
            }else{
              arrow = "→ ";
            }
          }
          fp << arrow;
          emit_evol(fp, us, *fam[i]);
          bases.emplace_back(fam[i], x, y);
        }else if(i >= pathends){
          // we continue along the main path only if there are three immediate
          // successors of the fork. this means there must be (a) three rows
          // and (b) (only) two more with us as parent.
          if(rows == 3){
            if(prev == nullptr){
              int successors = 0;
              auto sidx = forkidx;
              while(sidx < fam.size()){
                if(fam[sidx]->from == fam[i - 1]->name){
                  ++successors;
                }
                ++sidx;
              }
              if(successors == 2){
                prev = &fam[i]->name;
                if(bidirectional_evol_p(*fam[forkidx])){
                  arrow = "↔ ";
                }else{
                  arrow = "→ ";
                }
                fp << arrow;
                emit_evol(fp, us, *fam[forkidx]);
                bases.emplace_back(fam[forkidx], x, y);
                ++forkidx;
              }
            }else{
              if(fam[i]->from == *prev){
                if(bidirectional_evol_p(*fam[forkidx])){
                  arrow = "↔ ";
                }else{
                  arrow = "→ ";
                }
                fp << arrow;
                emit_evol(fp, us, *fam[forkidx]);
                bases.emplace_back(fam[forkidx], x, y);
                prev = &fam[i]->name;
                ++forkidx;
              }
            }
          }
        }
        ++i;
      }
      fp << "</td>";
    }
    fp << "</tr>" << std::endl;
  }
  std::sort(bases.begin(), bases.end());
  bool first = true;
  for(const auto &p : bases){
    emit_evol_cost(fp, p, length, first);
  }
}

static void
write_evol(std::ostream &fp, const species &s){
  fp << "<div id=\"transitions\">";
  fp << "<h2>transitions</h2>" << std::endl;
  const auto base = get_base_evolution(s);
  std::forward_list<species> store;
  std::vector<const species*> fam;
  get_family(base, fam, store);
  if(fam.size() < 2){ // should never be zero
    fp << "None</div>";
    return;
  }
  unsigned length;
  unsigned forkcolumn, forkidx;
  auto rows = family_geom(fam, length, forkcolumn, forkidx);
  fp << "<table id=\"evoltable\">" << std::endl;
  emit_evol_table(fp, s.name, fam, rows, length, forkcolumn, forkidx);
  fp << "</table>" << std::endl;
  fp << "</div>" << std::endl;
}

static void
write_counters(std::ostream &fp, const species &s){
  fp << "<div id=\"counterssec\">";
  fp << "<h2>counters</h2>" << std::endl;
  fp << "<h3>best Max attackers</h3>" << std::endl;
  //max_counters(fp, s);
  // FIXME do we want counters for each league? and gmax?
  fp << "</div>" << std::endl;
}

static int
write_mon_page(const species &s){
  std::string encname;
  encode_name(s.name, encname); // do *not* use URI encoding
  std::ofstream fp(encname + ".html");
  if(!fp.is_open()){
    std::cerr << "error opening " << encname << ".html for writing" << std::endl;
    return -1;
  }
  fp << std::setprecision(3);
  write_header(fp, s.name + " in Pokémon GO", true);
  fp << "<img src=\"../images/mon/" << encname << ".png\" height=\"512\" width=\"512\" alt=\"" << s.name << "\"/>" << std::endl;
  fp << "<h1 id=\"monname\">#" << std::format("{:04d} ", s.idx) << s.name << ' ';
  html_types(fp, s.t1, s.t2, true);
  fp << "</h1>" << std::endl;
  write_stats(fp, s);
  write_evol(fp, s);
  write_atk_effectiveness(fp, s);
  std::vector<const attack *> atks(s.attacks);
  if(s.shadow){
    atks.emplace_back(&ATK_Return);
  }
  if(!pvp_banned_p(s)){
    write_mon_attacks_3x3(fp, s, atks);
  }
  write_mon_attacks_nx1(fp, s, atks);
  write_mon_apairs(fp, s);
  if(!pvp_banned_p(s)){
    write_iv_tables(fp, s);
  }
  write_counters(fp, s);
  write_footer_and_close(fp, true);
  for(const auto &m : s.mforms){
    species smeg{&s, m};
    std::forward_list<attack> store;
    smeg.expand_plus_atk(store);
    if(write_mon_page(smeg)){
      return -1;
    }
  }
  // we *do* write a page for each gmax form, even though they're battle-only
  const auto *g = lookup_gmax_attack(s);
  if(g){
    write_mon_page({&s, *g});
  }
  return 0;
}

static int
write_summary(std::ostream &fp, const species &s){
  std::cerr << "Writing " << s.name << "..." << std::endl;
  fp << "<tr>";
  fp << "<td>" << std::format("{:04d}", s.idx) << "</td><td>";
  html_type(fp, s.t1, true);
  if(s.t2 != TYPECOUNT){
    fp << "<br/>";
    html_type(fp, s.t2, true);
  }
  fp << "</td><td>";
  fp << "<a href=\"";
  encode_name(fp, s.name);
  fp << ".html\">" << s.name << "</a>";
  fp << "</td>";
  fp << "<td>";
  if(s.shiny){
    fp << "✓";
  }
  fp << "</td>";
  fp << "<td>";
  if(s.shadow){
    fp << "✓";
  }
  fp << "</td>";
  fp << "<td>";
  if(s.dmax && s.dmax < GMAX_TIER){
    fp << "✓";
  }
  fp << "</td>";
  fp << "<td><img loading=\"lazy\" src=\"../images/mon/";
  encode_name(fp, s.name);
  fp << ".png\" height=\"64\" width=\"64\" alt=\"" << s.name << "\"/></td>";
  fp << "</tr>" << std::endl;
  for(const auto &m : s.mforms){
    write_summary(fp, {&s, m});
  }
  // gmax is a battle-only form. don't list them.
  return 0;
}

// assume we're in the target directory
static int
write_index(){
  calc_cmp_graph(cmpsgl, 0, GLCPCAP);
  if(write_cmp_graph("GL", cmpsgl)){
    return -1;
  }
  calc_cmp_graph(cmpsul, GLCPCAP, ULCPCAP);
  if(write_cmp_graph("UL", cmpsul)){
    return -1;
  }
  calc_cmp_graph(cmpsml, ULCPCAP, 0);
  if(write_cmp_graph("ML", cmpsml)){
    return -1;
  }
  std::ofstream fp("index.html");
  if(!fp.is_open()){
    std::cerr << "error opening index.html for writing" << std::endl;
    return -1;
  }
  write_header(fp, "Pokémon GO forms", true);
  fp << "<div class=\"intro\">The following forms are available in Pokémon GO. ";
  fp << "Functionally equivalent forms are not distinguished. ";
  fp << "Battle-only forms are not listed.</div>" << std::endl;
  fp << "<table id=\"allforms\">" << std::endl;
  fp << "<tr><th>Dex#</th><th>T</th><th>Form</th><th>";
  fp << "<img src=\"../images/shiny.png\" class=\"type\" alt=\"Shiny\"/>";
  fp << "</th><th>";
  fp << "<img src=\"../images/shadow.png\" class=\"type\" alt=\"Shadow\"/>";
  fp << "</th><th>";
  fp << "<img src=\"../images/dynamax.png\" class=\"type\" alt=\"Dynamax\"/>";
  fp << "</th><th>Sprite</th></tr>" << std::endl;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(write_summary(fp, *s)){
      return -1;
    }
    if(write_mon_page(*s)){
      return -1;
    }
  }
  fp << "</table>" << std::endl;
  write_footer_and_close(fp, true);
  return 0;
}

static void
usage(const char *argv0, int ret){
  std::cerr << "usage: " << argv0 << " targetdir" << std::endl;
  exit(ret);
}

// generate a directory full of species pages, overwriting any that exist.
// an index.html is also generated.
int main(int argc, const char **argv){
  if(argc != 2){
    usage(argv[0], EXIT_FAILURE);
  }
  if(chdir(argv[1])){
    std::cerr << "error changing directory to " << argv[1] << ": " << strerror(errno) << std::endl;
    usage(argv[0], EXIT_FAILURE);
  }
  if(write_index()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
