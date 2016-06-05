#ifndef USER_LEDS_H_INCLUDED
#define USER_LEDS_H_INCLUDED

// Includes
#include "stm32f4xx.h"

typedef uint32_t led_id_t;
enum
{
    // Error LEDs
    LED_E0,
    LED_E1,
    LED_E2,
    LED_E3,
    LED_E4,

    // Power LEDS
    LED_P0,
    LED_P1,
    LED_P2,
    LED_P3,

    // Total number of LEDs
    NUM_LEDS,

};

class Leds
{
  public: // methods

    // Constructor
    Leds(void);

    // Control the specified LED
    void set(led_id_t led);
    void clear(led_id_t led);
    void toggle(led_id_t led);
};

#endif  //USER_LEDS_H
