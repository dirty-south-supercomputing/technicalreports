#include "pgotypes.h"

// generate a latex table of the type relations,
// with defenders on rows, attackers on columns
int main(void){
  printf("\\begin{table}\n");
  printf("\\centering");
  printf("\\setlength{\\tabcolsep}{1pt}\n");
  printf("\\begin{tabular}{c g c g c g c g c g c g c g c g c g c}\n");
  printf("&\n");
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    print_type(static_cast<pgo_types_e>(i));
    if(i + 1 < TYPECOUNT){
      printf("&");
    }
  }
  printf("\\\\\n");
  for(int i = 0 ; i < TYPECOUNT ; ++i){
    if(i % 2 == 0){
      printf("\\rowcolor{Gray!25}\n");
    }
    print_type(static_cast<pgo_types_e>(i));
    for(int j = 0 ; j < TYPECOUNT ; ++j){
      int trel = trelations[j][i];
      printf(" & ");
      if(trel){
        printf("%d", trel);
      }
    }
    printf("\\\\\n");
  }
  printf("\\end{tabular}\n");
  printf("\\caption[Type relations]{Type relations. Rows defend, columns attack.\\label{table:relations}}\n");
  printf("\\end{table}\n");
  return EXIT_SUCCESS;
}
