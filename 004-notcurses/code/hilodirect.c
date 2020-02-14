#include <stdio.h>
#include <stdlib.h>
#include <notcurses.h>

int main(void){
  srand(time(NULL)); // gross
  struct ncdirect* n = notcurses_directmode(NULL, stdout);
  if(n == NULL){
    return EXIT_FAILURE;
  }
  long guess, secret = random();
  do{
    printf("Guess the number if you want to live: "); fflush(stdout);
    int rargs = scanf("%ld", &guess);
    if(rargs != 1){
      fprintf(stderr, "Die, infidel!\n");
      return EXIT_FAILURE;
    }
    if(guess > secret){
      printf("\tLOL what kind of jabronie guesses %ld? Too high.\n", guess);
    }else if(guess < secret){
      printf("\tSpineless worm, how could it be %ld? Too low.\n", guess);
    }
  }while(guess != secret);
  printf("Truly, you enjoy 20/20 vision into the hearts of men.\n");
  return EXIT_SUCCESS;
}
