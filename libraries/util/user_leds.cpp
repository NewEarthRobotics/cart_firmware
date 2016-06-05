// Includes
#include "user_leds.h"

//*****************************************************************************
Leds::Leds(void)
{
    // GPIOD Periph clock enable
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    // Configure the GPIO_LED pins
    GPIO_InitTypeDef  GPIO_InitStructure;

    // B Pins
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |
                                  GPIO_Pin_13 |
                                  GPIO_Pin_14 |
                                  GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // D Pins
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  |
                                  GPIO_Pin_9  |
                                  GPIO_Pin_10 |
                                  GPIO_Pin_11 |
                                  GPIO_Pin_12;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    for (uint32_t i = 0; i < NUM_LEDS; ++i)
    {
        clear(i);
    }
}

//*****************************************************************************
void Leds::set(led_id_t led)
{
    switch (led)
    {
        case LED_E0:
          GPIOD->BSRRH = GPIO_Pin_11;
          break;
        case LED_E1:
          GPIOD->BSRRH = GPIO_Pin_12;
          break;
        case LED_E2:
          GPIOB->BSRRH = GPIO_Pin_12;
          break;
        case LED_E3:
          GPIOB->BSRRH = GPIO_Pin_13;
          break;
        case LED_E4:
          GPIOB->BSRRH = GPIO_Pin_14;
          break;
        case LED_P0:
          GPIOD->BSRRH = GPIO_Pin_10;
          break;
        case LED_P1:
          GPIOD->BSRRH = GPIO_Pin_9;
          break;
        case LED_P2:
          GPIOD->BSRRH = GPIO_Pin_8;
          break;
        case LED_P3:
          GPIOB->BSRRH = GPIO_Pin_15;
          break;
    }
}

//*****************************************************************************
void Leds::clear(led_id_t led)
{
    switch (led)
    {
        case LED_E0:
          GPIOD->BSRRL = GPIO_Pin_11;
          break;
        case LED_E1:
          GPIOD->BSRRL = GPIO_Pin_12;
          break;
        case LED_E2:
          GPIOB->BSRRL = GPIO_Pin_12;
          break;
        case LED_E3:
          GPIOB->BSRRL = GPIO_Pin_13;
          break;
        case LED_E4:
          GPIOB->BSRRL = GPIO_Pin_14;
          break;
        case LED_P0:
          GPIOD->BSRRL = GPIO_Pin_10;
          break;
        case LED_P1:
          GPIOD->BSRRL = GPIO_Pin_9;
          break;
        case LED_P2:
          GPIOD->BSRRL = GPIO_Pin_8;
          break;
        case LED_P3:
          GPIOB->BSRRL = GPIO_Pin_15;
          break;
    }
}

//*****************************************************************************
void Leds::toggle(led_id_t led)
{
    switch (led)
    {
        case LED_E0:
          GPIOD->ODR ^= GPIO_Pin_11;
          break;
        case LED_E1:
          GPIOD->ODR ^= GPIO_Pin_12;
          break;
        case LED_E2:
          GPIOB->ODR ^= GPIO_Pin_12;
          break;
        case LED_E3:
          GPIOB->ODR ^= GPIO_Pin_13;
          break;
        case LED_E4:
          GPIOB->ODR ^= GPIO_Pin_14;
          break;
        case LED_P0:
          GPIOD->ODR ^= GPIO_Pin_10;
          break;
        case LED_P1:
          GPIOD->ODR ^= GPIO_Pin_9;
          break;
        case LED_P2:
          GPIOD->ODR ^= GPIO_Pin_8;
          break;
        case LED_P3:
          GPIOB->ODR ^= GPIO_Pin_15;
          break;
    }
}
