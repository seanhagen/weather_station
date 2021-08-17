#ifndef UV_H
#define UV_H
#include "setup.h"
#include <SparkFun_VEML6075_Arduino_Library.h>

// Uncomment to enable collecting UVA & UVB information
#define ENABLE_UV

typedef struct {
  float uva;
  float uvb;
  float index;
} uvInfo;

void setupUV();
uvInfo uvLoop();

#endif
