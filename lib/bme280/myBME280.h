#ifndef SENSOR_H
#define SENSOR_H
#include "SparkFunBME280.h"
#include "setup.h"

typedef struct {
  float tempC;
  float humidity;
  float pressure;
} bme280Reading;

void setupBME280();

bme280Reading loopBME280();

#endif
