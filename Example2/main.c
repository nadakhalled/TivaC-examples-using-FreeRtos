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
void traceTasks(void *para);
static void prvAutoReloadTimerCallback( TimerHandle_t xTimer );

/*Global vars*/
TaskHandle_t task1Handle,task2Handle,task3Handle,task4Handle;
SemaphoreHandle_t xBinarySemaphore,xCountingSemaphore,xMutexLed,xMutexPrint;
TimerHandle_t timerHandle;

printingTaskParams task1Params,task2Params;
switchHandlingTaskParams task3Params={0,0,GPIO_PIN_3};
switchHandlingTaskParams task4Params={0,0,GPIO_PIN_2};

/*main.c*/
int main(void)
{
    intializeSystem();

    /*creating binary, counting semaphore and necessary mutexes*/
    xBinarySemaphore=xSemaphoreCreateBinary();
    xCountingSemaphore=xSemaphoreCreateCounting(10,0);
    xMutexLed=xSemaphoreCreateMutex();
    xMutexPrint=xSemaphoreCreateMutex();

    /*creating SW timer and starting it*/
    uint32_t timerID;
    timerHandle=xTimerCreate("Timer1",pdMS_TO_TICKS(1000),pdTRUE,&timerID,prvAutoReloadTimerCallback);
    xTimerStart(timerHandle,0);

    /*Creating tasks and starting schedule*/
    /*Filling in the required params for tasks*/
    task1Params.mutexHandle=xMutexPrint;
    task1Params.stringToSend="Hello from task 1! \r\n";
    task2Params.mutexHandle=xMutexPrint;
    task2Params.stringToSend="Hello from task 2! \r\n";

    task3Params.semaphoreHandle=xBinarySemaphore;
    task3Params.mutexHandle=xMutexLed;
    task4Params.semaphoreHandle=xCountingSemaphore;
    task4Params.mutexHandle=xMutexLed;

    uint16_t task1CreationPassed=xTaskCreate(sendUARTTask,"task1",1000,(void*)(&task1Params),1,&task1Handle);
    uint16_t task2CreationPassed=xTaskCreate(sendUARTTask,"task2",1000,(void*)(&task2Params),2,&task2Handle);
    uint16_t task3CreationPassed=xTaskCreate(switchHandlingTask,"task3",1000,(void *)(&task3Params),3,&task3Handle);
    uint16_t task4CreationPassed=xTaskCreate(switchHandlingTask,"task4",1000,(void *)(&task4Params),3,&task4Handle);
    uint16_t task5CreationPassed=xTaskCreate(traceTasks,"debug",128,NULL,3,NULL);
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
    IntEnable(INT_GPIOF);

    /*intializing UART*/
    GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0|GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(), 9600,(UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE));
    UARTEnable(UART0_BASE);
}

/*GPIOF handler*/
void GPIOF_HANDLER(void)
{
    uint32_t interruptStatus=GPIOIntStatus(GPIO_PORTF_BASE,true);
    BaseType_t xHigherPriorityTaskWoken=pdFALSE;
    /*Low frequency interrupt that uses a binary semaphore for synchronization*/
    if((interruptStatus & GPIO_INT_PIN_0) == GPIO_INT_PIN_0)
    {
        GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_0);
        UART_sendString("Interrupt received\r\n");
        /*Defering the interrupt handling to handler task */
        xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );
    }
    /*High frequency interrupt that uses a counting semaphore for synchronization*/
    else if((interruptStatus & GPIO_INT_PIN_4) == GPIO_INT_PIN_4)
    {
        UART_sendString("Interrupt received\r\n");
        GPIOIntClear(GPIO_PORTF_BASE,GPIO_PIN_4);
        BaseType_t xHigherPriorityTaskWoken=pdFALSE;
        xSemaphoreGiveFromISR( xCountingSemaphore, &xHigherPriorityTaskWoken );
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

/* Rtos Task that traces the state of the tasks*/
void traceTasks(void *para)
{
    TaskStatus_t Details1,Details2,Details3;
    while(1)
    {
        vTaskGetInfo(task1Handle,&Details1, pdTRUE,eInvalid );
        vTaskGetInfo(task2Handle,&Details2, pdTRUE,eInvalid );
        vTaskGetInfo(task3Handle,&Details3, pdTRUE,eInvalid );
        UART_sendString("Tasks status: ");
        UARTCharPut(UART0_BASE,(char)(Details1.eCurrentState+'0'));
        UARTCharPut(UART0_BASE,' ');
        UARTCharPut(UART0_BASE,(char)(Details2.eCurrentState+'0'));
        UARTCharPut(UART0_BASE,' ');
        UARTCharPut(UART0_BASE,(char)(Details3.eCurrentState+'0'));
        UART_sendString("\r\n");
        vTaskDelay(1000); //1 second
    }
}

/*Autoreload timer callback function
 * params:
 *  -Timer: the handle of the timer that calls this callback function
 */
static void prvAutoReloadTimerCallback( TimerHandle_t xTimer )
{
    static bool isOn=0;

    if(isOn)
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_PIN_1);
    else
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,0);

    isOn=!isOn;

}
