#include "html.h"
#include <fstream>
#include <unistd.h>

static void
write_atk_3x3_row(std::ostream &fp, const attack &a, bool stab){
  fp << "<tr><td>";
  if(stab){
    fp << "✓";
  }else{
    fp << "🗙";
  }
  fp << "</td>";
  float energy = a.energytrain;
  float power = a.powertrain;
  if(stab){
    power = calc_stab(power);
  }
  if(fast_attack_p(a)){
    fp << "<td>" << a.turns << "</td>";
  }
  for(int te = -3 ; te < 3 ; ++te){
    fp << "<td>" << (power * type_effectiveness_mult(te)) << "</td>";
  }
  fp << "<td>" << energy << "</td>";
  float pp = power;
  if(fast_attack_p(a)){
    pp /= a.turns;
  }else{
    pp /= -energy;
  }
  for(int te = -3 ; te < 3 ; ++te){
    fp << "<td>" << (pp * type_effectiveness_mult(te)) << "</td>";
  }
  if(fast_attack_p(a)){
    fp << "<td>" << (energy / a.turns) << "</td>";
  }
  fp << "</tr>" << std::endl;
}

static void
write_atk_3x3(std::ostream &fp, const attack &a){
  fp << "<h2>3x3 stats</h2>" << std::endl;
  // FIXME detail buff if charged
  fp << "<table id=\"attack3x3\">" << std::endl;
  fp << "<tr>";
  fp << "<th>STAB?</th>";
  if(fast_attack_p(a)){
    fp << "<th>Turns</th>";
  }
  for(int te = -3 ; te < 3 ; ++te){
    fp << "<th>P<sub>" << te << "</sub></th>";
  }
  fp << "<th>E</th>";
  for(int te = -3 ; te < 3 ; ++te){
    if(fast_attack_p(a)){
      fp << "<th>PPT<sub>" << te << "</sub></th>";
    }else{
      fp << "<th>PPE<sub>" << te << "</sub></th>";
    }
  }
  if(fast_attack_p(a)){
    fp << "<th>EPT</th>";
  }
  fp << "</tr>" << std::endl;
  write_atk_3x3_row(fp, a, false);
  write_atk_3x3_row(fp, a, true);
  fp << "</table>" << std::endl;
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
  for(int te = -3 ; te < 3 ; ++te){
    fp << "<td>" << (power * type_effectiveness_mult(te)) << "</td>";
  }
  fp << "<td>" << energy << "</td>";
  float pp = power;
  if(fast_attack_p(a)){
    pp /= a.animdur;
  }else{
    pp /= energy;
  }
  for(int te = -3 ; te < 3 ; ++te){
    fp << "<td>" << (pp * type_effectiveness_mult(te)) << "</td>";
  }
  if(fast_attack_p(a)){
    fp << "<td>" << (energy / a.animdur) << "</td>";
  }
  fp << "</tr>" << std::endl;
}

static void
write_atk_Nx1(std::ostream &fp, const attack &a){
  fp << "<h2>Nx1 stats</h2>" << std::endl;
  fp << "<table id=\"attackNx1\">" << std::endl;
  fp << "<tr>";
  fp << "<th>STAB?</th>";
  fp << "<th>Turns</th>";
  for(int te = -3 ; te < 3 ; ++te){
    fp << "<th>P<sub>" << te << "</sub></th>";
  }
  fp << "<th>E</th>";
  for(int te = -3 ; te < 3 ; ++te){
    if(fast_attack_p(a)){
      fp << "<th>PPT<sub>" << te << "</sub></th>";
    }else{
      fp << "<th>PPE<sub>" << te << "</sub></th>";
    }
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
  fp << "<h2>Adventure Effects</h2>" << std::endl;
  // FIXME
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
  write_atk_3x3(fp, a);
  write_atk_Nx1(fp, a);
  write_atk_effects(fp, a);
  // FIXME moar crap ... populations, similar attacks
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
