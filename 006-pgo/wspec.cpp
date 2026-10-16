#include "html.h"
#include <cstdlib>
#include <fstream>
#include <unistd.h>

// returns true if we ought bother emitting the next highest table, false
// otherwise. we return true if any cp in the solution set equals the cpbound.
static bool
write_iv_table(std::ostream &fp, const species &s, const char *league,
               const char *leagueshort, int cpbound){
  bool graph = !s.name.contains("G-Max"); // FIXME
  if(graph){
    std::string cmd = "Rscript ../../gmean.R \"" + s.name + "\" " + leagueshort;
    if(system(cmd.c_str())){ // ugh
      std::cerr << "Rscript failed for " << s.name << std::endl;
      throw std::exception();
    }
  }
  unsigned ivcount;
  auto sets = order_ivs(s, cpbound, statscmp_gmean, &ivcount);
  fp << "<h2>" << league << "</h2>" << std::endl;
  if(graph){
    std::string enc;
    encode_name(s.name, enc);
    fp << "<img src=\"graphs/" << enc << leagueshort << ".png\" alt=\"scatterplot for " << leagueshort << "\"/>";
  }
  fp << "<h3>IVs ordered by geometric mean</h3>" << std::endl;
  // FIXME provide an actual stat summary
  fp << "<details><summary>expand table</summary>" << std::endl;
  fp << "<table class=\"evenshade alignright monivs\">" << std::endl;
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
      fp << std::setprecision(6);
      fp << "<td>" << s.effa << "</td>";
      fp << "<td>" << s.effd << "</td>";
      fp << "<td>" << s.mhp << "</td>";
      fp << "<td>" << s.geommean << "</td>";
      fp << std::setprecision(3);
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
write_mon_attacks_3x3(std::ostream &fp, const species &s){
  std::vector<const attack *> sortedatks(s.attacks);
  if(s.shadow){
    sortedatks.emplace_back(&ATK_Return);
  }
  std::sort(sortedatks.begin(), sortedatks.end(), [s](const attack *lhs, const attack *rhs){
        if(fast_attack_p(*lhs) && !fast_attack_p(*rhs)){
          return true;
        }else if(!fast_attack_p(*lhs) && fast_attack_p(*rhs)){
          return false;
        }
        float aratio = calc_eff_power(s, lhs);
        float bratio = calc_eff_power(s, rhs);
        if(fast_attack_p(*lhs)){
          aratio /= lhs->turns;
          bratio /= rhs->turns;
        }else{
          aratio /= -lhs->energytrain;
          bratio /= -rhs->energytrain;
        }
        return aratio < bratio;
      });
  fp << "<h2>attacks (3x3 stats)</h2>" << std::endl;
  write_fattacks_3x3(fp, s, sortedatks);
  write_cattacks_3x3(fp, s, sortedatks);
}

static void
write_mon_attacks_nx1(std::ostream &fp, const species &s){
  std::vector<const attack *> sortedatks(s.attacks);
  if(s.shadow){
    sortedatks.emplace_back(&ATK_Return);
  }
  std::sort(sortedatks.begin(), sortedatks.end(), [s](const attack *lhs, const attack *rhs){
        if(fast_attack_p(*lhs) && !fast_attack_p(*rhs)){
          return true;
        }else if(!fast_attack_p(*lhs) && fast_attack_p(*rhs)){
          return false;
        }
        float aratio = calc_eff_power_nx1(s, lhs);
        float bratio = calc_eff_power_nx1(s, rhs);
        aratio /= lhs->animdur;
        bratio /= rhs->animdur;
        return aratio < bratio;
      });
  fp << "<h2>attacks (Nx1 stats)</h2>" << std::endl;
  fp << "<table class=\"evenshade fastattacks\">" << std::endl;
  fp << "<tr><th>T</th><th>Attack</th><th>Turns</th><th>Power</th><th>Energy+</th><th>PPT</th><th>EPT</th></tr>" << std::endl;
  for(const auto *a : sortedatks){
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
    fp << "<td>" << prefix << (cpow / a->energyraid) << suffix << "</td>";
    fp << "</tr>" << std::endl;
  }
  fp << "</table>" << std::endl;
  fp << "<table class=\"evenshade chargedattacks\">" << std::endl;
  fp << "<tr><th>T</th><th>Attack</th><th>Turns</th><th>Power</th><th>Energy-</th><th>PPT</th><th>PPE</th></tr>" << std::endl;
  for(const auto a : sortedatks){
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
  const std::string css[6] = { "teneg3", "teneg2", "teneg1", "tepos0", "tepos1", "tepos2", };
  fp << "<h2>attack effectiveness</h2>" << std::endl;
  fp << "<table>" << std::endl;
  fp << "<tr>";
  for(int i = -3 ; i < 3 ; ++i){
    fp << "<th class=\"" << css[i + 3] << "\">" << i << "</th>";
  }
  fp << "</tr>" << std::endl;
  fp << "<tr>";
  for(int i = -3 ; i < 3 ; ++i){
    fp << "<td class=\"" << css[i + 3] << "\">";
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
  fp << "<div class=\"stats\">";
  fp << "Attack: " << s.atk << "<br/>";
  fp << "Defense: " << s.def << "<br/>";
  fp << "Stamina: " << s.sta << "<br/>";
  fp << "Max CP: " << s.maxcp() << "<br/>";
  fp << "Attack<sup>2</sup> / Bulk: " << (pow(s.atk, 2) / (s.def * s.sta)) << "<br/>";
  fp << "Catch reward: " << stardust_reward(s) << " SD<br/>";
  fp << "Second attack cost: " << s.a2cost << " kSD<br/>";
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
  if(s.dmax){
    fp << "Dynamax available<br/>";
  }else{
    fp << "Dynamax <i>not</i> available<br/>";
  }
  const auto *cstr = s.categorystr();
  if(strcmp(cstr, "")){
    fp << cstr << "<br/>";
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
            fp << "↗";//↱";
          }else{
            fp << "↘";//"↳";
          }
          emit_evol(fp, us, *fam[forkidx]);
          prev = &fam[forkidx]->name;
          ++forkidx;
        }else if(x > forkcolumn){
          if(forkidx < fam.size()){
            if(fam[forkidx]->from == *prev){
              fp << " → ";
              emit_evol(fp, us, *fam[forkidx]);
              prev = &fam[forkidx]->name;
              ++forkidx;
            }
          }
        }
      }else{ // main path
        if(!pathends || i < pathends){
          if(x > 0){
            fp << " → ";
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
                fp << " → ";
                emit_evol(fp, us, *fam[forkidx]);
                ++forkidx;
              }
            }else{
              if(fam[i]->from == *prev){
                fp << " → ";
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

static int
write_evol(std::ostream &fp, const species &s){
  fp << "<h2>transitions</h2>";
  const auto base = get_base_evolution(s);
  std::forward_list<species> store;
  std::vector<const species*> fam;
  get_family(base, fam, store);
  if(fam.size() < 2){ // should never be zero
    fp << "None";
    return 0;
  }
  unsigned length;
  unsigned forkcolumn, forkidx;
  auto rows = family_geom(fam, length, forkcolumn, forkidx);
  fp << "<table>" << std::endl;
  emit_evol_table(fp, s.name, fam, rows, length, forkcolumn, forkidx);
  fp << "</table>" << std::endl;
  return 0;
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
  write_header(fp, s.name);
  fp << "<img src=\"../images/mon/" << encname << ".png\" height=\"512\" width=\"512\" alt=\"" << s.name << "\"/>" << std::endl;
  fp << "<h1 id=\"monname\">#" << std::format("{:04d} ", s.idx) << s.name << ' ';
  html_types_pdir(fp, s.t1, s.t2);
  fp << "</h1>" << std::endl;
  write_stats(fp, s);
  write_evol(fp, s);
  write_atk_effectiveness(fp, s);
  write_mon_attacks_3x3(fp, s);
  write_mon_attacks_nx1(fp, s);
  write_iv_tables(fp, s);
  // FIXME moar crap ... counters, attack sets, time to first charged attack
  write_footer_and_close(fp);
  for(const auto &m : s.mforms){
    if(write_mon_page({&s, m})){
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
  if(s.dmax){
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
  write_header(fp, "Pokémon GO forms");
  fp << "<div class=\"intro\">The following forms are available in Pokémon GO. ";
  fp << "Functionally equivalent forms are not distinguished. ";
  fp << "Battle-only forms are not listed.</div>" << std::endl;
  fp << "<table>" << std::endl;
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
