#include <unistd.h>
#include <locale.h>
#include <stdlib.h>
#include <notcurses/notcurses.h>

int main(void){
  setlocale(LC_ALL, "");
  notcurses_options nopts;
  memset(&nopts, 0, sizeof(nopts));
  nopts.flags |= NCOPTION_NO_ALTERNATE_SCREEN;
  struct notcurses* nc = notcurses_init(&nopts, stdout);
  fflush(stdout);
  struct ncvisual* ncv = ncvisual_from_file("htp-speech-bubble.jpg");
  struct ncvisual_options vopts = {
    .scaling = NCSCALE_NONE,
  };
  struct ncplane* n = ncvisual_render(nc, ncv, &vopts);
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
