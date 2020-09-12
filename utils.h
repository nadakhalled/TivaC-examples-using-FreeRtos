 /******************************************************************************
 *
 * Module: utility functions
 *
 * File Name: utils.h
 *
 * Description: header file for utility functions
 *
 *******************************************************************************/


#ifndef __DELAY__H
#define __DELAY__H

#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"

/*Function declerations*/
void delayMs(int n);
void blinkLed(uint32_t led);

#endif /*__DELAY__H*/
