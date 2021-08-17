#ifndef WIND_SPEED_H
#define WIND_SPEED_H
#include "setup.h"

#define WIND_DIR_AVG_SIZE 120

typedef struct {
  float windSpeedMPH;
  float windGustMPH;
  float windSpeedMPH_Avg2Min;
  // true if windSpeedMPH is the highest gust in this 10 min bucket
  bool isGust10Min;
  bool isGustDay;
} windSpeedInfo;

void setupWindSpeed();
void wspeedIRQ();

void calcWindSpeed();
float getWindSpeed();
windSpeedInfo windSpeedLoop(byte minutes_10m);

void zeroWindSpeedMinute(int);
void zeroWindGust10m(int);

#endif
