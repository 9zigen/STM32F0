 /*
 ===============================================================================
								##### STM32F0-discovery board: millis #####
																h file
	timer_millis: STM32F0 family
	Using systick
 ===============================================================================
 * @date    5-Jan-2015
 * @author  Domen Jurkovic
 
 * initialize library with: systick_millis_init();
 * 10us "reliable" resolution. 
 
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SYSTICK_MILLIS_H
#define __SYSTICK_MILLIS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include <stm32f0xx_rcc.h>

#define INCREMENT_RESOLUTION	1000	//in microseconds [us]. Timer increments every INCREMENT_RESOLUTION us

// "private" function. systick initialization.
void systick_millis_init(void);

// Return milliseconds from timer initialization or timer reset
uint32_t millis(void);

// Wait in this function for specific amount of time.
// param: uint32_t [milliseconds]
void delay(uint32_t ms);

// Restart millis to 0. Doesn't STOP timer. 
void restartMillis(void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTICK_MILLIS_H */
