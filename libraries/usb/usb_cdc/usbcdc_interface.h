/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __USBCDC_INTERFACE_H
#define __USBCDC_INTERFACE_H

#include <stdint.h>

extern volatile uint8_t USBCDC_Cmd;
extern volatile uint8_t USBCDC_Cmd_Pending;

#ifdef __cplusplus
  extern "C" {
    uint16_t USBCDC_Send_Data(uint8_t* data, uint32_t size, uint8_t use_timeout);
    void USBCDC_Config(void);
    void USBCDC_Disconnect (void);
  }
#else
  uint16_t USBCDC_Send_Data(uint8_t* data, uint32_t size, uint8_t use_timeout);
  void USBCDC_Config(void);
  void USBCDC_Disconnect (void);
#endif

#endif /* __USBCDC_INTERFACE_H */