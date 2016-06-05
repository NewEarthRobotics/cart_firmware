The interface implemented here is very simplistic.  It includes the following exposed functions and variables
  
  uint16_t USBCDC_Send_Data(uint8_t* data, uint32_t size, uint8_t use_timeout);
    Sends data to the host with the buffer start at "data", and the length = size (bytes).
    If timeout is other than zero it will timeout if cannot transmit a byte for over a second and return a zero.
    Requires that systick interrupt be setup and the following two variables be declared globally
      __IO uint64_t SysTickCounts;
      uint16_t SysTickFrequency;
    Modify this function to remove this timeout dependcy, or change the send data stream.

  void USBCDC_Config(void);
    Use to configure.
  
  void USBCDC_Disconnect (void);
    Use to disconnect.
    
  extern volatile uint8_t USBCDC_Cmd;
    Command byte recieved from the host.  The host is only allowed to send one byte at a time.
    It must be enclosed in the following message frame {'<', '{', '(', USBCDC_Cmd, ')'}
  extern volatile uint8_t USBCDC_Cmd_Pending;
    Indicates a Command byte has been recieved if = 1. Should be cleared by the user.
    To change the recieve stream operation modify the function "VCP_DataRx()" in "usb_cdc_vcp.c"

To use this library include "usbcdc_interface.h" and do the following.
Note the host interface is a virtual com port implemented in the drivers in stm32_vcp.zip.

Include the following STM32_USB_Device_Library library files in the project
  STM32_USB_Device_Library\Class\cdc\src\usb_cdc_core.c
  STM32_USB_Device_Library\Core\src\usbd_core.c
  STM32_USB_Device_Library\Core\src\usbd_ioreq.c
  STM32_USB_Device_Library\Core\src\usbd_req.c

Include the following STM32_USB_OTG_Driver library files in the project  
  STM32_USB_OTG_Driver\src\usb_core.c
  STM32_USB_OTG_Driver\src\usb_dcd.c
  STM32_USB_OTG_Driver\src\usb_dcd_int.c
  
Include all the source files in this directory in the project

The preprocessor search path for include files should include
  \Libraries\CMSIS\ST\STM32F4xx\Include
  \Libraries\CMSIS\Include
  \Libraries\STM32F4xx_StdPeriph_Driver\inc
  \Libraries\STM32_USB_OTG_Driver\inc
  \Libraries\STM32_USB_Device_Library\Class\cdc\inc
  \Libraries\STM32_USB_Device_Library\Core\inc
  This directory
  
The following code needs to be included in stm32f4xx_it.c in appropriate places

      #include "usb_core.h"
      #include "usbd_core.h"
      #include "usbd_cdc_core.h"
      
      extern USB_OTG_CORE_HANDLE           USB_OTG_dev;
      extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
      
      void OTG_FS_WKUP_IRQHandler(void)
      {
        if(USB_OTG_dev.cfg.low_power)
        {
              /* Reset SLEEPDEEP and SLEEPONEXIT bits */
              SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
      
              /* After wake-up from sleep mode, reconfigure the system clock */
              SystemInit();
          USB_OTG_UngateClock(&USB_OTG_dev);
        }
        EXTI_ClearITPendingBit(EXTI_Line18);
      }
      
      void OTG_FS_WKUP_IRQHandler(void)
      {
        if(USB_OTG_dev.cfg.low_power)
        {
              /* Reset SLEEPDEEP and SLEEPONEXIT bits */
              SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
      
              /* After wake-up from sleep mode, reconfigure the system clock */
              SystemInit();
          USB_OTG_UngateClock(&USB_OTG_dev);
        }
        EXTI_ClearITPendingBit(EXTI_Line18);
      }
      
      void OTG_FS_IRQHandler(void)
      {
        USBD_OTG_ISR_Handler (&USB_OTG_dev);
      }
