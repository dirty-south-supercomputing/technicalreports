#include <limits.h>
#include <notcurses.h>

int main(void){
  srand(time(NULL)); // gross
  long guess, secret = random();
  struct ncdirect* n = notcurses_directmode(NULL, stdout);
  if(n == NULL){
    return EXIT_FAILURE;
  }
  int r = 0;
  do{
    if(!(r |= (ncdirect_fg_default(n)))){
      if(!(r |= printf("Guess the long: ", LONG_MAX) < 0)){
        if(!fflush(stdout)){
          int rargs = scanf("%ld", &guess); // super shitty to the max
          if(rargs != 1){
            fprintf(stderr, "Die, infidel!\n");
            return EXIT_FAILURE;
          }
          int offoom = labs(__builtin_clzl(guess) - __builtin_clzl(secret));
          if(guess > secret){
            r |= ncdirect_fg_rgb8(n, 0x40, 0x80, offoom * 6);
            r |= printf("\tLOL jabronies guess %ld. Too high!\n", guess) < 0;
          }else if(guess < secret){
            r |= ncdirect_fg_rgb8(n, offoom * 6, 0x80, 0x40);
            r |= printf("\tSpineless worm! %ld? Too low!\n", guess) < 0;
          }
        }
      }
    }
  }while(guess != secret && !r);
  if(r || printf("You enjoy 20/20 vision into the minds of men!\n") < 0){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
