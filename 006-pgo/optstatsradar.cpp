#include "pgotypes.h"

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << std::endl;
  exit(EXIT_FAILURE);
}

static int
write_radar(const species &s, const char *lshort, int cpceil){
  unsigned ivcount;
  stats *st = order_ivs(s, cpceil, statscmp_gmean, &ivcount);
  if(ivcount % IVLEVVEC){
    std::cerr << "unexpected vector count " << ivcount << std::endl;
    throw std::exception();
  }
  float higha = 0, highd = 0, highhp = 0;
  while(ivcount--){
    const auto *stt = &st[ivcount];
    higha = stt->effa > higha ? stt->effa : higha;
    highd = stt->effd > highd ? stt->effd : highd;
    highhp = stt->mhp > highhp ? stt->mhp : highhp;
  }
  delete[] st;
  std::cout << '\"' << s.name << lshort << '\"';
  std::cout << "\t" << round(higha) << "\t" << round(highd) << "\t" << highhp << std::endl;
  return 0;
}

// generate an R-format file for radar plots with each Pokémon writing out
// its minimum and maximum values for a given cp bound
int main(int argc, const char **argv){
  const char *argv0 = argv[0];
  if(argc != 1){
    usage(argv0);
  }
  std::cout << "Form\tATK\tDEF\tMHP" << std::endl;
  // FIXME calculate these!
  std::cout << "MaxGL\t255\t346\t421" << std::endl;
  std::cout << "MinGL\t26\t37\t68" << std::endl;
  std::cout << "MaxUL\t328\t288\t405" << std::endl;
  std::cout << "MinUL\t91\t48\t90" << std::endl;
  std::cout << "MaxML\t361\t291\t429" << std::endl;
  std::cout << "MinML\t121\t51\t113" << std::endl;
  std::vector<species> megas;
  for(auto s = species_begin() ; s != species_end() ; ++s){
    write_radar(*s, "GL", GLCPCAP);
    write_radar(*s, "UL", ULCPCAP);
    write_radar(*s, "ML", 0);
    for(const auto &m : s->mforms){
      megas.emplace_back(&*s, m);
    }
  }
  for(auto s : megas){
    write_radar(s, "GL", GLCPCAP);
    write_radar(s, "UL", ULCPCAP);
    write_radar(s, "ML", 0);
  }
  return EXIT_SUCCESS;
}
