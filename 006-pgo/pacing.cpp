#include "html.h"
#include <algorithm>
#include <unistd.h>

static unsigned
fast_attacks_req(int energy, const attack *a, const attack *ca){
  int req = -ca->energytrain - energy;
  if(req < 0){
    req = 0;
  }
  int n = req / a->energytrain;
  if(n * a->energytrain < req){
    ++n;
  }
  assert(n * a->energytrain >= req);
  return n;
}

static int
open_sim_output(const species &s, const attack *fa,
                const attack *c1, const attack *c2,
                std::ofstream &fout, std::string &fname){
  std::string faname, c1name, c2name;
  encode_name(s.name, fname);
  encode_name(fa->name, faname);
  encode_name(c1->name, c1name);
  encode_name(c2->name, c2name);
  fname += "-" + faname + "-" + "-" + c1name + "-" + c2name;
  fout.open(fname, std::ofstream::out);
  if(fout.rdstate() & std::ofstream::failbit){
    std::cerr << "error opening " << fname << std::endl;
    return -1;
  }
  return 0;
}

static void
dump_sim(std::ostream &fp, const std::string &sname,
         const attack *fa,
         const std::vector<int> &mods,
         const attack *c1, const attack *c2){
  fp << "digraph {" << std::endl;
  fp << "label=<" << sname << " Attacks: " << fa->name << ", " << c1->name;
  if(c2){
    fp << ", " << c2->name;
  }
  fp << ">" << std::endl;
  for(auto i : mods){
    fp << "node" << i << " [ label=<" << i << " energy>";
    if(i == 0){
      fp << ", peripheries=2";
    }
    fp << " ]" << std::endl;
    auto fn = fast_attacks_req(i, fa, c1);
    int mod = i + fn * fa->energytrain;
    mod += c1->energytrain;
    fp << "node" << i << " -> node" << mod << " [ color=\"";
    fp << "green";
    fp << "\" label=\"" << fn << "/" << c1->name << "\" ] " << std::endl;
    fn = fast_attacks_req(i, fa, c2);
    mod = i + fn * fa->energytrain;
    mod += c2->energytrain;
    fp << "node" << i << " -> node" << mod << " [ color=\"";
    fp << "black";
    fp << "\" label=\"" << fn << "/" << c2->name << "\" ] " << std::endl;
  }
  fp << "}" << std::endl;
}

// all we need to know is which moduli we can reach. each modulus can be
// considered a different node, and we can build to either charged move from
// any node.
static void
sim_pacing(std::ostream &fp, std::vector<int> &mods,
           const attack *a, const attack *c1,const attack *c2){
  const int incoming = mods.back();
  int energy = incoming;
  auto cf = fast_attacks_req(energy, a, c1);
  energy += cf * a->energytrain;
  energy += c1->energytrain;
  if(!std::ranges::contains(mods, energy)){
    mods.push_back(energy);
    sim_pacing(fp, mods, a, c1, c2);
  }
  energy = incoming;
  cf = fast_attacks_req(energy, a, c2);
  energy += cf * a->energytrain;
  energy += c2->energytrain;
  if(!std::ranges::contains(mods, energy)){
    mods.push_back(energy);
    sim_pacing(fp, mods, a, c1, c2);
  }
}

static void
pacing(std::ostream &fp, const species &s){
  std::vector<const attack*> fas, cas;
  for(const auto a : s.attacks){
    if(fast_attack_p(*a)){
      fas.push_back(a);
    }else{
      cas.push_back(a);
    }
  }
  for(const auto a : fas){
    for(unsigned c1idx = 0 ; c1idx < cas.size() ; ++c1idx){
      const auto *c1 = cas[c1idx];
      for(unsigned c2idx = c1idx + 1 ; c2idx < cas.size() ; ++c2idx){
        const auto *c2 = cas[c2idx];
        std::vector<int> mods{0};
        sim_pacing(fp, mods, a, c1, c2);
        std::ofstream fout;
        std::string fname;
        if(open_sim_output(s, a, c1, c2, fout, fname)){
          throw std::exception();
        }
        dump_sim(fout, s.name, a, mods, c1, c2);
        fout.close();
        if(fout.rdstate() & std::ofstream::failbit){
          std::cerr << "error closing sim output" << std::endl;
          throw std::exception();
        }
        std::string fname2 = fname + ".png";
        std::string gv = "circo -Tpng " + fname + " > " + fname2;
        if(system(gv.c_str())){
          std::cerr << "neato failed for " << fname << std::endl;
          throw std::exception();
        }
        if(unlink(fname.c_str())){
          std::cerr << "error removing " << fname << std::endl;
          throw std::exception();
        }
      }
    }
  }
}

[[noreturn]] static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " form" << std::endl;
  exit(EXIT_FAILURE);
}

// dump a dot file showing pacing
int main(int argc, const char **argv){
  if(argc != 2){
    usage(argv[0]);
  }
  const auto s = lookup_species(argv[1]);
  if(!s){
    std::cerr << "couldn't look up " << argv[1] << std::endl;
    usage(argv[0]);
  }
  pacing(std::cout, *s);
  return EXIT_SUCCESS;
}
