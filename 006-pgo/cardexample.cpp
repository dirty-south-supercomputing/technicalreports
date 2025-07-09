#include "pgotypes.cpp"

int main(void){
  const attack *a[] = { &ATK_Astonish, &ATK_Incinerate, &ATK_Brave_Bird, &ATK_Aeroblast, NULL, };
  species s("OV-099 Challenger");
  s.sta = 100;
  s.def = 120;
  s.atk = 200;
  s.attacks = a;
  s.shadow = true;
  s.t1 = TYPE_FLYING;
  s.t2 = TYPE_GHOST;
  print_species_latex(&s, true, false);
  return EXIT_SUCCESS;
}
