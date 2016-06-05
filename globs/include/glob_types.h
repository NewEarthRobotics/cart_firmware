// Define types of all globs. Instances are defined in globs.cpp
// To use globs include globs.h.
// To define new objects you must modify this file and globs.h

#ifndef GLOB_TYPES_H_INCLUDED
#define GLOB_TYPES_H_INCLUDED

// Includes
#include <cstdint>
#include "glob_constants.h"

//******************************************************************************
// Raw gyro and accel data that has NOT been calibrated and corrected for robot coordinate frame.
typedef struct
{
     float gyros[3];  // [rad/sec]
     float accels[3]; // [m/sec/sec]

} glo_raw_imu_t;

//******************************************************************************
// Gyro and accel data that has been calibrated and corrected for robot coordinate frame.
typedef struct
{
     float gyros[3];  // [rad/sec]
     float accels[3]; // [m/sec/sec]

} glo_imu_t;

//******************************************************************************
// Voltages reported by ADC.
typedef struct
{
    float voltages[9];     // [volts]
    float battery_voltage; // [volts]

} glo_analog_t;

//******************************************************************************
// Orientation estimate of robot as roll pitch yaw.
typedef struct
{
     float rpy[3]; // [radians]

} glo_roll_pitch_yaw_t;

//******************************************************************************
// Orientation estimate of robot as a quaternion.
typedef struct
{
     float q[4]; // [dimensionless]

} glo_quaternion_t;

//******************************************************************************
// Outgoing telemetry assert message for user feedback. Automatically sent when assert fails.
typedef struct
{
    uint32_t action; // corresponds to enum in util_assert.h (e.g. continue, restart, stop, etc)
    char text[TELEMETRY_TEXT_SIZE];
    uint32_t valid;  // Non-zero if text is valid.

} glo_assert_message_t;

//******************************************************************************
// Outgoing telemetry debug log/status message.  Separate message to prevent overwriting
// assert message in telemetry queue. Use debug_printf() to send message.
typedef struct
{
    char text[TELEMETRY_TEXT_SIZE];
    uint32_t valid;  // Non-zero if text is valid.

} glo_debug_message_t;

//******************************************************************************
// Request a glob to be sent back.
typedef struct
{
    uint8_t requested_id; // Glob ID that's being requested.

} glo_request_t;

//******************************************************************************
// Data recorded about a task when analyzing the task timing.
// Only published once the timing analysis is complete.
typedef struct
{
    char task_name[32];
    //int32_t task_id;

    uint32_t timer_frequency;

    float recording_duration;
    uint32_t execute_counts;
    uint32_t times_skipped;

    uint32_t delay_ticks_max, delay_ticks_min, delay_ticks_avg;
    uint32_t run_ticks_max, run_ticks_min, run_ticks_avg;
    uint32_t interval_ticks_max, interval_ticks_min, interval_ticks_avg;

} glo_task_timing_t;

//******************************************************************************
// Data that is sent back to user in main task.
typedef struct
{
     float roll;  // [degrees]
     float pitch; // [degrees]
     float yaw;   // [degrees]

     float battery_voltage; // [volts]

} glo_user_data_t;

#endif // GLOB_TYPES_H_INCLUDED
