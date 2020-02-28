#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void){
  srand(time(NULL)); // gross
  long g, secret = random();
  int r = 0;
  do{
    if(!(r |= printf("Guess the long: ") < 0)){
      if(!fflush(stdout)){
        int rargs = scanf("%ld", &g); // super shitty to the max
        if(rargs != 1){
          fprintf(stderr, "Die, infidel!\n");
          return EXIT_FAILURE;
        }
        r |= (printf(g > secret ? "\tLOL jabronies guess %ld. Too high!\n" : g < secret
                 ? "\tSpineless worm! %ld? Too low!\n" : "%ld is right! ", g) < 0);
      }
    }
  }while(g != secret && !r);
  if(r || printf("You enjoy 20/20 vision into the minds of antimen!\n") < 0){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
