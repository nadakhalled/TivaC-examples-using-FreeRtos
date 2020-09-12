#include "FreeRtosIncludes.h"
#include <stdint.h>
#include <stdbool.h>
#include <Task1.h>

#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"


void intializeSystem(void);
/**
 * main.c
 */
int main(void)
{
    intializeSystem();

    uint32_t task1handle;
    uint16_t task1CreationPassed=xTaskCreate(sendColorsUART,"task1",1000,NULL,1,&task1handle);
    uint16_t task2CreationPassed=xTaskCreate(blinkLedTask,"task2",128,NULL,1,NULL);
    vTaskStartScheduler();

    while(1)
    {

    }

	return 0;
}

void intializeSystem(void)
   {
       //SysCtlClockSet(SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ|SYSCTL_SYSDIV_2_5);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
       SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

       GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
       GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1);

       UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(), 9600,(UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE));
       UARTEnable(UART0_BASE);
   }
