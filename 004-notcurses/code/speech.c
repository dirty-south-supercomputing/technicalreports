#include <unistd.h>
#include <locale.h>
#include <stdlib.h>
#include <notcurses/notcurses.h>

int main(void){
  setlocale(LC_ALL, "");
  notcurses_options nopts;
  memset(&nopts, 0, sizeof(nopts));
  nopts.inhibit_alternate_screen = true;
  struct notcurses* nc = notcurses_init(&nopts, stdout);
  fflush(stdout);
  int averr;
  struct ncvisual* ncv = ncvisual_open_plane(nc, "htp-speech-bubble.jpg", &averr, 0, 0, NCSCALE_SCALE);
  ncvisual_decode(ncv, &averr);
  ncvisual_render(ncv, 0, 0, 0, 0);
  struct ncplane* n = ncvisual_plane(ncv);
  ncplane_putstr_yx(n, 7, 50, "hacking the planet!");
  ncplane_putstr_yx(n, 9, 51, "with libnotcurses");
  ncplane_putstr_yx(n, 13, 53, "by nick black");
  ncplane_putstr_yx(n, 14, 50, "author of notcurses");
  if(notcurses_render(nc)){
    return EXIT_FAILURE;
  };
  notcurses_stop(nc);
  return EXIT_SUCCESS;
}
