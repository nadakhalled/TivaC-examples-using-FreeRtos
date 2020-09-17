#include <RtosTasks.h>
#include "FreeRtosIncludes.h"
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

#define QUEUE_MAX_LENGTH 10

/*Function declerations*/
void intializeSystem(void);
void TaskState(void *para);

/*Global Variables*/
uint32_t task1handle,task2handle,task3handle;


/*Main Application */
int main(void)
{
    intializeSystem();

    /*Creating queue*/
    QueueHandle_t queue1_handle=xQueueCreate(QUEUE_MAX_LENGTH,sizeof(uint8_t));

    /*Creating Tasks*/
    uint16_t task1CreationPassed=xTaskCreate(sendColorsUART,"task1",1000,(void *)(queue1_handle),2,&task1handle);
    uint16_t task2CreationPassed=xTaskCreate(receiveUART,"task2",128,(void *)(queue1_handle),2,&task2handle);
    uint16_t task3CreationPassed=xTaskCreate(blinkLedTask,"task3",128,NULL,1,&task3handle);
    uint16_t task4CreationPassed=xTaskCreate(TaskState,"debug",128,NULL,3,NULL);
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

void TaskState(void *para)
{
    TaskStatus_t Details1,Details2,Details3;
    while(1)
    {
        vTaskGetInfo(task1handle,&Details1, pdTRUE,eInvalid );
        vTaskGetInfo(task2handle,&Details2, pdTRUE,eInvalid );
        vTaskGetInfo(task3handle,&Details3, pdTRUE,eInvalid );
        //UART0_SendString(Details1.pcTaskName);
        //UART0_Println();
        UARTCharPut(UART0_BASE,(char)(Details1.eCurrentState+'0'));
        UARTCharPut(UART0_BASE,'\n');
        UARTCharPut(UART0_BASE,(char)(Details2.eCurrentState+'0'));
        UARTCharPut(UART0_BASE,'\n');
        UARTCharPut(UART0_BASE,(char)(Details3.eCurrentState+'0'));
        UARTCharPut(UART0_BASE,'\n');
        vTaskDelay(1000); //1 second
    }
}
