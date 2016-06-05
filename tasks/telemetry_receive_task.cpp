// Includes
#include "telemetry_receive_task.h"
#include "globs.h"
#include "math_util.h"
#include "cart_settings.h"
#include "telemetry_send_task.h"
#include "util_assert.h"

//******************************************************************************
TelemetryReceiveTask::TelemetryReceiveTask(void) :
      Task("Receive", TASK_ID_TELEM_RECEIVE),
      glo_rx_link_(NULL),
      bus_(USART_BUS_2),
      serial_port_(NULL)
{
}

//******************************************************************************
void TelemetryReceiveTask::initialize(void)
{
    // Setup receive link.
    // Make sure interrupts are disabled for initialization because the
    // first dma_rx error occurs before the fields the dma_rx are populated.
    bool enabled = scheduler.disableInterrupts();

    serial_port_ = Usart::instance(bus_);
    assert(serial_port_ != NULL, ASSERT_STOP);

    scheduler.restoreInterrupts(enabled);

    glo_rx_link_ = new GloRxLink(serial_port_, newMessageCallback);
    assert(glo_rx_link_ != NULL, ASSERT_STOP);
}

//*****************************************************************************
bool TelemetryReceiveTask::needToRun(void)
{
    return glo_rx_link_->dataReady();
}

//******************************************************************************
void TelemetryReceiveTask::run(void)
{
    // Parse any received data to try to form a complete message.
    // If a message is received then it is immediately handled in new message callback.
    // This will only parse one byte to make sure task returns quickly.
    glo_rx_link_->parse();
}

//******************************************************************************
void TelemetryReceiveTask::newMessageCallback(uint8_t object_id, uint16_t instance, void * glob_data)
{
    // Received a new glob over the serial port so need to decide how to handle it.
    switch (object_id)
    {
        case GLO_ID_REQUEST:
            receive_task.handle(*((glo_request_t *)glob_data), instance);
            break;
        default:
            assert_always_msg(ASSERT_CONTINUE, "Received unhandled glob with id: %d", object_id);
            break;
    }
}

//******************************************************************************
void TelemetryReceiveTask::handle(glo_request_t & msg, uint16_t instance)
{
    if ((instance == 0) && (msg.requested_id < NUM_GLOBS))
    {
        if (msg.requested_id == GLO_ID_ASSERT_MESSAGE)
        {
            send_task.send_cached_assert_messages();
        }
        else if (msg.requested_id == GLO_ID_DEBUG_MESSAGE)
        {
            send_task.send_cached_debug_messages();
        }
        else
        {
            // Send back all instances
            send_task.send(msg.requested_id, 1, globs[msg.requested_id]->get_num_instances());
        }
    }
    else
    {
        // Just send back the one instance that was requested.
        send_task.send(msg.requested_id, instance);
    }
}
