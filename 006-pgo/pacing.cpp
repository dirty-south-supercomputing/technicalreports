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
open_sim_output(const attack *fa, const attack *c1, const attack *c2,
                std::ofstream &fout, std::string &fname){
  std::string faname, c1name, c2name;
  encode_name(fa->name, faname);
  encode_name(c1->name, c1name);
  encode_name(c2->name, c2name);
  fname = faname + "-" + c1name + "-" + c2name;
  fout.open(fname, std::ofstream::out);
  if(fout.rdstate() & std::ofstream::failbit){
    std::cerr << "error opening " << fname << std::endl;
    return -1;
  }
  return 0;
}

// FIXME broken (in places) for nullptr as c2
static void
dump_sim(std::ostream &fp, const attack *fa, const std::vector<int> &mods,
         const attack *c1, const attack *c2){
  fp << "digraph {" << std::endl;
  fp << "rankdir=LR" << std::endl;
  fp << "fontname=\"sans-serif\"" << std::endl;
  fp << "label=<" << fa->name << ", " << c1->name;
  if(c2){
    fp << ", " << c2->name;
  }
  fp << ">" << std::endl;
  // node and edge defaults
  fp << "node [shape=box,fontname=\"sans-serif\"]" << std::endl;
  fp << "edge [fontname=\"sans-serif\"]" << std::endl;
  for(auto i : mods){
    fp << "node" << i << " [ label=<" << i << " energy>";
    if(i == 0){
      fp << ", peripheries=2";
    }
    fp << " ]" << std::endl;
    auto fn = fast_attacks_req(i, fa, c1);
    int mod1 = i + fn * fa->energytrain + c1->energytrain;
    fn = fast_attacks_req(i, fa, c2);
    int mod2 = i + fn * fa->energytrain + c2->energytrain;
    if(mod1 != mod2){
      fp << "node" << i << " -> node" << mod1 << " [ color=\"";
      fp << "green";
      fp << "\" label=\"" << fn << "/" << c1->name << "\" ] " << std::endl;
      fp << "node" << i << " -> node" << mod2 << " [ color=\"";
      fp << "purple";
      fp << "\" label=\"" << fn << "/" << c2->name << "\" ] " << std::endl;
    }else{
      fp << "node" << i << " -> node" << mod1 << " [ color=\"black\" ";
      fp << "label=\"" << fn << "/*\" ] " << std::endl;
    }
  }
  fp << "}" << std::endl;
}

// all we need to know is which moduli we can reach. each modulus can be
// considered a different node, and we can build to either charged move from
// any node.
static void
sim_pacing(std::vector<int> &mods, const attack *a, const attack *c1,const attack *c2){
  const int incoming = mods.back();
  int energy = incoming;
  auto cf = fast_attacks_req(energy, a, c1);
  energy += cf * a->energytrain;
  energy += c1->energytrain;
  if(!std::ranges::contains(mods, energy)){
    mods.push_back(energy);
    sim_pacing(mods, a, c1, c2);
  }
  energy = incoming;
  cf = fast_attacks_req(energy, a, c2);
  energy += cf * a->energytrain;
  energy += c2->energytrain;
  if(!std::ranges::contains(mods, energy)){
    mods.push_back(energy);
    sim_pacing(mods, a, c1, c2);
  }
}

static void
pacing(const attack *a, const attack *c1, const attack *c2){
  std::vector<int> mods{0};
  sim_pacing(mods, a, c1, c2);
  std::ofstream fout;
  std::string fname; // should probably use tmpfile
  if(open_sim_output(a, c1, c2, fout, fname)){
    throw std::exception();
  }
  dump_sim(fout, a, mods, c1, c2);
  fout.close();
  if(fout.rdstate() & std::ofstream::failbit){
    std::cerr << "error closing sim output" << std::endl;
    throw std::exception();
  }
  std::string fname2 = fname + ".png";
  std::string gv = "dot -Tpng " + fname + " > " + fname2;
  if(system(gv.c_str())){
    std::cerr << "neato failed for " << fname << std::endl;
    throw std::exception();
  }
  if(unlink(fname.c_str())){
    std::cerr << "error removing " << fname << std::endl;
    throw std::exception();
  }
}

[[noreturn]] static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << " fast charged [charged]" << std::endl;
  exit(EXIT_FAILURE);
}

// dump dot files showing pacing for an attack duo or trio
int main(int argc, const char **argv){
  if(argc < 3 || argc > 4){
    usage(argv[0]);
  }
  const auto *fa = lookup_attack(argv[1]);
  if(!fa || !fast_attack_p(*fa)){
    std::cerr << "invalid fast attack: " << argv[1] << std::endl;
    usage(argv[0]);
  }
  const auto *ca1 = lookup_attack(argv[2]);
  if(!ca1 || !charged_attack_p(*ca1)){
    std::cerr << "invalid charged attack: " << argv[2] << std::endl;
    usage(argv[0]);
  }
  const attack *ca2;
  if(argv[3]){
    ca2 = lookup_attack(argv[3]);
    if(!ca2 || !charged_attack_p(*ca2)){
      std::cerr << "invalid charged attack: " << argv[3] << std::endl;
      usage(argv[0]);
    }
  }else{
    ca2 = nullptr;
  }
  pacing(fa, ca1, ca2);
  return EXIT_SUCCESS;
}
