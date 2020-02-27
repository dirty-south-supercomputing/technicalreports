static struct ncvisual* background(struct notcurses* nc, const char* fname){
  int err = 0;
  struct ncvisual* ncv = ncplane_visual_open(notcurses_stdplane(nc), fname, &err);
  if(!ncv || (err && err != AVERROR_EOF)){
    return NULL;
  }
  if(!ncvisual_decode(ncv, &err) || ncvisual_render(ncv, 0, 0, 0, 0)){
    ncvisual_destroy(ncv);
    return NULL;
  }
  ncplane_greyscale(notcurses_stdplane(nc));
  return ncv;
}
