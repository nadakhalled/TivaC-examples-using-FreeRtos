#include <Task1.h>

static char colors[4][7]={"Red!\n","Blue\n","Green\n"};
char color=0;

void receiveUART(void* pvParameter)
{
    while(1)
    {
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
          }
    }
}

void sendColorsUART(void* pvParameter)
{
    static char toSend[6]="Hello\n";
    TickType_t xLastWakeTime;

    while(1)
    {
        char currentChar=toSend[0];
        uint16_t index=0;
        while(currentChar!='\0')
        {
            UARTCharPut(UART0_BASE,currentChar);
            currentChar=toSend[++index];
            vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS( 1000 ) );
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
/*
static uint16_t counter=0;

    while(1)
    {
        char currentChar=colors[counter%3][0];
        uint16_t index=0;
        while(currentChar!='\0')
          {
              UARTCharPut(UART0_BASE,currentChar);
              currentChar=colors[color][++index];
          }
        counter++;
    }
 */
