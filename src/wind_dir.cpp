#include "wind_dir.h"

float rawWindDir;
float voltage;

WindDir getWindDir(int pin){
  rawWindDir = analogRead(pin);
  voltage = rawWindDir * (5.0 / 4095.0);
  // // for debugging the direction
  // Serial.print("Voltage: ");
  // Serial.print(voltage);
  // Serial.print("  --  ");
  
  // South
  if (voltage >= 1.25 && voltage <= 1.30){
    return South;
  }

  // South_SouthWest
  if (voltage >= 1.0 && voltage <= 1.10){
    return South_SouthWest;
  }

  // SouthWest
  if (voltage >= 3.0 && voltage < 3.4){
    return SouthWest;
  }

  // West_SouthWest
  if (voltage >= 2.85 && voltage <= 2.95){
    return West_SouthWest;
  }

  // West
  if (voltage >= 4.75 && voltage <= 5.0){
    return West;
  }

  // West_NorthWest
  if (voltage >= 4.0 && voltage <= 4.15){
    return West_NorthWest;
  }

  // NorthWest
  if (voltage >= 4.40 && voltage <= 4.50){
    return NorthWest;
  }

  // North_NorthWest -- 3.40
  if (voltage >= 3.40 && voltage <= 3.6){
    return North_NorthWest;
  }

  // North
  if (voltage >= 3.80 && voltage <= 3.95){
    return North;
  }

  // North_NorthEast
  if (voltage >= 1.85 && voltage <= 1.90){
    return North_NorthEast;
  }

  // NorthEast
  if (voltage >= 2.15 && voltage <= 2.20){
    return NorthEast;
  }

  // East_NorthEast
  if (voltage >= 0.20 && voltage < 0.25){
    return East_NorthEast;
  }

  // East
  if (voltage >= 0.25 && voltage <= 0.30){
    return East;
  }

  // East_SouthEast
  if (voltage >= 0.10 && voltage <= 0.15){
    return East_SouthEast;
  }

  // SouthEast
  if (voltage >= 0.70 && voltage <= 0.80){
    return SouthEast;
  }

  // South_SouthEast
  if (voltage >= 0.44 && voltage <= 0.50){
    return South_SouthEast;
  }

  // Serial.print("A1 voltage: ");
  // Serial.print(voltage);
  // Serial.println(", unknown direction!");
  return Unknown;
  
}
