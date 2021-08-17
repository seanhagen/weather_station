#ifndef RAIN_H
#define RAIN_H
#include "setup.h"

typedef struct {
  float dailyRain;
  float rainHour;
} rainInfo;

void setupRain();
void rainIRQ();

void setRainMinute(byte);
void zeroRain(byte);
void zeroRainDay();

rainInfo rainLoop();

#endif
