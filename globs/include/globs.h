// This is the only header file you should include to use globs.
// To define new objects you must modify this file and glob_types.h

#ifndef GLOBS_H_INCLUDED
#define GLOBS_H_INCLUDED

// Includes
#include <cstdint>
#include "glob_template.h"
#include "glob_types.h"

// Unique IDs given to globs are defined in this enumeration.
// Add an ID name to the enum for each new object.
// The bottom member of the enum should always be NUM_GLOBS which
// keeps track of the total number of objects (max number = 255)
typedef uint8_t glob_id_t;
enum
{
    GLO_ID_ASSERT_MESSAGE,
    GLO_ID_DEBUG_MESSAGE,
    GLO_ID_RAW_IMU,
    GLO_ID_ANALOG,
    GLO_ID_IMU,
    GLO_ID_ROLL_PITCH_YAW,
    GLO_ID_QUATERNION,
    GLO_ID_REQUEST,
    GLO_ID_TASK_TIMING,
    GLO_ID_USER_DATA,

    NUM_GLOBS,
};

// Macro used to allow globs.cpp to define the objects and avoid duplicate maintenance.
// In globs.cpp DEFINE_GLOBS forces the macro to define the objects.
// Elswhere it only declares the objects.
#ifndef DEFINE_GLOBS
#define GLOB(var_name, struct_type, id, num_instances, owner_task) \
    extern GlobTemplate<struct_type, num_instances, owner_task> var_name
#else
#define GLOB(var_name, struct_type, id, num_instances, owner_task) \
    GlobTemplate<struct_type, num_instances, owner_task> var_name(id)
#endif

// Forward declare task types for glob ownership.
class MainTask;
class TelemetryReceiveTask;
class TelemetrySendTask;

// Macro use:
// Argument 1: Glob variable name
// Argument 2: Name of struct (defined in glob_types.h)
// Argument 3: ID (must be unique, see enumeration above in CONSTANTS section above)
// Argument 4: How many instances of struct to hold (usually just one)
// Argument 5: The owner task allowed to publish the object
GLOB(glo_assert_message,       glo_assert_message_t,      GLO_ID_ASSERT_MESSAGE,       10,   TelemetrySendTask);
GLOB(glo_debug_message,        glo_debug_message_t,       GLO_ID_DEBUG_MESSAGE,        10,   TelemetrySendTask);
GLOB(glo_raw_imu,              glo_raw_imu_t,             GLO_ID_RAW_IMU,              1,    MainTask);
GLOB(glo_analog,               glo_analog_t,              GLO_ID_ANALOG,               1,    MainTask);
GLOB(glo_imu,                  glo_imu_t,                 GLO_ID_IMU,                  1,    MainTask);
GLOB(glo_roll_pitch_yaw,       glo_roll_pitch_yaw_t,      GLO_ID_ROLL_PITCH_YAW,       1,    MainTask);
GLOB(glo_quaternion,           glo_quaternion_t,          GLO_ID_QUATERNION,           1,    MainTask);
GLOB(glo_request,              glo_request_t,             GLO_ID_REQUEST,              1,    TelemetryReceiveTask);
GLOB(glo_task_timing,          glo_task_timing_t,         GLO_ID_TASK_TIMING,          1,    TelemetrySendTask);
GLOB(glo_user_data,            glo_user_data_t,           GLO_ID_USER_DATA,            1,    MainTask);

#endif // GLOBS_H_INCLUDED
