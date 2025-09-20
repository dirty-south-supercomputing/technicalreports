#include <stdio.h>
#include "pgotypes.h"

static const char* WNames[WEATHERCOUNT] = {
  "Clear",
  "Rainy",
  "Partly Cloudy",
  "Cloudy",
  "Windy",
  "Snow",
  "Fog",
  "Extreme"
};

// secondary names
static const char* WSNames[WEATHERCOUNT] = {
  "Clear Night",
  NULL,
  "Partly Cloudy Night",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

static pgo_weather_t wboosts[TYPECOUNT] = {
  WEATHER_RAIN,   // bug
  WEATHER_FOG,    // dark
  WEATHER_WINDY,  // dragon
  WEATHER_RAIN,   // electric
  WEATHER_CLOUDY, // fairy
  WEATHER_CLOUDY, // fighting
  WEATHER_CLEAR,  // fire
  WEATHER_WINDY,  // flying
  WEATHER_FOG,    // ghost
  WEATHER_CLEAR,  // grass
  WEATHER_CLEAR,  // ground
  WEATHER_SNOW,   // ice
  WEATHER_PARTLY_CLOUDY,  // normal
  WEATHER_CLOUDY, // poison
  WEATHER_WINDY,  // psychic
  WEATHER_PARTLY_CLOUDY,  // rock
  WEATHER_SNOW,   // steel
  WEATHER_RAIN,   // water
};

int main(void){
  printf("\\centering\\begin{tabular}{lcl|lcl}");
  printf("Weather & Icons & Types & Weather & Icons & Types\\\\\n");
  printf("\\Midrule\n");
  for(unsigned w = 0 ; w < static_cast<unsigned>(WEATHERCOUNT) ; ++w){
    printf("%s & \\includegraphics[height=1em,keepaspectratio]{images/%s-icon.png}", WNames[w], WNames[w]);
    if(WSNames[w]){
      printf("\\includegraphics[height=1em,keepaspectratio]{images/%s-icon.png}", WSNames[w]);
    }
    printf(" & ");
    bool printed = false;
    for(unsigned t = 0 ; t < static_cast<unsigned>(TYPECOUNT) ; ++t){
      if(wboosts[t] == w){
        print_type(static_cast<pgo_types_e>(t));
        printf(" ");
        printed = true;
      }
    }
    if(!printed){
      printf("None");
    }
    if(w % 2){
      printf("\\\\\n");
    }else{
      printf(" & ");
    }
  }
  printf("\\end{tabular}\\caption{Weather-boosted types\\label{table:weather}}");
}
