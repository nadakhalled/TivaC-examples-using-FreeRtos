#include "FreeRtosIncludes.h"

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "RtosTasks.h"

/*Function declerations*/
void intializeSystem(void);
void GPIOF_HANDLER(void);

/*Global vars*/
TaskHandle_t task1Handle,task2Handle;
SemaphoreHandle_t xBinarySemaphore;

/*main.c*/
int main(void)
{
    intializeSystem();

    /*creating binary semaphore*/
    xBinarySemaphore=xSemaphoreCreateBinary();

    /*Creating tasks and starting schedule*/
    uint16_t task1CreationPassed=xTaskCreate(sendUARTTask,"task1",1000,NULL,1,&task1Handle);
    uint16_t task2CreationPassed=xTaskCreate(switchHandlingTask,"task2",128,(void *)(xBinarySemaphore),2,&task2Handle);
    vTaskStartScheduler();

    while(1)
    {

    }

	return 0;
}


/* Function that intializes all the necessary peripherals for the system*/
void intializeSystem(void)
{
    /*Intializing system clocks and global interrupts*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    /*intializing leds*/
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    /*Intializing switches*/
    GPIOUnlockPin(GPIO_PORTF_BASE,GPIO_PIN_0);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
    GPIOIntDisable(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);
    GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4,GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);

    /*intializing UART*/
    GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(), 9600,(UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE));
    UARTEnable(UART0_BASE);
}

/*GPIOF handler*/
void GPIOF_HANDLER(void)
{
    uint32_t interruptStatus=GPIOIntStatus(GPIO_PORTF_BASE,true);
    if((interruptStatus & GPIO_INT_PIN_0) == GPIO_INT_PIN_0)
    {
        GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_0);
        UART_sendString("Interrupt received\n");
        /*Defering the interrupt handling to handler task*/
        BaseType_t xHigherPriorityTaskWoken=pdFALSE;
        xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}
