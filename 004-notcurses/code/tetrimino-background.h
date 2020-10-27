static struct ncvisual* background(struct notcurses* nc, const char* fname){
  struct ncvisual* ncv = ncvisual_from_file(fname);
  if(!ncv){
    return NULL;
  }
  if(ncvisual_render(nc, ncv, NULL) <= 0){
    ncvisual_destroy(ncv);
    return NULL;
  }
  ncplane_greyscale(notcurses_stdplane(nc));
  return ncv;
}
