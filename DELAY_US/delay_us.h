 /*
 ===============================================================================
							Microsecond delay
 ===============================================================================
 * @date    31-Jan-2016
 * @author  Domen Jurkovic
 
 * initialize library with: delay_us_init();
 * for 8MHz or more	core frequency clock
 
 * This library doesn't generate precise us delay, so be careful - take a few us more/less
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_US_H
#define __DELAY_US_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"


void delay_us_init(void);
	 
void delay_us(uint32_t micros);

#ifdef __cplusplus
}
#endif

#endif /* __DELAY_US_H */
