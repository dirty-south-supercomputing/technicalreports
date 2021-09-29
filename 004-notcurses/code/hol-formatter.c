#include <stdlib.h>
#include <notcurses/direct.h>

int main(void){
  const char blue[] = "house";
  const char *b = blue;
  struct ncdirect* n = ncdirect_core_init(NULL, stdout, 0);
  int c, ret = 0;
  if(n){
    while(!ret && (c = getchar()) != EOF){
      if(isalpha(c) && tolower(c) == *b){
        ++b;
      }else{
        if(b > blue){
          if(!*b){
            ret |= ncdirect_on_styles(n, NCSTYLE_BOLD);
            ret |= ncdirect_set_fg_rgb(n, 0x0339dc);
          }
          ret |= (printf("%.*s", (int)(b - blue), blue) < 0);
          if(!*b){
            ret |= ncdirect_set_fg_default(n);
            ret |= ncdirect_off_styles(n, NCSTYLE_BOLD);
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
