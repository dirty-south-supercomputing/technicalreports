#include "html.h"
#include <fstream>
#include <unistd.h>

static void
write_header(std::ofstream &fp, const std::string &title){
  fp << "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  fp << "<link rel=\"stylesheet\" type=\"text/css\" href=\"../list.css\">";
  fp << "<title>" << title << "</title></head>" << std::endl;
  fp << "<body>" << std::endl;
}

static void
write_footer_and_close(std::ofstream &fp){
  fp << "<hr/><a href=\"https://goldandappelpub.com/pgo-quantitative.html\"><i>Pokémon GO: A Quantitative Approach</i></a>";
  fp << "</body></html>" << std::endl;
  fp.close(); // FIXME check errors or enable exceptions
}

static void
encode_name(const std::string &s, std::string &encname){
  for(char c : s){
    if(!isspace(c) && !ispunct(c)){
      encname.push_back(c);
    }
  }
}

// returns true if we ought bother emitting the next highest table, false
// otherwise. we return true if any cp in the solution set equals the cpbound.
static bool
write_iv_table(std::ostream &fp, const species &s, const char *league, int cpbound){
  unsigned ivcount;
  auto sets = order_ivs(s, cpbound, statscmp_gmean, &ivcount);
  fp << "<h2>" << league << " IVs ordered by geometric mean</h2>" << std::endl;
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
  bool donext = write_iv_table(fp, s, "great league", GLCPCAP);
  if(donext){
    donext = write_iv_table(fp, s, "ultra league", ULCPCAP);
  }
  if(donext){
    donext = write_iv_table(fp, s, "master league", 0);
  }
}

static void
write_mon_attacks_3x3(std::ostream &fp, const species &s){
  std::vector<const attack *> sortedatks(s.attacks);
  if(s.shadow){
    sortedatks.emplace_back(&ATK_Return);
  }
  std::sort(sortedatks.begin(), sortedatks.end(), [s](const attack *lhs, const attack *rhs){
        if(fast_attack_p(lhs) && !fast_attack_p(rhs)){
          return true;
        }else if(!fast_attack_p(lhs) && fast_attack_p(rhs)){
          return false;
        }
        float aratio = calc_eff_power(s, lhs);
        float bratio = calc_eff_power(s, rhs);
        if(fast_attack_p(lhs)){
          aratio /= lhs->turns;
          bratio /= rhs->turns;
        }else{
          aratio /= -lhs->energytrain;
          bratio /= -rhs->energytrain;
        }
        return aratio < bratio;
      });
  fp << "<h2>attacks (3x3 stats)</h2>" << std::endl;
  fp << "<table class=\"evenshade fastattacks\">" << std::endl;
  fp << "<tr><th>T</th><th>Attack</th><th>Turns</th><th>Power</th><th>Energy+</th><th>PPT</th><th>EPT</th></tr>" << std::endl;
  for(const auto *a : sortedatks){
    if(!fast_attack_p(a)){
      continue;
    }
    auto cpow = calc_eff_power(s, a);
    fp << "<tr>";
    fp << "<td>";
    html_type_pdir(fp, a->type);
    fp << "</td>";
    fp << "<td>";
    emit_html_attack(fp, s, a);
    fp << "</td>";
    fp << "<td>";
    if(a->turns){
      fp << a->turns;
    }
    fp << "</td>";
    fp << "<td>" << cpow << "</td>";
    if(charged_attack_p(a)){
      fp << "<td>" << -a->energytrain << "</td>";
    }else{
      fp << "<td>" << a->energytrain << "</td>";
    }
    fp << "<td>" << (cpow / a->turns) << "</td>";
    fp << "<td>" << (static_cast<float>(a->energytrain) / a->turns) << "</td>";
    fp << "</tr>" << std::endl;
  }
  fp << "</table>" << std::endl;
  fp << "<table class=\"evenshade chargedattacks\">" << std::endl;
  fp << "<tr><th>T</th><th>Attack</th><th>Buffs</th><th>Power</th><th>Energy-</th><th>PPE</th></tr>" << std::endl;
  for(const auto *a : sortedatks){
    if(!charged_attack_p(a)){
      continue;
    }
    auto cpow = calc_eff_power(s, a);
    fp << "<tr>";
    fp << "<td>";
    html_type_pdir(fp, a->type);
    fp << "</td>";
    fp << "<td>";
    emit_html_attack(fp, s, a);
    fp << "</td>";
    fp << "<td>";
    summarize_buffs_html(fp, a);
    fp << "</td>";
    fp << "<td>" << cpow << "</td>";
    fp << "<td>" << -a->energytrain << "</td>";
    fp << "<td>" << (cpow / -a->energytrain) << "</td>";
    fp << "</tr>" << std::endl;
  }
  fp << "</table>" << std::endl;
}

static void
write_mon_attacks_nx1(std::ostream &fp, const species &s){
  std::vector<const attack *> sortedatks(s.attacks);
  if(s.shadow){
    sortedatks.emplace_back(&ATK_Return);
  }
  std::sort(sortedatks.begin(), sortedatks.end(), [s](const attack *lhs, const attack *rhs){
        if(fast_attack_p(lhs) && !fast_attack_p(rhs)){
          return true;
        }else if(!fast_attack_p(lhs) && fast_attack_p(rhs)){
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
    if(!fast_attack_p(a)){
      continue;
    }
    auto cpow = calc_eff_power_nx1(s, a);
    fp << "<tr>";
    fp << "<td>";
    html_type_pdir(fp, a->type);
    fp << "</td>";
    fp << "<td>";
    emit_html_attack(fp, s, a);
    fp << "</td>";
    fp << "<td>";
    if(a->animdur){
      fp << a->animdur;
    }
    fp << "</td>";
    fp << "<td>" << cpow << "</td>";
    fp << "<td>" << a->energyraid << "</td>";
    fp << "<td>" << (cpow / a->animdur) << "</td>";
    fp << "<td>" << (cpow / a->energyraid) << "</td>";
    fp << "</tr>" << std::endl;
  }
  fp << "</table>" << std::endl;
  fp << "<table class=\"evenshade chargedattacks\">" << std::endl;
  fp << "<tr><th>T</th><th>Attack</th><th>Turns</th><th>Power</th><th>Energy-</th><th>PPT</th><th>PPE</th></tr>" << std::endl;
  for(const auto *a : sortedatks){
    if(!charged_attack_p(a)){
      continue;
    }
    auto cpow = calc_eff_power_nx1(s, a);
    fp << "<tr>";
    fp << "<td>";
    html_type_pdir(fp, a->type);
    fp << "</td>";
    fp << "<td>";
    emit_html_attack(fp, s, a);
    fp << "</td>";
    fp << "<td>";
    if(a->animdur){
      fp << a->animdur;
    }
    fp << "</td>";
    fp << "<td>" << cpow << "</td>";
    fp << "<td>" << a->energyraid << "</td>";
    fp << "<td>" << (cpow / a->animdur) << "</td>";
    fp << "<td>" << (cpow / a->energyraid) << "</td>";
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
  fp << "Attack / Defense: " << (static_cast<float>(s.atk) / s.def) << "<br/>";
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
  if(cstr){
    fp << cstr << "<br/>";
  }
  fp << "</div>" << std::endl;
  return 0;
}

static int
print_previous_species(std::ostream &fp, const species &s){
  int ret = 1;
  const species *devol = get_previous_evolution(s);
  if(devol){
    ret += print_previous_species(fp, *devol);
  }
  link_to_name(fp, s.name);
  fp << " → ";
  return ret;
}

static int
write_evol(std::ostream &fp, const species &s){
  fp << "<h2>transitions</h2>";
  const species *devol = get_previous_evolution(s);
  int evolidx = 0;
  std::vector<const species*> evols;
  int rows = get_evolution_count(s, evols);
  if(devol || rows){
    // we need a table because the evolution can fan out
    if(rows == 0){
      rows = 1;
    }
    int immindex = -1; // see comment below
    std::vector<const species*> immevols;
    std::forward_list<species> store;
    get_evolutions(&s, immevols, store);
    for(int r = 0 ; r < rows ; ++r){
      if(r){
        fp << std::endl;
      }
      // first, print previous step(s)
      if(devol){
        print_previous_species(fp, *devol);
      }
      // next, print ourselves, in bold
      fp << "<b>" << s.name << "</b>";
      // now, the next evolutionary step(s), if they exist. we do only one row.
      // this requires knowing our index in the immevols array and the next
      // entry in the evols array. when we come across the next immevols entry
      // in evols, update immindex and pop. then print any successor and pop.
      if(evols.size()){
        if(immindex + 1u < immevols.size() && immevols[immindex + 1] == evols[evolidx]){
          ++immindex;
          ++evolidx;
        }
        const auto imm = immevols[immindex];
        fp << " → ";
        link_to_name(fp, imm->name);
        std::vector<const species*> waste;
        if(get_persistent_evolutions(*imm, waste)){
          fp << " → ";
          link_to_name(fp, evols[evolidx]->name);
          ++evolidx;
          ++r;
        }
        fp << "<br/>";
      }
    }
  }else{
    fp << "No evolution";
  }
  return 0;
}

static int
write_mon_page(const species &s){
  std::string encname;
  encode_name(s.name, encname);
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
  // for gmax, show logo instead of checkmark
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
