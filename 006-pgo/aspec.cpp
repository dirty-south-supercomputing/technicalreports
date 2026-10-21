#include "html.h"
#include <fstream>
#include <unistd.h>

static void
write_atk_3x3_row(std::ostream &fp, const attack &a, bool stab, int buff){
  fp << "<tr><td>";
  if(stab){
    fp << "✓";
  }else{
    fp << "🗙";
  }
  if(buff){
    fp << " (" << buff << ")";
  }
  fp << "</td>";
  float energy = a.energytrain;
  float power = a.powertrain;
  if(stab){
    power = calc_stab(power);
  }
  if(charged_attack_p(a)){
    energy = -energy;
  }
  power *= mapbuff(buff);
  if(fast_attack_p(a)){
    fp << "<td>" << a.turns << "</td>";
  }
  fp << "<td>" << energy << "</td>";
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
  if(fast_attack_p(a)){
    fp << "<td>" << (energy / a.turns) << "</td>";
  }
  fp << "</tr>" << std::endl;
}

static void
write_atk_3x3(std::ostream &fp, const attack &a){
  fp << "<h2>3x3 stats</h2>" << std::endl;
  fp << "<div>" << std::endl;
  summarize_buffs_html(fp, a, "<div id=\"stateffect\">Status effect: ", "</div>");
  if(fast_attack_p(a)){
    fp << "<table id=\"fattack3x3\">" << std::endl;
  }else{
    fp << "<table id=\"cattack3x3\">" << std::endl;
  }
  fp << "<tr>";
  fp << "<th>STAB?</th>";
  if(fast_attack_p(a)){
    fp << "<th>Turns</th>";
  }
  fp << "<th>E</th>";
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
  if(fast_attack_p(a)){
    fp << "<th>EPT</th>";
  }
  fp << "</tr>" << std::endl;
  for(int i = -MAXCHARGEDBUFF ; i < 0 ; ++i){
    write_atk_3x3_row(fp, a, false, i);
    write_atk_3x3_row(fp, a, true, i);
  }
  write_atk_3x3_row(fp, a, false, 0);
  write_atk_3x3_row(fp, a, true, 0);
  for(int i = 1 ; i < MAXCHARGEDBUFF + 1 ; ++i){
    write_atk_3x3_row(fp, a, false, i);
    write_atk_3x3_row(fp, a, true, i);
  }
  fp << "</table>" << std::endl;
  fp << "</div>" << std::endl; // buffs + table div

}

static void
write_atk_Nx1_row(std::ostream &fp, const attack &a, bool stab){
  fp << "<tr><td>";
  if(stab){
    fp << "✓";
  }else{
    fp << "🗙";
  }
  fp << "</td>";
  float energy = a.energyraid;
  float power = a.powerraid;
  if(stab){
    power = calc_stab(power);
  }
  fp << "<td>" << a.animdur << "</td>";
  fp << "<td>" << energy << "</td>";
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
  if(fast_attack_p(a)){
    fp << "<td>" << (energy / a.animdur) << "</td>";
  }
  fp << "</tr>" << std::endl;
}

static void
write_atk_Nx1(std::ostream &fp, const attack &a){
  fp << "<h2>Nx1 stats</h2>" << std::endl;
  if(fast_attack_p(a)){
    fp << "<table id=\"fattackNx1\">" << std::endl;
  }else{
    fp << "<table id=\"cattackNx1\">" << std::endl;
  }
  fp << "<tr>";
  fp << "<th>STAB?</th>";
  fp << "<th>Turns</th>";
  fp << "<th>E</th>";
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
  if(fast_attack_p(a)){
    fp << "<th>EPT</th>";
  }
  fp << "</tr>" << std::endl;
  write_atk_Nx1_row(fp, a, false);
  write_atk_Nx1_row(fp, a, true);
  fp << "</table>" << std::endl;
}

static void
write_atk_effects(std::ostream &fp, const attack &a){
  auto ae = adventure_effect(a.name);
  if(ae.empty()){
    return;
  }
  fp << "<h2>Adventure Effect</h2>" << std::endl;
  fp << ae << std::endl;
}

static void
write_atk_user(std::ostream &fp, const std::string &s, bool &printed, bool elitetm, bool stab){
  if(printed){
    fp << ", ";
  }else{
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
        if(m.plusatk && m.plusatk->name == a.name){
          bool stab = has_stab_p({&*s, m}, &a);
          write_atk_user(fp, m.name, printed, false, stab);
        }
      }
    }
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
  fp << std::setprecision(3);
  write_header(fp, a.name);
  fp << "<h1>" << a.name << ' ';
  html_type_pdir(fp, a.type);
  if(fast_attack_p(a)){
    fp << " fast";
  }else{
    fp << " charged";
  }
  fp << "</h1>" << std::endl;
  write_atk_effects(fp, a);
  write_atk_3x3(fp, a);
  write_atk_Nx1(fp, a);
  // FIXME moar crap ... similar attacks, graphs
  write_atk_users(fp, a);
  write_footer_and_close(fp);
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
  return EXIT_SUCCESS;
}
