// Includes
#include <cstdio>
#include "scheduler.h"
#include "util_assert.h"
#include "debug_printf.h"

// Task includes
#include "main_task.h"
#include "telemetry_receive_task.h"
#include "telemetry_send_task.h"

// Setup timer with microsecond resolution for keeping track of time.
SystemTimer sys_timer;

// Periodic Tasks ->     Task name        Frequency (Hz)
MainTask                 main_task            (500);

// Queued Tasks ->       Task name      Queue Size (elements)
TelemetrySendTask        send_task             (50);

// General Tasks ->      Task name
TelemetryReceiveTask     receive_task; // Runs when data is ready from serial port.

// Create the scheduler to manage when tasks run.
Scheduler::Scheduler scheduler;

//*****************************************************************************
int main(void)
{
    debug_printf("Cart started up.");

    // Add tasks here to register them with the scheduler.
    // A task at the beginning of the array will have a higher priority than one towards the end.
    static Scheduler::Task * tasks[] =
    {
        &main_task,
        &send_task,
        &receive_task,
    };

    const uint32_t number_of_tasks = sizeof(tasks) / sizeof(tasks[0]);

    // Make sure interrupts are enabled in case a task needs them during initialization.
    scheduler.restoreInterrupts(true);

    // Register all tasks.
    for (uint32_t i = 0; i < number_of_tasks; ++i)
    {
        bool registration_success = scheduler.registerTask(*tasks[i]);
        assert_msg(registration_success, ASSERT_STOP, "Failed to register task \"%s\"", tasks[i]->name());
    }

    debug_printf("Everything is setup correctly.");

    // Allow tasks to start running.
    scheduler.scheduleTasks();

    // Should never get here.
    while (true);
    return 0;
}
