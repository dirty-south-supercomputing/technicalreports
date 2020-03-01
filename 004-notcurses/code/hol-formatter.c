#include <stdlib.h>
#include <notcurses/notcurses.h>

int main(void){
  const char blue[] = "house";
  const char *b = blue;
  struct ncdirect* n = notcurses_directmode(NULL, stdout);
  int c, ret = 0;
  if(n){
    while(!ret && (c = getchar()) != EOF){
      if(isalpha(*b) && tolower(*b) == c){
        ++b;
      }else{
        if(b > blue){
          if(!*b){
            ret |= ncdirect_fg(n, 0x0339dc);
            ret |= ncdirect_styles_on(n, NCSTYLE_STANDOUT);
          }
          ret |= (printf("%.*s", (int)(b - blue), blue) < 0);
          if(!*b){
            ret |= ncdirect_fg_default(n);
            ret |= ncdirect_styles_off(n, NCSTYLE_STANDOUT);
          }
          b = blue;
        }
        ret |= (putchar(c) == EOF);
      }
    }
    if(b > blue){
      ret |= (printf("%.*s", (int)(b - blue), blue) < 0);
    }
  }
  return (!n || ncdirect_stop(n) || !feof(stdin) || ret) ? EXIT_FAILURE : EXIT_SUCCESS;
}
