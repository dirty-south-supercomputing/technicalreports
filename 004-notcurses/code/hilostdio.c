#include <limits.h>
#include <notcurses/notcurses.h>

int main(void){
  srand(time(NULL)); // gross
  long guess, secret = random();
  struct ncdirect* n = notcurses_directmode(NULL, stdout);
  if(n == NULL){
    return EXIT_FAILURE;
  }
  int r = 0;
  do{
    if(!(r |= printf("Guess the long: ") < 0)){
      if(!fflush(stdout)){
        int rargs = scanf("%ld", &guess); // super shitty to the max
        if(rargs != 1){
          fprintf(stderr, "Die, infidel!\n");
          return EXIT_FAILURE;
        }
        if(guess > secret){
          r |= printf("\tLOL jabronies guess %ld. Too high!\n", guess) < 0;
        }else if(guess < secret){
          r |= printf("\tSpineless worm! %ld? Too low!\n", guess) < 0;
        }
      }
    }
  }while(guess != secret && !r);
  if(r || printf("You enjoy 20/20 vision into the minds of men!\n") < 0){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
