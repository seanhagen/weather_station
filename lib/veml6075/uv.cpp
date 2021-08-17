#include "uv.h"

VEML6075 uv;

void setupUV() {
  if (uv.begin(Wire) != VEML6075_SUCCESS) {
    Serial.println("Unable to communicated with VEML6075 -- UV sensor");
    while (1)
      ;
  }
}

uvInfo uvLoop() { return uvInfo{uv.a(), uv.b(), uv.i()}; }
