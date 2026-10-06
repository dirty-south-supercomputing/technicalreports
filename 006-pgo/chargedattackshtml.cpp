#include "pgotypes.h"
#include <memory>
#include <cstring>
#include <cstdio>
#include <cstdlib>

static bool cmpatk(const attack* a1, const attack* a2){
  // remember, energytrain is negative
  float ppere1 = ((float)a1->powertrain) / a1->energytrain;
  float ppere2 = ((float)a2->powertrain) / a2->energytrain;
  if(ppere1 > ppere2){
    return true;
  }else if(ppere1 == ppere2){
    if(a1->energytrain > a2->energytrain){
      return true;
    }
  }
  return false;
}

void print_table(const std::vector<const attack*>& as, bool raid){
  std::cout << std::setprecision(3);
  std::cout << "<table class=\"evenshade\" id=\"allcharged\">" << std::endl;
  std::cout << "<tr>";
  std::cout << "<th>Attack</th>";
  if(raid){
    std::cout << "<th>S</th><th>E</th><th>P</th><th>PPS</th><th>6⁄5</th><th>Buff</th><th style=\"width: 70%\">Pop(STAB)</th>";
  }else{
    std::cout << "<th>E</th><th>P</th><th>PPE</th><th>6⁄5</th><th>Buff</th><th style=\"width: 70%\">Pop(STAB)</th>";
  }
  std::cout << "</tr>" << std::endl;
  unsigned shadnormals; // number of shadows with normal type
  auto shadows = shadow_count(&shadnormals); // number of shadows
  for(const auto a : as){
    float p = raid ? a->powerraid : a->powertrain;
    float e = raid ? a->energyraid : -a->energytrain;
    std::cout << "<tr>";
    std::cout << "<td>";
    html_type(a->type);
    std::cout << " " << a->name << "</td>";
    if(raid){
      std::cout << "<td>" << (a->animdur * 0.5) << "</td>";
    }
    std::cout << "<td>" << e << "</td>";
    std::cout << "<td>" << p << "</td>";
    if(raid){
      std::cout << "<td>" << (p / (a->animdur * 0.5)) << "</td>";
      std::cout << "<td>" << ((p * 6) / (a->animdur * 2.5)) << "</td>";
    }else{
      std::cout << "<td>" << (p / e) << "</td>";
      std::cout << "<td>" << ((p * 6) / (e * 5)) << "</td>";
    }
    std::cout << "<td>";
    summarize_buffs_html(std::cout, a);
    std::cout << "</td><td>";
    if(!strcmp(a->name, "Frustration") || !strcmp(a->name, "Return")){
      std::cout << shadows << "(" << shadnormals << ")";
      // FIXME print populations also?
    }else{
      unsigned popstab;
      auto pop = learner_count(a, &popstab);
      std::cout << pop << "(" << popstab << ") ";
      bool printed = false;
      for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
        const species* s = &sdex[u];
        for(const auto &as : s->attacks){
          if(strcmp(a->name, as->name) == 0){
            if(printed){
              std::cout << ", ";
            }
            printed = true;
            bool stab = has_stab_p(s, a);
            bool excl = exclusive_attack_p(s, a);
            if(!stab){
              std::cout << "<i>";
            }
            if(excl){
              std::cout << "<b>";
            }
            std::cout << s->name;
            if(excl){
              std::cout << "</b>";
            }
            if(!stab){
              std::cout << "</i>";
            }
            break;
          }
        }
      }
    }
    std::cout << "</td>";
    std::cout << "</tr>" << std::endl;
  }
  std::cout << "</table>" << std::endl;
}

static void
usage(const char* argv0){
  std::cerr << "usage: " << argv0 << " [ -r ] [ -h ]" << std::endl;
  std::cerr << " -r: use raid values, not trainer battles" << std::endl;
  std::cerr << " -h: ignored" << std::endl;
  exit(EXIT_FAILURE);
}

int main(int argc, const char **argv){
  std::vector<const attack*> charged{};
  bool raidvalues = false;
  if(argc > 3){
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }else if(argc == 3){
    if(strcmp(argv[2], "-h")){ // in there for compatability with fastattacks binary, ugh
      usage(argv[0]);
      exit(EXIT_FAILURE);
    }
    if(strcmp(argv[1], "-r")){
      usage(argv[0]);
      exit(EXIT_FAILURE);
    }
    raidvalues = true;
  }else if(argc == 2){
    if(strcmp(argv[1], "-h")){
      if(strcmp(argv[1], "-r")){
        usage(argv[0]);
        exit(EXIT_FAILURE);
      }
      raidvalues = true;
    }
  }
  for(auto it = attacks_begin() ; it != attacks_end() ; ++it){
    const attack* a = *it;
    if(a->energytrain < 0){
      charged.push_back(a);
    }
  }
  std::sort(charged.begin(), charged.end(), cmpatk);
  print_table(charged, raidvalues);
  return EXIT_SUCCESS;
}
