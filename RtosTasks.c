#include <RtosTasks.h>

static char colors[4][7]={"Red!\n","Blue\n","Green\n"};

void receiveUART(void* pvParameter)
{
    QueueHandle_t queue1_handle=(QueueHandle_t ) pvParameter;
    TickType_t xLastWakeTime= xTaskGetTickCount();
    while(1)
    {
        char color=4;
        uint32_t receivedChar=UARTCharGet(UART0_BASE);

        switch(receivedChar)
          {
              case 'R':
              case 'r':
              {
                  color=0;
                  break;
              }
              case 'B':
              case 'b':
              {
                  color=1;
                  break;
              }
              case 'g':
              case 'G':
              {
                  color=2;
                  break;
              }
              default:
                  color=4;
          }
        if(color!=4)
            xQueueSendToBack(queue1_handle,&color,portMAX_DELAY);

        vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 250 ) );
    }
}

void sendColorsUART(void* pvParameter)
{

    QueueHandle_t queue1_handle=(QueueHandle_t )pvParameter;

    while(1)
    {
        uint16_t receivedColor=4;
        xQueueReceive(queue1_handle,&receivedColor,portMAX_DELAY);
        char currentChar=colors[receivedColor][0];
        uint16_t index=0;
        while(currentChar!='\0')
        {
            UARTCharPut(UART0_BASE,currentChar);
            currentChar=colors[receivedColor][++index];
        }

    }

}

void blinkLedTask(void* pvParameter)
{
    while(1)
    {
        blinkLed(GPIO_PIN_2);
    }

}
