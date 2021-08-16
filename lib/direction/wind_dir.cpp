#include "wind_dir.h"

float rawWindDir;
float voltage;

// nothing needs to be done for setting up wind direction, it just needs to
// read from an analog pin during runtime
void setupWindDir() {}

windDirInfo windDirLoop() {
  rawWindDir = analogRead(VANE_PIN);
  voltage = rawWindDir * (5.0 / 4095.0);
  // // for debugging the direction
  // Serial.print("Voltage: ");
  // Serial.print(voltage);
  // Serial.print("  --  ");

  windVaneDir vd = UNKNOWN;
  const char *name = "Unknown";

  // South
  if (voltage >= 1.20 && voltage <= 1.35) {
    name = "South";
    vd = SOUTH;
  }

  // South_SouthWest
  if (voltage >= 1.0 && voltage <= 1.15) {
    name = "South South-West";
    vd = SOUTH_SOUTHWEST;
  }

  // SouthWest
  if (voltage > 2.95 && voltage < 3.4) {
    name = "South-West";
    vd = SOUTHWEST;
  }

  // West_SouthWest
  if (voltage >= 2.85 && voltage <= 2.95) {
    name = "West South-West";
    vd = WEST_SOUTHWEST;
  }

  // West
  if (voltage >= 4.75 && voltage <= 5.0) {
    name = "West";
    vd = WEST;
  }

  // West_NorthWest
  if (voltage >= 4.0 && voltage <= 4.15) {
    name = "West North-West";
    vd = WEST_NORTHWEST;
  }

  // NorthWest
  if (voltage >= 4.40 && voltage <= 4.50) {
    name = "North-West";
    vd = NORTHWEST;
  }

  // North_NorthWest -- 3.40
  if (voltage >= 3.40 && voltage <= 3.6) {
    name = "North North-West";
    vd = NORTH_NORTHWEST;
  }

  // North
  if (voltage >= 3.80 && voltage <= 3.95) {
    name = "North";
    vd = NORTH;
  }

  // North_NorthEast
  if (voltage >= 1.85 && voltage <= 1.90) {
    name = "North North-East";
    vd = NORTH_NORTHEAST;
  }

  // NorthEast
  if (voltage >= 2.10 && voltage <= 2.22) {
    name = "North-East";
    vd = NORTHEAST;
  }

  // East_NorthEast
  if (voltage >= 0.20 && voltage < 0.25) {
    name = "East North-East";
    vd = EAST_NORTHEAST;
  }

  // East
  if (voltage >= 0.25 && voltage <= 0.34) {
    name = "East";
    vd = EAST;
  }

  // East_SouthEast
  if (voltage >= 0.10 && voltage <= 0.15) {
    name = "East South-East";
    vd = EAST_SOUTHEAST;
  }

  // SouthEast
  if (voltage >= 0.65 && voltage <= 0.85) {
    name = "South-East";
    vd = SOUTHEAST;
  }

  // South_SouthEast
  if (voltage >= 0.40 && voltage <= 0.50) {
    name = "South South-East";
    vd = SOUTH_SOUTHEAST;
  }

  return windDirInfo{voltage, vd, name};
}
