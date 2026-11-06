#include "html.h"

struct cmprange {
  const species *s;
  double effamin;
  double effamax;

  friend bool operator<(const cmprange &lhs, const cmprange &rhs){
    if(lhs.effamax < rhs.effamax){
      return true;
    }else if(rhs.effamax < lhs.effamax){
      return false;
    }else if(lhs.effamin < rhs.effamin){
      return true;
    }else if(rhs.effamin < lhs.effamin){
      return false;
    }
    return false; // equal?
  }
};

static void
calc_effa(std::vector<cmprange> &effas, int cpthresh, int cpbound, const species *s){
  unsigned ivcount;
  auto sets = order_ivs(*s, cpbound, statscmp_gmean, &ivcount);
  cmprange crange{s, 0, 0};
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
    if(st.effa < crange.effamin || crange.effamin == 0){
      crange.effamin = st.effa;
    }
    if(st.effa > crange.effamax){
      crange.effamax = st.effa;
    }
  }
  if(threshmet){
    effas.push_back(crange);
  }
  delete[] sets;
}

static void
emit_effa(std::ostream &fp, const cmprange &crange){
  fp << "<tr>";
  fp << "<td>" << crange.effamin << "</td><td>" << crange.effamax << "</td><td>" << crange.s->name << "</td>";
  fp << "</tr>" << std::endl;
}

static int
cmp_graph(std::ostream &fp, int cpthresh, int cpbound){
  std::vector<cmprange> effas;
  std::vector<species> megas;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    calc_effa(effas, cpthresh, cpbound, &*s);
    for(const auto &m : s->mforms){
      megas.emplace_back(&*s, m);
    }
  }
  for(const auto &m : megas){
    calc_effa(effas, cpthresh, cpbound, &m);
  }
  std::sort(effas.begin(), effas.end());
  fp << "<svg role=\"img\" aria-label=\"CMP chart\"";
  fp << " xmlns=\"http://www.w3.org/2000/svg\"";
  fp << " stroke=\"black\" fill=\"green\">" << std::endl;
  for(const auto &e : effas){
    emit_effa(fp, e);
  }
  fp << "</svg>" << std::endl;
  return 0;
}

[[noreturn]] static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " GL|UL|ML" << std::endl;
  exit(EXIT_FAILURE);
}

// generate SVG CMP graph for a league
int main(int argc, const char **argv){
  if(argc != 2){
    usage(argv[0]);
  }
  int ret;
  if(strcmp(argv[1], "GL") == 0){
    ret = cmp_graph(std::cout, 0, GLCPCAP);
  }else if(strcmp(argv[1], "UL") == 0){
    ret = cmp_graph(std::cout, GLCPCAP, ULCPCAP);
  }else if(strcmp(argv[1], "ML") == 0){
    ret = cmp_graph(std::cout, ULCPCAP, 0);
  }else{
    usage(argv[0]);
  }
  return ret;
}
