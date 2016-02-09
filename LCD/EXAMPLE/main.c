/**
  *	LCD test (STM32F051)
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

//onboard leds and buttons
#define D1 GPIO_Pin_9	//GPIOC, PC9 - GREEN
#define D2 GPIO_Pin_8	//GPIOC, P89 - BLUE
#define B1 GPIO_Pin_0	//GPIOA, PA0

int main(void)
{
	//small Damogran Labs logo
	//xxx - doesn't care;
	uint8_t custom_char[] = {
        0x00,	   // xxx 00000
        0x0F,
        0x13,
        0x11,
        0x11,
        0x0E,
        0x00,
        0x00 
    };
			
	systick_millis_init();	
		
	//LEDS
	gpio_pinSetup(GPIOC, D1, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_10MHz);
	GPIO_WriteBit(GPIOC, D1, Bit_SET);
	
	//Initialize LCD
	LCD_Init(2, 20);
	
	//Put string to LCD
	LCD_PrintString(0, 0, "DamogranLabs STM32F0");
	
	LCD_PrintString(1, 0, "F:");
	LCD_PrintFloat(1, 2, 12.0753);
	
	LCD_PrintString(1, 12, "N:");
	LCD_PrintFloat(1, 14, 123456);
	
	LCD_CreateChar(0, custom_char);
	LCD_PutCustom(1,10, 0);
	
	while (1)
  {
		gpio_toggleBit(GPIOC, D2);
		delay(1000);
  }
}


