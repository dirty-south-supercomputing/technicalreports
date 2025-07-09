#include "pgotypes.cpp"

// generate a latex table of the type relations,
// with defenders on rows, attackers on columns
int main(void){
  printf("\\begin{table}\n");
  printf("\\centering");
  printf("\\setlength{\\tabcolsep}{1pt}\n");
  printf("\\begin{tabular}{c g c g c g c g c g c g c g c g c g c}\n");
  printf("&\n");
  printf("\\includegraphics[width=1em]{images/bug.png} &\n");
  printf("\\includegraphics[width=1em]{images/dark.png} &\n");
  printf("\\includegraphics[width=1em]{images/dragon.png} &\n");
  printf("\\includegraphics[width=1em]{images/electric.png} &\n");
  printf("\\includegraphics[width=1em]{images/fairy.png} &\n");
  printf("\\includegraphics[width=1em]{images/fighting.png} &\n");
  printf("\\includegraphics[width=1em]{images/fire.png} &\n");
  printf("\\includegraphics[width=1em]{images/flying.png} &\n");
  printf("\\includegraphics[width=1em]{images/ghost.png} &\n");
  printf("\\includegraphics[width=1em]{images/grass.png} &\n");
  printf("\\includegraphics[width=1em]{images/ground.png} &\n");
  printf("\\includegraphics[width=1em]{images/ice.png} &\n");
  printf("\\includegraphics[width=1em]{images/normal.png} &\n");
  printf("\\includegraphics[width=1em]{images/poison.png} &\n");
  printf("\\includegraphics[width=1em]{images/psychic.png} &\n");
  printf("\\includegraphics[width=1em]{images/rock.png} &\n");
  printf("\\includegraphics[width=1em]{images/steel.png} &\n");
  printf("\\includegraphics[width=1em]{images/water.png}\n");
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
