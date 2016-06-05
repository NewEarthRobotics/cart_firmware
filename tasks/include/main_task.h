#ifndef MAIN_TASK_H_INCLUDED
#define MAIN_TASK_H_INCLUDED

// Includes
#include "analog_in.h"
#include "complementary_filter.h"
#include "digital_out.h"
#include "glob_types.h"
#include "mpu6000.h"
#include "periodic_task.h"
#include "user_leds.h"

// Estimate cart orientation using accel and gyro measurements, measure battery voltage from ADC
// and update LEDs.
class MainTask : public Scheduler::PeriodicTask
{
public: // methods

    // Constructor
    MainTask(float frequency);

private: // methods

    // Initialize sensors used by this task.
    virtual void initialize(void);

    // Run at the desired frequency to run the filter, measure battery and update LEDs.
    virtual void run(void);

    // Run filter to produce new roll-pitch-yaw measurement.
    void estimateState(void);

    // Publish data to all globs that this task owns.
    void publishNewData(void);

private: // fields

    // The IMU used for getting gyro and accel measurements.
    MPU6000 mpu_;

    // Filter used to provide state measurements.
    ComplementaryFilter complementary_filter_;

    // Used to read battery voltage and QTR array.
    AnalogIn analog_inputs_;

    // Battery LEDs
    Leds leds_;

    // Globs that this task owns.
    glo_raw_imu_t raw_imu_;
    glo_imu_t imu_;
    glo_quaternion_t quaternion_;
    glo_roll_pitch_yaw_t roll_pitch_yaw_;
    glo_analog_t analog_;

};

// Task instance - defined in main.cpp
extern MainTask main_task;

#endif
