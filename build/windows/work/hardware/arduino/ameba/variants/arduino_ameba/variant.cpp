/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "arduino.h"
#include "variant.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl8195a.h"
#include "hal_irqn.h"
#include "hal_diag.h"
#include "analogin_api.h"
#include "us_ticker_api.h"
#include "rt_os_service.h"

analogin_t   adc1;
analogin_t   adc2;
analogin_t   adc3;

#ifdef __cplusplus
  }
#endif

#include "wire.h"
#include "WiFi1.h"


#ifdef __cplusplus
extern "C" {
#endif


void __libc_init_array(void);

/*
 * Pins descriptions
 */
PinDescription g_APinDescription[]=
{
  
  {PA_6, NOT_INITIAL, NOT_INITIAL}, //D0
  {PA_7, NOT_INITIAL, NOT_INITIAL}, //D1
  {PA_5, NOT_INITIAL, NOT_INITIAL}, //D2
  {PD_4, NOT_INITIAL, NOT_INITIAL}, //D3 : PWM1
  {PD_5, NOT_INITIAL, NOT_INITIAL}, //D4 : PWM2
  {PA_4, NOT_INITIAL, NOT_INITIAL}, //D5
  {PA_3, NOT_INITIAL, NOT_INITIAL}, //D6
  {PA_2, NOT_INITIAL, NOT_INITIAL}, //D7
  {PB_4, NOT_INITIAL, NOT_INITIAL}, //D8 : PWM0
  {PB_5, NOT_INITIAL, NOT_INITIAL}, //D9 : PWM1
  {PC_0, NOT_INITIAL, NOT_INITIAL}, //D10 : PWM0
  {PC_2, NOT_INITIAL, NOT_INITIAL}, //D11 : PWM2
  {PC_3, NOT_INITIAL, NOT_INITIAL}, //D12 : PWM3
  {PC_1, NOT_INITIAL, NOT_INITIAL}  //D13 : PWM1

};

// uart
void UART_Handler(void);


//i2c
i2c_t i2cmaster_wire1;


#ifdef __cplusplus
  }
#endif

// LogUart
RingBuffer rx_buffer1;

UARTClass Serial(UART_LOG_IRQ, &rx_buffer1);
void serialEvent() __attribute__((weak));
void serialEvent() { }

// IT handlers
void UART_Handler(void)
{
  Serial.IrqHandler();
}

static void ReRegisterSerial(void)
{
    IRQ_HANDLE          UartIrqHandle;
    
    //4 Register Log Uart Callback function
    UartIrqHandle.Data = NULL;//(u32)&UartAdapter;
    UartIrqHandle.IrqNum = UART_LOG_IRQ; //UART_LOG_IRQ;
    UartIrqHandle.IrqFun = (IRQ_FUN)(UART_Handler);
    UartIrqHandle.Priority = 0;

    
    //4 Register Isr handle
    InterruptUnRegister(&UartIrqHandle); 
    InterruptRegister(&UartIrqHandle); 
}

void init_hal_uart(void)
{
	ReRegisterSerial();
}

//I2C

TwoWire Wire1;

void init_hal_i2c(void)
{
	Wire1.setpin(PD_7, PD_6);
	Wire1.setI2CMaster(&i2cmaster_wire1);
}

// adc

void init_hal_adc(void)
{
    analogin_init(&adc1, AD_1);
    analogin_init(&adc2, AD_2);
    analogin_init(&adc3, AD_3);
		
}


// DAC
DACClass1 DAC0;

void init_hal_dac(void)
{
    DAC0.begin(0);
}

// WiFi

void init_hal_wifi(void)
{
	WiFi1.init();
}
// ----------------------------------------------------------------------------

void serialEventRun(void)
{

  if (Serial.available()) serialEvent();
//  if (Serial1.available()) serialEvent1();
//  if (Serial2.available()) serialEvent2();
//  if (Serial3.available()) serialEvent3();

}


void init( void )
{

	// Initialize C library
	__libc_init_array();
	//rtl_libc_init();

	init_hal_uart();
	init_hal_i2c();
	//init_hal_adc();
	//init_hal_dac();
	init_hal_wifi();
	//_ticker_init();


	ConfigDebugInfo&= (~(_DBG_GPIO_));
	ConfigDebugErr&= (~(_DBG_MISC_));


}





