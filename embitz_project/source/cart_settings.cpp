
// Includes
#include "physical_constants.h"
#include "cart_settings.h"

// Current firmware version
const int32_t FIRMWARE_VERSION = 1;

// Scales and offsets for accelerometer.
const float ACCEL_SCALES[3] = { 1.0f, 1.0f, 1.0f };
const float ACCEL_OFFSETS[3] = { 0.0f, 0.0f, 0.0f };

// Scaling and offset for battery voltage
const float BATTERY_SCALE = 4.286f;
const float BATTERY_OFFSET = 0.343f;
