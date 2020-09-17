#include <stdint.h>
#include <stdbool.h>

#include "driverlib/uart.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "utils.h"

#include"FreeRtos.h"
#include"task.h"
#include "queue.h"

/*Functions Declerations*/

/* Rtos Task that receives a char [R-G-B] from UART and sends data to both queue and mail box
params:
    -Array of Queues: contains the handles of both queues
*/
void receiveUART(void* pvParameter);


/* Rtos Task that sends to UART [Red-Green-Blue] according to the charachter in the queue
params:
    -Queue handle: the queue's handle
*/
void sendColorsUART(void* pvParameter);


/* Rtos Task that changes the currently lighted led
params:
    -Mailbox handle:the handle of the mailbox that task reads from
*/
void changeLedColor(void* pvParameter);

