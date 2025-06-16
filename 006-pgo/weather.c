#include <stdio.h>
#include "pgotypes.c"

typedef enum {
  WEATHER_CLEAR,
  WEATHER_RAIN,
  WEATHER_PARTLY_CLOUDY,
  WEATHER_CLOUDY,
  WEATHER_WINDY,
  WEATHER_SNOW,
  WEATHER_FOG,
  WEATHER_EXTREME,
  WEATHERCOUNT
} pgo_weather_t;

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
  printf("\\begin{center}\\begin{tabular}{lcl}\n");
  printf("Weather & Icon & Types\\\\\n");
  printf("\\Midrule\n");
  for(unsigned w = 0 ; w < static_cast<unsigned>(WEATHERCOUNT) ; ++w){
    printf("%s & \\includegraphics[height=1em,width=1em]{images/%s.png} & ", WNames[w], WNames[w]);
    printf("FIXME\\\\\n");
  }
  printf("\\end{tabular}\\caption{Weather-boosted types}\\label{table:weather}\\end{center}\n");
}
