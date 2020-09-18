#ifndef RTOS__TASKS__H
#define RTOS__TASKS__H

#include <stdint.h>
#include <stdbool.h>

#include "driverlib/uart.h"
#include "driverlib/gpio.h"

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

#include"FreeRtos.h"
#include "semphr.h"
#include"task.h"

/*Struct that defines the parameters passed to switchHandlingTask
 * handle: the handle of the semaphore to take before execution
 * pin: the digital pin to write in PORTF
 * */
typedef struct{
    SemaphoreHandle_t handle;
    uint8_t pin;
} switchHandlingTaskParams;


/*Functions Declerations*/

/* Rtos Task that handles the interrupts in PORTF
params:
    -switchHandlingTaskParams
*/
void switchHandlingTask(void* PvParamater);

/* Rtos Task that sends a strings through the UART and delays for 500ms*/
void sendUARTTask(void* PvParamater);

/* Function that sends a string through the UART module
params:
    -stringToSend: the string to be sent through the UART
*/
void UART_sendString(char* stringToSend);


#endif /*RTOS__TASKS__H*/
