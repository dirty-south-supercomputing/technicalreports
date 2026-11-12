#include "html.h"

struct shadowcand {
  const species *s;
  double ameanreg;
  double ameanshad;
  double ameanrat;

  shadowcand(const species *S) :
    s(S)
  {
    auto mhp = calc_mhp(s->def, MAX_HALFLEVEL_BASIC);
    auto effareg = calc_eff_a(s->atk, MAX_HALFLEVEL_BASIC, false);
    auto effdreg = calc_eff_d(s->def, MAX_HALFLEVEL_BASIC, false);
    ameanreg = calc_amean(effareg, effdreg, mhp);
    auto effashad = calc_eff_a(s->atk, MAX_HALFLEVEL_BASIC, true);
    auto effdshad = calc_eff_d(s->def, MAX_HALFLEVEL_BASIC, true);
    ameanshad = calc_amean(effashad, effdshad, mhp);
    ameanrat = ameanshad / ameanreg;
  }

  void emit(std::ostream &fp){
    fp << "<tr><td>";
    html_types(fp, s->t1, s->t2, false);
    fp << "</td>";
    fp << "<td>";
    link_to_name(fp, s->name, true);
    fp << "</td>";
    auto rat = (ameanrat - 1) * 100;
    fp << "<td>" << rat << "%</td>";
    fp << "<td>" << purification_cost(*s) << "</td>";
    fp << "</tr>" << std::endl;
  }

  friend bool operator<(const shadowcand &lhs, const shadowcand &rhs){
    if(lhs.ameanrat < rhs.ameanrat){
      return true;
    }else if(rhs.ameanrat < lhs.ameanrat){
      return false;
    }
    return false;
  }

};

static void
calc_shadow(std::vector<shadowcand> &cands, const species &s){
  cands.emplace_back(&s);
}

// print a list of all forms with a shadow, cost to purify,
// and change in arithmetic mean for #1 in each league
int main(void){
  std::vector<shadowcand> cands;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    if(s->shadow){
      calc_shadow(cands, *s);
    }
  }
  std::sort(cands.begin(), cands.end());
  std::cout << std::setprecision(3);
  std::cout << "<table class=\"evenshade shadows\">" << std::endl;
  std::cout << "<tr><th>T</th><th>Form</th><th>ΔAmean</th><th>Purification (kSD)</th></tr>" << std::endl;
  for(auto &s : cands){
    s.emit(std::cout);
  }
  std::cout << "</table>" << std::endl;
  return EXIT_SUCCESS;
}
