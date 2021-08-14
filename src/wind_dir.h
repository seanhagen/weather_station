#ifndef WIND_DIR_H
#define WIND_DIR_H
#include <Arduino.h>
#include <stdint.h>

enum WindDir {
  North, // 0/360 deg
  North_NorthEast, // 22 deg
  NorthEast, // 45 deg
  East_NorthEast, // 67? deg
  East, // 90 deg
  East_SouthEast, // 112 deg
  SouthEast, // 135 deg
  South_SouthEast, // 157 deg
  South, // 180
  South_SouthWest, // 202 deg
  SouthWest, // 225 deg
  West_SouthWest, // 248 deg
  West, // 270 deg
  West_NorthWest, // 292 deg
  NorthWest, // 315 deg
  North_NorthWest, // 338 deg
  Unknown
};

WindDir getWindDir(int);

#endif
