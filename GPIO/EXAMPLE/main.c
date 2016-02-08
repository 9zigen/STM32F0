/**
  *	GPIO init test (STM32F030)
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

//onboard leds and buttons
#define D1 GPIO_Pin_9	//GPIOC, PC9 - GREEN
#define D2 GPIO_Pin_8	//GPIOC, P89 - BLUE
#define B1 GPIO_Pin_0	//GPIOA, PA0

void GPIO_Setup( void )
{
	//STM32F030 discovery onboard leds and button
	//LEDS
	gpio_pinSetup(GPIOC, D1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_10MHz);
	gpio_pinSetup(GPIOC, D2, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_10MHz);
		
	//BUTTON 
	gpio_pinSetup(GPIOA, B1, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_50MHz);
	gpio_pinSetup_interrupt(GPIOA, B1, EXTI_Trigger_Rising, 1);
}

int main(void)
{	
	uint8_t button_state = 0;
	
	GPIO_Setup();
	systick_millis_init();
	
	while(1){  
		delay(200);
		gpio_toggleBit(GPIOC, D1);
  }
}

// PA0 (button B1) interrupt handler
void EXTI0_1_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)  {	
		EXTI_ClearFlag(EXTI_Line0);
		gpio_toggleBit(GPIOC, D2);
	}
}


