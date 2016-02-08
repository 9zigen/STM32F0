 /*
 ===============================================================================
								##### STM32F0-discovery board: millis #####
																c file
	timer_millis: STM32F0 family
	Using systick
 ===============================================================================
 * @date    5-Jan-2015
 * @author  Domen Jurkovic
 
 * initialize library with: systick_millis_init();
 * 10us "reliable" resolution. 
 
 */

/* Includes ------------------------------------------------------------------*/
#include <systick_millis.h>

volatile uint32_t systick_millis = 0;

/*
	Timer 2 initialization.
*/
void systick_millis_init()
{
	RCC_ClocksTypeDef system_clocks;
	uint32_t system_freq;
	uint32_t timer_prescaler;
			
	RCC_GetClocksFreq(&system_clocks);	// get system clocks
	system_freq = system_clocks.SYSCLK_Frequency;	// system clock
	timer_prescaler = INCREMENT_RESOLUTION * (system_freq / 1000000);	//example: Prescaler = 1[us] * 48[MHz] = 48; 
		
	SysTick_Config(timer_prescaler);

}	

void SysTick_Handler(void)  
{
  systick_millis++;
}

// ------------------------------------------------------------------------

// Return milliseconds from timer initialization or timer reset
// This number will overflow (go back to zero), after approximately 49 days.
uint32_t millis()
{
	return systick_millis;
}

// Wait in this function for specific amount of time.
// param: unsigned long [milliseconds]
void delay(uint32_t ms)
{
	uint32_t delay_millis = systick_millis;
	delay_millis += ms;
	while (systick_millis < delay_millis){
			__nop();
		}
}

// Restart systick_millis to 0. Doesn't STOP timer. 
void restartMillis(void)
{
	systick_millis = 0;
	delay(1);
	systick_millis = 0;

}

