#ifndef WIND_DIR_H
#define WIND_DIR_H
#include <Arduino.h>
#include <stdint.h>

/*
 * A1 - GPIO pin 35, pad 38 on the MicroMod (Input Only!)
 * https://learn.sparkfun.com/tutorials/micromod-esp32-processor-board-hookup-guide/all
 */
#define VANE_PIN 35

enum windVaneDir {
  NORTH,           // 0/360 DEG
  NORTH_NORTHEAST, // 22 DEG
  NORTHEAST,       // 45 DEG
  EAST_NORTHEAST,  // 67? DEG
  EAST,            // 90 DEG
  EAST_SOUTHEAST,  // 112 DEG
  SOUTHEAST,       // 135 DEG
  SOUTH_SOUTHEAST, // 157 DEG
  SOUTH,           // 180
  SOUTH_SOUTHWEST, // 202 DEG
  SOUTHWEST,       // 225 DEG
  WEST_SOUTHWEST,  // 248 DEG
  WEST,            // 270 DEG
  WEST_NORTHWEST,  // 292 DEG
  NORTHWEST,       // 315 DEG
  NORTH_NORTHWEST, // 338 DEG
  UNKNOWN
};

typedef struct {
  float voltage;
  windVaneDir dir;
  const char *name;
} windDirInfo;

void setupWindDir();
windDirInfo windDirLoop();

#endif
