#include "pgotypes.h"

static void
usage(const char *argv0){
  std::cerr << "usage: " << argv0 << std::endl;
  exit(EXIT_FAILURE);
}

static int
write_radar(const species &s, int cpceil){
  unsigned ivcount;
  stats *st = order_ivs(&s, cpceil, statscmp_gmean, &ivcount);
  if(ivcount % IVLEVVEC){
    std::cerr << "unexpected vector count " << ivcount << std::endl;
    throw std::exception();
  }
  // FIXME for now, sort by lowest value. ought use median probably.
  /*std::sort(allstats.begin(), allstats.end(), [](const stats *lhs, const stats *rhs){
        return std::min_element(lhs.begin(), lhs.end(), fxn)
            //[](const stats *lhs, const stats *rhs){ return lhs->mhp < rhs->mhp; })
               < std::min_element(rhs.begin(), lhs.begin(), fxn);
                   //[](const stats *lhs, const stats *rhs){ });
      });*/
  float higha = 0, highd = 0, highhp = 0;
  while(ivcount--){
    const auto *stt = &st[ivcount];
    higha = stt->effa > higha ? stt->effa : higha;
    highd = stt->effd > highd ? stt->effd : highd;
    highhp = stt->mhp > highhp ? stt->mhp : highhp;
  }
  delete[] st;
  std::cout << s.name;
  if(cpceil){
    std::cout << cpceil;
  }
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
  std::cout << "Pokémon\tATK\tDEF\tMHP" << std::endl;
  std::cout << "Max\t250\t250\t250" << std::endl;
  std::cout << "Min\t90\t90\t90" << std::endl;
  for(unsigned u = 0 ; u < SPECIESCOUNT ; ++u){
    write_radar(sdex[u], GLCPCAP);
    write_radar(sdex[u], ULCPCAP);
    write_radar(sdex[u], 0);
  }
  return EXIT_SUCCESS;
}
