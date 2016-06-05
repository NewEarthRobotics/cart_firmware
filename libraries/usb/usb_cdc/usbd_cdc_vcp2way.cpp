/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2011
  * @brief   Generic media access Layer.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
extern "C"{
#include "usbd_cdc_vcp.h"
#include "usbcdc_interface2way.h" // KLM added
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
}

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment = 4
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

/* Public variables -----------------------------------------------------------*/
volatile uint8_t USBCDCobjID;
volatile uint16_t USBCDCinstance = 0;
volatile uint8_t USBCDCobjPending = 0;
volatile uint8_t USBCDC_Data[255];

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

LINE_CODING linecoding =
  {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
  };


/* These are external variables imported from CDC core to be used for IN
   transfer management. */
extern uint8_t  APP_Rx_Buffer []; /* Write CDC received data in this buffer.
                                     These data will be sent over USB IN endpoint
                                     in the CDC core functions. */
extern uint32_t APP_Rx_ptr_in;    /* Increment this pointer or roll it back to
                                     start address when writing received data
                                     in the buffer APP_Rx_Buffer. */

extern uint32_t APP_Rx_ptr_out;    // DS added
                                   // updated by the CDC core DO NOT CHANGE

/* Private function prototypes -----------------------------------------------*/
static uint16_t VCP_Init     (void);
static uint16_t VCP_DeInit   (void);
static uint16_t VCP_Ctrl     (uint32_t Cmd, uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataTx   (uint8_t* Buf, uint32_t Len);
static uint16_t VCP_DataRx   (uint8_t* Buf, uint32_t Len);

CDC_IF_Prop_TypeDef VCP_fops =
{
  VCP_Init,
  VCP_DeInit,
  VCP_Ctrl,
  VCP_DataTx,
  VCP_DataRx
};

/**************************/
/**************************/
static uint16_t VCP_Init(void)
{
  // nothing needed - removed com port
  return USBD_OK;
}

/**************************/
/**************************/
static uint16_t VCP_DataTx(uint8_t* Buf, uint32_t Len)
{
  // nothing needed - removed com port
  return USBD_OK;
}

/**************************/
/**************************/
static uint16_t VCP_DeInit(void)
{

  return USBD_OK;
}

/**************************/
/**************************/
static uint16_t VCP_Ctrl (uint32_t Cmd, uint8_t* Buf, uint32_t Len)
{
  switch (Cmd)
  {
  case SEND_ENCAPSULATED_COMMAND:
    /* Not  needed for this driver */
    break;

  case GET_ENCAPSULATED_RESPONSE:
    /* Not  needed for this driver */
    break;

  case SET_COMM_FEATURE:
    /* Not  needed for this driver */
    break;

  case GET_COMM_FEATURE:
    /* Not  needed for this driver */
    break;

  case CLEAR_COMM_FEATURE:
    /* Not  needed for this driver */
    break;

  case SET_LINE_CODING:
    /* Not  needed for this driver - removed com port */
    break;

  case GET_LINE_CODING:
    Buf[0] = (uint8_t)(linecoding.bitrate);
    Buf[1] = (uint8_t)(linecoding.bitrate >> 8);
    Buf[2] = (uint8_t)(linecoding.bitrate >> 16);
    Buf[3] = (uint8_t)(linecoding.bitrate >> 24);
    Buf[4] = linecoding.format;
    Buf[5] = linecoding.paritytype;
    Buf[6] = linecoding.datatype;
    break;

  case SET_CONTROL_LINE_STATE:
    /* Not  needed for this driver */
    break;

  case SEND_BREAK:
    /* Not  needed for this driver */
    break;

  default:
    break;
  }

  return USBD_OK;
}


/**
* USBCDC_Send_Data   Author: DS
* Use this function to pass a buffer starting at "data" and of length size
* over the USB CDC interface.  Note: will not return from this function until
* the message is copied to send buffer if use_timeout=0. If use_timout=1 and
* the buffer (2048 bytes) fills, then it will timeout after 1 second.
*/
uint16_t USBCDC_Send_Data(uint8_t* data, uint32_t size, uint8_t use_timeout)
{
  int32_t Rx_length=0;

  Rx_length = (int32_t)APP_Rx_ptr_in - (int32_t)APP_Rx_ptr_out;
  if (Rx_length < 0)
  {
      Rx_length = APP_RX_DATA_SIZE+Rx_length;
  }

  for(uint32_t i=0; i < size; i++)
  {
    while (Rx_length > (APP_RX_DATA_SIZE-2))
    {
      Rx_length = (int32_t)APP_Rx_ptr_in - (int32_t)APP_Rx_ptr_out;
      if (Rx_length < 0)
      {
          Rx_length = APP_RX_DATA_SIZE+Rx_length;
      }
    }

    APP_Rx_Buffer[APP_Rx_ptr_in] = data[i];

    if(APP_Rx_ptr_in == (APP_RX_DATA_SIZE-1))
    {
      APP_Rx_ptr_in = 0;                      // roll circular buffer over
    }
    else
    {
      APP_Rx_ptr_in++;
    }
    Rx_length++;
  }

  return 1;
}

/**************************/
/**************************/
void USBCDC_Config(void)
{
  USBD_Init(&USB_OTG_dev,
            USB_OTG_FS_CORE_ID,
            &USR_desc,
            &USBD_CDC_cb,
            &USR_cb);

  NVIC->IP[OTG_FS_IRQn] = 0x70;    // Put USB (OTG_FS) interrupt in the middle
}

/**************************/
/**************************/
void USBCDC_Disconnect (void)
{
    DCD_DevDisconnect(&USB_OTG_dev);
    USB_OTG_StopDevice(&USB_OTG_dev);
}

/**************************/
/**************************/
static uint16_t VCP_DataRx (uint8_t* Buf, uint32_t Len)
{
  // TODO copy data out of buffer into circular buffer.

  return USBD_OK;
}
