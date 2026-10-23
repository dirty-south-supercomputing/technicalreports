#include "html.h"

struct sstat {
  stats st;
  float percent; // percentage of max gmean
};

static void
check_good_hundo(int cpbound, int cpcutoff, std::vector<sstat> &rows, const species &s){
    unsigned vcount;
    auto stats = order_ivs(s, cpbound, statscmp_gmean, &vcount);
    const auto &best = stats[vcount - 1];
    float bestbp = best.effa * best.effd * best.mhp;
    float cutoff = 0.97 * bestbp;
    unsigned i = vcount - 1;
    // find the hundo
    while(stats[i].ia != 15 || stats[i].id != 15 || stats[i].is != 15){
      --i;
    }
    float hundobp = stats[i].effa * stats[i].effd * stats[i].mhp;
    if(hundobp >= cutoff){
      if(stats[i].cp >= cpcutoff){
        stats[i].s = &s;
        rows.emplace_back(stats[i], 100 * hundobp / bestbp);
      }
    }
    delete[] stats;
}

static void
emit_good_hundos(int cpbound){
  std::cout << "<table class=\"evenshade goodhundos\">" << std::endl;
  std::cout << "<tr>";
  std::cout << "<th>T</th><th>Form</th><th>L</th><th>MHP</th><th>Eff<sub>A</sub></th>";
  std::cout << "<th>Eff<sub>D</sub></th><th>Amean</th><th>Gmean</th><th>CP</th><th>%</th>";
  std::cout << "</tr>" << std::endl;
  std::vector<sstat> rows;
  std::vector<species> state;
  int cpcutoff = cpbound - 400; // gotta allow morpeko (2196) into UL
  std::forward_list<species> store;
  std::vector<const species*> smeg;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    check_good_hundo(cpbound, cpcutoff, rows, *s);
    for(const auto &m : s->mforms){
      store.emplace_front(&*s, m);
    }
  }
  for(const auto &s : store){
    check_good_hundo(cpbound, cpcutoff, rows, s);
  }
  sort(rows.begin(), rows.end(), [](const sstat &lhs, const sstat &rhs){
        if(lhs.st.geommean < rhs.st.geommean){
          return false;
        }else if(rhs.st.geommean < lhs.st.geommean){
          return true;
        }
        if(lhs.percent < rhs.percent){
          return false;
        }else if(rhs.percent < lhs.percent){
          return true;
        }
        if(lhs.st.s->name < rhs.st.s->name){
          return false;
        }
        return true;
      });
  for(const auto &r : rows){
    std::cout << std::setprecision(6);
    std::cout << "<tr>";
    std::cout << "<td>";
    html_types(r.st.s->t1, r.st.s->t2);
    std::cout << "</td>";
    std::cout << "<td>";
    link_to_name(std::cout, r.st.s->name, true);
    std::cout << "</td>";
    std::cout << "<td>";
    emit_halflevel_as_level(std::cout, r.st.hlevel);
    std::cout << "</td>";
    std::cout << "<td>" << r.st.mhp << "</td>";
    std::cout << "<td>" << r.st.effa << "</td>";
    std::cout << "<td>" << r.st.effd << "</td>";
    std::cout << "<td>" << r.st.average << "</td>";
    std::cout << "<td>" << r.st.geommean << "</td>";
    std::cout << "<td>" << r.st.cp << "</td>";
    std::cout << std::setprecision(4);
    std::cout << "<td>" << r.percent << "</td>";
    std::cout << "</tr>" << std::endl;
  }
  std::cout << "</table>" << std::endl;
}

[[noreturn]] static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " cpbound" << std::endl;
  std::cerr << "-1 for no bound" << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
  if(argc != 2){
    usage(argv[0]);
  }
  int cpbound = atoi(argv[1]);
  if(cpbound == 0){
    std::cerr << "invalid cp bound: " << argv[1] << std::endl;
    usage(argv[0]);
  }
  emit_good_hundos(cpbound);
  return EXIT_SUCCESS;
}
