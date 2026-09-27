#include "pgotypes.h"
#include <memory>
#include <cstdio>
#include <cstring>
#include <cstdlib>

static bool cmpatk(const attack* a1, const attack* a2){
  return a1->turns < a2->turns ? true
    : a1->turns > a2->turns ? false
    : a1->powertrain < a2->powertrain ? true
    : a1->powertrain > a2->powertrain ? false
    : a1->energytrain < a2->energytrain ? true
    : a1->energytrain > a2->energytrain ? false
    : false;
}

static bool cmpatkraid(const attack* a1, const attack* a2){
  return a1->animdur < a2->animdur ? true 
    : a1->animdur > a2->animdur ? false
    : a1->powerraid < a2->powerraid ? true
    : a1->powerraid > a2->powerraid ? false
    : a1->energyraid < a2->energyraid ? true 
    : a1->energyraid > a2->energyraid ? false
    : false;
}

static void
print_latex_table(const std::vector<const attack*>& as, bool raidvalues, bool html){
  std::cout << std::setprecision(3);
  if(html){
    std::cout << "<table>" << std::endl;
    std::cout << "<tr>";
    std::cout << "<th>Attack</th>";
    if(raidvalues){
      std::cout << "<th>S</th><th>EPS</th><th>PPS</th>";
    }else{
      std::cout << "<th>T</th><th>EPT</th><th>PPT</th>";
    }
    std::cout << "<th>6⁄5</th><th>Pop(STAB)</th>";
    std::cout << "</tr>";
  }else{
    printf("\\begin{center}\n");
    printf("\\footnotesize\n");
    printf("\\begin{longtable}{lrrrrrr}\n");
    // we removed E and P because they can be derived from T and xPT
    // FIXME handle raidvalues case correctly in header (see html)
    printf("Attack & T & \\EPT{} & \\PPT{} & $\\cdot\\frac{6}{5}$ & Pop(STAB)\\\\\n");
    printf("\\Midrule\n");
    printf("\\endhead\n");
  }
  for(const auto a : as){
    float power = raidvalues ? a->powerraid : a->powertrain;
    float t = raidvalues ? (a->animdur * 0.5) : a->turns;
    float energy = raidvalues ? a->energyraid : a->energytrain;
    float ppt = power / t;
    float ept = energy / t;
    if(html){
      std::cout << "<tr><td>";
      html_type(a->type);
      std::cout << ' ';
    }else{
      print_fast_attack_rowcolor(a);
      if(a->type != TYPECOUNT){
        print_type(a->type);
      }else{
        printf("\\hspace{1em}");
      }
    }
    unsigned pop, popstab;
    pop = learner_count(a, &popstab);
    if(html){
      std::cout << a->name << "</td><td>" << t << "</td><td>" << ept;
      std::cout << "</td><td>" << ppt << "</td><td>" << (power * 6.0) / (t * 5.0);
      std::cout << "</td><td>" << pop << "(" << popstab << ")" << "</td>";
      std::cout << "</tr>" << std::endl;
    }else{
      printf(" %s & %u & %.3g & %.3g & %g & %u(%u)\\\\\n",
            a->name, a->turns, /*energy,*/ ept,
            /*power, (power * 6.0) / 5,*/
            ppt, (power * 6.0) / (t * 5.0),
            pop, popstab);
    }
  }
  if(html){
    std::cout << "</table>" << std::endl;
  }else{
    const char* typestr = raidvalues ? "Mx1" : "3x3";
    printf("\\caption[Fast attacks, \\PPT{}, and \\EPT{} (%s stats)]", typestr);
    printf("{Fast attacks, \\PPT{}, and \\EPT{} (%s stats. Attacks with \\EPT{}·\\PPT{} ≥ 9 are highlighted in green)\\label{table:fastattacks%s}}\n", typestr, typestr);
    printf("\\end{longtable}\n");
    printf("\\end{center}\n");
  }
}

static void
usage(const char* argv0){
  std::cerr << "usage: " << argv0 << " [ -r ] [ -h ]" << std::endl;
  std::cerr << " -r: use raid values, not trainer battles" << std::endl;
  std::cerr << " -h: emit html, not latex" << std::endl;
  exit(EXIT_FAILURE);
}

// emit table of fast attacks by duration x energy x power
int main(int argc, const char** argv){
  bool raidvalues = false; // use Mx1 rather than 3x3 values
  bool html = false; // use latex rather than html
  if(argc != 1){
    if(argc != 2){
      if(argc != 3){
        usage(argv[0]);
      }
      if(strcmp(argv[2], "-h")){ // must be in -r -h order lol FIXME
        std::cerr << "invalid argument: " << argv[2] << std::endl;
        usage(argv[0]);
      }
      if(strcmp(argv[1], "-r")){
        std::cerr << "invalid argument: " << argv[1] << std::endl;
        usage(argv[0]);
      }
      raidvalues = true;
      html = true;
    }else{
      if(strcmp(argv[1], "-r") == 0){
        raidvalues = true;
      }else if(strcmp(argv[1], "-h") == 0){
        html = true;
      }else{
        std::cerr << "invalid argument: " << argv[1] << std::endl;
        usage(argv[0]);
      }
    }
  }
  std::vector<const attack*> fast{};
  for(auto it = attacks_begin() ; it != attacks_end() ; ++it){
    const attack* a = *it;
    if(a->energytrain > 0){
      fast.push_back(a);
    }
  }
  if(raidvalues){
    std::sort(fast.begin(), fast.end(), cmpatkraid);
  }else{
    std::sort(fast.begin(), fast.end(), cmpatk);
  }
  print_latex_table(fast, raidvalues, html);
  return EXIT_SUCCESS;
}
