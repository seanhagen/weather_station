#include "lightning.h"

#if defined(ENABLE_LIGHTNING)
SparkFun_AS3935 lightning;

// This variable holds the number representing the lightning or non-lightning
// event issued by the lightning detector.
int intVal = 0;
int noise = 2;     // Value between 1-7
int disturber = 2; // Value between 1-10

// // G3 - GPIO pin 17, pad 46 on the MicroMod Weather Carrier Board
// const int lightningInt = G3; // Interrupt pin for lightning detection

// G1 - GPIO pin 25, pad 42 on the MicroMod Weather Carrier Board
// #define LIGHTNING_CS_PIN 25
// const int spiCS = G1;        // SPI chip select pin

void setupLightning() {
  pinMode(LIGHTNING_INTERRUPT_PIN, INPUT);

  if (lightning.beginSPI(LIGHTNING_CS_PIN, 2000000) == false) {
    Serial.println("Lightning Detector did not start up, freezing!");
    while (1)
      ;
  }

  Serial.print("Possible lightning values: ");
  Serial.print("NOISE -> ");
  Serial.print(NOISE_INT);
  Serial.print(", DISTURBER -> ");
  Serial.print(DISTURBER_INT);
  Serial.print(", LIGHTNING -> ");
  Serial.println(LIGHTNING_INT);

  // The lightning detector defaults to an indoor setting at
  // the cost of less sensitivity, if you plan on using this outdoors
  // uncomment the following line:
  lightning.setIndoorOutdoor(OUTDOOR);
}

lightningInfo loopLightning() {
  // Hardware has alerted us to an event, now we read the interrupt register
  if (digitalRead(LIGHTNING_INTERRUPT_PIN) == HIGH) {
    intVal = lightning.readInterruptReg();
    Serial.print("Lightning: ");
    Serial.println(intVal);
    if (intVal == NOISE_INT) {
      // Too much noise? Uncomment the code below, a higher number means better
      // noise rejection.
      lightning.setNoiseLevel(noise);
      return lightningInfo{false, true, false, 0};
    } else if (intVal == DISTURBER_INT) {
      // Too many disturbers? Uncomment the code below, a higher number means
      // better disturber rejection.
      lightning.watchdogThreshold(disturber);
      return lightningInfo{false, false, true, 0};
    } else if (intVal == LIGHTNING_INT) {
      // Lightning! Now how far away is it? Distance estimation takes into
      // account any previously seen events in the last 15 seconds.
      byte distance = lightning.distanceToStorm();
      return lightningInfo{true, false, false, distance};
    }
  }
  return lightningInfo{false, false, false, 0};
}

#endif

#if !defined(ENABLE_LIGHTNING)
void setupLightning() {}
lightningInfo loopLightning() { return lightningInfo{false, false, false, -1}; }
#endif
