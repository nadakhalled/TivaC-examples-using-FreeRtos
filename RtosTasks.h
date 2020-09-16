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
void receiveUART(void* pvParameter);
void sendColorsUART(void* pvParameter);
void blinkLedTask(void* pvParameter);
