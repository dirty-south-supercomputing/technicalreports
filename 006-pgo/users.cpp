#include "pgotypes.cpp"

static void
print_attack_users(const attack *a){
  printf("\\begin{tcolorbox}[enhanced,title=%s,colbacktitle=%s]\n",
          a->name, a->type == TYPECOUNT ? "Black" : TNames[a->type]);
  printf("\\end{tcolorbox}\n");
}

int main(void){
  for(unsigned aidx = 0 ; aidx < ATTACKCOUNT ; ++aidx){
    const attack *a = attacks[aidx];
    print_attack_users(a);
  }
  return EXIT_SUCCESS;
}
