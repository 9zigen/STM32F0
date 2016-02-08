/**
  *	UART test (STM32F030)
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

}

int main(void)
{	
	RCC_ClocksTypeDef RCC_Clocks;
	
	GPIO_Setup();
	systick_millis_init();
	UART_Init();	// PA9, PA10
	
	printLn();
	printStringLn("------------------------------------------------");
	printStringLn("System initialized.");
		
	RCC_GetClocksFreq(&RCC_Clocks);
	printString("SYSCLK_Frequency: ");
	printNumberLn(RCC_Clocks.SYSCLK_Frequency, DEC); 
	printString("Is your clock: ");
	printFloat(48.0001);
	printStringLn("MHz???");
	
	
	while(1){  
		gpio_toggleBit(GPIOC, D1);
		printNumberLn(millis()/1000, DEC); 
		delay(500);
  }
}

void USART1_IRQHandler(){
	while(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){			// Received characters added to fifo
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		
		// Receive the character
		if(USART_ReceiveData(USART1) == 'x'){			
			printStringLn("uart in interrupt");    
		} 
	}
}


