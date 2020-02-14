#include <time.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int main(void){
  srand(time(NULL)); // gross
  long guess, secret = random();
  int r = 0;
  do{
    r |= printf("Guess [0..%ld] if you want to live: ", LONG_MAX) < 0 ? -1 : 0;
    fflush(stdout);
    int rargs = scanf("%ld", &guess); // super shitty to the max
    if(rargs != 1){
      fprintf(stderr, "Die, infidel!\n");
      return EXIT_FAILURE;
    }
    if(guess > secret){
      r |= printf("\tLOL jabronies guess %ld. Too high!\n", guess) < 0 ? -1 : 0;
    }else if(guess < secret){
      r |= printf("\tSpineless worm! %ld? Too low!\n", guess) < 0 ? -1 : 0;
    }
  }while(guess != secret && !r);
  if(r || printf("You enjoy 20/20 vision into the minds of men!\n") < 0){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
