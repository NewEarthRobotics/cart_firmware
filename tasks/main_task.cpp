// Includes
#include "main_task.h"
#include "coordinate_conversions.h"
#include "globs.h"
#include "cart_settings.h"
#include "util_assert.h"

//******************************************************************************
MainTask::MainTask(float frequency) :
        PeriodicTask("Main Task", TASK_ID_MAIN, frequency)
{
}

//******************************************************************************
void MainTask::initialize(void)
{
    uint32_t mpu_fail_count = 0;
    while (mpu_.initialize() != 0)
    {
        // Sensor failed to initialize properly.  Wait then try again.
        sys_timer.busyWait(0.5);
        if (++mpu_fail_count == 3)
        {
            assert_always_msg(ASSERT_CONTINUE, "MPU6000 failed to initialize.");
        }
    }
}

//******************************************************************************
void MainTask::publishNewData(void)
{
    glo_imu.publish(&imu_);
    glo_raw_imu.publish(&raw_imu_);
    glo_quaternion.publish(&quaternion_);
    glo_roll_pitch_yaw.publish(&roll_pitch_yaw_);
    glo_analog.publish(&analog_);
}

//******************************************************************************
void MainTask::run(void)
{
    mpu_.readAccel(raw_imu_.accels);
    mpu_.readGyro(raw_imu_.gyros);

    // Run the filter now that we have new measurements.
    estimateState();

    // Copy most recent ADC and estimate battery voltage.
    analog_inputs_.getVoltages(analog_.voltages);
    analog_.battery_voltage = BATTERY_SCALE * analog_.voltages[8] + BATTERY_OFFSET;

    if (analog_.battery_voltage > 11.5)
    {
        leds_.set(LED_P0);
        leds_.clear(LED_P1);
        leds_.clear(LED_P2);
        leds_.clear(LED_P3);
    }
    else if (analog_.battery_voltage > 11)
    {
        leds_.clear(LED_P0);
        leds_.set(LED_P1);
        leds_.clear(LED_P2);
        leds_.clear(LED_P3);
    }
    else // low battery
    {
        leds_.clear(LED_P0);
        leds_.clear(LED_P1);
        leds_.set(LED_P2);
        leds_.clear(LED_P3);
    }

    if (throttleHz(10))
    {
        glo_user_data_t user_data;

        user_data.roll = roll_pitch_yaw_.rpy[0] * RAD2DEG;
        user_data.pitch = roll_pitch_yaw_.rpy[1] * RAD2DEG;
        user_data.yaw = roll_pitch_yaw_.rpy[2] * RAD2DEG;
        user_data.battery_voltage = analog_.battery_voltage;

        glo_user_data.publish(&user_data);

        send_task.send(GLO_ID_USER_DATA);
    }

    publishNewData();
}

//******************************************************************************
void MainTask::estimateState(void)
{
    // Switch axes and apply calibration parameters.
    imu_.accels[0] = (raw_imu_.accels[0]*ACCEL_SCALES[0] + ACCEL_OFFSETS[0]);
    imu_.accels[1] = (raw_imu_.accels[1]*ACCEL_SCALES[1] + ACCEL_OFFSETS[1]);
    imu_.accels[2] = (raw_imu_.accels[2]*ACCEL_SCALES[2] + ACCEL_OFFSETS[2]);
    imu_.gyros[0] = raw_imu_.gyros[0];
    imu_.gyros[1] = raw_imu_.gyros[1];
    imu_.gyros[2] = raw_imu_.gyros[2];

    // Run the complementary filter to calculate roll pitch yaw
    complementary_filter_.update(delta_t_, imu_.gyros, imu_.accels);
    complementary_filter_.getAttitude(quaternion_.q);
    quaternion_2_rpy(quaternion_.q, roll_pitch_yaw_.rpy);
}
