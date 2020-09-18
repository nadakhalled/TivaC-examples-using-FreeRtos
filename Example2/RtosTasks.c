#include "RtosTasks.h"

void UART_sendString(char* stringToSend)
{
    uint8_t currentChar=stringToSend[0];
    uint8_t index=0;
    while(currentChar!='\0')
    {
        UARTCharPut(UART0_BASE,currentChar);
        currentChar=stringToSend[++index];
    }
}


void sendUARTTask(void* PvParamater)
{
    while(1)
    {
        UART_sendString("Task1: Hello \n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


void switchHandlingTask(void* PvParamater)
{
    switchHandlingTaskParams* params=(switchHandlingTaskParams *)( PvParamater);
    SemaphoreHandle_t semaphoreHandle=params->handle;

    while(1)
    {
        xSemaphoreTake( semaphoreHandle, portMAX_DELAY );
        UART_sendString("Handling Interrupt\n");
        GPIOPinWrite(GPIO_PORTF_BASE,params->pin,params->pin);
        vTaskDelay(pdMS_TO_TICKS(500));
        GPIOPinWrite(GPIO_PORTF_BASE,params->pin,0);
        vTaskDelay(pdMS_TO_TICKS(500));
        UART_sendString("Interrupt Handled\n");
    }

}
