#include "pgotypes.cpp"

static void
print_attack_users(const attack *a){
  printf("\\begin{tcolorbox}[title=%s]\n", a->name);
  printf("\\end{tcolorbox}\n");
}

int main(void){
  for(unsigned aidx = 0 ; aidx < ATTACKCOUNT ; ++aidx){
    const attack *a = attacks[aidx];
    print_attack_users(a);
  }
  return EXIT_SUCCESS;
}
