#ifndef TELEMETRY_RECEIVE_TASK_H_INCLUDED
#define TELEMETRY_RECEIVE_TASK_H_INCLUDED

// Includes
#include "digital_out.h"
#include "glob_types.h"
#include "glo_rx_link.h"
#include "task.h"

// Receive and handle data coming in over serial link.
class TelemetryReceiveTask : public Scheduler::Task
{
public: // methods

    // Constructor
    TelemetryReceiveTask(void);

    // Return true if there is anything in the receive port that needs to be parsed.
    virtual bool needToRun(void);

    // Send back the request glob.  If instance is 0 then sends back all instances.
    void handle(glo_request_t & msg, uint16_t instance);

private: // methods

    // Setup glo receive link.
    virtual void initialize(void);

    // Parse any new data.
    virtual void run(void);

    // Called whenever a new message is successfully parsed from the serial port.
    static void newMessageCallback(uint8_t object_id, uint16_t instance, void * glob_data);

private: // fields

    // Receive link for parsing incoming glob messages.
    GloRxLink * glo_rx_link_;

    // Serial bus wrapped by glo link.
    usart_bus_t bus_;
    Usart * serial_port_;

};

// Task instance - defined in main.cpp
extern TelemetryReceiveTask receive_task;

#endif
