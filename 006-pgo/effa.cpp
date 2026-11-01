#include "html.h"

static int
emit_effa(std::ostream &fp, int cpthresh, int cpbound, const species &s){
  unsigned ivcount;
  auto sets = order_ivs(s, cpbound, statscmp_gmean, &ivcount);
  float effamin = 0, effamax = 0;
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
    if(st.effa < effamin || effamin == 0){
      effamin = st.effa;
    }
    if(st.effa > effamax){
      effamax = st.effa;
    }
  }
  if(threshmet){
    fp << "<tr>";
    fp << "<td>" << effamin << "</td><td>" << effamax << "</td><td>" << s.name << "</td>";
    fp << "</tr>" << std::endl;
  }
  delete[] sets;
  return 0;
}

static int
effa_table(std::ostream &fp, int cpthresh, int cpbound){
  fp << "<table><tr><th>Low</th><th>High</th><th>Form</th></tr>" << std::endl;
  std::vector<species> megas;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    emit_effa(fp, cpthresh, cpbound, *s);
    for(const auto &m : s->mforms){
      megas.emplace_back(&*s, m);
    }
  }
  for(const auto &m : megas){
    emit_effa(fp, cpthresh, cpbound, m);
  }
  fp << "</table>" << std::endl;
  return 0;
}

[[noreturn]] static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " GL|UL|ML" << std::endl;
  exit(EXIT_FAILURE);
}

// generate three tables of eff_a ranges for each form, for graphing
int main(int argc, const char **argv){
  if(argc != 2){
    usage(argv[0]);
  }
  int ret;
  if(strcmp(argv[1], "GL") == 0){
    ret = effa_table(std::cout, 0, GLCPCAP);
  }else if(strcmp(argv[1], "UL") == 0){
    ret = effa_table(std::cout, GLCPCAP, ULCPCAP);
  }else if(strcmp(argv[1], "ML") == 0){
    ret = effa_table(std::cout, ULCPCAP, 0);
  }else{
    usage(argv[0]);
  }
  return ret;
}
