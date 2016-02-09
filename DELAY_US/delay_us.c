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

/* Includes ------------------------------------------------------------------*/
#include "delay_us.h"

uint32_t us_multiplier;

// init delay 
void delay_us_init(void) {
    RCC_ClocksTypeDef RCC_Clocks;
       
    RCC_GetClocksFreq(&RCC_Clocks); // Get system clocks 
    us_multiplier = RCC_Clocks.SYSCLK_Frequency / 1000000; //For 1 us delay, we need to divide with 1M */
}

// delay function: micros >= 1;   
void delay_us(uint32_t micros){
	micros *= us_multiplier;
	
	//substitute 8 cycles for each call of asm code below == //micros /= 8; 
	__asm(" mov r0, micros \n"
	"loop: subs r0, #8 \n"
	" bhi loop \n");
}
