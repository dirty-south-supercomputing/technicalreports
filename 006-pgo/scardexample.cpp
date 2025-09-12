#include "pgotypes.h"

// print a sample species card
int main(void){
  const species s(0, "OV-099 Challenger", TYPE_FLYING, TYPE_GHOST, 200, 120, 100, "Columbia",
                  { &ATK_Astonish, &ATK_Incinerate, &ATK_Brave_Bird, &ATK_Aeroblast, },
                  true, true, false, { &ATK_Aeroblast, }, species::CAT_NORMAL,
                  100, nullptr, species::EVOL_NOITEM);
  print_species_latex(&s, true, true, true);
}
