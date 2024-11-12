// include the .h file
#include "calibrate.h"

// define the constructor (for easier referencing in the .ino script)
calibrate::calibrate() : baseline(0), counter(0), sum(0) {
}

// gather the baseline value
void calibrate::gather(float value) {
  counter = counter + 1;
  sum = sum + value;
  baseline = sum / counter;

  }

float calibrate::getBaseline() {
  return baseline;
}