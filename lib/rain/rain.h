#ifndef RAIN_H
#define RAIN_H
#include <Arduino.h>

#define RAIN_PIN 27

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
