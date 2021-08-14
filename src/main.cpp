#include "SparkFunBME280.h"
#include "SparkFun_AS3935.h"
#include "SparkFun_VEML6075_Arduino_Library.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include "wind_dir.h"

BME280 tempSensor;
VEML6075 uv;
SparkFun_AS3935 lightning;

// const int A0 = 34;

/*
 * A1 - GPIO pin 35, pad 38 on the MicroMod (Input Only!)
 * https://learn.sparkfun.com/tutorials/micromod-esp32-processor-board-hookup-guide/all
 */
const int A1 = 35;

/*
 * D0 - GPIO pin 14, pad 10 on the MicroMod
 * https://learn.sparkfun.com/tutorials/micromod-esp32-processor-board-hookup-guide/all
 */
// const int D0 = 14;
// const int D0_alt = 10;

// const int D1 = 18;

// const int G1 = 42;
// const int G3 = 46;

// const int windSpeedPin = D0;
// const int rainPin = D1;
// const int soilMoisturePin = A0;
const int windDirPin = A1;

// const int lightningChipSelect = G1;
// const int lightningInterrupt = G3;

// volatile bool rainFlag = false;
// volatile bool windFlag = false;

// // Function is called every time the rain bucket tips
// void rainIRQ() { rainFlag = true; }

// // Function is called when the magnet in the anemometer is activated
// void wspeedIRQ() { windFlag = true; }

// // This variable holds the number representing the lightning or non-lightning
// // event issued by the lightning detector.
// int intVal = 0;
// int noise = 2;     // Value between 1-7
// int disturber = 2; // Value between 1-10

void testGetWindDir();

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    ;

  // Wire.begin();
  // SPI.begin();
  // pinMode(A1, INPUT_PULLUP);
  Serial.println("Trying to figure out this whole wind direction thing");
}

void loop(void) {
  WindDir dir = getWindDir(A1);
  Serial.print("Wind direction: ");
  switch (dir) {
  case North:
    Serial.print("North");
    break;
  case North_NorthEast:
    Serial.print("North North-East");
    break;
  case NorthEast:
    Serial.print("North-East");
    break;
  case East_NorthEast:
    Serial.print("East North-East");
    break;
  case East:
    Serial.print("East");
    break;
  case East_SouthEast:
    Serial.print("East South-East");
    break;
  case SouthEast:
    Serial.print("South-East");
    break;
  case South_SouthEast:
    Serial.print("South South-East");
    break;
  case South:
    Serial.print("South");
    break;
  case South_SouthWest:
    Serial.print("South South-West");
    break;
  case SouthWest:
    Serial.print("South-West");
    break;
  case West_SouthWest:
    Serial.print("West South-West");
    break;
  case West:
    Serial.print("West");
    break;
  case West_NorthWest:
    Serial.print("West North-West");
    break;
  case NorthWest:
    Serial.print("North-West");
    break;
  case North_NorthWest:
    Serial.print("North North-West");
    break;
  default:
    Serial.print(" -- UNKNOWN --");
    break;
  }
  Serial.println();
  delay(500);
}

// void testGetWindDir() {
//   rawWindDir = analogRead(A1);
//   // Serial.print("Raw wind dir value: A1:");
//   // Serial.print(rawWindDir);
//   // Serial.println();

//   float voltage = rawWindDir * (5.0 / 4095.0);
//   Serial.print("A1 voltage: ");
//   Serial.print(voltage);
//   Serial.println();

//   // pinMode(A1, INPUT_PULLUP);
// }
