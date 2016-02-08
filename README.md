STM32F0
===================

Here are some functions and custom libraries for beginners. Most of them should work out of the box, but some of them were created for specific hardware, so be sure to check GPIOs and other defines. 
Contact me if there is any problems. All examples were tested with STM32L100-Discovery board. 

1. GPIO: initialize GPIO with single function (e.g. without creating and filling gpio structure).
Example: 
	gpio_pinSetup(GPIOC, GPIO_Pin_9, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL, GPIO_Speed_40MHz);
	gpio_pinSetup_AF(GPIOA, GPIO_Pin_2, GPIO_AF_TIM9, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_2MHz);
Works also for setting interrupt pins.
	gpio_pinSetup(GPIOC, GPIO_Pin_4, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_40MHz);
	gpio_pinSetup_interrupt(GPIOC, GPIO_Pin_4, EXTI_Trigger_Falling, 0,);		

2. MILLIS: Function for initializing systick to create Arduino-like millis function.
Example:
	systick_millis_init();
	millis();
	delay(50);

3. STEPPER: Stepper motor driver for 2 or 4 pin motors. 
Include functions for optimal move to selected position, home position, ...
Examples included. 

4. USART: Arduino-like serial print of data (integers, floats, strings, raw data, ...)
Example:
	UART_Init();	//Supports character RX interrupt mode.
Print functions:
	printStringLn("String with new line");
	printLn();	// only new line
	printNumberLn(1234567890, DEC); // BIN, HEX
	printFloatLn(123.456);
	...


