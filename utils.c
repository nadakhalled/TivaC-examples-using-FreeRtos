 /******************************************************************************
 *
 * Module: utility functions
 *
 * File Name: utils.c
 *
 * Description: source file for utility functions
 *
 *******************************************************************************/

#include <utils.h>

/*User defined delay function
 * params:
 *  -n:number of milliseconds
 */
void delayMs(int n)
{
    int i,j;
    for(i=0;i<n;i++)
    {
        for(j=0;j<3180;j++);
    }
}

/*Function to blink a led in PORTF
 * parameters:
 *  -led:pin on which led is connected
 */
void blinkLed(uint32_t led)
{
    GPIOPinWrite(GPIO_PORTF_BASE,led,led);
    delayMs(500);
    GPIOPinWrite(GPIO_PORTF_BASE,led,0);
    delayMs(500);
}
