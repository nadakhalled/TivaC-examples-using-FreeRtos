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

void intializeSystem(void);

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
    TaskStatus_t Details1,Details2; // 2 structs for task1 and 2
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t number,x;
    unsigned long ulTotalRunTime;
    number =uxTaskGetNumberOfTasks();
    pxTaskStatusArray = pvPortMalloc( number * sizeof( TaskStatus_t ) );
    uxTaskGetSystemState( pxTaskStatusArray,number,&ulTotalRunTime );
    UART0_SendString("Number of tasks = ");
    UART0_SendInt(number);
    UART0_Println();
    for(x = 0 ; x < number ; x ++ )   //To print tasks names
    {
        UART0_SendString(pxTaskStatusArray[x].pcTaskName);
        UART0_Println();
    }

    while(1)
    {
        vTaskGetInfo(state1,&Details1, pdTRUE,eInvalid );
        vTaskGetInfo(state2,&Details2, pdTRUE,eInvalid );
        UART0_SendString(Details1.pcTaskName);
        UART0_Println();
        UART0_SendInt(Details1.eCurrentState);
        UART0_Println();
        UART0_SendString(Details2.pcTaskName);
        UART0_Println();
        UART0_SendInt(Details2.eCurrentState);
        UART0_Println();
        vTaskDelay(1000); //1 second
    }
}
