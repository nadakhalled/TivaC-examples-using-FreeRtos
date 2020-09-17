#include <RtosTasks.h>

static char colors[4][7]={"Red!\n","Blue\n","Green\n"};

void receiveUART(void* pvParameter)
{
    QueueHandle_t * queuesArray=(QueueHandle_t *)(pvParameter);
    QueueHandle_t queue1_handle=queuesArray[0];
    QueueHandle_t mailboxHandle=queuesArray[1];

    TickType_t xLastWakeTime= xTaskGetTickCount();
    while(1)
    {
        char color=4;
        uint8_t pin=0;
        uint32_t receivedChar=UARTCharGetNonBlocking(UART0_BASE);

        switch(receivedChar)
          {
              case 'R':
              case 'r':
              {
                  color=0;
                  pin=GPIO_PIN_1;
                  break;
              }
              case 'B':
              case 'b':
              {
                  color=1;
                  pin=GPIO_PIN_2;
                  break;
              }
              case 'g':
              case 'G':
              {
                  color=2;
                  pin=GPIO_PIN_3;
                  break;
              }
              default:
                  color=4;
          }
        if(color!=4)
        {
            xQueueSendToBack(queue1_handle,&color,portMAX_DELAY);
            xQueueOverwrite(mailboxHandle,&pin);
        }

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

void changeLedColor(void* pvParameter)
{
    QueueHandle_t mailboxHandle=(QueueHandle_t )pvParameter;
    while(1)
    {
        uint8_t pin;
        xQueueReceive(mailboxHandle,&pin,portMAX_DELAY);

        /*Resetting state of leds*/
        GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);
        GPIOPinWrite(GPIO_PORTF_BASE,pin,pin);
    }

}
