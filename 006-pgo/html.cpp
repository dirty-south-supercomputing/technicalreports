#include "html.h"

// it's always [ -3, 2 ], with 0 (neutral) at the fourth column, which must be white
const std::string reversecss[6] = { "teneg3", "teneg2", "teneg1", "tepos0", "tepos1", "tepos2", };
const std::string css[6] = { "tepos3", "tepos2", "tepos1", "tepos0", "teneg1", "teneg2", };

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
  fp << "</ul></nav>" << std::endl;
}

void write_header(std::ofstream &fp, const std::string &title, bool par){
  fp << "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  fp << "<link rel=\"stylesheet\" type=\"text/css\" href=\"../pgo.css\">";
  fp << "<title>" << title << "</title></head>" << std::endl;
  fp << "<body>" << std::endl;
  write_navbar(fp, par);
}

void html_weather(std::ostream &fp, pgo_weather_t w, bool par){
  fp << "<img src=\"";
  if(par){
    fp << "../";
  }
  fp << "images/" << WNames[w] << "-icon.png\" class=\"type\" alt=\""<< WNames[w] << "\"/> (";
  fp << WNames[w] << ")";
}

void html_weather(std::ostream &fp, pgo_types_e t1, pgo_types_e t2, bool par){
  pgo_weather_t w1, w2;
  if(t2 == TYPECOUNT){
    t2 = t1;
  }
  typing_weather_boosts(t1, t2, w1, w2);
  html_weather(fp, w1, par);
  if(w2 != w1){
    fp << ", ";
    html_weather(fp, w2, par);
  }
}

void emit_cand_html(const candidate& c, unsigned maxp){
  auto rp = c.powprod();
  //unsigned hhalf;
  std::cout << "<tr>";
  std::cout << "<td>";
  html_types(c.s->t1, c.s->t2);
  std::cout << "</td>";
  std::cout << "<td>";
  link_to_name(std::cout, c.s->name, true);
  std::cout << "</td>";
  std::cout << "<td>";
  html_type(c.atype);
  std::cout << ' ';
  // we do not attempt to indicate that a max attack is based on a legacy attack,
  // since it is possible that more than one fast attack enable the same max
  // attack, and they could have different legacy status (this happens with e.g.
  // dynamax machamp, which could have max knuckle from karate chop or counter).
  if(!c.hasstab){
    std::cout << "<i>";
  }
  std::cout << c.aname;
  if(!c.hasstab){
    std::cout << "</i>";
  }
  std::cout << "</td>";
  std::cout << std::setprecision(0) << std::fixed;
  for(int i = -3 ; i < 3 ; ++i){
    std::cout << "<td class=\"" << css[i + 3] << "\">" << (rp * type_effectiveness_mult(i)) << "</td>";
  }
  std::cout << std::setprecision(2) << std::fixed << "<td>" << (rp * 100.0 / maxp) << "%</td>";
  std::cout << std::setprecision(0) << std::fixed;
  std::cout << "</tr>" << std::endl;
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
  ttfs.emplace_back(&s, t, pfast, f, c);
}

void calctimetos3x3(std::vector<timetofirst> &ttfs, const species &s){
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
  }
}
