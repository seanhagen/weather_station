#ifndef LIGHTNING_H
#define LIGHTNING_H
#include "SparkFun_AS3935.h"
#include "setup.h"

#define INDOOR 0x12
#define OUTDOOR 0xE
#define LIGHTNING_INT 0x08
#define DISTURBER_INT 0x04
#define NOISE_INT 0x01

typedef struct {
  bool strike;
  bool isNoise;
  bool isDisturber;
  byte distance; // in kilometers
} lightningInfo;

void setupLightning();

lightningInfo loopLightning();

#endif
