 /*
 ===============================================================================
            ##### STM32F0-discovery board: GPIO custom functions#####
																	c file
 ===============================================================================
 * @date    7-Feb-2016
 * @author  Domen Jurkovic
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <stm32f0xx_gpio_init.h>

// Toggle bit
void gpio_toggleBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin) ==  Bit_SET){
		GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_RESET);
	}
	else{
		GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_SET);
	}
	
}

/*
	GPIOx: where x can be (A..H) to select the GPIO peripheral.
	pinNumber: GPIO_Pin_0 ... GPIO_Pin_15, GPIO_Pin_All
	pinMode: GPIO_Mode_IN, GPIO_Mode_OUT
	pinType: GPIO_OType_PP, GPIO_OType_OD - sets the pin type to push / pull  or open drain
	pinPull: GPIO_PuPd_NOPULL, GPIO_PuPd_UP, GPIO_PuPd_DOWN
	pinSpeed: GPIO_Speed_400KHz, GPIO_Speed_2MHz, GPIO_Speed_10MHz, GPIO_Speed_40MHz
	
	gpio_pinSetup(GPIOC, GPIO_Pin_9, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_40MHz);
*/
void gpio_pinSetup(GPIO_TypeDef* GPIOx,	uint32_t pinNumber, GPIOMode_TypeDef pinMode, GPIOOType_TypeDef pinType, GPIOPuPd_TypeDef pinPull, GPIOSpeed_TypeDef pinSpeed){
	GPIO_InitTypeDef GPIO_setup;
	
	if(GPIOx == GPIOA)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	}
	else if(GPIOx == GPIOB)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	}
	else if(GPIOx == GPIOC)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	}
	else if(GPIOx == GPIOD)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
	}
	else if(GPIOx == GPIOE)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	}
	else if(GPIOx == GPIOF)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	}
	
	GPIO_setup.GPIO_Pin		=	pinNumber;
	GPIO_setup.GPIO_Mode 	= pinMode;
	GPIO_setup.GPIO_OType 	= pinType;
	GPIO_setup.GPIO_PuPd 	= pinPull;
	GPIO_setup.GPIO_Speed 	= pinSpeed;
	GPIO_Init(GPIOx, &GPIO_setup);
}

/*
	GPIOx: where x can be (A..F) to select the GPIO peripheral.
	pinNumber: GPIO_Pin_0 ... GPIO_Pin_15, GPIO_Pin_All
  GPIO_AF: stm32f0xx_gpio.h, search: AF 0 selection.
		GPIO_AF_0:	WKUP, EVENTOUT, TIM15, SPI1, TIM17, MCO, SWDAT, SWCLK, TIM14, BOOT, USART1, 
								CEC, IR_OUT, SPI2, TS, TIM3, USART4, CAN, TIM3, USART2, USART3, CRS, TIM16, TIM1
		GPIO_AF_1:	USART2, CEC, TIM3, USART1, IR, EVENTOUT, I2C1, I2C2, TIM15, SPI2, USART3, TS, SPI1
		GPIO_AF_2:	TIM2, TIM1, EVENTOUT, TIM16, TIM17, USB
		GPIO_AF_3:	TS, I2C1, TIM15, EVENTOUT
		GPIO_AF_4:	TIM14, USART4, USART3, CRS, CAN, I2C1
		GPIO_AF_5:	TIM16, TIM17, TIM15, SPI2, I2C2, MCO, I2C1, USB
		GPIO_AF_6: EVENTOUT
		GPIO_AF_7: COMP1 OUT and COMP2 OUT
	pinType: GPIO_OType_PP, GPIO_OType_OD - sets the pin type to push / pull  or open drain
	pinPull: GPIO_PuPd_NOPULL, GPIO_PuPd_UP, GPIO_PuPd_DOWN
	pinSpeed: GPIO_Speed_2MHz, GPIO_Speed_10MHz, GPIO_Speed_50MHz

	gpio_pinSetup_AF(GPIOC, GPIO_PinSource6, GPIO_AF_0, GPIO_OType_PP, GPIO_PuPd_DOWN, GPIO_Speed_2MHz);
*/
void gpio_pinSetup_AF(GPIO_TypeDef* GPIOx, uint32_t pinNumber, uint8_t GPIO_AF, GPIOOType_TypeDef pinType, GPIOPuPd_TypeDef pinPull, GPIOSpeed_TypeDef pinSpeed){
	GPIO_InitTypeDef GPIO_setup;
	uint32_t pinSource = 0;

	if(GPIOx == GPIOA)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	}
	else if(GPIOx == GPIOB)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	}
	else if(GPIOx == GPIOC)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	}
	else if(GPIOx == GPIOD)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
	}
	else if(GPIOx == GPIOE)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
	}
	else if(GPIOx == GPIOF)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	}
	
	switch(pinNumber)
	{
		case GPIO_Pin_0: pinSource = GPIO_PinSource0; break;
		case GPIO_Pin_1: pinSource = GPIO_PinSource1; break;
		case GPIO_Pin_2: pinSource = GPIO_PinSource2; break;
		case GPIO_Pin_3: pinSource = GPIO_PinSource3; break;
		case GPIO_Pin_4: pinSource = GPIO_PinSource4; break;
		case GPIO_Pin_5: pinSource = GPIO_PinSource5; break;
		case GPIO_Pin_6: pinSource = GPIO_PinSource6; break;
		case GPIO_Pin_7: pinSource = GPIO_PinSource7; break;
		case GPIO_Pin_8: pinSource = GPIO_PinSource8; break;
		case GPIO_Pin_9: pinSource = GPIO_PinSource9; break;
		case GPIO_Pin_10: pinSource = GPIO_PinSource10; break;
		case GPIO_Pin_11: pinSource = GPIO_PinSource11; break;
		case GPIO_Pin_12: pinSource = GPIO_PinSource12; break;
		case GPIO_Pin_13: pinSource = GPIO_PinSource13; break;
		case GPIO_Pin_14: pinSource = GPIO_PinSource14; break;
		case GPIO_Pin_15: pinSource = GPIO_PinSource15; break;
		
	}

	GPIO_PinAFConfig(GPIOx, pinSource, GPIO_AF);
	
	GPIO_setup.GPIO_Pin		=	pinNumber;
	GPIO_setup.GPIO_Mode 	= 	GPIO_Mode_AF;
	GPIO_setup.GPIO_OType	=	pinType;
	GPIO_setup.GPIO_PuPd 	=	pinPull;
	GPIO_setup.GPIO_Speed	= pinSpeed;
	GPIO_Init(GPIOx, &GPIO_setup);
}

/*
	GPIO_TypeDef: GPIOA, GPIOB, GPIOC
	pinNumber: GPIO_Pin_0 ... GPIO_Pin_15
	EXTI_Trigger: EXTI_Trigger_Rising, EXTI_Trigger_Falling, EXTI_Trigger_Rising_Falling
	NVIC_IRQChannelPriority: This parameter can be a value between 0 and 3. 


	Note:
		- All pins with same number are connected to line with same number. They are multiplexed to one line.
		- EXTI_Mode = EXTI_Mode_Interrupt

	Example:
	gpio_pinSetup(GPIOC, GPIO_Pin_4, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_40MHz);
	gpio_pinSetup_interrupt(GPIOC, GPIO_Pin_4, EXTI_Trigger_Falling, 0);		
*/
void gpio_pinSetup_interrupt(GPIO_TypeDef* GPIOx, uint32_t pinNumber, EXTITrigger_TypeDef EXTI_Trigger, uint8_t NVIC_IRQChannelPriority){
	EXTI_InitTypeDef EXTI_InitStructure;
	uint8_t EXTI_PortSourceGPIOx;
	uint8_t EXTI_PinSourcex;
	uint32_t EXTI_LineX;
	NVIC_InitTypeDef NVIC_InitStructure;
	IRQn_Type EXTIx_IRQn;
	
	if(GPIOx == GPIOA)
	{
		EXTI_PortSourceGPIOx = EXTI_PortSourceGPIOA;
	}
	else if(GPIOx == GPIOB)
	{
		EXTI_PortSourceGPIOx = EXTI_PortSourceGPIOB;
	}
	else if(GPIOx == GPIOC)
	{
		EXTI_PortSourceGPIOx = EXTI_PortSourceGPIOC;
	}
	
	switch(pinNumber)
	{
		case GPIO_Pin_0: EXTI_PinSourcex = EXTI_PinSource1; EXTI_LineX = EXTI_Line0; break;
		case GPIO_Pin_1: EXTI_PinSourcex = EXTI_PinSource1; EXTI_LineX = EXTI_Line1; break;
		case GPIO_Pin_2: EXTI_PinSourcex = EXTI_PinSource2; EXTI_LineX = EXTI_Line2; break;
		case GPIO_Pin_3: EXTI_PinSourcex = EXTI_PinSource3; EXTI_LineX = EXTI_Line3; break;
		case GPIO_Pin_4: EXTI_PinSourcex = EXTI_PinSource4; EXTI_LineX = EXTI_Line4; break;
		case GPIO_Pin_5: EXTI_PinSourcex = EXTI_PinSource5; EXTI_LineX = EXTI_Line5; break;
		case GPIO_Pin_6: EXTI_PinSourcex = EXTI_PinSource6; EXTI_LineX = EXTI_Line6; break;
		case GPIO_Pin_7: EXTI_PinSourcex = EXTI_PinSource7; EXTI_LineX = EXTI_Line7; break;
		case GPIO_Pin_8: EXTI_PinSourcex = EXTI_PinSource8; EXTI_LineX = EXTI_Line8; break;
		case GPIO_Pin_9: EXTI_PinSourcex = EXTI_PinSource9; EXTI_LineX = EXTI_Line9; break;
		case GPIO_Pin_10: EXTI_PinSourcex = EXTI_PinSource10; EXTI_LineX = EXTI_Line10; break;
		case GPIO_Pin_11: EXTI_PinSourcex = EXTI_PinSource11; EXTI_LineX = EXTI_Line11; break;
		case GPIO_Pin_12: EXTI_PinSourcex = EXTI_PinSource12; EXTI_LineX = EXTI_Line12; break;
		case GPIO_Pin_13: EXTI_PinSourcex = EXTI_PinSource13; EXTI_LineX = EXTI_Line13; break;
		case GPIO_Pin_14: EXTI_PinSourcex = EXTI_PinSource14; EXTI_LineX = EXTI_Line14; break;
		case GPIO_Pin_15: EXTI_PinSourcex = EXTI_PinSource15; EXTI_LineX = EXTI_Line15; break;
	}
			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);		//SYSCFG APB clock must be enabled to get write access to SYSCFG_EXTICRx  registers
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOx, EXTI_PinSourcex);
	
	EXTI_InitStructure.EXTI_Line 		= EXTI_LineX;
	EXTI_InitStructure.EXTI_Mode 		= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
	
	// check for interrupt routine
	switch(EXTI_LineX)
	{
		case EXTI_Line0: case EXTI_Line1: EXTIx_IRQn = EXTI0_1_IRQn; break;
		case EXTI_Line2: case EXTI_Line3: EXTIx_IRQn = EXTI2_3_IRQn; break;
		case EXTI_Line4: case EXTI_Line5: case EXTI_Line6: case EXTI_Line7: case EXTI_Line8: case EXTI_Line9: case EXTI_Line10: case EXTI_Line11: case EXTI_Line12: case EXTI_Line13: case EXTI_Line14: case EXTI_Line15: EXTIx_IRQn = EXTI4_15_IRQn; break;
	}
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = NVIC_IRQChannelPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*	EXAMPLE INTERRUPT HANDLER
	ISR: 	EXTI0_1_IRQn  EXTI Line 0 and 1 Interrupts                           
				EXTI2_3_IRQn  EXTI Line 2 and 3 Interrupts                            
				EXTI4_15_IRQn	EXTI Line 4 to 15 Interrupts 
Note: for correct EXTIX_IRQ interrupt handler check switch state 

void EXTI0_1_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)  {	
		EXTI_ClearFlag(EXTI_Line0);
		gpio_toggleBit(GPIOC, D2);
	}
}*/

