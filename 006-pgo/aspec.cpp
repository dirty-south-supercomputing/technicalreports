#include "html.h"
#include <fstream>
#include <unistd.h>

static void
write_atk_3x3_row(std::ostream &fp, const attack &a, bool stab, int buff, float plusmult){
  fp << "<tr><td>";
  if(stab){
    fp << "✓";
  }else{
    fp << "🛇";
  }
  fp << "</td><td>";
  if(buff){
    fp << buff;
  }
  fp << "</td>";
  float energy = a.energytrain;
  float power = a.powertrain;
  if(stab){
    power = calc_stab(power);
  }
  power *= plusmult;
  if(charged_attack_p(a)){
    energy = -energy;
  }
  power *= mapbuff(buff);
  for(int te = -3 ; te < 3 ; ++te){
    fp << "<td class=\"" << css[te + 3] << "\">" << (power * type_effectiveness_mult(te)) << "</td>";
  }
  float pp = power;
  if(fast_attack_p(a)){
    pp /= a.turns;
  }else{
    pp /= energy;
  }
  for(int te = -3 ; te < 3 ; ++te){
    fp << "<td class=\"" << css[te + 3] << "\">" << (pp * type_effectiveness_mult(te)) << "</td>";
  }
  fp << "</tr>" << std::endl;
}

// emit a header for levels 1..3 (*not* 0)
static void
emit_plus_header(std::ostream &fp, int level, float pmult){
  if(level){
    fp << "<h3>super mega level " << level + 1 << " (" << pmult << "x)</h3>" << std::endl;
  }
}

static void
write_atk_3x3(std::ostream &fp, const attack &a){
  fp << "<h2>3x3 stats</h2>" << std::endl;
  fp << "<div class=\"atktext\">Status effects can have an effect from -4 to 4 on attacks.";
  if(charged_attack_p(a)){
    fp << " All charged attacks take one turn in 3x3 mode. " << a.name << " ";
  }else{
    fp << " " << a.name << " is a " << a.turns << "-turn 3x3 move, and ";
  }
  if(charged_attack_p(a)){
    fp << "consumes " << -a.energytrain << " energy.";
  }else{
    fp << "generates ";
    fp << a.energytrain << " energy (";
    fp << a.energytrain / static_cast<float>(a.turns);
    fp << " EPT).";
  }
  fp << "</div><br/>";
  bool plusatk = plus_attack_p(a);
  int maxlevel = plusatk ? 4 : 1;
  for(int meglevel = 0 ; meglevel < maxlevel ; ++meglevel){
    float plusmult = 1.0 + 0.1 * meglevel;
    emit_plus_header(fp, meglevel, plusmult);
    fp << "<div class=\"tablecontainer\">" << std::endl;
    summarize_buffs_html(fp, a, "<div class=\"stateffect\">Status effect: ", "</div>");
    if(fast_attack_p(a)){
      fp << "<table class=\"fattack3x3\">" << std::endl;
    }else{
      fp << "<table class=\"cattack3x3\">" << std::endl;
    }
    fp << "<tr>";
    fp << "<th>S?</th>";
    fp << "<th>SE</th>";
    for(int te = -3 ; te < 3 ; ++te){
      fp << "<th class=\"" << css[te + 3] << "\">P<sub>" << te << "</sub></th>";
    }
    for(int te = -3 ; te < 3 ; ++te){
      if(fast_attack_p(a)){
        fp << "<th class=\"" << css[te + 3] << "\">PPT<sub>" << te << "</sub></th>";
      }else{
        fp << "<th class=\"" << css[te + 3] << "\">PPE<sub>" << te << "</sub></th>";
      }
    }
    fp << "</tr>" << std::endl;
    for(int i = -MAXCHARGEDBUFF ; i < 0 ; ++i){
      write_atk_3x3_row(fp, a, false, i, plusmult);
      write_atk_3x3_row(fp, a, true, i, plusmult);
    }
    write_atk_3x3_row(fp, a, false, 0, plusmult);
    write_atk_3x3_row(fp, a, true, 0, plusmult);
    for(int i = 1 ; i < MAXCHARGEDBUFF + 1 ; ++i){
      write_atk_3x3_row(fp, a, false, i, plusmult);
      write_atk_3x3_row(fp, a, true, i, plusmult);
    }
    fp << "</table></div>" << std::endl;
  }
}

static void
emit_option(std::ostream &fp, bool op){
  fp << "<td>";
  if(op){
    fp << "✓";
  }else{
    fp << "🛇";
  }
  fp << "</td>";
}

static void
write_matk_Nx1_row(std::ostream &fp, const unsigned *plevs, unsigned level,
                   bool stab, bool wboost, bool bblade){
  if(bblade && level >= MAXATK_MAX_LEVEL){
    std::cerr << "dynamax cannon cannot be used with behemoth blade" << std::endl;
    throw std::exception();
  }
  fp << "<tr>";
  emit_option(fp, stab);
  emit_option(fp, wboost);
  emit_option(fp, bblade);
  fp << "<td>";
  fp << level + 1;
  fp << "</td>";
  float power = plevs[level];
  if(stab){
    power = calc_stab(power);
  }
  if(wboost){
    power = calc_wboost(power);
  }
  if(bblade){
    power = power * BBLADE_MAX_BOOST;
  }
  for(int te = -3 ; te < 3 ; ++te){
    fp << "<td class=\"" << css[te + 3] << "\">" << (power * type_effectiveness_mult(te)) << "</td>";
  }
  fp << "</tr>" << std::endl;
}

static void
write_atk_Nx1_row(std::ostream &fp, const attack &a, float megabonus, bool stab, bool wboost, float plusmult){
  fp << "<tr><td>";
  if(stab){
    fp << "✓";
  }else{
    fp << "🛇";
  }
  fp << "</td><td>";
  if(wboost){
    fp << "✓";
  }else{
    fp << "🛇";
  }
  fp << "</td><td>";
  if(megabonus){
    fp << (megabonus * 100) << '%';
  }else{
    fp << "🛇";
  }
  fp << "</td>";
  float energy = a.energyraid;
  float power = a.powerraid;
  if(stab){
    power = calc_stab(power);
  }
  if(wboost){
    power = calc_wboost(power);
  }
  power *= plusmult;
  power *= (1 + megabonus);
  for(int te = -3 ; te < 3 ; ++te){
    fp << "<td class=\"" << css[te + 3] << "\">" << (power * type_effectiveness_mult(te)) << "</td>";
  }
  float ppt = power / a.animdur;
  if(charged_attack_p(a)){
    float ppe = power / energy;
    for(int te = -3 ; te < 3 ; ++te){
      fp << "<td class=\"" << css[te + 3] << "\">" << (ppe * type_effectiveness_mult(te)) << "</td>";
    }
  }
  for(int te = -3 ; te < 3 ; ++te){
    fp << "<td class=\"" << css[te + 3] << "\">" << (ppt * type_effectiveness_mult(te)) << "</td>";
  }
  fp << "</tr>" << std::endl;
}

static void
write_atk_Nx1(std::ostream &fp, const attack &a){
  fp << "<h2>Nx1 stats</h2>" << std::endl;
  bool plusatk = plus_attack_p(a);
  int maxlevel = plusatk ? 4 : 1;
  fp << "<div class=\"atktext\">";
  fp << " " << a.name << " is a " << a.animdur << "-turn Nx1 move, and";
  if(charged_attack_p(a)){
    fp << " consumes " << a.energyraid;
  }else{
    fp << " generates " << a.energyraid;
  }
  fp << " energy (";
  fp << a.energyraid / static_cast<float>(a.animdur);
  fp << " EPT). It is boosted by ";
  if(a.type != TYPECOUNT){
    html_weather(fp, a.type, TYPECOUNT, true);
  }else{
    fp << "various"; // hidden power strikes again
  }
  fp << " weather.</div><br/>";
  for(int meglevel = 0 ; meglevel < maxlevel ; ++meglevel){
    float plusmult = 1.0 + 0.1 * meglevel;
    emit_plus_header(fp, meglevel, plusmult);
    if(fast_attack_p(a)){
      fp << "<table class=\"fattackNx1\">" << std::endl;
    }else{
      fp << "<table class=\"cattackNx1\">" << std::endl;
    }
    fp << "<tr>";
    fp << "<th>S?</th>";
    fp << "<th>W?</th>";
    fp << "<th>M</th>";
    for(int te = -3 ; te < 3 ; ++te){
      fp << "<th class=\"" << css[te + 3] << "\">P<sub>" << te << "</sub></th>";
    }
    if(charged_attack_p(a)){
      for(int te = -3 ; te < 3 ; ++te){
        fp << "<th class=\"" << css[te + 3] << "\">PPE<sub>" << te << "</sub></th>";
      }
    }
    for(int te = -3 ; te < 3 ; ++te){
      fp << "<th class=\"" << css[te + 3] << "\">PPT<sub>" << te << "</sub></th>";
    }
    fp << "</tr>" << std::endl;
    write_atk_Nx1_row(fp, a, 0, false, false, plusmult);
    write_atk_Nx1_row(fp, a, 0.1, false, false, plusmult);
    write_atk_Nx1_row(fp, a, 0, true, false, plusmult);
    write_atk_Nx1_row(fp, a, 0, false, true, plusmult);
    write_atk_Nx1_row(fp, a, 0.3, false, false, plusmult);
    write_atk_Nx1_row(fp, a, 0.1, true, false, plusmult);
    write_atk_Nx1_row(fp, a, 0.1, false, true, plusmult);
    write_atk_Nx1_row(fp, a, 0, true, true, plusmult);
    write_atk_Nx1_row(fp, a, 0.3, true, false, plusmult);
    write_atk_Nx1_row(fp, a, 0.3, false, true, plusmult);
    write_atk_Nx1_row(fp, a, 0.1, true, true, plusmult);
    write_atk_Nx1_row(fp, a, 0.3, true, true, plusmult);
    fp << "</table>" << std::endl;
  }
}

static void
write_atk_max(std::ostream &fp, const std::string &name, const unsigned *power, pgo_types_e type){
  fp << "<h2>Max stats</h2>" << std::endl;
  fp << "<p class=\"atktext\">";
  fp << " " << name << " is boosted by ";
  if(type != TYPECOUNT){
    html_weather(fp, type, TYPECOUNT, true);
  }else{
    fp << "various"; // hidden power strikes again
  }
  fp << " weather.<br/>The <a href=\"DynamaxCannon.html\">Dynamax Cannon</a> Adventure Effect boosts ";
  fp << " Max Level by one.<br/>The <a href=\"BehemothBlade.html\">Behemoth Blade</a> Adventure Effect boosts ";
  fp << " power by 5%.<br/>Only one Adventure Effect can be active at a time.</p><br/>";
  fp << "<table class=\"mattackNx1\">" << std::endl;
  fp << "<tr>";
  fp << "<th>S?</th>";
  fp << "<th>W?</th>";
  fp << "<th>BB?</th>";
  fp << "<th>Level</th>";
  for(int te = -3 ; te < 3 ; ++te){
    fp << "<th class=\"" << css[te + 3] << "\">P<sub>" << te << "</sub></th>";
  }
  fp << "</tr>" << std::endl;
  write_matk_Nx1_row(fp, power, 0, false, false, false);
  write_matk_Nx1_row(fp, power, 0, false, false, true);
  write_matk_Nx1_row(fp, power, 1, false, false, false);
  write_matk_Nx1_row(fp, power, 0, true, false, false);
  write_matk_Nx1_row(fp, power, 0, false, true, false);
  write_matk_Nx1_row(fp, power, 1, false, false, true);
  write_matk_Nx1_row(fp, power, 0, true, false, true);
  write_matk_Nx1_row(fp, power, 0, false, true, true);
  write_matk_Nx1_row(fp, power, 2, false, false, false);
  write_matk_Nx1_row(fp, power, 2, false, false, true);
  write_matk_Nx1_row(fp, power, 1, true, false, false);
  write_matk_Nx1_row(fp, power, 1, false, true, false);
  write_matk_Nx1_row(fp, power, 0, true, true, false);
  write_matk_Nx1_row(fp, power, 1, true, false, true);
  write_matk_Nx1_row(fp, power, 1, false, true, true);
  write_matk_Nx1_row(fp, power, 0, true, true, true);
  write_matk_Nx1_row(fp, power, 2, true, false, false);
  write_matk_Nx1_row(fp, power, 2, false, true, false);
  write_matk_Nx1_row(fp, power, 3, false, false, false);
  write_matk_Nx1_row(fp, power, 2, true, false, true);
  write_matk_Nx1_row(fp, power, 2, false, true, true);
  write_matk_Nx1_row(fp, power, 1, true, true, false);
  write_matk_Nx1_row(fp, power, 1, true, true, true);
  write_matk_Nx1_row(fp, power, 2, true, true, false);
  write_matk_Nx1_row(fp, power, 3, true, false, false);
  write_matk_Nx1_row(fp, power, 3, false, true, false);
  write_matk_Nx1_row(fp, power, 2, true, true, true);
  write_matk_Nx1_row(fp, power, 3, true, true, false);
  fp << "</table>" << std::endl;
}

static void
write_atk_effects(std::ostream &fp, const attack &a){
  auto ae = adventure_effect(a.name);
  if(ae.empty()){
    return;
  }
  fp << "<h2>adventure effect</h2>" << std::endl;
  fp << "<div class=\"atktext\">" << ae << "</div>" << std::endl;
}

static void
write_atk_user(std::ostream &fp, const std::string &s, bool &printed, bool elitetm, bool stab){
  if(printed){
    fp << ", ";
  }else{
    fp << "<div class=\"atkusertext\">";
    printed = true;
  }
  if(elitetm){
    fp << "<b>";
  }
  if(!stab){
    fp << "<i>";
  }
  link_to_name(fp, s, true, true);
  if(!stab){
    fp << "</i>";
  }
  if(elitetm){
    fp << "</b>";
  }
}

static void
write_atk_users(std::ostream &fp, const attack &a){
  fp << "<h2>users</h2>" << std::endl;
  bool printed = false;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    bool matched = false;
    for(const auto sa : s->attacks){
      if(a.name == sa->name){
        matched = true;
        bool stab = has_stab_p(*s, &a);
        bool elitetm = exclusive_attack_p(*s, &a);
        write_atk_user(fp, s->name, printed, elitetm, stab);
        for(const auto &m : s->mforms){
          stab = has_stab_p({&*s, m}, &a);
          write_atk_user(fp, m.name, printed, elitetm, stab);
        }
        break;
      }
    }
    if(!matched){
      for(const auto &m : s->mforms){
        if(m.plusatk && attack_named_p(m.plusatk, a.name)){
          bool stab = has_stab_p({&*s, m}, &a);
          write_atk_user(fp, m.name, printed, false, stab);
        }
      }
    }
  }
  if(printed){
    fp << "</div>" << std::endl;
  }
}

static void
write_atk_teffect(std::ostream &fp, pgo_types_e atype){
  fp << "<h2>type effectiveness</h2>" << std::endl;
  fp << "<table class=\"evenshade\" id=\"teffect\">" << std::endl;
  fp << "<tr>";
  fp << "<th></th>";
  for(pgo_types_e t = TYPESTART ; t < TYPECOUNT ; ++t){
    fp << "<th>";
    html_type(fp, t, true);
    fp << "</th>";
  }
  fp << "</tr>" << std::endl;
  for(pgo_types_e t1 = TYPESTART ; t1 < TYPECOUNT ; ++t1){
    fp << "<tr><td>";
    html_type(fp, t1, true);
    fp << "</td>";
    for(pgo_types_e t2 = TYPESTART ; t2 < TYPECOUNT ; ++t2){
      if(t1 > t2){
        fp << "<td style=\"color:#808080\">";
      }else{
        fp << "<td>";
      }
      auto te = typing_relation(atype, t1, t2);
      if(te){
        fp << te;
      }
      fp << "</td>";
    }
    fp << "</tr>";
  }
  fp << "</table>" << std::endl;
}

static void
write_max_atk_users(std::ostream &fp, pgo_types_e type){
  fp << "<h2>users</h2>" << std::endl;
  bool printed = false;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(!s->dmax || s->dmax == GMAX_TIER){
      continue;
    }
    for(const auto sa : s->attacks){
      if(!fast_attack_p(*sa)){
        continue;
      }
      bool stab = has_stab_raw_p(*s, type);
      write_atk_user(fp, s->name, printed, false, stab);
      break;
    }
  }
  if(printed){
    fp << "</div>" << std::endl;
  }
}

// assume we're in the target directory
static int
write_max_atk_page(const std::string &name, pgo_types_e atype){
  std::string encname;
  encode_name(name, encname);
  std::ofstream fp(encname + ".html");
  if(!fp.is_open()){
    std::cerr << "error opening " << encname << ".html for writing" << std::endl;
    return -1;
  }
  fp << std::setprecision(4);
  write_header(fp, name, true);
  fp << "<h1>";
  html_type(fp, atype, true);
  fp << " " << name << " (" << tname_capitalized(atype) << " Max attack)</h1>" << std::endl;
  write_atk_teffect(fp, atype);
  write_atk_max(fp, name, DMAX_POWER, atype);
  write_max_atk_users(fp, atype);
  write_footer_and_close(fp, true);
  return 0;
}

static int
write_gmax_atk_page(const gmaxattack *gm){
  std::string encname;
  encode_name(gm->name, encname);
  std::ofstream fp(encname + ".html");
  if(!fp.is_open()){
    std::cerr << "error opening " << encname << ".html for writing" << std::endl;
    return -1;
  }
  fp << std::setprecision(4);
  write_header(fp, gm->name, true);
  fp << "<h1>";
  html_type(fp, gm->type, true);
  fp << gm->name << " (" << tname_capitalized(gm->type) << " G-Max attack)</h1>" << std::endl;
  write_atk_teffect(fp, gm->type);
  write_atk_max(fp, gm->name, GMAX_POWER, gm->type);
  fp << "<h2>users</h2>" << std::endl;
  const auto *s = lookup_species(gm->sname);
  if(!s){
    throw std::exception();
  }
  bool stab = has_stab_raw_p(*s, gm->type);
  std::string sname = "G-Max " + s->name;
  bool printed = false;
  write_atk_user(fp, sname, printed, false, stab);
  if(printed){
    fp << "</div>" << std::endl;
  }
  write_footer_and_close(fp, true);
  return 0;
}

// several standard charged attacks can also be used as max attacks.
// handle them as special cases.
static void
write_freak_matks(std::ostream &fp, const attack &a){
  if(a.name == "Behemoth Blade" || a.name == "Behemoth Bash"){
    write_atk_max(fp, a.name, DMAX_POWER, a.type);
  }else if(a.name == "Dynamax Cannon"){
    write_atk_max(fp, a.name, GMAX_POWER, a.type);
  }
}

// assume we're in the target directory
static int
write_atk_page(const attack &a){
  std::string encname;
  encode_name(a.name, encname);
  std::ofstream fp(encname + ".html");
  if(!fp.is_open()){
    std::cerr << "error opening " << encname << ".html for writing" << std::endl;
    return -1;
  }
  fp << std::setprecision(4);
  write_header(fp, a.name, true);
  fp << "<h1>";
  if(a.type != TYPECOUNT){
    html_type(fp, a.type, true);
  }
  fp << " " << a.name << " (";
  if(a.type != TYPECOUNT){
    fp << tname_capitalized(a.type);
  }else{
    fp << "Various"; // ugh, hidden power
  }
  if(fast_attack_p(a)){
    fp << " fast attack)";
  }else{
    if(plus_attack_p(a)){
      fp << " super mega";
    }
    fp << " charged attack)";
  }
  fp << "</h1>" << std::endl;
  write_atk_effects(fp, a);
  if(a.type != TYPECOUNT){
    write_atk_teffect(fp, a.type);
  }
  write_atk_3x3(fp, a);
  write_atk_Nx1(fp, a);
  write_freak_matks(fp, a);
  // FIXME moar crap ... similar attacks, graphs
  write_atk_users(fp, a);
  write_footer_and_close(fp, true);
  return 0;
}

static void
usage(const char *argv0, int ret){
  std::cerr << "usage: " << argv0 << " targetdir" << std::endl;
  exit(ret);
}

// generate a directory full of attack pages, overwriting any that exist.
int main(int argc, const char **argv){
  if(argc != 2){
    usage(argv[0], EXIT_FAILURE);
  }
  if(chdir(argv[1])){
    std::cerr << "error changing directory to " << argv[1] << ": " << strerror(errno) << std::endl;
    usage(argv[0], EXIT_FAILURE);
  }
  for(auto a = attacks_begin() ; a != attacks_end() ; ++a){
    if(write_atk_page(**a)){
      return EXIT_FAILURE;
    }
  }
  // get the plus attacks from mega definitions
  for(auto s = species_begin() ; s != species_end() ; ++s){
    for(const auto &m : s->mforms){
      if(m.plusatk){
        if(write_atk_page(*m.plusatk)){
          return EXIT_FAILURE;
        }
      }
    }
  }
  // one dynamax attack per type
  for(pgo_types_e t = TYPESTART ; t < TYPECOUNT ; ++t){
    const auto name = max_attack_name(t);
    if(write_max_atk_page(name, t)){
      return EXIT_FAILURE;
    }
  }
  // do gmax attacks; dogs and eternatus must be merged into their attack pages! FIXME
  for(const auto *g = gmaxattacks_begin() ; g != gmaxattacks_end() ; ++g){
    if(write_gmax_atk_page(g)){
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
