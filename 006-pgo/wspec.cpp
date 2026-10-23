#include "html.h"
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <algorithm>

static void
write_iv_graphrefs(std::ostream &fp, const species &s, const char *leagueshort){
  std::string enc;
  if(s.name.starts_with("G-Max")){
    encode_name(s.name.substr(5), enc);
  }else{
    encode_name(s.name, enc);
  }
  fp << "<img class=\"monscatter\" src=\"graphs/" << enc << leagueshort << ".png\" alt=\"scatterplot for " << leagueshort << "\"/>";
  fp << "<img class=\"monradar\" src=\"graphs/" << enc << leagueshort << "Radar.png\" alt=\"radarplot for " << leagueshort << "\"/>";
}

struct tristats {
  float min, max, tot;
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
  for(unsigned i = 0 ; i < ivcount ; ++i){
    const auto &st = sets[i];
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
  }
  fp << std::fixed << std::setprecision(2);
  fp << "<table class=\"monleaguestats\">" << std::endl;
  fp << "<tr><th>Measure</th><th>Min</th><th>Mean</th><th>Max</th></tr>" << std::endl;
  fp << "<tr><td>Gmean</td><td>" << gmean.min << "</td><td>" << gmean.tot / ivcount << "</td><td>" << gmean.max << "</td></tr>" << std::endl;
  fp << "<tr><td>Amean</td><td>" << amean.min << "</td><td>" << amean.tot / ivcount << "</td><td>" << amean.max << "</td></tr>" << std::endl;
  fp << "<tr><td>Eff<sub>A</sub></td><td>" << effa.min << "</td><td>" << effa.tot / ivcount << "</td><td>" << effa.max << "</td></tr>" << std::endl;
  fp << "<tr><td>Eff<sub>D</sub></td><td>" << effd.min << "</td><td>" << effd.tot / ivcount << "</td><td>" << effd.max << "</td></tr>" << std::endl;
  fp << std::defaultfloat << std::setprecision(5);
  fp << "<tr><td>MHP</td><td>" << mhp.min << "</td><td>" << mhp.tot / ivcount << "</td><td>" << mhp.max << "</td></tr>" << std::endl;
  fp << "<tr><td>Level</td><td>";
  emit_halflevel_as_level(fp, lmin);
  fp << "</td><td>n/a</td><td>";
  emit_halflevel_as_level(fp, lmax);
  fp << "</td></tr>" << std::endl;
  fp << "</table>" << std::endl;
}

// returns true if we ought bother emitting the next highest table, false
// otherwise. we return true if any cp in the solution set equals the cpbound.
static bool
write_iv_table(std::ostream &fp, const species &s, const char *league,
               const char *leagueshort, int cpbound){
  unsigned ivcount;
  auto sets = order_ivs(s, cpbound, statscmp_gmean, &ivcount);
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
  fp << "<th>MHP</th>";
  fp << "<th>Gmean</th>";
  fp << "<th>%</th>";
  fp << "</tr>";
  float gmax = sets[ivcount - 1].geommean;
  float gprev = 0;
  unsigned grank = ivcount;
  int id = 1;
  bool ret = false;
  do{
    const stats &s = sets[--grank];
    if(!s.shadow){
      fp << std::fixed << std::setprecision(2);
      fp << "<tr>";
      if(s.geommean == gprev){
        fp << "<td></td>";
      }else{
        fp << "<td>" << id << "</td>";
        gprev = s.geommean;
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
  return ret;
}

static void
write_iv_tables(std::ostream &fp, const species &s){
  bool donext = write_iv_table(fp, s, "great league", "GL", GLCPCAP);
  if(donext){
    donext = write_iv_table(fp, s, "ultra league", "UL", ULCPCAP);
  }
  if(donext){
    donext = write_iv_table(fp, s, "master league", "ML", 0);
  }
}

static void
write_fattacks_3x3(std::ostream &fp, const species &s, const std::vector<const attack*> &sortedatks){
  fp << "<table class=\"evenshade fastattacks\">" << std::endl;
  fp << "<tr><th>T</th><th>Attack</th><th>Turns</th><th>Power</th><th>Energy+</th><th>PPT</th><th>EPT</th></tr>" << std::endl;
  for(const auto *a : sortedatks){
    if(!fast_attack_p(*a)){
      continue;
    }
    auto cpow = calc_eff_power(s, a);
    fp << "<tr>";
    fp << "<td>";
    html_type_pdir(fp, a->type);
    fp << "</td>";
    fp << "<td>";
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
  fp << "<tr><th>T</th><th>Attack</th><th>Buffs</th><th>Power</th><th>Energy-</th><th>PPE</th></tr>" << std::endl;
  for(const auto *a : sortedatks){
    if(!charged_attack_p(*a)){
      continue;
    }
    auto cpow = calc_eff_power(s, a);
    fp << "<tr>";
    fp << "<td>";
    html_type_pdir(fp, a->type);
    fp << "</td>";
    fp << "<td>";
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

static void
write_mon_attacks_3x3(std::ostream &fp, const species &s, std::vector<const attack *> &atks){
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
  fp << "<h2>attacks (3x3 stats)</h2>" << std::endl;
  write_fattacks_3x3(fp, s, atks);
  write_cattacks_3x3(fp, s, atks);
}

// atks duplicates s.attacks, but we can rearrange (sort) it
static void
write_mon_attacks_nx1(std::ostream &fp, const species &s, std::vector<const attack *> &atks){
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
  fp << "<h2>attacks (Nx1 stats)</h2>" << std::endl;
  fp << "<table class=\"evenshade fastattacks\">" << std::endl;
  fp << "<tr><th>T</th><th>Attack</th><th>Turns</th><th>Power</th><th>Energy+</th><th>PPT</th><th>EPT</th></tr>" << std::endl;
  for(const auto *a : atks){
    if(!fast_attack_p(*a)){
      continue;
    }
    auto cpow = calc_eff_power_nx1(s, a);
    fp << "<tr>";
    fp << "<td>";
    html_type_pdir(fp, a->type);
    fp << "</td>";
    fp << "<td>";
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
    fp << "<td>" << prefix << (a->energyraid / a->animdur) << suffix << "</td>";
    fp << "</tr>" << std::endl;
  }
  fp << "</table>" << std::endl;
  fp << "<table class=\"evenshade chargedattacks\">" << std::endl;
  fp << "<tr><th>T</th><th>Attack</th><th>Turns</th><th>Power</th><th>Energy-</th><th>PPT</th><th>PPE</th></tr>" << std::endl;
  for(const auto a : atks){
    if(!charged_attack_p(*a)){
      continue;
    }
    auto cpow = calc_eff_power_nx1(s, a);
    fp << "<tr>";
    fp << "<td>";
    html_type_pdir(fp, a->type);
    fp << "</td>";
    fp << "<td>";
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
}

static int
write_atk_effectiveness(std::ostream &fp, const species &s){
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
        html_type_pdir(fp, t);
        if(++printed % 3 == 0){
          fp << "<br/>";
        }else{
          fp << ' ';
        }
      }
    }
    if(printed == 0){
      fp << "🗙";
    }
    fp << "</td>";
  }
  fp << "</tr>" << std::endl;
  fp << "</table>" << std::endl;
  return 0;
}

static int
write_stats(std::ostream &fp, const species &s){
  fp << "<h2>basics</h2>" << std::endl;
  fp << "<div id=\"basics\">";
  fp << "Attack: " << s.atk << "<br/>";
  fp << "Defense: " << s.def << "<br/>";
  fp << "Stamina: " << s.sta << "<br/>";
  fp << "Max CP: " << s.maxcp() << "<br/>";
  fp << "Attack<sup>2</sup> / Bulk: " << (pow(s.atk, 2) / (s.def * s.sta)) << "<br/>";
  fp << "Catch reward: " << stardust_reward(s) << " SD<br/>";
  fp << "Second attack cost: ";
  if(multiple_charged_attacks(s)){
    fp << s.a2cost << " kSD";
  }else if(s.shadow){
    fp << s.a2cost << " kSD (Shadow only)";
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

static void
emit_evol_table(std::ostream &fp, const std::string &us,
                const std::vector<const species*> fam,
                unsigned rows, unsigned length,
                unsigned forkcolumn, unsigned forkidx){
  const auto midrow = rows / 2;
  int i = 0;
  int pathends = forkidx;
  const std::string *prev;
  for(unsigned y = 0 ; y < rows ; ++y){
    prev = nullptr;
    fp << "<tr>";
    for(unsigned x = 0 ; x < length ; ++x){
      fp << "<td>"; // we must have a td for each cell
      if(y != midrow){ // rows off the main path, implies a fork
        if(x == forkcolumn){
          if(y < midrow){
            if(bidirectional_evol_p(*fam[forkidx])){
              fp << "⤢ ";
            }else{
              fp << "↗ ";
            }
          }else{
            if(bidirectional_evol_p(*fam[forkidx])){
              fp << "⤡ ";
            }else{
              fp << "↘ ";
            }
          }
          emit_evol(fp, us, *fam[forkidx]);
          prev = &fam[forkidx]->name;
          ++forkidx;
        }else if(x > forkcolumn){
          if(forkidx < fam.size()){
            if(fam[forkidx]->from == *prev){
              if(bidirectional_evol_p(*fam[forkidx])){
                fp << "↔ ";
              }else{
                fp << "→ ";
              }
              emit_evol(fp, us, *fam[forkidx]);
              prev = &fam[forkidx]->name;
              ++forkidx;
            }
          }
        }
      }else{ // main path
        if(!pathends || i < pathends){
          if(x > 0){
            if(bidirectional_evol_p(*fam[i])){
              fp << "↔ ";
            }else{
              fp << "→ ";
            }
          }
          emit_evol(fp, us, *fam[i]);
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
                  fp << "↔ ";
                }else{
                  fp << "→ ";
                }
                emit_evol(fp, us, *fam[forkidx]);
                ++forkidx;
              }
            }else{
              if(fam[i]->from == *prev){
                if(bidirectional_evol_p(*fam[forkidx])){
                  fp << "↔ ";
                }else{
                  fp << "→ ";
                }
                emit_evol(fp, us, *fam[forkidx]);
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
}

static void
write_evol(std::ostream &fp, const species &s){
  fp << "<h2>transitions</h2>" << std::endl;
  const auto base = get_base_evolution(s);
  std::forward_list<species> store;
  std::vector<const species*> fam;
  get_family(base, fam, store);
  if(fam.size() < 2){ // should never be zero
    fp << "<div id=\"transitions\">None</div>";
    return;
  }
  unsigned length;
  unsigned forkcolumn, forkidx;
  auto rows = family_geom(fam, length, forkcolumn, forkidx);
  fp << "<table id=\"transitions\">" << std::endl;
  emit_evol_table(fp, s.name, fam, rows, length, forkcolumn, forkidx);
  fp << "</table>" << std::endl;
}

/*static unsigned
fast_attacks_req(int energy, const attack *a, const attack *ca){
  int req = -ca->energytrain - energy;
  if(req < 0){
    req = 0;
  }
  int n = req / a->energytrain;
  if(n * a->energytrain < req){
    ++n;
  }
  assert(n * a->energytrain >= req);
  return n;
}

// we get a record of charged moves. we must recover the number of fast
// moves, which is thankfully deterministic.
static void
dump_sim(std::ostream &fp, const attack *fa, const std::vector<const attack*> &hist){
  int energy = 0;
  for(const auto a : hist){
    auto fn = fast_attacks_req(energy, fa, a);
    energy += fn * fa->energytrain;
    energy += a->energytrain;
    fp << fn << "x + " << a->name << " (" << energy << ") ";
  }
  fp << "<br/>";
}

// we want to go through all possible cycles. it's a cycle if we ever repeat a
// remainder following a charged attack.
static void
sim_pacing(std::ostream &fp, std::vector<int> &mods,
           std::vector<const attack*> &hist,
           const attack *a, const attack *c1,const attack *c2){
  // determine number of fast attacks [0..n] necessary to get to the next
  // charged attack for each CA. add it to hist, and compute new energy.
  // if energy is not in mods, add it to mods and recurse. if it is, dump
  // history and recurse no further.
  const int incoming = mods.back();
  int energy = incoming;
  auto cf = fast_attacks_req(energy, a, c1);
  hist.push_back(c1);
  energy += cf * a->energytrain;
  energy += c1->energytrain;
  if(std::ranges::contains(mods, energy)){
    dump_sim(fp, a, hist);
  }else{
    mods.push_back(energy);
    sim_pacing(fp, mods, hist, a, c1, c2);
  }
  hist.pop_back();
  energy = incoming;
  cf = fast_attacks_req(energy, a, c2);
  hist.push_back(c2);
  energy += cf * a->energytrain;
  energy += c2->energytrain;
  if(std::ranges::contains(mods, energy)){
    dump_sim(fp, a, hist);
  }else{
    mods.push_back(energy);
    sim_pacing(fp, mods, hist, a, c1, c2);
  }
  hist.pop_back();
}

static void
write_mon_pacing(std::ostream &fp, const species &s){
  fp << "<h2>pacing (3x3 stats)</h2>" << std::endl;
  fp << "These pacing specifications assume the desired charged attack is used as soon as sufficient ";
  fp << "energy is built up.<br/>";
  // we first partition into fast and charged attacks. then, for each fast
  // attack, run the pacing with each pair of two charged attacks (unless
  // only one charged attack can be learned, in which case just do that).
  // we do not bother with single charged attack configurations otherwise.
  std::vector<const attack*> fas, cas;
  for(const auto a : s.attacks){
    if(fast_attack_p(*a)){
      fas.push_back(a);
    }else{
      cas.push_back(a);
    }
  }
  // if they have a shadow, we ought handle Return by adding it to cas FIXME
  if(cas.size() == 1){
    // FIXME
  }
  fp << "<details><summary>expand table</summary>" << std::endl;
  // FIXME include ARAs?
  fp << "<table id=\"monpacing\">" << std::endl;
  fp << "<tr><th>Attacks</th><th>Pacing</th></tr>" << std::endl;
  for(const auto a : fas){
    for(unsigned c1idx = 0 ; c1idx < cas.size() ; ++c1idx){
      const auto *c1 = cas[c1idx];
      for(unsigned c2idx = c1idx + 1 ; c2idx < cas.size() ; ++c2idx){
        const auto *c2 = cas[c2idx];
        fp << "<tr><td>";
        html_type_pdir(fp, a->type);
        fp << ' ';
        link_to_attack(fp, a->name, true, true);
        fp << " (" << a->energytrain << ") +<br/>";
        html_type_pdir(fp, c1->type);
        fp << ' ';
        link_to_attack(fp, c1->name, true, true);
        fp << " (" << -c1->energytrain << ") / ";
        html_type_pdir(fp, c2->type);
        fp << ' ';
        link_to_attack(fp, c2->name, true, true);
        fp << " (" << -c2->energytrain << ")</td>";
        fp << "<td>";
        // FIXME get this reduced so we needn't do summary
        fp << "<details><summary>expand table</summary>" << std::endl;
        std::vector<int> mods{0};
        std::vector<const attack*> hist;
        sim_pacing(fp, mods, hist, a, c1, c2);
        fp << "</details></td></tr>" << std::endl;
      }
    }
  }
  fp << "</table>" << std::endl;
  fp << "</details>" << std::endl;
}*/

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
  html_types_pdir(fp, s.t1, s.t2);
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
  //write_mon_pacing(fp, s);
  if(!pvp_banned_p(s)){
    write_iv_tables(fp, s);
  }
  // FIXME moar crap ... counters, attack sets, time to first charged attack
  write_footer_and_close(fp);
  for(const auto &m : s.mforms){
    species smeg{&s, m};
    std::forward_list<attack> store;
    smeg.expand_plus_atk(store);
    if(write_mon_page(smeg)){
      return -1;
    }
  }
  const auto *g = lookup_gmax_attack(s);
  if(g){
    write_mon_page({&s, *g});
  }
  return 0;
}

static void
write_summary(std::ostream &fp, const species &s){
  fp << "<tr>";
  fp << "<td>" << std::format("{:04d}", s.idx) << "</td><td>";
  html_type_pdir(fp, s.t1);
  if(s.t2 != TYPECOUNT){
    fp << "<br/>";
    html_type_pdir(fp, s.t2);
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
  const auto *g = lookup_gmax_attack(s);
  if(g){
    write_summary(fp, {&s, *g});
  }
}

// assume we're in the target directory
static int
write_index(){
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
    write_summary(fp, *s);
  }
  fp << "</table>" << std::endl;
  write_footer_and_close(fp);
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
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(write_mon_page(*s)){
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
