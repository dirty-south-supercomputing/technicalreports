#include "pgotypes.h"

// generate a colormap for octave
int main(void){
  for(int t = 0 ; t < TYPECOUNT ; ++t){
    const uint32_t c = TColors[t];
    float r = ((c >> 16u) % 256u) / 255.0;
    float g = ((c >> 8u) % 256u) / 255.0;
    float b = (c  % 256u) / 255.0;
    printf("%f %f %f\n", r, g, b);
  }
  return EXIT_SUCCESS;
}
