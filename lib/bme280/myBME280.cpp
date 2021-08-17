#include "myBME280.h"

#if defined(ENABLE_BME280)
BME280 sen;

void setupBME280() {
  if (sen.beginI2C() == false) { // Begin communication over I2C
    Serial.println("BME280 did not respond.");
    while (1)
      ; // Freeze
  }
}

bme280Reading loopBME280() {
  BME280_SensorMeasurements reading;
  sen.readAllMeasurements(&reading, 0);
  return bme280Reading{reading.temperature, reading.humidity, reading.pressure};
}

// Serial.print("Temperature: ");
// Serial.println(tempSensor.readTempF(), 2);
// Serial.print("Humidity: ");
// Serial.println(tempSensor.readFloatHumidity(), 0);
// Serial.print("Pressure: ");
// Serial.println(tempSensor.readFloatPressure(), 0);
// Serial.print("Altitude: ");
// Serial.println(tempSensor.readFloatAltitudeFeet(), 1);

#endif

#if !defined(ENABLE_BME280)
void setupBME280();
bme280Reading loopBME280() { return bme280Reading{}; }
#endif
